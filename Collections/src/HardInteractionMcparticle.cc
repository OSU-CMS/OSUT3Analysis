#include "OSUT3Analysis/Collections/interface/HardInteractionMcparticle.h"

#if IS_VALID(hardInteractionMcparticles)

osu::HardInteractionMcparticle::HardInteractionMcparticle ()
{
}

osu::HardInteractionMcparticle::HardInteractionMcparticle (const TYPE(hardInteractionMcparticles) &hardInteractionMcparticle) :
  TYPE(hardInteractionMcparticles) (hardInteractionMcparticle),
  motherPdgId_                     (INVALID_VALUE),
  motherStatus_                    (INVALID_VALUE)
{
}

const int
osu::HardInteractionMcparticle::motherPdgId () const
{
  return motherPdgId_;
}

const int
osu::HardInteractionMcparticle::motherStatus () const
{
  return motherStatus_;
}

osu::HardInteractionMcparticle::~HardInteractionMcparticle ()
{
}

#endif
