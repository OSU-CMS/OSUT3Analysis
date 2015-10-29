#include "OSUT3Analysis/Collections/interface/Eventvariable.h"

#if IS_VALID(eventvariables)

osu::Eventvariable::Eventvariable ()
{
}

osu::Eventvariable::Eventvariable (const TYPE(eventvariables) &eventvariable) :
  TYPE(eventvariables) (eventvariable)
{
}

osu::Eventvariable::~Eventvariable ()
{
}

#endif
