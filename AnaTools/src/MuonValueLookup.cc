#include <iostream>

#include "TVector2.h"

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

//!muon valueLookup
double
ValueLookupTree::valueLookup (const BNmuon *object, string variable){

  double value = numeric_limits<int>::min ();

  clog << "WARNING: invalid muon variable '" << variable << "'\n";

  return value;
} // end muon valueLookup
