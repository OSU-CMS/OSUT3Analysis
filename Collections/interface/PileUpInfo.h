#ifndef OSU_PILEUPINFO
#define OSU_PILEUPINFO

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

#if IS_VALID(pileupinfos)

namespace osu
{
  class PileUpInfo : public TYPE(pileupinfos)
    {
      public:
        PileUpInfo ();
        PileUpInfo (const TYPE(pileupinfos) &);
        ~PileUpInfo ();
    };
}

#else

namespace osu
{
  typedef TYPE(pileupinfos) PileUpInfo;
}

#endif

#endif
