#include "OSUT3Analysis/Collections/interface/Basicjet.h"

#if IS_VALID(basicjets)

osu::Basicjet::Basicjet ()
{
}

osu::Basicjet::Basicjet (const TYPE(basicjets) &basicjet) :
  GenMatchable (basicjet)
{
}

osu::Basicjet::Basicjet (const TYPE(basicjets) &basicjet, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (basicjet, particles)
{
}

osu::Basicjet::~Basicjet ()
{
}

#endif
