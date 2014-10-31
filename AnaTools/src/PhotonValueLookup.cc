#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!photon valueLookup
double
ValueLookup::valueLookup (const BNphoton &object, string variable){

  double value = 0.0;
  BNphoton *obj = new BNphoton (object);

  try
    {
      value = getMember ("BNphoton", obj, variable);
    }
  catch (...)
    {
      if(variable == "genDeltaRLowest") value = getGenDeltaRLowest(&object);

      else if(variable == "genMatchedPdgId"){
        int index = getGenMatchedParticleIndex(&object);
        if(index == -1) value = 0;
        else value = mcparticles->at(index).id;
      }



      else if(variable == "genMatchedId"){
        int index = getGenMatchedParticleIndex(&object);
        if(index == -1) value = 0;
        else value = getPdgIdBinValue(mcparticles->at(index).id);
      }
      else if(variable == "genMatchedMotherId"){
        int index = getGenMatchedParticleIndex(&object);
        if(index == -1) value = 0;
        else value = getPdgIdBinValue(mcparticles->at(index).motherId);
      }
      else if(variable == "genMatchedMotherIdReverse"){
        int index = getGenMatchedParticleIndex(&object);
        if(index == -1) value = 24;
        else value = 24 -getPdgIdBinValue(mcparticles->at(index).motherId);
      }
      else if(variable == "genMatchedGrandmotherId"){
        int index = getGenMatchedParticleIndex(&object);
        if(index == -1) value = 0;
        else if(fabs(mcparticles->at(index).motherId) == 15){
          int motherIndex = findTauMotherIndex(&mcparticles->at(index));
          if(motherIndex == -1) value = 0;
          else value = getPdgIdBinValue(mcparticles->at(motherIndex).motherId);
        }
        else value = getPdgIdBinValue(mcparticles->at(index).grandMotherId);
      }
      else if(variable == "genMatchedGrandmotherIdReverse"){
        int index = getGenMatchedParticleIndex(&object);
        if(index == -1) value = 24;
        else if(fabs(mcparticles->at(index).motherId) == 15){
          int motherIndex = findTauMotherIndex(&mcparticles->at(index));
          if(motherIndex == -1) value = 24;
          else value = 24 - getPdgIdBinValue(mcparticles->at(motherIndex).motherId);
        }
        else value = 24 - getPdgIdBinValue(mcparticles->at(index).grandMotherId);
      }


      else{
        clog << "WARNING: invalid photon variable '" << variable << "'\n";
        value = numeric_limits<int>::min ();
      }
    }

  delete obj;
  return value;
} // end photon valueLookup
