#include "OSUT3Analysis/Collections/interface/Trigobj.h"

#if IS_VALID(trigobjs)

osu::Trigobj::Trigobj ()
{
}

osu::Trigobj::Trigobj (const TYPE(trigobjs) &trigobj) :
  GenMatchable (trigobj)
{
}

osu::Trigobj::Trigobj (const TYPE(trigobjs) &trigobj, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (trigobj, particles)
{
}

osu::Trigobj::Trigobj (const TYPE(trigobjs) &trigobj, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable (trigobj, particles, cfg)
{
}

osu::Trigobj::~Trigobj ()
{
}

#endif
