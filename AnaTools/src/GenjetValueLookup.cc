#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!genjet valueLookup
double
ValueLookup::valueLookup (const BNgenjet &object, string variable){

  double value = 0.0;
  BNgenjet *obj = new BNgenjet (object);

  try
    {
      value = getMember ("BNgenjet", obj, variable);
    }
  catch (...)
    {
      clog << "WARNING: invalid genjet variable '" << variable << "'\n";
      value = numeric_limits<int>::min ();
    }

  delete obj;
  return value;
}
