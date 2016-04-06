#include "TFile.h"
#include "TH2D.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "OSUT3Analysis/Collections/plugins/OSUTrackProducer.h"

#if IS_VALID(tracks)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUTrackProducer::OSUTrackProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_ (cfg)
{
  collection_ = collections_.getParameter<edm::InputTag> ("tracks");

  produces<vector<osu::Track> > (collection_.instance ());

  token_ = consumes<vector<TYPE(tracks)> > (collection_);
  mcparticleToken_ = consumes<vector<osu::Mcparticle> > (collections_.getParameter<edm::InputTag> ("mcparticles"));
  jetsToken_ = consumes<vector<TYPE(jets)> > (collections_.getParameter<edm::InputTag> ("jets"));

  const edm::ParameterSet &fiducialMaps = cfg.getParameter<edm::ParameterSet> ("fiducialMaps");
  const edm::ParameterSet &electronFiducialMap = fiducialMaps.getParameter<edm::ParameterSet> ("electrons");
  const edm::ParameterSet &muonFiducialMap = fiducialMaps.getParameter<edm::ParameterSet> ("muons");

  extractFiducialMap (electronFiducialMap, electronVetoList_);
  extractFiducialMap (muonFiducialMap, muonVetoList_);

  sort (electronVetoList_.begin (), electronVetoList_.end (), [] (EtaPhi a, EtaPhi b) -> bool { return (a.eta < b.eta && a.phi < b.phi); });
  sort (muonVetoList_.begin (), muonVetoList_.end (), [] (EtaPhi a, EtaPhi b) -> bool { return (a.eta < b.eta && a.phi < b.phi); });

  clog << "================================================================================" << endl;
  clog << "electron veto regions in (eta, phi)" << endl;
  clog << "--------------------------------------------------------------------------------" << endl;
  for (const auto &etaPhi : electronVetoList_)
    clog << "(" << setw (10) << etaPhi.eta << "," << setw (10) << etaPhi.phi << ")" << endl;
  clog << "================================================================================" << endl;

  clog << "================================================================================" << endl;
  clog << "muon veto regions in (eta, phi)" << endl;
  clog << "--------------------------------------------------------------------------------" << endl;
  for (const auto &etaPhi : muonVetoList_)
    clog << "(" << setw (10) << etaPhi.eta << "," << setw (10) << etaPhi.phi << ")" << endl;
  clog << "================================================================================" << endl;
}

OSUTrackProducer::~OSUTrackProducer ()
{
}

void
OSUTrackProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(tracks)> > collection;
  if (!event.getByToken (token_, collection))
    return;
  edm::Handle<vector<osu::Mcparticle> > particles;
  event.getByToken (mcparticleToken_, particles);
  edm::Handle<vector<TYPE(jets)> > jets;
  if (!event.getByToken (jetsToken_, jets)) {
    clog << "ERROR:  Could not find jets collection." << endl;
    return;
  }


  pl_ = auto_ptr<vector<osu::Track> > (new vector<osu::Track> ());
  for (const auto &object : *collection)
    {
      osu::Track track (object, particles, cfg_, electronVetoList_, muonVetoList_);

      double dRMinJet = 999;
      for (const auto &jet : *jets) {
        if (!(jet.pt() > 30))         continue;
        if (!(fabs(jet.eta()) < 4.5)) continue;
        if (!anatools::jetPassesTightLepVeto(jet)) continue;
        double dR = deltaR(track, jet);
        if (dR < dRMinJet) {
          dRMinJet = dR;
        }
      }

      track.set_dRMinJet(dRMinJet);

      pl_->push_back (track);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

void
OSUTrackProducer::extractFiducialMap (const edm::ParameterSet &cfg, EtaPhiList &vetoList) const
{
  const edm::FileInPath &histFile = cfg.getParameter<edm::FileInPath> ("histFile");
  const string &beforeVetoHistName = cfg.getParameter<string> ("beforeVetoHistName");
  const string &afterVetoHistName = cfg.getParameter<string> ("afterVetoHistName");
  const double &thresholdForVeto = cfg.getParameter<double> ("thresholdForVeto");

  TFile *fin = TFile::Open (histFile.fullPath ().c_str ());
  if (!fin)
    {
      clog << "No file named \"" << histFile.fullPath () << "\" found. Skipping..." << endl;
      return;
    }

  TH2D *beforeVetoHist = (TH2D *) fin->Get (beforeVetoHistName.c_str ());
  beforeVetoHist->SetDirectory (0);
  TH2D *afterVetoHist = (TH2D *) fin->Get (afterVetoHistName.c_str ());
  afterVetoHist->SetDirectory (0);
  fin->Close ();

  //////////////////////////////////////////////////////////////////////////////
  // First calculate the mean efficiency and error on the mean efficiency.
  //////////////////////////////////////////////////////////////////////////////
  double a = 0, b = 0, aErr2 = 0, bErr2 = 0, mean, meanErr;
  for (int i = 1; i <= beforeVetoHist->GetXaxis ()->GetNbins (); i++)
    {
      for (int j = 1; j <= beforeVetoHist->GetYaxis ()->GetNbins (); j++)
        {
          if (i == 1 && j == 1)
            vetoList.minDeltaR = hypot (0.5 * beforeVetoHist->GetXaxis ()->GetBinWidth (i), 0.5 * beforeVetoHist->GetYaxis ()->GetBinWidth (j));

          double contentBeforeVeto = beforeVetoHist->GetBinContent (i, j),
                 errorBeforeVeto = beforeVetoHist->GetBinError (i, j);

          if (!contentBeforeVeto) // skip bins that are empty in the before-veto histogram
            continue;

          double contentAfterVeto = afterVetoHist->GetBinContent (i, j),
                 errorAfterVeto = afterVetoHist->GetBinError (i, j);

          a += contentAfterVeto;
          b += contentBeforeVeto;
          aErr2 += errorAfterVeto * errorAfterVeto;
          bErr2 += errorBeforeVeto * errorBeforeVeto;
        }
    }
  mean = a / b;
  meanErr = mean * hypot (sqrt (aErr2) / a, sqrt (bErr2) / b);
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // Then find the bins which are greater than the mean by more than
  // thresholdForVeto sigma. Add the coordinates for these bins to the veto
  // list.
  //////////////////////////////////////////////////////////////////////////////
  afterVetoHist->Divide (beforeVetoHist);
  for (int i = 1; i <= afterVetoHist->GetXaxis ()->GetNbins (); i++)
    {
      for (int j = 1; j <= afterVetoHist->GetYaxis ()->GetNbins (); j++)
        {
          double content = afterVetoHist->GetBinContent (i, j),
                 error = afterVetoHist->GetBinError (i, j),
                 eta = afterVetoHist->GetXaxis ()->GetBinCenter (i),
                 phi = afterVetoHist->GetYaxis ()->GetBinCenter (j);

          if ((content - mean) > thresholdForVeto * hypot (error, meanErr))
            vetoList.push_back (EtaPhi (eta, phi));
        }
    }
  //////////////////////////////////////////////////////////////////////////////
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUTrackProducer);

#endif
