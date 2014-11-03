#include <iostream>

#include "TLorentzVector.h"

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!track valueLookup
double
ValueLookup::valueLookup (const BNtrack &object, string variable){

  double value = numeric_limits<int>::min ();
  double pMag = hypot (object.pt, object.pz);
  BNtrack *obj = new BNtrack (object);

  try
    {
      value = getMember ("BNtrack", obj, variable);
    }
  catch (...)
    {
      if(variable == "detectorEta") value = getDetectorEta(object.eta, object.dZ);
      else if(variable == "detectorEtaMinusEta") value = getDetectorEta(object.eta, object.dZ) - object.eta;

      else if (!NmissoutShiftUp_ && !NmissoutShiftDown_ && variable == "nHitsMissingOuter"){
        value = object.nHitsMissingOuter;
      }
      else if (NmissoutShiftUp_ && variable == "nHitsMissingOuter") {
        double r;
        double d = -99;
        d = object.nHitsMissingOuter;
        r = ((double) rand() / (RAND_MAX));
        if (r > 0.11) value = d;
        else {
          value = d += 1;
        }
      }
      else if (NmissoutShiftDown_ && variable == "nHitsMissingOuter") {
        double r;
        double d = -99;
        d = object.nHitsMissingOuter;
        r = ((double) rand() / (RAND_MAX));
        if (r > 0.11) value = d;
        else {
          if (d != 0) value = d+= -1;
        }
      }

      //user defined variables
      else if(variable == "d0wrtBS") value = (object.vx-events->at(0).BSx)*object.py/object.pt - (object.vy-events->at(0).BSy)*object.px/object.pt;
      else if(variable == "dZwrtBS") value = object.dZ - events->at(0).BSz;

      else if(variable == "dPhiMet") {
        if (const BNmet *met = chosenMET ()) {
          value = deltaPhi (object.phi, met->phi);
        } else value = numeric_limits<int>::min ();
      }

      else if (variable == "rhoCorrRp5")                 value = getRhoCorr();
      else if(variable == "caloTotDeltaRp5RhoCorr")     value = getTrkCaloTotRhoCorr(&object);
      else if(variable == "caloTotDeltaRp5ByPRhoCorr")  value = getTrkCaloTotRhoCorr(&object) / pMag;
      else if(variable == "depTrkRp5RhoCorr")           value = getTrkDepTrkRp5RhoCorr(&object);
      else if(variable == "depTrkRp3RhoCorr")           value = getTrkDepTrkRp3RhoCorr(&object);

      else if(variable == "depTrkRp5MinusPtRhoCorr")    {
        if ( (getTrkDepTrkRp5RhoCorr(&object) - object.pt) < 0 ) value = 0;
        else {value = (getTrkDepTrkRp5RhoCorr(&object) - object.pt );}
      }

      else if(variable == "depTrkRp3MinusPtRhoCorr")
        {
          if ( (getTrkDepTrkRp3RhoCorr(&object) - object.pt) < 0 ) value = 0;
          else {value = (getTrkDepTrkRp3RhoCorr(&object) - object.pt );}
        }

      else if(variable == "isIso")                      value = getTrkIsIso(&object, tracks.product());
      else if(variable == "isMatchedDeadEcal")          value = getTrkIsMatchedDeadEcal(&object);
      else if(variable == "isMatchedDeadEcalDet")          value = getTrkIsMatchedDeadEcalDet(&object);
        else if(variable == "trkDeadEcalDeltaR")          value = getTrkDeadEcalDeltaR(&object);
      else if(variable == "isMatchedBadCSC")            value = getTrkIsMatchedBadCSC  (&object);
      else if(variable == "ptRes")                      value = getTrkPtRes(&object);
      else if(variable == "ptTrue")                     value = getTrkPtTrue(&object, mcparticles.product());
      else if (variable == "d0wrtPV"){
        double vx = object.vx - chosenVertex ()->x,
          vy = object.vy - chosenVertex ()->y,
          px = object.px,
          py = object.py,
          pt = object.pt;
        value = (-vx * py + vy * px) / pt;
      }
      else if (variable == "dZwrtPV"){
        double vx = object.vx - chosenVertex ()->x,
          vy = object.vy - chosenVertex ()->y,
          vz = object.vz - chosenVertex ()->z,
          px = object.px,
          py = object.py,
          pz = object.pz,
          pt = object.pt;
        value = vz - (vx * px + vy * py)/pt * (pz/pt);
      }

/*      else if(variable == "deltaRMinSubLeadJet") {
        // calculate minimum deltaR between track and any other subleading jet
        double trkJetDeltaRMin = 99.;
        if (!jets.product()) clog << "ERROR:  cannot find deltaRMinSubLeadJet because jets collection is not initialized." << endl;
        for (uint ijet = 0; ijet<jets->size(); ijet++) {
          double isSubLeadingJet = valueLookup(&jets->at(ijet), "disappTrkSubLeadingJetID");
          if (!isSubLeadingJet) continue;  // only consider jets that pass the subleading jet ID criteria
          double jetEta = valueLookup(&jets->at(ijet), "eta");
          double jetPhi = valueLookup(&jets->at(ijet), "phi");
          double trkJetDeltaR = deltaR(object.eta, object.phi, jetEta, jetPhi);
          if (trkJetDeltaR < trkJetDeltaRMin) trkJetDeltaRMin = trkJetDeltaR;
        }
        value = trkJetDeltaRMin;
      }*/

/*      else if(variable == "deltaPhiMaxSubLeadJet") {
        // calculate maximum deltaPhi between track and any other subleading jet
        double trkJetDeltaPhiMax = -99.;
        if (!jets.product()) clog << "ERROR:  cannot find deltaPhiMaxSubLeadJet because jets collection is not initialized." << endl;
        for (uint ijet = 0; ijet<jets->size(); ijet++) {
          double isSubLeadingJet = valueLookup(&jets->at(ijet), "disappTrkSubLeadingJetID");
          if (!isSubLeadingJet) continue;  // only consider jets that pass the subleading jet ID criteria
          //      double jetEta = valueLookup(&jets->at(ijet), "eta", "", empty);
          double jetPhi = valueLookup(&jets->at(ijet), "phi");
          double trkJetDeltaPhi = fabs(deltaPhi(object.phi, jetPhi));
          if (trkJetDeltaPhi > trkJetDeltaPhiMax) trkJetDeltaPhiMax = trkJetDeltaPhi;
        }
        value = trkJetDeltaPhiMax;
      }*/

/*      else if(variable == "deltaRMinElecLooseMvaId") {
        // calculate minimum deltaR between track and any other electron passing loose mva ID, i.e., mvaNonTrigV0 > 0
        double trkElecDeltaRMin = 99.;
        if (!electrons.product()) clog << "ERROR:  cannot find deltaRMinElecLooseMvaId because electrons collection is not initialized." << endl;
        for (uint ielec = 0; ielec<electrons->size(); ielec++) {
          double mvaNonTrigV0 = valueLookup(&electrons->at(ielec), "mvaNonTrigV0");
          if (mvaNonTrigV0 < 0) continue;  // only consider electrons with mvaNonTrigV0 > 0
          double elecEta = valueLookup(&electrons->at(ielec), "eta");
          double elecPhi = valueLookup(&electrons->at(ielec), "phi");
          double trkElecDeltaR = deltaR(object.eta, object.phi, elecEta, elecPhi);
          if (trkElecDeltaR < trkElecDeltaRMin) trkElecDeltaRMin = trkElecDeltaR;
        }
        value = trkElecDeltaRMin;
      }*/

/*      else if(variable == "deltaRMinMuonLooseId") {
        // calculate minimum deltaR between track and any muon
        double trkMuonDeltaRMin = 99.;
        if (!muons.product()) clog << "ERROR:  cannot find deltaRMinMuonLooseId because muons collection is not initialized." << endl;
        for (uint imuon = 0; imuon<muons->size(); imuon++) {
          double muonEta = valueLookup(&muons->at(imuon), "eta");
          double muonPhi = valueLookup(&muons->at(imuon), "phi");
          double trkMuonDeltaR = deltaR(object.eta, object.phi, muonEta, muonPhi);
          if (trkMuonDeltaR < trkMuonDeltaRMin) trkMuonDeltaRMin = trkMuonDeltaR;
        }
        value = trkMuonDeltaRMin;
      }*/
/*      else if(variable == "deltaRMinSecMuonLooseId") {
        // calculate minimum deltaR between track and any secondary muon
        double trkMuonDeltaRMin = 99.;
        if (!secMuons.product()) clog << "ERROR:  cannot find deltaRMinSecMuonLooseId because secMuons collection is not initialized." << endl;
        for (uint imuon = 0; imuon<secMuons->size(); imuon++) {
          double muonEta = valueLookup(&secMuons->at(imuon), "eta");
          double muonPhi = valueLookup(&secMuons->at(imuon), "phi");
          double trkMuonDeltaR = deltaR(object.eta, object.phi, muonEta, muonPhi);
          if (trkMuonDeltaR < trkMuonDeltaRMin) trkMuonDeltaRMin = trkMuonDeltaR;
        }
        value = trkMuonDeltaRMin;
      }*/

/*      else if(variable == "deltaRMinSecMuonLooseIdGlobal") {
        // calculate minimum deltaR between track and any other loose-Id muon
        double trkMuonDeltaRMin = 99.;
        if (!secMuons.product()) clog << "ERROR:  cannot find deltaRMinSecMuonLooseIdGlobal because secMuons collection is not initialized." << endl;
        for (uint imuon = 0; imuon<secMuons->size(); imuon++) {
          double isLooseIdMuon = valueLookup(&secMuons->at(imuon), "looseIDGlobalMuon");
          if (!isLooseIdMuon) continue;  // only consider muons that pass the looseIDGlobal criteria
          double muonEta = valueLookup(&secMuons->at(imuon), "eta");
          double muonPhi = valueLookup(&secMuons->at(imuon), "phi");
          double trkMuonDeltaR = deltaR(object.eta, object.phi, muonEta, muonPhi);
          if (trkMuonDeltaR < trkMuonDeltaRMin) trkMuonDeltaRMin = trkMuonDeltaR;
        }
        value = trkMuonDeltaRMin;
      }*/

/*      else if(variable == "deltaRMinTauLooseHadronicId") {
        // calculate minimum deltaR between track and any other tau passing loose hadronic ID
        double trkTauDeltaRMin = 99.;
        if (!taus.product()) clog << "ERROR:  cannot find deltaRMinTauLooseHadronicId because taus collection is not initialized." << endl;
        for (uint itau = 0; itau<taus->size(); itau++) {
          double isIdTau = valueLookup(&taus->at(itau), "looseHadronicID");
          if (!isIdTau) continue;  // only consider taus that pass the loose hadronic ID criteria
          double tauEta = valueLookup(&taus->at(itau), "eta");
          double tauPhi = valueLookup(&taus->at(itau), "phi");
          double trkTauDeltaR = deltaR(object.eta, object.phi, tauEta, tauPhi);
          if (trkTauDeltaR < trkTauDeltaRMin) trkTauDeltaRMin = trkTauDeltaR;
        }
        value = trkTauDeltaRMin;
      }*/

/*      else if(variable == "deltaPhiMuMuPair") {
        // delta phi between track and dimuon pair
        // Set to dummy value if there are not 2 muons with pT>25, opposite sign, and 80<m(mu-mu)<100 GeV.

        TLorentzVector p4muon0;  // first muon
        TLorentzVector p4muon1;  // second muon
        TLorentzVector p4mumu;   // dimuon pair
        double mMuon = 0.105658;  // mass from PDG
        int imuon0 = -1;
        int imuon1 = -1;
        double phiMuMu = 0;
        for (uint imuon = 0; imuon<muons->size(); imuon++) {
          double pt0   = valueLookup(&muons->at(imuon), "pt");
          double eta0  = valueLookup(&muons->at(imuon), "eta");
          double phi0  = valueLookup(&muons->at(imuon), "phi");
          double chg0  = valueLookup(&muons->at(imuon), "charge");
          if (pt0<25) continue;
          for (uint jmuon = imuon+1; jmuon<muons->size(); jmuon++) {
            double pt1   = valueLookup(&muons->at(jmuon), "pt");
            double eta1  = valueLookup(&muons->at(jmuon), "eta");
            double phi1  = valueLookup(&muons->at(jmuon), "phi");
            double chg1  = valueLookup(&muons->at(jmuon), "charge");
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
        if (imuon0>=0 && imuon1>=0) value = deltaPhi(phiMuMu, object.phi);  // only set the value if two muons pass the given criteria

      }*/

      else if(variable == "isNotSigOrMatchedToSusy") {
        // Return true if the input dataset is not signal MC or if the track is matched to a SUSY particle.
        if (datasetType_ != "signalMC") { value = 1; } else {
          int index = getGenMatchedParticleIndex(&object);
          if (index == -1) value = 0;  // not matched
          else {
            int pdgId = abs(mcparticles->at(index).id);
            if (pdgId>1000001 && pdgId<3160113) value = 1;  // matched to a SUSY particle
            else                                value = 0;
          }
        }
      }

      else if(variable == "genDeltaRLowest") value = getGenDeltaRLowest(&object);

      else if(variable == "genMatchedPdgId"){
        int index = getGenMatchedParticleIndex(&object);
        if(index == -1) value = 0;
        else value = mcparticles->at(index).id;
      }

      else if(variable == "genMatchedId"){
        int index = getGenMatchedParticleIndex(&object);
        if(index == -1) value = 0;
        else value = getPdgIdBinValue(mcparticles->at(index).id);
      }
      else if(variable == "genMatchedMotherId"){
        int index = getGenMatchedParticleIndex(&object);
        if(index == -1) value = 0;
        else value = getPdgIdBinValue(mcparticles->at(index).motherId);
      }
      else if(variable == "genMatchedMotherIdReverse"){
        int index = getGenMatchedParticleIndex(&object);
        if(index == -1) value = 24;
        else value = 24 -getPdgIdBinValue(mcparticles->at(index).motherId);
      }
      else if(variable == "genMatchedGrandmotherId"){
        int index = getGenMatchedParticleIndex(&object);
        if(index == -1) value = 0;
        else if(fabs(mcparticles->at(index).motherId) == 15){
          int motherIndex = findTauMotherIndex(&mcparticles->at(index));
          if(motherIndex == -1) value = 0;
          else value = getPdgIdBinValue(mcparticles->at(motherIndex).motherId);
        }
        else value = getPdgIdBinValue(mcparticles->at(index).grandMotherId);
      }
      else if(variable == "genMatchedGrandmotherIdReverse"){
        int index = getGenMatchedParticleIndex(&object);
        if(index == -1) value = 24;
        else if(fabs(mcparticles->at(index).motherId) == 15){
          int motherIndex = findTauMotherIndex(&mcparticles->at(index));
          if(motherIndex == -1) value = 24;
          else value = 24 - getPdgIdBinValue(mcparticles->at(motherIndex).motherId);
        }
        else value = 24 - getPdgIdBinValue(mcparticles->at(index).grandMotherId);
      }

      else
        clog << "WARNING: invalid track variable '" << variable << "'\n";
    }

  delete obj;
  return value;
} // end track valueLookup
