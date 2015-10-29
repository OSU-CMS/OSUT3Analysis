#include "OSUT3Analysis/Collections/interface/Jet.h"

#if IS_VALID(jets)

osu::Jet::Jet ()
{
}

osu::Jet::Jet (const TYPE(jets) &jet) :
  TYPE(jets) (jet)
{
}

osu::Jet::~Jet ()
{
}

#endif
