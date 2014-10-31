#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!userVariable valueLookup
double
ValueLookup::valueLookup (const map<string,double> &object, string variable){

  double value = 0.0;
  string empty = "";
  if (object.count(variable)==0) {
    clog << "Error:  could not find variable " << variable << " in userVariable collection, which has size = " << object.size() << ".  Will return default value of 0." << endl;     
  } else {
    value = object.at(variable);  
  }

  return value;

}  






