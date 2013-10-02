#ifndef SF_WEIGHT

#define SF_WEIGHT

#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <vector>
#include <cmath>


#include "TH1D.h"
#include "TH2D.h"
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
      double at (const double &, const double &);

    private:
      TH2 *muonSFWeight_;
  };



class ElectronSFWeight
  {
    public:
      ElectronSFWeight () {};
      ElectronSFWeight (const string &, const string &);
      ~ElectronSFWeight ();
      double at (const double &, const double &, const int &shiftUpDown = 0);

    private:
      string cmsswRelease_;
      string id_;
  };

#endif
