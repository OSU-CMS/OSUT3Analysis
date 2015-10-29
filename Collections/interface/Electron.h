#ifndef OSU_ELECTRON
#define OSU_ELECTRON

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

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

#endif

#endif
