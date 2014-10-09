#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!met valueLookup
double
ValueLookup::valueLookup (const BNmet* object, string variable, string function, string &stringValue){

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
