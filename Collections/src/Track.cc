#include "OSUT3Analysis/Collections/interface/Track.h"

#if IS_VALID(tracks)

osu::Track::Track ()
{
}

osu::Track::Track (const TYPE(tracks) &track) :
  TYPE(tracks) (track)
{
}

osu::Track::~Track ()
{
}

#endif
