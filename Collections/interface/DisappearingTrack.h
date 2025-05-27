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

#if DATA_FORMAT_FROM_MINIAOD && ( DATA_FORMAT_IS_2017 || DATA_FORMAT_IS_2022 )
#include "DataFormats/PatCandidates/interface/IsolatedTrack.h"
#include "OSUT3Analysis/Collections/interface/Met.h"
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
#elif DATA_FORMAT_FROM_MINIAOD && DATA_FORMAT_IS_2022
                        const edm::Handle<vector<pat::IsolatedTrack> > &);
#else
                        const edm::Handle<vector<CandidateTrack> > &);
#endif

      ~DisappearingTrack ();

      enum RhoType { All, Calo, CentralCalo };
      enum CaloType { Sum, EM, Had };

#if DATA_FORMAT != MINI_AOD_2022_CUSTOM && DATA_FORMAT != MINI_AOD_ONLY_2022_CUSTOM
      const edm::Ref<vector<CandidateTrack> > matchedCandidateTrack () const { return matchedCandidateTrack_; };
      const double dRToMatchedCandidateTrack () const { return (IS_INVALID(dRToMatchedCandidateTrack_)) ? MAX_DR : dRToMatchedCandidateTrack_; };
#endif

#if DATA_FORMAT_FROM_MINIAOD && (DATA_FORMAT_IS_2017 || DATA_FORMAT_IS_2022)
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

#if DATA_FORMAT_FROM_MINIAOD && ( DATA_FORMAT_IS_2017 || DATA_FORMAT_IS_2022)
      void set_isoTrackIsolation(const edm::Handle<vector<pat::IsolatedTrack> > &);
      //void set_caloNewEMDRp5 (double value) { caloNewEMDRp5_  = value; }; //mcarrigan
      void set_deepSetsScore(float value) { deepSetsElectronScore_ = value; };
      void set_fakeTrackScore(float value) { fakeTrackScore_ = value; };

      const float deepSetsElectronScore () const { return this->deepSetsElectronScore_; };
      const float fakeTrackScore() const { return this->fakeTrackScore_; };
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

      //Calo energies
      const float caloNewEMDRp5 ()  const { return this->caloNewEMDRp5_; }; //mcarrigan
      const float caloNewHadDRp5 () const { return this->caloNewHadDRp5_; };
      const float caloNewDRp5 ()    const { return this->caloNewEMDRp5_ + this->caloNewHadDRp5_; };

      // dR < 0.3
      const float caloNewEMDRp3 ()  const { return this->caloNewEMDRp3_; };
      const float caloNewHadDRp3 () const { return this->caloNewHadDRp3_; };
      const float caloNewDRp3 ()    const { return this->caloNewEMDRp3_ + this->caloNewHadDRp3_; };

      // dR < 0.2
      const float caloNewEMDRp2 ()  const { return this->caloNewEMDRp2_; };
      const float caloNewHadDRp2 () const { return this->caloNewHadDRp2_; };
      const float caloNewDRp2 ()    const { return this->caloNewEMDRp2_ + this->caloNewHadDRp2_; };

      // dR < 0.1
      const float caloNewEMDRp1 ()  const { return this->caloNewEMDRp1_; };
      const float caloNewHadDRp1 () const { return this->caloNewHadDRp1_; };
      const float caloNewDRp1 ()    const { return this->caloNewEMDRp1_ + this->caloNewHadDRp1_; };

      //Calo energy for calo jets
      const float caloTotFromCaloJet () const { return this->matchedCaloJetEmEnergy() + this->matchedCaloJetHadEnergy(); };

      //////////////////////////////////////
      // Get rhos
      //////////////////////////////////////

      const float rhoPUCorr ()            const { return this->rhoPUCorr_; };
      const float rhoPUCorrCalo ()        const { return this->rhoPUCorrCalo_; };
      const float rhoPUCorrCentralCalo () const { return this->rhoPUCorrCentralCalo_; };

      //////////////////////////////////////
      // Get track isolations
      //////////////////////////////////////

      const float trackIsoDRp5 ()            const { return this->trackIsoDRp5_; };
      const float trackIsoDRp3 ()            const { return this->trackIsoDRp3_; };
      const float trackIsoDRp2 ()            const { return this->trackIsoDRp2_; };
      const float trackIsoDRp1 ()            const { return this->trackIsoDRp1_; };

      const float trackIsoNoPUDRp5 ()        const { return this->trackIsoNoPUDRp5_; };
      const float trackIsoNoPUDRp3 ()        const { return this->trackIsoNoPUDRp3_; };
      const float trackIsoNoPUDRp2 ()        const { return this->trackIsoNoPUDRp2_; };
      const float trackIsoNoPUDRp1 ()        const { return this->trackIsoNoPUDRp1_; };
      
      const float trackIsoNoFakesDRp5 ()     const { return this->trackIsoNoFakesDRp5_; };
      const float trackIsoNoFakesDRp3 ()     const { return this->trackIsoNoFakesDRp3_; };
      const float trackIsoNoFakesDRp2 ()     const { return this->trackIsoNoFakesDRp2_; };
      const float trackIsoNoFakesDRp1 ()     const { return this->trackIsoNoFakesDRp1_; };
      
      const float trackIsoNoPUNoFakesDRp5 () const { return this->trackIsoNoPUNoFakesDRp5_; };
      const float trackIsoNoPUNoFakesDRp3 () const { return this->trackIsoNoPUNoFakesDRp3_; };
      const float trackIsoNoPUNoFakesDRp2 () const { return this->trackIsoNoPUNoFakesDRp2_; };
      const float trackIsoNoPUNoFakesDRp1 () const { return this->trackIsoNoPUNoFakesDRp1_; };

      const float trackIsoOldNoPUDRp5 () const { return this->trackIsoOldNoPUDRp5_; };
      const float trackIsoOldNoPUDRp3 () const { return this->trackIsoOldNoPUDRp3_; };
      const float trackIsoOldNoPUDRp2 () const { return this->trackIsoOldNoPUDRp2_; };
      const float trackIsoOldNoPUDRp1 () const { return this->trackIsoOldNoPUDRp1_; };

      const float dEdx_pixel                      () const { return this->dEdx_pixel_; };
      const float dEdxError_pixel                 () const { return this->dEdxError_pixel_; };
      const int dEdx_nSaturatedMeasurements_pixel () const { return this->dEdx_numberOfSaturatedMeasurements_pixel_; };
      const unsigned int dEdx_nMeasurements_pixel () const { return this->dEdx_numberOfMeasurements_pixel_; };

      const float dEdx_strip                      () const { return this->dEdx_strip_; };
      const float dEdxError_strip                 () const { return this->dEdxError_strip_; };
      const int dEdx_nSaturatedMeasurements_strip () const { return this->dEdx_numberOfSaturatedMeasurements_strip_; };
      const unsigned int dEdx_nMeasurements_strip () const { return this->dEdx_numberOfMeasurements_strip_; };

      // missing hits differentiated by location on track
      // re-implement these methods from osu::Track to provide a getter function when plotting osu::Track::matchedCandidateTrack()
      const unsigned char missingInnerHits_ ()  const { return this->hitPattern().trackerLayersWithoutMeasurement(reco::HitPattern::MISSING_INNER_HITS); };
      const unsigned char missingMiddleHits_ () const { return this->hitPattern().trackerLayersWithoutMeasurement(reco::HitPattern::TRACK_HITS); };
      const unsigned char missingOuterHits_ ()  const { return this->hitPattern().trackerLayersWithoutMeasurement(reco::HitPattern::MISSING_OUTER_HITS); };

      //////////////////////////////////////
      // Rho-corrected calo energies
      //////////////////////////////////////

      // New calculation that uses all rec hits in dR < 0.5 cone.
      const float caloNewNoPUDRp5 ()                   const { return caloTotNoPU(0.5, All, Sum); };
      const float caloNewNoPUDRp5JustEm ()             const { return caloTotNoPU(0.5, All, EM); };
      const float caloNewNoPUDRp5JustHad ()            const { return caloTotNoPU(0.5, All, Had); };
      
      const float caloNewNoPUDRp5Calo ()               const { return caloTotNoPU(0.5, Calo, Sum); };
      const float caloNewNoPUDRp5CaloJustEm ()         const { return caloTotNoPU(0.5, Calo, EM); };
      const float caloNewNoPUDRp5CaloJustHad ()        const { return caloTotNoPU(0.5, Calo, Had); };

      const float caloNewNoPUDRp5CentralCalo ()        const { return caloTotNoPU(0.5, CentralCalo, Sum); };
      const float caloNewNoPUDRp5CentralCaloJustEm ()  const { return caloTotNoPU(0.5, CentralCalo, EM); };
      const float caloNewNoPUDRp5CentralCaloJustHad () const { return caloTotNoPU(0.5, CentralCalo, Had); };

      // dR < 0.3
      const float caloNewNoPUDRp3 ()                   const { return caloTotNoPU(0.3, All, Sum); };
      const float caloNewNoPUDRp3JustEm ()             const { return caloTotNoPU(0.3, All, EM); };
      const float caloNewNoPUDRp3JustHad ()            const { return caloTotNoPU(0.3, All, Had); };
      
      const float caloNewNoPUDRp3Calo ()               const { return caloTotNoPU(0.3, Calo, Sum); };
      const float caloNewNoPUDRp3CaloJustEm ()         const { return caloTotNoPU(0.3, Calo, EM); };
      const float caloNewNoPUDRp3CaloJustHad ()        const { return caloTotNoPU(0.3, Calo, Had); };

      const float caloNewNoPUDRp3CentralCalo ()        const { return caloTotNoPU(0.3, CentralCalo, Sum); };
      const float caloNewNoPUDRp3CentralCaloJustEm ()  const { return caloTotNoPU(0.3, CentralCalo, EM); };
      const float caloNewNoPUDRp3CentralCaloJustHad () const { return caloTotNoPU(0.3, CentralCalo, Had); };

      // dR < 0.2
      const float caloNewNoPUDRp2 ()                   const { return caloTotNoPU(0.2, All, Sum); };
      const float caloNewNoPUDRp2JustEm ()             const { return caloTotNoPU(0.2, All, EM); };
      const float caloNewNoPUDRp2JustHad ()            const { return caloTotNoPU(0.2, All, Had); };
      
      const float caloNewNoPUDRp2Calo ()               const { return caloTotNoPU(0.2, Calo, Sum); };
      const float caloNewNoPUDRp2CaloJustEm ()         const { return caloTotNoPU(0.2, Calo, EM); };
      const float caloNewNoPUDRp2CaloJustHad ()        const { return caloTotNoPU(0.2, Calo, Had); };

      const float caloNewNoPUDRp2CentralCalo ()        const { return caloTotNoPU(0.2, CentralCalo, Sum); };
      const float caloNewNoPUDRp2CentralCaloJustEm ()  const { return caloTotNoPU(0.2, CentralCalo, EM); };
      const float caloNewNoPUDRp2CentralCaloJustHad () const { return caloTotNoPU(0.2, CentralCalo, Had); };

      // dR < 0.1
      const float caloNewNoPUDRp1 ()                   const { return caloTotNoPU(0.1, All, Sum); };
      const float caloNewNoPUDRp1JustEm ()             const { return caloTotNoPU(0.1, All, EM); };
      const float caloNewNoPUDRp1JustHad ()            const { return caloTotNoPU(0.1, All, Had); };
      
      const float caloNewNoPUDRp1Calo ()               const { return caloTotNoPU(0.1, Calo, Sum); };
      const float caloNewNoPUDRp1CaloJustEm ()         const { return caloTotNoPU(0.1, Calo, EM); };
      const float caloNewNoPUDRp1CaloJustHad ()        const { return caloTotNoPU(0.1, Calo, Had); };

      const float caloNewNoPUDRp1CentralCalo ()        const { return caloTotNoPU(0.1, CentralCalo, Sum); };
      const float caloNewNoPUDRp1CentralCaloJustEm ()  const { return caloTotNoPU(0.1, CentralCalo, EM); };
      const float caloNewNoPUDRp1CentralCaloJustHad () const { return caloTotNoPU(0.1, CentralCalo, Had); };

      // New calculation that uses calo jet energy in dR < 0.3 cone (standard distance from calo jet to track).
      const float caloNewFromCaloJetNoPUDRp3 ()                   const { return caloTotFromCaloJetNoPU(0.3, All); };
      const float caloNewFromCaloJetNoPUDRp3Calo ()               const { return caloTotFromCaloJetNoPU(0.3, Calo); };
      const float caloNewFromCaloJetNoPUDRp3CentralCalo ()        const { return caloTotFromCaloJetNoPU(0.3, CentralCalo); };

      // New calculation that uses calo jet energy in dR < 0.4 cone (standard size of a calo jet).
      const float caloNewFromCaloJetNoPUDRp4 ()                   const { return caloTotFromCaloJetNoPU(0.4, All); };
      const float caloNewFromCaloJetNoPUDRp4Calo ()               const { return caloTotFromCaloJetNoPU(0.4, Calo); };
      const float caloNewFromCaloJetNoPUDRp4CentralCalo ()        const { return caloTotFromCaloJetNoPU(0.4, CentralCalo); };

      //////////////////////////////////////
      // Set calo energies
      //////////////////////////////////////

      void set_caloNewEMDRp5 (double value) { caloNewEMDRp5_  = value; };
      void set_caloNewHadDRp5(double value) { caloNewHadDRp5_ = value; };

      void set_caloNewEMDRp3 (double value) { caloNewEMDRp3_  = value; };
      void set_caloNewHadDRp3(double value) { caloNewHadDRp3_ = value; };

      void set_caloNewEMDRp2 (double value) { caloNewEMDRp2_  = value; };
      void set_caloNewHadDRp2(double value) { caloNewHadDRp2_ = value; };

      void set_caloNewEMDRp1 (double value) { caloNewEMDRp1_  = value; };
      void set_caloNewHadDRp1(double value) { caloNewHadDRp1_ = value; };

      /////////////////////////////////////
      // Set rhos
      //////////////////////////////////////

      void set_rhoPUCorr  (double value) { rhoPUCorr_   = value; };
      void set_rhoPUCorrCalo         (double value) { rhoPUCorrCalo_   = value; };
      void set_rhoPUCorrCentralCalo  (double value) { rhoPUCorrCentralCalo_   = value; };

      //////////////////////////////////////
      // Set track isolations
      //////////////////////////////////////

      void set_trackIsoDRp5 (double value) { trackIsoDRp5_ = value; };
      void set_trackIsoDRp3 (double value) { trackIsoDRp3_ = value; };
      void set_trackIsoDRp2 (double value) { trackIsoDRp2_ = value; };
      void set_trackIsoDRp1 (double value) { trackIsoDRp1_ = value; };

      void set_trackIsoNoPUDRp5 (double value) { trackIsoNoPUDRp5_ = value; };
      void set_trackIsoNoPUDRp3 (double value) { trackIsoNoPUDRp3_ = value; };
      void set_trackIsoNoPUDRp2 (double value) { trackIsoNoPUDRp2_ = value; };
      void set_trackIsoNoPUDRp1 (double value) { trackIsoNoPUDRp1_ = value; };

      void set_trackIsoNoFakesDRp5 (double value) { trackIsoNoFakesDRp5_ = value; };
      void set_trackIsoNoFakesDRp3 (double value) { trackIsoNoFakesDRp3_ = value; };
      void set_trackIsoNoFakesDRp2 (double value) { trackIsoNoFakesDRp2_ = value; };
      void set_trackIsoNoFakesDRp1 (double value) { trackIsoNoFakesDRp1_ = value; };

      void set_trackIsoNoPUNoFakesDRp5 (double value) { trackIsoNoPUNoFakesDRp5_ = value; };
      void set_trackIsoNoPUNoFakesDRp3 (double value) { trackIsoNoPUNoFakesDRp3_ = value; };
      void set_trackIsoNoPUNoFakesDRp2 (double value) { trackIsoNoPUNoFakesDRp2_ = value; };
      void set_trackIsoNoPUNoFakesDRp1 (double value) { trackIsoNoPUNoFakesDRp1_ = value; };

      void set_trackIsoOldNoPUDRp5 (double value) { trackIsoOldNoPUDRp5_ = value; };
      void set_trackIsoOldNoPUDRp3 (double value) { trackIsoOldNoPUDRp3_ = value; };
      void set_trackIsoOldNoPUDRp2 (double value) { trackIsoOldNoPUDRp2_ = value; };
      void set_trackIsoOldNoPUDRp1 (double value) { trackIsoOldNoPUDRp1_ = value; };

      void set_dEdx_pixel (double value, 
                              double error, 
                              int nSaturatedMeasurements, 
                              unsigned int nMeasurements) { 
            dEdx_pixel_ = value; 
            dEdxError_pixel_ = error;
            dEdx_numberOfSaturatedMeasurements_pixel_ = nSaturatedMeasurements;
            dEdx_numberOfMeasurements_pixel_ = nMeasurements;
      };
      void set_dEdx_strip (double value, 
                              double error, 
                              int nSaturatedMeasurements, 
                              unsigned int nMeasurements) { 
            dEdx_strip_ = value; 
            dEdxError_strip_ = error;
            dEdx_numberOfSaturatedMeasurements_strip_ = nSaturatedMeasurements;
            dEdx_numberOfMeasurements_strip_ = nMeasurements;
      };

#if DATA_FORMAT_IS_2017 || DATA_FORMAT_IS_2022 // only makes sense with phase1 pixel upgrade
      // This could be in TrackBase, but is fairly specialized to the disappearing tracks search
      const bool isAllowedThreeLayerHitPattern() const;
#endif
#if DATA_FORMAT_FROM_MINIAOD && ( DATA_FORMAT_IS_2017 || DATA_FORMAT_IS_2022 )
      const edm::Ref<vector<pat::IsolatedTrack> > &findMatchedIsolatedTrack (const edm::Handle<vector<pat::IsolatedTrack> > &, edm::Ref<vector<pat::IsolatedTrack> > &, double &) const;
#endif

    protected:
      vector<double> eleVtx_d0Cuts_barrel_, eleVtx_d0Cuts_endcap_;
      vector<double> eleVtx_dzCuts_barrel_, eleVtx_dzCuts_endcap_;

    private:
      const edm::Ref<vector<CandidateTrack> > &findMatchedCandidateTrack (const edm::Handle<vector<CandidateTrack> > &, edm::Ref<vector<CandidateTrack> > &, double &) const;

      void set_primaryPFIsolations(const edm::Handle<vector<pat::PackedCandidate> > &);
      void set_additionalPFIsolations(const edm::Handle<vector<pat::PackedCandidate> > &, const edm::Handle<vector<pat::PackedCandidate> > &);
      void set_caloValues(); //mcarrigan

#if DATA_FORMAT != MINI_AOD_2022_CUSTOM && DATA_FORMAT != MINI_AOD_ONLY_2022_CUSTOM
      edm::Ref<vector<CandidateTrack> > matchedCandidateTrack_;
      double dRToMatchedCandidateTrack_;
      double maxDeltaR_candidateTrackMatching_;
#endif

#if DATA_FORMAT_FROM_MINIAOD && ( DATA_FORMAT_IS_2017 || DATA_FORMAT_IS_2022 )
      edm::Ref<vector<pat::IsolatedTrack> > matchedIsolatedTrack_;      
      double dRToMatchedIsolatedTrack_;
      double maxDeltaR_isolatedTrackMatching_;

      float deepSetsElectronScore_;
      float fakeTrackScore_;
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

      //float caloNewEMDRp5_; //mcarrigan
      //Additions from CandidateTracks
      const double caloTotNoPU (double, RhoType = All, CaloType = Sum) const;
      const double caloTotFromCaloJetNoPU (double, RhoType = All) const;

      float caloNewEMDRp5_;
      float caloNewHadDRp5_;

      float caloNewEMDRp3_;
      float caloNewHadDRp3_;

      float caloNewEMDRp2_;
      float caloNewHadDRp2_;

      float caloNewEMDRp1_;
      float caloNewHadDRp1_;

      float rhoPUCorr_;
      float rhoPUCorrCalo_;
      float rhoPUCorrCentralCalo_;

      float trackIsoDRp5_;
      float trackIsoDRp3_;
      float trackIsoDRp2_;
      float trackIsoDRp1_;

      float trackIsoNoPUDRp5_;
      float trackIsoNoPUDRp3_;
      float trackIsoNoPUDRp2_;
      float trackIsoNoPUDRp1_;

      float trackIsoNoFakesDRp5_;
      float trackIsoNoFakesDRp3_;
      float trackIsoNoFakesDRp2_;
      float trackIsoNoFakesDRp1_;

      float trackIsoNoPUNoFakesDRp5_;
      float trackIsoNoPUNoFakesDRp3_;
      float trackIsoNoPUNoFakesDRp2_;
      float trackIsoNoPUNoFakesDRp1_;

      float trackIsoOldNoPUDRp5_;
      float trackIsoOldNoPUDRp3_;
      float trackIsoOldNoPUDRp2_;
      float trackIsoOldNoPUDRp1_;

      //currently unset, need to put these in for the ML TreeMaker?
      float dEdx_pixel_;
      float dEdxError_pixel_;
      int dEdx_numberOfSaturatedMeasurements_pixel_;
      unsigned int dEdx_numberOfMeasurements_pixel_;

      float dEdx_strip_;
      float dEdxError_strip_;
      int dEdx_numberOfSaturatedMeasurements_strip_;
      unsigned int dEdx_numberOfMeasurements_strip_;

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
#elif DATA_FORMAT_FROM_MINIAOD && DATA_FORMAT_IS_2022
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
