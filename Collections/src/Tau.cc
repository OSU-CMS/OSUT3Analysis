#include "OSUT3Analysis/Collections/interface/Tau.h"

#if IS_VALID(taus)

osu::Tau::Tau ()
{
}

osu::Tau::Tau (const TYPE(taus) &tau) :
  TYPE(taus) (tau)
{
}

osu::Tau::~Tau ()
{
}

#endif
