#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!tau valueLookup
double
ValueLookup::valueLookup (const BNtau* object, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "px") value = object->px;
  else if(variable == "py") value = object->py;
  else if(variable == "pz") value = object->pz;
  else if(variable == "energy") value = object->energy;
  else if(variable == "et") value = object->et;
  else if(variable == "pt") value = object->pt;
  else if(variable == "eta") value = object->eta;
  else if(variable == "phi") value = object->phi;
  else if(variable == "emFraction") value = object->emFraction;
  else if(variable == "leadingTrackPt") value = object->leadingTrackPt;
  else if(variable == "leadingTrackIpVtdxy") value = object->leadingTrackIpVtdxy;
  else if(variable == "leadingTrackIpVtdz") value = object->leadingTrackIpVtdz;
  else if(variable == "leadingTrackIpVtdxyError") value = object->leadingTrackIpVtdxyError;
  else if(variable == "leadingTrackIpVtdzError") value = object->leadingTrackIpVtdzError;
  else if(variable == "leadingTrackVx") value = object->leadingTrackVx;
  else if(variable == "leadingTrackVy") value = object->leadingTrackVy;
  else if(variable == "leadingTrackVz") value = object->leadingTrackVz;
  else if(variable == "leadingTrackValidHits") value = object->leadingTrackValidHits;
  else if(variable == "leadingTrackNormChiSqrd") value = object->leadingTrackNormChiSqrd;
  else if(variable == "numProngs") value = object->numProngs;
  else if(variable == "numSignalGammas") value = object->numSignalGammas;
  else if(variable == "numSignalNeutrals") value = object->numSignalNeutrals;
  else if(variable == "numSignalPiZeros") value = object->numSignalPiZeros;
  else if(variable == "decayMode") value = object->decayMode;
  else if(variable == "charge") value = object->charge;
  else if(variable == "inTheCracks") value = object->inTheCracks;
  else if(variable == "HPSagainstElectronLoose") value = object->HPSagainstElectronLoose;
  else if(variable == "HPSagainstElectronMVA") value = object->HPSagainstElectronMVA;
  else if(variable == "HPSagainstElectronMedium") value = object->HPSagainstElectronMedium;
  else if(variable == "HPSagainstElectronTight") value = object->HPSagainstElectronTight;
  else if(variable == "HPSagainstMuonLoose") value = object->HPSagainstMuonLoose;
  else if(variable == "HPSagainstMuonMedium") value = object->HPSagainstMuonMedium;
  else if(variable == "HPSagainstMuonTight") value = object->HPSagainstMuonTight;
  else if(variable == "HPSbyLooseCombinedIsolationDeltaBetaCorr") value = object->HPSbyLooseCombinedIsolationDeltaBetaCorr;
  else if(variable == "HPSbyMediumCombinedIsolationDeltaBetaCorr") value = object->HPSbyMediumCombinedIsolationDeltaBetaCorr;
  else if(variable == "HPSbyTightCombinedIsolationDeltaBetaCorr") value = object->HPSbyTightCombinedIsolationDeltaBetaCorr;
  else if(variable == "HPSbyVLooseCombinedIsolationDeltaBetaCorr") value = object->HPSbyVLooseCombinedIsolationDeltaBetaCorr;
  else if(variable == "HPSdecayModeFinding") value = object->HPSdecayModeFinding;
  else if(variable == "leadingTrackValid") value = object->leadingTrackValid;

  else if (variable == "looseHadronicID") {
    //criteria taken from http://cms.cern.ch/iCMS/jsp/db_notes/showNoteDetails.jsp?noteID=CMS%20AN-2011/019
    value = object->pt > 30
      && fabs(object->eta) < 2.3
      && object->HPSbyLooseCombinedIsolationDeltaBetaCorr > 0
      && object->HPSdecayModeFinding > 0
      && object->HPSagainstElectronLoose > 0
      && object->HPSagainstMuonTight > 0;
  }

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


  else{clog << "WARNING: invalid tau variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
} // end tau valueLookup
