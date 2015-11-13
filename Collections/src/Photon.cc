#include "OSUT3Analysis/Collections/interface/Photon.h"

#if IS_VALID(photons)

osu::Photon::Photon ()
{
}

osu::Photon::Photon (const TYPE(photons) &photon) :
  GenMatchable (photon)
{
}

osu::Photon::Photon (const TYPE(photons) &photon, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (photon, particles)
{
}

osu::Photon::~Photon ()
{
}

#endif
