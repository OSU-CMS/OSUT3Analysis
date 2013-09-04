#ifndef STOP_CTAU_WEIGHT

#define STOP_CTAU_WEIGHT

#include <iostream>
#include <cmath>
#include <string>

#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"

#include "OSUT3Analysis/AnaTools/interface/BNstop.h"

using namespace std;

class StopCTauWeight
  {
    public:
      StopCTauWeight () {};
      StopCTauWeight (double, double);
      StopCTauWeight (double, double, edm::InputTag);
      ~StopCTauWeight ();
      double operator[] (const BNstopCollection &stops);
      double at (const BNstopCollection &stops) { return (*this)[stops]; };
      double operator[] (const edm::Event &event);
      double at (const edm::Event &event) { return (*this)[event]; };
      void setCurrentCTau (double currentCTau) { currentCTau_ = currentCTau; };
      void setTargetCTau (double targetCTau) { targetCTau_ = targetCTau; };

    private:
      double currentCTau_;
      double targetCTau_;
      edm::InputTag stops_;
  };

#endif
