#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

//!photon valueLookup
double
ValueLookupTree::valueLookup (const BNphoton *object, string variable){

  double value = numeric_limits<int>::min ();

  clog << "WARNING: invalid photon variable '" << variable << "'\n";

  return value;
} // end photon valueLookup
