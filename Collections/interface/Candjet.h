#ifndef OSU_CANDJET
#define OSU_CANDJET

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

#if IS_VALID(candjets)

namespace osu
{
  class Candjet : public TYPE(candjets)
    {
      public:
        Candjet ();
        Candjet (const TYPE(candjets) &);
        ~Candjet ();
    };
}

#endif

#endif
