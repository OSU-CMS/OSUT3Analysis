#include "OSUT3Analysis/Collections/interface/Photon.h"

#if IS_VALID(photons)

osu::Photon::Photon ()
{
}

osu::Photon::Photon (const TYPE(photons) &photon) :
  TYPE(photons) (photon)
{
}

osu::Photon::~Photon ()
{
}

#endif
