#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!primaryvertex valueLookup
double
ValueLookup::valueLookup (const BNprimaryvertex &object, string variable){

  double value = 0.0;
  BNprimaryvertex *obj = new BNprimaryvertex (object);

  try
    {
      value = getMember ("BNprimaryvertex", obj, variable);
    }
  catch (...)
    {
      clog << "WARNING: invalid primaryvertex variable '" << variable << "'\n";
      value = numeric_limits<int>::min ();
    }

  delete obj;
  return value;
}
