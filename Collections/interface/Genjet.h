#ifndef OSU_GENJET
#define OSU_GENJET

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

#if IS_VALID(genjets)

namespace osu
{
  class Genjet : public TYPE(genjets)
    {
      public:
        Genjet ();
        Genjet (const TYPE(genjets) &);
        ~Genjet ();
    };
}

#else

namespace osu
{
  typedef TYPE(genjets) Genjet;
}

#endif

#endif
