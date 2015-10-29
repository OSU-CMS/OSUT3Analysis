#include "OSUT3Analysis/Collections/interface/Candjet.h"

#if IS_VALID(candjets)

osu::Candjet::Candjet ()
{
}

osu::Candjet::Candjet (const TYPE(candjets) &candjet) :
  TYPE(candjets) (candjet)
{
}

osu::Candjet::~Candjet ()
{
}

#endif
