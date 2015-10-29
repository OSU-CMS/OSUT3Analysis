#include "OSUT3Analysis/Collections/interface/Supercluster.h"

#if IS_VALID(superclusters)

osu::Supercluster::Supercluster ()
{
}

osu::Supercluster::Supercluster (const TYPE(superclusters) &supercluster) :
  TYPE(superclusters) (supercluster)
{
}

osu::Supercluster::~Supercluster ()
{
}

#endif
