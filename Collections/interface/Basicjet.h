#ifndef OSU_BASICJET
#define OSU_BASICJET

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"

#if IS_VALID(basicjets)

namespace osu
{
  class Basicjet : public GenMatchable<TYPE(basicjets), 0>
    {
      public:
        Basicjet ();
        Basicjet (const TYPE(basicjets) &);
        Basicjet (const TYPE(basicjets) &, const edm::Handle<vector<osu::Mcparticle> > &);
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
