#include "OSUT3Analysis/Collections/interface/Muon.h"

#if IS_VALID(muons)

osu::Muon::Muon ()
{
}

osu::Muon::Muon (const TYPE(muons) &muon) :
  GenMatchable (muon)
{
}

osu::Muon::Muon (const TYPE(muons) &muon, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (muon, particles)
{
}

osu::Muon::~Muon ()
{
}

#endif
