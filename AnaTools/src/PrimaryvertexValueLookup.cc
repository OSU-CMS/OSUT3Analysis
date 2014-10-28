#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!primaryvertex valueLookup
double
ValueLookup::valueLookup (const BNprimaryvertex* object, string variable){

  double value = 0.0;

  if(variable == "x") value = object->x;
  else if(variable == "xError") value = object->xError;
  else if(variable == "y") value = object->y;
  else if(variable == "yError") value = object->yError;
  else if(variable == "z") value = object->z;
  else if(variable == "zError") value = object->zError;
  else if(variable == "rho") value = object->rho;
  else if(variable == "normalizedChi2") value = object->normalizedChi2;
  else if(variable == "ndof") value = object->ndof;
  else if(variable == "isFake") value = object->isFake;
  else if(variable == "isValid") value = object->isValid;
  else if(variable == "tracksSize") value = object->tracksSize;
  else if(variable == "isGood") value = object->isGood;


  else{clog << "WARNING: invalid primaryvertex variable '" << variable << "'\n"; value = numeric_limits<unsigned>::max ();}

  return value;
}
