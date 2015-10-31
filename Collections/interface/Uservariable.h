#ifndef OSU_USERVARIABLE
#define OSU_USERVARIABLE

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

#if IS_VALID(uservariables)

namespace osu
{
  class Uservariable : public TYPE(uservariables)
    {
      public:
        Uservariable ();
        Uservariable (const TYPE(uservariables) &);
        ~Uservariable ();
    };
}

#else

namespace osu
{
  typedef TYPE(uservariables) Uservariable;
}

#endif

#endif
