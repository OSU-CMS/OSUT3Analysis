#include "OSUT3Analysis/Collections/interface/Met.h"

#if IS_VALID(mets)

osu::Met::Met ()
{
}

osu::Met::Met (const TYPE(mets) &met) :
  TYPE(mets) (met)
{
}

osu::Met::~Met ()
{
}

#endif
