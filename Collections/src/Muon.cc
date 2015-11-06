#include "OSUT3Analysis/Collections/interface/Muon.h"

#if IS_VALID(muons)

osu::Muon::Muon ()
{
  isTightMuonWRTVtx_ = false;  
}

osu::Muon::Muon (const TYPE(muons) &muon) :
  TYPE(muons) (muon)
{
  isTightMuonWRTVtx_ = false;  
}

osu::Muon::~Muon ()
{
}

void osu::Muon::set_isTightMuonWRTVtx (bool isTightMuon) 
{ 
  isTightMuonWRTVtx_ = isTightMuon;  
}


#endif
