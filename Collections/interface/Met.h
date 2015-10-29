#ifndef OSU_MET
#define OSU_MET

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

#if IS_VALID(mets)

namespace osu
{
  class Met : public TYPE(mets)
    {
      public:
        Met ();
        Met (const TYPE(mets) &);
        ~Met ();
    };
}

#endif

#endif
