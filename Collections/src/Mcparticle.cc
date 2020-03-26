#include "OSUT3Analysis/Collections/interface/Mcparticle.h"

#if IS_VALID(mcparticles)

osu::Mcparticle::Mcparticle ()
{
}

osu::Mcparticle::Mcparticle (const TYPE(mcparticles) &mcparticle) :
  TYPE(mcparticles)  (mcparticle),
  uniqueMotherPdgId_ (INVALID_VALUE)
{
}

const int
osu::Mcparticle::uniqueMotherPdgId () const
{
  return uniqueMotherPdgId_;
}

osu::Mcparticle::~Mcparticle ()
{
}

#endif
