#ifndef CUT_FLOW

#define CUT_FLOW

#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <vector>

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TH1D.h"
#include "TStopwatch.h"

using namespace std;

class CutFlow
  {
    typedef void (CutFlow::*boolType) () const;

    public:
      CutFlow () {};
      CutFlow (const edm::Service<TFileService> &, const string &prefix = "");
      ~CutFlow ();
      bool &operator[] (const string &);
      bool &at (const string &cutName) { return (*this)[cutName]; };
      bool pass () const;
      operator boolType () const { return pass () ? &CutFlow::thisTypeDoesNotSupportComparisons : 0; };

      void fillCutFlow (double);
      void fillCutFlow () { fillCutFlow (1.0); };
      void outputTime ();
      void outputCutFlow ();

    private:
      TH1D *cutFlow_;
      TH1D *selection_;
      TH1D *minusOne_;

      TStopwatch sw_;
      string prefix_;
      vector<string> cutNames_;
      map<string, bool> cuts_;

      void thisTypeDoesNotSupportComparisons () const {};
  };

#endif
