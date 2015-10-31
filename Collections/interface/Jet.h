#ifndef OSU_JET
#define OSU_JET

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

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

#else

namespace osu
{
  typedef TYPE(jets) Jet;
}

#endif

#endif
