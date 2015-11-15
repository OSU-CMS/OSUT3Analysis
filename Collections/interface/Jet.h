#ifndef OSU_JET
#define OSU_JET

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"

#if IS_VALID(jets)

namespace osu
{
  class Jet : public GenMatchable<TYPE(jets), 0>
    {
      public:
        Jet ();
        Jet (const TYPE(jets) &);
        Jet (const TYPE(jets) &, const edm::Handle<vector<osu::Mcparticle> > &);
        Jet (const TYPE(jets) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &);
        ~Jet ();
    };
}

#else

namespace osu
{
  typedef TYPE(jets) Jet;
}

#endif

#endif
