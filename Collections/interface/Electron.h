#ifndef OSU_ELECTRON
#define OSU_ELECTRON

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM  
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#endif
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
        const int missingInnerHits () const;
        const float AEff () const;
        const float rho() const;
        const bool vtxFitConversion() const;
        void set_rho (float value) { rho_  = value; }  
        void set_AEff (float value) { AEff_  = value; }  
        void set_missingInnerHits(int value) { missingInnerHits_ = value; }  
        void set_vtxFitConversion(bool value) { vtxFitConversion_ = value; }  
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
        bool vtxFitConversion_;
        float rho_;
        int missingInnerHits_;
        float AEff_;
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
