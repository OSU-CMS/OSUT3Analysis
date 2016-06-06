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
        void set_matchedToLepton (float value) { matchedToLepton_  = value; }
        void set_pfCombinedSecondaryVertexV2BJetTags (float value) { pfCombinedSecondaryVertexV2BJetTags_ = value;}
        void set_pfCombinedInclusiveSecondaryVertexV2BJetTags (float value) { pfCombinedInclusiveSecondaryVertexV2BJetTags_ = value;}  
        void set_pileupJetId (float value) { pileupJetId_ = value;}  
      private:
        int matchedToLepton_;
        float pfCombinedSecondaryVertexV2BJetTags_;
        float pfCombinedInclusiveSecondaryVertexV2BJetTags_;
        float pileupJetId_;
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
