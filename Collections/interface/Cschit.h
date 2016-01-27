#ifndef OSU_CSCHIT
#define OSU_CSCHIT

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

#if IS_VALID(cschits)

namespace osu
{
  class Cschit : public TYPE(cschits)
    {
      public:
        Cschit ();
        Cschit (const TYPE(cschits) &);
        ~Cschit ();
    };
}

#else

namespace osu
{
  typedef TYPE(cschits) Cschit;
}

#endif

#endif
