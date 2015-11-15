#ifndef OSU_GENJET
#define OSU_GENJET

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"

#if IS_VALID(genjets)

namespace osu
{
  class Genjet : public GenMatchable<TYPE(genjets), 0>
    {
      public:
        Genjet ();
        Genjet (const TYPE(genjets) &);
        Genjet (const TYPE(genjets) &, const edm::Handle<vector<osu::Mcparticle> > &);
        Genjet (const TYPE(genjets) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &);
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
