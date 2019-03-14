#include "OSUT3Analysis/Collections/interface/DisappearingTrack.h"

#ifdef DISAPP_TRKS

#if IS_VALID(tracks)

osu::DisappearingTrack::DisappearingTrack() : 
  TrackBase(),
  deltaRToClosestElectron_       (INVALID_VALUE),
  deltaRToClosestVetoElectron_   (INVALID_VALUE),
  deltaRToClosestLooseElectron_  (INVALID_VALUE),
  deltaRToClosestMediumElectron_ (INVALID_VALUE),
  deltaRToClosestTightElectron_  (INVALID_VALUE),
  deltaRToClosestMuon_           (INVALID_VALUE),
  deltaRToClosestLooseMuon_      (INVALID_VALUE),
  deltaRToClosestMediumMuon_     (INVALID_VALUE),
  deltaRToClosestTightMuon_      (INVALID_VALUE),
  deltaRToClosestTau_            (INVALID_VALUE),
  deltaRToClosestTauHad_         (INVALID_VALUE),
  pfElectronIsoDR03_             (INVALID_VALUE),
  pfPUElectronIsoDR03_           (INVALID_VALUE),
  pfMuonIsoDR03_                 (INVALID_VALUE),
  pfPUMuonIsoDR03_               (INVALID_VALUE),
  pfHFIsoDR03_                   (INVALID_VALUE),
  pfPUHFIsoDR03_                 (INVALID_VALUE),
  pfLostTrackIsoDR03_            (INVALID_VALUE),
  pfPULostTrackIsoDR03_          (INVALID_VALUE),
  isoTrackIsoDR03_               (INVALID_VALUE),
  pfChHadIsoDR03_                (INVALID_VALUE),
  pfPUChHadIsoDR03_              (INVALID_VALUE),
  pfNeutralHadIsoDR03_           (INVALID_VALUE),
  pfPUNeutralHadIsoDR03_         (INVALID_VALUE),
  pfPhotonIsoDR03_               (INVALID_VALUE),
  pfPUPhotonIsoDR03_             (INVALID_VALUE)
{
}

osu::DisappearingTrack::DisappearingTrack(const TYPE(tracks) &track) : 
  TrackBase(track),
  deltaRToClosestElectron_       (INVALID_VALUE),
  deltaRToClosestVetoElectron_   (INVALID_VALUE),
  deltaRToClosestLooseElectron_  (INVALID_VALUE),
  deltaRToClosestMediumElectron_ (INVALID_VALUE),
  deltaRToClosestTightElectron_  (INVALID_VALUE),
  deltaRToClosestMuon_           (INVALID_VALUE),
  deltaRToClosestLooseMuon_      (INVALID_VALUE),
  deltaRToClosestMediumMuon_     (INVALID_VALUE),
  deltaRToClosestTightMuon_      (INVALID_VALUE),
  deltaRToClosestTau_            (INVALID_VALUE),
  deltaRToClosestTauHad_         (INVALID_VALUE),
  pfElectronIsoDR03_             (INVALID_VALUE),
  pfPUElectronIsoDR03_           (INVALID_VALUE),
  pfMuonIsoDR03_                 (INVALID_VALUE),
  pfPUMuonIsoDR03_               (INVALID_VALUE),
  pfHFIsoDR03_                   (INVALID_VALUE),
  pfPUHFIsoDR03_                 (INVALID_VALUE),
  pfLostTrackIsoDR03_            (INVALID_VALUE),
  pfPULostTrackIsoDR03_          (INVALID_VALUE),
  isoTrackIsoDR03_               (INVALID_VALUE),
  pfChHadIsoDR03_                (INVALID_VALUE),
  pfPUChHadIsoDR03_              (INVALID_VALUE),
  pfNeutralHadIsoDR03_           (INVALID_VALUE),
  pfPUNeutralHadIsoDR03_         (INVALID_VALUE),
  pfPhotonIsoDR03_               (INVALID_VALUE),
  pfPUPhotonIsoDR03_             (INVALID_VALUE)
{
}

osu::DisappearingTrack::DisappearingTrack(const TYPE(tracks) &track, 
                                          const edm::Handle<vector<osu::Mcparticle> > &particles) :
  TrackBase (track, particles),
  deltaRToClosestElectron_       (INVALID_VALUE),
  deltaRToClosestVetoElectron_   (INVALID_VALUE),
  deltaRToClosestLooseElectron_  (INVALID_VALUE),
  deltaRToClosestMediumElectron_ (INVALID_VALUE),
  deltaRToClosestTightElectron_  (INVALID_VALUE),
  deltaRToClosestMuon_           (INVALID_VALUE),
  deltaRToClosestLooseMuon_      (INVALID_VALUE),
  deltaRToClosestMediumMuon_     (INVALID_VALUE),
  deltaRToClosestTightMuon_      (INVALID_VALUE),
  deltaRToClosestTau_            (INVALID_VALUE),
  deltaRToClosestTauHad_         (INVALID_VALUE),
  pfElectronIsoDR03_             (INVALID_VALUE),
  pfPUElectronIsoDR03_           (INVALID_VALUE),
  pfMuonIsoDR03_                 (INVALID_VALUE),
  pfPUMuonIsoDR03_               (INVALID_VALUE),
  pfHFIsoDR03_                   (INVALID_VALUE),
  pfPUHFIsoDR03_                 (INVALID_VALUE),
  pfLostTrackIsoDR03_            (INVALID_VALUE),
  pfPULostTrackIsoDR03_          (INVALID_VALUE),
  isoTrackIsoDR03_               (INVALID_VALUE),
  pfChHadIsoDR03_                (INVALID_VALUE),
  pfPUChHadIsoDR03_              (INVALID_VALUE),
  pfNeutralHadIsoDR03_           (INVALID_VALUE),
  pfPUNeutralHadIsoDR03_         (INVALID_VALUE),
  pfPhotonIsoDR03_               (INVALID_VALUE),
  pfPUPhotonIsoDR03_             (INVALID_VALUE)
{
}

osu::DisappearingTrack::DisappearingTrack(const TYPE(tracks) &track, 
                                          const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  TrackBase(track, particles, cfg),
  deltaRToClosestElectron_       (INVALID_VALUE),
  deltaRToClosestVetoElectron_   (INVALID_VALUE),
  deltaRToClosestLooseElectron_  (INVALID_VALUE),
  deltaRToClosestMediumElectron_ (INVALID_VALUE),
  deltaRToClosestTightElectron_  (INVALID_VALUE),
  deltaRToClosestMuon_           (INVALID_VALUE),
  deltaRToClosestLooseMuon_      (INVALID_VALUE),
  deltaRToClosestMediumMuon_     (INVALID_VALUE),
  deltaRToClosestTightMuon_      (INVALID_VALUE),
  deltaRToClosestTau_            (INVALID_VALUE),
  deltaRToClosestTauHad_         (INVALID_VALUE),
  pfElectronIsoDR03_             (INVALID_VALUE),
  pfPUElectronIsoDR03_           (INVALID_VALUE),
  pfMuonIsoDR03_                 (INVALID_VALUE),
  pfPUMuonIsoDR03_               (INVALID_VALUE),
  pfHFIsoDR03_                   (INVALID_VALUE),
  pfPUHFIsoDR03_                 (INVALID_VALUE),
  pfLostTrackIsoDR03_            (INVALID_VALUE),
  pfPULostTrackIsoDR03_          (INVALID_VALUE),
  isoTrackIsoDR03_               (INVALID_VALUE),
  pfChHadIsoDR03_                (INVALID_VALUE),
  pfPUChHadIsoDR03_              (INVALID_VALUE),
  pfNeutralHadIsoDR03_           (INVALID_VALUE),
  pfPUNeutralHadIsoDR03_         (INVALID_VALUE),
  pfPhotonIsoDR03_               (INVALID_VALUE),
  pfPUPhotonIsoDR03_             (INVALID_VALUE)
{
}

osu::DisappearingTrack::DisappearingTrack(const TYPE(tracks) &track, 
                                          const edm::Handle<vector<osu::Mcparticle> > &particles, 
                                          const edm::ParameterSet &cfg, 
                                          const edm::Handle<vector<reco::GsfTrack> > &gsfTracks, 
                                          const EtaPhiList &electronVetoList, 
                                          const EtaPhiList &muonVetoList) :
  TrackBase(track, particles, cfg, gsfTracks, electronVetoList, muonVetoList),
  deltaRToClosestElectron_       (INVALID_VALUE),
  deltaRToClosestVetoElectron_   (INVALID_VALUE),
  deltaRToClosestLooseElectron_  (INVALID_VALUE),
  deltaRToClosestMediumElectron_ (INVALID_VALUE),
  deltaRToClosestTightElectron_  (INVALID_VALUE),
  deltaRToClosestMuon_           (INVALID_VALUE),
  deltaRToClosestLooseMuon_      (INVALID_VALUE),
  deltaRToClosestMediumMuon_     (INVALID_VALUE),
  deltaRToClosestTightMuon_      (INVALID_VALUE),
  deltaRToClosestTau_            (INVALID_VALUE),
  deltaRToClosestTauHad_         (INVALID_VALUE),
  pfElectronIsoDR03_             (INVALID_VALUE),
  pfPUElectronIsoDR03_           (INVALID_VALUE),
  pfMuonIsoDR03_                 (INVALID_VALUE),
  pfPUMuonIsoDR03_               (INVALID_VALUE),
  pfHFIsoDR03_                   (INVALID_VALUE),
  pfPUHFIsoDR03_                 (INVALID_VALUE),
  pfLostTrackIsoDR03_            (INVALID_VALUE),
  pfPULostTrackIsoDR03_          (INVALID_VALUE),
  isoTrackIsoDR03_               (INVALID_VALUE),
  pfChHadIsoDR03_                (INVALID_VALUE),
  pfPUChHadIsoDR03_              (INVALID_VALUE),
  pfNeutralHadIsoDR03_           (INVALID_VALUE),
  pfPUNeutralHadIsoDR03_         (INVALID_VALUE),
  pfPhotonIsoDR03_               (INVALID_VALUE),
  pfPUPhotonIsoDR03_             (INVALID_VALUE)
{
}

osu::DisappearingTrack::DisappearingTrack(const TYPE(tracks) &track, 
                                          const edm::Handle<vector<osu::Mcparticle> > &particles,
                                          const edm::Handle<vector<pat::PackedCandidate> > &pfCandidates, 
                                          const edm::Handle<vector<TYPE(jets)> > &jets,
                                          const edm::ParameterSet &cfg, 
                                          const edm::Handle<vector<reco::GsfTrack> > &gsfTracks, 
                                          const EtaPhiList &electronVetoList, 
                                          const EtaPhiList &muonVetoList, 
                                          const map<DetId, vector<double> > * const EcalAllDeadChannelsValMap, 
                                          const map<DetId, vector<int> > * const EcalAllDeadChannelsBitMap, 
                                          const bool dropHits) :
  TrackBase(track, particles, pfCandidates, jets, cfg, gsfTracks, electronVetoList, muonVetoList, EcalAllDeadChannelsValMap, EcalAllDeadChannelsBitMap, dropHits),
  deltaRToClosestElectron_       (INVALID_VALUE),
  deltaRToClosestVetoElectron_   (INVALID_VALUE),
  deltaRToClosestLooseElectron_  (INVALID_VALUE),
  deltaRToClosestMediumElectron_ (INVALID_VALUE),
  deltaRToClosestTightElectron_  (INVALID_VALUE),
  deltaRToClosestMuon_           (INVALID_VALUE),
  deltaRToClosestLooseMuon_      (INVALID_VALUE),
  deltaRToClosestMediumMuon_     (INVALID_VALUE),
  deltaRToClosestTightMuon_      (INVALID_VALUE),
  deltaRToClosestTau_            (INVALID_VALUE),
  deltaRToClosestTauHad_         (INVALID_VALUE),
  pfElectronIsoDR03_             (INVALID_VALUE),
  pfPUElectronIsoDR03_           (INVALID_VALUE),
  pfMuonIsoDR03_                 (INVALID_VALUE),
  pfPUMuonIsoDR03_               (INVALID_VALUE),
  pfHFIsoDR03_                   (INVALID_VALUE),
  pfPUHFIsoDR03_                 (INVALID_VALUE),
  pfLostTrackIsoDR03_            (INVALID_VALUE),
  pfPULostTrackIsoDR03_          (INVALID_VALUE),
  isoTrackIsoDR03_               (INVALID_VALUE),
  pfChHadIsoDR03_                (INVALID_VALUE),
  pfPUChHadIsoDR03_              (INVALID_VALUE),
  pfNeutralHadIsoDR03_           (INVALID_VALUE),
  pfPUNeutralHadIsoDR03_         (INVALID_VALUE),
  pfPhotonIsoDR03_               (INVALID_VALUE),
  pfPUPhotonIsoDR03_             (INVALID_VALUE)
{
}

// the DisappTrks constructor
osu::DisappearingTrack::DisappearingTrack (const TYPE(tracks) &track,
                   const edm::Handle<vector<osu::Mcparticle> > &particles,
                   const edm::Handle<vector<pat::PackedCandidate> > &pfCandidates,
                   const edm::Handle<vector<pat::PackedCandidate> > &lostTracks,
                   const edm::Handle<vector<TYPE(jets)> > &jets,
                   const edm::ParameterSet &cfg,
                   const edm::Handle<vector<reco::GsfTrack> > &gsfTracks,
                   const EtaPhiList &electronVetoList,
                   const EtaPhiList &muonVetoList,
                   const map<DetId, vector<double> > * const EcalAllDeadChannelsValMap,
                   const map<DetId, vector<int> > * const EcalAllDeadChannelsBitMap,
                   const bool dropHits,
                   const edm::Handle<vector<CandidateTrack> > &candidateTracks) :
  TrackBase(track, particles, pfCandidates, jets, cfg, gsfTracks, electronVetoList, muonVetoList, EcalAllDeadChannelsValMap, EcalAllDeadChannelsBitMap, dropHits),
  deltaRToClosestElectron_       (INVALID_VALUE),
  deltaRToClosestVetoElectron_   (INVALID_VALUE),
  deltaRToClosestLooseElectron_  (INVALID_VALUE),
  deltaRToClosestMediumElectron_ (INVALID_VALUE),
  deltaRToClosestTightElectron_  (INVALID_VALUE),
  deltaRToClosestMuon_           (INVALID_VALUE),
  deltaRToClosestLooseMuon_      (INVALID_VALUE),
  deltaRToClosestMediumMuon_     (INVALID_VALUE),
  deltaRToClosestTightMuon_      (INVALID_VALUE),
  deltaRToClosestTau_            (INVALID_VALUE),
  deltaRToClosestTauHad_         (INVALID_VALUE),
  pfElectronIsoDR03_             (INVALID_VALUE),
  pfPUElectronIsoDR03_           (INVALID_VALUE),
  pfMuonIsoDR03_                 (INVALID_VALUE),
  pfPUMuonIsoDR03_               (INVALID_VALUE),
  pfHFIsoDR03_                   (INVALID_VALUE),
  pfPUHFIsoDR03_                 (INVALID_VALUE),
  pfLostTrackIsoDR03_            (INVALID_VALUE),
  pfPULostTrackIsoDR03_          (INVALID_VALUE),
  isoTrackIsoDR03_               (INVALID_VALUE),
  pfChHadIsoDR03_                (INVALID_VALUE),
  pfPUChHadIsoDR03_              (INVALID_VALUE),
  pfNeutralHadIsoDR03_           (INVALID_VALUE),
  pfPUNeutralHadIsoDR03_         (INVALID_VALUE),
  pfPhotonIsoDR03_               (INVALID_VALUE),
  pfPUPhotonIsoDR03_             (INVALID_VALUE)
{
  eleVtx_d0Cuts_barrel_ = cfg.getParameter<vector<double> > ("eleVtx_d0Cuts_barrel");
  eleVtx_dzCuts_barrel_ = cfg.getParameter<vector<double> > ("eleVtx_dzCuts_barrel");
  eleVtx_d0Cuts_endcap_ = cfg.getParameter<vector<double> > ("eleVtx_d0Cuts_endcap");
  eleVtx_dzCuts_endcap_ = cfg.getParameter<vector<double> > ("eleVtx_dzCuts_endcap");

  assert(eleVtx_d0Cuts_barrel_.size() == 4);
  assert(eleVtx_dzCuts_barrel_.size() == 4);
  assert(eleVtx_d0Cuts_endcap_.size() == 4);
  assert(eleVtx_dzCuts_endcap_.size() == 4);

  set_primaryPFIsolations(pfCandidates);
  set_additionalPFIsolations(pfCandidates, lostTracks);

  // if the tracks collection itself is CandidateTracks, don't bother with matching this to itself
  if(cfg.getParameter<edm::ParameterSet>("collections").getParameter<edm::InputTag>("tracks").label() == "candidateTrackProducer")
    return;

  maxDeltaR_candidateTrackMatching_ = cfg.getParameter<double> ("maxDeltaRForCandidateTrackMatching");
  if(candidateTracks.isValid()) findMatchedCandidateTrack(candidateTracks, matchedCandidateTrack_, dRToMatchedCandidateTrack_);

}

osu::DisappearingTrack::~DisappearingTrack ()
{
  eleVtx_d0Cuts_barrel_.clear();
  eleVtx_dzCuts_barrel_.clear();
  eleVtx_d0Cuts_endcap_.clear();
  eleVtx_dzCuts_endcap_.clear();
}

const edm::Ref<vector<CandidateTrack> > &
osu::DisappearingTrack::findMatchedCandidateTrack (const edm::Handle<vector<CandidateTrack> > &candidateTracks, edm::Ref<vector<CandidateTrack> > &matchedCandidateTrack, double &dRToMatchedCandidateTrack) const
{
  dRToMatchedCandidateTrack = INVALID_VALUE;
  for(vector<CandidateTrack>::const_iterator candTrack = candidateTracks->begin(); candTrack != candidateTracks->end(); candTrack++) {
    double dR = deltaR(*candTrack, *this);
    if(maxDeltaR_candidateTrackMatching_ >= 0.0 && dR > maxDeltaR_candidateTrackMatching_) continue;
    if(dR < dRToMatchedCandidateTrack || dRToMatchedCandidateTrack < 0.0) {
      dRToMatchedCandidateTrack = dR;
      matchedCandidateTrack = edm::Ref<vector<CandidateTrack> >(candidateTracks, candTrack - candidateTracks->begin());
    }
  }

  return matchedCandidateTrack;
}

void 
osu::DisappearingTrack::set_minDeltaRToElectrons (const edm::Handle<edm::View<TYPE(electrons)> > &electrons,
                                                  const edm::Handle<vector<TYPE(primaryvertexs)> > &vertices,
                                                  const edm::Handle<edm::ValueMap<bool> > &vidVetoMap,
                                                  const edm::Handle<edm::ValueMap<bool> > &vidLooseMap,
                                                  const edm::Handle<edm::ValueMap<bool> > &vidMediumMap,
                                                  const edm::Handle<edm::ValueMap<bool> > &vidTightMap)
{
  deltaRToClosestElectron_       = INVALID_VALUE;
  deltaRToClosestVetoElectron_   = INVALID_VALUE;
  deltaRToClosestLooseElectron_  = INVALID_VALUE;
  deltaRToClosestMediumElectron_ = INVALID_VALUE;
  deltaRToClosestTightElectron_  = INVALID_VALUE;
  
  unsigned iEle = -1;
  double dR;

  for(const auto &ele : *electrons) {
    ++iEle;
    dR = deltaR(*this, ele);

    if(dR < deltaRToClosestElectron_ || deltaRToClosestElectron_ < 0.0) deltaRToClosestElectron_ = dR;

    bool passesVeto_dxy   = false, passesVeto_dz   = false;
    bool passesLoose_dxy  = false, passesLoose_dz  = false;
    bool passesMedium_dxy = false, passesMedium_dz = false;
    bool passesTight_dxy  = false, passesTight_dz  = false;

    // Note in below, these remain false if |eta| >= 2.5; thus an eta cut is also being applied here as intended
    double ele_d0 = fabs(ele.gsfTrack()->dxy(vertices->at(0).position()));
    double ele_dz = fabs(ele.gsfTrack()->dz(vertices->at(0).position()));

    if(fabs(ele.superCluster ()->eta()) <= 1.479) {
      passesVeto_dxy = (ele_d0 < eleVtx_d0Cuts_barrel_[0]);
      passesVeto_dz  = (ele_dz < eleVtx_dzCuts_barrel_[0]);

      passesLoose_dxy = (ele_d0 < eleVtx_d0Cuts_barrel_[1]);
      passesLoose_dz  = (ele_dz < eleVtx_dzCuts_barrel_[1]);

      passesMedium_dxy = (ele_d0 < eleVtx_d0Cuts_barrel_[2]);
      passesMedium_dz  = (ele_dz < eleVtx_dzCuts_barrel_[2]);

      passesTight_dxy = (ele_d0 < eleVtx_d0Cuts_barrel_[3]);
      passesTight_dz  = (ele_dz < eleVtx_dzCuts_barrel_[3]);
    }
    else if(fabs(ele.superCluster()->eta()) < 2.5) {
      passesVeto_dxy = (ele_d0 < eleVtx_d0Cuts_endcap_[0]);
      passesVeto_dz  = (ele_dz < eleVtx_dzCuts_endcap_[0]);

      passesLoose_dxy = (ele_d0 < eleVtx_d0Cuts_endcap_[1]);
      passesLoose_dz  = (ele_dz < eleVtx_dzCuts_endcap_[1]);

      passesMedium_dxy = (ele_d0 < eleVtx_d0Cuts_endcap_[2]);
      passesMedium_dz  = (ele_dz < eleVtx_dzCuts_endcap_[2]);

      passesTight_dxy = (ele_d0 < eleVtx_d0Cuts_endcap_[3]);
      passesTight_dz  = (ele_dz < eleVtx_dzCuts_endcap_[3]);
    }

    if((*vidVetoMap)  [(*electrons).refAt(iEle)] &&
       passesVeto_dxy &&
       passesVeto_dz &&
       (dR < deltaRToClosestVetoElectron_   || deltaRToClosestVetoElectron_   < 0.0)) {
      deltaRToClosestVetoElectron_   = dR;
    }

    if((*vidLooseMap) [(*electrons).refAt(iEle)] &&
       passesLoose_dxy &&
       passesLoose_dz &&
       (dR < deltaRToClosestLooseElectron_  || deltaRToClosestLooseElectron_  < 0.0)) {
      deltaRToClosestLooseElectron_  = dR;
    }

    if((*vidMediumMap)[(*electrons).refAt(iEle)] &&
       passesMedium_dxy &&
       passesMedium_dz &&
       (dR < deltaRToClosestMediumElectron_ || deltaRToClosestMediumElectron_ < 0.0)) {
      deltaRToClosestMediumElectron_ = dR;
    }
    
    if((*vidTightMap) [(*electrons).refAt(iEle)] &&
       passesTight_dxy &&
       passesTight_dz &&
       (dR < deltaRToClosestTightElectron_  || deltaRToClosestTightElectron_  < 0.0)) {
      deltaRToClosestTightElectron_  = dR;
    }
  } // for electrons
}

void 
osu::DisappearingTrack::set_minDeltaRToMuons(const edm::Handle<vector<TYPE(muons)> > &muons, const edm::Handle<vector<TYPE(primaryvertexs)> > &vertices) 
{
  deltaRToClosestMuon_       = INVALID_VALUE;
  deltaRToClosestLooseMuon_  = INVALID_VALUE;
  deltaRToClosestMediumMuon_ = INVALID_VALUE;
  deltaRToClosestTightMuon_  = INVALID_VALUE;

  double dR;
  bool hasPV = (vertices.isValid() && !vertices->empty());

  for(const auto &muon : *muons) {
    dR = deltaR(*this, muon);

    if(dR < deltaRToClosestMuon_ || deltaRToClosestMuon_ < 0.0) deltaRToClosestMuon_ = dR;
    if(muon.isLooseMuon()  && (dR < deltaRToClosestLooseMuon_  || deltaRToClosestLooseMuon_  < 0.0)) deltaRToClosestLooseMuon_ = dR;
    if(muon.isMediumMuon() && (dR < deltaRToClosestMediumMuon_ || deltaRToClosestMediumMuon_ < 0.0)) deltaRToClosestMediumMuon_ = dR;
    if(hasPV && 
       muon.isTightMuon(vertices->at(0)) && 
       (dR < deltaRToClosestTightMuon_ || deltaRToClosestTightMuon_ < 0.0)) {
      deltaRToClosestTightMuon_ = dR;
    }
  }
}

void
osu::DisappearingTrack::set_minDeltaRToTaus(const edm::Handle<vector<TYPE(taus)> > &taus) 
{
  deltaRToClosestTau_    = INVALID_VALUE;
  deltaRToClosestTauHad_ = INVALID_VALUE;

  double dR;
  bool passesDecayModeReconstruction, passesLightFlavorRejection;

  for(const auto &tau : *taus) {
    dR = deltaR(*this, tau);

    if(dR < deltaRToClosestTau_ || deltaRToClosestTau_ < 0.0) deltaRToClosestTau_ = dR;

    passesDecayModeReconstruction = (tau.isTauIDAvailable("decayModeFinding") && tau.tauID("decayModeFinding") > 0.5);

    passesLightFlavorRejection = (tau.isTauIDAvailable("againstElectronLooseMVA5") && tau.tauID("againstElectronLooseMVA5") > 0.5);
    passesLightFlavorRejection = passesLightFlavorRejection || (tau.isTauIDAvailable("againstElectronLooseMVA6") && tau.tauID("againstElectronLooseMVA6") > 0.5);
    passesLightFlavorRejection = passesLightFlavorRejection && (tau.isTauIDAvailable("againstMuonLoose3") && tau.tauID("againstMuonLoose3") > 0.5);

    if(passesDecayModeReconstruction && passesLightFlavorRejection && (dR < deltaRToClosestTauHad_  || deltaRToClosestTauHad_  < 0.0)) {
      deltaRToClosestTauHad_ = dR;
    }
  }
}

#if DATA_FORMAT_FROM_MINIAOD && DATA_FORMAT_IS_2017
void
osu::DisappearingTrack::set_isoTrackIsolation (const edm::Handle<vector<pat::IsolatedTrack> > &isolatedTracks) {
  if(isolatedTracks.isValid()) {
    isoTrackIsoDR03_ = 0.0;

    for(const auto &isoTrk : *isolatedTracks) {
      // set_additionalPFIsolations has already calculated PF candidates (including lostTracks)
      // so we need only tracks solely from generalTracks which have a null packedCandRef
      if(isoTrk.packedCandRef().isNonnull()) continue;

      // don't count this track itself
      double dR = deltaR(*this, isoTrk);
      if(dR < 1.0e-4 || dR >= 0.3) continue;

      isoTrackIsoDR03_ += isoTrk.pt();
    }

  }
}
#endif

#if DATA_FORMAT_IS_2017 // only makes sense with phase1 pixel upgrade
// This could be in TrackBase, but is fairly specialized to the disappearing tracks search
const bool
osu::DisappearingTrack::isAllowedThreeLayerHitPattern() const {
  // Allowed patterns:
  // 1) pxb 1, pxb 2, pxb 3 (missing pxb 4)
  // 2) pxb 1, pxb 2, pxb 3 (missing pxf 1)
  // 3) pxb 1, pxb 2, pxf 1 (missing pxf 2)
  // 4) pxb 1, pxf 1, pxf 2 (missing pxf 3)
  // defunct rule: pxf 1, pxf 2, pxf3 (not used because there's no pxf 4 to enforce 3 layers)

  if(this->hasValidHitInPixelBarrelLayer(1)) {
    if(this->hasValidHitInPixelBarrelLayer(2)) {
      if(this->hasValidHitInPixelBarrelLayer(3)) { // b1 b2 b3
        return (this->hasMissingHitInPixelBarrelLayer(4) || this->hasMissingHitInPixelEndcapLayer(1)); // pattern 1 or 2
      }
      else { // b1 b2
        return (this->hasValidHitInPixelEndcapLayer(1) && this->hasMissingHitInPixelEndcapLayer(2)); // pattern 3
      }
    }
    else { // b1
      return (this->hasValidHitInPixelEndcapLayer(1) && this->hasValidHitInPixelEndcapLayer(2) && this->hasMissingHitInPixelEndcapLayer(3)); // pattern 4
    }
  }

  return false;
}
#endif

void
osu::DisappearingTrack::set_primaryPFIsolations (const edm::Handle<vector<pat::PackedCandidate> > &pfCandidates)
{
  if(pfCandidates.isValid()) {
    pfChHadIsoDR03_      = pfPUChHadIsoDR03_      = 0.0;
    pfNeutralHadIsoDR03_ = pfPUNeutralHadIsoDR03_ = 0.0;
    pfPhotonIsoDR03_     = pfPUPhotonIsoDR03_     = 0.0;

    for(const auto &pfCandidate : *pfCandidates) {
      int pdgid = abs(pfCandidate.pdgId());
      if(pdgid != 211 && pdgid != 130 && pdgid != 22) continue;
      
      double dR = deltaR(*this, pfCandidate);
      if(dR >= 0.3 || dR < 1.0e-4) continue;
      //don't count track itself
      //NOTE: This is different than the CandidateTrack track isolation, which uses 1.0e-12.
      //      This is necessary because we are not comparing generalTrack to generalTrack.
      //      Comparing a track to a PFcandidate introduces rounding / different-reconstruction
      //        errors that make the same object only match to ~1.0e-5

      bool fromPV = (pfCandidate.fromPV() > 1 || fabs(pfCandidate.dz()) < 0.1);

      if (pdgid == 211 && fromPV)
        pfChHadIsoDR03_+= pfCandidate.pt();
      else if (pdgid == 211)
        pfPUChHadIsoDR03_ += pfCandidate.pt();
      if (pdgid == 130 && fromPV)
        pfNeutralHadIsoDR03_ += pfCandidate.pt();
      else if (pdgid == 130)
        pfPUNeutralHadIsoDR03_ += pfCandidate.pt();        
      if (pdgid == 22 && fromPV)
        pfPhotonIsoDR03_ += pfCandidate.pt();
      else if (pdgid == 22)
        pfPUPhotonIsoDR03_ += pfCandidate.pt();
    }
  }
}

void
osu::DisappearingTrack::set_additionalPFIsolations (const edm::Handle<vector<pat::PackedCandidate> > &pfCandidates, const edm::Handle<vector<pat::PackedCandidate> > &lostTracks)
{
  // stored in pfCandidates:
  // the particle charge and pdgId: 11, 13, 22 for ele/mu/gamma, 211 for charged hadrons, 130 for neutral hadrons, 1 and 2 for hadronic and em particles in HF.
  // only 211, 130, and 22 are stored in PFIsolation object
  // here we store them ourselves
  if(pfCandidates.isValid()) {
    pfElectronIsoDR03_ = pfPUElectronIsoDR03_ = 0.0;
    pfMuonIsoDR03_     = pfPUMuonIsoDR03_     = 0.0;
    pfHFIsoDR03_       = pfPUHFIsoDR03_       = 0.0;


    for(const auto &pfCandidate : *pfCandidates) {
      int pdgid = abs(pfCandidate.pdgId());
      if(pdgid != 11 && pdgid != 13 && pdgid != 1 && pdgid != 2) continue;

      double dR = deltaR(*this, pfCandidate);
      if(dR >= 0.3 || dR < 1.0e-4) continue;
      //don't count track itself
      //NOTE: This is different than the CandidateTrack track isolation, which uses 1.0e-12.
      //      This is necessary because we are not comparing generalTrack to generalTrack.
      //      Comparing a track to a PFcandidate introduces rounding / different-reconstruction
      //        errors that make the same object only match to ~1.0e-5


      bool fromPV = (pfCandidate.fromPV() > 1 || fabs(pfCandidate.dz()) < 0.1);

      if(fromPV) {
        if(pdgid == 11) pfElectronIsoDR03_ += pfCandidate.pt();
        else if(pdgid == 13) pfMuonIsoDR03_ += pfCandidate.pt();
        else if(pdgid == 1 || pdgid == 2) pfHFIsoDR03_ += pfCandidate.pt();
      }
      else {
        if(pdgid == 11) pfPUElectronIsoDR03_ += pfCandidate.pt();
        else if(pdgid == 13) pfPUMuonIsoDR03_ += pfCandidate.pt();
        else if(pdgid == 1 || pdgid == 2) pfPUHFIsoDR03_ += pfCandidate.pt();
      }

    }
  }

  if(lostTracks.isValid()) {
    pfLostTrackIsoDR03_   = 0.0;
    pfPULostTrackIsoDR03_ = 0.0;

    for(const auto &lostTrack : *lostTracks) {

      double dR = deltaR(*this, lostTrack);
      if(dR >= 0.3 || dR < 1.0e-4) continue;
      //don't count track itself
      //NOTE: This is different than the CandidateTrack track isolation, which uses 1.0e-12.
      //      This is necessary because we are not comparing generalTrack to generalTrack.
      //      Comparing a track to a PFcandidate introduces rounding / different-reconstruction
      //        errors that make the same object only match to ~1.0e-5

      bool fromPV = (lostTrack.fromPV() > 1 || fabs(lostTrack.dz()) < 0.1);

      if(fromPV) pfLostTrackIsoDR03_ += lostTrack.pt();
      else pfPULostTrackIsoDR03_ += lostTrack.pt();
    }
  }

}

#if IS_VALID(secondaryTracks)
osu::SecondaryDisappearingTrack::SecondaryDisappearingTrack() : 
  osu::DisappearingTrack() {}

osu::SecondaryDisappearingTrack::SecondaryDisappearingTrack(const TYPE(tracks) &secondaryTrack) : 
  osu::DisappearingTrack(secondaryTrack) {}

osu::SecondaryDisappearingTrack::SecondaryDisappearingTrack(const TYPE(tracks) &secondaryTrack, 
                                                            const edm::Handle<vector<osu::Mcparticle> > &particles) : 
  osu::DisappearingTrack(secondaryTrack, particles) {}

osu::SecondaryDisappearingTrack::SecondaryDisappearingTrack(const TYPE(tracks) &secondaryTrack, 
                                                            const edm::Handle<vector<osu::Mcparticle> > &particles, 
                                                            const edm::ParameterSet &cfg) :
  osu::DisappearingTrack(secondaryTrack, particles, cfg) {}

osu::SecondaryDisappearingTrack::SecondaryDisappearingTrack(const TYPE(secondaryTracks) &secondaryTrack, 
                                                            const edm::Handle<vector<osu::Mcparticle> > &particles, 
                                                            const edm::ParameterSet &cfg, 
                                                            const edm::Handle<vector<reco::GsfTrack> > &gsfTracks, 
                                                            const EtaPhiList &electronVetoList, 
                                                            const EtaPhiList &muonVetoList) :
  osu::DisappearingTrack(secondaryTrack, particles, cfg, gsfTracks, electronVetoList, muonVetoList) {}

osu::SecondaryDisappearingTrack::SecondaryDisappearingTrack(const TYPE(tracks) &secondaryTrack, 
                                                            const edm::Handle<vector<osu::Mcparticle> > &particles, 
                                                            const edm::Handle<vector<pat::PackedCandidate> > &pfCandidates,
                                                            const edm::Handle<vector<TYPE(jets)> > &jets,
                                                            const edm::ParameterSet &cfg, 
                                                            const edm::Handle<vector<reco::GsfTrack> > &gsfTracks, 
                                                            const EtaPhiList &electronVetoList, 
                                                            const EtaPhiList &muonVetoList, 
                                                            const map<DetId, vector<double> > * const EcalAllDeadChannelsValMap, 
                                                            const map<DetId, vector<int> > * const EcalAllDeadChannelsBitMap, 
                                                            const bool dropHits) :
  osu::DisappearingTrack(secondaryTrack, particles, pfCandidates, jets, cfg, gsfTracks, electronVetoList, muonVetoList, EcalAllDeadChannelsValMap, EcalAllDeadChannelsBitMap, dropHits) {}

// the DisappTrks constructor
osu::SecondaryDisappearingTrack::SecondaryDisappearingTrack (const TYPE(tracks) &track, 
                                                             const edm::Handle<vector<osu::Mcparticle> > &particles,
                                                             const edm::Handle<vector<pat::PackedCandidate> > &pfCandidates, 
                                                             const edm::Handle<vector<pat::PackedCandidate> > &lostTracks, 
                                                             const edm::Handle<vector<TYPE(jets)> > &jets,
                                                             const edm::ParameterSet &cfg, 
                                                             const edm::Handle<vector<reco::GsfTrack> > &gsfTracks, 
                                                             const EtaPhiList &electronVetoList, 
                                                             const EtaPhiList &muonVetoList, 
                                                             const map<DetId, vector<double> > * const EcalAllDeadChannelsValMap, 
                                                             const map<DetId, vector<int> > * const EcalAllDeadChannelsBitMap, 
                                                             const bool dropHits,
                                                             const edm::Handle<vector<CandidateTrack> > &candidateTracks) :
  osu::DisappearingTrack(track, particles, pfCandidates, lostTracks, jets, cfg, gsfTracks, electronVetoList, muonVetoList, EcalAllDeadChannelsValMap, EcalAllDeadChannelsBitMap, dropHits, candidateTracks) {}

osu::SecondaryDisappearingTrack::~SecondaryDisappearingTrack ()
{
  eleVtx_d0Cuts_barrel_.clear();
  eleVtx_dzCuts_barrel_.clear();
  eleVtx_d0Cuts_endcap_.clear();
  eleVtx_dzCuts_endcap_.clear();
}

#endif // IS_VALID(secondaryTracks)

#endif // IS_VALID(tracks)

#endif // ifdef DISAPP_TRKS
