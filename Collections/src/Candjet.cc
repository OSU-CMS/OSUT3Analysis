#include "OSUT3Analysis/Collections/interface/Candjet.h"

#if IS_VALID(candjets)

osu::Candjet::Candjet ()
{
}

osu::Candjet::Candjet (const TYPE(candjets) &candjet) :
  GenMatchable (candjet)
{
}

osu::Candjet::Candjet (const TYPE(candjets) &candjet, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (candjet, particles)
{
}

osu::Candjet::~Candjet ()
{
}

#endif
