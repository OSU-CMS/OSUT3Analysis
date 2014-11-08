#include <iostream>

#include "TVector2.h"

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

//!electron valueLookup
double
ValueLookupTree::valueLookup (const BNelectron *object, string variable){

  double value = numeric_limits<int>::min ();

  clog << "WARNING: invalid electron variable '" << variable << "'\n";

  return value;
} // end electron valueLookup
