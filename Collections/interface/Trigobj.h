#ifndef OSU_TRIGOBJ
#define OSU_TRIGOBJ

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

#if IS_VALID(trigobjs)

namespace osu
{
  class Trigobj : public TYPE(trigobjs)
    {
      public:
        Trigobj ();
        Trigobj (const TYPE(trigobjs) &);
        ~Trigobj ();
    };
}

#else

namespace osu
{
  typedef TYPE(trigobjs) Trigobj;
}

#endif

#endif
