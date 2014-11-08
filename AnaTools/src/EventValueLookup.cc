#include <iostream>

#include "TLorentzVector.h"
#include "TVector2.h"

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

//!event valueLookup
double
ValueLookupTree::valueLookup (const BNevent *object, string variable){

  double value = numeric_limits<int>::min ();

  clog << "WARNING: invalid event variable '" << variable << "'\n";

  return value;
} // end event valueLookup
