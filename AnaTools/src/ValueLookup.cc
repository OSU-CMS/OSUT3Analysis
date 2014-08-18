#include "OSUT3Analysis/AnaTools/plugins/OSUAnalysis.h"

//!jet valueLookup
double
OSUAnalysis::valueLookup (const BNjet* object, string variable, string function, string &stringValue){

  BNjet jetCorr;
  if (flagJESJERCorr_) {
    jetCorr = getCorrectedJet(*object, jESJERCorr_);  
    object = &jetCorr;  
  }

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
  else if(variable == "Upt") value = object->Upt;
  else if(variable == "Uenergy") value = object->Uenergy;
  else if(variable == "L2pt") value = object->L2pt;
  else if(variable == "L2L3pt") value = object->L2L3pt;
  else if(variable == "L2L3respt") value = object->L2L3respt;
  else if(variable == "respt") value = object->respt;
  else if(variable == "EMfrac") value = object->EMfrac;
  else if(variable == "Hadfrac") value = object->Hadfrac;
  else if(variable == "charge") value = object->charge;
  else if(variable == "mass") value = object->mass;
  else if(variable == "area") value = object->area;
  else if(variable == "fHPD") value = object->fHPD;
  else if(variable == "approximatefHPD") value = object->approximatefHPD;
  else if(variable == "genPartonET") value = object->genPartonET;
  else if(variable == "genPartonPT") value = object->genPartonPT;
  else if(variable == "genPartonEta") value = object->genPartonEta;
  else if(variable == "genPartonPhi") value = object->genPartonPhi;
  else if(variable == "genJetET") value = object->genJetET;
  else if(variable == "genJetPT") value = object->genJetPT;
  else if(variable == "genJetEta") value = object->genJetEta;
  else if(variable == "genJetPhi") value = object->genJetPhi;
  else if(variable == "btagTChighPur") value = object->btagTChighPur;
  else if(variable == "btagTChighEff") value = object->btagTChighEff;
  else if(variable == "btagJetProb") value = object->btagJetProb;
  else if(variable == "btagJetBProb") value = object->btagJetBProb;
  else if(variable == "btagSoftEle") value = object->btagSoftEle;
  else if(variable == "btagSoftMuon") value = object->btagSoftMuon;
  else if(variable == "btagSoftMuonNoIP") value = object->btagSoftMuonNoIP;
  else if(variable == "btagSecVertex") value = object->btagSecVertex;
  else if(variable == "btagSecVertexHighEff") value = object->btagSecVertexHighEff;
  else if(variable == "btagSecVertexHighPur") value = object->btagSecVertexHighPur;
  else if(variable == "btagCombinedSecVertex") value = object->btagCombinedSecVertex;
  else if(variable == "btagCombinedSecVertexMVA") value = object->btagCombinedSecVertexMVA;
  else if(variable == "btagSoftMuonByPt") value = object->btagSoftMuonByPt;
  else if(variable == "btagSoftMuonByIP3") value = object->btagSoftMuonByIP3;
  else if(variable == "btagSoftElectronByPt") value = object->btagSoftElectronByPt;
  else if(variable == "btagSoftElectronByIP3") value = object->btagSoftElectronByIP3;
  else if(variable == "n90Hits") value = object->n90Hits;
  else if(variable == "hitsInN90") value = object->hitsInN90;
  else if(variable == "chargedHadronEnergyFraction") value = object->chargedHadronEnergyFraction;
  else if(variable == "neutralHadronEnergyFraction") value = object->neutralHadronEnergyFraction;
  else if(variable == "chargedEmEnergyFraction") value = object->chargedEmEnergyFraction;
  else if(variable == "neutralEmEnergyFraction") value = object->neutralEmEnergyFraction;
  else if(variable == "chargedHadronEnergy") value = object->chargedHadronEnergyFraction*object->pt;
  else if(variable == "neutralHadronEnergy") value = object->neutralHadronEnergyFraction*object->pt;
  else if(variable == "chargedEmEnergy") value = object->chargedEmEnergyFraction*object->pt;
  else if(variable == "neutralEmEnergy") value = object->neutralEmEnergyFraction*object->pt;
  else if(variable == "fLong") value = object->fLong;
  else if(variable == "fShort") value = object->fShort;
  else if(variable == "etaetaMoment") value = object->etaetaMoment;
  else if(variable == "phiphiMoment") value = object->phiphiMoment;
  else if(variable == "JESunc") value = object->JESunc;
  else if(variable == "JECuncUp") value = object->JECuncUp;
  else if(variable == "JECuncDown") value = object->JECuncDown;
  else if(variable == "puJetMVA_full") value = object->puJetMVA_full;
  else if(variable == "puJetMVA_simple") value = object->puJetMVA_simple;
  else if(variable == "puJetMVA_cutbased") value = object->puJetMVA_cutbased;
  else if(variable == "dZ") value = object->dZ;
  else if(variable == "dR2Mean") value = object->dR2Mean;
  else if(variable == "dRMean") value = object->dRMean;
  else if(variable == "frac01") value = object->frac01;
  else if(variable == "frac02") value = object->frac02;
  else if(variable == "frac03") value = object->frac03;
  else if(variable == "frac04") value = object->frac04;
  else if(variable == "frac05") value = object->frac05;
  else if(variable == "frac06") value = object->frac06;
  else if(variable == "frac07") value = object->frac07;
  else if(variable == "beta") value = object->beta;
  else if(variable == "betaStar") value = object->betaStar;
  else if(variable == "betaClassic") value = object->betaClassic;
  else if(variable == "betaStarClassic") value = object->betaStarClassic;
  else if(variable == "ptD") value = object->ptD;
  else if(variable == "nvtx") value = object->nvtx;
  else if(variable == "d0") value = object->d0;
  else if(variable == "leadCandPt") value = object->leadCandPt;
  else if(variable == "leadCandVx") value = object->leadCandVx;
  else if(variable == "leadCandVy") value = object->leadCandVy;
  else if(variable == "leadCandVz") value = object->leadCandVz;
  else if(variable == "leadCandDistFromPV") value = object->leadCandDistFromPV;
  else if(variable == "flavour") value = object->flavour;
  else if(variable == "Nconst") value = object->Nconst;
  else if(variable == "jetIDMinimal") value = object->jetIDMinimal;
  else if(variable == "jetIDLooseAOD") value = object->jetIDLooseAOD;
  else if(variable == "jetIDLoose") value = object->jetIDLoose;
  else if(variable == "jetIDTight") value = object->jetIDTight;
  else if(variable == "genPartonId") value = object->genPartonId;
  else if(variable == "genPartonMotherId") value = object->genPartonMotherId;
  else if(variable == "genPartonMother0Id") value = object->genPartonMother0Id;
  else if(variable == "genPartonMother1Id") value = object->genPartonMother1Id;
  else if(variable == "genPartonGrandMotherId") value = object->genPartonGrandMotherId;
  else if(variable == "genPartonGrandMother00Id") value = object->genPartonGrandMother00Id;
  else if(variable == "genPartonGrandMother01Id") value = object->genPartonGrandMother01Id;
  else if(variable == "genPartonGrandMother10Id") value = object->genPartonGrandMother10Id;
  else if(variable == "genPartonGrandMother11Id") value = object->genPartonGrandMother11Id;
  else if(variable == "chargedMultiplicity") value = object->chargedMultiplicity;
  else if(variable == "neutralMultiplicity") value = object->neutralMultiplicity;
  else if(variable == "nconstituents") value = object->nconstituents;
  else if(variable == "nHit") value = object->nHit;
  else if(variable == "puJetId_full") value = object->puJetId_full;
  else if(variable == "puJetId_simple") value = object->puJetId_simple;
  else if(variable == "puJetId_cutbased") value = object->puJetId_cutbased;
  else if(variable == "puJetId_tight_full") value = object->puJetId_tight_full;
  else if(variable == "puJetId_tight_simple") value = object->puJetId_tight_simple;
  else if(variable == "puJetId_tight_cutbased") value = object->puJetId_tight_cutbased;
  else if(variable == "puJetId_medium_full") value = object->puJetId_medium_full;
  else if(variable == "puJetId_medium_simple") value = object->puJetId_medium_simple;
  else if(variable == "puJetId_medium_cutbased") value = object->puJetId_medium_cutbased;
  else if(variable == "puJetId_loose_full") value = object->puJetId_loose_full;
  else if(variable == "puJetId_loose_simple") value = object->puJetId_loose_simple;
  else if(variable == "puJetId_loose_cutbased") value = object->puJetId_loose_cutbased;

  //user defined variable
  else if(variable == "disappTrkLeadingJetID") {
    value = object->pt > 110
      && fabs(object->eta) < 2.4
      && object->chargedHadronEnergyFraction > 0.2
      && object->neutralHadronEnergyFraction < 0.7
      && object->chargedEmEnergyFraction < 0.5
      && object->neutralEmEnergyFraction < 0.7;
  }

  else if(variable == "disappTrkSubLeadingJetID") {
    value = object->pt > 30
      && fabs(object->eta) < 4.5
      && object->neutralHadronEnergyFraction < 0.7
      && object->chargedEmEnergyFraction < 0.5;
      }

  else if(variable == "dPhiMet") {
    if (const BNmet *met = chosenMET ()) {
      value = deltaPhi (object->phi, met->phi);
    } else value = -999;
  }

  else if(variable == "metPt") {  // allow making 2D plots of jet quantities vs. Met  
    if (const BNmet *met = chosenMET ()) {
      value = met->pt;  
    } else value = -999;
  }

  else if(variable == "isLeadingPtJet") {
    double ptMax = -99;  
    for (uint ijet = 0; ijet<jets->size(); ijet++) {
      string empty = "";  
      double jetPt = valueLookup(&jets->at(ijet), "pt", "", empty); 
      if (jetPt > ptMax) ptMax = jetPt;  
    }
    if (object->pt < ptMax) value = 0; 
    else                    value = 1;  
  }  

  else if(variable == "deltaRMuonPt20") {
    // calculate the minimum deltaR between the jet and any muon with pT>20 GeV
    double deltaRMin = 99; 
    if (!muons.product()) clog << "ERROR:  cannot find deltaRMuonPt20 because muons collection is not initialized." << endl;
    for (uint imuon = 0; imuon<muons->size(); imuon++) {
      string empty = "";  
      double muonPt = valueLookup(&muons->at(imuon), "pt", "", empty); 
      if (muonPt < 20) continue;  
      double muonEta = valueLookup(&muons->at(imuon), "eta", "", empty); 
      double muonPhi = valueLookup(&muons->at(imuon), "phi", "", empty); 
      double dR = deltaR(object->eta, object->phi, muonEta, muonPhi);  
      if (dR < deltaRMin) deltaRMin = dR;
    }
    value = deltaRMin; 
  }  
  else{clog << "WARNING: invalid jet variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
} // end jet valueLookup


//!muon valueLookup
double
OSUAnalysis::valueLookup (const BNmuon* object, string variable, string function, string &stringValue){

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
      value = -999;
  }
  else if(variable == "metDeltaPhi") {
    if (const BNmet *met = chosenMET ())
      value = deltaPhi (object->phi, met->phi);
    else
      value = -999;
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
      value = -999;
  }

  else if(variable == "correctedD0VertexInEBPlus"){
    if(fabs(object->eta) < 0.8 && object->eta > 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexOutEBPlus"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->eta > 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexEEPlus"){
    if(fabs(object->eta) > 1.479 && object->eta > 0) value = object->correctedD0Vertex;
    else value = -999;
  }

  else if(variable == "correctedD0BeamspotInEBPlus"){
    if(fabs(object->eta) < 0.8 && object->eta > 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotOutEBPlus"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->eta > 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotEEPlus"){
    if(fabs(object->eta) > 1.479 && object->eta > 0) value = object->correctedD0;
    else value = -999;
  }

  else if(variable == "correctedD0VertexInEBMinus"){
    if(fabs(object->eta) < 0.8 && object->eta < 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexOutEBMinus"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->eta < 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexEEMinus"){
    if(fabs(object->eta) > 1.479 && object->eta < 0) value = object->correctedD0Vertex;
    else value = -999;
  }

  else if(variable == "correctedD0BeamspotInEBMinus"){
    if(fabs(object->eta) < 0.8 && object->eta < 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotOutEBMinus"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->eta < 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotEEMinus"){
    if(fabs(object->eta) > 1.479 && object->eta < 0) value = object->correctedD0;
    else value = -999;
  }


  else if(variable == "correctedD0VertexInEBPositiveCharge"){
    if(fabs(object->eta) < 0.8 && object->charge > 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexOutEBPositiveCharge"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->charge > 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexEEPositiveCharge"){
    if(fabs(object->eta) > 1.479 && object->charge > 0) value = object->correctedD0Vertex;
    else value = -999;
  }

  else if(variable == "correctedD0BeamspotInEBPositiveCharge"){
    if(fabs(object->eta) < 0.8 && object->charge > 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotOutEBPositiveCharge"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->charge > 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotEEPositiveCharge"){
    if(fabs(object->eta) > 1.479 && object->charge > 0) value = object->correctedD0;
    else value = -999;
  }

  else if(variable == "correctedD0VertexInEBNegativeCharge"){
    if(fabs(object->eta) < 0.8 && object->charge < 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexOutEBNegativeCharge"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->charge < 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexEENegativeCharge"){
    if(fabs(object->eta) > 1.479 && object->charge < 0) value = object->correctedD0Vertex;
    else value = -999;
  }

  else if(variable == "correctedD0BeamspotInEBNegativeCharge"){
    if(fabs(object->eta) < 0.8 && object->charge < 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotOutEBNegativeCharge"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->charge < 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotEENegativeCharge"){
    if(fabs(object->eta) > 1.479 && object->charge < 0) value = object->correctedD0;
    else value = -999;
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



  else{clog << "WARNING: invalid muon variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
} // end muon valueLookup


//!electron valueLookup
double
OSUAnalysis::valueLookup (const BNelectron* object, string variable, string function, string &stringValue){

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


//!event valueLookup
double
OSUAnalysis::valueLookup (const BNevent* object, string variable, string function, string &stringValue){

  double value = 0.0;
  string empty = "";

  if(variable == "weight") value = object->weight;
  else if(variable == "pthat") value = object->pthat;
  else if(variable == "qScale") value = object->qScale;
  else if(variable == "alphaQCD") value = object->alphaQCD;
  else if(variable == "alphaQED") value = object->alphaQED;
  else if(variable == "scalePDF") value = object->scalePDF;
  else if(variable == "x1") value = object->x1;
  else if(variable == "nExtraPartons") value = GetNumExtraPartons(mcparticles.product());
  else if(variable == "x2") value = object->x2;
  else if(variable == "xPDF1") value = object->xPDF1;
  else if(variable == "xPDF2") value = object->xPDF2;
  else if(variable == "BSx") value = object->BSx;
  else if(variable == "BSy") value = object->BSy;
  else if(variable == "BSz") value = object->BSz;
  else if(variable == "PVx") value = object->PVx;
  else if(variable == "PVy") value = object->PVy;
  else if(variable == "PVz") value = object->PVz;
  else if(variable == "bField") value = object->bField;
  else if(variable == "instLumi") value = object->instLumi;
  else if(variable == "bxLumi") value = object->bxLumi;
  else if(variable == "FilterOutScrapingFraction") value = object->FilterOutScrapingFraction;
  else if(variable == "sumNVtx") value = object->sumNVtx;
  else if(variable == "sumTrueNVtx") value = object->sumTrueNVtx;
  else if(variable == "nm1_true") value = object->nm1_true;
  else if(variable == "n0_true") value = object->n0_true;
  else if(variable == "np1_true") value = object->np1_true;
  else if(variable == "numTruePV") value = object->numTruePV;
  else if(variable == "Q2ScaleUpWgt") value = object->Q2ScaleUpWgt;
  else if(variable == "Q2ScaleDownWgt") value = object->Q2ScaleDownWgt;
  else if(variable == "rho_kt6PFJets") value = object->rho_kt6PFJets;
  else if(variable == "rho_kt6PFJetsCentralChargedPileUp") value = object->rho_kt6PFJetsCentralChargedPileUp;
  else if(variable == "rho_kt6PFJetsCentralNeutral") value = object->rho_kt6PFJetsCentralNeutral;
  else if(variable == "rho_kt6PFJetsCentralNeutralTight") value = object->rho_kt6PFJetsCentralNeutralTight;
  else if(variable == "run") value = object->run;
  else if(variable == "lumi") value = object->lumi;
  else if(variable == "sample") value = object->sample;
  else if(variable == "numPV") value = object->numPV;
  else if(variable == "W0decay") value = object->W0decay;
  else if(variable == "W1decay") value = object->W1decay;
  else if(variable == "Z0decay") value = object->Z0decay;
  else if(variable == "Z1decay") value = object->Z1decay;
  else if(variable == "H0decay") value = object->H0decay;
  else if(variable == "H1decay") value = object->H1decay;
  else if(variable == "hcalnoiseLoose") value = object->hcalnoiseLoose;
  else if(variable == "hcalnoiseTight") value = object->hcalnoiseTight;
  else if(variable == "GoodVertex") value = object->GoodVertex;
  else if(variable == "FilterOutScraping") value = object->FilterOutScraping;
  else if(variable == "HBHENoiseFilter") value = object->HBHENoiseFilter;
  else if(variable == "CSCLooseHaloId") value = object->CSCLooseHaloId;
  else if(variable == "CSCTightHaloId") value = object->CSCTightHaloId;
  else if(variable == "EcalLooseHaloId") value = object->EcalLooseHaloId;
  else if(variable == "EcalTightHaloId") value = object->EcalTightHaloId;
  else if(variable == "HcalLooseHaloId") value = object->HcalLooseHaloId;
  else if(variable == "HcalTightHaloId") value = object->HcalTightHaloId;
  else if(variable == "GlobalLooseHaloId") value = object->GlobalLooseHaloId;
  else if(variable == "GlobalTightHaloId") value = object->GlobalTightHaloId;
  else if(variable == "LooseId") value = object->LooseId;
  else if(variable == "TightId") value = object->TightId;
  else if(variable == "numGenPV") value = object->numGenPV;
  else if(variable == "nm1") value = object->nm1;
  else if(variable == "n0") value = object->n0;
  else if(variable == "np1") value = object->np1;
  else if(variable == "id1") value = object->id1;
  else if(variable == "id2") value = object->id2;
  else if(variable == "evt") value = object->evt;
  else if(variable == "puScaleFactor"){
    if(doPileupReweighting_ && datasetType_ != "data")
      value = puWeight_->at (events->at (0).numTruePV);
    else
      value = 1.0;
  }
  else if(variable == "muonScaleFactor") value = muonScaleFactor_;
  else if(variable == "electronScaleFactor") value = electronScaleFactor_;
  else if(variable == "muonTrackScaleFactor") value = muonTrackScaleFactor_;
  else if(variable == "electronTrackScaleFactor") value = electronTrackScaleFactor_;
  else if(variable == "stopCTauScaleFactor") value = stopCTauScaleFactor_;
  else if(variable == "bTagScaleFactor") value = bTagScaleFactor_;
  else if(variable == "topPtScaleFactor") value = topPtScaleFactor_;
  else if(variable == "triggerScaleFactor") value = triggerScaleFactor_;
  else if(variable == "triggerMetScaleFactor") value = triggerMetScaleFactor_;
  else if(variable == "trackNMissOutScaleFactor") value = trackNMissOutScaleFactor_;
  else if(variable == "EcaloVaryScaleFactor") value = EcaloVaryScaleFactor_;
  else if(variable == "isrVaryScaleFactor") value = isrVaryScaleFactor_;  
  else if(variable == "globalScaleFactor") value = globalScaleFactor_;
  else if(variable == "channelScaleFactor") value = channelScaleFactor_;
  else if(variable == "eventScaleFactor") value = eventScaleFactor_;
  else if(variable == "unfilteredHt") value = getHt(jets.product());
  else if(variable == "ht") value = chosenHT ();
  else if(variable == "sumJetPt") value = getSumJetPt(jets.product());
  else if(variable == "unfilteredSt") value = getSt(electrons.product(),muons.product(),jets.product());
  else if(variable == "st") value = chosenST ();

  else if(variable == "metPt") { // allow making 2D plots of event quantities vs. Met
    if (const BNmet *met = chosenMET ()) {
      value = met->pt;
    } else value = -999;
  }
  else if(variable == "leadMuPairInvMass"){
    pair<const BNmuon *, const BNmuon *> muPair = leadMuonPair ();
    TLorentzVector p0 (muPair.first->px, muPair.first->py, muPair.first->pz, muPair.first->energy),
                   p1 (muPair.second->px, muPair.second->py, muPair.second->pz, muPair.second->energy);
    value = (p0 + p1).M ();
  }
  else if(variable == "leadMuPairPt"){
    pair<const BNmuon *, const BNmuon *> muPair = leadMuonPair ();
    TVector2 pt0 (muPair.first->px, muPair.first->py),
             pt1 (muPair.second->px, muPair.second->py);
    pt0 += pt1;
    value = pt0.Mod ();
  }
  else if(variable == "leadElPairInvMass"){
    pair<const BNelectron *, const BNelectron *> muPair = leadElectronPair ();
    TLorentzVector p0 (muPair.first->px, muPair.first->py, muPair.first->pz, muPair.first->energy),
                   p1 (muPair.second->px, muPair.second->py, muPair.second->pz, muPair.second->energy);
    value = (p0 + p1).M ();
  }
  else if(variable == "leadElPairPt"){
    pair<const BNelectron *, const BNelectron *> muPair = leadElectronPair ();
    TVector2 pt0 (muPair.first->px, muPair.first->py),
             pt1 (muPair.second->px, muPair.second->py);
    pt0 += pt1;
    value = pt0.Mod ();
  }
  else if(variable == "metPt") {  // allow making 2D plots of event quantities vs. Met                                                                                                                                                       
    if (const BNmet *met = chosenMET ()) {
      value = met->pt;
    } else value = -999;
  }
  else if(variable == "totalMcparticlePt") {
    TVector2 ptTot(0.0, 0.0); 
    if(find(objectsToCut.begin(),objectsToCut.end(),"mcparticles") != objectsToCut.end ()){
      flagPair mcFlags = getLastValidFlags("mcparticles");  
      for (uint mcIndex = 0; mcIndex != mcFlags.size(); mcIndex++) {
	if (!mcFlags.at(mcIndex).second) continue;
	TVector2 pt(mcparticles->at(mcIndex).px, mcparticles->at(mcIndex).py);
	ptTot = pt + ptTot;  
      }
    }
    value = ptTot.Mod();
  } 
  else if(variable == "totalMcparticlePhi") {
    TVector2 ptTot(0.0, 0.0); 
    if(find(objectsToCut.begin(),objectsToCut.end(),"mcparticles") != objectsToCut.end ()){
      flagPair mcFlags = getLastValidFlags("mcparticles");  
      for (uint mcIndex = 0; mcIndex != mcFlags.size(); mcIndex++) {
	if (!mcFlags.at(mcIndex).second) continue;
	TVector2 pt(mcparticles->at(mcIndex).px, mcparticles->at(mcIndex).py);
	ptTot = pt + ptTot;  
      }
    }
    value = ptTot.Phi();
  } 
  else if(variable == "totalMuonPt") {
    TVector2 ptTot(0.0, 0.0); 
    if(find(objectsToCut.begin(),objectsToCut.end(),"muons") != objectsToCut.end ()){
      flagPair muFlags = getLastValidFlags("muons");  
      for (uint muIndex = 0; muIndex != muFlags.size(); muIndex++) {
	if (!muFlags.at(muIndex).second) continue;
	TVector2 pt(muons->at(muIndex).px, muons->at(muIndex).py);
	ptTot = pt + ptTot;  
      }
    }
    value = ptTot.Mod();
  } 

  else if(variable == "totalMcparticlePtByMetPt") {
    double MET = 0;
    double mcPt = 0;

    //get met
if (const BNmet *met = chosenMET ()) {
      MET = met->pt;
    } else value = -999;

//get mcPt
 TVector2 ptTot(0.0, 0.0);

 if(find(objectsToCut.begin(),objectsToCut.end(),"mcparticles") != objectsToCut.end ()){
   flagPair mcFlags;
   //get the last valid flags in the flag map                                                                                               
   for (int i = cumulativeFlags.at("mcparticles").size() - 1; i >= 0; i--){
     if (cumulativeFlags.at("mcparticles").at(i).size()){
       mcFlags = cumulativeFlags.at("mcparticles").at(i);
       break;
     }
   }
   for (uint mcIndex = 0; mcIndex != mcFlags.size(); mcIndex++) {
     if (!mcFlags.at(mcIndex).second) continue;
     TVector2 pt(mcparticles->at(mcIndex).px, mcparticles->at(mcIndex).py);
     ptTot = pt + ptTot;
   }
 }
 mcPt = ptTot.Mod();

value = (mcPt/MET);
  }
  else if(variable == "totalMuonPhi") {
    TVector2 ptTot(0.0, 0.0); 
    if(find(objectsToCut.begin(),objectsToCut.end(),"muons") != objectsToCut.end ()){
      flagPair muFlags = getLastValidFlags("muons");  
      for (uint muIndex = 0; muIndex != muFlags.size(); muIndex++) {
	if (!muFlags.at(muIndex).second) continue;
	TVector2 pt(muons->at(muIndex).px, muons->at(muIndex).py);
	ptTot = pt + ptTot;  
      }
    }
    value = ptTot.Phi();
  } 
  else if(variable == "totalJetPt") {
    TVector2 ptTot(0.0, 0.0); 
    if(find(objectsToCut.begin(),objectsToCut.end(),"jets") != objectsToCut.end ()){
      flagPair jetFlags = getLastValidFlags("jets");  
      for (uint jetIndex = 0; jetIndex != jetFlags.size(); jetIndex++) {
	if (!jetFlags.at(jetIndex).second) continue;
	TVector2 pt(jets->at(jetIndex).px, jets->at(jetIndex).py);
	ptTot = pt + ptTot;  
      }
    }
    value = ptTot.Mod();
  } 
  else if(variable == "totalJetPhi") {
    TVector2 ptTot(0.0, 0.0); 
    if(find(objectsToCut.begin(),objectsToCut.end(),"jets") != objectsToCut.end ()){
      flagPair jetFlags = getLastValidFlags("jets");  
      for (uint jetIndex = 0; jetIndex != jetFlags.size(); jetIndex++) {
	if (!jetFlags.at(jetIndex).second) continue;
	TVector2 pt(jets->at(jetIndex).px, jets->at(jetIndex).py);
	ptTot = pt + ptTot;  
      }
    }
    value = ptTot.Phi();
  } 
  else if(variable == "dijetDeltaPhiMax") {
    double deltaPhiMax = -99.;
    if(find(objectsToCut.begin(),objectsToCut.end(),"jets") != objectsToCut.end ()){
      flagPair jetFlags = getLastValidFlags("jets");
      for   (uint iJet = 0;      iJet != jetFlags.size(); iJet++) {
	for (uint jJet = iJet+1; jJet != jetFlags.size(); jJet++) {
          if (!jetFlags.at(iJet).second) continue;
          if (!jetFlags.at(jJet).second) continue;
          double dPhi = fabs(deltaPhi(jets->at(iJet).phi, jets->at(jJet).phi));
          if (dPhi > deltaPhiMax) deltaPhiMax = dPhi;
        }
      }
    }
    value = deltaPhiMax;
  }
  else if(variable == "totalJetPtMinusTotalMcparticlePt") {
    value = 
      valueLookup(object, "totalJetPt",        "", empty) - 
      valueLookup(object, "totalMcparticlePt", "", empty);  
  }
  else if(variable == "totalJetPhiMinusTotalMcparticlePhi") {
    value = deltaPhi(valueLookup(object, "totalJetPhi",        "", empty),
		     valueLookup(object, "totalMcparticlePhi", "", empty));  
  }
  else if(variable == "totalMuonPtMinusTotalMcparticlePt") {
    value = 
      valueLookup(object, "totalMuonPt",       "", empty) - 
      valueLookup(object, "totalMcparticlePt", "", empty);  
  }
  else if(variable == "totalMuonPtMinusTotalJetPt") {
    value = 
      valueLookup(object, "totalMuonPt",       "", empty) - 
      valueLookup(object, "totalJetPt",        "", empty);  
  }
  else if(variable == "totalMuonPhiMinusTotalJetPhi") {
    value = deltaPhi(valueLookup(object, "totalMuonPhi", "", empty),
		     valueLookup(object, "totalJetPhi",  "", empty));  
  }
  else if(variable == "totalMcparticleStatus3SusyIdPt") {
    TVector2 ptTot(0.0, 0.0); 

    for (BNmcparticleCollection::const_iterator mcparticle = mcparticles->begin (); 
	 mcparticle < mcparticles->end(); 
	 mcparticle++) {

      if (mcparticle->status != 3) continue;  
      int pdgId = abs(mcparticle->id);  
      bool isSusy = (pdgId>1000001 && pdgId<3160113);  // matched to a SUSY particle
      if (!isSusy) continue;  
      TVector2 pt(mcparticle->px, mcparticle->py);
      ptTot = pt + ptTot;  
    }
    value = ptTot.Mod();
  }

  else{clog << "WARNING: invalid event variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
} // end event valueLookup


//!tau valueLookup
double
OSUAnalysis::valueLookup (const BNtau* object, string variable, string function, string &stringValue){

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


//!met valueLookup
double
OSUAnalysis::valueLookup (const BNmet* object, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "et") value = object->et;
  else if(variable == "pt") value = object->pt;
  else if(variable == "px") value = object->px;
  else if(variable == "py") value = object->py;
  else if(variable == "phi") value = object->phi;
  else if(variable == "Upt") value = object->Upt;
  else if(variable == "Uphi") value = object->Uphi;
  else if(variable == "NeutralEMFraction") value = object->NeutralEMFraction;
  else if(variable == "NeutralHadEtFraction") value = object->NeutralHadEtFraction;
  else if(variable == "ChargedEMEtFraction") value = object->ChargedEMEtFraction;
  else if(variable == "ChargedHadEtFraction") value = object->ChargedHadEtFraction;
  else if(variable == "MuonEtFraction") value = object->MuonEtFraction;
  else if(variable == "Type6EtFraction") value = object->Type6EtFraction;
  else if(variable == "Type7EtFraction") value = object->Type7EtFraction;
  else if(variable == "genPT") value = object->genPT;
  else if(variable == "genPhi") value = object->genPhi;
  else if(variable == "muonCorEx") value = object->muonCorEx;
  else if(variable == "muonCorEy") value = object->muonCorEy;
  else if(variable == "jet20CorEx") value = object->jet20CorEx;
  else if(variable == "jet20CorEy") value = object->jet20CorEy;
  else if(variable == "jet1CorEx") value = object->jet1CorEx;
  else if(variable == "jet1CorEy") value = object->jet1CorEy;
  else if(variable == "sumET") value = object->sumET;
  else if(variable == "corSumET") value = object->corSumET;
  else if(variable == "mEtSig") value = object->mEtSig;
  else if(variable == "metSignificance") value = object->metSignificance;
  else if(variable == "significance") value = object->significance;
  else if(variable == "sigmaX2") value = object->sigmaX2;
  else if(variable == "sigmaY2") value = object->sigmaY2;
  else if(variable == "sigmaXY") value = object->sigmaXY;
  else if(variable == "sigmaYX") value = object->sigmaYX;
  else if(variable == "maxEtInEmTowers") value = object->maxEtInEmTowers;
  else if(variable == "emEtFraction") value = object->emEtFraction;
  else if(variable == "emEtInEB") value = object->emEtInEB;
  else if(variable == "emEtInEE") value = object->emEtInEE;
  else if(variable == "emEtInHF") value = object->emEtInHF;
  else if(variable == "maxEtInHadTowers") value = object->maxEtInHadTowers;
  else if(variable == "hadEtFraction") value = object->hadEtFraction;
  else if(variable == "hadEtInHB") value = object->hadEtInHB;
  else if(variable == "hadEtInHE") value = object->hadEtInHE;
  else if(variable == "hadEtInHF") value = object->hadEtInHF;
  else if(variable == "hadEtInHO") value = object->hadEtInHO;
  else if(variable == "UDeltaPx") value = object->UDeltaPx;
  else if(variable == "UDeltaPy") value = object->UDeltaPy;
  else if(variable == "UDeltaP") value = object->UDeltaP;
  else if(variable == "Uscale") value = object->Uscale;
  else if(variable == "type2corPx") value = object->type2corPx;
  else if(variable == "type2corPy") value = object->type2corPy;
  else if(variable == "T2pt") value = object->T2pt;
  else if(variable == "T2px") value = object->T2px;
  else if(variable == "T2py") value = object->T2py;
  else if(variable == "T2phi") value = object->T2phi;
  else if(variable == "T2sumET") value = object->T2sumET;
  else if(variable == "pfT1jet1pt") value = object->pfT1jet1pt;
  else if(variable == "pfT1jet1phi") value = object->pfT1jet1phi;
  else if(variable == "pfT1jet6pt") value = object->pfT1jet6pt;
  else if(variable == "pfT1jet6phi") value = object->pfT1jet6phi;
  else if(variable == "pfT1jet10pt") value = object->pfT1jet10pt;
  else if(variable == "pfT1jet10phi") value = object->pfT1jet10phi;

  else if(variable == "hltPt") value = getPtSingleObjectMatchesAnyTrigger(object->phi, targetTriggers_, trigobjs.product());

  else if(variable == "metNoMu") { 
    // Calculate the MET, without including muons in the sum of E_T  
    TVector2 p2Met;
    TVector2 p2Muon;
    p2Met. SetMagPhi(object->pt, object->phi);
    p2Muon.SetMagPhi(0, 0);  
    if (!muons.product()) clog << "ERROR: cannot find metNoMu because muons collection is not initialized." << endl;
    for (uint imuon = 0; imuon<muons->size(); imuon++) {  
      string empty = "";
      double muonPt  = valueLookup(&muons->at(imuon), "pt",  "", empty);
      double muonPhi = valueLookup(&muons->at(imuon), "phi", "", empty);
      TVector2 p2MuonTmp;
      p2MuonTmp.SetMagPhi(muonPt, muonPhi);  
      p2Muon += p2MuonTmp;  
    }
    TVector2 p2MetNoMu = p2Met + p2Muon;
    value = p2MetNoMu.Mod();
  }

  else if(variable == "hltPtNoMu") {
    // Calculate the MET, without including muons in the sum of E_T                                                                                       
    TVector2 p2Met;
    TVector2 p2Muon;
    p2Met. SetMagPhi(object->pt, object->phi);
    p2Muon.SetMagPhi(0, 0);
    if (!muons.product()) clog << "ERROR: cannot find metNoMu because muons collection is not initialized." << endl;
    for (uint imuon = 0; imuon<muons->size(); imuon++) {
      string empty = "";
      double muonPt  = valueLookup(&muons->at(imuon), "pt",  "", empty);
      double muonPhi = valueLookup(&muons->at(imuon), "phi", "", empty);
      TVector2 p2MuonTmp;
      p2MuonTmp.SetMagPhi(muonPt, muonPhi);
      p2Muon += p2MuonTmp;
    }
    TVector2 p2MetNoMu = p2Met + p2Muon;
    //    value = p2MetNoMu.Mod();
    value = getPtSingleObjectMatchesAnyTrigger(p2MetNoMu.Phi(), targetTriggers_, trigobjs.product());
  }
  else if(variable == "metNoElec") { 
    // Calculate the MET, without including electrons in the sum of E_T  
    TVector2 p2Met;
    TVector2 p2Elec;
    p2Met. SetMagPhi(object->pt, object->phi);
    p2Elec.SetMagPhi(0, 0);  
    if (!electrons.product()) clog << "ERROR: cannot find metNoElec because electron collection is not initialized." << endl;
    for (uint ielec = 0; ielec<electrons->size(); ielec++) {  
      string empty = "";  
      double elecPt  = valueLookup(&electrons->at(ielec), "pt",  "", empty);
      double elecPhi = valueLookup(&electrons->at(ielec), "phi", "", empty);
      TVector2 p2ElecTmp;
      p2ElecTmp.SetMagPhi(elecPt, elecPhi);  
      p2Elec += p2ElecTmp;  
    }
    TVector2 p2MetNoElec = p2Met + p2Elec;
    value =  p2MetNoElec.Mod();
  }
  else if(variable == "deltaPhiMin2Jets") {
    // minimum |deltaPhi| between met vector and either of the two leading jets 
    // only consider jets with pt > 45 GeV and |eta|<2.8  
    if (!jets.product()) clog << "ERROR:  cannot find deltaPhiMin2Jets because jets collection is not initialized." << endl;
    double ptJet1 = -99; // leading jet
    double ptJet2 = -99; // 2nd jet 
    double phiJet1 = -99;
    double phiJet2 = -99;  
    if (verbose_>4) clog << "Debug:  calculating deltaPhiMin2Jets" << endl;  
    for (uint ijet = 0; ijet<jets->size(); ijet++) {
      // find indices of candidate jets 
      string empty = "";
      double jetPt  = valueLookup(&jets->at(ijet), "pt", "", empty); 
      double jetEta = valueLookup(&jets->at(ijet), "eta", "", empty); 
      double jetPhi = valueLookup(&jets->at(ijet), "phi", "", empty); 
      if (jetPt < 45 ||
	  fabs(jetEta) > 2.8) continue;  
      if (verbose_>4) clog << "  Found jet with pt=" << jetPt << ", eta=" << jetEta << ", phi=" << jetPhi << endl;  
      if        (jetPt > ptJet1) { 
	ptJet2 = ptJet1; 
	ptJet1 = jetPt;  
	phiJet2 = phiJet1;  
	phiJet1 = jetPhi;  
      } else if (jetPt > ptJet2) {
	ptJet2 = jetPt;  
	phiJet2 = jetPhi;  
      }
    }

    // find minimum deltaPhi
    double deltaPhiMin2Jets = 99.;  
    if (ptJet1 >=0) {
      double dPhi = fabs(deltaPhi (phiJet1, object->phi));  
      if (dPhi<deltaPhiMin2Jets) deltaPhiMin2Jets = dPhi;  
    }
    if (ptJet2 >=0) {
      double dPhi = fabs(deltaPhi (phiJet2, object->phi));  
      if (dPhi<deltaPhiMin2Jets) deltaPhiMin2Jets = dPhi;  
    }
    value = deltaPhiMin2Jets;  
    if (verbose_>4) clog  << "  Met phi==" << object->phi << endl;  
    if (verbose_>4) clog << "  Calculated value:  deltaPhiMin2Jets=" << deltaPhiMin2Jets << endl;  
  }

  else if(variable == "deltaPhiJet1") {
    // |deltaPhi| between met vector and leading jet 
    // only consider jets with pt > 45 GeV and |eta|<2.8  
    if (!jets.product()) clog << "ERROR:  cannot find deltaPhiJet1 because jets collection is not initialized." << endl;
    double ptJet1 = -99; // leading jet
    double phiJet1 = -99;
    if (verbose_>4) clog << "Debug:  calculating deltaPhiJet1" << endl;  
    for (uint ijet = 0; ijet<jets->size(); ijet++) {
      // find indices of candidate jets 
      string empty = "";
      double jetPt  = valueLookup(&jets->at(ijet), "pt", "", empty); 
      double jetEta = valueLookup(&jets->at(ijet), "eta", "", empty); 
      double jetPhi = valueLookup(&jets->at(ijet), "phi", "", empty); 
      if (jetPt < 45 ||
	  fabs(jetEta) > 2.8) continue;  
      if (verbose_>4) clog << "  Found jet with pt=" << jetPt << ", eta=" << jetEta << ", phi=" << jetPhi << endl;  
      if (jetPt > ptJet1) { 
	ptJet1 = jetPt;  
	phiJet1 = jetPhi;  
      }
    }

    double deltaPhiJet1 = 99.; 
    if (ptJet1 >=0) {
      double dPhi = fabs(deltaPhi (phiJet1, object->phi));  
      deltaPhiJet1 = dPhi;  
    }
    value = deltaPhiJet1;  
  }

  else if(variable == "deltaPhiJet2") {
    // |deltaPhi| between met vector and subleading jet 
    // only consider jets with pt > 45 GeV and |eta|<2.8  
    if (!jets.product()) clog << "ERROR:  cannot find deltaPhiJet2 because jets collection is not initialized." << endl;
    double ptJet1 = -99; // leading jet
    double ptJet2 = -99; // 2nd jet 
    double phiJet1 = -99;
    double phiJet2 = -99;  
    if (verbose_>4) clog << "Debug:  calculating deltaPhiMin2Jets" << endl;  
    for (uint ijet = 0; ijet<jets->size(); ijet++) {
      // find indices of candidate jets 
      string empty = "";
      double jetPt  = valueLookup(&jets->at(ijet), "pt", "", empty); 
      double jetEta = valueLookup(&jets->at(ijet), "eta", "", empty); 
      double jetPhi = valueLookup(&jets->at(ijet), "phi", "", empty); 
      if (jetPt < 45 ||
	  fabs(jetEta) > 2.8) continue;  
      if (verbose_>4) clog << "  Found jet with pt=" << jetPt << ", eta=" << jetEta << ", phi=" << jetPhi << endl;  
      if        (jetPt > ptJet1) { 
	ptJet2 = ptJet1; 
	ptJet1 = jetPt;  
	phiJet2 = phiJet1;  
	phiJet1 = jetPhi;  
      } else if (jetPt > ptJet2) {
	ptJet2 = jetPt;  
	phiJet2 = jetPhi;  
      }
    }

    // find minimum deltaPhi
    double deltaPhiJet2 = 99.;  
    if (ptJet2 >=0) {
      double dPhi = fabs(deltaPhi (phiJet2, object->phi));  
      deltaPhiJet2 = dPhi;  
    }
    value = deltaPhiJet2;  
  }

  else{clog << "WARNING: invalid met variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
} // end met valueLookup


//!track valueLookup
double
OSUAnalysis::valueLookup (const BNtrack* object, string variable, string function, string &stringValue){

  double value = 0.0;
  double pMag = sqrt(object->pt * object->pt +
                     object->pz * object->pz);

  if(variable == "pt") value = object->pt;
  else if(variable == "px") value = object->px;
  else if(variable == "py") value = object->py;
  else if(variable == "pz") value = object->pz;
  else if(variable == "phi") value = object->phi;
  else if(variable == "eta") value = object->eta;
  else if(variable == "detectorEta") value = getDetectorEta(object->eta, object->dZ);
  else if(variable == "detectorEtaMinusEta") value = getDetectorEta(object->eta, object->dZ) - object->eta;
  else if(variable == "theta") value = object->theta;
  else if(variable == "normChi2") value = object->normChi2;
  else if(variable == "dZ") value = object->dZ;
  else if(variable == "d0") value = object->d0;
  else if(variable == "d0err") value = object->d0err;
  else if(variable == "vx") value = object->vx;
  else if(variable == "vy") value = object->vy;
  else if(variable == "vz") value = object->vz;
  else if(variable == "charge") value = object->charge;
  else if(variable == "numValidHits") value = object->numValidHits;
  else if(variable == "isHighPurity") value = object->isHighPurity;

  //additional BNs info for disappTrks
  else if(variable == "caloEMDeltaRp3")     value = object->caloEMDeltaRp3;
  else if(variable == "caloHadDeltaRp3")    value = object->caloHadDeltaRp3;
  else if(variable == "caloEMDeltaRp4")     value = object->caloEMDeltaRp4;
  else if(variable == "caloHadDeltaRp4")    value = object->caloHadDeltaRp4;
  else if(variable == "caloEMDeltaRp5")     value = object->caloEMDeltaRp5;
  else if(variable == "caloHadDeltaRp5")    value = object->caloHadDeltaRp5;
  else if (!NmissoutShiftUp_ && !NmissoutShiftDown_ && variable == "nHitsMissin\
gOuter"){
    value = object->nHitsMissingOuter;
  }
  else if (NmissoutShiftUp_ && variable == "nHitsMissingOuter") {
    double r;
    double d = -99;
    d = object->nHitsMissingOuter;
    r = ((double) rand() / (RAND_MAX));
    if (r > 0.11) value = d;
    else {
      value = d += 1;
    }
  }
  else if (NmissoutShiftDown_ && variable == "nHitsMissingOuter") {
    double r;
    double d = -99;
    d = object->nHitsMissingOuter;
    r = ((double) rand() / (RAND_MAX));
    if (r > 0.11) value = d;
    else {
      if (d != 0) value = d+= -1;
    }
  }

  else if(variable == "nHitsMissingInner")  value = object->nHitsMissingInner;
  else if(variable == "nHitsMissingMiddle") value = object->nHitsMissingMiddle;
  else if(variable == "depTrkRp3")          value = object->depTrkRp3;
  else if(variable == "trkRelIsoRp3")     { value = (object->depTrkRp3 - object->pt) / object->pt; if (value<0) value = 0; }
  else if(variable == "trkRelIsoRp5")     { value = (object->depTrkRp5 - object->pt) / object->pt; if (value<0) value = 0; }
  else if(variable == "depEcalRp3")         value = object->depEcalRp3;
  else if(variable == "depHcalRp3")         value = object->depHcalRp3;
  else if(variable == "depHoRp3")           value = object->depHoRp3;
  else if(variable == "nTracksRp3")         value = object->nTracksRp3;
  else if(variable == "trackerVetoPtRp3")   value = object->trackerVetoPtRp3;
  else if(variable == "emVetoEtRp3")        value = object->emVetoEtRp3;
  else if(variable == "hadVetoEtRp3")       value = object->hadVetoEtRp3;
  else if(variable == "hoVetoEtRp3")        value = object->hoVetoEtRp3;
  else if(variable == "depTrkRp5")          value = object->depTrkRp5;
  else if(variable == "depEcalRp5")         value = object->depEcalRp5;
  else if(variable == "depHcalRp5")         value = object->depHcalRp5;
  else if(variable == "depHoRp5")           value = object->depHoRp5;
  else if(variable == "nTracksRp5")         value = object->nTracksRp5;
  else if(variable == "trackerVetoPtRp5")   value = object->trackerVetoPtRp5;
  else if(variable == "emVetoEtRp5")        value = object->emVetoEtRp5;
  else if(variable == "hadVetoEtRp5")       value = object->hadVetoEtRp5;
  else if(variable == "hoVetoEtRp5")        value = object->hoVetoEtRp5;

  //user defined variables
  else if(variable == "d0wrtBS") value = (object->vx-events->at(0).BSx)*object->py/object->pt - (object->vy-events->at(0).BSy)*object->px/object->pt;
  else if(variable == "dZwrtBS") value = object->dZ - events->at(0).BSz;
  else if(variable == "depTrkRp5MinusPt"){
    if ( (object->depTrkRp5 - object->pt) < 0 ) {
//       clog << "Warning:  found track with depTrkRp5 < pt:  depTrkRp5=" << object->depTrkRp5
//            << "; pt=" << object->pt
//            << "; object->depTrkRp5 - object->pt = " << object->depTrkRp5 - object->pt
//            << endl;
           value = 0;
         }
         else value =  (object->depTrkRp5 - object->pt);
  }
  else if(variable == "depTrkRp3MinusPt"){
    if ( (object->depTrkRp3 - object->pt) < 0 ) {
      value = 0;
    }
    else value =  (object->depTrkRp3 - object->pt);
  }

  else if(variable == "dPhiMet") {
    if (const BNmet *met = chosenMET ()) {
      value = deltaPhi (object->phi, met->phi);
    } else value = -999;
  }

  else if (variable == "rhoCorrRp5")                 value = getRhoCorr(object);
  else if(variable == "caloTotDeltaRp5")            value =  (object->caloHadDeltaRp5 + object->caloEMDeltaRp5);
  else if(variable == "caloTotDeltaRp5ByP")         value = ((object->caloHadDeltaRp5 + object->caloEMDeltaRp5)/pMag);
  else if(variable == "caloTotDeltaRp5RhoCorr")     value = getTrkCaloTotRhoCorr(object);
  else if(variable == "caloTotDeltaRp5ByPRhoCorr")  value = getTrkCaloTotRhoCorr(object) / pMag;
  else if(variable == "depTrkRp5RhoCorr")           value = getTrkDepTrkRp5RhoCorr(object);
  else if(variable == "depTrkRp3RhoCorr")           value = getTrkDepTrkRp3RhoCorr(object);

  else if(variable == "depTrkRp5MinusPtRhoCorr")    {
    if ( (getTrkDepTrkRp5RhoCorr(object) - object->pt) < 0 ) value = 0;
    else {value = (getTrkDepTrkRp5RhoCorr(object) - object->pt );}
  }

  else if(variable == "depTrkRp3MinusPtRhoCorr")
    {
      if ( (getTrkDepTrkRp3RhoCorr(object) - object->pt) < 0 ) value = 0;
      else {value = (getTrkDepTrkRp3RhoCorr(object) - object->pt );}
    }

  else if(variable == "isIso")                      value = getTrkIsIso(object, tracks.product());
  else if(variable == "isMatchedDeadEcal")          value = getTrkIsMatchedDeadEcal(object);
  else if(variable == "isMatchedDeadEcalDet")          value = getTrkIsMatchedDeadEcalDet(object);
    else if(variable == "trkDeadEcalDeltaR")          value = getTrkDeadEcalDeltaR(object);
  else if(variable == "isMatchedBadCSC")            value = getTrkIsMatchedBadCSC  (object);
  else if(variable == "ptErrorByPt")                value = (object->ptError/object->pt);
  else if(variable == "ptError")                    value = object->ptError;
  else if(variable == "ptRes")                      value = getTrkPtRes(object);
  else if(variable == "ptTrue")                     value = getTrkPtTrue(object, mcparticles.product());
  else if (variable == "d0wrtPV"){
    double vx = object->vx - chosenVertex ()->x,
      vy = object->vy - chosenVertex ()->y,
      px = object->px,
      py = object->py,
      pt = object->pt;
    value = (-vx * py + vy * px) / pt;
  }
  else if (variable == "dZwrtPV"){
    double vx = object->vx - chosenVertex ()->x,
      vy = object->vy - chosenVertex ()->y,
      vz = object->vz - chosenVertex ()->z,
      px = object->px,
      py = object->py,
      pz = object->pz,
      pt = object->pt;
    value = vz - (vx * px + vy * py)/pt * (pz/pt);
  }
  else if (variable == "dZSinTheta"){
    string empty = ""; 
    double dZwrtPV = valueLookup(object, "dZwrtPV", "", empty);
    value = dZwrtPV * (object->pt / pMag);  // sin(theta) = pt / p
  }

  else if(variable == "deltaRMinSubLeadJet") {
    // calculate minimum deltaR between track and any other subleading jet
    double trkJetDeltaRMin = 99.;
    if (!jets.product()) clog << "ERROR:  cannot find deltaRMinSubLeadJet because jets collection is not initialized." << endl;
    for (uint ijet = 0; ijet<jets->size(); ijet++) {
      string empty = "";
      double isSubLeadingJet = valueLookup(&jets->at(ijet), "disappTrkSubLeadingJetID", "", empty);
      if (!isSubLeadingJet) continue;  // only consider jets that pass the subleading jet ID criteria
      double jetEta = valueLookup(&jets->at(ijet), "eta", "", empty);
      double jetPhi = valueLookup(&jets->at(ijet), "phi", "", empty);
      double trkJetDeltaR = deltaR(object->eta, object->phi, jetEta, jetPhi);
      if (trkJetDeltaR < trkJetDeltaRMin) trkJetDeltaRMin = trkJetDeltaR;
    }
    value = trkJetDeltaRMin;
  }

  else if(variable == "deltaPhiMaxSubLeadJet") {
    // calculate maximum deltaPhi between track and any other subleading jet
    double trkJetDeltaPhiMax = -99.;
    if (!jets.product()) clog << "ERROR:  cannot find deltaPhiMaxSubLeadJet because jets collection is not initialized." << endl;
    for (uint ijet = 0; ijet<jets->size(); ijet++) {
      string empty = "";
      double isSubLeadingJet = valueLookup(&jets->at(ijet), "disappTrkSubLeadingJetID", "", empty);
      if (!isSubLeadingJet) continue;  // only consider jets that pass the subleading jet ID criteria
      //      double jetEta = valueLookup(&jets->at(ijet), "eta", "", empty);
      double jetPhi = valueLookup(&jets->at(ijet), "phi", "", empty);
      double trkJetDeltaPhi = fabs(deltaPhi(object->phi, jetPhi));
      if (trkJetDeltaPhi > trkJetDeltaPhiMax) trkJetDeltaPhiMax = trkJetDeltaPhi;  
    }
    value = trkJetDeltaPhiMax;
  }

  else if(variable == "deltaRMinElecLooseMvaId") {
    // calculate minimum deltaR between track and any other electron passing loose mva ID, i.e., mvaNonTrigV0 > 0
    double trkElecDeltaRMin = 99.;
    if (!electrons.product()) clog << "ERROR:  cannot find deltaRMinElecLooseMvaId because electrons collection is not initialized." << endl;
    for (uint ielec = 0; ielec<electrons->size(); ielec++) {
      string empty = "";
      double mvaNonTrigV0 = valueLookup(&electrons->at(ielec), "mvaNonTrigV0", "", empty);
      if (mvaNonTrigV0 < 0) continue;  // only consider electrons with mvaNonTrigV0 > 0
      double elecEta = valueLookup(&electrons->at(ielec), "eta", "", empty);
      double elecPhi = valueLookup(&electrons->at(ielec), "phi", "", empty);
      double trkElecDeltaR = deltaR(object->eta, object->phi, elecEta, elecPhi);
      if (trkElecDeltaR < trkElecDeltaRMin) trkElecDeltaRMin = trkElecDeltaR;
    }
    value = trkElecDeltaRMin;
  }

  else if(variable == "isPassMuonLooseID") {
    // boolean for whether track is loosely identified with a muon, 
    // i.e., true if it is DeltaR-matched to a member of either of the muon or secondary muon collections
    string empty = "";
    double trkMuonDeltaRMin    = valueLookup(object, "deltaRMinMuonLooseId",    "", empty);
    double trkSecMuonDeltaRMin = valueLookup(object, "deltaRMinSecMuonLooseId", "", empty); 
    value = 0;                                  // initialize to be false 
    if (trkMuonDeltaRMin    < 0.15) value = 1;  // true if matched to muon 
    if (trkSecMuonDeltaRMin < 0.15) value = 1;  // true if matched to secondary muon 
  }

  else if(variable == "deltaRMinMuonLooseId") {
    // calculate minimum deltaR between track and any muon
    double trkMuonDeltaRMin = 99.;
    if (!muons.product()) clog << "ERROR:  cannot find deltaRMinMuonLooseId because muons collection is not initialized." << endl;
    for (uint imuon = 0; imuon<muons->size(); imuon++) {
      string empty = "";
      double muonEta = valueLookup(&muons->at(imuon), "eta", "", empty);
      double muonPhi = valueLookup(&muons->at(imuon), "phi", "", empty);
      double trkMuonDeltaR = deltaR(object->eta, object->phi, muonEta, muonPhi);
      if (trkMuonDeltaR < trkMuonDeltaRMin) trkMuonDeltaRMin = trkMuonDeltaR;
    }
    value = trkMuonDeltaRMin;
  }
  else if(variable == "deltaRMinSecMuonLooseId") {
    // calculate minimum deltaR between track and any secondary muon                                       
    double trkMuonDeltaRMin = 99.;
    if (!secMuons.product()) clog << "ERROR:  cannot find deltaRMinSecMuonLooseId because secMuons collection is not initialized." << endl;
    for (uint imuon = 0; imuon<secMuons->size(); imuon++) {
      string empty = "";
      double muonEta = valueLookup(&secMuons->at(imuon), "eta", "", empty);
      double muonPhi = valueLookup(&secMuons->at(imuon), "phi", "", empty);
      double trkMuonDeltaR = deltaR(object->eta, object->phi, muonEta, muonPhi);
      if (trkMuonDeltaR < trkMuonDeltaRMin) trkMuonDeltaRMin = trkMuonDeltaR;
    }
    value = trkMuonDeltaRMin;
  }

  else if(variable == "deltaRMinSecMuonLooseIdGlobal") {
    // calculate minimum deltaR between track and any other loose-Id muon
    double trkMuonDeltaRMin = 99.;
    if (!secMuons.product()) clog << "ERROR:  cannot find deltaRMinSecMuonLooseIdGlobal because secMuons collection is not initialized." << endl;
    for (uint imuon = 0; imuon<secMuons->size(); imuon++) {
      string empty = "";
      double isLooseIdMuon = valueLookup(&secMuons->at(imuon), "looseIDGlobalMuon", "", empty);
      if (!isLooseIdMuon) continue;  // only consider muons that pass the looseIDGlobal criteria
      double muonEta = valueLookup(&secMuons->at(imuon), "eta", "", empty);
      double muonPhi = valueLookup(&secMuons->at(imuon), "phi", "", empty);
      double trkMuonDeltaR = deltaR(object->eta, object->phi, muonEta, muonPhi);
      if (trkMuonDeltaR < trkMuonDeltaRMin) trkMuonDeltaRMin = trkMuonDeltaR;
    }
    value = trkMuonDeltaRMin;
  }

  else if(variable == "deltaRMinTauLooseHadronicId") {
    // calculate minimum deltaR between track and any other tau passing loose hadronic ID
    double trkTauDeltaRMin = 99.;
    if (!taus.product()) clog << "ERROR:  cannot find deltaRMinTauLooseHadronicId because taus collection is not initialized." << endl;
    for (uint itau = 0; itau<taus->size(); itau++) {
      string empty = "";
      double isIdTau = valueLookup(&taus->at(itau), "looseHadronicID", "", empty);
      if (!isIdTau) continue;  // only consider taus that pass the loose hadronic ID criteria
      double tauEta = valueLookup(&taus->at(itau), "eta", "", empty);
      double tauPhi = valueLookup(&taus->at(itau), "phi", "", empty);
      double trkTauDeltaR = deltaR(object->eta, object->phi, tauEta, tauPhi);
      if (trkTauDeltaR < trkTauDeltaRMin) trkTauDeltaRMin = trkTauDeltaR;
    }
    value = trkTauDeltaRMin;
  }

  else if(variable == "deltaPhiMuMuPair") {
    // delta phi between track and dimuon pair   
    // Set to dummy value if there are not 2 muons with pT>25, opposite sign, and 80<m(mu-mu)<100 GeV.  
    string empty = "";  

    TLorentzVector p4muon0;  // first muon                                                                                                                                                             
    TLorentzVector p4muon1;  // second muon                                                                                                                                                            
    TLorentzVector p4mumu;   // dimuon pair                                                                                                                                                            
    double mMuon = 0.105658;  // mass from PDG
    int imuon0 = -1;
    int imuon1 = -1;  
    double phiMuMu = 0;  
    for (uint imuon = 0; imuon<muons->size(); imuon++) {
      double pt0   = valueLookup(&muons->at(imuon), "pt",     "", empty); 
      double eta0  = valueLookup(&muons->at(imuon), "eta",    "", empty); 
      double phi0  = valueLookup(&muons->at(imuon), "phi",    "", empty); 
      double chg0  = valueLookup(&muons->at(imuon), "charge", "", empty); 
      if (pt0<25) continue; 
      for (uint jmuon = imuon+1; jmuon<muons->size(); jmuon++) {
	double pt1   = valueLookup(&muons->at(jmuon), "pt",      "", empty); 
	double eta1  = valueLookup(&muons->at(jmuon), "eta",     "", empty); 
	double phi1  = valueLookup(&muons->at(jmuon), "phi",     "", empty); 
	double chg1  = valueLookup(&muons->at(jmuon), "charge",  "", empty); 
	if (pt1<25)          continue; 
	if (chg0*chg1 != -1) continue;
	p4muon0.SetPtEtaPhiM(pt0, eta0, phi0, mMuon); 
	p4muon1.SetPtEtaPhiM(pt1, eta1, phi1, mMuon); 
	p4mumu = p4muon0 + p4muon1;
	if (p4mumu.M()<80 || 100<p4mumu.M()) continue;  
	// Now two muons have passed the required criteria.  
	if (imuon0>=0 || imuon1>=0) {
	  clog << "Warning [OSUAnalysis::valueLookup()]: More than one dimuon pair passes criteria in deltaPhiMuMuPair calculation." << endl;    
	}
	imuon0 = imuon;
	imuon1 = jmuon;  
	phiMuMu = p4mumu.Phi();  
      }
    }

    value = -99;                            // initialize to dummy value  
    if (imuon0>=0 && imuon1>=0) value = deltaPhi(phiMuMu, object->phi);  // only set the value if two muons pass the given criteria  

  }

  else if(variable == "isNotSigOrMatchedToSusy") {
    // Return true if the input dataset is not signal MC or if the track is matched to a SUSY particle.  
    if (datasetType_ != "signalMC") { value = 1; } else {
      int index = getGenMatchedParticleIndex(object);
      if (index == -1) value = 0;  // not matched 
      else { 
	int pdgId = abs(mcparticles->at(index).id);  
	if (pdgId>1000001 && pdgId<3160113) value = 1;  // matched to a SUSY particle  
	else                                value = 0;  
      }        
    }  
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

  else { 
    //  else{clog << "WARNING: invalid track variable '" << variable << "'\n"; value = -999;}
    // Allow looking up event-level quantities.  
    // FIXME:  Check for whether "variable" is valid for valueLookup(&events->at(0)...).  
    value = valueLookup(&events->at(0), variable, function, stringValue); 
  }

  value = applyFunction(function, value);

  return value;
} // end track valueLookup


//!genjet valueLookup
double
OSUAnalysis::valueLookup (const BNgenjet* object, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "pt") value = object->pt;
  else if(variable == "eta") value = object->eta;
  else if(variable == "phi") value = object->phi;
  else if(variable == "px") value = object->px;
  else if(variable == "py") value = object->py;
  else if(variable == "pz") value = object->pz;
  else if(variable == "et") value = object->et;
  else if(variable == "energy") value = object->energy;
  else if(variable == "mass") value = object->mass;
  else if(variable == "emEnergy") value = object->emEnergy;
  else if(variable == "hadEnergy") value = object->hadEnergy;
  else if(variable == "invisibleEnergy") value = object->invisibleEnergy;
  else if(variable == "auxiliaryEnergy") value = object->auxiliaryEnergy;
  else if(variable == "charge") value = object->charge;


  else{clog << "WARNING: invalid genjet variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
}

//!mcparticle valueLookup
double
OSUAnalysis::valueLookup (const BNmcparticle* object, string variable, string function, string &stringValue){

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

//!primaryvertex valueLookup
double
OSUAnalysis::valueLookup (const BNprimaryvertex* object, string variable, string function, string &stringValue){

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


  else{clog << "WARNING: invalid primaryvertex variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
}

//!bxlumi valueLookup
double
OSUAnalysis::valueLookup (const BNbxlumi* object, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "bx_B1_now") value = object->bx_B1_now;
  else if(variable == "bx_B2_now") value = object->bx_B2_now;
  else if(variable == "bx_LUMI_now") value = object->bx_LUMI_now;


  else{clog << "WARNING: invalid bxlumi variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
}

//!photon valueLookup
double
OSUAnalysis::valueLookup (const BNphoton* object, string variable, string function, string &stringValue){

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

  value = applyFunction(function, value);

  return value;
} // end photon valueLookup


//!supercluster valueLookup
double
OSUAnalysis::valueLookup (const BNsupercluster* object, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "energy") value = object->energy;
  else if(variable == "et") value = object->et;
  else if(variable == "ex") value = object->ex;
  else if(variable == "ey") value = object->ey;
  else if(variable == "ez") value = object->ez;
  else if(variable == "phi") value = object->phi;
  else if(variable == "eta") value = object->eta;
  else if(variable == "theta") value = object->theta;


  else{clog << "WARNING: invalid supercluster variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
}

//!trigobj valueLookup
double
OSUAnalysis::valueLookup (const BNtrigobj* object, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "pt") value = object->pt;
  else if(variable == "ptWithForwardJets") value = (object->pt + getSumForwardJetPt(jets.product()));
  else if(variable == "eta") value = object->eta;
  else if(variable == "phi") value = object->phi;
  else if(variable == "px") value = object->px;
  else if(variable == "py") value = object->py;
  else if(variable == "pz") value = object->pz;
  else if(variable == "et") value = object->et;
  else if(variable == "energy") value = object->energy;
  else if(variable == "etTotal") value = object->etTotal;
  else if(variable == "id") value = object->id;
  else if(variable == "charge") value = object->charge;
  else if(variable == "isIsolated") value = object->isIsolated;
  else if(variable == "isMip") value = object->isMip;
  else if(variable == "isForward") value = object->isForward;
  else if(variable == "isRPC") value = object->isRPC;
  else if(variable == "bx") value = object->bx;
  else if(variable == "metPt") {  // allow making 2D plots of jet quantities vs. Met                                          
    if (const BNmet *met = chosenMET ()) {
      value = met->pt;
    } else value = -999;
  }
  else if(variable == "trackPt") {  // allow making 2D plots of jet quantities vs. Met                                                              
    if (const BNtrack *track = chosenTrack ()) {
      value = track->pt;
    } else value = -999;
  }
  else if(variable == "jetPt") {  // allow making 2D plots of jet quantities vs. Met                                                              
    if (const BNjet *jet = chosenJet ()) {
      value = jet->pt;
    } else value = -999;
  }
  else if(variable == "filter") {
    if ((stringValue = object->filter) == "")
      stringValue = "none";  // stringValue should only be empty if value is filled
  }


  else{clog << "WARNING: invalid trigobj variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
}

//!muon-muon pair valueLookup
double
OSUAnalysis::valueLookup (const BNmuon* object1, const BNmuon* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "deltaPt") value = fabs(object1->pt - object2->pt);
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "alpha")
    {
      static const double pi = 3.1415926535897932384626433832795028841971693993751058;
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (pi-threeVector1.Angle(threeVector2));
    }
  else if(variable == "deltaCorrectedD0Vertex") value = object1->correctedD0Vertex - object2->correctedD0Vertex;
  else if(variable == "deltaAbsCorrectedD0Vertex") value = fabs(object1->correctedD0Vertex) - fabs(object2->correctedD0Vertex);
  else if(variable == "d0Sign"){
    double d0Sign = (object1->correctedD0Vertex*object2->correctedD0Vertex)/fabs(object1->correctedD0Vertex*object2->correctedD0Vertex);
    if(d0Sign < 0) value = -0.5;
    else if (d0Sign > 0) value = 0.5;
    else value = -999;
  }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }
  else if(variable == "muon1CorrectedD0Vertex"){
    value = object1->correctedD0Vertex;
  }
  else if(variable == "muon2CorrectedD0Vertex"){
    value = object2->correctedD0Vertex;
  }
  else if(variable == "muon1timeAtIpInOut"){
    value = object1->timeAtIpInOut;
  }
  else if(variable == "muon2timeAtIpInOut"){
    value = object2->timeAtIpInOut;
  }
  else if(variable == "muon1EcalTime"){ value = object1->ecal_time;
  }
  else if(variable == "muon2EcalTime"){ value = object2->ecal_time;
  }
  else if(variable == "muon1HcalTime"){ value = object1->hcal_time;
  }
  else if(variable == "muon2HcalTime"){ value = object2->hcal_time;
  }
  else if(variable == "muon1CorrectedD0")
    {
      value = object1->correctedD0;
    }
  else if(variable == "muon2CorrectedD0")
    {
      value = object2->correctedD0;
    }

  else{clog << "WARNING: invalid muon-muon pair variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
} // end muon-muon pair valueLookup


//!muon-photon pair valueLookup
double
OSUAnalysis::valueLookup (const BNmuon* object1, const BNphoton* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "photonEta") value = object2->eta;
  else if(variable == "photonPt") value = object2->pt;
  else if(variable == "muonEta") value = object1->eta;
  else if(variable == "photonPhi") value = object2->phi;
  else if(variable == "muonPhi") value = object1->phi;
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "photonGenMotherId") value = object2->genMotherId;
  else if(variable == "muonRelPFdBetaIso") value = (object1->pfIsoR04SumChargedHadronPt + max(0.0, object1->pfIsoR04SumNeutralHadronEt + object1->pfIsoR04SumPhotonEt - 0.5*object1->pfIsoR04SumPUPt)) / object1->pt;
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "alpha")
    {
      static const double pi = 3.1415926535897932384626433832795028841971693993751058;
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (pi-threeVector1.Angle(threeVector2));
    }
  else{clog << "WARNING: invalid muon-photon pair variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
} // end muon-photon pair valueLookup


//!electron-photon pair valueLookup
double
OSUAnalysis::valueLookup (const BNelectron* object1, const BNphoton* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "photonEta") value = object2->eta;
  else if(variable == "photonPt") value = object2->pt;
  else if(variable == "electronEta") value = object1->eta;
  else if(variable == "photonPhi") value = object2->phi;
  else if(variable == "electronPhi") value = object1->phi;
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "photonGenMotherId") value = object2->genMotherId;
  else if(variable == "electronRelPFrhoIso") value = ( object1->chargedHadronIsoDR03 + max(0.0, object1->neutralHadronIsoDR03 + object1->photonIsoDR03 - object1->AEffDr03*object1->rhoPrime) ) / object1->pt;
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "alpha")
    {
      static const double pi = 3.1415926535897932384626433832795028841971693993751058;
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (pi-threeVector1.Angle(threeVector2));
    }
  else{clog << "WARNING: invalid electron-photon pair variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
} // end electron-photon pair valueLookup


//!electron-electron pair valueLookup
double
OSUAnalysis::valueLookup (const BNelectron* object1, const BNelectron* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "deltaCorrectedD0Vertex") value = object1->correctedD0Vertex - object2->correctedD0Vertex;
  else if(variable == "deltaAbsCorrectedD0Vertex") value = fabs(object1->correctedD0Vertex) - fabs(object2->correctedD0Vertex);
  else if(variable == "d0Sign"){
    double d0Sign = (object1->correctedD0Vertex*object2->correctedD0Vertex)/fabs(object1->correctedD0Vertex*object2->correctedD0Vertex);
    if(d0Sign < 0) value = -0.5;
    else if (d0Sign > 0) value = 0.5;
    else value = -999;
  }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }
  else if(variable == "electron1CorrectedD0Vertex"){
    value = object1->correctedD0Vertex;
  }
  else if(variable == "electron2CorrectedD0Vertex"){
    value = object2->correctedD0Vertex;
  }
  else if(variable == "electron1CorrectedD0"){
    value = object1->correctedD0;
  }
  else if(variable == "electron2CorrectedD0"){
    value = object2->correctedD0;
  }

  else{clog << "WARNING: invalid electron-electron pair variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
} // end electron-electron pair valueLookup


//!electron-muon pair valueLookup
double
OSUAnalysis::valueLookup (const BNelectron* object1, const BNmuon* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "deltaCorrectedD0Vertex") value = object1->correctedD0Vertex - object2->correctedD0Vertex;
  else if(variable == "deltaAbsCorrectedD0Vertex") value = fabs(object1->correctedD0Vertex) - fabs(object2->correctedD0Vertex);
  else if(variable == "d0Sign"){
    double d0Sign = (object1->correctedD0Vertex*object2->correctedD0Vertex)/fabs(object1->correctedD0Vertex*object2->correctedD0Vertex);
    if(d0Sign < 0) value = -0.5;
    else if (d0Sign > 0) value = 0.5;
    else value = -999;
  }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }
  else if(variable == "electronCorrectedD0Vertex"){
    value = object1->correctedD0Vertex;
  }
  else if(variable == "muonCorrectedD0Vertex"){
    value = object2->correctedD0Vertex;
  }
  else if(variable == "electronCorrectedD0"){
    value = object1->correctedD0;
  }
  else if(variable == "muonCorrectedD0"){
    value = object2->correctedD0;
  }
  else if(variable == "electronCorrectedD0Err") value =  hypot (object1->tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
  else if(variable == "electronCorrectedD0Sig") value =  object1->correctedD0 / hypot (object1->tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
  else if(variable == "muonCorrectedD0Err") value =  hypot (object2->tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
  else if(variable == "muonCorrectedD0Sig") value =  object2->correctedD0 / hypot (object2->tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
  else if(variable == "electronDetIso"){
    value = (object1->trackIso) / object1->pt;
  }
  else if(variable == "muonDetIso"){
    value = (object2->trackIsoDR03) / object2->pt;
  }
  else if(variable == "electronRelPFrhoIso"){
    value = ( object1->chargedHadronIsoDR03 + max(0.0, object1->neutralHadronIsoDR03 + object1->photonIsoDR03 - object1->AEffDr03*object1->rhoPrime) ) / object1->pt;
  }
  else if(variable == "muonRelPFdBetaIso"){
    value = (object2->pfIsoR04SumChargedHadronPt + max(0.0, object2->pfIsoR04SumNeutralHadronEt + object2->pfIsoR04SumPhotonEt - 0.5*object2->pfIsoR04SumPUPt)) / object2->pt;
  }
  else if(variable == "electronPt"){
    value = object1->pt;
  }
  else if(variable == "muonPt"){
    value = object2->pt;
  }
  else if(variable == "electronEta"){
    value = object1->eta;
  }
  else if(variable == "muonEta"){
    value = object2->eta;
  }
  else if(variable == "electronMetMT"){
    string dummy = "";
    value = valueLookup(object1,string("metMT"),string(""),dummy);
  }
  else if(variable == "muonMetMT"){
    string dummy = "";
    value = valueLookup(object2,string("metMT"),string(""),dummy);
  }
  else if(variable == "dz"){
    // based on definition of "dz" in DataFormats/TrackReco/interface/TrackBase.h
    value = object1->vz - object2->vz;
    value -= (object1->vx * object1->px + object1->vy * object1->py) / object1->pt * (object1->pz / object1->pt);
    value += (object2->vx * object2->px + object2->vy * object2->py) / object2->pt * (object2->pz / object2->pt);
  }
  else if(variable == "dzSig"){
    // based on definition of "dz" in DataFormats/TrackReco/interface/TrackBase.h
    value = object1->vz - object2->vz;
    value -= (object1->vx * object1->px + object1->vy * object1->py) / object1->pt * (object1->pz / object1->pt);
    value += (object2->vx * object2->px + object2->vy * object2->py) / object2->pt * (object2->pz / object2->pt);
    value /= hypot (object1->tkDZerr, object2->tkDZerr);
  }




  else{clog << "WARNING: invalid electron-muon pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;
} // end electron-muon pair valueLookup


//!electron-jet pair valueLookup
double
OSUAnalysis::valueLookup (const BNelectron* object1, const BNjet* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "distance")
   {
      Line::PositionType pos(GlobalPoint(object1->vx, object1->vy, object1->vz));
      Line::DirectionType dir(GlobalVector(object1->px, object1->py, object1->pz).unit());
      Line electron(pos, dir);
      Line::PositionType pos2(GlobalPoint(chosenVertex()->x, chosenVertex()->y, chosenVertex()->z));
      Line::DirectionType dir2(GlobalVector(object2->px, object2->py, object2->pz).unit());
      Line jet(pos2, dir2);
      value = (jet.distance(electron)).mag();
   }
  else if(variable == "jetEta") value = object2->eta;
  else if(variable == "jetPhi") value = object2->phi;
  else if(variable == "jetPt") value = object2->pt;
  else if(variable == "electronEta") value = object1->eta;
  else if(variable == "electronPhi") value = object1->phi;
  else if(variable == "electronPt") value = object1->pt;
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "deltaZ") value = object1->vz - object2->leadCandVz;
  else if(variable == "relPFrhoIso") value = ( object1->chargedHadronIsoDR03 + max(0.0, object1->neutralHadronIsoDR03 + object1->photonIsoDR03 - object1->AEffDr03*object1->rhoPrime) ) / object1->pt;
  else if(variable == "relPFrhoIsoNoPUSbtractedJet") value = (object2->chargedHadronEnergyFraction*object2->pt + max(0.0, object2->neutralHadronEnergyFraction*object2->pt + object2->neutralEmEnergyFraction*object2->pt)) / object1->pt;
  else if(variable == "relPFrhoIsoJet") value = (object2->chargedHadronEnergyFraction*object2->pt + max(0.0, object2->neutralHadronEnergyFraction*object2->pt + object2->neutralEmEnergyFraction*object2->pt - object1->AEffDr03*object1->rhoPrime)) / object1->pt;
  else if(variable == "PUInrelPFrhoIso") value = (object1->AEffDr03*object1->rhoPrime) / object1->pt;
  else if(variable == "ChargedHadInrelPFrhoIsoDiff") value = (object2->chargedHadronEnergyFraction*object2->pt - object1->chargedHadronIsoDR03) / object1->pt;
  else if(variable == "NeutralHadInrelPFrhoIsoDiff") value = (object2->neutralHadronEnergyFraction*object2->pt - object1->neutralHadronIsoDR03) / object1->pt;
  else if(variable == "PhotonInrelPFrhoIsoDiff") value = (object2->neutralEmEnergyFraction*object2->pt - object1->photonIsoDR03) / object1->pt;
  else if(variable == "ChargedHadInrelPFrhoIsoJet") value = (object2->chargedHadronEnergyFraction*object2->pt) / object1->pt;
  else if(variable == "NeutralHadInrelPFrhoIsoJet") value = (object2->neutralHadronEnergyFraction*object2->pt) / object1->pt;
  else if(variable == "PhotonInrelPFrhoIsoJet") value = (object2->neutralEmEnergyFraction*object2->pt) / object1->pt;
  else if(variable == "relPFrhoIsoDiff") value =  ( object1->chargedHadronIsoDR03 + max(0.0, object1->neutralHadronIsoDR03 + object1->photonIsoDR03 - object1->AEffDr03*object1->rhoPrime) ) / object1->pt - (object2->chargedHadronEnergyFraction*object2->pt + max(0.0, object2->neutralHadronEnergyFraction*object2->pt + object2->neutralEmEnergyFraction*object2->pt - object1->AEffDr03*object1->rhoPrime)) / object1->pt;
  else if(variable == "relPFrhoIsoNoPUSubtractedDiff") value = ( object1->chargedHadronIsoDR03 + max(0.0, object1->neutralHadronIsoDR03 + object1->photonIsoDR03) ) / object1->pt - (object2->chargedHadronEnergyFraction*object2->pt + max(0.0, object2->neutralHadronEnergyFraction*object2->pt + object2->neutralEmEnergyFraction*object2->pt)) / object1->pt;
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }
  else if(variable == "ptOfClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNelectron, BNjet> (object1, jets, "jets");
    if (!closestJet) value = -999;
    else value = closestJet->pt;
  }
  else if(variable == "massOfClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNelectron, BNjet> (object1, jets, "jets");
    if (!closestJet) value = -999;
    else value = closestJet->mass;
  }
  else if(variable == "areaOfClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNelectron, BNjet> (object1, jets, "jets");
    if (!closestJet) value = -999;
    else value = closestJet->area;
  }
  else if(variable == "pTransverseToClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNelectron, BNjet> (object1, jets, "jets");
    if (!closestJet) value = -999;
    else{
      TVector3 jetP (closestJet->px, closestJet->py, closestJet->pz),
               leptonP (object1->px, object1->py, object1->pz),
               projection, rejection;
      projection = jetP.Unit ();
      projection *= (leptonP * projection);
      rejection = leptonP - projection;
      value = rejection.Mag ();
    }
  }

  else{clog << "WARNING: invalid electron-jet pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;
}

//!electron-mcparticle pair valueLookup
double
OSUAnalysis::valueLookup (const BNelectron* object1, const BNmcparticle* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "id") value = fabs(object2->id);
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }
  else if(variable == "chargeProductById"){
    value = object1->charge*object2->id;
  }

  else{clog << "WARNING: invalid electron-mcparticle variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;
}

//!muon-mcparticle pair valueLookup
double
OSUAnalysis::valueLookup (const BNmuon* object1, const BNmcparticle* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "id") value = fabs(object2->id);
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }
  else if(variable == "chargeProductById"){
    value = object1->charge*object2->id;
  }

  else{clog << "WARNING: invalid muon-mcparticle variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;
}


//!mcparticle-mcparticle pair valueLookup
double
OSUAnalysis::valueLookup (const BNmcparticle* object1, const BNmcparticle* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }
  else{clog << "WARNING: invalid mcparticle-mcparticle variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;
}


//!jet-mcparticle pair valueLookup
double
OSUAnalysis::valueLookup (const BNjet* object1, const BNmcparticle* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);


  else{clog << "WARNING: invalid jet-mcparticle variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;
}



//!track-mcparticle pair valueLookup
double
OSUAnalysis::valueLookup (const BNtrack* object1, const BNmcparticle* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if     (variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "deltaR")   value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }

  else{clog << "WARNING: invalid electron-mcparticle variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;
}



//!stop-mcparticle pair valueLookup
double
OSUAnalysis::valueLookup (const BNstop* object1, const BNmcparticle* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if     (variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "deltaR")   value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }

  else{clog << "WARNING: invalid stop-mcparticle variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;
}




//!photon-jet pair valueLookup
double
OSUAnalysis::valueLookup (const BNphoton* object1, const BNjet* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "jetEta") value = object2->eta;
  else if(variable == "jetPhi") value = object2->phi;
  else if(variable == "photonEta") value = object1->eta;
  else if(variable == "photonPhi") value = object1->phi;
  else if(variable == "photonGenMotherId") value = object1->genMotherId;
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else{clog << "WARNING: invalid photon-jet pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;
}

// track-jet pair valueLookup
double
OSUAnalysis::valueLookup (const BNtrack* object1, const BNjet* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);

  else{clog << "WARNING: invalid track-jet pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;

}



// met-jet pair valueLookup
double
OSUAnalysis::valueLookup (const BNmet* object1, const BNjet* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));

  else{clog << "WARNING: invalid met-jet pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;

}

double
OSUAnalysis::valueLookup (const BNmet* object1, const BNmcparticle* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else{clog << "WARNING: invalid met-mcparticle pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;

}




//!muon-jet pair valueLookup
double
OSUAnalysis::valueLookup (const BNmuon* object1, const BNjet* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "jetEta") value = object2->eta;
  else if(variable == "distance")
   {
      Line::PositionType pos(GlobalPoint(object1->vx, object1->vy, object1->vz));
      Line::DirectionType dir(GlobalVector(object1->px, object1->py, object1->pz).unit());
      Line muon(pos, dir);
      Line::PositionType pos2(GlobalPoint(chosenVertex()->x, chosenVertex()->y, chosenVertex()->z));
      Line::DirectionType dir2(GlobalVector(object2->px, object2->py, object2->pz).unit());
      Line jet(pos2, dir2);
      value = (jet.distance(muon)).mag();
   }
  else if(variable == "relPFdBetaIso") value = (object1->pfIsoR04SumChargedHadronPt + max(0.0, object1->pfIsoR04SumNeutralHadronEt + object1->pfIsoR04SumPhotonEt - 0.5*object1->pfIsoR04SumPUPt)) / object1->pt;
  else if(variable == "relPFdBetaIsoNoPUSbtractedJet") value = (object2->chargedHadronEnergyFraction*object2->pt + max(0.0, object2->neutralHadronEnergyFraction*object2->pt + object2->neutralEmEnergyFraction*object2->pt)) / object1->pt;
  else if(variable == "relPFdBetaIsoJet") value = (object2->chargedHadronEnergyFraction*object2->pt + max(0.0, object2->neutralHadronEnergyFraction*object2->pt + object2->neutralEmEnergyFraction*object2->pt - 0.5*object1->pfIsoR04SumPUPt)) / object1->pt;
  else if(variable == "PUInrelPFdBetaIso") value = (0.5*object1->pfIsoR04SumPUPt) / object1->pt;
  else if(variable == "ChargedHadInrelPFdBetaIsoDiff") value = (object2->chargedHadronEnergyFraction*object2->pt - object1->pfIsoR04SumChargedHadronPt) / object1->pt;
  else if(variable == "NeutralHadInrelPFdBetaIsoDiff") value = (object2->neutralHadronEnergyFraction*object2->pt - object1->pfIsoR04SumNeutralHadronEt) / object1->pt;
  else if(variable == "PhotonInrelPFdBetaIsoDiff") value = (object2->neutralEmEnergyFraction*object2->pt - object1->pfIsoR04SumPhotonEt) / object1->pt;
  else if(variable == "ChargedHadInrelPFdBetaIsoJet") value = (object2->chargedHadronEnergyFraction*object2->pt) / object1->pt;
  else if(variable == "NeutralHadInrelPFdBetaIsoJet") value = (object2->neutralHadronEnergyFraction*object2->pt) / object1->pt;
  else if(variable == "PhotonInrelPFdBetaIsoJet") value = (object2->neutralEmEnergyFraction*object2->pt) / object1->pt;
  else if(variable == "relPFdBetaIsoDiff") value = (object1->pfIsoR04SumChargedHadronPt + max(0.0, object1->pfIsoR04SumNeutralHadronEt + object1->pfIsoR04SumPhotonEt - 0.5*object1->pfIsoR04SumPUPt))/ object1->pt - (object2->chargedHadronEnergyFraction*object2->pt + max(0.0, object2->neutralHadronEnergyFraction*object2->pt + object2->neutralEmEnergyFraction*object2->pt- 0.5*object1->pfIsoR04SumPUPt)) / object1 -> pt;
  else if(variable == "relPFdBetaIsoNoPUSubtractedDiff") value =(object1->pfIsoR04SumChargedHadronPt + max(0.0, object1->pfIsoR04SumNeutralHadronEt + object1->pfIsoR04SumPhotonEt))/ object1->pt - (object2->chargedHadronEnergyFraction*object2->pt + max(0.0, object2->neutralHadronEnergyFraction*object2->pt + object2->neutralEmEnergyFraction*object2->pt)) / object1->pt;
  else if(variable == "jetPt") value = object2->pt;
  else if(variable == "jetPhi") value = object2->phi;
  else if(variable == "deltaPt") value = object1->pt - object2->pt;
  else if(variable == "muonEta") value = object1->eta;
  else if(variable == "muonPt") value = object1->pt;
  else if(variable == "muonPhi") value = object1->phi;
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "deltaZ") value = object1->vz - object2->leadCandVz;
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }
  else if(variable == "ptOfClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNmuon, BNjet> (object1, jets, "jets");
    if (!closestJet) value = -999;
    else value = closestJet->pt;
  }
  else if(variable == "massOfClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNmuon, BNjet> (object1, jets, "jets");
    if (!closestJet) value = -999;
    else value = closestJet->mass;
  }
  else if(variable == "areaOfClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNmuon, BNjet> (object1, jets, "jets");
    if (!closestJet) value = -999;
    else value = closestJet->area;
  }
  else if(variable == "pTransverseToClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNmuon, BNjet> (object1, jets, "jets");
    if (!closestJet) value = -999;
    else{
      TVector3 jetP (closestJet->px, closestJet->py, closestJet->pz),
               leptonP (object1->px, object1->py, object1->pz),
               projection, rejection;
      projection = jetP.Unit ();
      projection *= (leptonP * projection);
      rejection = leptonP - projection;
      value = rejection.Mag ();
    }
  }

  else{clog << "WARNING: invalid muon-jet pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;
} // end muon-jet pair valueLookup


//!muon-event valueLookup
double
OSUAnalysis::valueLookup (const BNmuon* object1, const BNevent* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "muonEta") value = object1->eta;
  else if(variable == "muonPt") value = object1->pt;
  else if(variable == "muonPhi") value = object1->phi;
  else if(variable == "Ht") value = getHt(jets.product());
  else if(variable == "pthat")   value = object2->pthat;
  else if(variable == "numPV")   value = object2->numPV;
  else if(variable == "correctedD0")   value = object1->correctedD0;
  else if(variable == "relPFdBetaIso") value = (object1->pfIsoR04SumChargedHadronPt + max(0.0, object1->pfIsoR04SumNeutralHadronEt + object1->pfIsoR04SumPhotonEt - 0.5*object1->pfIsoR04SumPUPt)) / object1->pt;
  else{clog << "WARNING: invalid muon-event pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;
}
//!electron-event valueLookup
double
OSUAnalysis::valueLookup (const BNelectron* object1, const BNevent* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if     (variable == "numPV")       value = object2->numPV;
  else if(variable == "correctedD0") value = object1->correctedD0;
  else{clog << "WARNING: invalid electron-event pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;
}
//!jet-jet pair valueLookup
double
OSUAnalysis::valueLookup (const BNjet* object1, const BNjet* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "deltaPt") value = object1->pt - object2->pt;
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }

  else{clog << "WARNING: invalid jet-jet pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;
}

//!electron-track pair valueLookup
double
OSUAnalysis::valueLookup (const BNelectron* object1, const BNtrack* object2, string variable, string function, string &stringValue){
  double electronMass = 0.000511;
  double value = 0.0;
  TLorentzVector fourVector1(0, 0, 0, 0);
  TLorentzVector fourVector2(0, 0, 0, 0);
  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "deltaRLooseMvaId") {
    // only consider electrons that pass "loose MVA id", i.e., mvaNonTrigV0 > 0;
    // otherwise return very large value (99)
    if (object1->mvaNonTrigV0>0) value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
    else                         value = 99.;
  }
  else if(variable == "invMass"){
    fourVector1.SetPtEtaPhiM(object1->pt, object1->eta, object1->phi, electronMass);
    fourVector2.SetPtEtaPhiM(object2->pt, object2->eta, object2->phi, electronMass );

    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }
  else{clog << "WARNING: invalid electron-track pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);
  return value;

}


//!muon-track pair valueLookup
double
OSUAnalysis::valueLookup (const BNmuon* object1, const BNtrack* object2, string variable, string function, string &stringValue){
  double value = 0.0;
  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "alpha")
    {
      static const double pi = 3.1415926535897932384626433832795028841971693993751058;
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (pi-threeVector1.Angle(threeVector2));
    }
  else if(variable == "deltaPt")  value = fabs(object1->pt - object2->pt);  
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "deltaRLooseID") {
    // only consider muons that pass looseId criteria; otherwise return very large value (99.)
    string empty = "";
    double isLooseId = valueLookup(object1, "looseID", "", empty);
    if (isLooseId) value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
    else           value = 99.;
  }
  else if(variable == "deltaRGlobalMuon") {
    // only consider muons that pass looseId criteria; otherwise return very large value (99.)
    if (object1->isGlobalMuon) value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
    else                       value = 99.;
  }
  else if(variable == "invMass"){
    double pionMass = 0.140;
    double muonMass = 0.106;
    TLorentzVector fourVector1(0, 0, 0, 0);
    TLorentzVector fourVector2(0, 0, 0, 0);
    fourVector1.SetPtEtaPhiM(object1->pt, object1->eta, object1->phi, muonMass);
    fourVector2.SetPtEtaPhiM(object2->pt, object2->eta, object2->phi, pionMass );
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }

  else{clog << "WARNING: invalid muon-track pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);
  return value;
}

//!jet-track pair valueLookup
double
OSUAnalysis::valueLookup (const BNjet* object1, const BNtrack* object2, string variable, string function, string &stringValue){
  double value = 0.0;
  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "deltaPt")  value = fabs(object1->pt - object2->pt);  
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else{clog << "WARNING: invalid jet-track pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);
  return value;
}

//!tau-tau pair valueLookup
double
OSUAnalysis::valueLookup (const BNtau* object1, const BNtau* object2, string variable, string function, string &stringValue){
  double value = 0.0;
  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).M();
  }

  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }

  else{clog << "WARNING: invalid tau-tau pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);
  return value;
}

//!muon-tau pair valueLookup
double
OSUAnalysis::valueLookup (const BNmuon* object1, const BNtau* object2, string variable, string function, string &stringValue){
  double value = 0.0;
  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).M();
  }

  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }

  else{clog << "WARNING: invalid muon-tau pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);
  return value;
}

//!tau-track pair valueLookup
double
OSUAnalysis::valueLookup (const BNtau* object1, const BNtrack* object2, string variable, string function, string &stringValue){
  double value = 0.0;
  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "deltaRLooseHadronicID") {
    // only consider tau's that pass the loose hadronic ID criteria; otherwise return very large value (99)
    string empty = "";
    double isLooseHadronicID = valueLookup(object1, "looseHadronicID", "", empty);

    if (isLooseHadronicID) value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
    else                   value = 99.;
  }
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "invMass"){
    double tauMass = 1.777;  // PDG 2010
    double pionMass = 0.140; // PDG 2010
    TLorentzVector fourVector1(0, 0, 0, 0);
    TLorentzVector fourVector2(0, 0, 0, 0);
    fourVector1.SetPtEtaPhiM(object1->pt, object1->eta, object1->phi, tauMass);
    fourVector2.SetPtEtaPhiM(object2->pt, object2->eta, object2->phi, pionMass );
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }

  else{clog << "WARNING: invalid tau-track pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);
  return value;
}


//!track-event pair valueLookup
double
OSUAnalysis::valueLookup (const BNtrack* object1, const BNevent* object2, string variable, string function, string &stringValue){

  double value = 0.0;
  double pMag = sqrt(object1->pt * object1->pt +
                     object1->pz * object1->pz);

  if      (variable == "numPV")                      value = object2->numPV;
  else if (variable == "rhoCorrRp5")                 value = getRhoCorr(object1);
  else if (variable == "caloTotDeltaRp5")            value =  (object1->caloHadDeltaRp5 + object1->caloEMDeltaRp5);
  else if (variable == "caloTotDeltaRp5ByP")         value = ((object1->caloHadDeltaRp5 + object1->caloEMDeltaRp5)/pMag);
  else if (variable == "caloTotDeltaRp5_RhoCorr")    value = getTrkCaloTotRhoCorr(object1);
  else if (variable == "caloTotDeltaRp5ByP_RhoCorr") value = getTrkCaloTotRhoCorr(object1) / pMag;

  else { clog << "WARNING: invalid track-event pair variable '" << variable << "'\n"; value = -999; }

  value = applyFunction(function, value);

  return value;

}

//!electron-trigobj pair valueLookup
double
OSUAnalysis::valueLookup (const BNelectron* object1, const BNtrigobj* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if (variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if (variable == "match"){
    if (deltaR(object1->eta,object1->phi,object2->eta,object2->phi) < 0.2 && abs(object2->id) == 11)
      stringValue = object2->filter;
    else
      stringValue = "none";
  }

  else { clog << "WARNING: invalid electron-trigobj variable '" << variable << "'\n"; value = -999; }

  value = applyFunction(function, value);

  return value;

}

//!muon-trigobj pair valueLookup
double
OSUAnalysis::valueLookup (const BNmuon* object1, const BNtrigobj* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if (variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);

  else { clog << "WARNING: invalid muon-trigobj variable '" << variable << "'\n"; value = -999; }

  value = applyFunction(function, value);

  return value;

}

//!stop valueLookup
double
OSUAnalysis::valueLookup (const BNstop* object, string variable, string function, string &stringValue){


  double value = 0.0;

  if      (variable == "ctau") value = object->ctau;
  else if (variable == "pdgId") value = object->pdgId;  
  else if (variable == "status") value = object->status;  
  else if (variable == "pt") value = object->pt;  
  else if (variable == "eta") value = object->eta;  
  else if (variable == "phi") value = object->phi;  
  else if (variable == "px") value = object->px;  
  else if (variable == "py") value = object->py;  
  else if (variable == "pz") value = object->pz;  
  else if (variable == "energy") value = object->energy;  
  else if (variable == "mass") value = object->mass;  
  else if (variable == "beta") value = object->beta;  
  else if (variable == "gamma") value = object->gamma;  
  else if (variable == "betaAtDecay") value = object->betaAtDecay;  
  else if (variable == "gammaAtDecay") value = object->gammaAtDecay;  
  else if (variable == "vx") value = object->vx;  
  else if (variable == "vy") value = object->vy;  
  else if (variable == "vz") value = object->vz;  
  else if (variable == "decayVx") value = object->decayVx;  
  else if (variable == "decayVy") value = object->decayVy;  
  else if (variable == "decayVz") value = object->decayVz;  
  else if (variable == "decayLength")          value = object->decayLength;  
  else if (variable == "daughter0Id")          value = object->daughter0Id;  
  else if (variable == "daughter0Status")      value = object->daughter0Status;  
  else if (variable == "daughter0PT")          value = object->daughter0PT;  
  else if (variable == "daughter0Phi")         value = object->daughter0Phi;  
  else if (variable == "daughter0Eta")         value = object->daughter0Eta;  
  else if (variable == "daughter1Id")          value = object->daughter1Id;  
  else if (variable == "daughter1Status")      value = object->daughter1Status;  
  else if (variable == "daughter1PT")          value = object->daughter1PT;  
  else if (variable == "daughter1Phi")         value = object->daughter1Phi;  
  else if (variable == "daughter1Eta")         value = object->daughter1Eta;  
  else if (variable == "charge")               value = object->charge;  
  else if (variable == "threeCharge")          value = object->threeCharge;  
  else if (variable == "daughter0Charge")      value = object->daughter0Charge;  
  else if (variable == "daughter0ThreeCharge") value = object->daughter0ThreeCharge;  
  else if (variable == "daughter1Charge")      value = object->daughter1Charge;  
  else if (variable == "daughter1ThreeCharge") value = object->daughter1ThreeCharge;  
  else if (variable == "rHadronCharge")        value = object->rHadronCharge;  
  else if (variable == "rHadronPT")            value = object->rHadronPT;  
  else if (variable == "rHadronEnergy")        value = object->rHadronEnergy;  
  else if (variable == "rHadronMass")          value = object->rHadronMass;  
  else if (variable == "rHadronPhi")           value = object->rHadronPhi;  
  else if (variable == "rHadronEta")           value = object->rHadronEta;  
  else if (variable == "rHadronBeta")          value = object->rHadronBeta;  
  else if (variable == "rHadronGamma")         value = object->rHadronGamma;  
  else if (variable == "rHadronId")            value = object->rHadronId;  
  else if (variable == "rHadronStatus")        value = object->rHadronStatus;  
  else if (variable == "rHadronThreeCharge")   value = object->rHadronThreeCharge;  
            
  // derived quantities  
  else if (variable == "betaGamma") value = object->beta * object->gamma;  
  else if (variable == "vxy") value = sqrt(object->vx * object->vx + object->vy * object->vy);  
  else if (variable == "decayVxy") value = sqrt(object->decayVx * object->decayVx + object->decayVy * object->decayVy);  

  else if (variable == "d0"){
    double vx = object->vx - chosenVertex ()->x,
      vy = object->vy - chosenVertex ()->y,
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

  else if (variable == "minD0"){
    double minD0=999;
    for(BNprimaryvertexCollection::const_iterator vertex = primaryvertexs->begin (); vertex != primaryvertexs->end (); vertex++){
      double vx = object->vx - vertex->x,
        vy = object->vy - vertex->y,
        px = object->px,
        py = object->py,
        pt = object->pt;
      value = (-vx * py + vy * px) / pt;
      if(abs(value) < abs(minD0)) minD0 = value;
    }
    value = minD0;
  }
  else if (variable == "minDz"){
    double minDz=999;
    for(BNprimaryvertexCollection::const_iterator vertex = primaryvertexs->begin (); vertex != primaryvertexs->end (); vertex++){
      double vx = object->vx - vertex->x,
        vy = object->vy - vertex->y,
        vz = object->vz - vertex->z,
        px = object->px,
        py = object->py,
        pz = object->pz,
        pt = object->pt;
      value = vz - (vx * px + vy * py)/pt * (pz/pt);
      if(abs(value) < abs(minDz)) minDz = value;
    }
    value = minDz;
  }
  else if(variable == "distToVertex"){
    value = sqrt((object->vx-chosenVertex()->x)*(object->vx-chosenVertex()->x) + \
                 (object->vy-chosenVertex()->y)*(object->vy-chosenVertex()->y) + \
                 (object->vz-chosenVertex()->z)*(object->vz-chosenVertex()->z));
  }
  else if (variable == "minDistToVertex"){
    double minDistToVertex=999;
    for(BNprimaryvertexCollection::const_iterator vertex = primaryvertexs->begin (); vertex != primaryvertexs->end (); vertex++){
      value = sqrt((object->vx-vertex->x)*(object->vx-vertex->x) + \
                   (object->vy-vertex->y)*(object->vy-vertex->y) + \
                   (object->vz-vertex->z)*(object->vz-vertex->z));

      if(abs(value) < abs(minDistToVertex)) minDistToVertex = value;
    }
    value = minDistToVertex;
  }
  else if (variable == "distToVertexDifference"){
    double minDistToVertex=999;
    for(BNprimaryvertexCollection::const_iterator vertex = primaryvertexs->begin (); vertex != primaryvertexs->end (); vertex++){
      value = sqrt((object->vx-vertex->x)*(object->vx-vertex->x) + \
                   (object->vy-vertex->y)*(object->vy-vertex->y) + \
                   (object->vz-vertex->z)*(object->vz-vertex->z));

      if(abs(value) < abs(minDistToVertex)) minDistToVertex = value;
    }
    double distToChosenVertex = sqrt((object->vx-chosenVertex()->x)*(object->vx-chosenVertex()->x) + \
                                     (object->vy-chosenVertex()->y)*(object->vy-chosenVertex()->y) + \
                                     (object->vz-chosenVertex()->z)*(object->vz-chosenVertex()->z));

    value = distToChosenVertex - minDistToVertex;
  }

  else if (variable == "closestVertexRank"){
    double minDistToVertex=999;
    int vertex_rank = 0;
    for(BNprimaryvertexCollection::const_iterator vertex = primaryvertexs->begin (); vertex != primaryvertexs->end (); vertex++){
      vertex_rank++;
      int dist = sqrt((object->vx-vertex->x)*(object->vx-vertex->x) + \
                      (object->vy-vertex->y)*(object->vy-vertex->y) + \
                      (object->vz-vertex->z)*(object->vz-vertex->z));

      if(abs(dist) < abs(minDistToVertex)){
        value = vertex_rank;
        minDistToVertex = dist;
      }
    }
  }

  else if (variable == "decaysToTau"){
    value = abs (object->daughter0Id) == 15 || abs (object->daughter1Id) == 15;
  }




  else { clog << "WARNING: invalid stop variable '" << variable << "'\n"; value = -999; }

  value = applyFunction(function, value);

  return value;

} // end stop valueLookup


double
OSUAnalysis::applyFunction(string function, double value){

  if(function == "abs") value = fabs(value);
  else if(function == "fabs") value = fabs(value);
  else if(function == "log10") value = log10(value);
  else if(function == "log") value = log10(value);

  else if(function == "") value = value;
  else{clog << "WARNING: invalid function '" << function << "'\n";}

  return value;

}


double
OSUAnalysis::getRhoCorr(const BNtrack* track) {
  // Return the pile-up (rho) corrected isolation energy, i.e., the total calorimeter energy around the candidate track.                                                                                  
  if (!useTrackCaloRhoCorr_) return -99;
  // if (!rhokt6CaloJetsHandle_) {                                                                                                                                                                        
  //   clog << "ERROR [getTrkCaloTotRhoCorr]:  The collection rhokt6CaloJetsHandle is not available!" << endl;                                                                                            
  //   return -99;                                                                                                                                                                                        
  // }                                                                                                                                                                                                    
  double radDeltaRCone = 0.5;
  double rhoCorr_kt6CaloJets = *rhokt6CaloJetsHandle_ * TMath::Pi() * pow(radDeltaRCone, 2);  // Define effective area as pi*r^2, where r is radius of DeltaR cone.                                       
  return rhoCorr_kt6CaloJets;

}


double
OSUAnalysis::getTrkCaloTotRhoCorr(const BNtrack* track) {
  // Return the pile-up (rho) corrected isolation energy, i.e., the total calorimeter energy around the candidate track.
  if (!useTrackCaloRhoCorr_) return -99;
  // if (!rhokt6CaloJetsHandle_) {
  //   clog << "ERROR [getTrkCaloTotRhoCorr]:  The collection rhokt6CaloJetsHandle is not available!" << endl;
  //   return -99;
  // }
  double radDeltaRCone = 0.5;
  double rhoCorr_kt6CaloJets = *rhokt6CaloJetsHandle_ * TMath::Pi() * pow(radDeltaRCone, 2);  // Define effective area as pi*r^2, where r is radius of DeltaR cone.
  double rawCaloTot = track->caloHadDeltaRp5 + track->caloEMDeltaRp5;
  double caloTotRhoCorrCalo = TMath::Max(0., rawCaloTot - rhoCorr_kt6CaloJets);
  return caloTotRhoCorrCalo;

}


//calculate the scalar sum of Jet Pt in the event.
double
OSUAnalysis::getHt (const BNjetCollection* jetColl){
  double Ht = 0;
  for(BNjetCollection::const_iterator jet = jetColl->begin(); jet !=jetColl->end(); jet++){
    Ht += abs(jet->pt);
  }
  return Ht;
}


double
OSUAnalysis::getSumForwardJetPt (const BNjetCollection* jetColl){
  double sumForwardJetPt = 0;
  TLorentzVector hT (0, 0, 0, 0);
  for(BNjetCollection::const_iterator jet = jetColl->begin(); jet !=jetColl->end(); jet++){
    TLorentzVector hT_2(0, 0, 0, 0);
    if (fabs(jet->eta) < 3 ) continue;
    hT_2.SetPtEtaPhiM(jet->pt, jet->eta, jet->phi, jet->mass);
    hT += hT_2;
    sumForwardJetPt = hT.Pt();

  }

  return sumForwardJetPt;
}


const BNprimaryvertex *
OSUAnalysis::chosenVertex ()
{
  const BNprimaryvertex *chosenVertex = 0;
  if(cumulativeFlags.find ("primaryvertexs") != cumulativeFlags.end ()){
    flagPair vertexFlags;
    for (int i = cumulativeFlags.at("primaryvertexs").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("primaryvertexs").at(i).size()){
        vertexFlags = cumulativeFlags.at("primaryvertexs").at(i);
        break;
      }
    }
    for (uint vertexIndex = 0; vertexIndex != vertexFlags.size(); vertexIndex++){
      if(!vertexFlags.at(vertexIndex).first) continue;
      chosenVertex = & primaryvertexs->at(vertexIndex);
      break;
    }
  }
  else if (find (objectsToGet.begin (), objectsToGet.end (), "primaryvertexs") != objectsToGet.end ())
    chosenVertex = & primaryvertexs->at (0);

  return chosenVertex;
}


const BNmet *
OSUAnalysis::chosenMET ()
{
  const BNmet *chosenMET = 0;
  if(cumulativeFlags.find ("mets") != cumulativeFlags.end ()){
    flagPair metFlags;
    for (int i = cumulativeFlags.at("mets").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("mets").at(i).size()){
        metFlags = cumulativeFlags.at("mets").at(i);
        break;
      }
    }
    for (uint metIndex = 0; metIndex != metFlags.size(); metIndex++){
      if(!metFlags.at(metIndex).first) continue;
      chosenMET = & mets->at(metIndex);
      break;
    }
  }
  else if (find (objectsToGet.begin (), objectsToGet.end (), "mets") != objectsToGet.end ())
    chosenMET = & mets->at (0);

  return chosenMET;
}


const BNtrack *
OSUAnalysis::chosenTrack ()
{
  const BNtrack *chosenTrack = 0;
  if(cumulativeFlags.find ("tracks") != cumulativeFlags.end ()){
    flagPair trackFlags;
    for (int i = cumulativeFlags.at("tracks").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("tracks").at(i).size()){
        trackFlags = cumulativeFlags.at("tracks").at(i);
	break;
      }
    }
    for (uint trackIndex = 0; trackIndex != trackFlags.size(); trackIndex++){
      if(!trackFlags.at(trackIndex).first) continue;
      chosenTrack = & tracks->at(trackIndex);
      break;
    }
  }
  else if (find (objectsToGet.begin (), objectsToGet.end (), "tracks") != objectsToGet.end ())
    chosenTrack = & tracks->at (0);

  return chosenTrack;
}


const BNjet *
OSUAnalysis::chosenJet ()
{
  const BNjet *chosenJet = 0;
  if(cumulativeFlags.find ("jets") != cumulativeFlags.end ()){
    flagPair jetFlags;
    for (int i = cumulativeFlags.at("jets").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("jets").at(i).size()){
        jetFlags = cumulativeFlags.at("jets").at(i);
	break;
      }
    }
    for (uint jetIndex = 0; jetIndex != jetFlags.size(); jetIndex++){
      if(!jetFlags.at(jetIndex).first) continue;
      chosenJet = & jets->at(jetIndex);
      break;
    }
  }
  else if (find (objectsToGet.begin (), objectsToGet.end (), "jets") != objectsToGet.end ())
    chosenJet = & jets->at (0);

  return chosenJet;
}


const BNelectron *
OSUAnalysis::chosenElectron ()
{
  const BNelectron *chosenElectron = 0;
  if(cumulativeFlags.find ("electrons") != cumulativeFlags.end ()){
    flagPair electronFlags;
    for (int i = cumulativeFlags.at("electrons").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("electrons").at(i).size()){
        electronFlags = cumulativeFlags.at("electrons").at(i);
        break;
      }
    }
    for (uint electronIndex = 0; electronIndex != electronFlags.size(); electronIndex++){
      if(!electronFlags.at(electronIndex).first) continue;
      chosenElectron = & electrons->at(electronIndex);
      break;
    }
  }
  else if (find (objectsToGet.begin (), objectsToGet.end (), "electrons") != objectsToGet.end ())
    chosenElectron = & electrons->at (0);

  return chosenElectron;
}


const BNmuon *
OSUAnalysis::chosenMuon ()
{
  const BNmuon *chosenMuon = 0;
  if(cumulativeFlags.find ("muons") != cumulativeFlags.end ()){
    flagPair muonFlags;
    for (int i = cumulativeFlags.at("muons").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("muons").at(i).size()){
        muonFlags = cumulativeFlags.at("muons").at(i);
        break;
      }
    }
    for (uint muonIndex = 0; muonIndex != muonFlags.size(); muonIndex++){
      if(!muonFlags.at(muonIndex).first) continue;
      chosenMuon = & muons->at(muonIndex);
      break;
    }
  }
  else if (find (objectsToGet.begin (), objectsToGet.end (), "muons") != objectsToGet.end ())
    chosenMuon = & muons->at (0);

  return chosenMuon;
}


double
OSUAnalysis::chosenHT ()
{
  double chosenHT = 0.0;
  if(cumulativeFlags.find ("jets") != cumulativeFlags.end ()){
    flagPair jetFlags;
    for (int i = cumulativeFlags.at("jets").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("jets").at(i).size()){
        jetFlags = cumulativeFlags.at("jets").at(i);
        break;
      }
    }
    for (uint jetIndex = 0; jetIndex != jetFlags.size(); jetIndex++){
      if(!jetFlags.at(jetIndex).first) continue;
      chosenHT += jets->at(jetIndex).pt;
    }
  }

  return chosenHT;
}


double
OSUAnalysis::chosenST ()
{
  double chosenST = 0.0;
  if(cumulativeFlags.find ("electrons") != cumulativeFlags.end ()){
    flagPair electronFlags;
    for (int i = cumulativeFlags.at("electrons").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("electrons").at(i).size()){
        electronFlags = cumulativeFlags.at("electrons").at(i);
        break;
      }
    }
    for (uint electronIndex = 0; electronIndex != electronFlags.size(); electronIndex++){
      if(!electronFlags.at(electronIndex).first) continue;
      chosenST += electrons->at(electronIndex).pt;
    }
  }

  if(cumulativeFlags.find ("muons") != cumulativeFlags.end ()){
    flagPair muonFlags;
    for (int i = cumulativeFlags.at("muons").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("muons").at(i).size()){
        muonFlags = cumulativeFlags.at("muons").at(i);
        break;
      }
    }
    for (uint muonIndex = 0; muonIndex != muonFlags.size(); muonIndex++){
      if(!muonFlags.at(muonIndex).first) continue;
      chosenST += muons->at(muonIndex).pt;
    }
  }

  if(cumulativeFlags.find ("jets") != cumulativeFlags.end ()){
    flagPair jetFlags;
    for (int i = cumulativeFlags.at("jets").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("jets").at(i).size()){
        jetFlags = cumulativeFlags.at("jets").at(i);
        break;
      }
    }
    for (uint jetIndex = 0; jetIndex != jetFlags.size(); jetIndex++){
      if(!jetFlags.at(jetIndex).first) continue;
      chosenST += jets->at(jetIndex).pt;
    }
  }

  return chosenST;
}


template<typename T0, typename T1> const T1 *
OSUAnalysis::closest (const T0 *object0, const edm::Handle<vector<T1> > &object1Collection, const string &name)
{
  const T1 *closestObject1 = 0;
  double smallestDeltaR;
  if(cumulativeFlags.find (name) != cumulativeFlags.end ()){
    flagPair flags;
    for (int i = cumulativeFlags.at(name).size() - 1; i >= 0; i--){
      if (cumulativeFlags.at(name).at(i).size()){
        flags = cumulativeFlags.at(name).at(i);
        break;
      }
    }
    for (uint index = 0; index != flags.size(); index++){
      if(!flags.at(index).first) continue;
      const T1 *object1 = &object1Collection->at(index);
      double dR;
      if(!closestObject1 || (dR = reco::deltaR (object0->eta, object0->phi, object1->eta, object1->phi)) < smallestDeltaR){
        closestObject1 = object1;
        smallestDeltaR = dR;
      }
    }
  }
  else if (find (objectsToGet.begin (), objectsToGet.end (), name) != objectsToGet.end ())
    closestObject1 = & object1Collection->at(0);

  return closestObject1;
}


// Returns the smallest DeltaR between the object and any generated true particle in the event.
template <class InputObject>
double OSUAnalysis::getGenDeltaRLowest(InputObject object){
  double genDeltaRLowest = 999.;
  for(BNmcparticleCollection::const_iterator mcparticle = mcparticles->begin (); mcparticle != mcparticles->end (); mcparticle++){
    double deltaRtemp = deltaR(mcparticle->eta, mcparticle->phi, object->eta, object->phi);
    if (deltaRtemp < genDeltaRLowest) genDeltaRLowest = deltaRtemp;
  }
  return genDeltaRLowest;
}


template <class InputObject>
int OSUAnalysis::getGenMatchedParticleIndex(InputObject object){

  int bestMatchIndex = -1;
  double bestMatchDeltaR = 999;

  for(BNmcparticleCollection::const_iterator mcparticle = mcparticles->begin (); mcparticle != mcparticles->end (); mcparticle++){

    double currentDeltaR = deltaR(object->eta,object->phi,mcparticle->eta,mcparticle->phi);
    if(currentDeltaR > 0.05) continue;
    //     clog << setprecision(3) << setw(20)
    //          << "\tcurrentParticle:  eta = " << mcparticles->at(mcparticle - mcparticles->begin()).eta
    //          << setw(20)
    //          << "\tphi = " << mcparticles->at(mcparticle - mcparticles->begin()).phi
    //          << setw(20)
    //          << "\tdeltaR = " << currentDeltaR
    //          << setprecision(1)
    //          << setw(20)
    //          << "\tid = " << mcparticles->at(mcparticle - mcparticles->begin()).id
    //          << setw(20)
    //          << "\tmotherId = " << mcparticles->at(mcparticle - mcparticles->begin()).motherId
    //          << setw(20)
    //          << "\tstatus = " << mcparticles->at(mcparticle - mcparticles->begin()).status<< endl;
    if(currentDeltaR < bestMatchDeltaR && mcparticles->at(mcparticle - mcparticles->begin()).id != mcparticles->at(mcparticle - mcparticles->begin()).motherId){
      bestMatchIndex = mcparticle - mcparticles->begin();
      bestMatchDeltaR = currentDeltaR;
    }

  }
  //   if(bestMatchDeltaR != 999)  clog << "bestMatch:  deltaR = " << bestMatchDeltaR << "   id = " << mcparticles->at(bestMatchIndex).id << "   motherId = " << mcparticles->at(bestMatchIndex).motherId << endl;
  //   else clog << "no match found..." << endl;
  return bestMatchIndex;

}

// bin      particle type
// ---      -------------
//  0        unmatched
//  1        u
//  2        d
//  3        s
//  4        c
//  5        b
//  6        t
//  7        e
//  8        mu
//  9        tau
// 10        nu
// 11        g
// 12        gamma
// 13        Z
// 14        W
// 15        light meson
// 16        K meson
// 17        D meson
// 18        B meson
// 19        light baryon
// 20        strange baryon
// 21        charm baryon
// 22        bottom baryon
// 23        QCD string
// 24        other

int OSUAnalysis::getPdgIdBinValue(int pdgId){

  int binValue = -999;
  int absPdgId = fabs(pdgId);
  if(pdgId == -1) binValue = 0;
  else if(absPdgId <= 6 ) binValue = absPdgId;
  else if(absPdgId == 11 ) binValue = 7;
  else if(absPdgId == 13 ) binValue = 8;
  else if(absPdgId == 15 ) binValue = 9;
  else if(absPdgId == 12 || absPdgId == 14 || absPdgId == 16 ) binValue = 10;
  else if(absPdgId == 21 ) binValue = 11;
  else if(absPdgId == 22 ) binValue = 12;
  else if(absPdgId == 23 ) binValue = 13;
  else if(absPdgId == 24 ) binValue = 14;
  else if(absPdgId > 100 && absPdgId < 300 && absPdgId != 130  ) binValue = 15;
  else if( absPdgId == 130 || (absPdgId > 300 && absPdgId < 400)  ) binValue = 16;
  else if(absPdgId > 400 && absPdgId < 500  ) binValue = 17;
  else if(absPdgId > 500 && absPdgId < 600  ) binValue = 18;
  else if(absPdgId > 1000 && absPdgId < 3000  ) binValue = 19;
  else if(absPdgId > 3000 && absPdgId < 4000  ) binValue = 20;
  else if(absPdgId > 4000 && absPdgId < 5000  ) binValue = 21;
  else if(absPdgId > 5000 && absPdgId < 6000  ) binValue = 22;
  else if(absPdgId == 92  ) binValue = 23;

  else binValue = 24;

  return binValue;

}


int OSUAnalysis::findTauMotherIndex(const BNmcparticle* tau){

  int bestMatchIndex = -1;
  double bestMatchDeltaR = 999;

  for(BNmcparticleCollection::const_iterator mcparticle = mcparticles->begin (); mcparticle != mcparticles->end (); mcparticle++){

    if(fabs(mcparticle->id) != 15 || mcparticle->status !=3) continue;

    double currentDeltaR = deltaR(tau->eta,tau->phi,mcparticle->eta,mcparticle->phi);
    if(currentDeltaR > 0.05) continue;

    if(currentDeltaR < bestMatchDeltaR && mcparticles->at(mcparticle - mcparticles->begin()).id != mcparticles->at(mcparticle - mcparticles->begin()).motherId){
      bestMatchIndex = mcparticle - mcparticles->begin();
      bestMatchDeltaR = currentDeltaR;
    }

  }

  return bestMatchIndex;
}


unsigned int OSUAnalysis::GetNumExtraPartons(const BNmcparticleCollection* genPartColl){

  int nList = 0;
  unsigned int nPartons = 0;
  for( BNmcparticleCollection::const_iterator mcparticle = genPartColl->begin(); mcparticle != genPartColl->end(); mcparticle++)
   {
     nList ++;
    int id = mcparticle->id;
    int motherID = mcparticle->motherId;
    int status = mcparticle->status;
    int aid = abs(id);

    if(status == 3){//only status 3 particles (which are listed first)
      if(nList>6){//dont look at first 6 (incomming event)
        if( (aid>0 && aid<6) || aid ==21 || aid ==9){//udscb gluon
          if(abs(motherID) !=23 && abs(motherID) !=24 && abs(motherID) != 25 && abs(motherID)!=6 ){ //not from WZHt
            nPartons++;
          }
        }
      }
    }else{
      break; //break when you run out of stat==3
    }
  }

  return nPartons;

}


double
OSUAnalysis::getSumJetPt (const BNjetCollection* jetColl){
  double sumJetPt = 0;
  TLorentzVector hT (0, 0, 0, 0);
  for(BNjetCollection::const_iterator jet = jetColl->begin(); jet !=jetColl->end(); jet++){
    TLorentzVector hT_2(0, 0, 0, 0);
    hT_2.SetPtEtaPhiM(jet->pt, jet->eta, jet->phi, jet->mass);
    hT += hT_2;
    sumJetPt = hT.Pt();
    
  }
  return sumJetPt;
}


//calculate the scalar sum of Electron, Muon, and Jet Pt in the event.
double
OSUAnalysis::getSt (const BNelectronCollection* electronColl, const BNmuonCollection* muonColl, const BNjetCollection* jetColl){
  double St = 0;
  if (!electronColl || !muonColl || !jetColl) return -999.0;
  for(BNelectronCollection::const_iterator electron = electronColl->begin(); electron !=electronColl->end(); electron++){
    St += abs(electron->pt);
  }
  for(BNmuonCollection::const_iterator muon = muonColl->begin(); muon !=muonColl->end(); muon++){
    St += abs(muon->pt);
  }
  for(BNjetCollection::const_iterator jet = jetColl->begin(); jet !=jetColl->end(); jet++){
    St += abs(jet->pt);
  }

  return St;
}


pair<const BNmuon *, const BNmuon *>
OSUAnalysis::leadMuonPair ()
{
  pair<const BNmuon *, const BNmuon *> leadMuonPair;
  leadMuonPair.first = leadMuonPair.second = 0;

  if(cumulativeFlags.find ("muons") != cumulativeFlags.end ()){
    flagPair muonFlags;
    for (int i = cumulativeFlags.at("muons").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("muons").at(i).size()){
        muonFlags = cumulativeFlags.at("muons").at(i);
        break;
      }
    }
    for (uint muonIndex0 = 0; muonIndex0 != muonFlags.size(); muonIndex0++){
      if(!muonFlags.at(muonIndex0).first) continue;
      for (uint muonIndex1 = muonIndex0 + 1; muonIndex1 < muonFlags.size(); muonIndex1++){
        if(!muonFlags.at(muonIndex1).first) continue;
        const BNmuon *mu0 = & muons->at(muonIndex0);
        const BNmuon *mu1 = & muons->at(muonIndex1);
        if(leadMuonPair.first == 0 || leadMuonPair.second == 0){
          leadMuonPair.first = mu0;
          leadMuonPair.second = mu1;
        }
        else{
          TVector2 newPt0 (mu0->px, mu0->py),
                   newPt1 (mu1->px, mu1->py),
                   oldPt0 (leadMuonPair.first->px, leadMuonPair.first->py),
                   oldPt1 (leadMuonPair.second->px, leadMuonPair.second->py);
          if(newPt0.Mod () + newPt1.Mod () > oldPt0.Mod() + oldPt1.Mod ())
            {
              leadMuonPair.first = mu0;
              leadMuonPair.second = mu1;
            }
        }
      }
    }
  }

  return leadMuonPair;
}


pair<const BNelectron *, const BNelectron *>
OSUAnalysis::leadElectronPair ()
{
  pair<const BNelectron *, const BNelectron *> leadElectronPair;
  leadElectronPair.first = leadElectronPair.second = 0;
  if(cumulativeFlags.find ("electrons") != cumulativeFlags.end ()){
    flagPair electronFlags;
    for (int i = cumulativeFlags.at("electrons").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("electrons").at(i).size()){
        electronFlags = cumulativeFlags.at("electrons").at(i);
        break;
      }
    }
    for (uint electronIndex0 = 0; electronIndex0 != electronFlags.size(); electronIndex0++){
      if(!electronFlags.at(electronIndex0).first) continue;
      for (uint electronIndex1 = electronIndex0 + 1; electronIndex1 < electronFlags.size(); electronIndex1++){
        if(!electronFlags.at(electronIndex1).first) continue;
        const BNelectron *el0 = & electrons->at(electronIndex0);
        const BNelectron *el1 = & electrons->at(electronIndex1);
        if(leadElectronPair.first == 0 || leadElectronPair.second == 0){
          leadElectronPair.first = el0;
          leadElectronPair.second = el1;
        }
        else{
          TVector2 newPt0 (el0->px, el0->py),
                   newPt1 (el1->px, el1->py),
                   oldPt0 (leadElectronPair.first->px, leadElectronPair.first->py),
                   oldPt1 (leadElectronPair.second->px, leadElectronPair.second->py);
          if(newPt0.Mod () + newPt1.Mod () > oldPt0.Mod() + oldPt1.Mod ())
            {
              leadElectronPair.first = el0;
              leadElectronPair.second = el1;
            }
        }
      }
    }
  }

  return leadElectronPair;
}


//return corrected jet
BNjet OSUAnalysis::getCorrectedJet(const BNjet &iJet, string jERCase){

    if(datasetType_ == "data"){ return iJet; }
    double jetFactor = 1;

  // Make appopriate correction to the xy components of input jet
    if(jERCase == "JERup")   jetFactor *= getJERfactor(1, fabs(iJet.eta), iJet.genJetPT, iJet.pt);
    if(jERCase == "JERdown") jetFactor *= getJERfactor(-1, fabs(iJet.eta), iJet.genJetPT, iJet.pt);
    if(jERCase == "JESup") {
      jetFactor *= (1. + iJet.JESunc);
      jetFactor *= getJERfactor(0, fabs(iJet.eta), iJet.genJetPT, jetFactor * iJet.pt);
    }
    if(jERCase =="JESdown"){
      jetFactor *= (1. - iJet.JESunc);
      jetFactor *= getJERfactor(0, fabs(iJet.eta), iJet.genJetPT, jetFactor * iJet.pt);
    }
    else jetFactor *= getJERfactor(0, fabs(iJet.eta), iJet.genJetPT, iJet.pt);
  // Make a copy of the input jet for output and update 4-vector values
  BNjet result = iJet;
    
  result.px   *= jetFactor;
  result.py   *= jetFactor;
  result.pz   *= jetFactor;
  result.pt   *= jetFactor;
  result.et      *= jetFactor;
  result.energy *= jetFactor;
  result.mass     *= jetFactor;

  // Update CSV value (i.e. reshape it if applicable)
  // result.btagCombinedSecVertex = GetCSVvalue(result,iSysType);

  return result;


}


double
OSUAnalysis::getDetectorEta(double evntEta,double z0) {
  // Use algorithm from Sam Harper, https://hypernews.cern.ch/HyperNews/CMS/get/EXO-12-034/19.html  
  double thetaEvt = etaToTheta(evntEta);
  //   std::cout << "**************************************** " << std::endl;
  //   std::cout << "Event Theta: " << thetaEvt << std::endl;
  double z = 129.4 / tan(thetaEvt); //129.4 is the average barrel radius
  double zTot = z+z0;
  
  if(fabs(zTot)<269){ //269 is an emperically derived number which means that < its likely in th barrel
    return zTot !=0 ? thetaToEta(atan(129.4/zTot)) : 0.;
  }

  //otherwise endcap time
  double endcapZ = 319.2; //average z position of endcap
  if(evntEta<0) endcapZ*=-1;
  double rxy = tan(thetaEvt) * (endcapZ-z0);
  //   std::cout << "rxy: " << rxy << std::endl;
  //   std::cout << "endcapZ: " << endcapZ << std::endl;
  //   std::cout << "Event Eta: " << evntEta << std::endl;
  //   std::cout << "Result of getDetectorEta: " << thetaToEta(atan(rxy/endcapZ)) << std::endl;
  //  std::cout << "Result of getDetectorEta: " << -log(tan((atan(rxy/endcapZ))/2)) << std::endl;
  return thetaToEta(atan(rxy/endcapZ));
  //  return -log(tan((atan(rxy/endcapZ))/2));
  
}


double
OSUAnalysis::getTrkDepTrkRp5RhoCorr(const BNtrack* track) {
  // Return the pile-up (rho) corrected isolation energy, i.e., the total calorimeter energy around the candidate track.
  if (!useTrackCaloRhoCorr_) return -99;
  // if (!rhokt6CaloJetsHandle_) {
  //   clog << "ERROR [getTrkCaloTotRhoCorr]:  The collection rhokt6CaloJetsHandle is not available!" << endl;
  //   return -99;
  // }
  double radDeltaRCone = 0.5;
  double rhoCorr_kt6CaloJets = *rhokt6CaloJetsHandle_ * TMath::Pi() * pow(radDeltaRCone, 2);  // Define effective area as pi*r^2, where r is radius of DeltaR cone.
  double rawDepTrkRp5 = track->depTrkRp5;
 double depTrkRp5RhoCorr = TMath::Max(0., rawDepTrkRp5 - rhoCorr_kt6CaloJets);
 return depTrkRp5RhoCorr;

}


double
OSUAnalysis::getTrkDepTrkRp3RhoCorr(const BNtrack* track) {
  // Return the pile-up (rho) corrected isolation energy, i.e., the total calorimeter energy around the candidate track.
  if (!useTrackCaloRhoCorr_) return -99;
  // if (!rhokt6CaloJetsHandle_) {
  //   clog << "ERROR [getTrkCaloTotRhoCorr]:  The collection rhokt6CaloJetsHandle is not available!" << endl;
  //   return -99;
  // }
  double radDeltaRCone = 0.3;
// Define effective area as pi*r^2, where r is radius of DeltaR cone
  double rhoCorr_kt6CaloJets = *rhokt6CaloJetsHandle_ * TMath::Pi() * pow(radDeltaRCone, 2);
  double rawDepTrkRp3 = track->depTrkRp3;
  double depTrkRp3RhoCorr = TMath::Max(0., rawDepTrkRp3 - rhoCorr_kt6CaloJets);
  return depTrkRp3RhoCorr;

}


// Calculate the number of tracks in cone of DeltaR<0.5 around track1.
// Return true iff no other tracks are found in this cone.
int
OSUAnalysis::getTrkIsIso (const BNtrack* track1, const BNtrackCollection* trackColl){
  for(BNtrackCollection::const_iterator track2 = trackColl->begin(); track2 !=trackColl->end(); track2++){
    if(track1->eta == track2->eta && track1->phi == track2->phi) continue; // Do not compare the track to itself.
    double deltaRtrk = deltaR(track1->eta, track1->phi, track2->eta, track2->phi);
    if(deltaRtrk < 0.5) return 0;
  }
  return 1;

}


//if a track is found within dR<0.05 of a dead Ecal channel value = 1, otherwise value = 0
int
OSUAnalysis::getTrkIsMatchedDeadEcal (const BNtrack* track1){
  int value = 0;
  if (getTrkDeadEcalDeltaR(track1)<0.05) {value = 1;}
  else {value = 0;}
  return value;
}



int
OSUAnalysis::getTrkIsMatchedDeadEcalDet (const BNtrack* track1){
  int value = 0;
  if (getDetDeadEcalDeltaR(track1)<0.05) {value = 1;}
  else {value = 0;}
  return value;
}


double
OSUAnalysis::getTrkDeadEcalDeltaR (const BNtrack* track1){
  double deltaRLowest = 999;
  if (DeadEcalVec.size() == 0) WriteDeadEcal();
  for(vector<DeadEcal>::const_iterator ecal = DeadEcalVec.begin(); ecal != DeadEcalVec.end(); ++ecal){
    double eta = ecal->etaEcal;
    double phi = ecal->phiEcal;
    double deltaRtemp = deltaR(eta, phi, track1->eta, track1->phi);
    if(deltaRtemp < deltaRLowest) deltaRLowest = deltaRtemp;
  }
  if (verbose_) clog << deltaRLowest << endl;
  return deltaRLowest;
}


// If a track is found within dR<0.25 of a bad CSC chamber, value = 1, otherwise value = 0.  
// FIXME:  Instead of a deltaR cut, it would be better to use the boundaries of the chamber 
int
OSUAnalysis::getTrkIsMatchedBadCSC (const BNtrack* track1){
  double deltaRLowest = 999;
  int value = 0;
  if (BadCSCVec.size() == 0) WriteBadCSC();
  for(vector<BadCSC>::const_iterator csc = BadCSCVec.begin(); csc != BadCSCVec.end(); ++csc) {
    double eta = csc->etaCSC;
    double phi = csc->phiCSC;
    double deltaRtemp = deltaR(eta, phi, track1->eta, track1->phi);
    if(deltaRtemp < deltaRLowest) deltaRLowest = deltaRtemp;
  }
  if (deltaRLowest<0.25) { value = 1; }
  else                  { value = 0; } 
  return value;
}


double
OSUAnalysis::getTrkPtRes (const BNtrack* track1){

  double ptTrue = getTrkPtTrue(track1, mcparticles.product());
  double PtRes = (track1->pt - ptTrue) / ptTrue;

  return PtRes;

}


double
OSUAnalysis::getTrkPtTrue (const BNtrack* track1, const BNmcparticleCollection* genPartColl){
  double value = -99;
  double genDeltaRLowest = 999;

  for (BNmcparticleCollection::const_iterator genPart = genPartColl->begin(); genPart !=genPartColl->end(); genPart++){
    double genDeltaRtemp = deltaR(genPart->eta, genPart->phi,track1->eta, track1->phi);
    if (genDeltaRtemp < genDeltaRLowest) {
      genDeltaRLowest = genDeltaRtemp;
      if (genDeltaRLowest < 0.15) {   // Only consider it truth-matched if DeltaR<0.15.
        double ptTrue = genPart->pt;
        value = ptTrue;
      }
    }
  }

  return value;

}


double OSUAnalysis::getPtSingleObjectMatchesAnyTrigger(double recoPhi, const vector<string> & targetTriggers, const BNtrigobjCollection * triggerObjects){

  double matchPt = -99;

  for ( BNtrigobjCollection::const_iterator iObj = triggerObjects->begin();
        iObj != triggerObjects->end();
        iObj ++ ) {
    // look for a matching name again                                                                                                          
    for (vector<string>::const_iterator iTarget = targetTriggers.begin();
         iTarget != targetTriggers.end();
         iTarget++) {
      // if this is the right name                                                                                                             
      if ( iObj->filter.find((*iTarget)) != std::string::npos) {
        double deltaPhiCalc = fabs(deltaPhi(recoPhi, iObj->phi));

        if (deltaPhiCalc < 0.3) matchPt = iObj->pt;

      } 

    }// end for each target                                                                                                                    


  } // end for each object                                                                                                                     


  return matchPt;


}


double OSUAnalysis::getJERfactor(int returnType, double jetAbsETA, double genjetPT, double recojetPT){

  double factor = 1.;
  
  double scale_JER = 1., scale_JERup = 1., scale_JERdown = 1.;
  if( jetAbsETA<0.5 ){ 
    scale_JER = 1.052; scale_JERup = 1.052 + sqrt( 0.012*0.012 + 0.062*0.062 ); scale_JERdown = 1.052 - sqrt( 0.012*0.012 + 0.061*0.061 );
  }
  else if( jetAbsETA<1.1 ){ 
    scale_JER = 1.057; scale_JERup = 1.057 + sqrt( 0.012*0.012 + 0.056*0.056 ); scale_JERdown = 1.057 - sqrt( 0.012*0.012 + 0.055*0.055 );
  }
  else if( jetAbsETA<1.7 ){ 
    scale_JER = 1.096; scale_JERup = 1.096 + sqrt( 0.017*0.017 + 0.063*0.063 ); scale_JERdown = 1.096 - sqrt( 0.017*0.017 + 0.062*0.062 );
  }
  else if( jetAbsETA<2.3 ){ 
    scale_JER = 1.134; scale_JERup = 1.134 + sqrt( 0.035*0.035 + 0.087*0.087 ); scale_JERdown = 1.134 - sqrt( 0.035*0.035 + 0.085*0.085 );
  }
  else if( jetAbsETA<5.0 ){ 
    scale_JER = 1.288; scale_JERup = 1.288 + sqrt( 0.127*0.127 + 0.155*0.155 ); scale_JERdown = 1.288 - sqrt( 0.127*0.127 + 0.153*0.153 );
  }

  double jetPt_JER = recojetPT;
  double jetPt_JERup = recojetPT;
  double jetPt_JERdown = recojetPT;

  double diff_recojet_genjet = recojetPT - genjetPT;

  if( genjetPT>10. ){
    jetPt_JER = std::max( 0., genjetPT + scale_JER * ( diff_recojet_genjet ) );
    jetPt_JERup = std::max( 0., genjetPT + scale_JERup * ( diff_recojet_genjet ) );
    jetPt_JERdown = std::max( 0., genjetPT + scale_JERdown * ( diff_recojet_genjet ) );
  }

  if( returnType==1 )       factor = jetPt_JERup/recojetPT;
  else if( returnType==-1 ) factor = jetPt_JERdown/recojetPT;
  else                      factor = jetPt_JER/recojetPT;

  if( !(genjetPT>10.) ) factor = 1.;

  return factor;


}


double
OSUAnalysis::etaToTheta(double eta) {
  if(eta<0) return -2*atan(exp(eta));
  else return 2*atan(exp(-1*eta));
}


double
OSUAnalysis::getDetDeadEcalDeltaR (const BNtrack* track1){
  double deltaRLowest = 999;
  if (DeadEcalVec.size() == 0) WriteDeadEcal();
  for(vector<DeadEcal>::const_iterator ecal = DeadEcalVec.begin(); ecal != DeadEcalVec.end(); ++ecal){
    double eta = ecal->etaEcal;
    double phi = ecal->phiEcal;
    double deltaRtemp = deltaR(eta, phi, getDetectorEta(track1->eta, track1->dZ), track1->phi);
    if(deltaRtemp < deltaRLowest) deltaRLowest = deltaRtemp;
  }
  if (verbose_) clog << deltaRLowest << endl;
  return deltaRLowest;
}


//creates a map of the dead Ecal channels in the barrel and endcap
//to see how the map of dead Ecal channels is created look at function getChannelStatusMaps() here:
//http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/jbrinson/DisappTrk/OSUT3Analysis/AnaTools/src/OSUAnalysis.cc?revision=1.88&view=markup
void
OSUAnalysis::WriteDeadEcal (){
  double etaEcal, phiEcal;
  ifstream DeadEcalFile(deadEcalFile_);
  if(!DeadEcalFile) {
    clog << "Error: DeadEcalFile has not been found." << endl;
    return;
  }
  if(DeadEcalVec.size()!= 0){
    clog << "Error: DeadEcalVec has a nonzero size" << endl;
    return;
  }
  while(!DeadEcalFile.eof())
    {
      DeadEcalFile >> etaEcal >> phiEcal;
      DeadEcal newChan;
      newChan.etaEcal = etaEcal;
      newChan.phiEcal = phiEcal;
      DeadEcalVec.push_back(newChan);
    }
  if(DeadEcalVec.size() == 0) clog << "Warning: No dead Ecal channels have been found." << endl;
}


// Creates a map of the bad CSC chambers.  
// The list of bad CSC chambers is taken from:  
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/CSCDPGConditions#CSC_bad_chambers_in_2012_30_05_2
void
OSUAnalysis::WriteBadCSC() {
  double etaCSC, phiCSC;
  ifstream BadCSCFile(badCSCFile_);
  if (!BadCSCFile) {
    clog << "Error: BadCSCFile has not been found." << endl;
    return;
  }
  if (BadCSCVec.size()!= 0) {
    clog << "Error: BadCSCVec has a nonzero size" << endl;
    return;
  }
  while (!BadCSCFile.eof()) {
    BadCSCFile >> etaCSC >> phiCSC;
    BadCSC newChan;
    newChan.etaCSC = etaCSC;
    newChan.phiCSC = phiCSC;
    BadCSCVec.push_back(newChan);
    //    clog << "Debug:  Adding bad CSC with eta=" << etaCSC << ", phi=" << phiCSC << endl;  
  }
  if (BadCSCVec.size() == 0) clog << "Warning: No bad CSC chambers have been found." << endl;
}  


double
OSUAnalysis::thetaToEta(double theta) {
  if (theta < 0) theta += TMath::Pi();  // to calculate eta, theta must be in range (0,pi)
  return -log(tan(theta/2.));
}
