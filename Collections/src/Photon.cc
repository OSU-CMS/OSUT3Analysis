#include "OSUT3Analysis/Collections/interface/Photon.h"

#if IS_VALID(photons)

osu::Photon::Photon ()
{
}

osu::Photon::Photon (const TYPE(photons) &photon) :
  GenMatchable (photon),
  rho_ (INVALID_VALUE)
{
}

osu::Photon::Photon (const TYPE(photons) &photon, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (photon, particles),
  rho_ (INVALID_VALUE)
{
}

osu::Photon::Photon (const TYPE(photons) &photon, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable (photon, particles, cfg),
  rho_ (INVALID_VALUE)
{
}

osu::Photon::~Photon ()
{
}

const float osu::Photon::Aeff_neutralHadron () const {
  return AEff_neutralHadron_;
}

const float osu::Photon::Aeff_chargedHadron () const {
  return Aeff_chargedHadron_;
}

const float osu::Photon::Aeff_photon () const {
  return Aeff_photon_;
}

const float osu::Photon::rho () const {
  return this->rho_;
}

#endif

