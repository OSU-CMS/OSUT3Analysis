#include "OSUT3Analysis/Collections/interface/Candele.h"

#if IS_VALID(candeles)

osu::Candele::Candele ()
{
}

osu::Candele::Candele (const TYPE(candeles) &candele) :
  TYPE(candeles) (candele)
{
}

osu::Candele::~Candele ()
{
}

#endif
