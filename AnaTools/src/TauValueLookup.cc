#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

//!tau valueLookup
double
ValueLookupTree::valueLookup (const BNtau *object, string variable){

  double value = numeric_limits<int>::min ();

  clog << "WARNING: invalid tau variable '" << variable << "'\n";

  return value;
} // end tau valueLookup
