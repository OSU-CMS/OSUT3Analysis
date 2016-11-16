#include "TFile.h"
#include "TH2D.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "OSUT3Analysis/Collections/plugins/OSUSecondaryTrackProducer.h"

#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"

#if IS_VALID(secondaryTracks)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUSecondaryTrackProducer::OSUSecondaryTrackProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_ (cfg)
{
  collection_ = collections_.getParameter<edm::InputTag> ("secondaryTracks");

  produces<vector<osu::SecondaryTrack> > (collection_.instance ());

  token_ = consumes<vector<TYPE(secondaryTracks)> > (collection_);
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

#ifdef DISAPP_TRKS
  stringstream ss;
  for (const auto &electronFiducialMap : electronFiducialMaps)
    {
      const edm::FileInPath &histFile = electronFiducialMap.getParameter<edm::FileInPath> ("histFile");

      ss << "================================================================================" << endl;
      ss << "calculating electron veto regions in (eta, phi)..." << endl;
      ss << "extracting histograms from \"" << histFile.relativePath () << "\"..." << endl;
      ss << "--------------------------------------------------------------------------------" << endl;

      extractFiducialMap (electronFiducialMap, electronVetoList_, ss);

      ss << "================================================================================" << endl;
    }

  for (const auto &muonFiducialMap : muonFiducialMaps)
    {
      const edm::FileInPath &histFile = muonFiducialMap.getParameter<edm::FileInPath> ("histFile");

      ss << "================================================================================" << endl;
      ss << "calculating muon veto regions in (eta, phi)..." << endl;
      ss << "extracting histograms from \"" << histFile.relativePath () << "\"..." << endl;
      ss << "--------------------------------------------------------------------------------" << endl;

      extractFiducialMap (muonFiducialMap, muonVetoList_, ss);

      ss << "================================================================================" << endl;
    }

  sort (electronVetoList_.begin (), electronVetoList_.end (), [] (EtaPhi a, EtaPhi b) -> bool { return (a.eta < b.eta && a.phi < b.phi); });
  sort (muonVetoList_.begin (), muonVetoList_.end (), [] (EtaPhi a, EtaPhi b) -> bool { return (a.eta < b.eta && a.phi < b.phi); });

  ss << "================================================================================" << endl;
  ss << "electron veto regions in (eta, phi)" << endl;
  ss << "--------------------------------------------------------------------------------" << endl;
  for (const auto &etaPhi : electronVetoList_)
    ss << "(" << setw (10) << etaPhi.eta << "," << setw (10) << etaPhi.phi << ")" << endl;
  ss << "================================================================================" << endl;

  ss << "================================================================================" << endl;
  ss << "muon veto regions in (eta, phi)" << endl;
  ss << "--------------------------------------------------------------------------------" << endl;
  for (const auto &etaPhi : muonVetoList_)
    ss << "(" << setw (10) << etaPhi.eta << "," << setw (10) << etaPhi.phi << ")" << endl;
  ss << "================================================================================";
  edm::LogInfo ("OSUSecondaryTrackProducer") << ss.str ();
#endif
}

OSUSecondaryTrackProducer::~OSUSecondaryTrackProducer ()
{
}

void
OSUSecondaryTrackProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(secondaryTracks)> > collection;
  if (!event.getByToken (token_, collection))
    {
      edm::LogWarning ("OSUSecondaryTrackProducer") << "SecondaryTrack collection not found. Skipping production of osu::SecondaryTrack collection...";
      return;
    }

#ifdef DISAPP_TRKS
  edm::Handle<vector<TYPE(jets)> > jets;
  if (!event.getByToken (jetsToken_, jets))
    edm::LogWarning ("OSUSecondaryTrackProducer") << "Jet collection not found.";

  edm::Handle<vector<osu::Mcparticle> > particles;
  event.getByToken (mcparticleToken_, particles);

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

#endif

  pl_ = unique_ptr<vector<osu::SecondaryTrack> > (new vector<osu::SecondaryTrack> ());
  for (const auto &object : *collection)
    {
#ifdef DISAPP_TRKS
      pl_->emplace_back (object, particles, cfg_, gsfTracks, electronVetoList_, muonVetoList_);
      osu::SecondaryTrack &secondaryTrack = pl_->back ();
#else
      pl_->emplace_back (object);
#endif

#ifdef DISAPP_TRKS
      // Calculate the associated calorimeter energy for the disappearing tracks search.

      if (jets.isValid ())
        {
          double dRMinJet = 999;
          for (const auto &jet : *jets) {
            if (!(jet.pt() > 30))         continue;
            if (!(fabs(jet.eta()) < 4.5)) continue;
            if (!anatools::jetPassesTightLepVeto(jet)) continue;
            double dR = deltaR(secondaryTrack, jet);
            if (dR < dRMinJet) {
              dRMinJet = dR;
            }
          }

          secondaryTrack.set_dRMinJet(dRMinJet);
        }

      double eEM = 0;
      double dR = 0.5;
      for (EBRecHitCollection::const_iterator hit=EBRecHits->begin(); hit!=EBRecHits->end(); hit++) {
        if (insideCone(secondaryTrack, (*hit).detid(), dR)) {
          eEM += (*hit).energy();
          // cout << "       Added EB rec hit with (eta, phi) = "
          //      << getPosition((*hit).detid()).eta() << ", "
          //      << getPosition((*hit).detid()).phi() << endl;
        }
      }
      for (EERecHitCollection::const_iterator hit=EERecHits->begin(); hit!=EERecHits->end(); hit++) {
        if (insideCone(secondaryTrack, (*hit).detid(), dR)) {
          eEM += (*hit).energy();
          // cout << "       Added EE rec hit with (eta, phi) = "
          //      << getPosition((*hit).detid()).eta() << ", "
          //      << getPosition((*hit).detid()).phi() << endl;
        }
      }
      double eHad = 0;
      for (HBHERecHitCollection::const_iterator hit = HBHERecHits->begin(); hit != HBHERecHits->end(); hit++) {
        if (insideCone(secondaryTrack, (*hit).detid(), dR)) {
          eHad += (*hit).energy();
        }
      }

      secondaryTrack.set_caloNewEMDRp5(eEM);
      secondaryTrack.set_caloNewHadDRp5(eHad);
#endif
    }

  event.put (std::move (pl_), collection_.instance ());
  pl_.reset ();
}

bool OSUSecondaryTrackProducer::insideCone(TYPE(secondaryTracks)& candTrack, const DetId& id, const double dR)
{
   GlobalPoint idPosition = getPosition(id);
   if (idPosition.mag()<0.01) return false;
   math::XYZVector idPositionRoot( idPosition.x(), idPosition.y(), idPosition.z() );
   return deltaR(candTrack, idPositionRoot) < dR;
}

GlobalPoint OSUSecondaryTrackProducer::getPosition( const DetId& id)
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
OSUSecondaryTrackProducer::extractFiducialMap (const edm::ParameterSet &cfg, EtaPhiList &vetoList, stringstream &ss) const
{
  const edm::FileInPath &histFile = cfg.getParameter<edm::FileInPath> ("histFile");
  const string &beforeVetoHistName = cfg.getParameter<string> ("beforeVetoHistName");
  const string &afterVetoHistName = cfg.getParameter<string> ("afterVetoHistName");
  const double &thresholdForVeto = cfg.getParameter<double> ("thresholdForVeto");

  TFile *fin = TFile::Open (histFile.fullPath ().c_str ());
  if (!fin)
    {
      edm::LogWarning ("OSUSecondaryTrackProducer") << "No file named \"" << histFile.fullPath () << "\" found. Skipping...";
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

          content && ss << "(" << setw (10) << eta << ", " << setw (10) << phi << "): " << setw (10) << (content - mean) / hypot (error, meanErr) << " sigma above mean of " << setw (10) << mean;
          if ((content - mean) > thresholdForVeto * hypot (error, meanErr))
            {
              vetoList.emplace_back (eta, phi);
              ss << " * HOT SPOT *";
            }
          content && ss << endl;
        }
    }
  //////////////////////////////////////////////////////////////////////////////
  delete beforeVetoHist;
  delete afterVetoHist;
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUSecondaryTrackProducer);

#endif
