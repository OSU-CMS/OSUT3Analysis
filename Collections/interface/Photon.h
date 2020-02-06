#ifndef OSU_PHOTON
#define OSU_PHOTON

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"

#if IS_VALID(photons)

namespace osu
{
  class Photon : public GenMatchable<TYPE(photons), 22>
    {
      public:
        Photon ();
        Photon (const TYPE(photons) &);
        Photon (const TYPE(photons) &, const edm::Handle<vector<osu::Mcparticle> > &);
        Photon (const TYPE(photons) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &);
        ~Photon ();

        const float rho() const;
        const float Aeff_neutralHadron () const;
        const float Aeff_chargedHadron () const;
        const float Aeff_photon () const;

        const bool passesVID_looseID () const;
        const bool passesVID_mediumID () const;
        const bool passesVID_tightID () const;

        void set_rho(float v) { rho_  = v; }
        void set_passesVID_looseID (const bool);
        void set_passesVID_mediumID (const bool);
        void set_passesVID_tightID (const bool);
        void set_AEff_neutralHadron(float v) { AEff_neutralHadron_ = v; }
        void set_Aeff_chargedHadron(float v) { Aeff_chargedHadron_ = v; }
        void set_Aeff_photon(float v) { Aeff_photon_ = v; }

      private:
        float rho_;
        bool passesVID_looseID_;
        bool passesVID_mediumID_;
        bool passesVID_tightID_;
        float AEff_neutralHadron_;
        float Aeff_chargedHadron_;
        float Aeff_photon_;
    };
}

#else

namespace osu
{
  typedef TYPE(photons) Photon;
}

#endif

#endif
