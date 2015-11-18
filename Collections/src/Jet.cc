#include "OSUT3Analysis/Collections/interface/Jet.h"

#if IS_VALID(jets)

osu::Jet::Jet ()
{
}

osu::Jet::Jet (const TYPE(jets) &jet) :
  GenMatchable (jet),
  pfCombinedSecondaryVertexV2BJetTags_           (numeric_limits<float>::min ()),
  pfCombinedInclusiveSecondaryVertexV2BJetTags_  (numeric_limits<float>::min ())
{
}

osu::Jet::Jet (const TYPE(jets) &jet, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (jet, particles),
  pfCombinedSecondaryVertexV2BJetTags_           (numeric_limits<float>::min ()),
  pfCombinedInclusiveSecondaryVertexV2BJetTags_  (numeric_limits<float>::min ())
{
}

osu::Jet::Jet (const TYPE(jets) &jet, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable (jet, particles, cfg),
  pfCombinedSecondaryVertexV2BJetTags_           (numeric_limits<float>::min ()),
  pfCombinedInclusiveSecondaryVertexV2BJetTags_  (numeric_limits<float>::min ())
{
}

const float
osu::Jet::pfCombinedSecondaryVertexV2BJetTags () const
{
  return pfCombinedSecondaryVertexV2BJetTags_;
}

const float
osu::Jet::pfCombinedInclusiveSecondaryVertexV2BJetTags () const
{
  return pfCombinedInclusiveSecondaryVertexV2BJetTags_;
}


osu::Jet::~Jet ()
{
}

#endif
