#include "OSUT3Analysis/Collections/interface/Muon.h"

#if IS_VALID(muons)

osu::Muon::Muon ()
{
  isTightMuonWRTVtx_ = false;
}

osu::Muon::Muon (const TYPE(muons) &muon) :
  GenMatchable (muon),
  pfdBetaIsoCorr_             (INVALID_VALUE)
{
  isTightMuonWRTVtx_ = false;
}

osu::Muon::Muon (const TYPE(muons) &muon, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (muon),
  pfdBetaIsoCorr_             (INVALID_VALUE)
{
  isTightMuonWRTVtx_ = false;
}

osu::Muon::Muon (const TYPE(muons) &muon, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable (muon),
  pfdBetaIsoCorr_             (INVALID_VALUE)
{
  isTightMuonWRTVtx_ = false;
}

osu::Muon::~Muon ()
{
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

#endif
