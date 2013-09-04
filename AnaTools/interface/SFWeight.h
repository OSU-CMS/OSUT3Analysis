#ifndef SF_WEIGHT

#define SF_WEIGHT

#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <vector>
#include <cmath>

#include "TH1D.h"
#include "TGraphAsymmErrors.h"
#include "TFile.h"

using namespace std;

class MuonSFWeight
  {
    public:
      MuonSFWeight () {};
      MuonSFWeight (const string &, const string &);
      MuonSFWeight (const string &, const string &, const string &);
      ~MuonSFWeight ();
      double operator[] (const double &eta) { return muonSFWeight_->FindBin (eta) > muonSFWeight_->GetNbinsX () ? 0.0 : muonSFWeight_->GetBinContent (muonSFWeight_->FindBin (eta)); };
      double at (const double &eta) { return (*this)[eta]; };

    private:
      TH1D *muonSFWeight_;
  };

class ElectronSFWeight
  {
    public:
      ElectronSFWeight () {};
      ElectronSFWeight (const string &, const string &);
      ~ElectronSFWeight ();
      double at (const double &, const double &);

    private:
      string cmsswRelease_;
      string id_;
  };

#endif
