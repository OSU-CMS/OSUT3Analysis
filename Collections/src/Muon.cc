#include "OSUT3Analysis/Collections/interface/Muon.h"

#if IS_VALID(muons)

#ifndef STOPPPED_PTLS

osu::Muon::Muon ()
{
  isTightMuonWRTVtx_ = false;
}

osu::Muon::Muon (const TYPE(muons) &muon) :
  GenMatchable             (muon),
  rho_                     (INVALID_VALUE),
  isTightMuonWRTVtx_       (false),
  isSoftMuonWRTVtx_        (false),
  pfdBetaIsoCorr_          (INVALID_VALUE),
  sumChargedHadronPtCorr_  (INVALID_VALUE),
  muonPVIndex_             (INVALID_VALUE),
  sumPUPtCorr_             (INVALID_VALUE),
  genVx_                   (INVALID_VALUE),
  genVy_                   (INVALID_VALUE),
  genPx_                   (INVALID_VALUE),
  genPy_                   (INVALID_VALUE),
  genD0_                   (INVALID_VALUE),
  d0SmearingVal_           (INVALID_VALUE),
  metMinusOnePt_           (INVALID_VALUE),
  metMinusOnePx_           (INVALID_VALUE),
  metMinusOnePy_           (INVALID_VALUE),
  metMinusOnePhi_          (INVALID_VALUE),
  metNoMuMinusOnePt_       (INVALID_VALUE),
  metNoMuMinusOnePx_       (INVALID_VALUE),
  metNoMuMinusOnePy_       (INVALID_VALUE),
  metNoMuMinusOnePhi_      (INVALID_VALUE)
{
}

osu::Muon::Muon (const TYPE(muons) &muon, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable             (muon, particles),
  rho_                     (INVALID_VALUE),
  isTightMuonWRTVtx_       (false),
  isSoftMuonWRTVtx_        (false),
  pfdBetaIsoCorr_          (INVALID_VALUE),
  sumChargedHadronPtCorr_  (INVALID_VALUE),
  muonPVIndex_             (INVALID_VALUE),
  sumPUPtCorr_             (INVALID_VALUE),
  genVx_                   (INVALID_VALUE),
  genVy_                   (INVALID_VALUE),
  genPx_                   (INVALID_VALUE),
  genPy_                   (INVALID_VALUE),
  genD0_                   (INVALID_VALUE),
  d0SmearingVal_           (INVALID_VALUE),
  metMinusOnePt_           (INVALID_VALUE),
  metMinusOnePx_           (INVALID_VALUE),
  metMinusOnePy_           (INVALID_VALUE),
  metMinusOnePhi_          (INVALID_VALUE),
  metNoMuMinusOnePt_       (INVALID_VALUE),
  metNoMuMinusOnePx_       (INVALID_VALUE),
  metNoMuMinusOnePy_       (INVALID_VALUE),
  metNoMuMinusOnePhi_      (INVALID_VALUE)
{
}

osu::Muon::Muon (const TYPE(muons) &muon, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable             (muon, particles, cfg),
  rho_                     (INVALID_VALUE),
  isTightMuonWRTVtx_       (false),
  isSoftMuonWRTVtx_        (false),
  pfdBetaIsoCorr_          (INVALID_VALUE),
  sumChargedHadronPtCorr_  (INVALID_VALUE),
  muonPVIndex_             (INVALID_VALUE),
  sumPUPtCorr_             (INVALID_VALUE),
  genVx_                   (INVALID_VALUE),
  genVy_                   (INVALID_VALUE),
  genPx_                   (INVALID_VALUE),
  genPy_                   (INVALID_VALUE),
  genD0_                   (INVALID_VALUE),
  d0SmearingVal_           (INVALID_VALUE),
  metMinusOnePt_           (INVALID_VALUE),
  metMinusOnePx_           (INVALID_VALUE),
  metMinusOnePy_           (INVALID_VALUE),
  metMinusOnePhi_          (INVALID_VALUE),
  metNoMuMinusOnePt_       (INVALID_VALUE),
  metNoMuMinusOnePx_       (INVALID_VALUE),
  metNoMuMinusOnePy_       (INVALID_VALUE),
  metNoMuMinusOnePhi_      (INVALID_VALUE)
{
}

osu::Muon::Muon (const TYPE(muons) &muon, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg, const osu::Met &met) :
  GenMatchable             (muon, particles, cfg),
  rho_                     (INVALID_VALUE),
  isTightMuonWRTVtx_       (false),
  isSoftMuonWRTVtx_        (false),
  pfdBetaIsoCorr_          (INVALID_VALUE),
  sumChargedHadronPtCorr_  (INVALID_VALUE),
  muonPVIndex_             (INVALID_VALUE),
  sumPUPtCorr_             (INVALID_VALUE),
  genVx_                   (INVALID_VALUE),
  genVy_                   (INVALID_VALUE),
  genPx_                   (INVALID_VALUE),
  genPy_                   (INVALID_VALUE),
  genD0_                   (INVALID_VALUE),
  d0SmearingVal_           (INVALID_VALUE),
  metMinusOnePt_           (INVALID_VALUE),
  metMinusOnePx_           (INVALID_VALUE),
  metMinusOnePy_           (INVALID_VALUE),
  metMinusOnePhi_          (INVALID_VALUE),
  metNoMuMinusOnePt_       (INVALID_VALUE),
  metNoMuMinusOnePx_       (INVALID_VALUE),
  metNoMuMinusOnePy_       (INVALID_VALUE),
  metNoMuMinusOnePhi_      (INVALID_VALUE)
{
  TVector2 p (met.px () + this->px (), met.py () + this->py ()),
           pNoMu (met.noMuPx (), met.noMuPy ()); // we do not add the muon's pt
                                                 // here because the pt of all
                                                 // muons has already been
                                                 // added as invisible energy

  metMinusOnePt_ = p.Mod ();
  metMinusOnePx_ = p.Px ();
  metMinusOnePy_ = p.Py ();
  metMinusOnePhi_ = p.Phi ();

  metNoMuMinusOnePt_ = pNoMu.Mod ();
  metNoMuMinusOnePx_ = pNoMu.Px ();
  metNoMuMinusOnePy_ = pNoMu.Py ();
  metNoMuMinusOnePhi_ = pNoMu.Phi ();
}

void osu::Muon::set_isTightMuonWRTVtx (const bool isTightMuon)
{
  isTightMuonWRTVtx_ = isTightMuon;
}

void osu::Muon::set_isSoftMuonWRTVtx (const bool isSoftMuon)
{
  isSoftMuonWRTVtx_ = isSoftMuon;
}

const float
osu::Muon::rho () const
{
  return this->rho_;
}

const double
osu::Muon::pfdBetaIsoCorr () const
{
  return pfdBetaIsoCorr_;
}

const double
osu::Muon::sumChargedHadronPtCorr () const
{
  return sumChargedHadronPtCorr_;
}
const int
osu::Muon::muonPVIndex () const
{
  return muonPVIndex_;
}

const double
osu::Muon::sumPUPtCorr () const
{
  return sumPUPtCorr_;
}

const double
osu::Muon::genVx () const
{
  return genVx_;
}

const double
osu::Muon::genVy () const
{
  return genVy_;
}

const double
osu::Muon::genPx () const
{
  return genPx_;
}

const double
osu::Muon::genPy () const
{
  return genPy_;
}

const double
osu::Muon::genD0 () const
{
  return genD0_;
}

const double
osu::Muon::d0SmearingVal () const
{
  return d0SmearingVal_;
}

void
osu::Muon::set_hltMatch (const string name, const bool flag)
{
  hltMatches_[name] = flag;
}

const int
osu::Muon::missingInnerHitsFromTrackerLayersWithoutMeasurements () const
{
  return (this->innerTrack ()->hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::MISSING_INNER_HITS));
}

const int
osu::Muon::missingMiddleHitsFromTrackerLayersWithoutMeasurements () const
{
  return (this->innerTrack ()->hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::TRACK_HITS));
}

const int
osu::Muon::missingOuterHitsFromTrackerLayersWithoutMeasurements () const
{
  return (this->innerTrack ()->hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::MISSING_OUTER_HITS));
}

const bool
osu::Muon::hasValidHitInPixelBarrelLayer (const uint16_t layer) const
{

  const reco::HitPattern &p = this->innerTrack()->hitPattern();

  // Loop over TRACK_HITS
#if CMSSW_VERSION_CODE >= CMSSW_VERSION(9,4,0)
  for (int i = 0; i < p.numberOfAllHits(reco::HitPattern::TRACK_HITS); i++) {
#else
  for (int i = 0; i < p.numberOfHits(reco::HitPattern::TRACK_HITS); i++) {
#endif
    uint16_t hit = p.getHitPattern(reco::HitPattern::TRACK_HITS, i);
    if(reco::HitPattern::pixelBarrelHitFilter(hit) &&
       reco::HitPattern::getLayer(hit) == layer &&
       reco::HitPattern::validHitFilter(hit)) {
      return true;
    }
  }

  return false;
}

const bool
osu::Muon::hasValidHitInPixelEndcapLayer (const uint16_t layer) const
{

  const reco::HitPattern &p = this->innerTrack()->hitPattern();

  // Loop over TRACK_HITS
#if CMSSW_VERSION_CODE >= CMSSW_VERSION(9,4,0)
  for (int i = 0; i < p.numberOfAllHits(reco::HitPattern::TRACK_HITS); i++) {
#else
  for (int i = 0; i < p.numberOfHits(reco::HitPattern::TRACK_HITS); i++) {
#endif
    uint16_t hit = p.getHitPattern(reco::HitPattern::TRACK_HITS, i);
    if(reco::HitPattern::pixelEndcapHitFilter(hit) &&
       reco::HitPattern::getLayer(hit) == layer &&
       reco::HitPattern::validHitFilter(hit)) {
      return true;
    }
  }

  return false;
}

const int
osu::Muon::layerOfFirstValidPixelHit () const
{
  if(hasValidHitInPixelBarrelLayer1() || hasValidHitInPixelEndcapLayer1()) return 1;
  else if(hasValidHitInPixelBarrelLayer2() || hasValidHitInPixelEndcapLayer2()) return 2;
  else if(hasValidHitInPixelBarrelLayer3() || hasValidHitInPixelEndcapLayer3()) return 3;
  else if(hasValidHitInPixelBarrelLayer4()) return 4;
  else return 0;
}

const double
osu::Muon::metMinusOnePt () const
{
  return metMinusOnePt_;
}

const double
osu::Muon::metMinusOnePx () const
{
  return metMinusOnePx_;
}

const double
osu::Muon::metMinusOnePy () const
{
  return metMinusOnePy_;
}

const double
osu::Muon::metMinusOnePhi () const
{
  return metMinusOnePhi_;
}

const double
osu::Muon::metNoMuMinusOnePt () const
{
  return metNoMuMinusOnePt_;
}

const double
osu::Muon::metNoMuMinusOnePx () const
{
  return metNoMuMinusOnePx_;
}

const double
osu::Muon::metNoMuMinusOnePy () const
{
  return metNoMuMinusOnePy_;
}

const double
osu::Muon::metNoMuMinusOnePhi () const
{
  return metNoMuMinusOnePhi_;
}

const bool
osu::Muon::get_hltMatch (const string name) const
{
  return hltMatches_.find(name)->second;
}

#else // STOPPPED_PTLS

osu::Muon::Muon ()
{
}

osu::Muon::Muon (const TYPE(muons) &muon) :
    TYPE(muons) (muon)
{
}

#endif // STOPPPED_PTLS

osu::Muon::~Muon ()
{
}

#endif // #if IS_VALID(muons)
