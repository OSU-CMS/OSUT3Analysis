#ifndef OSU_BJET
#define OSU_BJET

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

#if IS_VALID(bjets)

namespace osu
{
  class Bjet : public TYPE(bjets)
    {
      public:
        Bjet ();
        Bjet (const TYPE(bjets) &);
        ~Bjet ();
    };
}

#endif

#endif
