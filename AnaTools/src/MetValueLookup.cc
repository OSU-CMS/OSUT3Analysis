#include <iostream>

#include "TVector2.h"

#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

//!met valueLookup
double
ValueLookupTree::valueLookup (const BNmet * const object, const string &variable) const {

  double value = numeric_limits<int>::min ();

  clog << "WARNING: invalid met variable '" << variable << "'\n";

  return value;
} // end met valueLookup
