#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

//!bxlumi valueLookup
double
ValueLookupTree::valueLookup (const BNbxlumi *object, string variable){

  double value = numeric_limits<int>::min ();

  clog << "WARNING: invalid bxlumi variable '" << variable << "'\n";

  return value;
}
