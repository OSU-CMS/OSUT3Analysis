#ifndef OSU_JET
#define OSU_JET

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"

#if IS_VALID(jets)

#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD
namespace osu
{
  class Jet : public GenMatchable<TYPE(jets), 0>
    {
      public:
        Jet ();
        Jet (const TYPE(jets) &);
        Jet (const TYPE(jets) &, const edm::Handle<vector<osu::Mcparticle> > &);
        Jet (const TYPE(jets) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &);
        ~Jet ();
        const float pfCombinedSecondaryVertexV2BJetTags () const;
        const float pfCombinedInclusiveSecondaryVertexV2BJetTags () const;
        void set_pfCombinedSecondaryVertexV2BJetTags (float value) { pfCombinedSecondaryVertexV2BJetTags_ = value;}
        void set_pfCombinedInclusiveSecondaryVertexV2BJetTags (float value) { pfCombinedInclusiveSecondaryVertexV2BJetTags_ = value;}  
        
      private:
        float pfCombinedSecondaryVertexV2BJetTags_;
        float pfCombinedInclusiveSecondaryVertexV2BJetTags_;
    };
}
#elif DATA_FORMAT == AOD_CUSTOM
namespace osu
{
  class Jet : public TYPE(jets)
    {
      public:
        Jet ();
        Jet (const TYPE(jets) &);
        ~Jet ();
    };
}
#endif

#else

namespace osu
{
  typedef TYPE(jets) Jet;
}

#endif

#endif
