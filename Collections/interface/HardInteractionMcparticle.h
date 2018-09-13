#ifndef OSU_HARDINTERACTIONMCPARTICLE
#define OSU_HARDINTERACTIONMCPARTICLE

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

#if IS_VALID(hardInteractionMcparticles)

namespace osu
{
  class HardInteractionMcparticle : public TYPE(hardInteractionMcparticles)
    {
      public:
        HardInteractionMcparticle ();
        HardInteractionMcparticle (const TYPE(hardInteractionMcparticles) &);
        ~HardInteractionMcparticle ();
    };
}

#else

namespace osu
{
  typedef TYPE(hardInteractionMcparticles) HardInteractionMcparticle;
}

#endif

#endif
