#include "OSUT3Analysis/Collections/interface/Photon.h"

#if IS_VALID(photons)

osu::Photon::Photon ()
{
}

osu::Photon::Photon (const TYPE(photons) &photon) :
  GenMatchable (photon),
  rho_ (INVALID_VALUE),
  passesVID_looseID_  (false),
  passesVID_mediumID_ (false),
  passesVID_tightID_  (false)
{
}

osu::Photon::Photon (const TYPE(photons) &photon, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (photon, particles),
  rho_ (INVALID_VALUE),
  passesVID_looseID_  (false),
  passesVID_mediumID_ (false),
  passesVID_tightID_  (false)
{
}

osu::Photon::Photon (const TYPE(photons) &photon, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable (photon, particles, cfg),
  rho_ (INVALID_VALUE),
  passesVID_looseID_  (false),
  passesVID_mediumID_ (false),
  passesVID_tightID_  (false)
{
}

osu::Photon::~Photon ()
{
}

void
osu::Photon::set_passesVID_looseID (const bool flag)
{
  passesVID_looseID_ = flag;
}

const bool
osu::Photon::passesVID_looseID () const
{
  return passesVID_looseID_;
}

void
osu::Photon::set_passesVID_mediumID (const bool flag)
{
  passesVID_mediumID_ = flag;
}

const bool
osu::Photon::passesVID_mediumID () const
{
  return passesVID_mediumID_;
}

void
osu::Photon::set_passesVID_tightID (const bool flag)
{
  passesVID_tightID_ = flag;
}

const bool
osu::Photon::passesVID_tightID () const
{
  return passesVID_tightID_;
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

