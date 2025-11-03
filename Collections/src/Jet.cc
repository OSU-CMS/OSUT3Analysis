#include "OSUT3Analysis/Collections/interface/Jet.h"

#if IS_VALID(jets)

osu::Jet::Jet ()
{
}

#ifndef STOPPPED_PTLS

osu::Jet::Jet (const TYPE(jets) &jet) :
  GenMatchable (jet),
  matchedToLepton_                               (INVALID_VALUE),
  pfCombinedSecondaryVertexV2BJetTags_           (INVALID_VALUE),
  pfCombinedInclusiveSecondaryVertexV2BJetTags_  (INVALID_VALUE),
  // pileupJetId_                                   (INVALID_VALUE), // Not available in Puppi jets
  smearedPt_                                     (INVALID_VALUE),
  smearedPtJesSystUp_                            (INVALID_VALUE),
  smearedPtJesSystDown_                          (INVALID_VALUE),
  smearedPtJerSystUp_                            (INVALID_VALUE),
  smearedPtJerSystDown_                          (INVALID_VALUE),
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
  // pileupJetId_                                   (INVALID_VALUE), // Not available in Puppi jets
  smearedPt_                                     (INVALID_VALUE),
  smearedPtJesSystUp_                            (INVALID_VALUE),
  smearedPtJesSystDown_                          (INVALID_VALUE),
  smearedPtJerSystUp_                            (INVALID_VALUE),
  smearedPtJerSystDown_                          (INVALID_VALUE),
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
  // pileupJetId_                                   (INVALID_VALUE), // Not available in Puppi jets
  smearedPt_                                     (INVALID_VALUE),
  smearedPtJesSystUp_                            (INVALID_VALUE),
  smearedPtJesSystDown_                          (INVALID_VALUE),
  smearedPtJerSystUp_                            (INVALID_VALUE),
  smearedPtJerSystDown_                          (INVALID_VALUE),
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

// const float  // Not available in Puppi jets
// osu::Jet::pileupJetId () const
// {
//   return pileupJetId_;
// }


const float
osu::Jet::smearedPt () const
{
  return smearedPt_;
}

const float
osu::Jet::smearedPtJesSystUp () const
{
  return smearedPtJesSystUp_;
}

const float
osu::Jet::smearedPtJesSystDown () const
{
  return smearedPtJesSystDown_;
}

const float
osu::Jet::smearedPtJerSystUp () const
{
  return smearedPtJerSystUp_;
}

const float
osu::Jet::smearedPtJerSystDown () const
{
  return smearedPtJerSystDown_;
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

#else // STOPPPED_PTLS
osu::Jet::Jet (const TYPE(jets) &jet) :
  TYPE(jets) (jet)
{
}
#endif // STOPPPED_PTLS

osu::Jet::~Jet ()
{
}

//////////////////////////////
// osu::Bjet
//////////////////////////////

#if IS_VALID(bjets)

osu::Bjet::Bjet() : 
  osu::Jet() {}

#ifndef STOPPPED_PTLS
osu::Bjet::Bjet(const TYPE(jets) &bjet) : 
  osu::Jet(bjet) {}

osu::Bjet::Bjet(const TYPE(jets) &bjet,
                const edm::Handle<vector<osu::Mcparticle> > &particles) :
  osu::Jet(bjet, particles) {}

osu::Bjet::Bjet(const TYPE(jets) &bjet,
                const edm::Handle<vector<osu::Mcparticle> > &particles,
                const edm::ParameterSet &cfg) :
  osu::Jet(bjet, particles, cfg) {}
#else // STOPPPED_PTLS
osu::Bjet::Bjet(const TYPE(jets) &bjet) : 
  osu::Jet(bjet) {}
#endif // STOPPPED_PTLS

osu::Bjet::~Bjet() {}

#endif // IS_VALID(bjets)


//////////////////////////////

#endif // IS_VALID(jets)
