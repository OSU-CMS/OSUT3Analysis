#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!supercluster valueLookup
double
ValueLookup::valueLookup (const BNsupercluster* object, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "energy") value = object->energy;
  else if(variable == "et") value = object->et;
  else if(variable == "ex") value = object->ex;
  else if(variable == "ey") value = object->ey;
  else if(variable == "ez") value = object->ez;
  else if(variable == "phi") value = object->phi;
  else if(variable == "eta") value = object->eta;
  else if(variable == "theta") value = object->theta;


  else{clog << "WARNING: invalid supercluster variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
}
