#include "OSUT3Analysis/Collections/interface/Primaryvertex.h"

#if IS_VALID(primaryvertexs)

osu::Primaryvertex::Primaryvertex ()
{
}

osu::Primaryvertex::Primaryvertex (const TYPE(primaryvertexs) &primaryvertex) :
  TYPE(primaryvertexs) (primaryvertex)
{
}

osu::Primaryvertex::~Primaryvertex ()
{
}

#endif
