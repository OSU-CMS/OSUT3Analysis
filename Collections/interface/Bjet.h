#ifndef OSU_BJET
#define OSU_BJET

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

#if IS_VALID(bjets)

namespace osu
{
  class Bjet : public TYPE(bjets)
    {
      public:
        Bjet ();
        Bjet (const TYPE(bjets) &);
        ~Bjet ();
    };
}

#else

namespace osu
{
  typedef TYPE(bjets) Bjet;
}

#endif

#endif
