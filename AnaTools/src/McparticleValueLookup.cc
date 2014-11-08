#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

//!mcparticle valueLookup
double
ValueLookupTree::valueLookup (const BNmcparticle *object, string variable){

  double value = numeric_limits<int>::min ();

  clog << "WARNING: invalid mcparticle variable '" << variable << "'\n";

  return value;
} // end mcparticle valueLookup
