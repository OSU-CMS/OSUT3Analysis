#ifndef OSU_MUON
#define OSU_MUON

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

#if IS_VALID(muons)

namespace osu
{
  class Muon : public TYPE(muons)
    {
      public:
        Muon ();
        Muon (const TYPE(muons) &);
        ~Muon ();
    };
}

#endif

#endif
