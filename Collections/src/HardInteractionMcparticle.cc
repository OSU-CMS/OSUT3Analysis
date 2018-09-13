#include "OSUT3Analysis/Collections/interface/HardInteractionMcparticle.h"

#if IS_VALID(hardInteractionMcparticles)

osu::HardInteractionMcparticle::HardInteractionMcparticle ()
{
}

osu::HardInteractionMcparticle::HardInteractionMcparticle (const TYPE(hardInteractionMcparticles) &hardInteractionMcparticle) :
  TYPE(hardInteractionMcparticles) (hardInteractionMcparticle)
{
}

osu::HardInteractionMcparticle::~HardInteractionMcparticle ()
{
}

#endif
