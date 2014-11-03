#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!supercluster valueLookup
double
ValueLookup::valueLookup (const BNsupercluster &object, string variable){

  double value = numeric_limits<int>::min ();
  BNsupercluster *obj = new BNsupercluster (object);

  try
    {
      value = getMember ("BNsupercluster", obj, variable);
    }
  catch (...)
    {
      clog << "WARNING: invalid supercluster variable '" << variable << "'\n";
    }

  delete obj;
  return value;
}
