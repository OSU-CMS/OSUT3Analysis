#ifndef OSU_BEAMSPOT
#define OSU_BEAMSPOT

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

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

#endif

#endif
