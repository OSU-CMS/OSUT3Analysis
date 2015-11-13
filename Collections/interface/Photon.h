#ifndef OSU_PHOTON
#define OSU_PHOTON

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"

#if IS_VALID(photons)

namespace osu
{
  class Photon : public GenMatchable<TYPE(photons), 22>
    {
      public:
        Photon ();
        Photon (const TYPE(photons) &);
        Photon (const TYPE(photons) &, const edm::Handle<vector<osu::Mcparticle> > &);
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
