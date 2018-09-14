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

	const int motherPdgId () const;
	const int motherStatus () const;

	void set_motherPdgId (double value) { motherPdgId_  = value; };
	void set_motherStatus (double value) { motherStatus_  = value; };

	int motherPdgId_;
	int motherStatus_;

    };
}

#else

namespace osu
{
  typedef TYPE(hardInteractionMcparticles) HardInteractionMcparticle;
}

#endif

#endif
