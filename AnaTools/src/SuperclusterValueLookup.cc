#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

//!supercluster valueLookup
double
ValueLookupTree::valueLookup (const BNsupercluster * const object, const string &variable) const {

  double value = numeric_limits<int>::min ();

  clog << "WARNING: invalid supercluster variable '" << variable << "'\n";

  return value;
}
