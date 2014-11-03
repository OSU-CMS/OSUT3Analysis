#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!bxlumi valueLookup
double
ValueLookup::valueLookup (const BNbxlumi &object, string variable){

  double value = numeric_limits<int>::min ();
  BNbxlumi *obj = new BNbxlumi (object);

  try
    {
      value = getMember ("BNbxlumi", obj, variable);
    }
  catch (...)
    {
      clog << "WARNING: invalid bxlumi variable '" << variable << "'\n";
    }

  delete obj;
  return value;
}
