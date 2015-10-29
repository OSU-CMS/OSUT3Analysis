#include "OSUT3Analysis/Collections/interface/Uservariable.h"

#if IS_VALID(uservariables)

osu::Uservariable::Uservariable ()
{
}

osu::Uservariable::Uservariable (const TYPE(uservariables) &uservariable) :
  TYPE(uservariables) (uservariable)
{
}

osu::Uservariable::~Uservariable ()
{
}

#endif
