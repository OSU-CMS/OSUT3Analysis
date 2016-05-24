#include "OSUT3Analysis/Collections/interface/Muon.h"

#if IS_VALID(muons)
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD
osu::Muon::Muon ()
{
  isTightMuonWRTVtx_ = false;
}

osu::Muon::Muon (const TYPE(muons) &muon) :
  GenMatchable         (muon),
  isTightMuonWRTVtx_   (false),
  pfdBetaIsoCorr_      (INVALID_VALUE),
  metMinusOnePt_       (INVALID_VALUE),
  metMinusOnePx_       (INVALID_VALUE),
  metMinusOnePy_       (INVALID_VALUE),
  metMinusOnePhi_      (INVALID_VALUE),
  metNoMuMinusOnePt_   (INVALID_VALUE),
  metNoMuMinusOnePx_   (INVALID_VALUE),
  metNoMuMinusOnePy_   (INVALID_VALUE),
  metNoMuMinusOnePhi_  (INVALID_VALUE)
{
}

osu::Muon::Muon (const TYPE(muons) &muon, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable         (muon),
  isTightMuonWRTVtx_   (false),
  pfdBetaIsoCorr_      (INVALID_VALUE),
  metMinusOnePt_       (INVALID_VALUE),
  metMinusOnePx_       (INVALID_VALUE),
  metMinusOnePy_       (INVALID_VALUE),
  metMinusOnePhi_      (INVALID_VALUE),
  metNoMuMinusOnePt_   (INVALID_VALUE),
  metNoMuMinusOnePx_   (INVALID_VALUE),
  metNoMuMinusOnePy_   (INVALID_VALUE),
  metNoMuMinusOnePhi_  (INVALID_VALUE)
{
}

osu::Muon::Muon (const TYPE(muons) &muon, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable         (muon),
  isTightMuonWRTVtx_   (false),
  pfdBetaIsoCorr_      (INVALID_VALUE),
  metMinusOnePt_       (INVALID_VALUE),
  metMinusOnePx_       (INVALID_VALUE),
  metMinusOnePy_       (INVALID_VALUE),
  metMinusOnePhi_      (INVALID_VALUE),
  metNoMuMinusOnePt_   (INVALID_VALUE),
  metNoMuMinusOnePx_   (INVALID_VALUE),
  metNoMuMinusOnePy_   (INVALID_VALUE),
  metNoMuMinusOnePhi_  (INVALID_VALUE)
{
}

osu::Muon::Muon (const TYPE(muons) &muon, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg, const osu::Met &met) :
  GenMatchable         (muon),
  isTightMuonWRTVtx_   (false),
  pfdBetaIsoCorr_      (INVALID_VALUE),
  metMinusOnePt_       (INVALID_VALUE),
  metMinusOnePx_       (INVALID_VALUE),
  metMinusOnePy_       (INVALID_VALUE),
  metMinusOnePhi_      (INVALID_VALUE),
  metNoMuMinusOnePt_   (INVALID_VALUE),
  metNoMuMinusOnePx_   (INVALID_VALUE),
  metNoMuMinusOnePy_   (INVALID_VALUE),
  metNoMuMinusOnePhi_  (INVALID_VALUE)
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


const int
osu::Muon::missingInnerHits () const
{
  return (this->innerTrack ()->hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::MISSING_INNER_HITS));
}

const int
osu::Muon::missingMiddleHits () const
{
  return (this->innerTrack ()->hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::TRACK_HITS));
}

const int
osu::Muon::missingOuterHits () const
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

#elif DATA_FORMAT == AOD_CUSTOM

osu::Muon::Muon ()
{
}

osu::Muon::Muon (const TYPE(muons) &muon) :
    TYPE(muons) (muon)
{
}
#endif

osu::Muon::~Muon ()
{
}


#endif
