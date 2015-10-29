#include "OSUT3Analysis/Collections/interface/Beamspot.h"

#if IS_VALID(beamspots)

osu::Beamspot::Beamspot ()
{
}

osu::Beamspot::Beamspot (const TYPE(beamspots) &beamspot) :
  TYPE(beamspots) (beamspot)
{
}

osu::Beamspot::~Beamspot ()
{
}

#endif
