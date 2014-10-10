#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!track valueLookup
double
ValueLookup::valueLookup (const BNtrack* object, string variable, string function, string &stringValue){

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
          clog << "Warning [valueLookup()]: More than one dimuon pair passes criteria in deltaPhiMuMuPair calculation." << endl;
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
