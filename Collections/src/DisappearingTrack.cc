#ifdef DISAPP_TRKS

#include "OSUT3Analysis/Collections/interface/DisappearingTrack.h"

#if IS_VALID(tracks)

osu::DisappearingTrack::DisappearingTrack() : 
  TrackBase()
{
}

osu::DisappearingTrack::DisappearingTrack(const TYPE(tracks) &track) : 
  TrackBase(track)
{
}

osu::DisappearingTrack::DisappearingTrack(const TYPE(tracks) &track, 
                                          const edm::Handle<vector<osu::Mcparticle> > &particles) :
  TrackBase (track, particles)
{
}

osu::DisappearingTrack::DisappearingTrack(const TYPE(tracks) &track, 
                                          const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  TrackBase(track, particles, cfg)
{
}

osu::DisappearingTrack::DisappearingTrack(const TYPE(tracks) &track, 
                                          const edm::Handle<vector<osu::Mcparticle> > &particles, 
                                          const edm::ParameterSet &cfg, 
                                          const edm::Handle<vector<reco::GsfTrack> > &gsfTracks, 
                                          const EtaPhiList &electronVetoList, 
                                          const EtaPhiList &muonVetoList) :
  TrackBase(track, particles, cfg, gsfTracks, electronVetoList, muonVetoList)
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
  TrackBase(track, particles, pfCandidates, jets, cfg, gsfTracks, electronVetoList, muonVetoList, EcalAllDeadChannelsValMap, EcalAllDeadChannelsBitMap, dropHits)
{
}

// the DisappTrks constructor
osu::DisappearingTrack::DisappearingTrack (const TYPE(tracks) &track,
                   const edm::Handle<vector<osu::Mcparticle> > &particles,
                   const edm::Handle<vector<pat::PackedCandidate> > &pfCandidates,
                   const edm::Handle<vector<TYPE(jets)> > &jets,
                   const edm::ParameterSet &cfg,
                   const edm::Handle<vector<reco::GsfTrack> > &gsfTracks,
                   const EtaPhiList &electronVetoList,
                   const EtaPhiList &muonVetoList,
                   const map<DetId, vector<double> > * const EcalAllDeadChannelsValMap,
                   const map<DetId, vector<int> > * const EcalAllDeadChannelsBitMap,
                   const bool dropHits,
                   const edm::Handle<vector<CandidateTrack> > &candidateTracks) :
  TrackBase(track, particles, pfCandidates, jets, cfg, gsfTracks, electronVetoList, muonVetoList, EcalAllDeadChannelsValMap, EcalAllDeadChannelsBitMap, dropHits)
{
  eleVtx_d0Cuts_barrel_ = cfg.getParameter<vector<double> > ("eleVtx_d0Cuts_barrel");
  eleVtx_dzCuts_barrel_ = cfg.getParameter<vector<double> > ("eleVtx_dzCuts_barrel");
  eleVtx_d0Cuts_endcap_ = cfg.getParameter<vector<double> > ("eleVtx_d0Cuts_endcap");
  eleVtx_dzCuts_endcap_ = cfg.getParameter<vector<double> > ("eleVtx_dzCuts_endcap");

  assert(eleVtx_d0Cuts_barrel_.size() == 4);
  assert(eleVtx_dzCuts_barrel_.size() == 4);
  assert(eleVtx_d0Cuts_endcap_.size() == 4);
  assert(eleVtx_dzCuts_endcap_.size() == 4);

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
osu::Track::findMatchedCandidateTrack (const edm::Handle<vector<CandidateTrack> > &candidateTracks, edm::Ref<vector<CandidateTrack> > &matchedCandidateTrack, double &dRToMatchedCandidateTrack) const
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
osu::Track::set_minDeltaRToElectrons (const edm::Handle<edm::View<TYPE(electrons)> > &electrons,
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
osu::Track::set_minDeltaRToMuons(const edm::Handle<vector<TYPE(muons)> > &muons, const edm::Handle<vector<TYPE(primaryvertexs)> > &vertices) 
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
osu::Track::set_minDeltaRToTaus(const edm::Handle<vector<TYPE(taus)> > &taus) 
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
#endif // IS_VALID(secondaryTracks)

#endif // IS_VALID(tracks)

#endif // ifdef DISAPP_TRKS
