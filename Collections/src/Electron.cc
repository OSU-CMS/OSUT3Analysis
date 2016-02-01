#include "OSUT3Analysis/Collections/interface/Electron.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

#if IS_VALID(electrons)

osu::Electron::Electron ()
{
}

#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM  


osu::Electron::Electron (const TYPE(electrons) &electron) :
  GenMatchable               (electron),
  vtxFitConversion_          (false),
  rho_                       (INVALID_VALUE),
  passesTightID_noIsolation_ (false)
{
}

osu::Electron::Electron (const TYPE(electrons) &electron, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable               (electron, particles),
  vtxFitConversion_          (false),
  rho_                       (INVALID_VALUE),
  passesTightID_noIsolation_ (false)
{
}

osu::Electron::Electron (const TYPE(electrons) &electron, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable               (electron, particles, cfg),
  vtxFitConversion_          (false),
  rho_                       (INVALID_VALUE),
  passesTightID_noIsolation_ (false)
{
}

const int
osu::Electron::missingInnerHits () const
{
  return missingInnerHits_;
}

const float
osu::Electron::AEff () const
{
  return AEff_;
}

const float
osu::Electron::rho () const
{
  return this->rho_;
}

const bool
osu::Electron::vtxFitConversion () const
{
  return this->vtxFitConversion_;
}

const bool
osu::Electron::passesTightID_noIsolation () const
{
  return this->passesTightID_noIsolation_;
}

void
osu::Electron::set_passesTightID_noIsolation (const reco::BeamSpot &beamspot, const TYPE(primaryvertexs) &vertex, const edm::Handle<vector<reco::Conversion> > &conversions)
{
  passesTightID_noIsolation_ = false;
  if (fabs (this->superCluster ()->eta ()) <= 1.479)
    {
      passesTightID_noIsolation_ = (this->full5x5_sigmaIetaIeta ()                                                        <   0.0101
                                 && fabs (this->deltaEtaSuperClusterTrackAtVtx ())                                        <   0.00926
                                 && fabs (this->deltaPhiSuperClusterTrackAtVtx ())                                        <   0.0336
                                 && this->hadronicOverEm ()                                                               <   0.0597
                                 && fabs (1.0 / this->ecalEnergy () - this->eSuperClusterOverP () / this->ecalEnergy ())  <   0.012
                                 && fabs (this->gsfTrack ()->dxy (vertex.position ()))                                    <   0.0111
                                 && fabs (this->gsfTrack ()->dz (vertex.position ()))                                     <   0.0466
                                 && this->gsfTrack ()->hitPattern ().numberOfHits (reco::HitPattern::MISSING_INNER_HITS)  <=  2
                                 && !ConversionTools::hasMatchedConversion (*this, conversions, beamspot.position ()));
    }
  else if (fabs (this->superCluster ()->eta ()) < 2.5)
    {
      passesTightID_noIsolation_ = (this->full5x5_sigmaIetaIeta ()                                                        <   0.0279
                                 && fabs (this->deltaEtaSuperClusterTrackAtVtx ())                                        <   0.00724
                                 && fabs (this->deltaPhiSuperClusterTrackAtVtx ())                                        <   0.0918
                                 && this->hadronicOverEm ()                                                               <   0.0615
                                 && fabs (1.0 / this->ecalEnergy () - this->eSuperClusterOverP () / this->ecalEnergy ())  <   0.00999
                                 && fabs (this->gsfTrack ()->dxy (vertex.position ()))                                    <   0.0351
                                 && fabs (this->gsfTrack ()->dz (vertex.position ()))                                     <   0.417
                                 && this->gsfTrack ()->hitPattern ().numberOfHits (reco::HitPattern::MISSING_INNER_HITS)  <=  1
                                 && !ConversionTools::hasMatchedConversion (*this, conversions, beamspot.position ()));
    }
}

#else

osu::Electron::Electron (const TYPE(electrons) &electron) :
  GenMatchable (electron)
{
}

osu::Electron::Electron (const TYPE(electrons) &electron, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (electron, particles)
{
}

osu::Electron::Electron (const TYPE(electrons) &electron, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable (electron, particles, cfg)
{
}

#endif

osu::Electron::~Electron ()
{
}
#endif
