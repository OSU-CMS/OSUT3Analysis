#ifndef OSU_USERVARIABLE
#define OSU_USERVARIABLE

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

#if IS_VALID(uservariables)

namespace osu
{
  class Uservariable : public TYPE(uservariables)
    {
      public:
        Uservariable ();
        Uservariable (const TYPE(uservariables) &);
        ~Uservariable ();
    };
}

#endif

#endif
