#include "OSUT3Analysis/AnaTools/interface/StopCTauWeight.h"

StopCTauWeight::StopCTauWeight (double currentCTau, double targetCTau) :
  currentCTau_ (currentCTau),
  targetCTau_ (targetCTau)
{
}

StopCTauWeight::StopCTauWeight (double currentCTau, double targetCTau, edm::InputTag stops) :
  currentCTau_ (currentCTau),
  targetCTau_ (targetCTau),
  stops_ (stops)
{
}

StopCTauWeight::~StopCTauWeight ()
{
}

double
StopCTauWeight::operator[] (const BNstopCollection &stops)
{

  if (stops.size () > 2) cout << "Too many stops!: " << stops.size () << endl;
  double  weight = 1.0;  
  for (uint i=0; i<stops.size(); i++) { 
    double ctau = stops.at(i).ctau;
    double wtTarget  = (1. /  targetCTau_) * exp(-(ctau) /  targetCTau_ );  
    double wtCurrent = (1. / currentCTau_) * exp(-(ctau) / currentCTau_ );  
    double wt = wtTarget / wtCurrent;  
    if (ctau==0) {
      cout << "Warning[StopCTauWeight]:  Found event with ctau==0." << endl;  
      //      wt = 0;  // Set 0 weight for any event with a ctau of identically 0; indicates that a problem occurred.  
    }
    weight *= wt;  
  }


  return weight;
}

double
StopCTauWeight::operator[] (const edm::Event &event)
{
  edm::Handle<BNstopCollection> stops;
  event.getByLabel (stops_, stops);

  return (*this)[*stops.product ()];
}
