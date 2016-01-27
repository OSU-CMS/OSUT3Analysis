#ifndef OSU_CSCSEG
#define OSU_CSCSEG

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

#if IS_VALID(cscsegs)

namespace osu
{
  class Cscseg : public TYPE(cscsegs)
    {
      public:
        Cscseg ();
        Cscseg (const TYPE(cscsegs) &);
        ~Cscseg ();
    };
}

#else

namespace osu
{
  typedef TYPE(cscsegs) Cscseg;
}

#endif

#endif
