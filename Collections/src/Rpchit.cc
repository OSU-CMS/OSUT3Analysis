#include "OSUT3Analysis/Collections/interface/Rpchit.h"

#if IS_VALID(rpchits)

osu::Rpchit::Rpchit ()
{
}

osu::Rpchit::Rpchit (const TYPE(rpchits) &rpchit) :
  TYPE(rpchits) (rpchit)
{
}

osu::Rpchit::~Rpchit ()
{
}

#endif
