#ifndef OSU_TRIGOBJ
#define OSU_TRIGOBJ

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

#if IS_VALID(trigobjs)

namespace osu
{
  class Trigobj : public TYPE(trigobjs)
    {
      public:
        Trigobj ();
        Trigobj (const TYPE(trigobjs) &);
        ~Trigobj ();
    };
}

#endif

#endif
