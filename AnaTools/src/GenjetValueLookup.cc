#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!genjet valueLookup
double
ValueLookup::valueLookup (const BNgenjet* object, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "pt") value = object->pt;
  else if(variable == "eta") value = object->eta;
  else if(variable == "phi") value = object->phi;
  else if(variable == "px") value = object->px;
  else if(variable == "py") value = object->py;
  else if(variable == "pz") value = object->pz;
  else if(variable == "et") value = object->et;
  else if(variable == "energy") value = object->energy;
  else if(variable == "mass") value = object->mass;
  else if(variable == "emEnergy") value = object->emEnergy;
  else if(variable == "hadEnergy") value = object->hadEnergy;
  else if(variable == "invisibleEnergy") value = object->invisibleEnergy;
  else if(variable == "auxiliaryEnergy") value = object->auxiliaryEnergy;
  else if(variable == "charge") value = object->charge;


  else{clog << "WARNING: invalid genjet variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
}
