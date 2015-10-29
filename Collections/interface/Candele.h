#ifndef OSU_CANDELE
#define OSU_CANDELE

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

#if IS_VALID(candeles)

namespace osu
{
  class Candele : public TYPE(candeles)
    {
      public:
        Candele ();
        Candele (const TYPE(candeles) &);
        ~Candele ();
    };
}

#endif

#endif
