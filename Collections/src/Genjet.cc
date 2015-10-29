#include "OSUT3Analysis/Collections/interface/Genjet.h"

#if IS_VALID(genjets)

osu::Genjet::Genjet ()
{
}

osu::Genjet::Genjet (const TYPE(genjets) &genjet) :
  TYPE(genjets) (genjet)
{
}

osu::Genjet::~Genjet ()
{
}

#endif
