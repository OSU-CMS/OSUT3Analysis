#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

//!primaryvertex valueLookup
double
ValueLookupTree::valueLookup (const BNprimaryvertex * const object, const string &variable) const {

  double value = numeric_limits<int>::min ();

  clog << "WARNING: invalid primaryvertex variable '" << variable << "'\n";

  return value;
}
