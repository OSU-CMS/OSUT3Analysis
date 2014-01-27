#ifndef SF_WEIGHT

#define SF_WEIGHT

#include <iostream>
#include <fstream>
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



class TrackSFWeight
{
 public:
  TrackSFWeight () {};
  ~TrackSFWeight ();
  double at (const double &, const int &shiftUpDown = 0);

 private:
  double *trackSFWeight_;
};


class MuonSFWeight
  {
    public:
      MuonSFWeight () {};
      MuonSFWeight (const string &, const string &);
      ~MuonSFWeight ();
      double at (const double &, const double &, const int &shiftUpDown = 0);

    private:
      TH2 *muonSFWeight_;
  };



class ElectronSFWeight
  {
    public:
      ElectronSFWeight () {};
      ElectronSFWeight (const string &, const string &, const string &sfFile = "", const string &dataOverMC = "");
      ~ElectronSFWeight ();
      double at (const double &, const double &, const int &shiftUpDown = 0);

    private:
      string cmsswRelease_;
      string id_;

      TH2F *electronSFWeight_;
  };

#endif
