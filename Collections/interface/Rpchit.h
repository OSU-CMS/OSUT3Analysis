#ifndef OSU_RPCHIT
#define OSU_RPCHIT

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

#if IS_VALID(rpchits)

namespace osu
{
  class Rpchit : public TYPE(rpchits)
    {
      public:
        Rpchit ();
        Rpchit (const TYPE(rpchits) &);
        ~Rpchit ();
    };
}

#else

namespace osu
{
  typedef TYPE(rpchits) Rpchit;
}

#endif

#endif
