#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

//!trigobj valueLookup
double
ValueLookupTree::valueLookup (const BNtrigobj * const object, const string &variable) const {

  double value = numeric_limits<int>::min ();

  clog << "WARNING: invalid trigobj variable '" << variable << "'\n";

  return value;
}
