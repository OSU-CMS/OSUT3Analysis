#include "OSUT3Analysis/Collections/interface/Tau.h"

#if IS_VALID(taus)

osu::Tau::Tau ()
{
}

osu::Tau::Tau (const TYPE(taus) &tau) :
  GenMatchable (tau)
{
}

osu::Tau::Tau (const TYPE(taus) &tau, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (tau, particles)
{
}

osu::Tau::Tau (const TYPE(taus) &tau, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable (tau, particles, cfg)
{
}

osu::Tau::~Tau ()
{
}

#endif
