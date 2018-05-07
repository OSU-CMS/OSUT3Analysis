#include "OSUT3Analysis/Collections/interface/Muon.h"

#if IS_VALID(muons)

#ifndef STOPPPED_PTLS

osu::Muon::Muon ()
{
  isTightMuonWRTVtx_ = false;
}

osu::Muon::Muon (const TYPE(muons) &muon) :
  GenMatchable             (muon),
  isTightMuonWRTVtx_       (false),
  pfdBetaIsoCorr_          (INVALID_VALUE),
  sumChargedHadronPtCorr_  (INVALID_VALUE),
  muonPVIndex_             (INVALID_VALUE),
  sumPUPtCorr_             (INVALID_VALUE),
  genD0_                   (INVALID_VALUE),
  genPt_                   (INVALID_VALUE),
  genEta_                  (INVALID_VALUE),
  genPhi_                  (INVALID_VALUE),
  d0_                      (INVALID_VALUE),
  d0Sig_                   (INVALID_VALUE),
  dz_                      (INVALID_VALUE),
  metMinusOnePt_           (INVALID_VALUE),
  metMinusOnePx_           (INVALID_VALUE),
  metMinusOnePy_           (INVALID_VALUE),
  metMinusOnePhi_          (INVALID_VALUE),
  metNoMuMinusOnePt_       (INVALID_VALUE),
  metNoMuMinusOnePx_       (INVALID_VALUE),
  metNoMuMinusOnePy_       (INVALID_VALUE),
  metNoMuMinusOnePhi_      (INVALID_VALUE),
  match_HLT_IsoMu24_v_     (false),
  match_HLT_IsoTkMu24_v_   (false),
  match_HLT_IsoMu20_v_     (false),
  match_HLT_IsoTkMu20_v_   (false)
{
}

osu::Muon::Muon (const TYPE(muons) &muon, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable             (muon, particles),
  isTightMuonWRTVtx_       (false),
  pfdBetaIsoCorr_          (INVALID_VALUE),
  sumChargedHadronPtCorr_  (INVALID_VALUE),
  muonPVIndex_             (INVALID_VALUE),
  sumPUPtCorr_             (INVALID_VALUE),
  genD0_                   (INVALID_VALUE),
  genPt_                   (INVALID_VALUE),
  genEta_                  (INVALID_VALUE),
  genPhi_                  (INVALID_VALUE),
  d0_                      (INVALID_VALUE),
  d0Sig_                   (INVALID_VALUE),
  dz_                      (INVALID_VALUE),
  metMinusOnePt_           (INVALID_VALUE),
  metMinusOnePx_           (INVALID_VALUE),
  metMinusOnePy_           (INVALID_VALUE),
  metMinusOnePhi_          (INVALID_VALUE),
  metNoMuMinusOnePt_       (INVALID_VALUE),
  metNoMuMinusOnePx_       (INVALID_VALUE),
  metNoMuMinusOnePy_       (INVALID_VALUE),
  metNoMuMinusOnePhi_      (INVALID_VALUE),
  match_HLT_IsoMu24_v_     (false),
  match_HLT_IsoTkMu24_v_   (false),
  match_HLT_IsoMu20_v_     (false),
  match_HLT_IsoTkMu20_v_   (false)
{
}

osu::Muon::Muon (const TYPE(muons) &muon, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable             (muon, particles, cfg),
  isTightMuonWRTVtx_       (false),
  pfdBetaIsoCorr_          (INVALID_VALUE),
  sumChargedHadronPtCorr_  (INVALID_VALUE),
  muonPVIndex_             (INVALID_VALUE),
  sumPUPtCorr_             (INVALID_VALUE),
  genD0_                   (INVALID_VALUE),
  genPt_                   (INVALID_VALUE),
  genEta_                  (INVALID_VALUE),
  genPhi_                  (INVALID_VALUE),
  d0_                      (INVALID_VALUE),
  d0Sig_                   (INVALID_VALUE),
  dz_                      (INVALID_VALUE),
  metMinusOnePt_           (INVALID_VALUE),
  metMinusOnePx_           (INVALID_VALUE),
  metMinusOnePy_           (INVALID_VALUE),
  metMinusOnePhi_          (INVALID_VALUE),
  metNoMuMinusOnePt_       (INVALID_VALUE),
  metNoMuMinusOnePx_       (INVALID_VALUE),
  metNoMuMinusOnePy_       (INVALID_VALUE),
  metNoMuMinusOnePhi_      (INVALID_VALUE),
  match_HLT_IsoMu24_v_     (false),
  match_HLT_IsoTkMu24_v_   (false),
  match_HLT_IsoMu20_v_     (false),
  match_HLT_IsoTkMu20_v_   (false)
{
}

osu::Muon::Muon (const TYPE(muons) &muon, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg, const osu::Met &met) :
  GenMatchable             (muon, particles, cfg),
  isTightMuonWRTVtx_       (false),
  pfdBetaIsoCorr_          (INVALID_VALUE),
  sumChargedHadronPtCorr_  (INVALID_VALUE),
  muonPVIndex_             (INVALID_VALUE),
  sumPUPtCorr_             (INVALID_VALUE),
  genD0_                   (INVALID_VALUE),
  genPt_                   (INVALID_VALUE),
  genEta_                  (INVALID_VALUE),
  genPhi_                  (INVALID_VALUE),
  d0_                      (INVALID_VALUE),
  d0Sig_                   (INVALID_VALUE),
  dz_                      (INVALID_VALUE),
  metMinusOnePt_           (INVALID_VALUE),
  metMinusOnePx_           (INVALID_VALUE),
  metMinusOnePy_           (INVALID_VALUE),
  metMinusOnePhi_          (INVALID_VALUE),
  metNoMuMinusOnePt_       (INVALID_VALUE),
  metNoMuMinusOnePx_       (INVALID_VALUE),
  metNoMuMinusOnePy_       (INVALID_VALUE),
  metNoMuMinusOnePhi_      (INVALID_VALUE),
  match_HLT_IsoMu24_v_     (false),
  match_HLT_IsoTkMu24_v_   (false),
  match_HLT_IsoMu20_v_     (false),
  match_HLT_IsoTkMu20_v_   (false)
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
osu::Muon::genD0 () const
{
  return genD0_;
}

const double
osu::Muon::genPt () const
{
  return genPt_;
}

const double
osu::Muon::genEta () const
{
  return genEta_;
}

const double
osu::Muon::genPhi () const
{
  return genPhi_;
}

const double
osu::Muon::d0 () const
{
  return d0_;
}

const double
osu::Muon::d0Sig () const
{
  return d0Sig_;
}

const double
osu::Muon::dz () const
{
  return dz_;
}

void
osu::Muon::set_match_HLT_IsoMu24_v (const bool flag)
{
  match_HLT_IsoMu24_v_ = flag;
}

void
osu::Muon::set_match_HLT_IsoTkMu24_v (const bool flag)
{
  match_HLT_IsoTkMu24_v_ = flag;
}

void
osu::Muon::set_match_HLT_IsoMu20_v (const bool flag)
{
  match_HLT_IsoMu20_v_ = flag;
}

void
osu::Muon::set_match_HLT_IsoTkMu20_v (const bool flag)
{
  match_HLT_IsoTkMu20_v_ = flag;
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
osu::Muon::match_HLT_IsoMu24_v () const
{
  return match_HLT_IsoMu24_v_;
}

const bool
osu::Muon::match_HLT_IsoTkMu24_v () const
{
  return match_HLT_IsoTkMu24_v_;
}

const bool
osu::Muon::match_HLT_IsoMu20_v () const
{
  return match_HLT_IsoMu20_v_;
}

const bool
osu::Muon::match_HLT_IsoTkMu20_v () const
{
  return match_HLT_IsoTkMu20_v_;
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
