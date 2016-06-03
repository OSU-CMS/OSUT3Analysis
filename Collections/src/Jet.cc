#include "OSUT3Analysis/Collections/interface/Jet.h"

#if IS_VALID(jets)

osu::Jet::Jet ()
{
}
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD

osu::Jet::Jet (const TYPE(jets) &jet) :
  GenMatchable (jet),
  matchedToLepton_                               (INVALID_VALUE),
  pfCombinedSecondaryVertexV2BJetTags_           (INVALID_VALUE),
  pfCombinedInclusiveSecondaryVertexV2BJetTags_  (INVALID_VALUE),
  pileupJetId_                                   (INVALID_VALUE)
{
}

osu::Jet::Jet (const TYPE(jets) &jet, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (jet, particles),
  matchedToLepton_                               (INVALID_VALUE),
  pfCombinedSecondaryVertexV2BJetTags_           (INVALID_VALUE),
  pfCombinedInclusiveSecondaryVertexV2BJetTags_  (INVALID_VALUE),
  pileupJetId_                                   (INVALID_VALUE)
{
}

osu::Jet::Jet (const TYPE(jets) &jet, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable (jet, particles, cfg),
  matchedToLepton_                               (INVALID_VALUE),
  pfCombinedSecondaryVertexV2BJetTags_           (INVALID_VALUE),
  pfCombinedInclusiveSecondaryVertexV2BJetTags_  (INVALID_VALUE),
  pileupJetId_                                   (INVALID_VALUE)
{
}

const int
osu::Jet::matchedToLepton () const
{
  return matchedToLepton_;
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

const float
osu::Jet::pileupJetId () const
{
  return pileupJetId_;
}

#elif DATA_FORMAT == AOD_CUSTOM
osu::Jet::Jet (const TYPE(jets) &jet) :
  TYPE(jets) (jet)
{
}
#endif

osu::Jet::~Jet ()
{
}

#endif
