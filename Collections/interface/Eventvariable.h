#ifndef OSU_EVENTVARIABLE
#define OSU_EVENTVARIABLE

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

#if IS_VALID(eventvariables)

namespace osu
{
  class Eventvariable : public TYPE(eventvariables)
    {
      public:
        Eventvariable ();
        Eventvariable (const TYPE(eventvariables) &);
        ~Eventvariable ();
    };
}

#endif

#endif
