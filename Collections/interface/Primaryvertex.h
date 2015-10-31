#ifndef OSU_PRIMARYVERTEX
#define OSU_PRIMARYVERTEX

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

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

#else

namespace osu
{
  typedef TYPE(primaryvertexs) Primaryvertex;
}

#endif

#endif
