#include "OSUT3Analysis/Collections/interface/Mcparticle.h"

#if IS_VALID(mcparticles)

osu::Mcparticle::Mcparticle ()
{
}

osu::Mcparticle::Mcparticle (const TYPE(mcparticles) &mcparticle) :
  TYPE(mcparticles) (mcparticle)
{
}

osu::Mcparticle::~Mcparticle ()
{
}

#endif
