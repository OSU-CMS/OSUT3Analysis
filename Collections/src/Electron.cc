#include "TVector2.h"

#include "OSUT3Analysis/Collections/interface/Electron.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

#if IS_VALID(electrons)

osu::Electron::Electron ()
{
}

#if DATA_FORMAT_FROM_MINIAOD

osu::Electron::Electron (const TYPE(electrons) &electron) :
  GenMatchable                          (electron),
  rho_                                  (INVALID_VALUE),
  pfdRhoIsoCorr_                        (INVALID_VALUE),
  sumChargedHadronPtCorr_               (INVALID_VALUE),
  electronPVIndex_                      (INVALID_VALUE),
  sumPUPtCorr_                          (INVALID_VALUE),
  genVx_                                (INVALID_VALUE),
  genVy_                                (INVALID_VALUE),
  genPx_                                (INVALID_VALUE),
  genPy_                                (INVALID_VALUE),
  genD0_                                (INVALID_VALUE),
  d0SmearingVal_                        (INVALID_VALUE),
  passesTightID_noIsolation_LegacySpring15_ (false),
  passesVID_vetoID_                     (false),
  passesVID_looseID_                    (false),
  passesVID_mediumID_                   (false),
  passesVID_tightID_                    (false),
  match_HLT_Ele25_eta2p1_WPTight_Gsf_v_ (false),
  match_HLT_Ele22_eta2p1_WPLoose_Gsf_v_ (false),
  match_HLT_Ele32_WPTight_Gsf_v_        (false),
  match_HLT_Ele35_WPTight_Gsf_v_        (false),
  metMinusOnePt_                        (INVALID_VALUE),
  metMinusOnePx_                        (INVALID_VALUE),
  metMinusOnePy_                        (INVALID_VALUE),
  metMinusOnePhi_                       (INVALID_VALUE),
  metNoMuMinusOnePt_                    (INVALID_VALUE),
  metNoMuMinusOnePx_                    (INVALID_VALUE),
  metNoMuMinusOnePy_                    (INVALID_VALUE),
  metNoMuMinusOnePhi_                   (INVALID_VALUE),
  metMinusOneUpPt_                      (INVALID_VALUE),
  metMinusOneUpPx_                      (INVALID_VALUE),
  metMinusOneUpPy_                      (INVALID_VALUE),
  metMinusOneUpPhi_                     (INVALID_VALUE),
  metNoMuMinusOneUpPt_                  (INVALID_VALUE),
  metNoMuMinusOneUpPx_                  (INVALID_VALUE),
  metNoMuMinusOneUpPy_                  (INVALID_VALUE),
  metNoMuMinusOneUpPhi_                 (INVALID_VALUE)
{
}

osu::Electron::Electron (const TYPE(electrons) &electron, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable                          (electron, particles),
  rho_                                  (INVALID_VALUE),
  pfdRhoIsoCorr_                        (INVALID_VALUE),
  sumChargedHadronPtCorr_               (INVALID_VALUE),
  electronPVIndex_                      (INVALID_VALUE),
  sumPUPtCorr_                          (INVALID_VALUE),
  genVx_                                (INVALID_VALUE),
  genVy_                                (INVALID_VALUE),
  genPx_                                (INVALID_VALUE),
  genPy_                                (INVALID_VALUE),
  genD0_                                (INVALID_VALUE),
  d0SmearingVal_                        (INVALID_VALUE),
  passesTightID_noIsolation_LegacySpring15_ (false),
  passesVID_vetoID_                     (false),
  passesVID_looseID_                    (false),
  passesVID_mediumID_                   (false),
  passesVID_tightID_                    (false),
  match_HLT_Ele25_eta2p1_WPTight_Gsf_v_ (false),
  match_HLT_Ele22_eta2p1_WPLoose_Gsf_v_ (false),
  match_HLT_Ele32_WPTight_Gsf_v_        (false),
  match_HLT_Ele35_WPTight_Gsf_v_        (false),
  metMinusOnePt_                        (INVALID_VALUE),
  metMinusOnePx_                        (INVALID_VALUE),
  metMinusOnePy_                        (INVALID_VALUE),
  metMinusOnePhi_                       (INVALID_VALUE),
  metNoMuMinusOnePt_                    (INVALID_VALUE),
  metNoMuMinusOnePx_                    (INVALID_VALUE),
  metNoMuMinusOnePy_                    (INVALID_VALUE),
  metNoMuMinusOnePhi_                   (INVALID_VALUE),
  metMinusOneUpPt_                      (INVALID_VALUE),
  metMinusOneUpPx_                      (INVALID_VALUE),
  metMinusOneUpPy_                      (INVALID_VALUE),
  metMinusOneUpPhi_                     (INVALID_VALUE),
  metNoMuMinusOneUpPt_                  (INVALID_VALUE),
  metNoMuMinusOneUpPx_                  (INVALID_VALUE),
  metNoMuMinusOneUpPy_                  (INVALID_VALUE),
  metNoMuMinusOneUpPhi_                 (INVALID_VALUE)
{
}

osu::Electron::Electron (const TYPE(electrons) &electron, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable                          (electron, particles, cfg),
  rho_                                  (INVALID_VALUE),
  pfdRhoIsoCorr_                        (INVALID_VALUE),
  sumChargedHadronPtCorr_               (INVALID_VALUE),
  electronPVIndex_                      (INVALID_VALUE),
  sumPUPtCorr_                          (INVALID_VALUE),
  genVx_                                (INVALID_VALUE),
  genVy_                                (INVALID_VALUE),
  genPx_                                (INVALID_VALUE),
  genPy_                                (INVALID_VALUE),
  genD0_                                (INVALID_VALUE),
  d0SmearingVal_                        (INVALID_VALUE),
  passesTightID_noIsolation_LegacySpring15_ (false),
  passesVID_vetoID_                     (false),
  passesVID_looseID_                    (false),
  passesVID_mediumID_                   (false),
  passesVID_tightID_                    (false),
  match_HLT_Ele25_eta2p1_WPTight_Gsf_v_ (false),
  match_HLT_Ele22_eta2p1_WPLoose_Gsf_v_ (false),
  match_HLT_Ele32_WPTight_Gsf_v_        (false),
  match_HLT_Ele35_WPTight_Gsf_v_        (false),
  metMinusOnePt_                        (INVALID_VALUE),
  metMinusOnePx_                        (INVALID_VALUE),
  metMinusOnePy_                        (INVALID_VALUE),
  metMinusOnePhi_                       (INVALID_VALUE),
  metNoMuMinusOnePt_                    (INVALID_VALUE),
  metNoMuMinusOnePx_                    (INVALID_VALUE),
  metNoMuMinusOnePy_                    (INVALID_VALUE),
  metNoMuMinusOnePhi_                   (INVALID_VALUE),
  metMinusOneUpPt_                      (INVALID_VALUE),
  metMinusOneUpPx_                      (INVALID_VALUE),
  metMinusOneUpPy_                      (INVALID_VALUE),
  metMinusOneUpPhi_                     (INVALID_VALUE),
  metNoMuMinusOneUpPt_                  (INVALID_VALUE),
  metNoMuMinusOneUpPx_                  (INVALID_VALUE),
  metNoMuMinusOneUpPy_                  (INVALID_VALUE),
  metNoMuMinusOneUpPhi_                 (INVALID_VALUE)
{
}

osu::Electron::Electron (const TYPE(electrons) &electron, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg, const osu::Met &met) :
  GenMatchable                          (electron, particles, cfg),
  rho_                                  (INVALID_VALUE),
  pfdRhoIsoCorr_                        (INVALID_VALUE),
  sumChargedHadronPtCorr_               (INVALID_VALUE),
  electronPVIndex_                      (INVALID_VALUE),
  sumPUPtCorr_                          (INVALID_VALUE),
  genVx_                                (INVALID_VALUE),
  genVy_                                (INVALID_VALUE),
  genPx_                                (INVALID_VALUE),
  genPy_                                (INVALID_VALUE),
  genD0_                                (INVALID_VALUE),
  d0SmearingVal_                        (INVALID_VALUE),
  passesTightID_noIsolation_LegacySpring15_ (false),
  passesVID_vetoID_                     (false),
  passesVID_looseID_                    (false),
  passesVID_mediumID_                   (false),
  passesVID_tightID_                    (false),
  match_HLT_Ele25_eta2p1_WPTight_Gsf_v_ (false),
  match_HLT_Ele22_eta2p1_WPLoose_Gsf_v_ (false),
  match_HLT_Ele32_WPTight_Gsf_v_        (false),
  match_HLT_Ele35_WPTight_Gsf_v_        (false),
  metMinusOnePt_                        (INVALID_VALUE),
  metMinusOnePx_                        (INVALID_VALUE),
  metMinusOnePy_                        (INVALID_VALUE),
  metMinusOnePhi_                       (INVALID_VALUE),
  metNoMuMinusOnePt_                    (INVALID_VALUE),
  metNoMuMinusOnePx_                    (INVALID_VALUE),
  metNoMuMinusOnePy_                    (INVALID_VALUE),
  metNoMuMinusOnePhi_                   (INVALID_VALUE),
  metMinusOneUpPt_                      (INVALID_VALUE),
  metMinusOneUpPx_                      (INVALID_VALUE),
  metMinusOneUpPy_                      (INVALID_VALUE),
  metMinusOneUpPhi_                     (INVALID_VALUE),
  metNoMuMinusOneUpPt_                  (INVALID_VALUE),
  metNoMuMinusOneUpPx_                  (INVALID_VALUE),
  metNoMuMinusOneUpPy_                  (INVALID_VALUE),
  metNoMuMinusOneUpPhi_                 (INVALID_VALUE)
{
  TVector2 p (met.px () + this->px (), met.py () + this->py ()),
           pNoMu (met.noMuPx () + this->px (), met.noMuPy () + this->py ()),
           p10;

  p10.SetMagPhi (10.0, this->phi ());

  metMinusOnePt_ = p.Mod ();
  metMinusOnePx_ = p.Px ();
  metMinusOnePy_ = p.Py ();
  metMinusOnePhi_ = p.Phi ();

  metNoMuMinusOnePt_ = pNoMu.Mod ();
  metNoMuMinusOnePx_ = pNoMu.Px ();
  metNoMuMinusOnePy_ = pNoMu.Py ();
  metNoMuMinusOnePhi_ = pNoMu.Phi ();

  metMinusOneUpPt_ = (p - p10).Mod ();
  metMinusOneUpPx_ = (p - p10).Px ();
  metMinusOneUpPy_ = (p - p10).Py ();
  metMinusOneUpPhi_ = (p - p10).Phi ();

  metNoMuMinusOneUpPt_ = (pNoMu - p10).Mod ();
  metNoMuMinusOneUpPx_ = (pNoMu - p10).Px ();
  metNoMuMinusOneUpPy_ = (pNoMu - p10).Py ();
  metNoMuMinusOneUpPhi_ = (pNoMu - p10).Phi ();
}

const int
osu::Electron::missingInnerHitsFromAllHits () const
{
#if CMSSW_VERSION_CODE >= CMSSW_VERSION(9,4,0)
  return (this->gsfTrack ()->hitPattern ().numberOfAllHits (reco::HitPattern::MISSING_INNER_HITS));
#else
  return (this->gsfTrack ()->hitPattern ().numberOfHits (reco::HitPattern::MISSING_INNER_HITS));
#endif
}

const int
osu::Electron::missingInnerHitsFromLostHits () const
{
  return (this->gsfTrack ()->hitPattern ().numberOfLostHits (reco::HitPattern::MISSING_INNER_HITS));
}

const int
osu::Electron::missingInnerHitsFromTrackerLayersWithoutMeasurements () const
{
  return (this->gsfTrack ()->hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::MISSING_INNER_HITS));
}

const int
osu::Electron::missingMiddleHitsFromTrackerLayersWithoutMeasurements () const
{
  return (this->gsfTrack ()->hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::TRACK_HITS));
}

const int
osu::Electron::missingOuterHitsFromTrackerLayersWithoutMeasurements () const
{
  return (this->gsfTrack ()->hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::MISSING_OUTER_HITS));
}

const bool
osu::Electron::hasValidHitInPixelLayer (const uint16_t layer) const
{

  const reco::HitPattern &p = this->gsfTrack()->hitPattern();

  // Loop over TRACK_HITS
#if CMSSW_VERSION_CODE >= CMSSW_VERSION(9,4,0)
  for (int i = 0; i < p.numberOfAllHits(reco::HitPattern::TRACK_HITS); i++) {
#else
  for (int i = 0; i < p.numberOfHits(reco::HitPattern::TRACK_HITS); i++) {
#endif
    uint16_t hit = p.getHitPattern(reco::HitPattern::TRACK_HITS, i);
    if(reco::HitPattern::getLayer(hit) == layer &&
       reco::HitPattern::validHitFilter(hit)) {
      return true;
    }
  }

  return false;
}

const int
osu::Electron::layerOfFirstValidPixelHit () const
{
  if(hasValidHitInPixelLayer(1)) return 1;
  else if(hasValidHitInPixelLayer(2)) return 2;
  else if(hasValidHitInPixelLayer(3)) return 3;
  else if(hasValidHitInPixelLayer(4)) return 4;
  else return 0;
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

const float
osu::Electron::dEtaInSeed () const
{
  return this->superCluster().isNonnull() && this->superCluster()->seed().isNonnull() ?
    this->deltaEtaSuperClusterTrackAtVtx() - this->superCluster()->eta() + this->superCluster()->seed()->eta() : INVALID_VALUE;
}

const bool
osu::Electron::pass_GsfEleHadronicOverEMEnergyScaledCut (const float c0, const float cE, const float cR) const
{
  if(!this->superCluster().isNonnull()) return INVALID_VALUE;

  const float energy = this->superCluster()->energy();

  return (this->hadronicOverEm() < c0 + cE/energy + cR*rho_/energy);
}

const double
osu::Electron::sumPUPtCorr () const
{
  return sumPUPtCorr_;
}

const double
osu::Electron::genVx () const
{
  return genVx_;
}

const double
osu::Electron::genVy () const
{
  return genVy_;
}

const double
osu::Electron::genPx () const
{
  return genPx_;
}

const double
osu::Electron::genPy () const
{
  return genPy_;
}

const double
osu::Electron::genD0 () const
{
  return genD0_;
}

const double
osu::Electron::d0SmearingVal () const
{
  return d0SmearingVal_;
}

const bool
osu::Electron::passesTightID_noIsolation_LegacySpring15 () const
{
  return passesTightID_noIsolation_LegacySpring15_;
}

void
osu::Electron::set_passesTightID_noIsolation_LegacySpring15 (const reco::BeamSpot &beamspot, const TYPE(primaryvertexs) &vertex, const edm::Handle<vector<reco::Conversion> > &conversions)
{
  passesTightID_noIsolation_LegacySpring15_ = false;

#if CMSSW_VERSION_CODE < CMSSW_VERSION(9,0,0)
  // not 94X -- actually corresponds to https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2Archive#Spring15_selection_25ns
  if (fabs (this->superCluster ()->eta ()) <= 1.479) {
      passesTightID_noIsolation_LegacySpring15_ = (this->full5x5_sigmaIetaIeta ()                                                        <   0.0101  &&
                                                   fabs (this->deltaEtaSuperClusterTrackAtVtx ())                                        <   0.00926 &&
                                                   fabs (this->deltaPhiSuperClusterTrackAtVtx ())                                        <   0.0336  &&
                                                   this->hadronicOverEm ()                                                               <   0.0597  &&
                                                   fabs (1.0 / this->ecalEnergy () - this->eSuperClusterOverP () / this->ecalEnergy ())  <   0.012   &&
                                                   fabs (this->gsfTrack ()->dxy (vertex.position ()))                                    <   0.0111  &&
                                                   fabs (this->gsfTrack ()->dz (vertex.position ()))                                     <   0.0466  &&
                                                   this->gsfTrack ()->hitPattern ().numberOfHits (reco::HitPattern::MISSING_INNER_HITS)  <=  2       &&
                                                   !ConversionTools::hasMatchedConversion (*this, conversions, beamspot.position ()));
  }
  else if (fabs (this->superCluster ()->eta ()) < 2.5) {
      passesTightID_noIsolation_LegacySpring15_ = (this->full5x5_sigmaIetaIeta ()                                                        <   0.0279  &&
                                                   fabs (this->deltaEtaSuperClusterTrackAtVtx ())                                        <   0.00724 &&
                                                   fabs (this->deltaPhiSuperClusterTrackAtVtx ())                                        <   0.0918  &&
                                                   this->hadronicOverEm ()                                                               <   0.0615  &&
                                                   fabs (1.0 / this->ecalEnergy () - this->eSuperClusterOverP () / this->ecalEnergy ())  <   0.00999 &&
                                                   fabs (this->gsfTrack ()->dxy (vertex.position ()))                                    <   0.0351  &&
                                                   fabs (this->gsfTrack ()->dz (vertex.position ()))                                     <   0.417   &&
                                                   this->gsfTrack ()->hitPattern ().numberOfHits (reco::HitPattern::MISSING_INNER_HITS)  <=  1       &&
                                                   !ConversionTools::hasMatchedConversion (*this, conversions, beamspot.position ()));
  }
#endif

}

void
osu::Electron::set_passesVID_vetoID (const bool flag)
{
  passesVID_vetoID_ = flag;
}

const bool
osu::Electron::passesVID_vetoID () const
{
  return passesVID_vetoID_;
}

void
osu::Electron::set_passesVID_looseID (const bool flag)
{
  passesVID_looseID_ = flag;
}

const bool
osu::Electron::passesVID_looseID () const
{
  return passesVID_looseID_;
}

void
osu::Electron::set_passesVID_mediumID (const bool flag)
{
  passesVID_mediumID_ = flag;
}

const bool
osu::Electron::passesVID_mediumID () const
{
  return passesVID_mediumID_;
}

void
osu::Electron::set_passesVID_tightID (const bool flag)
{
  passesVID_tightID_ = flag;
}

const bool
osu::Electron::passesVID_tightID () const
{
  return passesVID_tightID_;
}

const bool
osu::Electron::match_HLT_Ele25_eta2p1_WPTight_Gsf_v () const
{
  return match_HLT_Ele25_eta2p1_WPTight_Gsf_v_;
}

void
osu::Electron::set_match_HLT_Ele25_eta2p1_WPTight_Gsf_v (const bool flag)
{
  match_HLT_Ele25_eta2p1_WPTight_Gsf_v_ = flag;
}

const bool
osu::Electron::match_HLT_Ele22_eta2p1_WPLoose_Gsf_v () const
{
  return match_HLT_Ele22_eta2p1_WPLoose_Gsf_v_;
}

void
osu::Electron::set_match_HLT_Ele22_eta2p1_WPLoose_Gsf_v (const bool flag)
{
  match_HLT_Ele22_eta2p1_WPLoose_Gsf_v_ = flag;
}


const bool
osu::Electron::match_HLT_Ele32_WPTight_Gsf_v () const
{
  return match_HLT_Ele32_WPTight_Gsf_v_;
}

void
osu::Electron::set_match_HLT_Ele32_WPTight_Gsf_v (const bool flag)
{
  match_HLT_Ele32_WPTight_Gsf_v_ = flag;
}

const bool
osu::Electron::match_HLT_Ele35_WPTight_Gsf_v () const
{
  return match_HLT_Ele35_WPTight_Gsf_v_;
}

void
osu::Electron::set_match_HLT_Ele35_WPTight_Gsf_v (const bool flag)
{
  match_HLT_Ele35_WPTight_Gsf_v_ = flag;
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

const double
osu::Electron::metMinusOneUpPt () const
{
  return metMinusOneUpPt_;
}

const double
osu::Electron::metMinusOneUpPx () const
{
  return metMinusOneUpPx_;
}

const double
osu::Electron::metMinusOneUpPy () const
{
  return metMinusOneUpPy_;
}

const double
osu::Electron::metMinusOneUpPhi () const
{
  return metMinusOneUpPhi_;
}

const double
osu::Electron::metNoMuMinusOneUpPt () const
{
  return metNoMuMinusOneUpPt_;
}

const double
osu::Electron::metNoMuMinusOneUpPx () const
{
  return metNoMuMinusOneUpPx_;
}

const double
osu::Electron::metNoMuMinusOneUpPy () const
{
  return metNoMuMinusOneUpPy_;
}

const double
osu::Electron::metNoMuMinusOneUpPhi () const
{
  return metNoMuMinusOneUpPhi_;
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
