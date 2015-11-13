#include "OSUT3Analysis/Collections/interface/Muon.h"

#if IS_VALID(muons)

osu::Muon::Muon ()
{
  isTightMuonWRTVtx_ = false;  
}

osu::Muon::Muon (const TYPE(muons) &muon) :
  GenMatchable (muon)
{
}

osu::Muon::Muon (const TYPE(muons) &muon, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (muon, particles)
{
  isTightMuonWRTVtx_ = false;  
}

osu::Muon::~Muon ()
{
}

void osu::Muon::set_isTightMuonWRTVtx (const bool isTightMuon) 
{ 
  isTightMuonWRTVtx_ = isTightMuon;  
}


#endif
