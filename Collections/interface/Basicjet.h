#ifndef OSU_BASICJET
#define OSU_BASICJET

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

#if IS_VALID(basicjets)

namespace osu
{
  class Basicjet : public TYPE(basicjets)
    {
      public:
        Basicjet ();
        Basicjet (const TYPE(basicjets) &);
        ~Basicjet ();
    };
}

#endif

#endif
