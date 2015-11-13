#include "OSUT3Analysis/Collections/interface/Track.h"

#if IS_VALID(tracks)

osu::Track::Track ()
{
}

osu::Track::Track (const TYPE(tracks) &track) :
  GenMatchable (track)
{
}

osu::Track::Track (const TYPE(tracks) &track, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (track, particles)
{
}

osu::Track::~Track ()
{
}

#endif
