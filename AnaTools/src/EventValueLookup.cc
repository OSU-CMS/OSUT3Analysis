#include <iostream>

#include "TLorentzVector.h"
#include "TVector2.h"

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!event valueLookup
double
ValueLookup::valueLookup (const BNevent* object, string variable){

  double value = 0.0;

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
      vector<bool> mcFlags = getLastValidFlags("mcparticles");
      for (uint mcIndex = 0; mcIndex != mcFlags.size(); mcIndex++) {
        if (!mcFlags.at(mcIndex)) continue;
        TVector2 pt(mcparticles->at(mcIndex).px, mcparticles->at(mcIndex).py);
        ptTot = pt + ptTot;
      }
    }
    value = ptTot.Mod();
  }
  else if(variable == "totalMcparticlePhi") {
    TVector2 ptTot(0.0, 0.0);
    if(find(objectsToCut.begin(),objectsToCut.end(),"mcparticles") != objectsToCut.end ()){
      vector<bool> mcFlags = getLastValidFlags("mcparticles");
      for (uint mcIndex = 0; mcIndex != mcFlags.size(); mcIndex++) {
        if (!mcFlags.at(mcIndex)) continue;
        TVector2 pt(mcparticles->at(mcIndex).px, mcparticles->at(mcIndex).py);
        ptTot = pt + ptTot;
      }
    }
    value = ptTot.Phi();
  }
  else if(variable == "totalMuonPt") {
    TVector2 ptTot(0.0, 0.0);
    if(find(objectsToCut.begin(),objectsToCut.end(),"muons") != objectsToCut.end ()){
      vector<bool> muFlags = getLastValidFlags("muons");
      for (uint muIndex = 0; muIndex != muFlags.size(); muIndex++) {
        if (!muFlags.at(muIndex)) continue;
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
   vector<bool> mcFlags;
   //get the last valid flags in the flag map
   for (int i = cumulativeFlags.at("mcparticles").size() - 1; i >= 0; i--){
     if (cumulativeFlags.at("mcparticles").at(i).size()){
       mcFlags = cumulativeFlags.at("mcparticles").at(i);
       break;
     }
   }
   for (uint mcIndex = 0; mcIndex != mcFlags.size(); mcIndex++) {
     if (!mcFlags.at(mcIndex)) continue;
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
      vector<bool> muFlags = getLastValidFlags("muons");
      for (uint muIndex = 0; muIndex != muFlags.size(); muIndex++) {
        if (!muFlags.at(muIndex)) continue;
        TVector2 pt(muons->at(muIndex).px, muons->at(muIndex).py);
        ptTot = pt + ptTot;
      }
    }
    value = ptTot.Phi();
  }
  else if(variable == "totalJetPt") {
    TVector2 ptTot(0.0, 0.0);
    if(find(objectsToCut.begin(),objectsToCut.end(),"jets") != objectsToCut.end ()){
      vector<bool> jetFlags = getLastValidFlags("jets");
      for (uint jetIndex = 0; jetIndex != jetFlags.size(); jetIndex++) {
        if (!jetFlags.at(jetIndex)) continue;
        TVector2 pt(jets->at(jetIndex).px, jets->at(jetIndex).py);
        ptTot = pt + ptTot;
      }
    }
    value = ptTot.Mod();
  }
  else if(variable == "totalJetPhi") {
    TVector2 ptTot(0.0, 0.0);
    if(find(objectsToCut.begin(),objectsToCut.end(),"jets") != objectsToCut.end ()){
      vector<bool> jetFlags = getLastValidFlags("jets");
      for (uint jetIndex = 0; jetIndex != jetFlags.size(); jetIndex++) {
        if (!jetFlags.at(jetIndex)) continue;
        TVector2 pt(jets->at(jetIndex).px, jets->at(jetIndex).py);
        ptTot = pt + ptTot;
      }
    }
    value = ptTot.Phi();
  }
  else if(variable == "dijetDeltaPhiMax") {
    double deltaPhiMax = -99.;
    if(find(objectsToCut.begin(),objectsToCut.end(),"jets") != objectsToCut.end ()){
      vector<bool> jetFlags = getLastValidFlags("jets");
      for   (uint iJet = 0;      iJet != jetFlags.size(); iJet++) {
        for (uint jJet = iJet+1; jJet != jetFlags.size(); jJet++) {
          if (!jetFlags.at(iJet)) continue;
          if (!jetFlags.at(jJet)) continue;
          double dPhi = fabs(deltaPhi(jets->at(iJet).phi, jets->at(jJet).phi));
          if (dPhi > deltaPhiMax) deltaPhiMax = dPhi;
        }
      }
    }
    value = deltaPhiMax;
  }
  else if(variable == "totalJetPtMinusTotalMcparticlePt") {
    value =
      valueLookup(object, "totalJetPt") -
      valueLookup(object, "totalMcparticlePt");
  }
  else if(variable == "totalJetPhiMinusTotalMcparticlePhi") {
    value = deltaPhi(valueLookup(object, "totalJetPhi"),
                     valueLookup(object, "totalMcparticlePhi"));
  }
  else if(variable == "totalMuonPtMinusTotalMcparticlePt") {
    value =
      valueLookup(object, "totalMuonPt") -
      valueLookup(object, "totalMcparticlePt");
  }
  else if(variable == "totalMuonPtMinusTotalJetPt") {
    value =
      valueLookup(object, "totalMuonPt") -
      valueLookup(object, "totalJetPt");
  }
  else if(variable == "totalMuonPhiMinusTotalJetPhi") {
    value = deltaPhi(valueLookup(object, "totalMuonPhi"),
                     valueLookup(object, "totalJetPhi"));
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

  return value;
} // end event valueLookup
