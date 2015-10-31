#ifndef OSU_BASICJET
#define OSU_BASICJET

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

#if IS_VALID(basicjets)

namespace osu
{
  class Basicjet : public TYPE(basicjets)
    {
      public:
        Basicjet ();
        Basicjet (const TYPE(basicjets) &);
        ~Basicjet ();
    };
}

#else

namespace osu
{
  typedef TYPE(basicjets) Basicjet;
}

#endif

#endif
