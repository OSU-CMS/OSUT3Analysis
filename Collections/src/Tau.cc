#include "OSUT3Analysis/Collections/interface/Tau.h"

#if IS_VALID(taus)

osu::Tau::Tau ()
{
}

osu::Tau::Tau (const TYPE(taus) &tau) :
  GenMatchable         (tau),
  metMinusOnePt_       (INVALID_VALUE),
  metMinusOnePx_       (INVALID_VALUE),
  metMinusOnePy_       (INVALID_VALUE),
  metMinusOnePhi_      (INVALID_VALUE),
  metNoMuMinusOnePt_   (INVALID_VALUE),
  metNoMuMinusOnePx_   (INVALID_VALUE),
  metNoMuMinusOnePy_   (INVALID_VALUE),
  metNoMuMinusOnePhi_  (INVALID_VALUE),
  metMinusOneUpPt_              (INVALID_VALUE),
  metMinusOneUpPx_              (INVALID_VALUE),
  metMinusOneUpPy_              (INVALID_VALUE),
  metMinusOneUpPhi_             (INVALID_VALUE),
  metNoMuMinusOneUpPt_          (INVALID_VALUE),
  metNoMuMinusOneUpPx_          (INVALID_VALUE),
  metNoMuMinusOneUpPy_          (INVALID_VALUE),
  metNoMuMinusOneUpPhi_         (INVALID_VALUE)
{
}

osu::Tau::Tau (const TYPE(taus) &tau, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable         (tau,             particles),
  metMinusOnePt_       (INVALID_VALUE),
  metMinusOnePx_       (INVALID_VALUE),
  metMinusOnePy_       (INVALID_VALUE),
  metMinusOnePhi_      (INVALID_VALUE),
  metNoMuMinusOnePt_   (INVALID_VALUE),
  metNoMuMinusOnePx_   (INVALID_VALUE),
  metNoMuMinusOnePy_   (INVALID_VALUE),
  metNoMuMinusOnePhi_  (INVALID_VALUE),
  metMinusOneUpPt_              (INVALID_VALUE),
  metMinusOneUpPx_              (INVALID_VALUE),
  metMinusOneUpPy_              (INVALID_VALUE),
  metMinusOneUpPhi_             (INVALID_VALUE),
  metNoMuMinusOneUpPt_          (INVALID_VALUE),
  metNoMuMinusOneUpPx_          (INVALID_VALUE),
  metNoMuMinusOneUpPy_          (INVALID_VALUE),
  metNoMuMinusOneUpPhi_         (INVALID_VALUE)
{
}

osu::Tau::Tau (const TYPE(taus) &tau, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable         (tau,             particles,  cfg),
  metMinusOnePt_       (INVALID_VALUE),
  metMinusOnePx_       (INVALID_VALUE),
  metMinusOnePy_       (INVALID_VALUE),
  metMinusOnePhi_      (INVALID_VALUE),
  metNoMuMinusOnePt_   (INVALID_VALUE),
  metNoMuMinusOnePx_   (INVALID_VALUE),
  metNoMuMinusOnePy_   (INVALID_VALUE),
  metNoMuMinusOnePhi_  (INVALID_VALUE),
  metMinusOneUpPt_              (INVALID_VALUE),
  metMinusOneUpPx_              (INVALID_VALUE),
  metMinusOneUpPy_              (INVALID_VALUE),
  metMinusOneUpPhi_             (INVALID_VALUE),
  metNoMuMinusOneUpPt_          (INVALID_VALUE),
  metNoMuMinusOneUpPx_          (INVALID_VALUE),
  metNoMuMinusOneUpPy_          (INVALID_VALUE),
  metNoMuMinusOneUpPhi_         (INVALID_VALUE)
{
}

osu::Tau::Tau (const TYPE(taus) &tau, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg, const osu::Met &met) :
  GenMatchable         (tau,             particles,  cfg),
  metMinusOnePt_       (INVALID_VALUE),
  metMinusOnePx_       (INVALID_VALUE),
  metMinusOnePy_       (INVALID_VALUE),
  metMinusOnePhi_      (INVALID_VALUE),
  metNoMuMinusOnePt_   (INVALID_VALUE),
  metNoMuMinusOnePx_   (INVALID_VALUE),
  metNoMuMinusOnePy_   (INVALID_VALUE),
  metNoMuMinusOnePhi_  (INVALID_VALUE),
  metMinusOneUpPt_              (INVALID_VALUE),
  metMinusOneUpPx_              (INVALID_VALUE),
  metMinusOneUpPy_              (INVALID_VALUE),
  metMinusOneUpPhi_             (INVALID_VALUE),
  metNoMuMinusOneUpPt_          (INVALID_VALUE),
  metNoMuMinusOneUpPx_          (INVALID_VALUE),
  metNoMuMinusOneUpPy_          (INVALID_VALUE),
  metNoMuMinusOneUpPhi_         (INVALID_VALUE)
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

osu::Tau::~Tau ()
{
}

// Tau ID and isolation discriminators are documented here:
// https://twiki.cern.ch/twiki/bin/view/CMS/TauIDRecommendation13TeV
// The names of the discriminators are out-of-date though, with the current
// names for 76X samples found here:
// https://github.com/cms-sw/cmssw/blob/CMSSW_7_6_X/PhysicsTools/PatAlgos/python/producersLayer1/tauProducer_cfi.py#L62-L106

#if DATA_FORMAT_FROM_MINIAOD
const bool
osu::Tau::passesDecayModeReconstruction () const
{
  // This were the selections used for Run 2; they were changed in Run 3 to use the DeepTau selections and a
  // distinct decay mode ID https://twiki.cern.ch/twiki/bin/view/CMS/TauIDRecommendationForRun3
  // The new IDs can be found in https://github.com/cms-sw/cmssw/blob/388ec50d980a31e7edb67eba169d45743d6623f5/Validation/RecoTau/python/RecoTauValidationMiniAOD_cfi.py (not sure if it is updated, though)

  // if (this->isTauIDAvailable ("decayModeFinding"))
  //   return (this->tauID ("decayModeFinding") > 0.5);
  // else
  //   {
  //     edm::LogWarning ("osu_Tau") << "Tau ID \"decayModeFinding\" unavailable.";
  //     return false;
  //   }

  if (this->isTauIDAvailable ("decayModeFindingNewDMs"))
    return (this->tauID ("decayModeFindingNewDMs") > 0.5);
  else
    {
      edm::LogWarning ("osu_Tau") << "Tau ID \"decayModeFindingNewDMs\" unavailable.";
      return false;
    }
}

const bool
osu::Tau::passesLightFlavorRejection () const
{
  bool flag = true;

  // This were the selections used for Run 2; they were changed in Run 3 to use the DeepTau selections and a
  // distinct decay mode ID https://twiki.cern.ch/twiki/bin/view/CMS/TauIDRecommendationForRun3
  // The new IDs can be found in https://github.com/cms-sw/cmssw/blob/388ec50d980a31e7edb67eba169d45743d6623f5/Validation/RecoTau/python/RecoTauValidationMiniAOD_cfi.py (not sure if it is updated, though)

  // if (this->isTauIDAvailable ("againstElectronLooseMVA5"))
  //   flag = flag && (this->tauID ("againstElectronLooseMVA5") > 0.5);
  // else if (this->isTauIDAvailable ("againstElectronLooseMVA6"))
  //   flag = flag && (this->tauID ("againstElectronLooseMVA6") > 0.5);
  // else
  //   {
  //     edm::LogWarning ("osu_Tau") << "Tau IDs \"againstElectronLooseMVA5\" and \"againstElectronLooseMVA6\" unavailable.";
  //     flag = flag && false;
  //   }

  // if (this->isTauIDAvailable ("againstMuonLoose3"))
  //   flag = flag && (this->tauID ("againstMuonLoose3") > 0.5);
  // else
  //   {
  //     edm::LogWarning ("osu_Tau") << "Tau ID \"againstMuonLoose3\" unavailable.";
  //     flag = flag && false;
  //   }

  if (this->isTauIDAvailable ("byVVVLooseDeepTau2017v2p1VSe") && this->isTauIDAvailable ("byVVVLooseDeepTau2018v2p5VSe"))
    flag = flag && ((this->tauID ("byVVVLooseDeepTau2017v2p1VSe") > 0.5) || (this->tauID ("byVVVLooseDeepTau2018v2p5VSe") > 0.5));
  else
    {
      edm::LogWarning ("osu_Tau") << "Tau IDs \"byVVVLooseDeepTau2017v2p1VSe\" and \"byVVVLooseDeepTau2018v2p5VSe\" unavailable.";
      flag = flag && false;
    }

  // againstMuonLoose3 still exists as a tau ID, but will follow the recommentdation to use DeepTau
  // This should probably be checked with Tau POG experts
  if (this->isTauIDAvailable ("byVLooseDeepTau2017v2p1VSmu") && this->isTauIDAvailable ("byVLooseDeepTau2018v2p5VSmu"))
    flag = flag && ((this->tauID ("byVLooseDeepTau2017v2p1VSmu") > 0.5) || (this->tauID ("byVLooseDeepTau2018v2p5VSmu") > 0.5));
  else
    {
      edm::LogWarning ("osu_Tau") << "Tau ID \"byVLooseDeepTau2017v2p1VSmu\" and \"byVLooseDeepTau2018v2p5VSmu\" unavailable.";
      flag = flag && false;
    }

  return flag;
}

const bool
osu::Tau::passesLooseCombinedIsolation () const
{
  if (this->isTauIDAvailable ("byLooseCombinedIsolationDeltaBetaCorr3Hits"))
    return (this->tauID ("byLooseCombinedIsolationDeltaBetaCorr3Hits") > 0.5);
  else
    {
      edm::LogWarning ("osu_Tau") << "Tau ID \"byLooseCombinedIsolationDeltaBetaCorr3Hits\" unavailable.";
      return false;
    }
}

const bool
osu::Tau::passesMediumCombinedIsolation () const
{
  if (this->isTauIDAvailable ("byMediumCombinedIsolationDeltaBetaCorr3Hits"))
    return (this->tauID ("byMediumCombinedIsolationDeltaBetaCorr3Hits") > 0.5);
  else
    {
      edm::LogWarning ("osu_Tau") << "Tau ID \"byMediumCombinedIsolationDeltaBetaCorr3Hits\" unavailable.";
      return false;
    }
}

const bool
osu::Tau::passesTightCombinedIsolation () const
{
  if (this->isTauIDAvailable ("byTightCombinedIsolationDeltaBetaCorr3Hits"))
    return (this->tauID ("byTightCombinedIsolationDeltaBetaCorr3Hits") > 0.5);
  else
    {
      edm::LogWarning ("osu_Tau") << "Tau ID \"byTightCombinedIsolationDeltaBetaCorr3Hits\" unavailable.";
      return false;
    }
}

const bool
osu::Tau::passesTightMVAIsolation () const
{
#if CMSSW_VERSION_CODE < CMSSW_VERSION(8,0,0)
  const string idName = "byTightIsolationMVA3oldDMwLT";
#else
  const string idName = "byTightIsolationMVArun2v1DBoldDMwLT";
#endif

  if(this->isTauIDAvailable (idName))
    return (this->tauID (idName) > 0.5);
  else
    {
      edm::LogWarning ("osu_Tau") << "Tau Id \"" << idName << "\" unavailable.";
      return false;
    }
}
#endif

const double
osu::Tau::metMinusOnePt () const
{
  return metMinusOnePt_;
}

const double
osu::Tau::metMinusOnePx () const
{
  return metMinusOnePx_;
}

const double
osu::Tau::metMinusOnePy () const
{
  return metMinusOnePy_;
}

const double
osu::Tau::metMinusOnePhi () const
{
  return metMinusOnePhi_;
}

const double
osu::Tau::metNoMuMinusOnePt () const
{
  return metNoMuMinusOnePt_;
}

const double
osu::Tau::metNoMuMinusOnePx () const
{
  return metNoMuMinusOnePx_;
}

const double
osu::Tau::metNoMuMinusOnePy () const
{
  return metNoMuMinusOnePy_;
}

const double
osu::Tau::metNoMuMinusOnePhi () const
{
  return metNoMuMinusOnePhi_;
}

const double
osu::Tau::metMinusOneUpPt () const
{
  return metMinusOneUpPt_;
}

const double
osu::Tau::metMinusOneUpPx () const
{
  return metMinusOneUpPx_;
}

const double
osu::Tau::metMinusOneUpPy () const
{
  return metMinusOneUpPy_;
}

const double
osu::Tau::metMinusOneUpPhi () const
{
  return metMinusOneUpPhi_;
}

const double
osu::Tau::metNoMuMinusOneUpPt () const
{
  return metNoMuMinusOneUpPt_;
}

const double
osu::Tau::metNoMuMinusOneUpPx () const
{
  return metNoMuMinusOneUpPx_;
}

const double
osu::Tau::metNoMuMinusOneUpPy () const
{
  return metNoMuMinusOneUpPy_;
}

const double
osu::Tau::metNoMuMinusOneUpPhi () const
{
  return metNoMuMinusOneUpPhi_;
}

const bool
osu::Tau::match_HLT_LooseIsoPFTau50_Trk30_eta2p1_v () const
{
  return match_HLT_LooseIsoPFTau50_Trk30_eta2p1_v_;
}

const bool
osu::Tau::match_HLT_MediumChargedIsoPFTau50_Trk30_eta2p1_1pr_v () const
{
  return match_HLT_MediumChargedIsoPFTau50_Trk30_eta2p1_1pr_v_;
}

void
osu::Tau::set_match_HLT_LooseIsoPFTau50_Trk30_eta2p1_v (const bool flag)
{
  match_HLT_LooseIsoPFTau50_Trk30_eta2p1_v_ = flag;
}

void
osu::Tau::set_match_HLT_MediumChargedIsoPFTau50_Trk30_eta2p1_1pr_v (const bool flag)
{
  match_HLT_MediumChargedIsoPFTau50_Trk30_eta2p1_1pr_v_ = flag;
}

#endif
