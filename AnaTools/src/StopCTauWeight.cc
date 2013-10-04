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
  double weight, ctau0, ctau1;

  if (stops.size () != 2)
    cout << "Wrong number of stops!: " << stops.size () << endl;
  ctau0 = stops.at (0).ctau;
  ctau1 = stops.at (1).ctau;

  weight = (currentCTau_ / targetCTau_) * (currentCTau_ / targetCTau_);
  weight *= exp (-(ctau0 + ctau1) * ((1 / targetCTau_) - (1 / currentCTau_)));

  return weight;
}

double
StopCTauWeight::operator[] (const edm::Event &event)
{
  edm::Handle<BNstopCollection> stops;
  event.getByLabel (stops_, stops);

  return (*this)[*stops.product ()];
}
