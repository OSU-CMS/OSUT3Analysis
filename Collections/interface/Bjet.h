#ifndef OSU_BJET
#define OSU_BJET

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"

#if IS_VALID(bjets)

namespace osu
{
  class Bjet : public GenMatchable<TYPE(bjets), 0>
    {
      public:
        Bjet ();
        Bjet (const TYPE(bjets) &);
        Bjet (const TYPE(bjets) &, const edm::Handle<vector<osu::Mcparticle> > &);
        ~Bjet ();
    };
}

#else

namespace osu
{
  typedef TYPE(bjets) Bjet;
}

#endif

#endif
