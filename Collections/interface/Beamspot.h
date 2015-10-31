#ifndef OSU_BEAMSPOT
#define OSU_BEAMSPOT

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

#if IS_VALID(beamspots)

namespace osu
{
  class Beamspot : public TYPE(beamspots)
    {
      public:
        Beamspot ();
        Beamspot (const TYPE(beamspots) &);
        ~Beamspot ();
    };
}

#else

namespace osu
{
  typedef TYPE(beamspots) Beamspot;
}

#endif

#endif
