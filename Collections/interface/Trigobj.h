#ifndef OSU_TRIGOBJ
#define OSU_TRIGOBJ

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"

#if IS_VALID(trigobjs)

namespace osu
{
  class Trigobj : public GenMatchable<TYPE(trigobjs), 0>
    {
      public:
        Trigobj ();
        Trigobj (const TYPE(trigobjs) &);
        Trigobj (const TYPE(trigobjs) &, const edm::Handle<vector<osu::Mcparticle> > &);
        Trigobj (const TYPE(trigobjs) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &);
        ~Trigobj ();
    };
}

#else

namespace osu
{
  typedef TYPE(trigobjs) Trigobj;
}

#endif

#endif
