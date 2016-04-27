#include "OSUT3Analysis/Collections/interface/Cscseg.h"

#if IS_VALID(cscsegs)

osu::Cscseg::Cscseg ()
{
}

osu::Cscseg::Cscseg (const TYPE(cscsegs) &cscseg) :
  TYPE(cscsegs) (cscseg)
{
}

osu::Cscseg::~Cscseg ()
{
}

#endif
