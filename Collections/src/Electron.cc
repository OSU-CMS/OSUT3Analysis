#include "OSUT3Analysis/Collections/interface/Electron.h"

#if IS_VALID(electrons)

osu::Electron::Electron ()
{
}

#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM  


osu::Electron::Electron (const reco::GsfElectron &electron) :
  GenMatchable (pat::Electron(electron)),
  vtxFitConversion_     (false),
  rho_                  (INVALID_VALUE)
{
}

osu::Electron::Electron (const reco::GsfElectron &electron, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (pat::Electron(electron), particles),
  vtxFitConversion_     (false),
  rho_                  (INVALID_VALUE)
{
}

osu::Electron::Electron (const reco::GsfElectron &electron, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable (pat::Electron(electron), particles, cfg),
  vtxFitConversion_     (false),
  rho_                  (INVALID_VALUE)
{
}

const int
osu::Electron::missingInnerHits () const
{
  return missingInnerHits_;
}

const float
osu::Electron::AEff () const
{
  return AEff_;
}

const float
osu::Electron::rho () const
{
  return this->rho_;
}

const bool
osu::Electron::vtxFitConversion () const
{
  return this->vtxFitConversion_;
}
#else

osu::Electron::Electron (const TYPE(electrons) &electron) :
  GenMatchable (electron)
{
}

osu::Electron::Electron (const TYPE(electrons) &electron, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (electron, particles)
{
}

osu::Electron::Electron (const TYPE(electrons) &electron, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable (electron, particles, cfg)
{
}
#endif

osu::Electron::~Electron ()
{
}
#endif
