#ifndef OSU_CANDJET
#define OSU_CANDJET

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"

#if IS_VALID(candjets)

namespace osu
{
  class Candjet : public GenMatchable<TYPE(candjets), 0>
    {
      public:
        Candjet ();
        Candjet (const TYPE(candjets) &);
        Candjet (const TYPE(candjets) &, const edm::Handle<vector<osu::Mcparticle> > &);
        ~Candjet ();
    };
}

#else

namespace osu
{
  typedef TYPE(candjets) Candjet;
}

#endif

#endif
