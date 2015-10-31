#ifndef OSU_BXLUMI
#define OSU_BXLUMI

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

#if IS_VALID(bxlumis)

namespace osu
{
  class Bxlumi : public TYPE(bxlumis)
    {
      public:
        Bxlumi ();
        Bxlumi (const TYPE(bxlumis) &);
        ~Bxlumi ();
    };
}

#else

namespace osu
{
  typedef TYPE(bxlumis) Bxlumi;
}

#endif

#endif
