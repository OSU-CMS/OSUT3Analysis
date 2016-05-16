#include "TFile.h"
#include "TH2D.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "OSUT3Analysis/Collections/plugins/OSUTrackProducer.h"

#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"

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
  const vector<edm::ParameterSet> &electronFiducialMaps = fiducialMaps.getParameter<vector<edm::ParameterSet> > ("electrons");
  const vector<edm::ParameterSet> &muonFiducialMaps = fiducialMaps.getParameter<vector<edm::ParameterSet> > ("muons");

  EBRecHitsTag_    =  cfg.getParameter<edm::InputTag>  ("EBRecHits");
  EERecHitsTag_    =  cfg.getParameter<edm::InputTag>  ("EERecHits");
  HBHERecHitsTag_  =  cfg.getParameter<edm::InputTag>  ("HBHERecHits");

  EBRecHitsToken_    =  consumes<EBRecHitCollection>    (EBRecHitsTag_);
  EERecHitsToken_    =  consumes<EERecHitCollection>    (EERecHitsTag_);
  HBHERecHitsToken_  =  consumes<HBHERecHitCollection>  (HBHERecHitsTag_);

  gsfTracksToken_ = consumes<vector<reco::GsfTrack> > (cfg.getParameter<edm::InputTag> ("gsfTracks"));

  bool outputElectronHotSpots = false;
  for (const auto &electronFiducialMap : electronFiducialMaps)
    {
      const bool &outputHotSpots = electronFiducialMap.getParameter<bool> ("outputHotSpots");
      const edm::FileInPath &histFile = electronFiducialMap.getParameter<edm::FileInPath> ("histFile");
      outputElectronHotSpots = outputElectronHotSpots || outputHotSpots;

      outputHotSpots && clog << "================================================================================" << endl;
      outputHotSpots && clog << "calculating electron veto regions in (eta, phi)..." << endl;
      outputHotSpots && clog << "extracting histograms from \"" << histFile.relativePath () << "\"..." << endl;
      outputHotSpots && clog << "--------------------------------------------------------------------------------" << endl;

      extractFiducialMap (electronFiducialMap, electronVetoList_);

      outputHotSpots && clog << "================================================================================" << endl;
    }

  bool outputMuonHotSpots = false;
  for (const auto &muonFiducialMap : muonFiducialMaps)
    {
      const bool &outputHotSpots = muonFiducialMap.getParameter<bool> ("outputHotSpots");
      const edm::FileInPath &histFile = muonFiducialMap.getParameter<edm::FileInPath> ("histFile");
      outputMuonHotSpots = outputMuonHotSpots || outputHotSpots;

      outputHotSpots && clog << "================================================================================" << endl;
      outputHotSpots && clog << "calculating muon veto regions in (eta, phi)..." << endl;
      outputHotSpots && clog << "extracting histograms from \"" << histFile.relativePath () << "\"..." << endl;
      outputHotSpots && clog << "--------------------------------------------------------------------------------" << endl;

      extractFiducialMap (muonFiducialMap, muonVetoList_);

      outputHotSpots && clog << "================================================================================" << endl;
    }

  sort (electronVetoList_.begin (), electronVetoList_.end (), [] (EtaPhi a, EtaPhi b) -> bool { return (a.eta < b.eta && a.phi < b.phi); });
  sort (muonVetoList_.begin (), muonVetoList_.end (), [] (EtaPhi a, EtaPhi b) -> bool { return (a.eta < b.eta && a.phi < b.phi); });

  outputElectronHotSpots && clog << "================================================================================" << endl;
  outputElectronHotSpots && clog << "electron veto regions in (eta, phi)" << endl;
  outputElectronHotSpots && clog << "--------------------------------------------------------------------------------" << endl;
  if (outputElectronHotSpots)
    for (const auto &etaPhi : electronVetoList_)
      clog << "(" << setw (10) << etaPhi.eta << "," << setw (10) << etaPhi.phi << ")" << endl;
  outputElectronHotSpots && clog << "================================================================================" << endl;

  outputMuonHotSpots && clog << "================================================================================" << endl;
  outputMuonHotSpots && clog << "muon veto regions in (eta, phi)" << endl;
  outputMuonHotSpots && clog << "--------------------------------------------------------------------------------" << endl;
  if (outputMuonHotSpots)
  for (const auto &etaPhi : muonVetoList_)
    clog << "(" << setw (10) << etaPhi.eta << "," << setw (10) << etaPhi.phi << ")" << endl;
  outputMuonHotSpots && clog << "================================================================================" << endl;
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
  edm::Handle<EBRecHitCollection> EBRecHits;
  event.getByToken(EBRecHitsToken_, EBRecHits);
  if (!EBRecHits.isValid()) throw cms::Exception("FatalError") << "Unable to find EBRecHitCollection in the event!\n";
  edm::Handle<EERecHitCollection> EERecHits;
  event.getByToken(EERecHitsToken_, EERecHits);
  if (!EERecHits.isValid()) throw cms::Exception("FatalError") << "Unable to find EERecHitCollection in the event!\n";
  edm::Handle<HBHERecHitCollection> HBHERecHits;
  event.getByToken(HBHERecHitsToken_, HBHERecHits);
  if (!HBHERecHits.isValid()) throw cms::Exception("FatalError") << "Unable to find HBHERecHitCollection in the event!\n";

  setup.get<CaloGeometryRecord>().get(caloGeometry_);
  if (!caloGeometry_.isValid())
    throw cms::Exception("FatalError") << "Unable to find CaloGeometryRecord in event!\n";

  edm::Handle<vector<reco::GsfTrack> > gsfTracks;
  event.getByToken (gsfTracksToken_, gsfTracks);

  pl_ = auto_ptr<vector<osu::Track> > (new vector<osu::Track> ());
  for (const auto &object : *collection)
    {
      osu::Track track (object, particles, cfg_, gsfTracks, electronVetoList_, muonVetoList_);

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

#ifdef DISAPP_TRKS
      // Calculate the associated calorimeter energy for the disappearing tracks search.

      double eEM = 0;
      double dR = 0.5;
      for (EBRecHitCollection::const_iterator hit=EBRecHits->begin(); hit!=EBRecHits->end(); hit++) {
        if (insideCone(track, (*hit).detid(), dR)) {
          eEM += (*hit).energy();
          // cout << "       Added EB rec hit with (eta, phi) = "
          //      << getPosition((*hit).detid()).eta() << ", "
          //      << getPosition((*hit).detid()).phi() << endl;
        }
      }
      for (EERecHitCollection::const_iterator hit=EERecHits->begin(); hit!=EERecHits->end(); hit++) {
        if (insideCone(track, (*hit).detid(), dR)) {
          eEM += (*hit).energy();
          // cout << "       Added EE rec hit with (eta, phi) = "
          //      << getPosition((*hit).detid()).eta() << ", "
          //      << getPosition((*hit).detid()).phi() << endl;
        }
      }
      double eHad = 0;
      for (HBHERecHitCollection::const_iterator hit = HBHERecHits->begin(); hit != HBHERecHits->end(); hit++) {
        if (insideCone(track, (*hit).detid(), dR)) {
          eHad += (*hit).energy();
        }
      }

      track.set_caloNewEMDRp5(eEM);
      track.set_caloNewHadDRp5(eHad);
#endif

      pl_->push_back (track);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

bool OSUTrackProducer::insideCone(TYPE(tracks)& candTrack, const DetId& id, const double dR)
{
   GlobalPoint idPosition = getPosition(id);
   if (idPosition.mag()<0.01) return false;
   math::XYZVector idPositionRoot( idPosition.x(), idPosition.y(), idPosition.z() );
   return deltaR(candTrack, idPositionRoot) < dR;
}

GlobalPoint OSUTrackProducer::getPosition( const DetId& id)
{
   if ( ! caloGeometry_.isValid() ||
        ! caloGeometry_->getSubdetectorGeometry(id) ||
        ! caloGeometry_->getSubdetectorGeometry(id)->getGeometry(id) ) {
      throw cms::Exception("FatalError") << "Failed to access geometry for DetId: " << id.rawId();
      return GlobalPoint(0,0,0);
   }
   return caloGeometry_->getSubdetectorGeometry(id)->getGeometry(id)->getPosition();
}



void
OSUTrackProducer::extractFiducialMap (const edm::ParameterSet &cfg, EtaPhiList &vetoList) const
{
  const edm::FileInPath &histFile = cfg.getParameter<edm::FileInPath> ("histFile");
  const string &beforeVetoHistName = cfg.getParameter<string> ("beforeVetoHistName");
  const string &afterVetoHistName = cfg.getParameter<string> ("afterVetoHistName");
  const double &thresholdForVeto = cfg.getParameter<double> ("thresholdForVeto");
  const bool &outputHotSpots = cfg.getParameter<bool> ("outputHotSpots");

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
          double binRadius = hypot (0.5 * beforeVetoHist->GetXaxis ()->GetBinWidth (i), 0.5 * beforeVetoHist->GetYaxis ()->GetBinWidth (j));
          (vetoList.minDeltaR < binRadius) && (vetoList.minDeltaR = binRadius);

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

          outputHotSpots && content && clog << "(" << setw (10) << eta << ", " << setw (10) << phi << "): " << setw (10) << (content - mean) / hypot (error, meanErr) << " sigma above mean of " << setw (10) << mean;
          if ((content - mean) > thresholdForVeto * hypot (error, meanErr))
            {
              vetoList.push_back (EtaPhi (eta, phi));
              outputHotSpots && clog << " * HOT SPOT *";
            }
          outputHotSpots && content && clog << endl;
        }
    }
  //////////////////////////////////////////////////////////////////////////////
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUTrackProducer);

#endif
