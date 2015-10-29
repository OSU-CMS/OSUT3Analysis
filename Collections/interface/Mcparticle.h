#ifndef OSU_MCPARTICLE
#define OSU_MCPARTICLE

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

#if IS_VALID(mcparticles)

namespace osu
{
  class Mcparticle : public TYPE(mcparticles)
    {
      public:
        Mcparticle ();
        Mcparticle (const TYPE(mcparticles) &);
        ~Mcparticle ();
    };
}

#endif

#endif
