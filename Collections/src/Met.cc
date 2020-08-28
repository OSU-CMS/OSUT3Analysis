#include "TVector2.h"

#include "OSUT3Analysis/Collections/interface/Met.h"

#if IS_VALID(mets)

osu::Met::Met ()
{
}

osu::Met::Met (const TYPE(mets) &met) :
  TYPE(mets) (met),
  noMuPt_                   (INVALID_VALUE),
  noMuPx_                   (INVALID_VALUE),
  noMuPy_                   (INVALID_VALUE),
  noMuPhi_                  (INVALID_VALUE),
  noMuPt_JetResUp_          (INVALID_VALUE),
  noMuPt_JetEnUp_           (INVALID_VALUE),
  noMuPt_ElectronEnUp_      (INVALID_VALUE),
  noMuPt_TauEnUp_           (INVALID_VALUE),
  noMuPt_UnclusteredEnUp_   (INVALID_VALUE),
  noMuPt_PhotonEnUp_        (INVALID_VALUE),
  noMuPt_JetResDown_        (INVALID_VALUE),
  noMuPt_JetEnDown_         (INVALID_VALUE),
  noMuPt_ElectronEnDown_    (INVALID_VALUE),
  noMuPt_TauEnDown_         (INVALID_VALUE),
  noMuPt_UnclusteredEnDown_ (INVALID_VALUE),
  noMuPt_PhotonEnDown_      (INVALID_VALUE),
  badChargedCandidateFilter_ (true),
  badPFMuonFilter_           (true),
  passecalBadCalibFilterUpdate_ (true)
{
}

osu::Met::Met (const TYPE(mets) &met, const edm::Handle<vector<pat::PackedCandidate> > &pfCandidates) :
  TYPE(mets) (met),
  noMuPt_                   (INVALID_VALUE),
  noMuPx_                   (INVALID_VALUE),
  noMuPy_                   (INVALID_VALUE),
  noMuPhi_                  (INVALID_VALUE),
  noMuPt_JetResUp_          (INVALID_VALUE),
  noMuPt_JetEnUp_           (INVALID_VALUE),
  noMuPt_ElectronEnUp_      (INVALID_VALUE),
  noMuPt_TauEnUp_           (INVALID_VALUE),
  noMuPt_UnclusteredEnUp_   (INVALID_VALUE),
  noMuPt_PhotonEnUp_        (INVALID_VALUE),
  noMuPt_JetResDown_        (INVALID_VALUE),
  noMuPt_JetEnDown_         (INVALID_VALUE),
  noMuPt_ElectronEnDown_    (INVALID_VALUE),
  noMuPt_TauEnDown_         (INVALID_VALUE),
  noMuPt_UnclusteredEnDown_ (INVALID_VALUE),
  noMuPt_PhotonEnDown_      (INVALID_VALUE),
  badChargedCandidateFilter_ (true),
  badPFMuonFilter_           (true),
  passecalBadCalibFilterUpdate_ (true)
{
  if (pfCandidates.isValid ()) {
      TVector2 metNoMu (met.px(), met.py());

#if DATA_FORMAT != AOD
      TVector2 metNoMu_JetResUp          (met.shiftedP2(MET::JetResUp).px, met.shiftedP2(MET::JetResUp).py);
      TVector2 metNoMu_JetEnUp           (met.shiftedP2(MET::JetEnUp).px, met.shiftedP2(MET::JetEnUp).py);
      TVector2 metNoMu_ElectronEnUp      (met.shiftedP2(MET::ElectronEnUp).px, met.shiftedP2(MET::ElectronEnUp).py);
      TVector2 metNoMu_TauEnUp           (met.shiftedP2(MET::TauEnUp).px, met.shiftedP2(MET::TauEnUp).py);
      TVector2 metNoMu_UnclusteredEnUp   (met.shiftedP2(MET::UnclusteredEnUp).px, met.shiftedP2(MET::UnclusteredEnUp).py);
      TVector2 metNoMu_PhotonEnUp        (met.shiftedP2(MET::PhotonEnUp).px, met.shiftedP2(MET::PhotonEnUp).py);

      TVector2 metNoMu_JetResDown        (met.shiftedP2(MET::JetResDown).px, met.shiftedP2(MET::JetResDown).py);
      TVector2 metNoMu_JetEnDown         (met.shiftedP2(MET::JetEnDown).px, met.shiftedP2(MET::JetEnDown).py);
      TVector2 metNoMu_ElectronEnDown    (met.shiftedP2(MET::ElectronEnDown).px, met.shiftedP2(MET::ElectronEnDown).py);
      TVector2 metNoMu_TauEnDown         (met.shiftedP2(MET::TauEnDown).px, met.shiftedP2(MET::TauEnDown).py);
      TVector2 metNoMu_UnclusteredEnDown (met.shiftedP2(MET::UnclusteredEnDown).px, met.shiftedP2(MET::UnclusteredEnDown).py);
      TVector2 metNoMu_PhotonEnDown      (met.shiftedP2(MET::PhotonEnDown).px, met.shiftedP2(MET::PhotonEnDown).py);
#endif

      for (const auto &pfCandidate : *pfCandidates) {
          if (abs (pfCandidate.pdgId ()) != 13) continue;

          TVector2 muon (pfCandidate.px (), pfCandidate.py ());

          metNoMu += muon;

#if DATA_FORMAT != AOD
          metNoMu_JetResUp += muon;
          metNoMu_JetEnUp += muon;
          metNoMu_ElectronEnUp += muon;
          metNoMu_TauEnUp += muon;
          metNoMu_UnclusteredEnUp += muon;

          metNoMu_JetResDown += muon;
          metNoMu_JetEnDown += muon;
          metNoMu_ElectronEnDown += muon;
          metNoMu_TauEnDown += muon;
          metNoMu_UnclusteredEnDown += muon;
#endif
        }

      noMuPt_ = metNoMu.Mod ();
      noMuPx_ = metNoMu.Px ();
      noMuPy_ = metNoMu.Py ();
      noMuPhi_ = metNoMu.Phi ();

#if DATA_FORMAT != AOD
      noMuPt_JetResUp_ = metNoMu_JetResUp.Mod();
      noMuPt_JetEnUp_ = metNoMu_JetEnUp.Mod();
      noMuPt_ElectronEnUp_ = metNoMu_ElectronEnUp.Mod();
      noMuPt_TauEnUp_ = metNoMu_TauEnUp.Mod();
      noMuPt_UnclusteredEnUp_ = metNoMu_UnclusteredEnUp.Mod();
      noMuPt_PhotonEnUp_ = metNoMu_PhotonEnUp.Mod();

      noMuPt_JetResDown_ = metNoMu_JetResDown.Mod();
      noMuPt_JetEnDown_ = metNoMu_JetEnDown.Mod();
      noMuPt_ElectronEnDown_ = metNoMu_ElectronEnDown.Mod();
      noMuPt_TauEnDown_ = metNoMu_TauEnDown.Mod();
      noMuPt_UnclusteredEnDown_ = metNoMu_UnclusteredEnDown.Mod();
      noMuPt_PhotonEnDown_ = metNoMu_PhotonEnDown.Mod();
#endif

    }
}

osu::Met::~Met ()
{
}

const double
osu::Met::noMuPt () const
{
  return noMuPt_;
}

const double
osu::Met::noMuPx () const
{
  return noMuPx_;
}

const double
osu::Met::noMuPy () const
{
  return noMuPy_;
}

const double
osu::Met::noMuPhi () const
{
  return noMuPhi_;
}

void
osu::Met::setBadChargedCandidateFilter (const bool flag)
{
  badChargedCandidateFilter_ = flag;
}

void
osu::Met::setBadPFMuonFilter (const bool flag)
{
  badPFMuonFilter_ = flag;
}

void
osu::Met::setPassecalBadCalibFilterUpdate (const bool flag)
{
  passecalBadCalibFilterUpdate_ = flag;
}
#endif
