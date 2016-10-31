#ifndef OSU_BJET
#define OSU_BJET

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"

#if IS_VALID(bjets)

#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD
namespace osu
{
  class Bjet : public GenMatchable<TYPE(bjets), 0>
    {
      public:
        Bjet ();
        Bjet (const TYPE(bjets) &);
        Bjet (const TYPE(bjets) &, const edm::Handle<vector<osu::Mcparticle> > &);
        Bjet (const TYPE(bjets) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &);
        ~Bjet ();

        const int matchedToLepton () const;
        const float pfCombinedSecondaryVertexV2BJetTags () const;
        const float pfCombinedInclusiveSecondaryVertexV2BJetTags () const;
        const float pileupJetId () const;

        const float jecUncertainty () const;
        const float jer () const;
        const float jerSF () const;
        const float jerSFUp () const;
        const float jerSFDown () const;
        const float smearedPt () const;
        const float smearedPtUp () const;
        const float smearedPtDown () const;

        void set_matchedToLepton (float value) { matchedToLepton_  = value; }
        void set_pfCombinedSecondaryVertexV2BJetTags (float value) { pfCombinedSecondaryVertexV2BJetTags_ = value;}
        void set_pfCombinedInclusiveSecondaryVertexV2BJetTags (float value) { pfCombinedInclusiveSecondaryVertexV2BJetTags_ = value;}
        void set_pileupJetId (float value) { pileupJetId_ = value;}

        void set_jecUncertainty (float value) { jecUncertainty_ = value;}
        void set_jetPtResolution (float value) { jetPtResolution_ = value;}
        void set_setJetPtResolutionSF (float value, float value_up, float value_down) {
          jetPtResolutionSF_ = value;
          jetPtResolutionSFUp_ = value_up;
          jetPtResolutionSFDown_ = value_down;
        }

        void set_smearedPt (float value) { smearedPt_ = value;}
        void set_smearedPtUp (float value) { smearedPtUp_ = value;}
        void set_smearedPtDown (float value) { smearedPtDown_ = value;}

      private:
        int matchedToLepton_;
        float pfCombinedSecondaryVertexV2BJetTags_;
        float pfCombinedInclusiveSecondaryVertexV2BJetTags_;
        float pileupJetId_;
        float jecUncertainty_;
        float jetPtResolution_;
        float jetPtResolutionSF_;
        float jetPtResolutionSFUp_;
        float jetPtResolutionSFDown_;
        float smearedPt_;
        float smearedPtUp_;
        float smearedPtDown_;
    };
}
#elif DATA_FORMAT == AOD_CUSTOM
namespace osu
{
  class Bjet : public TYPE(bjets)
    {
      public:
        Bjet ();
        Bjet (const TYPE(bjets) &);
        ~Bjet ();
    };
}
#endif

#else

namespace osu
{
  typedef TYPE(bjets) Bjet;
}

#endif

#endif
