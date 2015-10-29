#ifndef OSU_GENJET
#define OSU_GENJET

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

#if IS_VALID(genjets)

namespace osu
{
  class Genjet : public TYPE(genjets)
    {
      public:
        Genjet ();
        Genjet (const TYPE(genjets) &);
        ~Genjet ();
    };
}

#endif

#endif
