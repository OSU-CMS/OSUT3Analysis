#include "OSUT3Analysis/Collections/interface/Bjet.h"

#if IS_VALID(bjets)

osu::Bjet::Bjet ()
{
}

osu::Bjet::Bjet (const TYPE(bjets) &bjet) :
  GenMatchable (bjet)
{
}

osu::Bjet::Bjet (const TYPE(bjets) &bjet, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (bjet, particles)
{
}

osu::Bjet::Bjet (const TYPE(bjets) &bjet, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable (bjet, particles, cfg)
{
}

osu::Bjet::~Bjet ()
{
}

#endif
