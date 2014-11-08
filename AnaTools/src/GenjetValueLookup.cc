#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

//!genjet valueLookup
double
ValueLookupTree::valueLookup (const BNgenjet *object, string variable){

  double value = numeric_limits<int>::min ();

  clog << "WARNING: invalid genjet variable '" << variable << "'\n";

  return value;
}
