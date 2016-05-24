#ifndef OSU_BJET
#define OSU_BJET

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"

#if IS_VALID(bjets)

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
        const float pfCombinedSecondaryVertexV2BJetTags () const;
        const float pfCombinedInclusiveSecondaryVertexV2BJetTags () const;
        void set_pfCombinedSecondaryVertexV2BJetTags (float value) { pfCombinedSecondaryVertexV2BJetTags_ = value;}
        void set_pfCombinedInclusiveSecondaryVertexV2BJetTags (float value) { pfCombinedInclusiveSecondaryVertexV2BJetTags_ = value;}
      private:
        float pfCombinedSecondaryVertexV2BJetTags_;
        float pfCombinedInclusiveSecondaryVertexV2BJetTags_;
    };
}

#else

namespace osu
{
  typedef TYPE(bjets) Bjet;
}

#endif

#endif
