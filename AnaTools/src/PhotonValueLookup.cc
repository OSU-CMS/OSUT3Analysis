#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!photon valueLookup
double
ValueLookup::valueLookup (const BNphoton* object, string variable){

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
  else if(variable == "trackIso") value = object->trackIso;
  else if(variable == "ecalIso") value = object->ecalIso;
  else if(variable == "hcalIso") value = object->hcalIso;
  else if(variable == "caloIso") value = object->caloIso;
  else if(variable == "trackIsoHollowConeDR03") value = object->trackIsoHollowConeDR03;
  else if(variable == "trackIsoSolidConeDR03") value = object->trackIsoSolidConeDR03;
  else if(variable == "ecalIsoDR03") value = object->ecalIsoDR03;
  else if(variable == "hcalIsoDR03") value = object->hcalIsoDR03;
  else if(variable == "caloIsoDR03") value = object->caloIsoDR03;
  else if(variable == "trackIsoHollowConeDR04") value = object->trackIsoHollowConeDR04;
  else if(variable == "trackIsoSolidConeDR04") value = object->trackIsoSolidConeDR04;
  else if(variable == "ecalIsoDR04") value = object->ecalIsoDR04;
  else if(variable == "hcalIsoDR04") value = object->hcalIsoDR04;
  else if(variable == "caloIsoDR04") value = object->caloIsoDR04;
  else if(variable == "hadOverEm") value = object->hadOverEm;
  else if(variable == "sigmaEtaEta") value = object->sigmaEtaEta;
  else if(variable == "sigmaIetaIeta") value = object->sigmaIetaIeta;
  else if(variable == "r9") value = object->r9;
  else if(variable == "scEnergy") value = object->scEnergy;
  else if(variable == "scRawEnergy") value = object->scRawEnergy;
  else if(variable == "scSeedEnergy") value = object->scSeedEnergy;
  else if(variable == "scEta") value = object->scEta;
  else if(variable == "scPhi") value = object->scPhi;
  else if(variable == "scZ") value = object->scZ;
  else if(variable == "genET") value = object->genET;
  else if(variable == "genPT") value = object->genPT;
  else if(variable == "genPhi") value = object->genPhi;
  else if(variable == "genEta") value = object->genEta;
  else if(variable == "genMotherET") value = object->genMotherET;
  else if(variable == "genMotherPT") value = object->genMotherPT;
  else if(variable == "genMotherPhi") value = object->genMotherPhi;
  else if(variable == "genMotherEta") value = object->genMotherEta;
  else if(variable == "eMax") value = object->eMax;
  else if(variable == "eLeft") value = object->eLeft;
  else if(variable == "eRight") value = object->eRight;
  else if(variable == "eTop") value = object->eTop;
  else if(variable == "eBottom") value = object->eBottom;
  else if(variable == "e3x3") value = object->e3x3;
  else if(variable == "swissCross") value = object->swissCross;
  else if(variable == "seedEnergy") value = object->seedEnergy;
  else if(variable == "seedTime") value = object->seedTime;
  else if(variable == "swissCrossNoI85") value = object->swissCrossNoI85;
  else if(variable == "swissCrossI85") value = object->swissCrossI85;
  else if(variable == "E2overE9NoI85") value = object->E2overE9NoI85;
  else if(variable == "E2overE9I85") value = object->E2overE9I85;
  else if(variable == "IDTight") value = object->IDTight;
  else if(variable == "IDLoose") value = object->IDLoose;
  else if(variable == "IDLooseEM") value = object->IDLooseEM;
  else if(variable == "genId") value = object->genId;
  else if(variable == "genCharge") value = object->genCharge;
  else if(variable == "genMotherId") value = object->genMotherId;
  else if(variable == "genMotherCharge") value = object->genMotherCharge;
  else if(variable == "isEB") value = object->isEB;
  else if(variable == "isEE") value = object->isEE;
  else if(variable == "isGap") value = object->isGap;
  else if(variable == "isEBEEGap") value = object->isEBEEGap;
  else if(variable == "isEBGap") value = object->isEBGap;
  else if(variable == "isEEGap") value = object->isEEGap;
  else if(variable == "hasPixelSeed") value = object->hasPixelSeed;
  else if(variable == "seedRecoFlag") value = object->seedRecoFlag;




  else if(variable == "genDeltaRLowest") value = getGenDeltaRLowest(object);

  else if(variable == "genMatchedPdgId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else value = mcparticles->at(index).id;
  }



  else if(variable == "genMatchedId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else value = getPdgIdBinValue(mcparticles->at(index).id);
  }
  else if(variable == "genMatchedMotherId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else value = getPdgIdBinValue(mcparticles->at(index).motherId);
  }
  else if(variable == "genMatchedMotherIdReverse"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 24;
    else value = 24 -getPdgIdBinValue(mcparticles->at(index).motherId);
  }
  else if(variable == "genMatchedGrandmotherId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else if(fabs(mcparticles->at(index).motherId) == 15){
      int motherIndex = findTauMotherIndex(&mcparticles->at(index));
      if(motherIndex == -1) value = 0;
      else value = getPdgIdBinValue(mcparticles->at(motherIndex).motherId);
    }
    else value = getPdgIdBinValue(mcparticles->at(index).grandMotherId);
  }
  else if(variable == "genMatchedGrandmotherIdReverse"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 24;
    else if(fabs(mcparticles->at(index).motherId) == 15){
      int motherIndex = findTauMotherIndex(&mcparticles->at(index));
      if(motherIndex == -1) value = 24;
      else value = 24 - getPdgIdBinValue(mcparticles->at(motherIndex).motherId);
    }
    else value = 24 - getPdgIdBinValue(mcparticles->at(index).grandMotherId);
  }


  else{clog << "WARNING: invalid photon variable '" << variable << "'\n"; value = -999;}

  return value;
} // end photon valueLookup
