#ifndef TRACK_BASE
#define TRACK_BASE

#include <random>
#include <chrono>

#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"

#ifndef MAX_DR
#define MAX_DR (99.0)
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

struct EtaPhi {
  double eta;
  double phi;
  double sigma;

  EtaPhi(const double a, const double b, const double sigma = -1.0) :
    eta(a),
    phi(b),
    sigma(sigma)
  {
  }
};

struct EtaPhiList : public vector<EtaPhi> {
  double minDeltaR;

  EtaPhiList() :
    minDeltaR(0.0)
  {
  }
};

#if IS_VALID(tracks)

namespace osu
{
  class Track : public GenMatchable<TYPE(tracks), 0> {
      public:
        Track();
        Track(const TYPE(tracks) &);
        Track(const TYPE(tracks) &, 
              const edm::Handle<vector<osu::Mcparticle> > &);
        Track(const TYPE(tracks) &, 
              const edm::Handle<vector<osu::Mcparticle> > &, 
              const edm::ParameterSet &);
        Track(const TYPE(tracks) &, 
              const edm::Handle<vector<osu::Mcparticle> > &, 
              const edm::ParameterSet &, 
              const edm::Handle<vector<reco::GsfTrack> > &, 
              const EtaPhiList &, 
              const EtaPhiList &);
        Track(const TYPE(tracks) &, 
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

        ~Track();

        const double dRMinJet() const              { return (IS_INVALID(dRMinJet_)) ? MAX_DR : dRMinJet_; };
        void  set_dRMinJet(const double dRMinJet)  { dRMinJet_ = dRMinJet; };

        const bool isFiducialElectronTrack() const { return isFiducialElectronTrack_; };
        const bool isFiducialMuonTrack() const     { return isFiducialMuonTrack_; };
        const bool isFiducialECALTrack() const     { return isFiducialECALTrack_; };

        const double maxSigmaForFiducialElectronTrack() const { return maxSigmaForFiducialElectronTrack_; };
        const double maxSigmaForFiducialMuonTrack() const     { return maxSigmaForFiducialMuonTrack_; };

        const edm::Ref<vector<reco::GsfTrack> > matchedGsfTrack() const { return matchedGsfTrack_; };
        const double dRToMatchedGsfTrack() const                        { return (IS_INVALID(dRToMatchedGsfTrack_)) ? MAX_DR : dRToMatchedGsfTrack_; };

        const int gsfTrackNumberOfValidHits() const;
        const int gsfTrackNumberOfValidPixelHits() const;
        const int gsfTrackNumberOfValidPixelBarrelHits() const;
        const int gsfTrackNumberOfValidPixelEndcapHits() const;
        const int gsfTrackMissingInnerHits() const;
        const int gsfTrackMissingMiddleHits() const;
        const int gsfTrackMissingOuterHits() const;

        const int bestTrackNumberOfValidHits() const;
        const int bestTrackNumberOfValidPixelHits() const;
        const int bestTrackNumberOfValidPixelBarrelHits() const;
        const int bestTrackNumberOfValidPixelEndcapHits() const;
        const int bestTrackMissingInnerHits() const;
        const int bestTrackMissingMiddleHits() const;
        const int bestTrackMissingOuterHits() const;

        const int hitDrop_missingMiddleHits() const;
        const int hitDrop_gsfTrackMissingMiddleHits() const;
        const int hitDrop_bestTrackMissingMiddleHits() const;

        const int hitAndTOBDrop_missingOuterHits() const;
        const int hitAndTOBDrop_gsfTrackMissingOuterHits() const;
        const int hitAndTOBDrop_bestTrackMissingOuterHits() const;

        // Debug methods for HitPattern
        const bool hasValidHitInPixelBarrelLayer(const uint16_t layer) const;
        const bool hasValidHitInPixelBarrelLayer1() const { return hasValidHitInPixelBarrelLayer(1); };
        const bool hasValidHitInPixelBarrelLayer2() const { return hasValidHitInPixelBarrelLayer(2); };
        const bool hasValidHitInPixelBarrelLayer3() const { return hasValidHitInPixelBarrelLayer(3); };

        const bool hasValidHitInPixelEndcapLayer(const uint16_t layer) const;
        const bool hasValidHitInPixelEndcapLayer1() const { return hasValidHitInPixelEndcapLayer(1); };
        const bool hasValidHitInPixelEndcapLayer2() const { return hasValidHitInPixelEndcapLayer(2); };

        const uint16_t packedPixelBarrelHitPattern() const;
        const uint16_t packedPixelEndcapHitPattern() const;

        const uint16_t firstLayerWithValidHit() const;
        const uint16_t lastLayerWithValidHit() const;

        void PrintTrackHitCategoryPatterns(const reco::HitPattern::HitCategory category) const;
        void PrintTrackHitPatternInfo() const;

#if DATA_FORMAT != MINI_AOD_2017
        const double innerP() const;
        const double outerP() const;
        const double fbrem() const;
        const double bremEnergy() const;
#endif

        const bool inTOBCrack() const;

        const float deltaRToClosestPFElectron() const { return this->deltaRToClosestPFElectron_; };
        const float deltaRToClosestPFMuon()     const { return this->deltaRToClosestPFMuon_; };
        const float deltaRToClosestPFChHad()    const { return this->deltaRToClosestPFChHad_; };

        const float energyOfElectron() const { return energyGivenMass(0.000510998928); };
        const float energyOfMuon()     const { return energyGivenMass(0.1056583715); };
        const float energyOfTau()      const { return energyGivenMass(1.77686); };
        const float energyOfPion()     const { return energyGivenMass(0.13957018); };
        const float energyOfProton()   const { return energyGivenMass(0.938272046); };

        // number of hits differentiated by location in detector
        const unsigned char numberOfTrackerHits()     const { return this->hitPattern().trackerLayersWithMeasurement(); };
        const unsigned char numberOfPixelHits()       const { return this->hitPattern().pixelLayersWithMeasurement(); };
        const unsigned char numberOfStripHits()       const { return this->hitPattern().stripLayersWithMeasurement(); };
        const unsigned char numberOfPixelBarrelHits() const { return this->hitPattern().pixelBarrelLayersWithMeasurement(); };
        const unsigned char numberOfPixelEndcapHits() const { return this->hitPattern().pixelEndcapLayersWithMeasurement(); };
        const unsigned char numberOfStripTIBHits()    const { return this->hitPattern().stripTIBLayersWithMeasurement(); };
        const unsigned char numberOfStripTIDHits()    const { return this->hitPattern().stripTIDLayersWithMeasurement(); };
        const unsigned char numberOfStripTOBHits()    const { return this->hitPattern().stripTOBLayersWithMeasurement(); };
        const unsigned char numberOfStripTECHits()    const { return this->hitPattern().stripTECLayersWithMeasurement(); };

        // missing hits differentiated by location on track
        const unsigned char missingInnerHits()  const { return this->hitPattern().trackerLayersWithoutMeasurement(reco::HitPattern::MISSING_INNER_HITS); };
        const unsigned char missingMiddleHits() const { return this->hitPattern().trackerLayersWithoutMeasurement(reco::HitPattern::TRACK_HITS); };
        const unsigned char missingOuterHits()  const { return this->hitPattern().trackerLayersWithoutMeasurement(reco::HitPattern::MISSING_OUTER_HITS); };

        // missing hits differentiated by location in detector
        const unsigned char missingTrackerHits() const;
        const unsigned char missingPixelHits() const;
        const unsigned char missingStripHits() const;
        const unsigned char missingPixelBarrelHits() const;
        const unsigned char missingPixelEndcapHits() const;
        const unsigned char missingStripTIBHits() const;
        const unsigned char missingStripTIDHits() const;
        const unsigned char missingStripTOBHits() const;
        const unsigned char missingStripTECHits() const;

        // expected hits differentiated by location in detector
        const unsigned char expectedTrackerHits() const;
        const unsigned char expectedPixelHits() const;
        const unsigned char expectedStripHits() const;
        const unsigned char expectedPixelBarrelHits() const;
        const unsigned char expectedPixelEndcapHits() const;
        const unsigned char expectedStripTIBHits() const;
        const unsigned char expectedStripTIDHits() const;
        const unsigned char expectedStripTOBHits() const;
        const unsigned char expectedStripTECHits() const;

    private:
        double dRMinJet_;
        double minDeltaRForFiducialTrack_;

        bool isFiducialElectronTrack_;
        bool isFiducialMuonTrack_;

        double maxSigmaForFiducialElectronTrack_;
        double maxSigmaForFiducialMuonTrack_;

        edm::Ref<vector<reco::GsfTrack> > matchedGsfTrack_;
        double dRToMatchedGsfTrack_;

        double maxDeltaR_;

        const map<DetId, vector<double> > * EcalAllDeadChannelsValMap_;
        const map<DetId, vector<int> >    * EcalAllDeadChannelsBitMap_;

        bool isFiducialECALTrack_;

        double dropTOBProbability_;
        double preTOBDropHitProbability_;
        double postTOBDropHitProbability_;
        double hitProbability_;

        bool dropTOBDecision_;
        vector<bool> dropHitDecisions_;
        vector<bool> dropMiddleHitDecisions_;

        float deltaRToClosestPFElectron_;
        float deltaRToClosestPFMuon_;
        float deltaRToClosestPFChHad_;

        const bool isFiducialTrack(const EtaPhiList &, const double, double &) const;
        const edm::Ref<vector<reco::GsfTrack> > &findMatchedGsfTrack(const edm::Handle<vector<reco::GsfTrack> > &, edm::Ref<vector<reco::GsfTrack> > &, double &) const;
        const bool isBadGsfTrack(const reco::GsfTrack &) const;
        int isCloseToBadEcalChannel(const double &);
        template<class T> const int extraMissingMiddleHits(const T &) const;
        template<class T> const int extraMissingOuterHits(const T &) const;

        const double energyGivenMass(const double) const;
    };

} // namespace osu

#endif // IS_VALID(tracks)

// If "DISAPP_TRKS" is defined by setupFramework.py with the -d option,
// osu::Track and osu::SecondaryTrack will instead be defined by
// DisappearingTrack.h

#ifndef DISAPP_TRKS
namespace osu {
#if IS_VALID(tracks)
  typedef TrackBase Track
#if IS_VALID(secondaryTracks)
  typedef TrackBase SecondaryTrack
#endif
#endif
}
#endif // ifndef DISAPP_TRKS

#endif // ifndef TRACK_BASE
