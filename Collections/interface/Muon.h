#ifndef OSU_MUON
#define OSU_MUON

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

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

#else

namespace osu
{
  typedef TYPE(muons) Muon;
}

#endif

#endif
