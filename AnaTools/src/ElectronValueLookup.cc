#include <iostream>

#include "TVector2.h"

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!electron valueLookup
double
ValueLookup::valueLookup (const BNelectron &object, string variable){

  double value = numeric_limits<int>::min ();
  BNelectron *obj = new BNelectron (object);

  try
    {
      value = getMember ("BNelectron", obj, variable);
    }
  catch (...)
    {
      //user-defined variables
      if(variable == "vertexDistZ") value = object.vz - chosenVertex()->z;
      else if(variable == "correctedD0VertexErr") value =  hypot (object.tkD0err, hypot (chosenVertex ()->xError, chosenVertex ()->yError));
      else if(variable == "correctedD0VertexSig") value =  object.correctedD0Vertex / hypot (object.tkD0err, hypot (chosenVertex ()->xError, chosenVertex ()->yError));
      else if(variable == "correctedD0Err") value =  hypot (object.tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
      else if(variable == "correctedD0Sig") value =  object.correctedD0 / hypot (object.tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
      else if(variable == "metMT") {
        if (const BNmet *met = chosenMET ())
          {
            double dPhi = deltaPhi (object.phi, met->phi);
            value = sqrt (2 * object.pt * met->pt * (1 - cos (dPhi)));
          }
        else
          value = numeric_limits<int>::min ();
      }
      else if(variable == "metDeltaPhi") {
        if (const BNmet *met = chosenMET ())
          value = deltaPhi (object.phi, met->phi);
        else
          value = numeric_limits<int>::min ();
      }
      else if(variable == "ptPlusMet") {
        // Calculate the magnitude of the vector sum of the electron pT and the Met.
        if (const BNmet *met = chosenMET ())
          {
            TVector2 p2Met;
            TVector2 p2Elec;
            p2Met. SetMagPhi(   met->pt,    met->phi);
            p2Elec.SetMagPhi(object.pt, object.phi);
            TVector2 p2MetElec = p2Met + p2Elec;
            value = p2MetElec.Mod();
          }
        else
          value = numeric_limits<int>::min ();
      }

      else if(variable == "genDeltaRLowest") value = getGenDeltaRLowest(&object);

      else if(variable == "genMatchedPdgId"){
        int index = getGenMatchedParticleIndex(&object);
        if(index == -1) value = 0;
        else value = mcparticles->at(index).id;
      }

      else if(variable == "genRecoChargeProduct"){
        int index = getGenMatchedParticleIndex(&object);
        if(index == -1) value = 2;
        else value = object.charge * mcparticles->at(index).charge;
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
      else if(variable == "pfElectronsFromVertex"){
        double d0Error, dzError;

        d0Error = hypot (object.tkD0err, hypot (chosenVertex ()->xError, chosenVertex ()->yError));
        dzError = hypot (object.tkDZerr, chosenVertex ()->zError);
        value = fabs (object.correctedD0Vertex) > 0.2 || fabs (object.correctedDZ) > 0.5
          || fabs (object.correctedD0Vertex / d0Error) > 99.0
          || fabs (object.correctedDZ / dzError) > 99.0;
        value = !value;
      }

      else
        clog << "WARNING: invalid electron variable '" << variable << "'\n";
    }

  delete obj;
  return value;
} // end electron valueLookup
