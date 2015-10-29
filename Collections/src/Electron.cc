#include "OSUT3Analysis/Collections/interface/Electron.h"

#if IS_VALID(electrons)

osu::Electron::Electron ()
{
}

osu::Electron::Electron (const TYPE(electrons) &electron) :
  TYPE(electrons) (electron)
{
}

osu::Electron::~Electron ()
{
}

#endif
