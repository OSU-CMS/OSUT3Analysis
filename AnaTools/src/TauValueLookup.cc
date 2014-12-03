#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

//!tau valueLookup
double
ValueLookupTree::valueLookup (const BNtau * const object, const string &variable) const {

  double value = numeric_limits<int>::min ();

  clog << "WARNING: invalid tau variable '" << variable << "'\n";

  return value;
} // end tau valueLookup
