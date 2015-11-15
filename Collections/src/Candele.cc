#include "OSUT3Analysis/Collections/interface/Candele.h"

#if IS_VALID(candeles)

osu::Candele::Candele ()
{
}

osu::Candele::Candele (const TYPE(candeles) &candele) :
  GenMatchable (candele)
{
}

osu::Candele::Candele (const TYPE(candeles) &candele, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (candele, particles)
{
}

osu::Candele::Candele (const TYPE(candeles) &candele, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable (candele, particles, cfg)
{
}

osu::Candele::~Candele ()
{
}

#endif
