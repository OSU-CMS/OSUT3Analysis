#ifndef OSU_SUPERCLUSTER
#define OSU_SUPERCLUSTER

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

#if IS_VALID(superclusters)

namespace osu
{
  class Supercluster : public TYPE(superclusters)
    {
      public:
        Supercluster ();
        Supercluster (const TYPE(superclusters) &);
        ~Supercluster ();
    };
}

#endif

#endif
