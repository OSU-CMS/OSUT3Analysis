#ifndef PU_WEIGHT

#define PU_WEIGHT

#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <vector>

#include "TH1D.h"
#include "TFile.h"

using namespace std;

class PUWeight
  {
    public:
      PUWeight () {};
      PUWeight (const string &, const string &, const string &);
      ~PUWeight ();
      double operator[] (const unsigned &pu) { return puWeight_->GetBinContent (puWeight_->FindBin (pu)); };
      double at (const unsigned &pu) { return (*this)[pu]; };

    private:
      TH1D *puWeight_;
  };

#endif
