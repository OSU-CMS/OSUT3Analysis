#include <iostream>

#include "TLorentzVector.h"

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

//!track valueLookup
double
ValueLookupTree::valueLookup (const BNtrack *object, string variable){

  double value = numeric_limits<int>::min ();

  clog << "WARNING: invalid track variable '" << variable << "'\n";

  return value;
} // end track valueLookup
