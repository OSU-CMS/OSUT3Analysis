#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!mcparticle valueLookup
double
ValueLookup::valueLookup (const BNmcparticle* object, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "energy") value = object->energy;
  else if(variable == "et") value = object->et;
  else if(variable == "pt") value = object->pt;
  else if(variable == "px") value = object->px;
  else if(variable == "py") value = object->py;
  else if(variable == "pz") value = object->pz;
  else if(variable == "phi") value = object->phi;
  else if(variable == "eta") value = object->eta;
  else if(variable == "theta") value = object->theta;
  else if(variable == "mass") value = object->mass;
  else if(variable == "vx") value = object->vx;
  else if(variable == "vy") value = object->vy;
  else if(variable == "vz") value = object->vz;
  else if(variable == "motherET") value = object->motherET;
  else if(variable == "motherPT") value = object->motherPT;
  else if(variable == "motherPhi") value = object->motherPhi;
  else if(variable == "motherEta") value = object->motherEta;
  else if(variable == "mother0ET") value = object->mother0ET;
  else if(variable == "mother0PT") value = object->mother0PT;
  else if(variable == "mother0Phi") value = object->mother0Phi;
  else if(variable == "mother0Eta") value = object->mother0Eta;
  else if(variable == "mother1ET") value = object->mother1ET;
  else if(variable == "mother1PT") value = object->mother1PT;
  else if(variable == "mother1Phi") value = object->mother1Phi;
  else if(variable == "mother1Eta") value = object->mother1Eta;
  else if(variable == "daughter0ET") value = object->daughter0ET;
  else if(variable == "daughter0PT") value = object->daughter0PT;
  else if(variable == "daughter0Phi") value = object->daughter0Phi;
  else if(variable == "daughter0Eta") value = object->daughter0Eta;
  else if(variable == "daughter1ET") value = object->daughter1ET;
  else if(variable == "daughter1PT") value = object->daughter1PT;
  else if(variable == "daughter1Phi") value = object->daughter1Phi;
  else if(variable == "daughter1Eta") value = object->daughter1Eta;
  else if(variable == "grandMotherET") value = object->grandMotherET;
  else if(variable == "grandMotherPT") value = object->grandMotherPT;
  else if(variable == "grandMotherPhi") value = object->grandMotherPhi;
  else if(variable == "grandMotherEta") value = object->grandMotherEta;
  else if(variable == "grandMother00ET") value = object->grandMother00ET;
  else if(variable == "grandMother00PT") value = object->grandMother00PT;
  else if(variable == "grandMother00Phi") value = object->grandMother00Phi;
  else if(variable == "grandMother00Eta") value = object->grandMother00Eta;
  else if(variable == "grandMother01ET") value = object->grandMother01ET;
  else if(variable == "grandMother01PT") value = object->grandMother01PT;
  else if(variable == "grandMother01Phi") value = object->grandMother01Phi;
  else if(variable == "grandMother01Eta") value = object->grandMother01Eta;
  else if(variable == "grandMother10ET") value = object->grandMother10ET;
  else if(variable == "grandMother10PT") value = object->grandMother10PT;
  else if(variable == "grandMother10Phi") value = object->grandMother10Phi;
  else if(variable == "grandMother10Eta") value = object->grandMother10Eta;
  else if(variable == "grandMother11ET") value = object->grandMother11ET;
  else if(variable == "grandMother11PT") value = object->grandMother11PT;
  else if(variable == "grandMother11Phi") value = object->grandMother11Phi;
  else if(variable == "grandMother11Eta") value = object->grandMother11Eta;
  else if(variable == "charge") value = object->charge;
  else if(variable == "id") value = object->id;
  else if(variable == "status") value = object->status;
  else if(variable == "motherId") value = object->motherId;
  else if(variable == "motherCharge") value = object->motherCharge;
  else if(variable == "mother0Id") value = object->mother0Id;
  else if(variable == "mother0Status") value = object->mother0Status;
  else if(variable == "mother0Charge") value = object->mother0Charge;
  else if(variable == "mother1Id") value = object->mother1Id;
  else if(variable == "mother1Status") value = object->mother1Status;
  else if(variable == "mother1Charge") value = object->mother1Charge;
  else if(variable == "daughter0Id") value = object->daughter0Id;
  else if(variable == "daughter0Status") value = object->daughter0Status;
  else if(variable == "daughter0Charge") value = object->daughter0Charge;
  else if(variable == "daughter1Id") value = object->daughter1Id;
  else if(variable == "daughter1Status") value = object->daughter1Status;
  else if(variable == "daughter1Charge") value = object->daughter1Charge;
  else if(variable == "grandMotherId") value = object->grandMotherId;
  else if(variable == "grandMotherCharge") value = object->grandMotherCharge;
  else if(variable == "grandMother00Id") value = object->grandMother00Id;
  else if(variable == "grandMother00Status") value = object->grandMother00Status;
  else if(variable == "grandMother00Charge") value = object->grandMother00Charge;
  else if(variable == "grandMother01Id") value = object->grandMother01Id;
  else if(variable == "grandMother01Status") value = object->grandMother01Status;
  else if(variable == "grandMother01Charge") value = object->grandMother01Charge;
  else if(variable == "grandMother10Id") value = object->grandMother10Id;
  else if(variable == "grandMother10Status") value = object->grandMother10Status;
  else if(variable == "grandMother10Charge") value = object->grandMother10Charge;
  else if(variable == "grandMother11Id") value = object->grandMother11Id;
  else if(variable == "grandMother11Status") value = object->grandMother11Status;
  else if(variable == "grandMother11Charge") value = object->grandMother11Charge;


  //user-defined variables
  else if(variable == "genMatchedPdgGrandmotherId"){
    if(fabs(object->motherId) == 15){
      int motherIndex = findTauMotherIndex(object);
      if(motherIndex == -1) value = 0;
      else value = mcparticles->at(motherIndex).motherId;
    }
    else value = object->grandMotherId;
  }





  else if(variable == "genMatchedId") value = getPdgIdBinValue(object->id);
  else if(variable == "genMatchedMotherId") value = getPdgIdBinValue(object->motherId);
  else if(variable == "genMatchedMotherIdReverse") value = 24 - getPdgIdBinValue(object->motherId);
  else if(variable == "genMatchedGrandmotherId"){
    if(fabs(object->motherId) == 15){
      int motherIndex = findTauMotherIndex(object);
      if(motherIndex == -1) value = 0;
      else value = getPdgIdBinValue(mcparticles->at(motherIndex).motherId);
    }
    else value = getPdgIdBinValue(object->grandMotherId);
  }
  else if(variable == "genMatchedGrandmotherIdReverse"){
    if(fabs(object->motherId) == 15){
      int motherIndex = findTauMotherIndex(object);
      if(motherIndex == -1) value = 0;
      else value = 24 - getPdgIdBinValue(mcparticles->at(motherIndex).motherId);
    }
    else value = 24 - getPdgIdBinValue(object->grandMotherId);
  }



  else if (variable == "d0Vertex"){
    double vx = object->vx - chosenVertex ()->x,
      vy = object->vy - chosenVertex ()->y,
      px = object->px,
      py = object->py,
      pt = object->pt;
    value = (-vx * py + vy * px) / pt;
  }
  else if (variable == "d0Beamspot"){
    double vx = object->vx - events->at(0).BSx,
      vy = object->vy - events->at(0).BSy,
      px = object->px,
      py = object->py,
      pt = object->pt;
    value = (-vx * py + vy * px) / pt;
  }
  else if (variable == "d0Origin"){
    double vx = object->vx,
      vy = object->vy,
      px = object->px,
      py = object->py,
      pt = object->pt;
    value = (-vx * py + vy * px) / pt;
  }

  else if (variable == "dz"){
    double vx = object->vx - chosenVertex ()->x,
      vy = object->vy - chosenVertex ()->y,
      vz = object->vz - chosenVertex ()->z,
      px = object->px,
      py = object->py,
      pz = object->pz,
      pt = object->pt;
    value = vz - (vx * px + vy * py)/pt * (pz/pt);
  }
  else if(variable == "v0"){
    value = sqrt(object->vx*object->vx + object->vy*object->vy);
  }
  else if(variable == "deltaV0"){
    value = sqrt((object->vx-chosenVertex ()->x)*(object->vx-chosenVertex ()->x) + (object->vy-chosenVertex ()->y)*(object->vy-chosenVertex ()->y));
  }
  else if (variable == "deltaVx"){
    value = object->vx - chosenVertex ()->x;
  }
  else if (variable == "deltaVy"){
    value = object->vy - chosenVertex ()->y;
  }
  else if (variable == "deltaVz"){
    value = object->vz - chosenVertex ()->z;
  }
  else if (variable == "rho"){
    value = sqrt (object->vx * object->vx + object->vy * object->vy);
  }

  else if(variable == "deltaPhiMaxSubLeadJet") {
    // calculate maximum deltaPhi between mcparticle and any subleading jet
    double mcpartJetDeltaPhiMax = -99.;
    if (!jets.product()) clog << "ERROR:  cannot find deltaPhiMaxSubLeadJet because jets collection is not initialized." << endl;
    for (uint ijet = 0; ijet<jets->size(); ijet++) {
      string empty = "";
      double isSubLeadingJet = valueLookup(&jets->at(ijet), "disappTrkSubLeadingJetID", "", empty);
      if (!isSubLeadingJet) continue;  // only consider jets that pass the subleading jet ID criteria
      double jetPhi = valueLookup(&jets->at(ijet), "phi", "", empty);
      double mcpartJetDeltaPhi = fabs(deltaPhi(object->phi, jetPhi));
      if (mcpartJetDeltaPhi > mcpartJetDeltaPhiMax) mcpartJetDeltaPhiMax = mcpartJetDeltaPhi;
    }
    value = mcpartJetDeltaPhiMax;
  }


  else{clog << "WARNING: invalid mcparticle variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
} // end mcparticle valueLookup
