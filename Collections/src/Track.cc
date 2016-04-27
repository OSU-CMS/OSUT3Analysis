#include "OSUT3Analysis/Collections/interface/Track.h"

#if IS_VALID(tracks)

osu::Track::Track () :
  dRMinJet_ (999),
  isFiducialElectronTrack_ (true),
  isFiducialMuonTrack_ (true),
  matchedGsfTrack_ (),
  dRToMatchedGsfTrack_ (INVALID_VALUE),
  maxDeltaR_ (-1.0)
{
}

osu::Track::Track (const TYPE(tracks) &track) :
  GenMatchable (track),
  dRMinJet_ (999),
  isFiducialElectronTrack_ (true),
  isFiducialMuonTrack_ (true),
  matchedGsfTrack_ (),
  dRToMatchedGsfTrack_ (INVALID_VALUE),
  maxDeltaR_ (-1.0)
{
}

osu::Track::Track (const TYPE(tracks) &track, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (track, particles),
  dRMinJet_ (999),
  isFiducialElectronTrack_ (true),
  isFiducialMuonTrack_ (true),
  matchedGsfTrack_ (),
  dRToMatchedGsfTrack_ (INVALID_VALUE),
  maxDeltaR_ (-1.0)
{
}

osu::Track::Track (const TYPE(tracks) &track, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable (track, particles, cfg),
  dRMinJet_ (999),
  isFiducialElectronTrack_ (true),
  isFiducialMuonTrack_ (true),
  matchedGsfTrack_ (),
  dRToMatchedGsfTrack_ (INVALID_VALUE),
  maxDeltaR_ (-1.0)
{
}

osu::Track::Track (const TYPE(tracks) &track, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg, const edm::Handle<vector<reco::GsfTrack> > &gsfTracks, const EtaPhiList &electronVetoList, const EtaPhiList &muonVetoList) :
  GenMatchable (track, particles, cfg),
  dRMinJet_ (999),
  minDeltaRForFiducialTrack_ (cfg.getParameter<double> ("minDeltaRForFiducialTrack")),
  isFiducialElectronTrack_ (isFiducialTrack (electronVetoList, minDeltaRForFiducialTrack_)),
  isFiducialMuonTrack_ (isFiducialTrack (muonVetoList, minDeltaRForFiducialTrack_))
{
  maxDeltaR_ = cfg.getParameter<double> ("maxDeltaRForGsfTrackMatching");
  if (gsfTracks.isValid ())
    findMatchedGsfTrack (gsfTracks, matchedGsfTrack_, dRToMatchedGsfTrack_);
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

const edm::Ref<vector<reco::GsfTrack> >
osu::Track::matchedGsfTrack () const
{
  return matchedGsfTrack_;
}

const double
osu::Track::dRToMatchedGsfTrack () const
{
  return dRToMatchedGsfTrack_;
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

const edm::Ref<vector<reco::GsfTrack> > &
osu::Track::findMatchedGsfTrack (const edm::Handle<vector<reco::GsfTrack> > &gsfTracks, edm::Ref<vector<reco::GsfTrack> > &matchedGsfTrack, double &dRToMatchedGsfTrack) const
{
  dRToMatchedGsfTrack = INVALID_VALUE;
  for (vector<reco::GsfTrack>::const_iterator gsfTrack = gsfTracks->begin (); gsfTrack != gsfTracks->end (); gsfTrack++)
    {
      double dR = deltaR (*gsfTrack, *this);
      if (maxDeltaR_ >= 0.0 && dR > maxDeltaR_)
        continue;
      if (dR < dRToMatchedGsfTrack || dRToMatchedGsfTrack < 0.0)
        {
          dRToMatchedGsfTrack = dR;
          matchedGsfTrack = edm::Ref<vector<reco::GsfTrack> > (gsfTracks, gsfTrack - gsfTracks->begin ());
        }
    }

  return matchedGsfTrack;
}

#endif
