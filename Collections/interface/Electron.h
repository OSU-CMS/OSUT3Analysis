#ifndef OSU_ELECTRON
#define OSU_ELECTRON

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"

#if IS_VALID(electrons)

namespace osu
{
  class Electron : public GenMatchable<TYPE(electrons), 11>
    {
      public:
        Electron ();
        Electron (const TYPE(electrons) &);
        Electron (const TYPE(electrons) &, const edm::Handle<vector<osu::Mcparticle> > &);
        ~Electron ();
    };
}

#else

namespace osu
{
  typedef TYPE(electrons) Electron;
}

#endif

#endif
