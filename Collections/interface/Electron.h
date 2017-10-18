#ifndef OSU_ELECTRON
#define OSU_ELECTRON

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"
#include "OSUT3Analysis/Collections/interface/Met.h"

#if IS_VALID(electrons)

namespace osu
{
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_2017 || DATA_FORMAT == MINI_AOD_CUSTOM
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
        const double genD0 () const;
        const double d0 () const;
        const double d0Sig () const;
        const double dz () const;
        const int electronPVIndex () const;
        const int missingInnerHits () const;
        const int missingMiddleHits () const;
        const int missingOuterHits () const;
        const bool passesTightID_noIsolation () const;
        void set_rho (float value) { rho_  = value; }
        void set_AEff (float value) { AEff_  = value; }
        void set_pfdRhoIsoCorr (double value) { pfdRhoIsoCorr_  = value; }
        void set_sumChargedHadronPtCorr (double value) { sumChargedHadronPtCorr_  = value; };
        void set_sumPUPtCorr (double value) { sumPUPtCorr_  = value; };
        void set_electronPVIndex (int value) { electronPVIndex_  = value; };
        void set_genD0 (double value) { genD0_  = value; };
        void set_d0 (double value) { d0_  = value; };
        void set_d0Sig (double value) { d0Sig_  = value; };
        void set_dz (double value) { dz_  = value; };
        void set_passesTightID_noIsolation (const reco::BeamSpot &, const TYPE(primaryvertexs) &, const edm::Handle<vector<reco::Conversion> > &);

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

#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_2017 || DATA_FORMAT == MINI_AOD_CUSTOM
      private:
        float rho_;
        float AEff_;
        double pfdRhoIsoCorr_;
        double sumChargedHadronPtCorr_;
        int electronPVIndex_;
        double sumPUPtCorr_;
        double genD0_;
        double d0_;
        double d0Sig_;
        double dz_;
        bool passesTightID_noIsolation_;

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
