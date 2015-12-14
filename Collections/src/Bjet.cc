#include "OSUT3Analysis/Collections/interface/Bjet.h"

#if IS_VALID(bjets)

osu::Bjet::Bjet ()
{
}

osu::Bjet::Bjet (const TYPE(bjets) &bjet) :
  GenMatchable (bjet),
  pfCombinedSecondaryVertexV2BJetTags_           (INVALID_VALUE),
  pfCombinedInclusiveSecondaryVertexV2BJetTags_  (INVALID_VALUE)
{
}

osu::Bjet::Bjet (const TYPE(bjets) &bjet, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (bjet, particles),
  pfCombinedSecondaryVertexV2BJetTags_           (INVALID_VALUE),
  pfCombinedInclusiveSecondaryVertexV2BJetTags_  (INVALID_VALUE)
{
}

osu::Bjet::Bjet (const TYPE(bjets) &bjet, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable (bjet, particles, cfg),
  pfCombinedSecondaryVertexV2BJetTags_           (INVALID_VALUE),
  pfCombinedInclusiveSecondaryVertexV2BJetTags_  (INVALID_VALUE)
{
}

const float
osu::Bjet::pfCombinedSecondaryVertexV2BJetTags () const
{
  return pfCombinedSecondaryVertexV2BJetTags_;
}

const float
osu::Bjet::pfCombinedInclusiveSecondaryVertexV2BJetTags () const
{
  return pfCombinedInclusiveSecondaryVertexV2BJetTags_;
}
osu::Bjet::~Bjet ()
{
}

#endif
