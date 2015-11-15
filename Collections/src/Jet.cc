#include "OSUT3Analysis/Collections/interface/Jet.h"

#if IS_VALID(jets)

osu::Jet::Jet ()
{
}

osu::Jet::Jet (const TYPE(jets) &jet) :
  GenMatchable (jet)
{
}

osu::Jet::Jet (const TYPE(jets) &jet, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (jet, particles)
{
}

osu::Jet::Jet (const TYPE(jets) &jet, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable (jet, particles, cfg)
{
}

osu::Jet::~Jet ()
{
}

#endif
