#include "OSUT3Analysis/Collections/interface/Event.h"

#if IS_VALID(events)

osu::Event::Event ()
{
}

osu::Event::Event (const TYPE(events) &event) :
  TYPE(events) (event)
{
}

osu::Event::~Event ()
{
}

#endif
