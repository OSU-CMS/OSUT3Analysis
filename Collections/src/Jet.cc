#include "OSUT3Analysis/Collections/interface/Jet.h"

#if IS_VALID(jets)

osu::Jet::Jet ()
{
}
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_2017 || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD

osu::Jet::Jet (const TYPE(jets) &jet) :
  GenMatchable (jet),
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
  smearedPtDown_                                 (INVALID_VALUE),
  alphamax_                                      (INVALID_VALUE),
  ipsig_                                         (INVALID_VALUE),
  log10ipsig_                                    (INVALID_VALUE),
  medianlog10ipsig_                              (INVALID_VALUE)
{
}

osu::Jet::Jet (const TYPE(jets) &jet, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (jet, particles),
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
  smearedPtDown_                                 (INVALID_VALUE),
  alphamax_                                      (INVALID_VALUE),
  ipsig_                                         (INVALID_VALUE),
  log10ipsig_                                    (INVALID_VALUE),
  medianlog10ipsig_                              (INVALID_VALUE)
{
}

osu::Jet::Jet (const TYPE(jets) &jet, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable (jet, particles, cfg),
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
  smearedPtDown_                                 (INVALID_VALUE),
  alphamax_                                      (INVALID_VALUE),
  ipsig_                                         (INVALID_VALUE),
  log10ipsig_                                    (INVALID_VALUE),
  medianlog10ipsig_                              (INVALID_VALUE)
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

const float
osu::Jet::jecUncertainty () const
{
  return jecUncertainty_;
}

const float
osu::Jet::jer () const
{
  return jetPtResolution_;
}

const float
osu::Jet::jerSF () const
{
  return jetPtResolutionSF_;
}

const float
osu::Jet::jerSFUp () const
{
  return jetPtResolutionSFUp_;
}

const float
osu::Jet::jerSFDown () const
{
  return jetPtResolutionSFDown_;
}

const float
osu::Jet::smearedPt () const
{
  return smearedPt_;
}

const float
osu::Jet::smearedPtUp () const
{
  return smearedPtUp_;
}

const float
osu::Jet::smearedPtDown () const
{
  return smearedPtDown_;
}

const float
osu::Jet::alphamax () const
{
  return alphamax_;
}

const float 
osu::Jet::ipsig () const
{
  return ipsig_;
}

const float
osu::Jet::log10ipsig () const
{
  return log10ipsig_;
}

const float
osu::Jet::medianlog10ipsig () const
{
  return medianlog10ipsig_;
}

#elif DATA_FORMAT == AOD_CUSTOM
osu::Jet::Jet (const TYPE(jets) &jet) :
  TYPE(jets) (jet)
{
}
#endif // DATA_FORMAT

osu::Jet::~Jet ()
{
}

//////////////////////////////
// osu::Bjet
//////////////////////////////

#if IS_VALID(bjets)

osu::Bjet::Bjet() : 
  osu::Jet() {}

#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_2017 || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD
osu::Bjet::Bjet(const TYPE(jets) &bjet) : 
  osu::Jet(bjet) {}

osu::Bjet::Bjet(const TYPE(jets) &bjet,
                const edm::Handle<vector<osu::Mcparticle> > &particles) :
  osu::Jet(bjet, particles) {}

osu::Bjet::Bjet(const TYPE(jets) &bjet,
                const edm::Handle<vector<osu::Mcparticle> > &particles,
                const edm::ParameterSet &cfg) :
  osu::Jet(bjet, particles, cfg) {}
#elif DATA_FORMAT == AOD_CUSTOM
osu::Bjet::Bjet(const TYPE(jets) &bjet) : 
  osu::Jet(bjet) {}
#endif // DATA_FORMAT

osu::Bjet::~Bjet() {}

#endif // IS_VALID(bjets)

//////////////////////////////
// osu::Basicjet
//////////////////////////////

#if IS_VALID(basicjets)

osu::Basicjet::Basicjet() : 
  osu::Jet() {}

#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_2017 || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD
osu::Basicjet::Basicjet(const TYPE(jets) &basicjet) : 
  osu::Jet(basicjet) {}

osu::Basicjet::Basicjet(const TYPE(jets) &basicjet,
                        const edm::Handle<vector<osu::Mcparticle> > &particles) :
  osu::Jet(basicjet, particles) {}

osu::Basicjet::Basicjet(const TYPE(jets) &basicjet,
                        const edm::Handle<vector<osu::Mcparticle> > &particles,
                        const edm::ParameterSet &cfg) :
  osu::Jet(basicjet, particles, cfg) {}
#elif DATA_FORMAT == AOD_CUSTOM
osu::Basicjet::Basicjet(const TYPE(jets) &basicjet) : 
  osu::Jet(basicjet) {}
#endif // DATA_FORMAT

osu::Basicjet::~Basicjet() {}

#endif // IS_VALID(basicjets)

//////////////////////////////

#endif // IS_VALID(jets)
