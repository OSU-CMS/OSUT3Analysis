#ifndef OSU_CANDELE
#define OSU_CANDELE

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"

#if IS_VALID(candeles)

namespace osu
{
  class Candele : public GenMatchable<TYPE(candeles), 11>
    {
      public:
        Candele ();
        Candele (const TYPE(candeles) &);
        Candele (const TYPE(candeles) &, const edm::Handle<vector<osu::Mcparticle> > &);
        Candele (const TYPE(candeles) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &);
        ~Candele ();
    };
}

#else

namespace osu
{
  typedef TYPE(candeles) Candele;
}

#endif

#endif
