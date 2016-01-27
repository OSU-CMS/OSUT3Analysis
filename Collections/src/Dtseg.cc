#include "OSUT3Analysis/Collections/interface/Dtseg.h"

#if IS_VALID(dtsegs)

osu::Dtseg::Dtseg ()
{
}

osu::Dtseg::Dtseg (const TYPE(dtsegs) &dtseg) :
  TYPE(dtsegs) (dtseg)
{
}

osu::Dtseg::~Dtseg ()
{
}

#endif
