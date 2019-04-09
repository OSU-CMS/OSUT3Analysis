#ifndef OSU_MET
#define OSU_MET

#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

#if IS_VALID(mets)

namespace osu
{
  class Met : public TYPE(mets)
    {
      public:
        Met ();
        Met (const TYPE(mets) &);
        Met (const TYPE(mets) &, const edm::Handle<vector<pat::PackedCandidate> > &);
        ~Met ();

        // N.B.: "MET::JetResUpSmear" is not a disinct shift, rather
        //       it is "MET::JetResUp" for the case of MET::Type1Smear
        //       See https://github.com/cms-sw/cmssw/blob/CMSSW_7_4_14/DataFormats/PatCandidates/src/MET.cc#L208-L209

        const double pt_JetResUp () const          { return this->shiftedPt(MET::JetResUp); }
        const double pt_JetEnUp () const           { return this->shiftedPt(MET::JetEnUp); }
        const double pt_MuonEnUp () const          { return this->shiftedPt(MET::MuonEnUp); }
        const double pt_ElectronEnUp () const      { return this->shiftedPt(MET::ElectronEnUp); }
        const double pt_TauEnUp () const           { return this->shiftedPt(MET::TauEnUp); }
        const double pt_UnclusteredEnUp () const   { return this->shiftedPt(MET::UnclusteredEnUp); }
        const double pt_PhotonEnUp () const        { return this->shiftedPt(MET::PhotonEnUp); }

        const double pt_JetResDown () const        { return this->shiftedPt(MET::JetResDown); }
        const double pt_JetEnDown () const         { return this->shiftedPt(MET::JetEnDown); }
        const double pt_MuonEnDown () const        { return this->shiftedPt(MET::MuonEnDown); }
        const double pt_ElectronEnDown () const    { return this->shiftedPt(MET::ElectronEnDown); }
        const double pt_TauEnDown () const         { return this->shiftedPt(MET::TauEnDown); }
        const double pt_UnclusteredEnDown () const { return this->shiftedPt(MET::UnclusteredEnDown); }
        const double pt_PhotonEnDown () const      { return this->shiftedPt(MET::PhotonEnUp); }

        const double noMuPt () const;
        const double noMuPx () const;
        const double noMuPy () const;
        const double noMuPhi () const;

        const double noMuPt_JetResUp () const          { return noMuPt_JetResUp_; }
        const double noMuPt_JetEnUp () const           { return noMuPt_JetEnUp_; }
        const double noMuPt_ElectronEnUp () const      { return noMuPt_ElectronEnUp_; }
        const double noMuPt_TauEnUp () const           { return noMuPt_TauEnUp_; }
        const double noMuPt_UnclusteredEnUp () const   { return noMuPt_UnclusteredEnUp_; }
        const double noMuPt_PhotonEnUp () const        { return noMuPt_PhotonEnUp_; }

        const double noMuPt_JetResDown () const        { return noMuPt_JetResDown_; }
        const double noMuPt_JetEnDown () const         { return noMuPt_JetEnDown_; }
        const double noMuPt_ElectronEnDown () const    { return noMuPt_ElectronEnDown_; }
        const double noMuPt_TauEnDown () const         { return noMuPt_TauEnDown_; }
        const double noMuPt_UnclusteredEnDown () const { return noMuPt_UnclusteredEnDown_; }
        const double noMuPt_PhotonEnDown () const      { return noMuPt_PhotonEnDown_; }

        void setBadChargedCandidateFilter (const bool);
        void setBadPFMuonFilter (const bool);
        void setPassecalBadCalibFilterUpdate (const bool);

        const bool badChargedCandidateFilter () const { return badChargedCandidateFilter_; }
        const bool badPFMuonFilter () const { return badPFMuonFilter_; }
        const bool passecalBadCalibFilterUpdate () const { return passecalBadCalibFilterUpdate_; }

      private:
        double noMuPt_;
        double noMuPx_;
        double noMuPy_;
        double noMuPhi_;

        double noMuPt_JetResUp_;
        double noMuPt_JetEnUp_;
        double noMuPt_ElectronEnUp_;
        double noMuPt_TauEnUp_;
        double noMuPt_UnclusteredEnUp_;
        double noMuPt_PhotonEnUp_;

        double noMuPt_JetResDown_;
        double noMuPt_JetEnDown_;
        double noMuPt_ElectronEnDown_;
        double noMuPt_TauEnDown_;
        double noMuPt_UnclusteredEnDown_;
        double noMuPt_PhotonEnDown_;

        // Requesting met.shiftedP2(MET::JetResUpSmear) gives an error,
        // it's not actually a distinct value from MET::JetResUp.
        // Really if you use "MET::Type1Smear" corrections, then requesting MET::JetResUp gives MET::JetResUpSmear
        // https://github.com/cms-sw/cmssw/blob/CMSSW_7_4_14/DataFormats/PatCandidates/src/MET.cc#L208-L209
        //double noMuPt_JetResUpSmear_;
        //double noMuPt_JetResDownSmear_;

        bool badChargedCandidateFilter_;
        bool badPFMuonFilter_;
        bool passecalBadCalibFilterUpdate_;
    };
}

#else

namespace osu
{
  typedef TYPE(mets) Met;
}

#endif

#endif
