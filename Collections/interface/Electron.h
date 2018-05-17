#ifndef OSU_ELECTRON
#define OSU_ELECTRON

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"
#include "OSUT3Analysis/Collections/interface/Met.h"

#if IS_VALID(electrons)

namespace osu
{
#if DATA_FORMAT_FROM_MINIAOD
  class Electron : public GenMatchable<TYPE(electrons), 11>
    {
      public:
        Electron ();
        Electron (const TYPE(electrons) &);
        Electron (const TYPE(electrons) &, const edm::Handle<vector<osu::Mcparticle> > &);
        Electron (const TYPE(electrons) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &);
        Electron (const TYPE(electrons) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &, const osu::Met &);
        const float rho() const;
        const float AEff () const;
        const double pfdRhoIsoCorr () const;
        const double sumChargedHadronPtCorr () const;
        const double sumPUPtCorr () const;
        const int electronPVIndex () const;
        const int missingInnerHitsFromAllHits () const;
        const int missingInnerHitsFromTrackerLayersWithoutMeasurements () const;
        const int missingMiddleHitsFromTrackerLayersWithoutMeasurements () const;
        const int missingOuterHitsFromTrackerLayersWithoutMeasurements () const;
        const bool passesTightID_noIsolation () const;
        const bool match_HLT_Ele25_eta2p1_WPTight_Gsf_v () const;
        const bool match_HLT_Ele22_eta2p1_WPLoose_Gsf_v () const;
        void set_rho (float value) { rho_  = value; }
        void set_AEff (float value) { AEff_  = value; }
        void set_pfdRhoIsoCorr (double value) { pfdRhoIsoCorr_  = value; }
        void set_sumChargedHadronPtCorr (double value) { sumChargedHadronPtCorr_  = value; };
        void set_sumPUPtCorr (double value) { sumPUPtCorr_  = value; };
        void set_electronPVIndex (int value) { electronPVIndex_  = value; };
        void set_passesTightID_noIsolation (const reco::BeamSpot &, const TYPE(primaryvertexs) &, const edm::Handle<vector<reco::Conversion> > &);
        void set_match_HLT_Ele25_eta2p1_WPTight_Gsf_v (const bool);
        void set_match_HLT_Ele22_eta2p1_WPLoose_Gsf_v (const bool);

        const double metMinusOnePt () const;
        const double metMinusOnePx () const;
        const double metMinusOnePy () const;
        const double metMinusOnePhi () const;

        const double metNoMuMinusOnePt () const;
        const double metNoMuMinusOnePx () const;
        const double metNoMuMinusOnePy () const;
        const double metNoMuMinusOnePhi () const;

        const double metMinusOneUpPt () const;
        const double metMinusOneUpPx () const;
        const double metMinusOneUpPy () const;
        const double metMinusOneUpPhi () const;

        const double metNoMuMinusOneUpPt () const;
        const double metNoMuMinusOneUpPx () const;
        const double metNoMuMinusOneUpPy () const;
        const double metNoMuMinusOneUpPhi () const;
#else
  class Electron : public GenMatchable<TYPE(electrons), 11>
    {
      public:
        Electron ();
        Electron (const TYPE(electrons) &);
        Electron (const TYPE(electrons) &, const edm::Handle<vector<osu::Mcparticle> > &);
        Electron (const TYPE(electrons) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &);
#endif
        ~Electron ();

#if DATA_FORMAT_FROM_MINIAOD
      private:
        float rho_;
        float AEff_;
        double pfdRhoIsoCorr_;
        double sumChargedHadronPtCorr_;
        int electronPVIndex_;
        double sumPUPtCorr_;
        bool passesTightID_noIsolation_;
        bool match_HLT_Ele25_eta2p1_WPTight_Gsf_v_;
        bool match_HLT_Ele22_eta2p1_WPLoose_Gsf_v_;

        double metMinusOnePt_;
        double metMinusOnePx_;
        double metMinusOnePy_;
        double metMinusOnePhi_;

        double metNoMuMinusOnePt_;
        double metNoMuMinusOnePx_;
        double metNoMuMinusOnePy_;
        double metNoMuMinusOnePhi_;

        double metMinusOneUpPt_;
        double metMinusOneUpPx_;
        double metMinusOneUpPy_;
        double metMinusOneUpPhi_;

        double metNoMuMinusOneUpPt_;
        double metNoMuMinusOneUpPx_;
        double metNoMuMinusOneUpPy_;
        double metNoMuMinusOneUpPhi_;
#endif

    };
}

#else

namespace osu
{
  typedef TYPE(electrons) Electron;
}

#endif

#endif
