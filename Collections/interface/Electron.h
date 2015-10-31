#ifndef OSU_ELECTRON
#define OSU_ELECTRON

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

#if IS_VALID(electrons)

namespace osu
{
  class Electron : public TYPE(electrons)
    {
      public:
        Electron ();
        Electron (const TYPE(electrons) &);
        ~Electron ();
    };
}

#else

namespace osu
{
  typedef TYPE(electrons) Electron;
}

#endif

#endif
