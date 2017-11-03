#include "TFile.h"
#include "TH2D.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "OSUT3Analysis/Collections/plugins/OSUGenericTrackProducer.h"

#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"

#if IS_VALID(tracks)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

template<class T> 
OSUGenericTrackProducer<T>::OSUGenericTrackProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_ (cfg)
{
  collection_ = collections_.getParameter<edm::InputTag> ("tracks");

  produces<vector<T> > (collection_.instance ());

  token_ = consumes<vector<TYPE(tracks)> > (collection_);
  mcparticleToken_   = consumes<vector<osu::Mcparticle> > (collections_.getParameter<edm::InputTag> ("mcparticles"));
  jetsToken_         = consumes<vector<TYPE(jets)> > (collections_.getParameter<edm::InputTag> ("jets"));
  pfCandidatesToken_ = consumes<vector<pat::PackedCandidate> > (cfg.getParameter<edm::InputTag> ("pfCandidates"));
#ifdef DISAPP_TRKS
  candidateTracksToken_ = consumes<vector<CandidateTrack> > (cfg.getParameter<edm::InputTag> ("candidateTracks"));
#endif

  const edm::ParameterSet &fiducialMaps = cfg.getParameter<edm::ParameterSet> ("fiducialMaps");
  const vector<edm::ParameterSet> &electronFiducialMaps = fiducialMaps.getParameter<vector<edm::ParameterSet> > ("electrons");
  const vector<edm::ParameterSet> &muonFiducialMaps = fiducialMaps.getParameter<vector<edm::ParameterSet> > ("muons");

  maskedEcalChannelStatusThreshold_ = cfg.getParameter<int> ("maskedEcalChannelStatusThreshold");
  outputBadEcalChannels_ = cfg.getParameter<bool> ("outputBadEcalChannels");

  EBRecHitsTag_    =  cfg.getParameter<edm::InputTag>  ("EBRecHits");
  EERecHitsTag_    =  cfg.getParameter<edm::InputTag>  ("EERecHits");
  HBHERecHitsTag_  =  cfg.getParameter<edm::InputTag>  ("HBHERecHits");

  EBRecHitsToken_    =  consumes<EBRecHitCollection>    (EBRecHitsTag_);
  EERecHitsToken_    =  consumes<EERecHitCollection>    (EERecHitsTag_);
  HBHERecHitsToken_  =  consumes<HBHERecHitCollection>  (HBHERecHitsTag_);

  gsfTracksToken_ = consumes<vector<reco::GsfTrack> > (cfg.getParameter<edm::InputTag> ("gsfTracks"));

  // Get the general tracks collection explicitly. This is to fix a bug in the
  // disappearing track ntuples.
  tracksToken_ = consumes<vector<reco::Track> > (edm::InputTag ("generalTracks", "", "RECO"));

#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_2017
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
  edm::LogInfo ("OSUGenericTrackProducer") << ss.str ();
#endif // MINI_AOD or MINI_AOD_2017
}

template<class T> 
OSUGenericTrackProducer<T>::~OSUGenericTrackProducer ()
{
}

template<class T> void 
OSUGenericTrackProducer<T>::beginRun (const edm::Run &run, const edm::EventSetup& setup)
{
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_2017
  envSet (setup);
  getChannelStatusMaps ();
#endif // MINI_AOD or MINI_AOD_2017
}

template<class T> void 
OSUGenericTrackProducer<T>::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(tracks)> > collection;
  if (!event.getByToken (token_, collection))
    {
      edm::LogWarning ("OSUGenericTrackProducer") << "Track collection not found. Skipping production of " << typeid(T).name() << " collection...";
      return;
    }

#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_2017
  edm::Handle<vector<TYPE(jets)> > jets;
  if (!event.getByToken (jetsToken_, jets))
    edm::LogWarning ("OSUGenericTrackProducer") << "Jet collection not found.";

  edm::Handle<vector<osu::Mcparticle> > particles;
  event.getByToken (mcparticleToken_, particles);

  edm::Handle<EBRecHitCollection> EBRecHits;
  event.getByToken(EBRecHitsToken_, EBRecHits);
  edm::Handle<EERecHitCollection> EERecHits;
  event.getByToken(EERecHitsToken_, EERecHits);
  edm::Handle<HBHERecHitCollection> HBHERecHits;
  event.getByToken(HBHERecHitsToken_, HBHERecHits);

  edm::Handle<vector<reco::GsfTrack> > gsfTracks;
  event.getByToken (gsfTracksToken_, gsfTracks);

  edm::Handle<vector<reco::Track> > tracks;
  event.getByToken (tracksToken_, tracks);

  edm::Handle<vector<pat::PackedCandidate> > pfCandidates;
  event.getByToken (pfCandidatesToken_, pfCandidates);

#ifdef DISAPP_TRKS
  edm::Handle<vector<CandidateTrack> > candidateTracks;
  event.getByToken (candidateTracksToken_, candidateTracks);
#endif

#endif // MINI_AOD or MINI_AOD_2017

  pl_ = unique_ptr<vector<T> > (new vector<T> ());
  for (const auto &object : *collection)
    {

#ifdef DISAPP_TRKS
      pl_->emplace_back (object, 
                         particles, 
                         pfCandidates, 
                         jets, 
                         cfg_, 
                         gsfTracks, 
                         electronVetoList_, 
                         muonVetoList_, 
                         &EcalAllDeadChannelsValMap_, 
                         &EcalAllDeadChannelsBitMap_, 
                         !event.isRealData (), 
                         candidateTracks);
#elif DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_2017
      pl_->emplace_back (object, 
                         particles, 
                         pfCandidates, 
                         jets, 
                         cfg_, 
                         gsfTracks, 
                         electronVetoList_, 
                         muonVetoList_, 
                         &EcalAllDeadChannelsValMap_, 
                         &EcalAllDeadChannelsBitMap_, 
                         !event.isRealData ());
#else
      pl_->emplace_back (object);
#endif

#ifdef DISAPP_TRKS
#if DATA_FORMAT != MINI_AOD_2017
      // Calculate the associated calorimeter energy for the disappearing tracks search.
      T &track = pl_->back ();

      if (EBRecHits.isValid () && EERecHits.isValid () && HBHERecHits.isValid ())
        {
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
        }

      if (tracks.isValid ())
        {
          track.set_trackIsoDRp3 (getTrackIsolation (track, *tracks, false, false, 0.3));
          track.set_trackIsoDRp5 (getTrackIsolation (track, *tracks, false, false, 0.5));
          track.set_trackIsoNoPUDRp3 (getTrackIsolation (track, *tracks, true, false, 0.3));
          track.set_trackIsoNoPUDRp5 (getTrackIsolation (track, *tracks, true, false, 0.5));
          track.set_trackIsoNoFakesDRp3 (getTrackIsolation (track, *tracks, false, true, 0.3));
          track.set_trackIsoNoFakesDRp5 (getTrackIsolation (track, *tracks, false, true, 0.5));
          track.set_trackIsoNoPUNoFakesDRp3 (getTrackIsolation (track, *tracks, true, true, 0.3));
          track.set_trackIsoNoPUNoFakesDRp5 (getTrackIsolation (track, *tracks, true, true, 0.5));

          track.set_trackIsoOldNoPUDRp3 (getOldTrackIsolation (track, *tracks, true, 0.3));
          track.set_trackIsoOldNoPUDRp5 (getOldTrackIsolation (track, *tracks, true, 0.5));
        }
#endif // DATA_FORMAT != MINI_AOD_2017
#endif // DISAPP_TRKS
    }

  event.put (std::move (pl_), collection_.instance ());
  pl_.reset ();
}

template<class T> bool 
OSUGenericTrackProducer<T>::insideCone(TYPE(tracks)& candTrack, const DetId& id, const double dR)
{
   GlobalPoint idPosition = getPosition(id);
   if (idPosition.mag()<0.01) return false;
   math::XYZVector idPositionRoot( idPosition.x(), idPosition.y(), idPosition.z() );
   return deltaR(candTrack, idPositionRoot) < dR;
}

template<class T> GlobalPoint 
OSUGenericTrackProducer<T>::getPosition( const DetId& id)
{
   if ( ! caloGeometry_.isValid() ||
        ! caloGeometry_->getSubdetectorGeometry(id) ||
        ! caloGeometry_->getSubdetectorGeometry(id)->getGeometry(id) ) {
      throw cms::Exception("FatalError") << "Failed to access geometry for DetId: " << id.rawId();
      return GlobalPoint(0,0,0);
   }
   return caloGeometry_->getSubdetectorGeometry(id)->getGeometry(id)->getPosition();
}



template<class T> void 
OSUGenericTrackProducer<T>::extractFiducialMap (const edm::ParameterSet &cfg, EtaPhiList &vetoList, stringstream &ss) const
{
  const edm::FileInPath &histFile = cfg.getParameter<edm::FileInPath> ("histFile");
  const string &beforeVetoHistName = cfg.getParameter<string> ("beforeVetoHistName");
  const string &afterVetoHistName = cfg.getParameter<string> ("afterVetoHistName");
  const double &thresholdForVeto = cfg.getParameter<double> ("thresholdForVeto");

  edm::LogInfo ("OSUGenericTrackProducer") << "Attempting to extract \"" << beforeVetoHistName << "\" and \"" << afterVetoHistName << "\" from \"" << histFile.fullPath () << "\"...";
  TFile *fin = TFile::Open (histFile.fullPath ().c_str ());
  if (!fin || fin->IsZombie ())
    {
      edm::LogWarning ("OSUGenericTrackProducer") << "No file named \"" << histFile.fullPath () << "\" found. Skipping...";
      return;
    }

  TH2D *beforeVetoHist = (TH2D *) fin->Get (beforeVetoHistName.c_str ());
  beforeVetoHist->SetDirectory (0);
  TH2D *afterVetoHist = (TH2D *) fin->Get (afterVetoHistName.c_str ());
  afterVetoHist->SetDirectory (0);
  fin->Close ();
  delete fin;

  //////////////////////////////////////////////////////////////////////////////
  // First calculate the mean efficiency.
  //////////////////////////////////////////////////////////////////////////////

  double totalEventsBeforeVeto = 0, totalEventsAfterVeto = 0;
  int nBinsWithTags = 0;
  for (int i = 1; i <= beforeVetoHist->GetXaxis ()->GetNbins (); i++)
    {
      for (int j = 1; j <= beforeVetoHist->GetYaxis ()->GetNbins (); j++)
        {
          double binRadius = hypot (0.5 * beforeVetoHist->GetXaxis ()->GetBinWidth (i), 0.5 * beforeVetoHist->GetYaxis ()->GetBinWidth (j));
          (vetoList.minDeltaR < binRadius) && (vetoList.minDeltaR = binRadius);

          double contentBeforeVeto = beforeVetoHist->GetBinContent (i, j);

          if (!contentBeforeVeto) // skip bins that are empty in the before-veto histogram
            continue;

          nBinsWithTags++;

          totalEventsBeforeVeto += contentBeforeVeto;
          totalEventsAfterVeto  += afterVetoHist->GetBinContent (i, j);
        }
    }
  double meanInefficiency = totalEventsAfterVeto / totalEventsBeforeVeto;

  //////////////////////////////////////////////////////////////////////////////
  // Then calculate the standard deviation of the mean inefficiency.
  //////////////////////////////////////////////////////////////////////////////
  double stdDevInefficiency = 0;
  afterVetoHist->Divide (beforeVetoHist);
  for (int i = 1; i <= beforeVetoHist->GetXaxis ()->GetNbins (); i++)
    {
      for (int j = 1; j <= beforeVetoHist->GetYaxis ()->GetNbins (); j++)
        {
          if(beforeVetoHist->GetBinContent (i, j) == 0) // skip bins that are empty in the before-veto histogram
            continue;

          double thisInefficiency = afterVetoHist->GetBinContent (i, j);

          stdDevInefficiency += (thisInefficiency - meanInefficiency) * (thisInefficiency - meanInefficiency);
        }
    }

  if (nBinsWithTags < 2) stdDevInefficiency = 0.;
  else {
    stdDevInefficiency /= nBinsWithTags - 1;
    stdDevInefficiency = sqrt(stdDevInefficiency);
  }

  //////////////////////////////////////////////////////////////////////////////
  // Then find the bins which are greater than the mean by more than
  // thresholdForVeto sigma. Add the coordinates for these bins to the veto
  // list.
  //////////////////////////////////////////////////////////////////////////////
  for (int i = 1; i <= afterVetoHist->GetXaxis ()->GetNbins (); i++)
    {
      for (int j = 1; j <= afterVetoHist->GetYaxis ()->GetNbins (); j++)
        {
          double content = afterVetoHist->GetBinContent (i, j),
	    //error = afterVetoHist->GetBinError (i, j),
                 eta = afterVetoHist->GetXaxis ()->GetBinCenter (i),
                 phi = afterVetoHist->GetYaxis ()->GetBinCenter (j);

          content && ss << "(" << setw (10) << eta << ", " << setw (10) << phi << "): " << setw (10) << (content - meanInefficiency) / stdDevInefficiency << " sigma above mean of " << setw (10) << meanInefficiency;
          if ((content - meanInefficiency) > thresholdForVeto * stdDevInefficiency)
            {
              vetoList.emplace_back (eta, phi, (content - meanInefficiency) / stdDevInefficiency);
              ss << " * HOT SPOT *";
            }
          content && ss << endl;
        }
    }
  //////////////////////////////////////////////////////////////////////////////
  delete beforeVetoHist;
  delete afterVetoHist;
}

template<class T> void 
OSUGenericTrackProducer<T>::envSet (const edm::EventSetup& iSetup)
{
  iSetup.get<EcalChannelStatusRcd> ().get(ecalStatus_);
  iSetup.get<CaloGeometryRecord>   ().get(caloGeometry_);

  if( !ecalStatus_.isValid() )  throw "Failed to get ECAL channel status!";
  if( !caloGeometry_.isValid()   )  throw "Failed to get the caloGeometry_!";
}

template<class T> int 
OSUGenericTrackProducer<T>::getChannelStatusMaps ()
{
  EcalAllDeadChannelsValMap_.clear(); EcalAllDeadChannelsBitMap_.clear();
  TH2D *badChannels = (outputBadEcalChannels_ ? new TH2D ("badChannels", ";#eta;#phi", 360, -3.0, 3.0, 360, -3.2, 3.2) : NULL);

// Loop over EB ...
  for( int ieta=-85; ieta<=85; ieta++ ){
     for( int iphi=0; iphi<=360; iphi++ ){
        if(! EBDetId::validDetId( ieta, iphi ) )  continue;

        const EBDetId detid = EBDetId( ieta, iphi, EBDetId::ETAPHIMODE );
        EcalChannelStatus::const_iterator chit = ecalStatus_->find( detid );
// refer https://twiki.cern.ch/twiki/bin/viewauth/CMS/EcalChannelStatus
        int status = ( chit != ecalStatus_->end() ) ? chit->getStatusCode() & 0x1F : -1;

        const CaloSubdetectorGeometry*  subGeom = caloGeometry_->getSubdetectorGeometry (detid);
        const CaloCellGeometry*        cellGeom = subGeom->getGeometry (detid);
        double eta = cellGeom->getPosition ().eta ();
        double phi = cellGeom->getPosition ().phi ();
        double theta = cellGeom->getPosition().theta();

        if(status >= maskedEcalChannelStatusThreshold_){
           std::vector<double> valVec; std::vector<int> bitVec;
           valVec.push_back(eta); valVec.push_back(phi); valVec.push_back(theta);
           bitVec.push_back(1); bitVec.push_back(ieta); bitVec.push_back(iphi); bitVec.push_back(status);
           EcalAllDeadChannelsValMap_.insert( std::make_pair(detid, valVec) );
           EcalAllDeadChannelsBitMap_.insert( std::make_pair(detid, bitVec) );
           if (outputBadEcalChannels_)
             badChannels->Fill (eta, phi);
        }
     } // end loop iphi
  } // end loop ieta

// Loop over EE detid
  for( int ix=0; ix<=100; ix++ ){
     for( int iy=0; iy<=100; iy++ ){
        for( int iz=-1; iz<=1; iz++ ){
           if(iz==0)  continue;
           if(! EEDetId::validDetId( ix, iy, iz ) )  continue;

           const EEDetId detid = EEDetId( ix, iy, iz, EEDetId::XYMODE );
           EcalChannelStatus::const_iterator chit = ecalStatus_->find( detid );
           int status = ( chit != ecalStatus_->end() ) ? chit->getStatusCode() & 0x1F : -1;

           const CaloSubdetectorGeometry*  subGeom = caloGeometry_->getSubdetectorGeometry (detid);
           const CaloCellGeometry*        cellGeom = subGeom->getGeometry (detid);
           double eta = cellGeom->getPosition ().eta () ;
           double phi = cellGeom->getPosition ().phi () ;
           double theta = cellGeom->getPosition().theta();

           if(status >= maskedEcalChannelStatusThreshold_){
              std::vector<double> valVec; std::vector<int> bitVec;
              valVec.push_back(eta); valVec.push_back(phi); valVec.push_back(theta);
              bitVec.push_back(2); bitVec.push_back(ix); bitVec.push_back(iy); bitVec.push_back(iz); bitVec.push_back(status);
              EcalAllDeadChannelsValMap_.insert( std::make_pair(detid, valVec) );
              EcalAllDeadChannelsBitMap_.insert( std::make_pair(detid, bitVec) );
               if (outputBadEcalChannels_)
                 badChannels->Fill (eta, phi);
           }
        } // end loop iz
     } // end loop iy
  } // end loop ix

  if (outputBadEcalChannels_)
    {
      TFile *fout = new TFile ("badEcalChannels.root", "recreate");
      fout->cd ();
      badChannels->Write ();
      fout->Close ();

      delete badChannels;
      delete fout;
    }

  return 1;
}

template<class T> const double 
OSUGenericTrackProducer<T>::getTrackIsolation (const reco::Track &track, const vector<reco::Track> &tracks, const bool noPU, const bool noFakes, const double outerDeltaR, const double innerDeltaR) const
{
  double sumPt = 0.0;

  for (const auto &t : tracks)
    {

      if (noFakes && (t.normalizedChi2() > 20.0 ||
                      t.hitPattern().pixelLayersWithMeasurement() < 2 ||
                      t.hitPattern().trackerLayersWithMeasurement() < 5 ||
                      fabs(t.d0() / t.d0Error()) > 5.0))
        continue;
      if (noPU && track.dz(t.vertex()) > 3.0 * hypot(track.dzError(), t.dzError()))
        continue;

      double dR = deltaR (track, t);
      if (dR < outerDeltaR && dR > innerDeltaR)
        sumPt += t.pt ();
    }

  return sumPt;
}

// durp -- this is actually a fix of a bugged function, normally it would be track. instead of t.
template<class T> const double 
OSUGenericTrackProducer<T>::getOldTrackIsolation (const reco::Track &track, const vector<reco::Track> &tracks, const bool noPU, const double outerDeltaR, const double innerDeltaR) const
{
  double sumPt = 0.0;

  for (const auto &t : tracks)
    {
      if (noPU && (t.normalizedChi2() > 20.0 ||
                   t.hitPattern().pixelLayersWithMeasurement() < 2 ||
                   t.hitPattern().trackerLayersWithMeasurement() < 5 ||
                   fabs(t.d0() / t.d0Error()) > 5.0 ||
                   track.dz(t.vertex()) > 3.0 * hypot(track.dzError(), t.dzError())))
        continue;

      double dR = deltaR (track, t);
      if (dR < outerDeltaR && dR > innerDeltaR)
        sumPt += t.pt ();
    }

  return sumPt;
}

#include "FWCore/Framework/interface/MakerMacros.h"
typedef OSUGenericTrackProducer<osu::Track> OSUTrackProducer;
DEFINE_FWK_MODULE(OSUTrackProducer);

#if IS_VALID(secondaryTracks)
typedef OSUGenericTrackProducer<osu::SecondaryTrack> OSUSecondaryTrackProducer;
DEFINE_FWK_MODULE(OSUSecondaryTrackProducer);
#endif

#endif // IS_VALID(tracks)
