#include "OSUT3Analysis/Collections/interface/Track.h"

#if IS_VALID(tracks)

osu::Track::Track () :
  dRMinJet_ (999),
  isFiducialElectronTrack_ (true),
  isFiducialMuonTrack_ (true)
{
}

osu::Track::Track (const TYPE(tracks) &track) :
  GenMatchable (track),
  dRMinJet_ (999),
  isFiducialElectronTrack_ (true),
  isFiducialMuonTrack_ (true)
{
}

osu::Track::Track (const TYPE(tracks) &track, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (track, particles),
  dRMinJet_ (999),
  isFiducialElectronTrack_ (true),
  isFiducialMuonTrack_ (true)
{
}

osu::Track::Track (const TYPE(tracks) &track, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable (track, particles, cfg),
  dRMinJet_ (999),
  isFiducialElectronTrack_ (true),
  isFiducialMuonTrack_ (true)
{
}

osu::Track::Track (const TYPE(tracks) &track, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg, const EtaPhiList &electronVetoList, const EtaPhiList &muonVetoList) :
  GenMatchable (track, particles, cfg),
  dRMinJet_ (999),
  minDeltaRForFiducialTrack_ (cfg.getParameter<double> ("minDeltaRForFiducialTrack")),
  isFiducialElectronTrack_ (isFiducialTrack (electronVetoList, minDeltaRForFiducialTrack_)),
  isFiducialMuonTrack_ (isFiducialTrack (muonVetoList, minDeltaRForFiducialTrack_))
{
}

osu::Track::~Track ()
{
}

const bool
osu::Track::isFiducialElectronTrack () const
{
  return isFiducialElectronTrack_;
}

const bool
osu::Track::isFiducialMuonTrack () const
{
  return isFiducialMuonTrack_;
}

void
osu::Track::set_dRMinJet(const double dRMinJet) 
{
  dRMinJet_ = dRMinJet;  
}

const bool
osu::Track::isFiducialTrack (const EtaPhiList &vetoList, const double minDeltaR) const
{
  const double minDR = max (minDeltaR, vetoList.minDeltaR); // use the given parameter unless the bin size from which the veto list is calculated is larger
  for (const auto &etaPhi : vetoList)
    if (deltaR (this->eta (), this->phi (), etaPhi.eta, etaPhi.phi) < minDR)
      return false;
  return true;
}

#endif
