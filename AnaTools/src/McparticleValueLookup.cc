#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

//!mcparticle valueLookup
double
ValueLookupTree::valueLookup (const BNmcparticle * const object, const string &variable) const {

  double value = numeric_limits<int>::min ();

  clog << "WARNING: invalid mcparticle variable '" << variable << "'\n";

  return value;
} // end mcparticle valueLookup
