#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!bxlumi valueLookup
double
ValueLookup::valueLookup (const BNbxlumi* object, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "bx_B1_now") value = object->bx_B1_now;
  else if(variable == "bx_B2_now") value = object->bx_B2_now;
  else if(variable == "bx_LUMI_now") value = object->bx_LUMI_now;


  else{clog << "WARNING: invalid bxlumi variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
}
