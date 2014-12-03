#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

//!genjet valueLookup
double
ValueLookupTree::valueLookup (const BNgenjet * const object, const string &variable) const {

  double value = numeric_limits<int>::min ();

  clog << "WARNING: invalid genjet variable '" << variable << "'\n";

  return value;
}
