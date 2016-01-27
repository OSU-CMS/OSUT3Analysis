#include "OSUT3Analysis/Collections/interface/Cschit.h"

#if IS_VALID(cschits)

osu::Cschit::Cschit ()
{
}

osu::Cschit::Cschit (const TYPE(cschits) &cschit) :
  TYPE(cschits) (cschit)
{
}

osu::Cschit::~Cschit ()
{
}

#endif
