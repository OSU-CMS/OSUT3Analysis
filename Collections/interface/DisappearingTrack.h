#ifndef DISAPPEARING_TRACK
#define DISAPPEARING_TRACK

/////////////////////////////////////////////////////////////////
// osu::DisappearingTrack
//   This class is defined as a wrapper for osu::TrackBase to
//   separate out methods only the disappearing track analysis uses
//   If one uses setupFramework.py with "-d DISAPP_TRKS", then
//   this class will become osu::Track. Otherwise, osu::TrackBase
//   is the entire osu::Track class and this class can be ignored.
/////////////////////////////////////////////////////////////////


#include "OSUT3Analysis/Collections/interface/TrackBase.h"

#ifdef DISAPP_TRKS

#include <assert.h>
#include "DisappTrks/CandidateTrackProducer/interface/CandidateTrack.h"

#if DATA_FORMAT_FROM_MINIAOD && DATA_FORMAT_IS_2017
#include "DataFormats/PatCandidates/interface/IsolatedTrack.h"
#endif

#if IS_VALID(tracks)

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
                        const edm::Handle<vector<pat::PackedCandidate> > &,
                        const edm::Handle<vector<TYPE(jets)> > &,
                        const edm::ParameterSet &, 
                        const edm::Handle<vector<reco::GsfTrack> > &, 
                        const EtaPhiList &, 
                        const EtaPhiList &, 
                        const map<DetId, vector<double> > * const, 
                        const map<DetId, vector<int> > * const, 
                        const bool,
#if DATA_FORMAT_FROM_MINIAOD && DATA_FORMAT_IS_2017
                        const edm::Handle<vector<CandidateTrack> > &,
                        const edm::Handle<vector<pat::IsolatedTrack> > &);
#else
                        const edm::Handle<vector<CandidateTrack> > &);
#endif

      ~DisappearingTrack ();

      const edm::Ref<vector<CandidateTrack> > matchedCandidateTrack () const { return matchedCandidateTrack_; };
      const double dRToMatchedCandidateTrack () const { return (IS_INVALID(dRToMatchedCandidateTrack_)) ? MAX_DR : dRToMatchedCandidateTrack_; };
#if DATA_FORMAT_FROM_MINIAOD && DATA_FORMAT_IS_2017
      const edm::Ref<vector<pat::IsolatedTrack> > matchedIsolatedTrack () const { return matchedIsolatedTrack_; };
      const double dRToMatchedIsolatedTrack () const { return (IS_INVALID(dRToMatchedIsolatedTrack_)) ? MAX_DR : dRToMatchedIsolatedTrack_; };
#endif

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

#if DATA_FORMAT_FROM_MINIAOD && DATA_FORMAT_IS_2017
      void set_isoTrackIsolation(const edm::Handle<vector<pat::IsolatedTrack> > &);
#endif

      const float pfElectronIsoDR03 ()    const { return this->pfElectronIsoDR03_; };
      const float pfPUElectronIsoDR03 ()  const { return this->pfPUElectronIsoDR03_; };
      const float pfMuonIsoDR03 ()        const { return this->pfMuonIsoDR03_; };
      const float pfPUMuonIsoDR03 ()      const { return this->pfPUMuonIsoDR03_; };
      const float pfHFIsoDR03 ()          const { return this->pfHFIsoDR03_; };
      const float pfPUHFIsoDR03 ()        const { return this->pfPUHFIsoDR03_; };
      const float pfLostTrackIsoDR03 ()   const { return this->pfLostTrackIsoDR03_; };
      const float pfPULostTrackIsoDR03 () const { return this->pfPULostTrackIsoDR03_; };

      const float isoTrackIsoDR03 ()     const { return this->isoTrackIsoDR03_; };

      const float pfChHadIsoDR03 ()        const { return this->pfChHadIsoDR03_; };
      const float pfPUChHadIsoDR03 ()      const { return this->pfPUChHadIsoDR03_; };
      const float pfNeutralHadIsoDR03 ()   const { return this->pfNeutralHadIsoDR03_; };
      const float pfPUNeutralHadIsoDR03 () const { return this->pfPUNeutralHadIsoDR03_; };
      const float pfPhotonIsoDR03 ()       const { return this->pfPhotonIsoDR03_; };
      const float pfPUPhotonIsoDR03 ()     const { return this->pfPUPhotonIsoDR03_; };

#if DATA_FORMAT_IS_2017 // only makes sense with phase1 pixel upgrade
      // This could be in TrackBase, but is fairly specialized to the disappearing tracks search
      const bool isAllowedThreeLayerHitPattern() const;
#endif

    protected:
      vector<double> eleVtx_d0Cuts_barrel_, eleVtx_d0Cuts_endcap_;
      vector<double> eleVtx_dzCuts_barrel_, eleVtx_dzCuts_endcap_;

    private:
      const edm::Ref<vector<CandidateTrack> > &findMatchedCandidateTrack (const edm::Handle<vector<CandidateTrack> > &, edm::Ref<vector<CandidateTrack> > &, double &) const;
#if DATA_FORMAT_FROM_MINIAOD && DATA_FORMAT_IS_2017
      const edm::Ref<vector<pat::IsolatedTrack> > &findMatchedIsolatedTrack (const edm::Handle<vector<pat::IsolatedTrack> > &, edm::Ref<vector<pat::IsolatedTrack> > &, double &) const;
#endif

      void set_primaryPFIsolations(const edm::Handle<vector<pat::PackedCandidate> > &);
      void set_additionalPFIsolations(const edm::Handle<vector<pat::PackedCandidate> > &, const edm::Handle<vector<pat::PackedCandidate> > &);

      edm::Ref<vector<CandidateTrack> > matchedCandidateTrack_;
      double dRToMatchedCandidateTrack_;
      double maxDeltaR_candidateTrackMatching_;

#if DATA_FORMAT_FROM_MINIAOD && DATA_FORMAT_IS_2017
      edm::Ref<vector<pat::IsolatedTrack> > matchedIsolatedTrack_;      
      double dRToMatchedIsolatedTrack_;
      double maxDeltaR_isolatedTrackMatching_;
#endif

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

      float pfElectronIsoDR03_, pfPUElectronIsoDR03_;
      float pfMuonIsoDR03_, pfPUMuonIsoDR03_;
      float pfHFIsoDR03_, pfPUHFIsoDR03_;
      float pfLostTrackIsoDR03_, pfPULostTrackIsoDR03_;

      float isoTrackIsoDR03_;
      float pfChHadIsoDR03_, pfPUChHadIsoDR03_;
      float pfNeutralHadIsoDR03_, pfPUNeutralHadIsoDR03_;
      float pfPhotonIsoDR03_, pfPUPhotonIsoDR03_;
  };

#if IS_VALID(secondaryTracks)
  class SecondaryDisappearingTrack : public DisappearingTrack {
    public:
      SecondaryDisappearingTrack();
      SecondaryDisappearingTrack(const TYPE(tracks) &);
      SecondaryDisappearingTrack(const TYPE(tracks) &, 
                                 const edm::Handle<vector<osu::Mcparticle> > &);
      SecondaryDisappearingTrack(const TYPE(tracks) &, 
                                 const edm::Handle<vector<osu::Mcparticle> > &, 
                                 const edm::ParameterSet &);
      SecondaryDisappearingTrack(const TYPE(tracks) &, 
                                 const edm::Handle<vector<osu::Mcparticle> > &, 
                                 const edm::ParameterSet &, 
                                 const edm::Handle<vector<reco::GsfTrack> > &, 
                                 const EtaPhiList &, 
                                 const EtaPhiList &);
      SecondaryDisappearingTrack(const TYPE(tracks) &, 
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
      SecondaryDisappearingTrack(const TYPE(tracks) &, 
                                 const edm::Handle<vector<osu::Mcparticle> > &,
                                 const edm::Handle<vector<pat::PackedCandidate> > &,
                                 const edm::Handle<vector<pat::PackedCandidate> > &,
                                 const edm::Handle<vector<TYPE(jets)> > &,
                                 const edm::ParameterSet &, 
                                 const edm::Handle<vector<reco::GsfTrack> > &, 
                                 const EtaPhiList &, 
                                 const EtaPhiList &, 
                                 const map<DetId, vector<double> > * const, 
                                 const map<DetId, vector<int> > * const, 
                                 const bool,
#if DATA_FORMAT_FROM_MINIAOD && DATA_FORMAT_IS_2017
                                 const edm::Handle<vector<CandidateTrack> > &,
                                 const edm::Handle<vector<pat::IsolatedTrack> > &);
#else
                                 const edm::Handle<vector<CandidateTrack> > &);
#endif

      ~SecondaryDisappearingTrack();
  };
#endif // IS_VALID(secondaryTracks)

} // namespace osu

#endif // if IS_VALID(tracks)

namespace osu {
#if IS_VALID(tracks)
  typedef osu::DisappearingTrack Track;
#if IS_VALID(secondaryTracks)
  typedef osu::SecondaryDisappearingTrack SecondaryTrack;
#else
  typedef TYPE(secondaryTracks) SecondaryTrack;
#endif // IS_VALID(secondaryTracks)
#else
  typedef TYPE(tracks) Track;
#endif // IS_VALID(tracks)
}

#endif // ifdef DISAPP_TRKS

#endif // ifndef DISAPPEARING_TRACK
