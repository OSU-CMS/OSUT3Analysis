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
  class TrackBase : public GenMatchable<TYPE(tracks), 0> {
    public:
      TrackBase();
      TrackBase(const TYPE(tracks) &);
      TrackBase(const TYPE(tracks) &, 
                const edm::Handle<vector<osu::Mcparticle> > &);
      TrackBase(const TYPE(tracks) &, 
                const edm::Handle<vector<osu::Mcparticle> > &, 
                const edm::ParameterSet &);
      TrackBase(const TYPE(tracks) &, 
                const edm::Handle<vector<osu::Mcparticle> > &, 
                const edm::ParameterSet &, 
                const edm::Handle<vector<reco::GsfTrack> > &, 
                const EtaPhiList &, 
                const EtaPhiList &);
      TrackBase(const TYPE(tracks) &, 
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

      ~TrackBase();

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

      // Methods for HitPattern
      const bool hasValidHitInPixelBarrelLayer(const uint16_t layer) const;
      const bool hasValidHitInPixelBarrelLayer1() const { return hasValidHitInPixelBarrelLayer(1); };
      const bool hasValidHitInPixelBarrelLayer2() const { return hasValidHitInPixelBarrelLayer(2); };
      const bool hasValidHitInPixelBarrelLayer3() const { return hasValidHitInPixelBarrelLayer(3); };
      const bool hasValidHitInPixelBarrelLayer4() const { return hasValidHitInPixelBarrelLayer(4); };

      const bool hasValidHitInPixelEndcapLayer(const uint16_t layer) const;
      const bool hasValidHitInPixelEndcapLayer1() const { return hasValidHitInPixelEndcapLayer(1); };
      const bool hasValidHitInPixelEndcapLayer2() const { return hasValidHitInPixelEndcapLayer(2); };
      const bool hasValidHitInPixelEndcapLayer3() const { return hasValidHitInPixelEndcapLayer(3); };

      const bool hasMissingHitInPixelBarrelLayer(const uint16_t layer) const;
      const bool hasMissingHitInPixelBarrelLayer1() const { return hasMissingHitInPixelBarrelLayer(1); };
      const bool hasMissingHitInPixelBarrelLayer2() const { return hasMissingHitInPixelBarrelLayer(2); };
      const bool hasMissingHitInPixelBarrelLayer3() const { return hasMissingHitInPixelBarrelLayer(3); };
      const bool hasMissingHitInPixelBarrelLayer4() const { return hasMissingHitInPixelBarrelLayer(4); };

      const bool hasMissingHitInPixelEndcapLayer(const uint16_t layer) const;
      const bool hasMissingHitInPixelEndcapLayer1() const { return hasMissingHitInPixelEndcapLayer(1); };
      const bool hasMissingHitInPixelEndcapLayer2() const { return hasMissingHitInPixelEndcapLayer(2); };
      const bool hasMissingHitInPixelEndcapLayer3() const { return hasMissingHitInPixelEndcapLayer(3); };

      const uint16_t packedPixelBarrelHitPattern() const;
      const uint16_t packedPixelEndcapHitPattern() const;

      const uint16_t firstLayerWithValidHit() const;
      const uint16_t lastLayerWithValidHit() const;

      void PrintTrackHitCategoryPatterns(const reco::HitPattern::HitCategory category) const;
      void PrintTrackHitPatternInfo() const;

#if DATA_FORMAT != MINI_AOD_2017 && DATA_FORMAT != MINI_AOD_2022_CUSTOM
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
      const float energyOfMassless() const { return energyGivenMass(0.0); };

      // counts the number of _layers_ with a VALID hit, in each area of the detector
      const unsigned char numberOfTrackerHits()     const { return this->hitPattern().trackerLayersWithMeasurement(); };
      const unsigned char numberOfPixelHits()       const { return this->hitPattern().pixelLayersWithMeasurement(); };
      const unsigned char numberOfStripHits()       const { return this->hitPattern().stripLayersWithMeasurement(); };
      const unsigned char numberOfPixelBarrelHits() const { return this->hitPattern().pixelBarrelLayersWithMeasurement(); };
      const unsigned char numberOfPixelEndcapHits() const { return this->hitPattern().pixelEndcapLayersWithMeasurement(); };
      const unsigned char numberOfStripTIBHits()    const { return this->hitPattern().stripTIBLayersWithMeasurement(); };
      const unsigned char numberOfStripTIDHits()    const { return this->hitPattern().stripTIDLayersWithMeasurement(); };
      const unsigned char numberOfStripTOBHits()    const { return this->hitPattern().stripTOBLayersWithMeasurement(); };
      const unsigned char numberOfStripTECHits()    const { return this->hitPattern().stripTECLayersWithMeasurement(); };

      // "missing hits" -- actually counts the number of _layers_ that contain a MISSING hit
      // and do not also have a VALID hit (in cases of stereo modules where you could miss one side)
      // It does not count INACTIVE, BAD, or NULL hits towards this; be careful!

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

      // "expected hits" -- number of layers with either a VALID or MISSING hit
      // Does not count INACTIVE, BAD, or NULL hits; be careful!

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

      // "expected hits, include inactive" -- number of layers with _any_ hit
      // Includes VALID, MISSING, BAD, or INACTIVE. This should be the total
      // number of layers the trajectory passed through, except if there is
      // this bug found in AN-15-213 we described. There is also NULL_RETURN
      // which nominally is where the track is outside the acceptance or in
      // a gap of the layer in question; NULL_RETURN hits are not included
      // in this number.

#if CMSSW_VERSION_CODE >= CMSSW_VERSION(9,4,0)
      // expected hits (including inactive modules) differentiated by location in detector
      const unsigned char expectedIncludeInactiveTrackerHits() const;
      const unsigned char expectedIncludeInactivePixelHits() const;
      const unsigned char expectedIncludeInactiveStripHits() const;
      const unsigned char expectedIncludeInactivePixelBarrelHits() const;
      const unsigned char expectedIncludeInactivePixelEndcapHits() const;
      const unsigned char expectedIncludeInactiveStripTIBHits() const;
      const unsigned char expectedIncludeInactiveStripTIDHits() const;
      const unsigned char expectedIncludeInactiveStripTOBHits() const;
      const unsigned char expectedIncludeInactiveStripTECHits() const;
#endif
      
      // "lost" hits -- the total number of MISSING hits; same as missingInnerHits (etc)
      // except in the case of a stereo module, this will count a VALID+MISSING as one
      // missing, and missingInnerHits would count that as zero.
      const unsigned char numberOfLostInnerHits()  const { return this->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS); };
      const unsigned char numberOfLostMiddleHits() const { return this->hitPattern().numberOfLostHits(reco::HitPattern::TRACK_HITS); };
      const unsigned char numberOfLostOuterHits()  const { return this->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_OUTER_HITS); };
      const unsigned char numberOfLostHits() const { return this->numberOfLostInnerHits() + this->numberOfLostMiddleHits() + this->numberOfLostOuterHits(); };

#if CMSSW_VERSION_CODE >= CMSSW_VERSION(9,4,0)
      const unsigned char numberOfTotallyOffOrBadInnerHits()  const { return this->hitPattern().trackerLayersTotallyOffOrBad(reco::HitPattern::MISSING_INNER_HITS); };
      const unsigned char numberOfTotallyOffOrBadMiddleHits() const { return this->hitPattern().trackerLayersTotallyOffOrBad(reco::HitPattern::TRACK_HITS); };
      const unsigned char numberOfTotallyOffOrBadOuterHits()  const { return this->hitPattern().trackerLayersTotallyOffOrBad(reco::HitPattern::MISSING_OUTER_HITS); };
      const unsigned char numberOfTotallyOffOrBadHits()       const { return this->numberOfTotallyOffOrBadInnerHits() + 
                                                                             this->numberOfTotallyOffOrBadMiddleHits() + 
                                                                             this->numberOfTotallyOffOrBadOuterHits(); };
#endif

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

      const std::array<reco::HitPattern::HitCategory, 3> hitCategories = {{reco::HitPattern::TRACK_HITS, reco::HitPattern::MISSING_INNER_HITS, reco::HitPattern::MISSING_OUTER_HITS}};
  };

  //////////////////////////////////////////////////
  // osu::SecondaryTracks
  //////////////////////////////////////////////////

#if IS_VALID(secondaryTracks)
  class SecondaryTrackBase : public TrackBase {
    public:
      SecondaryTrackBase();
      SecondaryTrackBase(const TYPE(tracks) &);
      SecondaryTrackBase(const TYPE(tracks) &, 
                         const edm::Handle<vector<osu::Mcparticle> > &);
      SecondaryTrackBase(const TYPE(tracks) &, 
                         const edm::Handle<vector<osu::Mcparticle> > &, 
                         const edm::ParameterSet &);
      SecondaryTrackBase(const TYPE(tracks) &, 
                         const edm::Handle<vector<osu::Mcparticle> > &, 
                         const edm::ParameterSet &, 
                         const edm::Handle<vector<reco::GsfTrack> > &, 
                         const EtaPhiList &, 
                         const EtaPhiList &);
      SecondaryTrackBase(const TYPE(tracks) &, 
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

      ~SecondaryTrackBase();
  };
#endif // IS_VALID(secondaryTracks)

} // namespace osu

#endif // IS_VALID(tracks)

// If "DISAPP_TRKS" is defined by setupFramework.py with the -d option,
// osu::Track and osu::SecondaryTrack will instead be defined by
// DisappearingTrack.h

#ifndef DISAPP_TRKS
namespace osu {
#if IS_VALID(tracks)
  typedef osu::TrackBase Track;
#if IS_VALID(secondaryTracks)
  typedef osu::SecondaryTrackBase SecondaryTrack;
#else
  typedef TYPE(secondaryTracks) SecondaryTrack;
#endif // IS_VALID(secondaryTracks)
#else
  typedef TYPE(tracks) Track;
  typedef TYPE(secondaryTracks) SecondaryTrack;
#endif // IS_VALID(tracks)
}
#endif // ifndef DISAPP_TRKS

#endif // ifndef TRACK_BASE
