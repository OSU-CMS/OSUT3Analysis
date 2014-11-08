#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

//!trigobj valueLookup
double
ValueLookupTree::valueLookup (const BNtrigobj *object, string variable){

  double value = numeric_limits<int>::min ();

  clog << "WARNING: invalid trigobj variable '" << variable << "'\n";

  return value;
}
