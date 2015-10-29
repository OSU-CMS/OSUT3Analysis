#include "OSUT3Analysis/Collections/interface/Basicjet.h"

#if IS_VALID(basicjets)

osu::Basicjet::Basicjet ()
{
}

osu::Basicjet::Basicjet (const TYPE(basicjets) &basicjet) :
  TYPE(basicjets) (basicjet)
{
}

osu::Basicjet::~Basicjet ()
{
}

#endif
