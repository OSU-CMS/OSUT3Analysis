#include "OSUT3Analysis/Collections/interface/Track.h"

#if IS_VALID(tracks)

osu::Track::Track ()
{
  dRMinJet_ = 999;  
}

osu::Track::Track (const TYPE(tracks) &track) :
  GenMatchable (track)
{
  dRMinJet_ = 999;  
}

osu::Track::Track (const TYPE(tracks) &track, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (track, particles)
{
  dRMinJet_ = 999;  
}

osu::Track::Track (const TYPE(tracks) &track, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable (track, particles, cfg)
{
  dRMinJet_ = 999;  
}

osu::Track::~Track ()
{
  dRMinJet_ = 999;  
}

void osu::Track::set_dRMinJet(const double dRMinJet) 
{
  dRMinJet_ = dRMinJet;  
}

#endif
