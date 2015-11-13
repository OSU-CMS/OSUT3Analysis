#include "OSUT3Analysis/Collections/interface/PileUpInfo.h"

#if IS_VALID(pileupinfos)

osu::PileUpInfo::PileUpInfo ()
{
}

osu::PileUpInfo::PileUpInfo (const TYPE(pileupinfos) &pileupinfo) :
  TYPE(pileupinfos) (pileupinfo)
{
}

osu::PileUpInfo::~PileUpInfo ()
{
}

#endif
