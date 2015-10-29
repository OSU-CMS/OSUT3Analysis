#include "OSUT3Analysis/Collections/interface/Trigobj.h"

#if IS_VALID(trigobjs)

osu::Trigobj::Trigobj ()
{
}

osu::Trigobj::Trigobj (const TYPE(trigobjs) &trigobj) :
  TYPE(trigobjs) (trigobj)
{
}

osu::Trigobj::~Trigobj ()
{
}

#endif
