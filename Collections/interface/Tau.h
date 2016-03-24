#ifndef OSU_TAU
#define OSU_TAU

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"

#if IS_VALID(taus)

namespace osu
{
  class Tau : public GenMatchable<TYPE(taus), 15>
    {
      public:
        Tau ();
        Tau (const TYPE(taus) &);
        Tau (const TYPE(taus) &, const edm::Handle<vector<osu::Mcparticle> > &);
        Tau (const TYPE(taus) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &);
        ~Tau ();

        const bool passesDecayModeReconstruction () const;
        const bool passesLightFlavorRejection () const;
    };
}

#else

namespace osu
{
  typedef TYPE(taus) Tau;
}

#endif

#endif
