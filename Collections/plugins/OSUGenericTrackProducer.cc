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
  ecalStatusToken_ (esConsumes<edm::Transition::BeginRun>()),
  trackerTopologyToken_ (esConsumes<edm::Transition::BeginRun>())

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

  rhoToken_             = consumes<double>  (cfg.getParameter<edm::InputTag> ("rhoTag"));
  rhoCaloToken_         = consumes<double>  (cfg.getParameter<edm::InputTag> ("rhoCaloTag"));
  rhoCentralCaloToken_  = consumes<double>  (cfg.getParameter<edm::InputTag> ("rhoCentralCaloTag"));

  EBRecHitsToken_    =  consumes<EBRecHitCollection>    (EBRecHitsTag_);
  EERecHitsToken_    =  consumes<EERecHitCollection>    (EERecHitsTag_);
  HBHERecHitsToken_  =  consumes<HBHERecHitCollection>  (HBHERecHitsTag_);

#if AOD_SKIM
  gt2dedxPixelTag_ =  cfg.getParameter<edm::InputTag>  ("dEdxDataPixel");
  gt2dedxStripTag_ =  cfg.getParameter<edm::InputTag>  ("dEdxDataStrip");
  gt2dedxPixelToken_    = consumes<edm::ValueMap<reco::DeDxData> > (gt2dedxPixelTag_);
  gt2dedxStripToken_    = consumes<edm::ValueMap<reco::DeDxData> > (gt2dedxStripTag_);
#endif

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

  edm::Handle<double> rhoHandle;
  event.getByToken (rhoToken_, rhoHandle );
  edm::Handle<double> rhoCaloHandle;
  event.getByToken (rhoCaloToken_, rhoCaloHandle );
  edm::Handle<double> rhoCentralCaloHandle;
  event.getByToken (rhoCentralCaloToken_, rhoCentralCaloHandle );

#if AOD_SKIM
  edm::Handle<edm::ValueMap<reco::DeDxData> > gt2dedxPixel;
  event.getByToken(gt2dedxPixelToken_, gt2dedxPixel);
  edm::Handle<edm::ValueMap<reco::DeDxData> > gt2dedxStrip;
  event.getByToken(gt2dedxStripToken_, gt2dedxStrip);
#endif

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
//#endif //temp mcarrigan

// in the specific case of TYPE(tracks)==CandidateTracks (where DATA_FORMAT is xyz_CUSTOM)
// and running over CandidateTracks ntuples, then generalTracks and RecHits may be available
//#if DATA_FORMAT_IS_CUSTOM && !DATA_FORMAT_IS_2022
#if DATA_FORMAT_IS_CUSTOM || (DATA_FORMAT_FROM_MINIAOD && DATA_FORMAT_IS_2022 && AOD_SKIM) //mcarrigan
      // Calculate the associated calorimeter energy for the disappearing tracks search.

      track.set_rhoPUCorr(*rhoHandle);
      track.set_rhoPUCorrCalo(*rhoCaloHandle);
      track.set_rhoPUCorrCentralCalo(*rhoCentralCaloHandle);

      const CaloEnergy &caloE_0p5 = calculateCaloE(track, *EBRecHits, *EERecHits, *HBHERecHits, 0.5);
      track.set_caloNewEMDRp5 (caloE_0p5.eEM);
      track.set_caloNewHadDRp5 (caloE_0p5.eHad);

      const CaloEnergy &caloE_0p3 = calculateCaloE(track, *EBRecHits, *EERecHits, *HBHERecHits, 0.3);
      track.set_caloNewEMDRp3 (caloE_0p3.eEM);
      track.set_caloNewHadDRp3 (caloE_0p3.eHad);

      const CaloEnergy &caloE_0p2 = calculateCaloE(track, *EBRecHits, *EERecHits, *HBHERecHits, 0.2);
      track.set_caloNewEMDRp2 (caloE_0p2.eEM);
      track.set_caloNewHadDRp2 (caloE_0p2.eHad);

      const CaloEnergy &caloE_0p1 = calculateCaloE(track, *EBRecHits, *EERecHits, *HBHERecHits, 0.1);
      track.set_caloNewEMDRp1 (caloE_0p1.eEM);
      track.set_caloNewHadDRp1 (caloE_0p1.eHad);


      // this could be removed; if CandidateTrackProdcuer sets these,
      // then these values need not be recalculated -- and RecHits can all be dropped*/
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
          //track.set_caloNewHadDRp5(eHad);
        }
#endif
#if DATA_FORMAT_IS_CUSTOM || DATA_FORMAT_IS_2022

      // this is called only for ntuples with generalTracks explicitly kept (really just signal),
      // to re-calculate the track isolations calculated wrong when ntuples were produces (thus "old" vs not-old)
      if (isolatedTracks.isValid ())
        {
          track.set_trackIsoDRp3 (getTrackIsolation (track, *isolatedTracks, false, false, 0.3));
          track.set_trackIsoDRp5 (getTrackIsolation (track, *isolatedTracks, false, false, 0.5));
          track.set_trackIsoNoPUDRp3 (getTrackIsolation (track, *isolatedTracks, true, false, 0.3));
          track.set_trackIsoNoPUDRp5 (getTrackIsolation (track, *isolatedTracks, true, false, 0.5));
          track.set_trackIsoNoFakesDRp3 (getTrackIsolation (track, *isolatedTracks, false, true, 0.3));
          track.set_trackIsoNoFakesDRp5 (getTrackIsolation (track, *isolatedTracks, false, true, 0.5));
          track.set_trackIsoNoPUNoFakesDRp3 (getTrackIsolation (track, *isolatedTracks, true, true, 0.3));
          track.set_trackIsoNoPUNoFakesDRp5 (getTrackIsolation (track, *isolatedTracks, true, true, 0.5));

          //track.set_trackIsoOldNoPUDRp3 (getOldTrackIsolation (track, *tracks, true, 0.3));
          //track.set_trackIsoOldNoPUDRp5 (getOldTrackIsolation (track, *tracks, true, 0.5));
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

template<class T> const bool 
OSUGenericTrackProducer<T>::insideCone(TYPE(tracks)& candTrack, const DetId& id, const double dR) const
{
   GlobalPoint idPosition = getPosition(id);
   if (idPosition.mag()<0.01) return false;
   math::XYZVector idPositionRoot( idPosition.x(), idPosition.y(), idPosition.z() );
   return deltaR(candTrack, idPositionRoot) < dR;
}

template<class T> const GlobalPoint 
OSUGenericTrackProducer<T>::getPosition( const DetId& id) const
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
  trackerTopology_ = iSetup.getHandle(trackerTopologyToken_);


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
OSUGenericTrackProducer<T>::getTrackIsolation (TYPE(tracks)& track, const vector<TYPE(tracks)> &tracks, const bool noPU, const bool noFakes, const double outerDeltaR, const double innerDeltaR) const
{
  double sumPt = 0.0;
  for (const auto &t : tracks)
    {
      //if (noFakes && (t.normalizedChi2() > 20.0 ||
      if (noFakes &&
            (t.hitPattern().pixelLayersWithMeasurement() < 2 ||
            t.hitPattern().trackerLayersWithMeasurement() < 5 ||
            fabs(t.dxy() / t.dxyError()) > 5.0))
        continue;
#if DATA_FORMAT_IS_2022
      if (noPU && track.dz() > 3.0 * hypot(track.dzError(), t.dzError()))
        continue;
#else
      if (noPU && track.dz(t.vertex()) > 3.0 * hypot(track.dzError(), t.dzError()))
        continue;
#endif


      double dR = deltaR (track, t);
      if (dR < outerDeltaR && dR > innerDeltaR)
        sumPt += t.pt ();
    }

  return sumPt;
}

// this is actually a fix of a bugged function, normally it would be track. instead of t.
template<class T> const double 
OSUGenericTrackProducer<T>::getOldTrackIsolation (TYPE(tracks)& track, const vector<reco::Track> &tracks, const bool noPU, const double outerDeltaR, const double innerDeltaR) const
{
  double sumPt = 0.0;

  for (const auto &t : tracks)
    {
#ifdef DATA_FORMAT_IS_2022
      if (noPU && (t.normalizedChi2() > 20.0 ||
                   t.hitPattern().pixelLayersWithMeasurement() < 2 ||
                   t.hitPattern().trackerLayersWithMeasurement() < 5 ||
                   fabs(t.d0() / t.d0Error()) > 5.0 ||
                   track.dz() > 3.0 * hypot(track.dzError(), t.dzError())))
#else
      if (noPU && (t.normalizedChi2() > 20.0 ||
                   t.hitPattern().pixelLayersWithMeasurement() < 2 ||
                   t.hitPattern().trackerLayersWithMeasurement() < 5 ||
                   fabs(t.d0() / t.d0Error()) > 5.0 ||
                   track.dz(t.vertex()) > 3.0 * hypot(track.dzError(), t.dzError())))
#endif
        continue;

      double dR = deltaR (track, t);
      if (dR < outerDeltaR && dR > innerDeltaR)
        sumPt += t.pt ();
    }

  return sumPt;
}

template<class T> const CaloEnergy
OSUGenericTrackProducer<T>::calculateCaloE (TYPE(tracks)& track, const EBRecHitCollection &EBRecHits, const EERecHitCollection &EERecHits, const HBHERecHitCollection &HBHERecHits, const double dR) const
{ 
  double eEM = 0;
  for (const auto &hit : EBRecHits) {
    if (insideCone(track, hit.detid(), dR)) {
      eEM += hit.energy();
    }
  }
  for (const auto &hit : EERecHits) {
    if (insideCone(track, hit.detid(), dR)) {
      eEM += hit.energy();
    }
  }

  double eHad = 0;
  for (const auto &hit : HBHERecHits) {
    if (insideCone(track, hit.detid(), dR)) {
      eHad += hit.energy();
    }
  }

  return {eEM, eHad};
}

template<class T> int
OSUGenericTrackProducer<T>::countGoodPrimaryVertices(const vector<reco::Vertex> &vertices) const
{
  int n = 0;
  for(const auto &v : vertices) {
    if(v.isValid() && v.ndof() >= 4 && fabs(v.z()) < 24.0 && hypot(v.x(), v.y()) < 2.0) n++;
  }
  return n;
}

template<class T> int
OSUGenericTrackProducer<T>::countGoodJets(const vector<pat::Jet> &jets) const
{
  int n = 0;
  vector<double> goodJetPhis;

  for(const auto &jet : jets) {
    // let's not deal with jet smearing for now...
    if(jet.pt() <= 110) continue;
    if(fabs(jet.eta()) >= 2.4) continue;
    if(!anatools::jetPassesTightLepVeto(jet)) continue;

    n++;
  }
  return n;
}

template<class T> double
OSUGenericTrackProducer<T>::getMaxDijetDeltaPhi(const vector<pat::Jet> &jets) const
{
  vector<double> goodJetPhis;
  for(const auto &jet : jets) {
    if(jet.pt() > 30 && fabs(jet.eta()) < 4.5 && anatools::jetPassesTightLepVeto(jet)) goodJetPhis.push_back(jet.phi());
  }

  double maxDeltaPhi = -999.;
  for(unsigned int i = 0; i < goodJetPhis.size(); i++) {
    for(unsigned int j = 0; j < goodJetPhis.size(); j++) {
      if(i == j) continue;
      double dPhi = fabs(deltaPhi(goodJetPhis[i], goodJetPhis[j]));
      if(dPhi > maxDeltaPhi) maxDeltaPhi = dPhi;
    }
  }

  return maxDeltaPhi;
}

template<class T> double
OSUGenericTrackProducer<T>::getLeadingJetMetPhi(const vector<pat::Jet> &jets, const pat::MET &met) const
{
  double deltaPhiMetJetLeading = 999.;
  double ptJetLeading = -1;

  for(const auto &jet : jets) {
    if(jet.pt() > 30 && 
       fabs(jet.eta()) < 4.5 && 
       anatools::jetPassesTightLepVeto(jet)) {
      if(jet.pt() > ptJetLeading) {
        ptJetLeading = jet.pt();
        deltaPhiMetJetLeading = fabs(deltaPhi(jet, met));
      }
    }
  }

  return deltaPhiMetJetLeading;
}

template<class T> vector<pat::Electron>
OSUGenericTrackProducer<T>::getTagElectrons(const edm::Event &event,
                                           const edm::TriggerResults &triggers,
                                           const vector<pat::TriggerObjectStandAlone> &trigObjs,
                                           const reco::Vertex &vertex,
                                           const edm::View<pat::Electron> &electrons)
{
  vector<pat::Electron> tagElectrons;

  for(const auto &electron : electrons) {
    if(electron.pt() <= (is2017_ ? 35 : 32)) continue;

    if(!anatools::isMatchedToTriggerObject(event,
                                           triggers,
                                           electron,
                                           trigObjs,
                                           "hltEgammaCandidates::HLT", 
                                           (is2017_ ? "hltEle35noerWPTightGsfTrackIsoFilter" : "hltEle32WPTightGsfTrackIsoFilter"))) {
      continue; // cutElectronMatchToTrigObj
    }

    if(fabs(electron.eta()) >= 2.1) continue;
    if(!electron.electronID(is2017_ ? "cutBasedElectronID-Fall17-94X-V1-tight" : "cutBasedElectronID-Fall17-94X-V2-tight")) continue;
    
    if(fabs(electron.superCluster()->eta()) <= 1.479) {
      if(fabs(electron.gsfTrack()->dxy(vertex.position())) >= 0.05) continue;
      if(fabs(electron.gsfTrack()->dz(vertex.position())) >= 0.10) continue;
    }
    else {
      if(fabs(electron.gsfTrack()->dxy(vertex.position())) >= 0.10) continue;
      if(fabs(electron.gsfTrack()->dz(vertex.position())) >= 0.20) continue;
    }

    tagElectrons.push_back(electron);
  }

  return tagElectrons;
}

template <class T> vector<pat::Muon>
OSUGenericTrackProducer<T>::getTagMuons(const edm::Event &event,
                                       const edm::TriggerResults &triggers,
                                       const vector<pat::TriggerObjectStandAlone> &trigObjs,
                                       const reco::Vertex &vertex,
                                       const vector<pat::Muon> &muons)
{
  vector<pat::Muon> tagMuons;

  for(const auto &muon : muons) {
    if(muon.pt() <= (is2017_ ? 29 : 26)) continue;
    if(fabs(muon.eta()) >= 2.1) continue;
    if(!muon.isTightMuon(vertex)) continue;

    double iso = muon.pfIsolationR04().sumNeutralHadronEt +
                 muon.pfIsolationR04().sumPhotonEt +
                 -0.5 * muon.pfIsolationR04().sumPUPt;
    iso = muon.pfIsolationR04().sumChargedHadronPt + max(0.0, iso);
    if(iso / muon.pt() >= 0.15) continue;

    if(!anatools::isMatchedToTriggerObject(event,
                                           triggers,
                                           muon,
                                           trigObjs,
                                           (is2017_ ? "hltIterL3MuonCandidates::HLT" : "hltHighPtTkMuonCands::HLT"),
                                           (is2017_ ? "hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07" : "hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07"))) {
      continue; // cutMuonMatchToTrigObj
    }
    
    tagMuons.push_back(muon);
  }

  return tagMuons;
}

template<class T> TrackInfo
OSUGenericTrackProducer<T>::getTrackInfo(const T &track,
                              const reco::Vertex &pv, 
                              const vector<pat::Jet> &jets,
                              const edm::View<pat::Electron> &electrons,
                              const vector<pat::Muon> &muons,
                              const vector<pat::Tau> &taus,
                              const vector<pat::Electron> &tagElectrons,
                              const vector<pat::Muon> &tagMuons,
                              const pat::MET &met,
                              const edm::Handle<vector<pat::IsolatedTrack> > tracks, 
                              const edm::Handle<reco::DeDxHitInfoAss> isoTrk2dedxHitInfo,
                              const edm::Handle<edm::ValueMap<reco::DeDxData> > dEdxStrip,
                              const edm::Handle<edm::ValueMap<reco::DeDxData> > dEdxPixel)
{
  //for(vector<pat::IsolatedTrack>::const_iterator it_track = tracks->begin(); it_track != tracks->end(); it_track++) {

  //pat::IsolatedTrack track = tracks.at(iTrack);    
  TrackInfo info;
  
  //apply track pt cut
  if(minTrackPt_ > 0 && track.pt() <= minTrackPt_) return info;

  info.trackIso = 0.0;
  for(const auto &t : *tracks) {
    const auto theptinv2 = 1 / pow(track.pt(),2);
    float dz_track = (track.vz() - t.vz()) - ((track.vx() - t.vx()) * track.px() + (track.vy() - t.vy()) * track.py()) * track.pz() * theptinv2;
    if(fabs(dz_track) > 3.0 * hypot(track.dzError(), t.dzError())) continue;
    double dR = deltaR(track, t);
    if(dR < 0.3 && dR > 1.0e-12) info.trackIso += t.pt();
  }

  // apply relative track isolation cut
  if(maxRelTrackIso_ > 0 && info.trackIso / track.pt() >= maxRelTrackIso_) return info;

  info.px = track.px();
  info.py = track.py();
  info.pz = track.pz();
  info.vx = track.vx();
  info.vy = track.vy();
  info.vz = track.vz();
  info.eta = track.eta();
  info.pt = track.pt();
  //info.ptError = track.ptError();
  info.phi = track.phi();
  info.charge = track.charge();

  info.dEdxInfo.clear();

  edm::Ref<vector<pat::IsolatedTrack> > matchedIsolatedTrack;
  double dRToMatchedIsolatedTrack = 0.01;
  track.findMatchedIsolatedTrack(tracks, matchedIsolatedTrack, dRToMatchedIsolatedTrack);

  /*if(dRToMatchedIsolatedTrack == INVALID_VALUE) {
    info.dEdxInfo.push_back(TrackDeDxInfo());
  }
  else {
    if(isoTrk2dedxHitInfo->contains(matchedIsolatedTrack.id())) {
      const reco::DeDxHitInfo * hitInfo = (*isoTrk2dedxHitInfo)[matchedIsolatedTrack].get();
      if(hitInfo == nullptr) {
        //edm::LogWarning ("disappTrks_DeDxHitInfoVarProducer") << "Encountered a null DeDxHitInfo object from a pat::IsolatedTrack? Skipping this track...";
        continue;
      }*/

  //edm::Ref<vector<pat::IsolatedTrack> > matchedIsolatedTrack = edm::Ref<vector<pat::IsolatedTrack> >(tracks, track);

  if(isoTrk2dedxHitInfo->contains(matchedIsolatedTrack.id())) {
    const reco::DeDxHitInfo * hitInfo = (*isoTrk2dedxHitInfo)[matchedIsolatedTrack].get();
    if(hitInfo == nullptr) {
      //edm::LogWarning ("disappTrks_DeDxHitInfoVarProducer") << "Encountered a null DeDxHitInfo object from a pat::IsolatedTrack? Skipping this track...";
      return info;
    }

    for(unsigned int iHit = 0; iHit < hitInfo->size(); iHit++) {
      bool isPixel = (hitInfo->pixelCluster(iHit) != nullptr);
      bool isStrip = (hitInfo->stripCluster(iHit) != nullptr);
      if(!isPixel && !isStrip) continue; // probably shouldn't happen
      if(isPixel && isStrip) continue;
      
      //subdet Id = {1, pbx}, {2, pxf}, {3, tib}, {4, tid}, {5, tob}, {6, tec}
      int subDet = hitInfo->detId(iHit).subdetId();
      if(subDet == PixelSubdetector::PixelBarrel) subDet = 1;
      else if (subDet == PixelSubdetector::PixelEndcap) subDet = 2;
      else if(subDet == StripSubdetector::TIB) subDet = 3;  //N.B. in CMSSW_11 StripSubdetector -> SiStripSubdetector
      else if (subDet == StripSubdetector::TID) subDet = 4;
      else if (subDet == StripSubdetector::TOB) subDet = 5;
      else if (subDet == StripSubdetector::TEC) subDet = 6;

      float norm = isPixel ? 3.61e-06 : 3.61e-06 * 265;

      info.dEdxInfo.push_back(
        TrackDeDxInfo(subDet,
                      norm * hitInfo->charge(iHit) / hitInfo->pathlength(iHit),
                      isPixel ? hitInfo->pixelCluster(iHit)->size()  : -1,
                      isPixel ? hitInfo->pixelCluster(iHit)->sizeX() : -1,
                      isPixel ? hitInfo->pixelCluster(iHit)->sizeY() : -1,
#if CMSSW_VERSION_CODE >= CMSSW_VERSION(12,4,0)
                      isStrip ? deDxTools::shapeSelection(*(hitInfo->stripCluster(iHit))) : false,
#else
                      isStrip ? deDxTools::shapeSelection(*(hitInfo->stripCluster(iHit))) : false,
#endif
                      hitInfo->pos(iHit).x(),
                      hitInfo->pos(iHit).y(),
                      hitInfo->pos(iHit).z(),
                      trackerTopology_->layer(hitInfo->detId(iHit)))); // gives layer within sub detector
    }
  } // if isoTrk in association map
  else {
    info.dEdxInfo.push_back(TrackDeDxInfo()); // if somehow the matched isoTrk isn't in the hitInfo?
  }
  //} //if dRToMatchedIsoTrk != invalid

  /*edm::Ref<vector<reco::Track> > matchedGenTrack;
  double dRToMatchedGenTrack;
  findMatchedGenTrack(genTracks, matchedGenTrack, dRToMatchedGenTrack, track);
  if(dRToMatchedGenTrack == INVALID_VALUE){
    info.dEdxPixel = -10;
    info.numMeasurementsPixel = -10;
    info.numSatMeasurementsPixel = -10;
    info.dEdxStrip = -10;
    info.numMeasurementsStrip = -10;
    info.numSatMeasurementsStrip = -10;
  }
  else{
    const reco::DeDxData &dEdxDataPixel = (*dEdxPixel)[matchedGenTrack];
    const reco::DeDxData &dEdxDataStrip = (*dEdxStrip)[matchedGenTrack];

    info.dEdxPixel = dEdxDataPixel.dEdx();
    info.numMeasurementsPixel = dEdxDataPixel.numberOfMeasurements();
    info.numSatMeasurementsPixel = dEdxDataPixel.numberOfSaturatedMeasurements();
    info.dEdxStrip = dEdxDataStrip.dEdx();
    info.numMeasurementsStrip = dEdxDataStrip.numberOfMeasurements();
    info.numSatMeasurementsStrip = dEdxDataStrip.numberOfSaturatedMeasurements();
  }*/

  info.dEdxPixel = track.dEdxPixel();
  info.dEdxStrip = track.dEdxStrip();
  info.numMeasurementsPixel = track.hitPattern().numberOfValidPixelHits();
  info.numMeasurementsStrip = track.hitPattern().numberOfValidStripHits();


  info.dRMinJet = -1;
  for(const auto &jet : jets) {
    if(jet.pt() > 30 &&
        fabs(jet.eta()) < 4.5 &&
        (((jet.neutralHadronEnergyFraction()<0.90 && jet.neutralEmEnergyFraction()<0.90 && (jet.chargedMultiplicity() + jet.neutralMultiplicity())>1 && jet.muonEnergyFraction()<0.8) && ((fabs(jet.eta())<=2.4 && jet.chargedHadronEnergyFraction()>0 && jet.chargedMultiplicity()>0 && jet.chargedEmEnergyFraction()<0.90) || fabs(jet.eta())>2.4) && fabs(jet.eta())<=3.0)
          || (jet.neutralEmEnergyFraction()<0.90 && jet.neutralMultiplicity()>10 && fabs(jet.eta())>3.0))) {
      double dR = deltaR(track, jet);
      if(info.dRMinJet < 0 || dR < info.dRMinJet) info.dRMinJet = dR;
    }
  }

  bool inTOBCrack = (fabs(track.dz()) < 0.5 && fabs(M_PI_2 - track.theta()) < 1.0e-3);
  bool inECALCrack = (fabs(track.eta()) >= 1.42 && fabs(track.eta()) <= 1.65);
  bool inDTWheelGap = (fabs(track.eta()) >= 0.15 && fabs(track.eta()) <= 0.35);
  bool inCSCTransitionRegion = (fabs(track.eta()) >= 1.55 && fabs(track.eta()) <= 1.85);
  info.inGap = (inTOBCrack || inECALCrack || inDTWheelGap || inCSCTransitionRegion);

  info.dRMinBadEcalChannel = minDRBadEcalChannel(track);

  info.nValidPixelHits        = track.hitPattern().numberOfValidPixelHits();
  info.nValidHits             = track.hitPattern().numberOfValidHits();
  info.numberOfValidMuonHits  = track.hitPattern().numberOfValidMuonHits();
  info.missingInnerHits       = track.hitPattern().trackerLayersWithoutMeasurement(reco::HitPattern::MISSING_INNER_HITS);
  info.missingMiddleHits      = track.hitPattern().trackerLayersWithoutMeasurement(reco::HitPattern::TRACK_HITS);
  info.missingOuterHits       = track.hitPattern().trackerLayersWithoutMeasurement(reco::HitPattern::MISSING_OUTER_HITS);
  info.nLayersWithMeasurement = track.hitPattern().trackerLayersWithMeasurement();
  info.pixelLayersWithMeasurement = track.hitPattern().pixelLayersWithMeasurement();

  // d0 wrt pv (2d) = (vertex - pv) cross p / |p|
  info.d0 = ((track.vx() - pv.x()) * track.py() - (track.vy() - pv.y()) * track.px()) / track.pt(); 

  // dz wrt pv (2d) = (v_z - pv_z) - p_z * [(vertex - pv) dot p / |p|^2]
  info.dz = track.vz() - pv.z() -
    ((track.vx() - pv.x()) * track.px() + (track.vy() - pv.y()) * track.py()) * track.pz() / track.pt() / track.pt();

  //info.normalizedChi2 = track.normalizedChi2();
  info.highPurityFlag = track.isHighPurityTrack();

  info.deltaRToClosestElectron = -1;
  for(const auto &electron : electrons) {
    double thisDR = deltaR(electron, track);
    if(info.deltaRToClosestElectron < 0 || thisDR < info.deltaRToClosestElectron) info.deltaRToClosestElectron = thisDR;
  }

  info.deltaRToClosestMuon = -1;
  for(const auto &muon : muons) {
    double thisDR = deltaR(muon, track);
    if(info.deltaRToClosestMuon < 0 || thisDR < info.deltaRToClosestMuon) info.deltaRToClosestMuon = thisDR;
  }

  info.deltaRToClosestTauHad = -1;
  for(const auto &tau : taus) {
    if(tau.isTauIDAvailable("againstElectronLooseMVA5")) {
      if(tau.tauID("decayModeFinding") <= 0.5 ||
          tau.tauID("againstElectronLooseMVA5") <= 0.5 ||
          tau.tauID("againstMuonLoose3") <= 0.5) {
        continue;
      }
    }
    else if(tau.isTauIDAvailable("againstElectronLooseMVA6")) {
      if(tau.tauID("decayModeFinding") <= 0.5 ||
          tau.tauID("againstElectronLooseMVA6") <= 0.5 ||
          tau.tauID("againstMuonLoose3") <= 0.5) {
        continue;
      }
    }
    else {
      continue;
    }

    double thisDR = deltaR(tau, track);
    if(info.deltaRToClosestTauHad < 0 || thisDR < info.deltaRToClosestTauHad) info.deltaRToClosestTauHad = thisDR;
  }

  info.passesProbeSelection = isProbeTrack(info);

  info.deltaRToClosestTagElectron = -1;
  info.deltaRToClosestTagMuon = -1;

  info.isTagProbeElectron = 0;
  info.isTagProbeTauToElectron = 0;

  info.isTagProbeMuon = 0;
  info.isTagProbeTauToMuon = 0;

  if(info.passesProbeSelection) {
    for(const auto &tag : tagElectrons) {
      double thisDR = deltaR(tag, track);
      if(info.deltaRToClosestTagElectron < 0 || thisDR < info.deltaRToClosestTagElectron) {
        info.deltaRToClosestTagElectron = thisDR;
      }
      info.isTagProbeElectron |= isTagProbeElePair(track, tag);
      info.isTagProbeTauToElectron |= isTagProbeTauToElePair(track, tag, met);
    }

    for(const auto &tag : tagMuons) {
      double thisDR = deltaR(tag, track);
      if(info.deltaRToClosestTagMuon < 0 || thisDR < info.deltaRToClosestTagMuon) {
        info.deltaRToClosestTagMuon = thisDR;
      }
      info.isTagProbeMuon |= isTagProbeMuonPair(track, tag);
      info.isTagProbeTauToMuon |= isTagProbeTauToMuonPair(track, tag, met);
    }
  }

  info.ecalo = 0; // calculated in getRecHits

    //trackInfos_.push_back(info);
  //}

  return info;
}

template<class T> const bool
OSUGenericTrackProducer<T>::isProbeTrack(const TrackInfo info) const
{
  if(info.pt <= 30 ||
     fabs(info.eta) >= 2.1 ||
     // skip fiducial sElectrons
     info.nValidPixelHits < 4 ||
     info.nValidHits < 4 ||
     info.missingInnerHits != 0 ||
     info.missingMiddleHits != 0 ||
     info.trackIso / info.pt >= 0.05 ||
     fabs(info.d0) >= 0.02 ||
     fabs(info.dz) >= 0.5 ||
     // skip lepton vetoes
     fabs(info.dRMinJet) <= 0.5) {
    return false;
  }

  return true;
}

template<class T> const double
OSUGenericTrackProducer<T>::minDRBadEcalChannel(const pat::IsolatedTrack &track) const
{
   double trackEta = track.eta(), trackPhi = track.phi();

   double min_dist = -1;
   DetId min_detId;

   map<DetId, vector<int> >::const_iterator bitItor;
   for(bitItor = EcalAllDeadChannelsBitMap_.begin(); bitItor != EcalAllDeadChannelsBitMap_.end(); bitItor++) {
      DetId maskedDetId = bitItor->first;
      map<DetId, std::vector<double> >::const_iterator valItor = EcalAllDeadChannelsValMap_.find(maskedDetId);
      if(valItor == EcalAllDeadChannelsValMap_.end()){ 
        cout << "Error cannot find maskedDetId in EcalAllDeadChannelsValMap_ ?!" << endl;
        continue;
      }

      double eta = (valItor->second)[0], phi = (valItor->second)[1];
      double dist = reco::deltaR(eta, phi, trackEta, trackPhi);

      if(min_dist > dist || min_dist < 0) {
        min_dist = dist;
        min_detId = maskedDetId;
      }
   }

   return min_dist;
}

template<class T> const unsigned int
OSUGenericTrackProducer<T>::isTagProbeElePair(const pat::IsolatedTrack &probe, const pat::Electron &tag) const 
{
  TLorentzVector t(tag.px(), tag.py(), tag.pz(), tag.energy());
  TLorentzVector p(probe.px(), 
                   probe.py(), 
                   probe.pz(), 
                   sqrt(probe.px() * probe.px() + 
                        probe.py() * probe.py() + 
                        probe.pz() * probe.pz() + 
                        0.000510998928 * 0.000510998928)); // energyOfElectron()

  if(fabs((t + p).M() - 91.1876) >= 10.0) return 0b00;
  return (tag.charge() * probe.charge() < 0) ? 0b01 : 0b10;
}

template<class T> const unsigned int
OSUGenericTrackProducer<T>::isTagProbeTauToElePair(const pat::IsolatedTrack &probe, 
                                                  const pat::Electron &tag, 
                                                  const pat::MET &met) const 
{
  double dPhi = deltaPhi(tag.phi(), probe.phi());
  if(sqrt(2.0 * tag.pt() * probe.pt() * (1 - cos(dPhi))) >= 40) return false; // cutElectronLowMT

  TLorentzVector t(tag.px(), tag.py(), tag.pz(), tag.energy());
  TLorentzVector p(probe.px(), 
                   probe.py(), 
                   probe.pz(), 
                   sqrt(probe.px() * probe.px() + 
                        probe.py() * probe.py() + 
                        probe.pz() * probe.pz() + 
                        0.000510998928 * 0.000510998928)); // energyOfElectron()

  double invMass = (t + p).M();

  if(invMass <= 91.1876 - 50 || invMass >= 91.1876 - 15) return 0b00;
  return (tag.charge() * probe.charge() < 0) ? 0b01 : 0b10;
}

template<class T> const unsigned int
OSUGenericTrackProducer<T>::isTagProbeMuonPair(const pat::IsolatedTrack &probe, const pat::Muon &tag) const 
{
  TLorentzVector t(tag.px(), tag.py(), tag.pz(), tag.energy());
  TLorentzVector p(probe.px(), 
                   probe.py(), 
                   probe.pz(), 
                   sqrt(probe.px() * probe.px() + 
                        probe.py() * probe.py() + 
                        probe.pz() * probe.pz() + 
                        0.1056583715 * 0.1056583715)); // energyOfMuon()

  if(fabs((t + p).M() - 91.1876) >= 10.0) return 0b00;
  return (tag.charge() * probe.charge() < 0) ? 0b01 : 0b10;
}

template<class T>const unsigned int
OSUGenericTrackProducer<T>::isTagProbeTauToMuonPair(const pat::IsolatedTrack &probe, 
                                                   const pat::Muon &tag, 
                                                   const pat::MET &met) const 
{
  double dPhi = deltaPhi(tag.phi(), probe.phi());
  if(sqrt(2.0 * tag.pt() * probe.pt() * (1 - cos(dPhi))) >= 40) return false; // cutMuonLowMT

  TLorentzVector t(tag.px(), tag.py(), tag.pz(), tag.energy());
  TLorentzVector p(probe.px(), 
                   probe.py(), 
                   probe.pz(), 
                   sqrt(probe.px() * probe.px() + 
                        probe.py() * probe.py() + 
                        probe.pz() * probe.pz() + 
                        0.1056583715 * 0.1056583715)); // energyOfMuon()

  double invMass = (t + p).M();

  if(invMass <= 91.1876 - 50 || invMass >= 91.1876 - 15) return 0b00;
  return (tag.charge() * probe.charge() < 0) ? 0b01 : 0b10;
}

template<class T> void 
OSUGenericTrackProducer<T>::getRecHits(const edm::Event &event)
{
  recHitInfos_.clear();

  edm::Handle<EBRecHitCollection> EBRecHits;
  event.getByToken(EBRecHitsToken_, EBRecHits);
  for(const auto &hit : *EBRecHits) {
    GlobalPoint pos = getPosition(hit.detid());
    recHitInfos_.push_back(RecHitInfo(pos.eta(), pos.phi(), hit.energy(), -999., DetType::EB));

    for(auto &info : trackInfos_) {
      double dR2 = deltaR2(pos.eta(), pos.phi(), info.eta, info.phi);
      if(dR2 < 0.5*0.5) info.ecalo += hit.energy();
    }
  }

  edm::Handle<EERecHitCollection> EERecHits;
  event.getByToken(EERecHitsToken_, EERecHits);
  for(const auto &hit : *EERecHits) {
    GlobalPoint pos = getPosition(hit.detid());
    recHitInfos_.push_back(RecHitInfo(pos.eta(), pos.phi(), hit.energy(), -999., DetType::EE));

    for(auto &info : trackInfos_) {
      double dR2 = deltaR2(pos.eta(), pos.phi(), info.eta, info.phi);
      if(dR2 < 0.5*0.5) info.ecalo += hit.energy();
    }
  }

  /*edm::Handle<ESRecHitCollection> ESRecHits;
  event.getByToken(ESRecHitsToken_, ESRecHits);
  for(const auto &hit : *ESRecHits) {
    math::XYZVector pos = getPosition(hit.detid());
    recHitInfos_.push_back(RecHitInfo(pos.eta(), pos.phi(), hit.energy(), -999., DetType::ES));
  }*/

  edm::Handle<HBHERecHitCollection> HBHERecHits;
  event.getByToken(HBHERecHitsToken_, HBHERecHits);
  for(const auto &hit : *HBHERecHits) {
    GlobalPoint pos = getPosition(hit.detid());
    recHitInfos_.push_back(RecHitInfo(pos.eta(), pos.phi(), hit.energy(), -999., DetType::HCAL));

    for(auto &info : trackInfos_) {
      double dR2 = deltaR2(pos.eta(), pos.phi(), info.eta, info.phi);
      if(dR2 < 0.5*0.5) info.ecalo += hit.energy();
    }
  }
}

template<class T> int
OSUGenericTrackProducer<T>::getDetectorIndex(const int detectorIndex) const
{
  if(detectorIndex == DetType::EB or detectorIndex == DetType::EE)
  {
    return 0;
  }
  else if (detectorIndex == DetType::HCAL)
  {
    return 1;
  }
  else if (detectorIndex >= DetType::CSC and detectorIndex <= DetType::RPC)
  {
    return 2;
  }
  else
  {
    return -1;
  }
  
}

template<class T> std::vector<std::vector<double>>
OSUGenericTrackProducer<T>::getHitMap(const vector<TrackDeDxInfo> trackDeDxInfos) const {

  //hit.hitLayerId, hit.charge, hit.subDet, hit.pixelHitSize, 
  //hit.pixelHitSizeX, hit.pixelHitSizeY, hit.stripShapeSelection, hit.hitPosX, hit.hitPosY]

  int hitsWanted = 16;
  int numHitVar = 4; //9 for full
  int numHits = 0;
  bool newLayer = true;
  std::vector<std::vector<double>> layerHits(hitsWanted, vector<double>(numHitVar));

  for(auto &hit : trackDeDxInfos){

    newLayer = true;

    //std::cout << "Get Hit Map, sub detector: " << hit.subDet << ", layer: " << hit.hitLayerId << ", energy: " << hit.charge << ", num hits: " << numHits<< std::endl;

    //if(numHits == hitsWanted-1) break; //check if greater hits then break

    for(int i=0; i < numHits; i++){
      //check to see if this detector/layer already has a hit
      if(hit.subDet == layerHits[i][2] && hit.hitLayerId == layerHits[i][0]){
        newLayer = false;
        //check if the hit has more energy, want to have max energy hit in detector/layer
        if(hit.charge > layerHits[i][1]){
          /*std::vector<double> thisLayer = {(double)hit.hitLayerId, (double)hit.charge, (double)hit.subDet, (double)hit.pixelHitSize, (double)hit.pixelHitSizeX, (double)hit.pixelHitSizeY,
                                          (double)hit.stripShapeSelection, (double)hit.hitPosX, (double)hit.hitPosY};*/
          std::vector<double> thisLayer = {(double)hit.charge, (double)hit.pixelHitSize, (double)hit.pixelHitSizeX, (double)hit.pixelHitSizeY};
          layerHits[i] = thisLayer;
        }
        break;
      }
    } //end loop over existing hits
    if(newLayer && numHits < hitsWanted-1){
      /*std::vector<double> thisLayer = {(double)hit.hitLayerId, (double)hit.charge, (double)hit.subDet, (double)hit.pixelHitSize, (double)hit.pixelHitSizeX, (double)hit.pixelHitSizeY,
                                      (double)hit.stripShapeSelection, (double)hit.hitPosX, (double)hit.hitPosY};*/
      std::vector<double> thisLayer = {(double)hit.charge, (double)hit.pixelHitSize, (double)hit.pixelHitSizeX, (double)hit.pixelHitSizeY};

      layerHits[numHits] = thisLayer;
      numHits++;
    }
    
  }//end loop over dedxInfos

  return layerHits;

}

template<class T> std::pair<double, double>
OSUGenericTrackProducer<T>::getMaxHits(const vector<TrackDeDxInfo> trackDeDxInfos) const {

  double maxEnergy = 0;
  double totalEnergy = 0;

  for(auto &hit : trackDeDxInfos){
    totalEnergy += hit.charge;
    if(hit.charge > maxEnergy) maxEnergy = hit.charge;
  }

  std::pair<double, double> hitEnergy(maxEnergy, totalEnergy);
  return hitEnergy;
}

//std::bitset<29> 
template<class T> unsigned long
OSUGenericTrackProducer<T>::encodeLayers(const std::vector<std::vector<double>> layerHits) const {
  
  //number of layers in each sub detector {pbx, pex, TIB, TOB, TID, TEC}
  int numLayers[6] = {4, 3, 4, 6, 3, 9};

  std::bitset<29> encodedHits;


  for(unsigned int i=0; i < layerHits.size(); i++){
    
    int thisLayer = layerHits[i][0];
    int thisDet = layerHits[i][2];
    int bit = thisLayer-1;

    for(int det = 0; det < thisDet-1; det++){
      bit += numLayers[det];
    }

    std::bitset<29> thisHit = 1<<bit;
    encodedHits = encodedHits | thisHit;

  }

  unsigned long encodedHitsInt = encodedHits.to_ulong();

  return encodedHitsInt;

}

template<class T> std::pair<std::array<double, 3>, std::array<double, 3>>
OSUGenericTrackProducer<T>::getClosestVertices(const std::vector<VertexInfo> v_info, float track_vz, float track_vx, float track_vy) const {

  std::array<double, 3> d0 {{10e3, 10e3, 10e3}};
  std::array<double, 3> dz {{10e3, 10e3, 10e3}};

  for(auto &info : v_info){
    float deltaZ = abs(info.vertex.Z() - track_vz);
    float delta0 = abs(sqrt(pow(info.vertex.X()-track_vx,2) + pow(info.vertex.Y()-track_vy,2)));
    //std::cout << "delta0: " << delta0 << " , deltaZ: " << deltaZ << std::endl;
    int size = sizeof(dz)/sizeof(dz[0]);
    //std::cout << "size: " << size << std::endl;
    if(abs(deltaZ) < abs(dz[size-1])) dz[size-1] = deltaZ;
    std::sort(dz.begin(), dz.end(), [](float i, float j){ return abs(i) < abs(j); });
    if(abs(delta0) < abs(d0[size-1])) d0[size-1] = delta0;
    std::sort(d0.begin(), d0.end(), [](float i, float j){ return abs(i) < abs(j); });
    //std::cout << "d0: " << d0[0] << " " << d0[1] << " " << d0[2] << ", dz: " << dz[0] << " " << dz[1] << " " << dz[2] << std::endl;
  }

  std::pair<std::array<double, 3>, std::array<double, 3>> closestVertices(dz, d0);
  return closestVertices;

}

#include "FWCore/Framework/interface/MakerMacros.h"
typedef OSUGenericTrackProducer<osu::Track> OSUTrackProducer;
DEFINE_FWK_MODULE(OSUTrackProducer);

#if IS_VALID(secondaryTracks)
typedef OSUGenericTrackProducer<osu::SecondaryTrack> OSUSecondaryTrackProducer;
DEFINE_FWK_MODULE(OSUSecondaryTrackProducer);
#endif

#endif // IS_VALID(tracks)
