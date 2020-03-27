#ifndef OSU_MCPARTICLE
#define OSU_MCPARTICLE

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

#if IS_VALID(mcparticles)

namespace osu
{
  class Mcparticle : public TYPE(mcparticles)
    {
      public:
        Mcparticle ();
        Mcparticle (const TYPE(mcparticles) &);
        ~Mcparticle ();

	const int uniqueMotherPdgId () const;

	void set_uniqueMotherPdgId (double value) { uniqueMotherPdgId_  = value; };

	int uniqueMotherPdgId_;

    };

}

#else

namespace osu
{
  typedef TYPE(mcparticles) Mcparticle;
}

#endif

#endif
