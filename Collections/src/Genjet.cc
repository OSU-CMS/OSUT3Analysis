#include "OSUT3Analysis/Collections/interface/Genjet.h"

#if IS_VALID(genjets)

osu::Genjet::Genjet ()
{
}

osu::Genjet::Genjet (const TYPE(genjets) &genjet) :
  GenMatchable (genjet)
{
}

osu::Genjet::Genjet (const TYPE(genjets) &genjet, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (genjet, particles)
{
}

osu::Genjet::~Genjet ()
{
}

#endif
