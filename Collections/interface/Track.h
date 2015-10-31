#ifndef OSU_TRACK
#define OSU_TRACK

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

#if IS_VALID(tracks)

namespace osu
{
  class Track : public TYPE(tracks)
    {
      public:
        Track ();
        Track (const TYPE(tracks) &);
        ~Track ();
    };
}

#else

namespace osu
{
  typedef TYPE(tracks) Track;
}

#endif

#endif
