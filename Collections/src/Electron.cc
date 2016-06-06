#include "TVector2.h"

#include "OSUT3Analysis/Collections/interface/Electron.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

#if IS_VALID(electrons)

osu::Electron::Electron ()
{
}

#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM


osu::Electron::Electron (const TYPE(electrons) &electron) :
  GenMatchable                (electron),
  rho_                        (INVALID_VALUE),
  pfdRhoIsoCorr_              (INVALID_VALUE),
  sumChargedHadronPtCorr_     (INVALID_VALUE),
  electronPVIndex_            (INVALID_VALUE),
  sumPUPtCorr_                (INVALID_VALUE),
  passesTightID_noIsolation_  (false),
  metMinusOnePt_              (INVALID_VALUE),
  metMinusOnePx_              (INVALID_VALUE),
  metMinusOnePy_              (INVALID_VALUE),
  metMinusOnePhi_             (INVALID_VALUE),
  metNoMuMinusOnePt_          (INVALID_VALUE),
  metNoMuMinusOnePx_          (INVALID_VALUE),
  metNoMuMinusOnePy_          (INVALID_VALUE),
  metNoMuMinusOnePhi_         (INVALID_VALUE)
{
}

osu::Electron::Electron (const TYPE(electrons) &electron, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable                (electron,        particles),
  rho_                        (INVALID_VALUE),
  pfdRhoIsoCorr_              (INVALID_VALUE),
  sumChargedHadronPtCorr_     (INVALID_VALUE),
  electronPVIndex_            (INVALID_VALUE),
  sumPUPtCorr_                (INVALID_VALUE),
  passesTightID_noIsolation_  (false),
  metMinusOnePt_              (INVALID_VALUE),
  metMinusOnePx_              (INVALID_VALUE),
  metMinusOnePy_              (INVALID_VALUE),
  metMinusOnePhi_             (INVALID_VALUE),
  metNoMuMinusOnePt_          (INVALID_VALUE),
  metNoMuMinusOnePx_          (INVALID_VALUE),
  metNoMuMinusOnePy_          (INVALID_VALUE),
  metNoMuMinusOnePhi_         (INVALID_VALUE)
{
}

osu::Electron::Electron (const TYPE(electrons) &electron, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable                (electron,        particles,  cfg),
  rho_                        (INVALID_VALUE),
  pfdRhoIsoCorr_              (INVALID_VALUE),
  sumChargedHadronPtCorr_     (INVALID_VALUE),
  electronPVIndex_            (INVALID_VALUE),
  sumPUPtCorr_                (INVALID_VALUE),
  passesTightID_noIsolation_  (false),
  metMinusOnePt_              (INVALID_VALUE),
  metMinusOnePx_              (INVALID_VALUE),
  metMinusOnePy_              (INVALID_VALUE),
  metMinusOnePhi_             (INVALID_VALUE),
  metNoMuMinusOnePt_          (INVALID_VALUE),
  metNoMuMinusOnePx_          (INVALID_VALUE),
  metNoMuMinusOnePy_          (INVALID_VALUE),
  metNoMuMinusOnePhi_         (INVALID_VALUE)
{
}

osu::Electron::Electron (const TYPE(electrons) &electron, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg, const osu::Met &met) :
  GenMatchable                (electron,        particles,  cfg),
  rho_                        (INVALID_VALUE),
  pfdRhoIsoCorr_              (INVALID_VALUE),
  sumChargedHadronPtCorr_     (INVALID_VALUE),
  electronPVIndex_            (INVALID_VALUE),
  sumPUPtCorr_                (INVALID_VALUE),
  passesTightID_noIsolation_  (false),
  metMinusOnePt_              (INVALID_VALUE),
  metMinusOnePx_              (INVALID_VALUE),
  metMinusOnePy_              (INVALID_VALUE),
  metMinusOnePhi_             (INVALID_VALUE),
  metNoMuMinusOnePt_          (INVALID_VALUE),
  metNoMuMinusOnePx_          (INVALID_VALUE),
  metNoMuMinusOnePy_          (INVALID_VALUE),
  metNoMuMinusOnePhi_         (INVALID_VALUE)
{
  TVector2 p (met.px () + this->px (), met.py () + this->py ()),
           pNoMu (met.noMuPx () + this->px (), met.noMuPy () + this->py ());

  metMinusOnePt_ = p.Mod ();
  metMinusOnePx_ = p.Px ();
  metMinusOnePy_ = p.Py ();
  metMinusOnePhi_ = p.Phi ();

  metNoMuMinusOnePt_ = pNoMu.Mod ();
  metNoMuMinusOnePx_ = pNoMu.Px ();
  metNoMuMinusOnePy_ = pNoMu.Py ();
  metNoMuMinusOnePhi_ = pNoMu.Phi ();
}

const int
osu::Electron::missingInnerHits () const
{
  return (this->gsfTrack ()->hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::MISSING_INNER_HITS));
}

const int
osu::Electron::missingMiddleHits () const
{
  return (this->gsfTrack ()->hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::TRACK_HITS));
}

const int
osu::Electron::missingOuterHits () const
{
  return (this->gsfTrack ()->hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::MISSING_OUTER_HITS));
}

const float
osu::Electron::AEff () const
{
  return AEff_;
}

const double
osu::Electron::pfdRhoIsoCorr () const
{
  return pfdRhoIsoCorr_;
}

const float
osu::Electron::rho () const
{
  return this->rho_;
}

const double
osu::Electron::sumChargedHadronPtCorr () const
{
  return sumChargedHadronPtCorr_;
}
const int
osu::Electron::electronPVIndex () const
{
  return electronPVIndex_;
}

const double
osu::Electron::sumPUPtCorr () const
{
  return sumPUPtCorr_;
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

const double
osu::Electron::metMinusOnePt () const
{
  return metMinusOnePt_;
}

const double
osu::Electron::metMinusOnePx () const
{
  return metMinusOnePx_;
}

const double
osu::Electron::metMinusOnePy () const
{
  return metMinusOnePy_;
}

const double
osu::Electron::metMinusOnePhi () const
{
  return metMinusOnePhi_;
}

const double
osu::Electron::metNoMuMinusOnePt () const
{
  return metNoMuMinusOnePt_;
}

const double
osu::Electron::metNoMuMinusOnePx () const
{
  return metNoMuMinusOnePx_;
}

const double
osu::Electron::metNoMuMinusOnePy () const
{
  return metNoMuMinusOnePy_;
}

const double
osu::Electron::metNoMuMinusOnePhi () const
{
  return metNoMuMinusOnePhi_;
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
