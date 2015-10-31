#ifndef OSU_TAU
#define OSU_TAU

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

#if IS_VALID(taus)

namespace osu
{
  class Tau : public TYPE(taus)
    {
      public:
        Tau ();
        Tau (const TYPE(taus) &);
        ~Tau ();
    };
}

#else

namespace osu
{
  typedef TYPE(taus) Tau;
}

#endif

#endif
