#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

//!bxlumi valueLookup
double
ValueLookupTree::valueLookup (const BNbxlumi * const object, const string &variable) const {

  double value = numeric_limits<int>::min ();

  clog << "WARNING: invalid bxlumi variable '" << variable << "'\n";

  return value;
}
