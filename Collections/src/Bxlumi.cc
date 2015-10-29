#include "OSUT3Analysis/Collections/interface/Bxlumi.h"

#if IS_VALID(bxlumis)

osu::Bxlumi::Bxlumi ()
{
}

osu::Bxlumi::Bxlumi (const TYPE(bxlumis) &bxlumi) :
  TYPE(bxlumis) (bxlumi)
{
}

osu::Bxlumi::~Bxlumi ()
{
}

#endif
