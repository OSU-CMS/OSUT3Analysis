#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!mcparticle valueLookup
double
ValueLookup::valueLookup (const BNmcparticle &object, string variable){

  double value = 0.0;
  BNmcparticle *obj = new BNmcparticle (object);

  try
    {
      value = getMember ("BNmcparticle", obj, variable);
    }
  catch (...)
    {
      //user-defined variables
      if(variable == "genMatchedPdgGrandmotherId"){
        if(fabs(object.motherId) == 15){
          int motherIndex = findTauMotherIndex(&object);
          if(motherIndex == -1) value = 0;
          else value = mcparticles->at(motherIndex).motherId;
        }
        else value = object.grandMotherId;
      }

      else if(variable == "genMatchedId") value = getPdgIdBinValue(object.id);
      else if(variable == "genMatchedMotherId") value = getPdgIdBinValue(object.motherId);
      else if(variable == "genMatchedMotherIdReverse") value = 24 - getPdgIdBinValue(object.motherId);
      else if(variable == "genMatchedGrandmotherId"){
        if(fabs(object.motherId) == 15){
          int motherIndex = findTauMotherIndex(&object);
          if(motherIndex == -1) value = 0;
          else value = getPdgIdBinValue(mcparticles->at(motherIndex).motherId);
        }
        else value = getPdgIdBinValue(object.grandMotherId);
      }
      else if(variable == "genMatchedGrandmotherIdReverse"){
        if(fabs(object.motherId) == 15){
          int motherIndex = findTauMotherIndex(&object);
          if(motherIndex == -1) value = 0;
          else value = 24 - getPdgIdBinValue(mcparticles->at(motherIndex).motherId);
        }
        else value = 24 - getPdgIdBinValue(object.grandMotherId);
      }

      else if (variable == "d0Vertex"){
        double vx = object.vx - chosenVertex ()->x,
          vy = object.vy - chosenVertex ()->y,
          px = object.px,
          py = object.py,
          pt = object.pt;
        value = (-vx * py + vy * px) / pt;
      }
      else if (variable == "d0Beamspot"){
        double vx = object.vx - events->at(0).BSx,
          vy = object.vy - events->at(0).BSy,
          px = object.px,
          py = object.py,
          pt = object.pt;
        value = (-vx * py + vy * px) / pt;
      }
      else if (variable == "d0Origin"){
        double vx = object.vx,
          vy = object.vy,
          px = object.px,
          py = object.py,
          pt = object.pt;
        value = (-vx * py + vy * px) / pt;
      }

      else if (variable == "dz"){
        double vx = object.vx - chosenVertex ()->x,
          vy = object.vy - chosenVertex ()->y,
          vz = object.vz - chosenVertex ()->z,
          px = object.px,
          py = object.py,
          pz = object.pz,
          pt = object.pt;
        value = vz - (vx * px + vy * py)/pt * (pz/pt);
      }
      else if(variable == "v0"){
        value = sqrt(object.vx*object.vx + object.vy*object.vy);
      }
      else if(variable == "deltaV0"){
        value = sqrt((object.vx-chosenVertex ()->x)*(object.vx-chosenVertex ()->x) + (object.vy-chosenVertex ()->y)*(object.vy-chosenVertex ()->y));
      }
      else if (variable == "deltaVx"){
        value = object.vx - chosenVertex ()->x;
      }
      else if (variable == "deltaVy"){
        value = object.vy - chosenVertex ()->y;
      }
      else if (variable == "deltaVz"){
        value = object.vz - chosenVertex ()->z;
      }
      else if (variable == "rho"){
        value = sqrt (object.vx * object.vx + object.vy * object.vy);
      }

/*      else if(variable == "deltaPhiMaxSubLeadJet") {
        // calculate maximum deltaPhi between mcparticle and any subleading jet
        double mcpartJetDeltaPhiMax = -99.;
        if (!jets.product()) clog << "ERROR:  cannot find deltaPhiMaxSubLeadJet because jets collection is not initialized." << endl;
        for (uint ijet = 0; ijet<jets->size(); ijet++) {
          double isSubLeadingJet = valueLookup(&jets->at(ijet), "disappTrkSubLeadingJetID");
          if (!isSubLeadingJet) continue;  // only consider jets that pass the subleading jet ID criteria
          double jetPhi = valueLookup(&jets->at(ijet), "phi");
          double mcpartJetDeltaPhi = fabs(deltaPhi(object.phi, jetPhi));
          if (mcpartJetDeltaPhi > mcpartJetDeltaPhiMax) mcpartJetDeltaPhiMax = mcpartJetDeltaPhi;
        }
        value = mcpartJetDeltaPhiMax;
      }*/

      else{
        clog << "WARNING: invalid mcparticle variable '" << variable << "'\n";
        value = numeric_limits<int>::min ();
      }
    }

  delete obj;
  return value;
} // end mcparticle valueLookup
