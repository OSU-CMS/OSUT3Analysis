#include "OSUT3Analysis/Collections/interface/Muon.h"

#if IS_VALID(muons)

osu::Muon::Muon ()
{
}

osu::Muon::Muon (const TYPE(muons) &muon) :
  TYPE(muons) (muon)
{
}

osu::Muon::~Muon ()
{
}

#endif
