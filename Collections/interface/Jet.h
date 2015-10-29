#ifndef OSU_JET
#define OSU_JET

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

#if IS_VALID(jets)

namespace osu
{
  class Jet : public TYPE(jets)
    {
      public:
        Jet ();
        Jet (const TYPE(jets) &);
        ~Jet ();
    };
}

#endif

#endif
