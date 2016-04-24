#include "OSUT3Analysis/Collections/interface/Muon.h"

#if IS_VALID(muons)
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD
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


void osu::Muon::set_isTightMuonWRTVtx (const bool isTightMuon)
{
  isTightMuonWRTVtx_ = isTightMuon;
}

const double
osu::Muon::pfdBetaIsoCorr () const
{
  return pfdBetaIsoCorr_;
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
