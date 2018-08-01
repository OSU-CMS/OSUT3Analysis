#ifndef DISAPPEARING_TRACK
#define DISAPPEARING_TRACK

#ifdef DISAPP_TRKS
#if IS_VALID(tracks)

#include <assert.h>

#include "OSUT3Analysis/Collections/interface/TrackBase.h"
#include "DisappTrks/CandidateTrackProducer/interface/CandidateTrack.h"

namespace osu {
  class DisappearingTrack : public TrackBase {
    public:
      DisappearingTrack();
      DisappearingTrack(const TYPE(tracks) &);
      DisappearingTrack(const TYPE(tracks) &, 
                        const edm::Handle<vector<osu::Mcparticle> > &);
      DisappearingTrack(const TYPE(tracks) &, 
                        const edm::Handle<vector<osu::Mcparticle> > &, 
                        const edm::ParameterSet &);
      DisappearingTrack(const TYPE(tracks) &, 
                        const edm::Handle<vector<osu::Mcparticle> > &, 
                        const edm::ParameterSet &, 
                        const edm::Handle<vector<reco::GsfTrack> > &, 
                        const EtaPhiList &, 
                        const EtaPhiList &);
      DisappearingTrack(const TYPE(tracks) &, 
                        const edm::Handle<vector<osu::Mcparticle> > &, 
                        const edm::Handle<vector<pat::PackedCandidate> > &, 
                        const edm::Handle<vector<TYPE(jets)> > &,
                        const edm::ParameterSet &, 
                        const edm::Handle<vector<reco::GsfTrack> > &, 
                        const EtaPhiList &, 
                        const EtaPhiList &, 
                        const map<DetId, vector<double> > * const, 
                        const map<DetId, vector<int> > * const, 
                        const bool);

      // the DisappTrks constructor
      DisappearingTrack(const TYPE(tracks) &, 
                        const edm::Handle<vector<osu::Mcparticle> > &, 
                        const edm::Handle<vector<pat::PackedCandidate> > &, 
                        const edm::Handle<vector<TYPE(jets)> > &,
                        const edm::ParameterSet &, 
                        const edm::Handle<vector<reco::GsfTrack> > &, 
                        const EtaPhiList &, 
                        const EtaPhiList &, 
                        const map<DetId, vector<double> > * const, 
                        const map<DetId, vector<int> > * const, 
                        const bool,
                        const edm::Handle<vector<CandidateTrack> > &);

      ~DisappearingTrack ();

      const edm::Ref<vector<CandidateTrack> > matchedCandidateTrack () const { return matchedCandidateTrack_; };
      const double dRToMatchedCandidateTrack () const { return (IS_INVALID(dRToMatchedCandidateTrack_)) ? MAX_DR : dRToMatchedCandidateTrack_; };

      void set_minDeltaRToElectrons(const edm::Handle<edm::View<TYPE(electrons)> > &,
                                    const edm::Handle<vector<TYPE(primaryvertexs)> > &,
                                    const edm::Handle<edm::ValueMap<bool> > &,
                                    const edm::Handle<edm::ValueMap<bool> > &,
                                    const edm::Handle<edm::ValueMap<bool> > &,
                                    const edm::Handle<edm::ValueMap<bool> > &);
      void set_minDeltaRToMuons(const edm::Handle<vector<TYPE(muons)> > &, const edm::Handle<vector<TYPE(primaryvertexs)> > &);
      void set_minDeltaRToTaus(const edm::Handle<vector<TYPE(taus)> > &);

      const float deltaRToClosestElectron ()         const { return (IS_INVALID(deltaRToClosestElectron_))       ? MAX_DR : deltaRToClosestElectron_; };
      const float deltaRToClosestVetoElectron ()     const { return (IS_INVALID(deltaRToClosestVetoElectron_))   ? MAX_DR : deltaRToClosestVetoElectron_; };
      const float deltaRToClosestLooseElectron ()    const { return (IS_INVALID(deltaRToClosestLooseElectron_))  ? MAX_DR : deltaRToClosestLooseElectron_; };
      const float deltaRToClosestMediumElectron ()   const { return (IS_INVALID(deltaRToClosestMediumElectron_)) ? MAX_DR : deltaRToClosestMediumElectron_; };
      const float deltaRToClosestTightElectron ()    const { return (IS_INVALID(deltaRToClosestTightElectron_))  ? MAX_DR : deltaRToClosestTightElectron_; };
      const float deltaRToClosestMuon ()             const { return (IS_INVALID(deltaRToClosestMuon_))           ? MAX_DR : deltaRToClosestMuon_; };
      const float deltaRToClosestLooseMuon ()        const { return (IS_INVALID(deltaRToClosestLooseMuon_))      ? MAX_DR : deltaRToClosestLooseMuon_; };
      const float deltaRToClosestMediumMuon ()       const { return (IS_INVALID(deltaRToClosestMediumMuon_))     ? MAX_DR : deltaRToClosestMediumMuon_; };
      const float deltaRToClosestTightMuon ()        const { return (IS_INVALID(deltaRToClosestTightMuon_))      ? MAX_DR : deltaRToClosestTightMuon_; };
      const float deltaRToClosestTau ()              const { return (IS_INVALID(deltaRToClosestTau_))            ? MAX_DR : deltaRToClosestTau_; };
      const float deltaRToClosestTauHad ()           const { return (IS_INVALID(deltaRToClosestTauHad_))         ? MAX_DR : deltaRToClosestTauHad_; };

    private:
      const edm::Ref<vector<CandidateTrack> > &findMatchedCandidateTrack (const edm::Handle<vector<CandidateTrack> > &, edm::Ref<vector<CandidateTrack> > &, double &) const;

      edm::Ref<vector<CandidateTrack> > matchedCandidateTrack_;
      double dRToMatchedCandidateTrack_;
      double maxDeltaR_candidateTrackMatching_;

      vector<double> eleVtx_d0Cuts_barrel_, eleVtx_d0Cuts_endcap_;
      vector<double> eleVtx_dzCuts_barrel_, eleVtx_dzCuts_endcap_;

      float deltaRToClosestElectron_;
      float deltaRToClosestVetoElectron_;
      float deltaRToClosestLooseElectron_;
      float deltaRToClosestMediumElectron_;
      float deltaRToClosestTightElectron_;
      float deltaRToClosestMuon_;
      float deltaRToClosestLooseMuon_;
      float deltaRToClosestMediumMuon_;
      float deltaRToClosestTightMuon_;
      float deltaRToClosestTau_;
      float deltaRToClosestTauHad_;
  };

  typedef DisappearingTrack Track;
#if IS_VALID(secondaryTracks)
  typedef DisappearingTrack SecondaryTrack;
#endif

} // namespace osu

#else // IS_VALID(tracks)
namespace osu {
  typedef TYPE(tracks) Track;
  typedef TYPE(secondaryTracks) SecondaryTrack;
}
#endif // if IS_VALID(tracks)

#endif // ifdef DISAPP_TRKS

#endif // ifndef DISAPPEARING_TRACK
