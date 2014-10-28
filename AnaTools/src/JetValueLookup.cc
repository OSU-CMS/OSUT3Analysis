#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!jet valueLookup
double
ValueLookup::valueLookup (const BNjet* object, string variable){

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
    } else value = numeric_limits<unsigned>::max ();
  }

  else if(variable == "metPt") {  // allow making 2D plots of jet quantities vs. Met
    if (const BNmet *met = chosenMET ()) {
      value = met->pt;
    } else value = numeric_limits<unsigned>::max ();
  }

  else if(variable == "isLeadingPtJet") {
    double ptMax = -99;
    for (uint ijet = 0; ijet<jets->size(); ijet++) {
      double jetPt = valueLookup(&jets->at(ijet), "pt");
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
      double muonPt = valueLookup(&muons->at(imuon), "pt");
      if (muonPt < 20) continue;
      double muonEta = valueLookup(&muons->at(imuon), "eta");
      double muonPhi = valueLookup(&muons->at(imuon), "phi");
      double dR = deltaR(object->eta, object->phi, muonEta, muonPhi);
      if (dR < deltaRMin) deltaRMin = dR;
    }
    value = deltaRMin;
  }
  else{clog << "WARNING: invalid jet variable '" << variable << "'\n"; value = numeric_limits<unsigned>::max ();}

  return value;
} // end jet valueLookup
