#ifndef OSU_ELECTRON
#define OSU_ELECTRON

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"
#if IS_VALID(electrons)

namespace osu
{
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM
  class Electron : public GenMatchable<TYPE(electrons), 11>
    {
      public:
        Electron ();
        Electron (const TYPE(electrons) &);
        Electron (const TYPE(electrons) &, const edm::Handle<vector<osu::Mcparticle> > &);
        Electron (const TYPE(electrons) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &);
        const float rho() const;
        const float AEff () const;
        const double pfdRhoIsoCorr () const;
        const int missingInnerHits () const;
        const bool passesTightID_noIsolation () const;
        void set_rho (float value) { rho_  = value; }  
        void set_AEff (float value) { AEff_  = value; }  
        void set_pfdRhoIsoCorr (double value) { pfdRhoIsoCorr_  = value; }  
        void set_missingInnerHits(int value) { missingInnerHits_ = value; }  
        void set_passesTightID_noIsolation (const reco::BeamSpot &, const TYPE(primaryvertexs) &, const edm::Handle<vector<reco::Conversion> > &);
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
        
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM     
      private:
        float rho_;
        int missingInnerHits_;
        float AEff_;
        double pfdRhoIsoCorr_;
        bool passesTightID_noIsolation_;
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
