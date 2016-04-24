#include "OSUT3Analysis/Collections/interface/Tau.h"

#if IS_VALID(taus)

osu::Tau::Tau ()
{
}

osu::Tau::Tau (const TYPE(taus) &tau) :
  GenMatchable (tau)
{
}

osu::Tau::Tau (const TYPE(taus) &tau, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (tau, particles)
{
}

osu::Tau::Tau (const TYPE(taus) &tau, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable (tau, particles, cfg)
{
}

osu::Tau::~Tau ()
{
}

const bool
osu::Tau::passesDecayModeReconstruction () const
{
  return (this->tauID ("decayModeFinding") > 0.5);
}

const bool
osu::Tau::passesLightFlavorRejection () const
{
  return (this->tauID ("againstElectronLooseMVA5") > 0.5 && this->tauID ("againstMuonLoose3") > 0.5);
}

#endif
