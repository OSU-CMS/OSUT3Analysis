#include <iostream>

#include "TVector2.h"

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!muon valueLookup
double
ValueLookup::valueLookup (const BNmuon* object, string variable){

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
  else if(variable == "trackIsDR03") value = object->trackIsoDR03;
  else if(variable == "ecalIsoDR03") value = object->ecalIsoDR03;
  else if(variable == "hcalIsoDR03") value = object->hcalIsoDR03;
  else if(variable == "caloIsoDR03") value = object->caloIsoDR03;
  else if(variable == "trackVetoIsoDR03") value = object->trackVetoIsoDR03;
  else if(variable == "ecalVetoIsoDR03") value = object->ecalVetoIsoDR03;
  else if(variable == "hcalVetoIsoDR03") value = object->hcalVetoIsoDR03;
  else if(variable == "caloVetoIsoDR03") value = object->caloVetoIsoDR03;
  else if(variable == "trackIsoDR05") value = object->trackIsoDR05;
  else if(variable == "ecalIsoDR05") value = object->ecalIsoDR05;
  else if(variable == "hcalIsoDR05") value = object->hcalIsoDR05;
  else if(variable == "caloIsoDR05") value = object->caloIsoDR05;
  else if(variable == "trackVetoIsoDR05") value = object->trackVetoIsoDR05;
  else if(variable == "ecalVetoIsoDR05") value = object->ecalVetoIsoDR05;
  else if(variable == "hcalVetoIsoDR05") value = object->hcalVetoIsoDR05;
  else if(variable == "caloVetoIsoDR05") value = object->caloVetoIsoDR05;
  else if(variable == "hcalE") value = object->hcalE;
  else if(variable == "ecalE") value = object->ecalE;
  else if(variable == "genET") value = object->genET;
  else if(variable == "genPT") value = object->genPT;
  else if(variable == "genPhi") value = object->genPhi;
  else if(variable == "genEta") value = object->genEta;
  else if(variable == "genMotherET") value = object->genMotherET;
  else if(variable == "genMotherPT") value = object->genMotherPT;
  else if(variable == "genMotherPhi") value = object->genMotherPhi;
  else if(variable == "genMotherEta") value = object->genMotherEta;
  else if(variable == "vx") value = object->vx;
  else if(variable == "vy") value = object->vy;
  else if(variable == "vz") value = object->vz;
  else if(variable == "tkNormChi2") value = object->tkNormChi2;
  else if(variable == "tkPT") value = object->tkPT;
  else if(variable == "tkEta") value = object->tkEta;
  else if(variable == "tkPhi") value = object->tkPhi;
  else if(variable == "tkDZ") value = object->tkDZ;
  else if(variable == "tkD0") value = object->tkD0;
  else if(variable == "tkD0bs") value = object->tkD0bs;
  else if(variable == "tkD0err") value = object->tkD0err;
  else if(variable == "samNormChi2") value = object->samNormChi2;
  else if(variable == "samPT") value = object->samPT;
  else if(variable == "samEta") value = object->samEta;
  else if(variable == "samPhi") value = object->samPhi;
  else if(variable == "samDZ") value = object->samDZ;
  else if(variable == "samD0") value = object->samD0;
  else if(variable == "samD0bs") value = object->samD0bs;
  else if(variable == "samD0err") value = object->samD0err;
  else if(variable == "comNormChi2") value = object->comNormChi2;
  else if(variable == "comPT") value = object->comPT;
  else if(variable == "comEta") value = object->comEta;
  else if(variable == "comPhi") value = object->comPhi;
  else if(variable == "comDZ") value = object->comDZ;
  else if(variable == "comD0") value = object->comD0;
  else if(variable == "comD0bs") value = object->comD0bs;
  else if(variable == "comD0err") value = object->comD0err;
  else if(variable == "isolationR03emVetoEt") value = object->isolationR03emVetoEt;
  else if(variable == "isolationR03hadVetoEt") value = object->isolationR03hadVetoEt;
  else if(variable == "normalizedChi2") value = object->normalizedChi2;
  else if(variable == "dVzPVz") value = object->dVzPVz;
  else if(variable == "dB") value = object->dB;
  else if(variable == "ptErr") value = object->ptErr;
  else if(variable == "innerTrackNormChi2") value = object->innerTrackNormChi2;
  else if(variable == "correctedD0") value = object->correctedD0;
  else if(variable == "correctedD0Vertex") value = object->correctedD0Vertex;
  else if(variable == "correctedDZ") value = object->correctedDZ;
  else if(variable == "particleIso") value = object->particleIso;
  else if(variable == "chargedHadronIso") value = object->chargedHadronIso;
  else if(variable == "neutralHadronIso") value = object->neutralHadronIso;
  else if(variable == "photonIso") value = object->photonIso;
  else if(variable == "puChargedHadronIso") value = object->puChargedHadronIso;
  else if(variable == "chargedHadronIsoDR03") value = object->chargedHadronIsoDR03;
  else if(variable == "neutralHadronIsoDR03") value = object->neutralHadronIsoDR03;
  else if(variable == "photonIsoDR03") value = object->photonIsoDR03;
  else if(variable == "puChargedHadronIsoDR03") value = object->puChargedHadronIsoDR03;
  else if(variable == "chargedHadronIsoDR04") value = object->chargedHadronIsoDR04;
  else if(variable == "neutralHadronIsoDR04") value = object->neutralHadronIsoDR04;
  else if(variable == "photonIsoDR04") value = object->photonIsoDR04;
  else if(variable == "puChargedHadronIsoDR04") value = object->puChargedHadronIsoDR04;
  else if(variable == "rhoPrime") value = object->rhoPrime;
  else if(variable == "AEffDr03") value = object->AEffDr03;
  else if(variable == "AEffDr04") value = object->AEffDr04;
  else if(variable == "pfIsoR03SumChargedHadronPt") value = object->pfIsoR03SumChargedHadronPt;
  else if(variable == "pfIsoR03SumNeutralHadronEt") value = object->pfIsoR03SumNeutralHadronEt;
  else if(variable == "pfIsoR03SumPhotonEt") value = object->pfIsoR03SumPhotonEt;
  else if(variable == "pfIsoR03SumPUPt") value = object->pfIsoR03SumPUPt;
  else if(variable == "relpfIsoR04SumExceptChargedHad") value = (object->pfIsoR04SumNeutralHadronEt + object->pfIsoR04SumPhotonEt - 0.5*object->pfIsoR04SumPUPt)/ object->pt;
  else if(variable == "relpfIsoR04SumChargedHadronPt") value = object->pfIsoR04SumChargedHadronPt/object->pt;
  else if(variable == "relpfIsoR04SumNeutralHadronEt") value = object->pfIsoR04SumNeutralHadronEt/object->pt;
  else if(variable == "relpfIsoR04SumPhotonEt") value = object->pfIsoR04SumPhotonEt/object->pt;
  else if(variable == "relpfIsoR04SumPUPt") value = object->pfIsoR04SumPUPt/object->pt;
  else if(variable == "pfIsoR04SumChargedHadronPt") value = object->pfIsoR04SumChargedHadronPt;
  else if(variable == "pfIsoR04SumNeutralHadronEt") value = object->pfIsoR04SumNeutralHadronEt;
  else if(variable == "pfIsoR04SumPhotonEt") value = object->pfIsoR04SumPhotonEt;
  else if(variable == "pfIsoR04SumPUPt") value = object->pfIsoR04SumPUPt;
  else if(variable == "IP") value = object->IP;
  else if(variable == "IPError") value = object->IPError;
  else if(variable == "timeAtIpInOut") value = object->timeAtIpInOut;
  else if(variable == "timeAtIpInOutErr") value = object->timeAtIpInOutErr;
  else if(variable == "timeAtIpOutIn") value = object->timeAtIpOutIn;
  else if(variable == "timeAtIpOutInErr") value = object->timeAtIpOutInErr;
  else if(variable == "ecal_time") value = object->ecal_time;
  else if(variable == "hcal_time") value = object->hcal_time;
  else if(variable == "time_between_ecal_hcal") value = object->hcal_time - object->ecal_time;
  else if(variable == "ecal_timeError") value = object->ecal_timeError;
  else if(variable == "hcal_timeError") value = object->hcal_timeError;
  else if(variable == "energy_ecal") value = object->energy_ecal;
  else if(variable == "energy_hcal") value = object->energy_hcal;
  else if(variable == "e3x3_ecal") value = object->e3x3_ecal;
  else if(variable == "e3x3_hcal") value = object->e3x3_hcal;
  else if(variable == "energyMax_ecal") value = object->energyMax_ecal;
  else if(variable == "energyMax_hcal") value = object->energyMax_hcal;
  else if(variable == "charge") value = object->charge;
  else if(variable == "IDGMPTight") value = object->IDGMPTight;
  else if(variable == "tkNumValidHits") value = object->tkNumValidHits;
  else if(variable == "tkCharge") value = object->tkCharge;
  else if(variable == "samNumValidHits") value = object->samNumValidHits;
  else if(variable == "samCharge") value = object->samCharge;
  else if(variable == "comNumValidHits") value = object->comNumValidHits;
  else if(variable == "comCharge") value = object->comCharge;
  else if(variable == "genId") value = object->genId;
  else if(variable == "genCharge") value = object->genCharge;
  else if(variable == "genNumberOfMothers") value = object->genNumberOfMothers;
  else if(variable == "genMotherId") value = object->genMotherId;
  else if(variable == "genMotherCharge") value = object->genMotherCharge;
  else if(variable == "genMother0Id") value = object->genMother0Id;
  else if(variable == "genMother1Id") value = object->genMother1Id;
  else if(variable == "genGrandMother00Id") value = object->genGrandMother00Id;
  else if(variable == "genGrandMother01Id") value = object->genGrandMother01Id;
  else if(variable == "genGrandMother10Id") value = object->genGrandMother10Id;
  else if(variable == "genGrandMother11Id") value = object->genGrandMother11Id;
  else if(variable == "isPFMuon") value = object->isPFMuon;
  else if(variable == "isGoodMuon_1StationTight") value = object->isGoodMuon_1StationTight;
  else if(variable == "isGlobalMuon") value = object->isGlobalMuon;
  else if(variable == "isTrackerMuon") value = object->isTrackerMuon;
  else if(variable == "isStandAloneMuon") value = object->isStandAloneMuon;
  else if(variable == "isGlobalMuonPromptTight") value = object->isGlobalMuonPromptTight;
  else if(variable == "numberOfValidMuonHits") value = object->numberOfValidMuonHits;
  else if(variable == "numberOfValidTrackerHits") value = object->numberOfValidTrackerHits;
  else if(variable == "numberOfLayersWithMeasurement") value = object->numberOfLayersWithMeasurement;
  else if(variable == "pixelLayersWithMeasurement") value = object->pixelLayersWithMeasurement;
  else if(variable == "numberOfMatches") value = object->numberOfMatches;
  else if(variable == "numberOfValidTrackerHitsInnerTrack") value = object->numberOfValidTrackerHitsInnerTrack;
  else if(variable == "numberOfValidPixelHits") value = object->numberOfValidPixelHits;
  else if(variable == "numberOfMatchedStations") value = object->numberOfMatchedStations;
  else if(variable == "time_ndof") value = object->time_ndof;

  //user-defined variables
  else if(variable == "looseID") {
    value = object->pt > 10 &&
      (object->isGlobalMuon  > 0 ||
       object->isTrackerMuon > 0);
  }
  else if(variable == "looseIDGlobalMuon") {
    value = object->pt > 10 &&
      object->isGlobalMuon  > 0;
  }
  else if(variable == "vertexDistZ") value = object->vz - chosenVertex()->z;
  else if(variable == "correctedD0VertexErr") value =  hypot (object->tkD0err, hypot (chosenVertex ()->xError, chosenVertex ()->yError));
  else if(variable == "correctedD0VertexSig") value =  object->correctedD0Vertex / hypot (object->tkD0err, hypot (chosenVertex ()->xError, chosenVertex ()->yError));
  else if(variable == "correctedD0Err") value =  hypot (object->tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
  else if(variable == "correctedD0Sig") value =  object->correctedD0 / hypot (object->tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
  else if(variable == "detIso") value = (object->trackIsoDR03) / object->pt;
  else if(variable == "relPFdBetaIso") value = (object->pfIsoR04SumChargedHadronPt + max(0.0, object->pfIsoR04SumNeutralHadronEt + object->pfIsoR04SumPhotonEt - 0.5*object->pfIsoR04SumPUPt)) / object->pt;
  else if(variable == "PUInrelPFdBetaIso") value = (object->pfIsoR04SumPUPt)/ object->pt;
  else if(variable == "ChargedHadInrelPFdBetaIso") value = (object->pfIsoR04SumChargedHadronPt)/ object->pt;
  else if(variable == "NeutralHadInrelPFdBetaIso") value = (object->pfIsoR04SumNeutralHadronEt)/ object->pt;
  else if(variable == "PhotonInrelPFdBetaIso") value = (object->pfIsoR04SumPhotonEt)/ object->pt;
  else if(variable == "relPFdBetaIsoNoPUSubtracted") value = (object->pfIsoR04SumChargedHadronPt + max(0.0, object->pfIsoR04SumNeutralHadronEt + object->pfIsoR04SumPhotonEt)) / object->pt;
  else if(variable == "relPFdBetaIsoNew") value = (object->chargedHadronIsoDR04 + max(0.0, object->neutralHadronIsoDR04 + object->photonIsoDR04 - 0.5*object->puChargedHadronIsoDR04)) / object->pt;
  else if(variable == "relPFdBetaIsoPseudo") value = (object->pfIsoR04SumChargedHadronPt + object->pfIsoR04SumNeutralHadronEt + object->pfIsoR04SumPhotonEt - 0.5*object->pfIsoR04SumPUPt) / object->pt;
  else if(variable == "relPFrhoIso") value = ( object->chargedHadronIso + max(0.0, object->neutralHadronIso + object->photonIso - object->AEffDr03*object->rhoPrime) ) / object->pt;
  else if(variable == "metMT") {
    if (const BNmet *met = chosenMET ())
      {
        double dPhi = deltaPhi (object->phi, met->phi);
        value = sqrt (2 * object->pt * met->pt * (1 - cos (dPhi)));
      }
    else
      value = numeric_limits<unsigned>::max ();
  }
  else if(variable == "metDeltaPhi") {
    if (const BNmet *met = chosenMET ())
      value = deltaPhi (object->phi, met->phi);
    else
      value = numeric_limits<unsigned>::max ();
  }
  else if(variable == "ptPlusMet") {
    // Calculate the magnitude of the vector sum of the muon pT and the Met.
    if (const BNmet *met = chosenMET ())
      {
        TVector2 p2Met;
        TVector2 p2Muon;
        p2Met. SetMagPhi(   met->pt,    met->phi);
        p2Muon.SetMagPhi(object->pt, object->phi);
        TVector2 p2MetMuon = p2Met + p2Muon;
        value = p2MetMuon.Mod();
      }
    else
      value = numeric_limits<unsigned>::max ();
  }

  else if(variable == "correctedD0VertexInEBPlus"){
    if(fabs(object->eta) < 0.8 && object->eta > 0) value = object->correctedD0Vertex;
    else value = numeric_limits<unsigned>::max ();
  }
  else if(variable == "correctedD0VertexOutEBPlus"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->eta > 0) value = object->correctedD0Vertex;
    else value = numeric_limits<unsigned>::max ();
  }
  else if(variable == "correctedD0VertexEEPlus"){
    if(fabs(object->eta) > 1.479 && object->eta > 0) value = object->correctedD0Vertex;
    else value = numeric_limits<unsigned>::max ();
  }

  else if(variable == "correctedD0BeamspotInEBPlus"){
    if(fabs(object->eta) < 0.8 && object->eta > 0) value = object->correctedD0;
    else value = numeric_limits<unsigned>::max ();
  }
  else if(variable == "correctedD0BeamspotOutEBPlus"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->eta > 0) value = object->correctedD0;
    else value = numeric_limits<unsigned>::max ();
  }
  else if(variable == "correctedD0BeamspotEEPlus"){
    if(fabs(object->eta) > 1.479 && object->eta > 0) value = object->correctedD0;
    else value = numeric_limits<unsigned>::max ();
  }

  else if(variable == "correctedD0VertexInEBMinus"){
    if(fabs(object->eta) < 0.8 && object->eta < 0) value = object->correctedD0Vertex;
    else value = numeric_limits<unsigned>::max ();
  }
  else if(variable == "correctedD0VertexOutEBMinus"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->eta < 0) value = object->correctedD0Vertex;
    else value = numeric_limits<unsigned>::max ();
  }
  else if(variable == "correctedD0VertexEEMinus"){
    if(fabs(object->eta) > 1.479 && object->eta < 0) value = object->correctedD0Vertex;
    else value = numeric_limits<unsigned>::max ();
  }

  else if(variable == "correctedD0BeamspotInEBMinus"){
    if(fabs(object->eta) < 0.8 && object->eta < 0) value = object->correctedD0;
    else value = numeric_limits<unsigned>::max ();
  }
  else if(variable == "correctedD0BeamspotOutEBMinus"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->eta < 0) value = object->correctedD0;
    else value = numeric_limits<unsigned>::max ();
  }
  else if(variable == "correctedD0BeamspotEEMinus"){
    if(fabs(object->eta) > 1.479 && object->eta < 0) value = object->correctedD0;
    else value = numeric_limits<unsigned>::max ();
  }


  else if(variable == "correctedD0VertexInEBPositiveCharge"){
    if(fabs(object->eta) < 0.8 && object->charge > 0) value = object->correctedD0Vertex;
    else value = numeric_limits<unsigned>::max ();
  }
  else if(variable == "correctedD0VertexOutEBPositiveCharge"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->charge > 0) value = object->correctedD0Vertex;
    else value = numeric_limits<unsigned>::max ();
  }
  else if(variable == "correctedD0VertexEEPositiveCharge"){
    if(fabs(object->eta) > 1.479 && object->charge > 0) value = object->correctedD0Vertex;
    else value = numeric_limits<unsigned>::max ();
  }

  else if(variable == "correctedD0BeamspotInEBPositiveCharge"){
    if(fabs(object->eta) < 0.8 && object->charge > 0) value = object->correctedD0;
    else value = numeric_limits<unsigned>::max ();
  }
  else if(variable == "correctedD0BeamspotOutEBPositiveCharge"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->charge > 0) value = object->correctedD0;
    else value = numeric_limits<unsigned>::max ();
  }
  else if(variable == "correctedD0BeamspotEEPositiveCharge"){
    if(fabs(object->eta) > 1.479 && object->charge > 0) value = object->correctedD0;
    else value = numeric_limits<unsigned>::max ();
  }

  else if(variable == "correctedD0VertexInEBNegativeCharge"){
    if(fabs(object->eta) < 0.8 && object->charge < 0) value = object->correctedD0Vertex;
    else value = numeric_limits<unsigned>::max ();
  }
  else if(variable == "correctedD0VertexOutEBNegativeCharge"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->charge < 0) value = object->correctedD0Vertex;
    else value = numeric_limits<unsigned>::max ();
  }
  else if(variable == "correctedD0VertexEENegativeCharge"){
    if(fabs(object->eta) > 1.479 && object->charge < 0) value = object->correctedD0Vertex;
    else value = numeric_limits<unsigned>::max ();
  }

  else if(variable == "correctedD0BeamspotInEBNegativeCharge"){
    if(fabs(object->eta) < 0.8 && object->charge < 0) value = object->correctedD0;
    else value = numeric_limits<unsigned>::max ();
  }
  else if(variable == "correctedD0BeamspotOutEBNegativeCharge"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->charge < 0) value = object->correctedD0;
    else value = numeric_limits<unsigned>::max ();
  }
  else if(variable == "correctedD0BeamspotEENegativeCharge"){
    if(fabs(object->eta) > 1.479 && object->charge < 0) value = object->correctedD0;
    else value = numeric_limits<unsigned>::max ();
  }


  else if(variable == "tightID") {
    // Defined in https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId#Tight_Muon
    value = object->isGlobalMuon > 0                \
      && object->isPFMuon > 0                        \
      && object->normalizedChi2 < 10                \
      && object->numberOfValidMuonHits > 0        \
      && object->numberOfMatchedStations > 1        \
      && fabs(object->correctedD0Vertex) < 0.2        \
      && fabs(object->correctedDZ) < 0.5        \
      && object->numberOfValidPixelHits > 0                \
      && object->numberOfLayersWithMeasurement > 5;
  }
  else if(variable == "tightIDdisplaced"){
    value = object->isGlobalMuon > 0                \
      && object->isPFMuon > 0                        \
      && object->normalizedChi2 < 10                \
      && object->numberOfValidMuonHits > 0        \
      && object->numberOfMatchedStations > 1        \
      && object->numberOfValidPixelHits > 0        \
      && object->numberOfLayersWithMeasurement > 5;
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
    else value = 24 - getPdgIdBinValue(mcparticles->at(index).motherId);
  }
  else if(variable == "genRecoChargeProduct"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 2;
    else value = object->charge * mcparticles->at(index).charge;
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
  else if(variable == "pfMuonsFromVertex"){
    double d0Error, dzError;

    d0Error = hypot (object->tkD0err, hypot (chosenVertex ()->xError, chosenVertex ()->yError));
    dzError = hypot (object->tkDZerr, chosenVertex ()->zError);
    value = fabs (object->correctedD0Vertex) > 0.2 || fabs (object->correctedDZ) > 0.5
      || fabs (object->correctedD0Vertex / d0Error) > 99.0
      || fabs (object->correctedDZ / dzError) > 99.0;
    value = (object->isStandAloneMuon && !object->isTrackerMuon && !object->isGlobalMuon) || !value;
  }



  else{clog << "WARNING: invalid muon variable '" << variable << "'\n"; value = numeric_limits<unsigned>::max ();}

  return value;
} // end muon valueLookup
