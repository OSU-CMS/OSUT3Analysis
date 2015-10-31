#ifndef OSU_PHOTON
#define OSU_PHOTON

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

#if IS_VALID(photons)

namespace osu
{
  class Photon : public TYPE(photons)
    {
      public:
        Photon ();
        Photon (const TYPE(photons) &);
        ~Photon ();
    };
}

#else

namespace osu
{
  typedef TYPE(photons) Photon;
}

#endif

#endif
