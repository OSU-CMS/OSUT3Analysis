#include <iostream>

#include "TLorentzVector.h"

#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

//!track valueLookup
double
ValueLookupTree::valueLookup (const BNtrack * const object, const string &variable) const {

  double value = numeric_limits<int>::min ();

  clog << "WARNING: invalid track variable '" << variable << "'\n";

  return value;
} // end track valueLookup
