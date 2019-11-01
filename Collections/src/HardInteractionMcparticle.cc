#include "OSUT3Analysis/Collections/interface/HardInteractionMcparticle.h"

#if IS_VALID(hardInteractionMcparticles)

osu::HardInteractionMcparticle::HardInteractionMcparticle ()
{
}

osu::HardInteractionMcparticle::HardInteractionMcparticle (const TYPE(hardInteractionMcparticles) &hardInteractionMcparticle) :
  TYPE(hardInteractionMcparticles) (hardInteractionMcparticle),
  uniqueMotherPdgId_                     (INVALID_VALUE)
{
}

const int
osu::HardInteractionMcparticle::uniqueMotherPdgId () const
{
  return uniqueMotherPdgId_;
}

osu::HardInteractionMcparticle::~HardInteractionMcparticle ()
{
}

#endif
