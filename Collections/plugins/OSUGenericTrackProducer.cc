#include "TFile.h"
#include "TH2D.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "OSUT3Analysis/Collections/plugins/OSUGenericTrackProducer.h"

#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"

#if IS_VALID(tracks)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

template<class T> 
OSUGenericTrackProducer<T>::OSUGenericTrackProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_ (cfg),
  useEraByEraFiducialMaps_ (cfg.getParameter<bool> ("useEraByEraFiducialMaps")),
  //caloGeometryToken_ (esConsumes<CaloGeometry, CaloGeometryRecord>()),
  caloGeometryToken_ (esConsumes<edm::Transition::BeginRun>()),
  //ecalStatusToken_ (esConsumes<EcalChannelStatus, EcalChannelStatusRcd>())
  ecalStatusToken_ (esConsumes<edm::Transition::BeginRun>())
{
  collection_ = collections_.getParameter<edm::InputTag> ("tracks");

  produces<vector<T> > (collection_.instance ());

  token_ = consumes<vector<TYPE(tracks)> > (collection_);
  mcparticleToken_   = consumes<vector<osu::Mcparticle> > (collections_.getParameter<edm::InputTag> ("mcparticles"));
  jetsToken_         = consumes<vector<TYPE(jets)> > (collections_.getParameter<edm::InputTag> ("jets"));
  pfCandidatesToken_ = consumes<vector<pat::PackedCandidate> > (cfg.getParameter<edm::InputTag> ("pfCandidates"));

#ifdef DISAPP_TRKS
  electronsToken_ = consumes<edm::View<TYPE(electrons)> > (cfg.getParameter<edm::InputTag> ("originalElectrons"));
  muonsToken_     = consumes<vector<TYPE(muons)> >     (cfg.getParameter<edm::InputTag> ("originalMuons"));
  tausToken_      = consumes<vector<TYPE(taus)> >      (cfg.getParameter<edm::InputTag> ("originalTaus"));

  eleVIDVetoIdMapToken_   = consumes<edm::ValueMap<bool> > (cfg.getParameter<edm::InputTag> ("eleVIDVetoIdMap"));
  eleVIDLooseIdMapToken_  = consumes<edm::ValueMap<bool> > (cfg.getParameter<edm::InputTag> ("eleVIDLooseIdMap"));
  eleVIDMediumIdMapToken_ = consumes<edm::ValueMap<bool> > (cfg.getParameter<edm::InputTag> ("eleVIDMediumIdMap"));
  eleVIDTightIdMapToken_  = consumes<edm::ValueMap<bool> > (cfg.getParameter<edm::InputTag> ("eleVIDTightIdMap"));

  primaryvertexToken_ = consumes<vector<TYPE(primaryvertexs)> > (collections_.getParameter<edm::InputTag> ("primaryvertexs"));

  lostTracksToken_ = consumes<vector<pat::PackedCandidate> > (cfg.getParameter<edm::InputTag> ("lostTracks"));
#if DATA_FORMAT_FROM_MINIAOD && ( DATA_FORMAT_IS_2017 || DATA_FORMAT_IS_2022 )
  isolatedTracksToken_ = consumes<vector<pat::IsolatedTrack> > (cfg.getParameter<edm::InputTag> ("isolatedTracks"));
#endif

#if !DATA_FORMAT_IS_2022
  candidateTracksToken_ = consumes<vector<CandidateTrack> > (cfg.getParameter<edm::InputTag> ("candidateTracks"));
#endif

#endif //ifdef DISAPP_TRKS

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

  //caloGeometryToken_  = esConsumes();
  //ecalStatusToken_    = esConsumes();

#if DATA_FORMAT_FROM_MINIAOD && !DATA_FORMAT_IS_2022
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
#endif // DATA_FORMAT_FROM_MINIAOD
}

template<class T> 
OSUGenericTrackProducer<T>::~OSUGenericTrackProducer ()
{
}

template<class T> void 
OSUGenericTrackProducer<T>::beginRun (const edm::Run &run, const edm::EventSetup& setup)
{
#if DATA_FORMAT_FROM_MINIAOD
  envSet (setup);
  getChannelStatusMaps ();
#endif // DATA_FORMAT_FROM_MINIAOD
}

template<class T> void 
OSUGenericTrackProducer<T>::produce (edm::Event &event, const edm::EventSetup &setup)
{
  //caloGeometry_ = setup.getHandle(caloGeometryToken_);
  //ecalStatus_   = setup.getHandle(ecalStatusToken_);


  //if( !ecalStatus_.isValid() )  throw "Failed to get ECAL channel status!";
  //if( !caloGeometry_.isValid()   )  throw "Failed to get the caloGeometry_!";

  edm::Handle<vector<TYPE(tracks)> > collection;
  if (!event.getByToken (token_, collection))
    {
      edm::LogWarning ("OSUGenericTrackProducer") << "Track collection not found. Skipping production of " << typeid(T).name() << " collection...";
      return;
    }

#if DATA_FORMAT_FROM_MINIAOD
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
  edm::Handle<edm::View<TYPE(electrons)> > electrons;
  event.getByToken(electronsToken_, electrons);
  edm::Handle<vector<TYPE(muons)> > muons;
  event.getByToken(muonsToken_, muons);
  edm::Handle<vector<TYPE(taus)> > taus;
  event.getByToken(tausToken_, taus);

  edm::Handle<edm::ValueMap<bool> > eleVIDVetoIdMap, eleVIDLooseIdMap, eleVIDMediumIdMap, eleVIDTightIdMap;
  event.getByToken(eleVIDVetoIdMapToken_,   eleVIDVetoIdMap);
  event.getByToken(eleVIDLooseIdMapToken_,  eleVIDLooseIdMap);
  event.getByToken(eleVIDMediumIdMapToken_, eleVIDMediumIdMap);
  event.getByToken(eleVIDTightIdMapToken_,  eleVIDTightIdMap);

  edm::Handle<vector<TYPE(primaryvertexs)> > vertices;
  event.getByToken (primaryvertexToken_, vertices);

  edm::Handle<vector<pat::PackedCandidate> > lostTracks;
  event.getByToken (lostTracksToken_, lostTracks);

#if DATA_FORMAT_FROM_MINIAOD && ( DATA_FORMAT_IS_2017 || DATA_FORMAT_IS_2022 )
  edm::Handle<vector<pat::IsolatedTrack> > isolatedTracks;
  event.getByToken (isolatedTracksToken_, isolatedTracks);
#endif
#if !DATA_FORMAT_IS_2022
  edm::Handle<vector<CandidateTrack> > candidateTracks;
  event.getByToken (candidateTracksToken_, candidateTracks);
#endif
#endif // DISAPP_TRKS

#endif // DATA_FORMAT_FROM_MINIAOD

  pl_ = unique_ptr<vector<T> > (new vector<T> ());
  for (const auto &object : *collection)
    {

#ifdef DISAPP_TRKS
      pl_->emplace_back (object,
                         particles,
                         pfCandidates,
                         lostTracks,
                         jets,
                         cfg_,
                         gsfTracks,
                         electronVetoList_,
                         muonVetoList_,
                         &EcalAllDeadChannelsValMap_,
                         &EcalAllDeadChannelsBitMap_,
                         !event.isRealData (),
#if DATA_FORMAT_FROM_MINIAOD && DATA_FORMAT_IS_2017
                         candidateTracks,
                         isolatedTracks);
#elif DATA_FORMAT_FROM_MINIAOD && DATA_FORMAT_IS_2022
                         isolatedTracks);
#else
                         candidateTracks);
#endif
#elif DATA_FORMAT_FROM_MINIAOD
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
      T &track = pl_->back ();

// in the specific case of TYPE(tracks)==CandidateTracks (where DATA_FORMAT is xyz_CUSTOM)
// and running over CandidateTracks ntuples, then generalTracks and RecHits may be available
#if DATA_FORMAT_IS_CUSTOM && !DATA_FORMAT_IS_2022
      // Calculate the associated calorimeter energy for the disappearing tracks search.

      // this could be removed; if CandidateTrackProdcuer sets these,
      // then these values need not be recalculated -- and RecHits can all be dropped
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

      // this is called only for ntuples with generalTracks explicitly kept (really just signal),
      // to re-calculate the track isolations calculated wrong when ntuples were produces (thus "old" vs not-old)
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
#endif // DATA_FORMAT_IS_CUSTOM

        track.set_minDeltaRToElectrons(electrons, vertices, eleVIDVetoIdMap, eleVIDLooseIdMap, eleVIDMediumIdMap, eleVIDTightIdMap);
        track.set_minDeltaRToMuons(muons, vertices);
        track.set_minDeltaRToTaus(taus);

#if DATA_FORMAT_FROM_MINIAOD && ( DATA_FORMAT_IS_2017 || DATA_FORMAT_IS_2022 )
        track.set_isoTrackIsolation(isolatedTracks);
#endif

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
  const string &era = cfg.getParameter<string> ("era");
  const string &beforeVetoHistName = useEraByEraFiducialMaps_ ? cfg.getParameter<string> ("beforeVetoHistName") + era : cfg.getParameter<string> ("beforeVetoHistName");
  const string &afterVetoHistName  = useEraByEraFiducialMaps_ ? cfg.getParameter<string> ("afterVetoHistName")  + era : cfg.getParameter<string> ("afterVetoHistName");
  const double &thresholdForVeto = cfg.getParameter<double> ("thresholdForVeto");

  edm::LogInfo ("OSUGenericTrackProducer") << "Attempting to extract \"" << beforeVetoHistName << "\" and \"" << afterVetoHistName << "\" from \"" << histFile.fullPath () << "\"...";
  TFile *fin = TFile::Open (histFile.fullPath ().c_str ());
  if (!fin || fin->IsZombie ())
    {
      edm::LogWarning ("OSUGenericTrackProducer") << "No file named \"" << histFile.fullPath () << "\" found. Skipping...";
      return;
    }

  TH2D * beforeVetoHist = (TH2D*)fin->Get(beforeVetoHistName.c_str());
  TH2D * afterVetoHist  = (TH2D*)fin->Get(afterVetoHistName.c_str());
  if (!beforeVetoHist) {
    edm::LogWarning("OSUGenericTrackProducer") << "No histogram named \"" << beforeVetoHistName.c_str() << "\" found. Skipping...";
    return;
  }
  if (!afterVetoHist) {
    edm::LogWarning("OSUGenericTrackProducer") << "No histogram named \"" << afterVetoHistName.c_str() << "\" found. Skipping...";
    return;
  }

  beforeVetoHist->SetDirectory(0);
  afterVetoHist->SetDirectory(0);
  fin->Close();
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
  caloGeometry_ = iSetup.getHandle(caloGeometryToken_);
  ecalStatus_   = iSetup.getHandle(ecalStatusToken_);

  // Old style, deprecated
  //iSetup.get<EcalChannelStatusRcd> ().get(ecalStatus_);
  //iSetup.get<CaloGeometryRecord>   ().get(caloGeometry_);

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
        auto cellGeom = subGeom->getGeometry (detid);
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
           auto cellGeom = subGeom->getGeometry (detid);
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

// this is actually a fix of a bugged function, normally it would be track. instead of t.
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
