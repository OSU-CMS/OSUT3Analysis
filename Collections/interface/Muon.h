#ifndef OSU_MUON
#define OSU_MUON

#include "TVector2.h"

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"
#include "OSUT3Analysis/Collections/interface/Met.h"

#if IS_VALID(muons)

#ifndef STOPPPED_PTLS
namespace osu
{
  class Muon : public GenMatchable<TYPE(muons), 13>
    {
      public:
        Muon ();
        Muon (const TYPE(muons) &);
        Muon (const TYPE(muons) &, const edm::Handle<vector<osu::Mcparticle> > &);
        Muon (const TYPE(muons) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &);
        Muon (const TYPE(muons) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &, const osu::Met &);
        ~Muon ();

        const float rho() const;
        const double pfdBetaIsoCorr () const;
        const double sumChargedHadronPtCorr () const;
        const double sumPUPtCorr () const;
        const int muonPVIndex () const;
        const double genVx () const;
        const double genVy () const;
        const double genPx () const;
        const double genPy () const;
        const double genD0 () const;
        const double d0SmearingVal () const;
        const bool isTightMuonWRTVtx() const { return isTightMuonWRTVtx_; }
        const bool isSoftMuonWRTVtx() const { return isSoftMuonWRTVtx_; }
        void set_rho (float value) { rho_  = value; }
        void set_isTightMuonWRTVtx(const bool isTightMuon);
        void set_isSoftMuonWRTVtx(const bool isSoftMuon);
        void set_pfdBetaIsoCorr (double value) { pfdBetaIsoCorr_  = value; };
        void set_sumChargedHadronPtCorr (double value) { sumChargedHadronPtCorr_  = value; };
        void set_sumPUPtCorr (double value) { sumPUPtCorr_  = value; };
        void set_muonPVIndex (int value) { muonPVIndex_  = value; };
        void set_genVx (double value) { genVx_  = value; };
        void set_genVy (double value) { genVy_  = value; };
        void set_genPx (double value) { genPx_  = value; };
        void set_genPy (double value) { genPy_  = value; };
        void set_genD0 (double value) { genD0_  = value; };
        void set_d0SmearingVal (double value) { d0SmearingVal_  = value; };

        void set_hltMatch (const string, const bool);

	      void set_match_HLT_IsoMu27_v (const bool);
        void set_match_HLT_IsoMu24_v (const bool);
        void set_match_HLT_IsoTkMu24_v (const bool);
        void set_match_HLT_IsoMu20_v (const bool);
        void set_match_HLT_IsoTkMu20_v (const bool);

        const int missingInnerHitsFromTrackerLayersWithoutMeasurements () const;
        const int missingMiddleHitsFromTrackerLayersWithoutMeasurements () const;
        const int missingOuterHitsFromTrackerLayersWithoutMeasurements () const;

        const bool hasValidHitInPixelBarrelLayer(const uint16_t layer) const;
        const bool hasValidHitInPixelBarrelLayer1() const { return hasValidHitInPixelBarrelLayer(1); };
        const bool hasValidHitInPixelBarrelLayer2() const { return hasValidHitInPixelBarrelLayer(2); };
        const bool hasValidHitInPixelBarrelLayer3() const { return hasValidHitInPixelBarrelLayer(3); };
        const bool hasValidHitInPixelBarrelLayer4() const { return hasValidHitInPixelBarrelLayer(4); };

        const bool hasValidHitInPixelEndcapLayer(const uint16_t layer) const;
        const bool hasValidHitInPixelEndcapLayer1() const { return hasValidHitInPixelEndcapLayer(1); };
        const bool hasValidHitInPixelEndcapLayer2() const { return hasValidHitInPixelEndcapLayer(2); };
        const bool hasValidHitInPixelEndcapLayer3() const { return hasValidHitInPixelEndcapLayer(3); };

        const int layerOfFirstValidPixelHit () const;

        const double metMinusOnePt () const;
        const double metMinusOnePx () const;
        const double metMinusOnePy () const;
        const double metMinusOnePhi () const;

        const double metNoMuMinusOnePt () const;
        const double metNoMuMinusOnePx () const;
        const double metNoMuMinusOnePy () const;
        const double metNoMuMinusOnePhi () const;

        const bool get_hltMatch (const string) const;

        const bool match_HLT_IsoMu20_v   () const { return get_hltMatch("HLT_IsoMu20_v"); };
        const bool match_HLT_IsoTkMu20_v () const { return get_hltMatch("HLT_IsoTkMu20_v"); };
        const bool match_HLT_IsoMu24_v   () const { return get_hltMatch("HLT_IsoMu24_v"); };
        const bool match_HLT_IsoTkMu24_v () const { return get_hltMatch("HLT_IsoTkMu24_v"); };
        const bool match_HLT_IsoMu27_v   () const { return get_hltMatch("HLT_IsoMu27_v"); };

      private:
        float rho_;
        bool isTightMuonWRTVtx_;
        bool isSoftMuonWRTVtx_;
        double pfdBetaIsoCorr_;
        double sumChargedHadronPtCorr_;
        int muonPVIndex_;
        double sumPUPtCorr_;
        double genVx_;
        double genVy_;
        double genPx_;
        double genPy_;
        double genD0_;
        double d0SmearingVal_;

        double metMinusOnePt_;
        double metMinusOnePx_;
        double metMinusOnePy_;
        double metMinusOnePhi_;

        double metNoMuMinusOnePt_;
        double metNoMuMinusOnePx_;
        double metNoMuMinusOnePy_;
        double metNoMuMinusOnePhi_;

        map<string, bool> hltMatches_;
    };
}
#else // STOPPPED_PTLS
namespace osu
{
  class Muon : public TYPE(muons)
    {
      public:
        Muon ();
        Muon (const TYPE(muons) &);
        ~Muon ();
    };
}
#endif // STOPPPED_PTLS

#else

namespace osu
{
  typedef TYPE(muons) Muon;
}

#endif

#endif
