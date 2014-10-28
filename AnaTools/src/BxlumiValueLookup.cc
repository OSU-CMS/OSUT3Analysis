#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!bxlumi valueLookup
double
ValueLookup::valueLookup (const BNbxlumi* object, string variable){

  double value = 0.0;

  if(variable == "bx_B1_now") value = object->bx_B1_now;
  else if(variable == "bx_B2_now") value = object->bx_B2_now;
  else if(variable == "bx_LUMI_now") value = object->bx_LUMI_now;


  else{clog << "WARNING: invalid bxlumi variable '" << variable << "'\n"; value = numeric_limits<unsigned>::max ();}

  return value;
}
