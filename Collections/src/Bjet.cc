#include "OSUT3Analysis/Collections/interface/Bjet.h"

#if IS_VALID(bjets)

osu::Bjet::Bjet ()
{
}

osu::Bjet::Bjet (const TYPE(bjets) &bjet) :
  TYPE(bjets) (bjet)
{
}

osu::Bjet::~Bjet ()
{
}

#endif
