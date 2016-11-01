#include "OSUT3Analysis/Collections/interface/Bjet.h"

#if IS_VALID(bjets)

osu::Bjet::Bjet ()
{
}
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD

osu::Bjet::Bjet (const TYPE(bjets) &bjet) :
  GenMatchable (bjet),
  matchedToLepton_                               (INVALID_VALUE),
  pfCombinedSecondaryVertexV2BJetTags_           (INVALID_VALUE),
  pfCombinedInclusiveSecondaryVertexV2BJetTags_  (INVALID_VALUE),
  pileupJetId_                                   (INVALID_VALUE),
  jecUncertainty_                                (INVALID_VALUE),
  jetPtResolution_                               (INVALID_VALUE),
  jetPtResolutionSF_                             (INVALID_VALUE),
  jetPtResolutionSFUp_                           (INVALID_VALUE),
  jetPtResolutionSFDown_                         (INVALID_VALUE),
  smearedPt_                                     (INVALID_VALUE),
  smearedPtUp_                                   (INVALID_VALUE),
  smearedPtDown_                                 (INVALID_VALUE)
{
}

osu::Bjet::Bjet (const TYPE(bjets) &bjet, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (bjet, particles),
  matchedToLepton_                               (INVALID_VALUE),
  pfCombinedSecondaryVertexV2BJetTags_           (INVALID_VALUE),
  pfCombinedInclusiveSecondaryVertexV2BJetTags_  (INVALID_VALUE),
  pileupJetId_                                   (INVALID_VALUE),
  jecUncertainty_                                (INVALID_VALUE),
  jetPtResolution_                               (INVALID_VALUE),
  jetPtResolutionSF_                             (INVALID_VALUE),
  jetPtResolutionSFUp_                           (INVALID_VALUE),
  jetPtResolutionSFDown_                         (INVALID_VALUE),
  smearedPt_                                     (INVALID_VALUE),
  smearedPtUp_                                   (INVALID_VALUE),
  smearedPtDown_                                 (INVALID_VALUE)
{
}

osu::Bjet::Bjet (const TYPE(bjets) &bjet, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable (bjet, particles, cfg),
  matchedToLepton_                               (INVALID_VALUE),
  pfCombinedSecondaryVertexV2BJetTags_           (INVALID_VALUE),
  pfCombinedInclusiveSecondaryVertexV2BJetTags_  (INVALID_VALUE),
  pileupJetId_                                   (INVALID_VALUE),
  jecUncertainty_                                (INVALID_VALUE),
  jetPtResolution_                               (INVALID_VALUE),
  jetPtResolutionSF_                             (INVALID_VALUE),
  jetPtResolutionSFUp_                           (INVALID_VALUE),
  jetPtResolutionSFDown_                         (INVALID_VALUE),
  smearedPt_                                     (INVALID_VALUE),
  smearedPtUp_                                   (INVALID_VALUE),
  smearedPtDown_                                 (INVALID_VALUE)
{
}

const int
osu::Bjet::matchedToLepton () const
{
  return matchedToLepton_;
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

const float
osu::Bjet::pileupJetId () const
{
  return pileupJetId_;
}

const float
osu::Bjet::jecUncertainty () const
{
  return jecUncertainty_;
}

const float
osu::Bjet::jer () const
{
  return jetPtResolution_;
}

const float
osu::Bjet::jerSF () const
{
  return jetPtResolutionSF_;
}

const float
osu::Bjet::jerSFUp () const
{
  return jetPtResolutionSFUp_;
}

const float
osu::Bjet::jerSFDown () const
{
  return jetPtResolutionSFDown_;
}

const float
osu::Bjet::smearedPt () const
{
  return smearedPt_;
}

const float
osu::Bjet::smearedPtUp () const
{
  return smearedPtUp_;
}

const float
osu::Bjet::smearedPtDown () const
{
  return smearedPtDown_;
}

#elif DATA_FORMAT == AOD_CUSTOM
osu::Bjet::Bjet (const TYPE(bjets) &bjet) :
  TYPE(bjets) (bjet)
{
}
#endif

osu::Bjet::~Bjet ()
{
}

#endif
