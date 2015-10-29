#ifndef OSU_PRIMARYVERTEX
#define OSU_PRIMARYVERTEX

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

#if IS_VALID(primaryvertexs)

namespace osu
{
  class Primaryvertex : public TYPE(primaryvertexs)
    {
      public:
        Primaryvertex ();
        Primaryvertex (const TYPE(primaryvertexs) &);
        ~Primaryvertex ();
    };
}

#endif

#endif
