#ifndef OSU_EVENT
#define OSU_EVENT

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

#if IS_VALID(events)

namespace osu
{
  class Event : public TYPE(events)
    {
      public:
        Event ();
        Event (const TYPE(events) &);
        ~Event ();
    };
}

#endif

#endif
