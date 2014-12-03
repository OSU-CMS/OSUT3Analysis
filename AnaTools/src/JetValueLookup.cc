#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

//!jet valueLookup
double
ValueLookupTree::valueLookup (const BNjet * const object, const string &variable) const {

  double value = numeric_limits<int>::min ();

  clog << "WARNING: invalid jet variable '" << variable << "'\n";

  return value;
} // end jet valueLookup
