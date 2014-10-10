#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!electron valueLookup
double
ValueLookup::valueLookup (const BNelectron* object, string variable, string function, string &stringValue){

  double value = 0.0;
  double pMag = sqrt(object->pt * object->pt +
                     object->pz * object->pz);
  if(variable == "energy") value = object->energy;
  else if(variable == "et") value = object->et;
  else if(variable == "gsfEt") value = object->gsfEt;
  else if(variable == "pt") value = object->pt;
  else if(variable == "px") value = object->px;
  else if(variable == "py") value = object->py;
  else if(variable == "pz") value = object->pz;
  else if(variable == "phi") value = object->phi;
  else if(variable == "eta") value = object->eta;
  else if(variable == "theta") value = object->theta;
  else if(variable == "pIn") value = object->pIn;
  else if(variable == "pOut") value = object->pOut;
  else if(variable == "EscOverPin") value = object->EscOverPin;
  else if(variable == "EseedOverPout") value = object->EseedOverPout;
  else if(variable == "hadOverEm") value = object->hadOverEm;
  else if(variable == "trackIso") value = object->trackIso;
  else if(variable == "ecalIso") value = object->ecalIso;
  else if(variable == "hcalIso") value = object->hcalIso;
  else if(variable == "caloIso") value = object->caloIso;
  else if(variable == "trackIsoDR03") value = object->trackIsoDR03;
  else if(variable == "ecalIsoDR03") value = object->ecalIsoDR03;
  else if(variable == "hcalIsoDR03") value = object->hcalIsoDR03;
  else if(variable == "hcalIsoDR03depth1") value = object->hcalIsoDR03depth1;
  else if(variable == "hcalIsoDR03depth2") value = object->hcalIsoDR03depth2;
  else if(variable == "caloIsoDR03") value = object->caloIsoDR03;
  else if(variable == "trackIsoDR04") value = object->trackIsoDR04;
  else if(variable == "ecalIsoDR04") value = object->ecalIsoDR04;
  else if(variable == "hcalIsoDR04") value = object->hcalIsoDR04;
  else if(variable == "hcalIsoDR04depth1") value = object->hcalIsoDR04depth1;
  else if(variable == "hcalIsoDR04depth2") value = object->hcalIsoDR04depth2;
  else if(variable == "caloIsoDR04") value = object->caloIsoDR04;
  else if(variable == "fbrem") value = object->fbrem;
  else if(variable == "absInvEMinusInvPin") value = object->absInvEMinusInvPin;
  else if(variable == "delPhiIn") value = object->delPhiIn;
  else if(variable == "delEtaIn") value = object->delEtaIn;
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
  else if(variable == "scEnergy") value = object->scEnergy;
  else if(variable == "scRawEnergy") value = object->scRawEnergy;
  else if(variable == "scEnergyRes") value = (object->scEnergy-object->scRawEnergy)/object->scRawEnergy;
  else if(variable == "scRawOverCorrectedEnergy") value = (object->scRawEnergy)/object->scEnergy;
  else if(variable == "scSigmaEtaEta") value = object->scSigmaEtaEta;
  else if(variable == "scSigmaIEtaIEta") value = object->scSigmaIEtaIEta;
  else if(variable == "scE1x5") value = object->scE1x5;
  else if(variable == "scE2x5Max") value = object->scE2x5Max;
  else if(variable == "scE5x5") value = object->scE5x5;
  else if(variable == "scEt") value = object->scEt;
  else if(variable == "scEta") value = object->scEta;
  else if(variable == "scPhi") value = object->scPhi;
  else if(variable == "scZ") value = object->scZ;
  else if(variable == "tkNormChi2") value = object->tkNormChi2;
  else if(variable == "tkPT") value = object->tkPT;
  else if(variable == "tkEta") value = object->tkEta;
  else if(variable == "tkPhi") value = object->tkPhi;
  else if(variable == "tkDZ") value = object->tkDZ;
  else if(variable == "tkD0") value = object->tkD0;
  else if(variable == "tkD0bs") value = object->tkD0bs;
  else if(variable == "tkD0err") value = object->tkD0err;
  else if(variable == "mva") value = object->mva;
  else if(variable == "mvaTrigV0") value = object->mvaTrigV0;
  else if(variable == "mvaTrigNoIPV0") value = object->mvaTrigNoIPV0;
  else if(variable == "mvaNonTrigV0") value = object->mvaNonTrigV0;
  else if(variable == "dist") value = object->dist;
  else if(variable == "dcot") value = object->dcot;
  else if(variable == "convradius") value = object->convradius;
  else if(variable == "convPointX") value = object->convPointX;
  else if(variable == "convPointY") value = object->convPointY;
  else if(variable == "convPointZ") value = object->convPointZ;
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
  else if(variable == "IP") value = object->IP;
  else if(variable == "IPError") value = object->IPError;
  else if(variable == "charge") value = object->charge;
  else if(variable == "classification") value = object->classification;
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
  else if(variable == "numClusters") value = object->numClusters;
  else if(variable == "tkNumValidHits") value = object->tkNumValidHits;
  else if(variable == "tkCharge") value = object->tkCharge;
  else if(variable == "gsfCharge") value = object->gsfCharge;
  else if(variable == "isEB") value = object->isEB;
  else if(variable == "isEE") value = object->isEE;
  else if(variable == "isGap") value = object->isGap;
  else if(variable == "isEBEEGap") value = object->isEBEEGap;
  else if(variable == "isEBGap") value = object->isEBGap;
  else if(variable == "isEEGap") value = object->isEEGap;
  else if(variable == "isEcalDriven") value = object->isEcalDriven;
  else if(variable == "isTrackerDriven") value = object->isTrackerDriven;
  else if(variable == "numberOfLostHits") value = object->numberOfLostHits;
  else if(variable == "numberOfExpectedInnerHits") value = object->numberOfExpectedInnerHits;
  else if(variable == "numberOfValidPixelHits") value = object->numberOfValidPixelHits;
  else if(variable == "numberOfValidPixelBarrelHits") value = object->numberOfValidPixelBarrelHits;
  else if(variable == "numberOfValidPixelEndcapHits") value = object->numberOfValidPixelEndcapHits;
  else if(variable == "isHEEP") value = object->isHEEP;
  else if(variable == "isHEEPnoEt") value = object->isHEEPnoEt;
  else if(variable == "seedRecoFlag") value = object->seedRecoFlag;
  else if(variable == "eidRobustHighEnergy") value = object->eidRobustHighEnergy;
  else if(variable == "eidRobustLoose") value = object->eidRobustLoose;
  else if(variable == "eidRobustTight") value = object->eidRobustTight;
  else if(variable == "eidLoose") value = object->eidLoose;
  else if(variable == "eidTight") value = object->eidTight;
  else if(variable == "eidVeryLooseMC") value = object->eidVeryLooseMC;
  else if(variable == "eidLooseMC") value = object->eidLooseMC;
  else if(variable == "eidMediumMC") value = object->eidMediumMC;
  else if(variable == "eidTightMC") value = object->eidTightMC;
  else if(variable == "eidSuperTightMC") value = object->eidSuperTightMC;
  else if(variable == "eidHyperTight1MC") value = object->eidHyperTight1MC;
  else if(variable == "eidHyperTight2MC") value = object->eidHyperTight2MC;
  else if(variable == "eidHyperTight3MC") value = object->eidHyperTight3MC;
  else if(variable == "eidHyperTight4MC") value = object->eidHyperTight4MC;
  else if(variable == "passConvVeto") value = object->passConvVeto;

  //user-defined variables
  else if(variable == "EOverP") value = (object->energy)/pMag;
  else if(variable == "vertexDistZ") value = object->vz - chosenVertex()->z;
  else if(variable == "correctedD0VertexErr") value =  hypot (object->tkD0err, hypot (chosenVertex ()->xError, chosenVertex ()->yError));
  else if(variable == "correctedD0VertexSig") value =  object->correctedD0Vertex / hypot (object->tkD0err, hypot (chosenVertex ()->xError, chosenVertex ()->yError));
  else if(variable == "correctedD0Err") value =  hypot (object->tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
  else if(variable == "correctedD0Sig") value =  object->correctedD0 / hypot (object->tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
  else if(variable == "detIso") value = (object->trackIso) / object->pt;
  else if(variable == "relPFrhoIso") value = ( object->chargedHadronIsoDR03 + max(0.0, object->neutralHadronIsoDR03 + object->photonIsoDR03 - object->AEffDr03*object->rhoPrime) ) / object->pt;
  else if(variable == "relPFrhoIsoNoPUSubtracted") value = ( object->chargedHadronIsoDR03 + max(0.0, object->neutralHadronIsoDR03 + object->photonIsoDR03) ) / object->pt;
  else if(variable == "PUInrelPFrhoIso") value = (object->AEffDr03*object->rhoPrime)/ object->pt;
  else if(variable == "ChargedHadInrelPFrhoIso") value = (object->chargedHadronIsoDR03)/ object->pt;
  else if(variable == "NeutralHadInrelPFrhoIso") value = (object->neutralHadronIsoDR03)/ object->pt;
  else if(variable == "PhotonInrelPFrhoIso") value = (object->photonIsoDR03)/ object->pt;
  else if(variable == "relPFrhoIsoEB") value = object->isEB ? ( object->chargedHadronIsoDR03 + max(0.0, object->neutralHadronIsoDR03 + object->photonIsoDR03 - object->AEffDr03*object->rhoPrime) ) / object->pt : -999;
  else if(variable == "relPFrhoIsoEE") value = object->isEE ? ( object->chargedHadronIsoDR03 + max(0.0, object->neutralHadronIsoDR03 + object->photonIsoDR03 - object->AEffDr03*object->rhoPrime) ) / object->pt : -999;
  else if(variable == "metMT") {
    if (const BNmet *met = chosenMET ())
      {
        double dPhi = deltaPhi (object->phi, met->phi);
        value = sqrt (2 * object->pt * met->pt * (1 - cos (dPhi)));
      }
    else
      value = -999;
  }
  else if(variable == "metDeltaPhi") {
    if (const BNmet *met = chosenMET ())
      value = deltaPhi (object->phi, met->phi);
    else
      value = -999;
  }
  else if(variable == "ptPlusMet") {
    // Calculate the magnitude of the vector sum of the electron pT and the Met.
    if (const BNmet *met = chosenMET ())
      {
        TVector2 p2Met;
        TVector2 p2Elec;
        p2Met. SetMagPhi(   met->pt,    met->phi);
        p2Elec.SetMagPhi(object->pt, object->phi);
        TVector2 p2MetElec = p2Met + p2Elec;
        value = p2MetElec.Mod();
      }
    else
      value = -999;
  }

  else if(variable == "correctedD0VertexEEPositiveChargeLowPt"){
    if(fabs(object->eta) > 1.479 && object->charge > 0 && object->pt > 45) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexEEPositiveChargeHighPt"){
    if(fabs(object->eta) > 1.479 && object->charge > 0 && object->pt < 45) value = object->correctedD0Vertex;
    else value = -999;
  }

  else if(variable == "correctedD0VertexInEBPlus"){
    if(fabs(object->eta) < 0.8 && object->eta > 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexOutEBPlus"){
    if(object->isEB && fabs(object->eta) > 0.8 && object->eta > 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexEEPlus"){
    if(object->isEE && object->eta > 0) value = object->correctedD0Vertex;
    else value = -999;
  }

  else if(variable == "correctedD0BeamspotInEBPlus"){
    if(fabs(object->eta) < 0.8 && object->eta > 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotOutEBPlus"){
    if(object->isEB && fabs(object->eta) > 0.8 && object->eta > 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotEEPlus"){
    if(object->isEE && object->eta > 0) value = object->correctedD0;
    else value = -999;
  }

  else if(variable == "correctedD0VertexInEBMinus"){
    if(fabs(object->eta) < 0.8 && object->eta < 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexOutEBMinus"){
    if(object->isEB && fabs(object->eta) > 0.8 && object->eta < 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexEEMinus"){
    if(object->isEE && object->eta < 0) value = object->correctedD0Vertex;
    else value = -999;
  }

  else if(variable == "correctedD0BeamspotInEBMinus"){
    if(fabs(object->eta) < 0.8 && object->eta < 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotOutEBMinus"){
    if(object->isEB && fabs(object->eta) > 0.8 && object->eta < 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotEEMinus"){
    if(object->isEE && object->eta < 0) value = object->correctedD0;
    else value = -999;
  }

  else if(variable == "looseID"){
    if (object->isEB)
      {
        value = fabs(object->delEtaIn) < 0.007 \
          && fabs (object->delPhiIn) < 0.15 \
          && object->scSigmaIEtaIEta < 0.01 \
          && object->hadOverEm < 0.12 \
          && fabs (object->correctedD0Vertex) < 0.02 \
          && fabs (object->correctedDZ) < 0.2 \
          && object->absInvEMinusInvPin < 0.05 \
          && object->passConvVeto;
      }
    else
      {
        value = fabs(object->delEtaIn) < 0.009 \
          && fabs (object->delPhiIn) < 0.10 \
          && object->scSigmaIEtaIEta < 0.03 \
          && object->hadOverEm < 0.10 \
          && fabs (object->correctedD0Vertex) < 0.02 \
          && fabs (object->correctedDZ) < 0.2 \
          && object->absInvEMinusInvPin < 0.05 \
          && object->passConvVeto;
      }
  }

  else if(variable == "tightID"){
    if (object->isEB)
      {
        value = fabs(object->delEtaIn) < 0.004 \
          && fabs (object->delPhiIn) < 0.03 \
          && object->scSigmaIEtaIEta < 0.01 \
          && object->hadOverEm < 0.12 \
          && fabs (object->correctedD0Vertex) < 0.02 \
          && fabs (object->correctedDZ) < 0.1 \
          && object->absInvEMinusInvPin < 0.05 \
          && object->passConvVeto;
      }
    else
      {
        value = fabs(object->delEtaIn) < 0.005 \
          && fabs (object->delPhiIn) < 0.02 \
          && object->scSigmaIEtaIEta < 0.03 \
          && object->hadOverEm < 0.10 \
          && fabs (object->correctedD0Vertex) < 0.02 \
          && fabs (object->correctedDZ) < 0.1 \
          && object->absInvEMinusInvPin < 0.05 \
          && object->passConvVeto;
      }
  }

  else if(variable == "mvaNonTrig_HtoZZto4l"){ // https://twiki.cern.ch/twiki/bin/view/CMS/MultivariateElectronIdentification#Non_triggering_MVA
    value = false;
    if (object->pt > 7.0 && object->pt < 10.0)
      {
        if (abs (object->scEta) < 0.8 && object->mvaNonTrigV0 > 0.47)
          value = true;
        else if (abs (object->scEta) < 1.479 && object->mvaNonTrigV0 > 0.004)
          value = true;
        else if (abs (object->scEta) < 2.5 && object->mvaNonTrigV0 > 0.295)
          value = true;
      }
    else if (object->pt >= 10.0)
      {
        if (abs (object->scEta) < 0.8 && object->mvaNonTrigV0 > -0.34)
          value = true;
        else if (abs (object->scEta) < 1.479 && object->mvaNonTrigV0 > -0.65)
          value = true;
        else if (abs (object->scEta) < 2.5 && object->mvaNonTrigV0 > 0.60)
          value = true;
      }
  }
  else if(variable == "mvaTrig_HtoWWto2l2nu"){ // https://twiki.cern.ch/twiki/bin/view/CMS/MultivariateElectronIdentification#Triggering_MVA
    value = false;
    if (object->pt > 10.0 && object->pt < 20.0)
      {
        if (abs (object->scEta) < 0.8 && object->mvaTrigV0 > 0.00)
          value = true;
        else if (abs (object->scEta) < 1.479 && object->mvaTrigV0 > 0.10)
          value = true;
        else if (abs (object->scEta) < 2.5 && object->mvaTrigV0 > 0.62)
          value = true;
      }
    else if (object->pt >= 20.0)
      {
        if (abs (object->scEta) < 0.8 && object->mvaTrigV0 > 0.94)
          value = true;
        else if (abs (object->scEta) < 1.479 && object->mvaTrigV0 > 0.85)
          value = true;
        else if (abs (object->scEta) < 2.5 && object->mvaTrigV0 > 0.92)
          value = true;
      }
  }

  else if(variable == "looseID_MVA"){
    value = object->pt > 10
      && object->mvaNonTrigV0 > 0;
      }
  else if(variable == "correctedD0VertexInEBPositiveCharge"){
    if(fabs(object->eta) < 0.8 && object->charge > 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexOutEBPositiveCharge"){
    if(object->isEB && fabs(object->eta) > 0.8 && object->charge > 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexEEPositiveCharge"){
    if(object->isEE && object->charge > 0) value = object->correctedD0Vertex;
    else value = -999;
  }

  else if(variable == "correctedD0BeamspotInEBPositiveCharge"){
    if(fabs(object->eta) < 0.8 && object->charge > 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotOutEBPositiveCharge"){
    if(object->isEB && fabs(object->eta) > 0.8 && object->charge > 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotEEPositiveCharge"){
    if(object->isEE && object->charge > 0) value = object->correctedD0;
    else value = -999;
  }

  else if(variable == "correctedD0VertexInEBNegativeCharge"){
    if(fabs(object->eta) < 0.8 && object->charge < 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexOutEBNegativeCharge"){
    if(object->isEB && fabs(object->eta) > 0.8 && object->charge < 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexEENegativeCharge"){
    if(object->isEE && object->charge < 0) value = object->correctedD0Vertex;
    else value = -999;
  }

  else if(variable == "correctedD0BeamspotInEBNegativeCharge"){
    if(fabs(object->eta) < 0.8 && object->charge < 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotOutEBNegativeCharge"){
    if(object->isEB && fabs(object->eta) > 0.8 && object->charge < 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotEENegativeCharge"){
    if(object->isEE && object->charge < 0) value = object->correctedD0;
    else value = -999;
  }


  else if(variable == "tightIDdisplaced"){
    if (object->isEB)
      {
        value = fabs(object->delEtaIn) < 0.004 \
          && fabs (object->delPhiIn) < 0.03 \
          && object->scSigmaIEtaIEta < 0.01 \
          && object->hadOverEm < 0.12 \
          && object->absInvEMinusInvPin < 0.05;
      }
    else
      {
        value = fabs (object->delEtaIn) < 0.005 \
          && fabs (object->delPhiIn) < 0.02 \
          && object->scSigmaIEtaIEta < 0.03 \
          && object->hadOverEm < 0.10 \
          && object->absInvEMinusInvPin < 0.05;
      }
  }


  else if(variable == "genDeltaRLowest") value = getGenDeltaRLowest(object);

  else if(variable == "genMatchedPdgId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else value = mcparticles->at(index).id;
  }

  else if(variable == "genRecoChargeProduct"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 2;
    else value = object->charge * mcparticles->at(index).charge;
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
  else if(variable == "pfElectronsFromVertex"){
    double d0Error, dzError;

    d0Error = hypot (object->tkD0err, hypot (chosenVertex ()->xError, chosenVertex ()->yError));
    dzError = hypot (object->tkDZerr, chosenVertex ()->zError);
    value = fabs (object->correctedD0Vertex) > 0.2 || fabs (object->correctedDZ) > 0.5
      || fabs (object->correctedD0Vertex / d0Error) > 99.0
      || fabs (object->correctedDZ / dzError) > 99.0;
    value = !value;
  }



  else{clog << "WARNING: invalid electron variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
} // end electron valueLookup
