#ifndef OSU_DTSEG
#define OSU_DTSEG

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

#if IS_VALID(dtsegs)

namespace osu
{
  class Dtseg : public TYPE(dtsegs)
    {
      public:
        Dtseg ();
        Dtseg (const TYPE(dtsegs) &);
        ~Dtseg ();
    };
}

#else

namespace osu
{
  typedef TYPE(dtsegs) Dtseg;
}

#endif

#endif
