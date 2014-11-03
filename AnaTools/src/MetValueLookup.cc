#include <iostream>

#include "TVector2.h"

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!met valueLookup
double
ValueLookup::valueLookup (const BNmet &object, string variable){

  double value = numeric_limits<int>::min ();
  BNmet *obj = new BNmet (object);

  try
    {
      value = getMember ("BNmet", obj, variable);
    }
  catch (...)
    {
      if(variable == "hltPt") value = getPtSingleObjectMatchesAnyTrigger(object.phi, targetTriggers_, trigobjs.product());

/*      else if(variable == "metNoMu") {
        // Calculate the MET, without including muons in the sum of E_T
        TVector2 p2Met;
        TVector2 p2Muon;
        p2Met. SetMagPhi(object.pt, object.phi);
        p2Muon.SetMagPhi(0, 0);
        if (!muons.product()) clog << "ERROR: cannot find metNoMu because muons collection is not initialized." << endl;
        for (uint imuon = 0; imuon<muons->size(); imuon++) {
          double muonPt  = valueLookup(&muons->at(imuon), "pt");
          double muonPhi = valueLookup(&muons->at(imuon), "phi");
          TVector2 p2MuonTmp;
          p2MuonTmp.SetMagPhi(muonPt, muonPhi);
          p2Muon += p2MuonTmp;
        }
        TVector2 p2MetNoMu = p2Met + p2Muon;
        value = p2MetNoMu.Mod();
      }*/

/*      else if(variable == "hltPtNoMu") {
        // Calculate the MET, without including muons in the sum of E_T
        TVector2 p2Met;
        TVector2 p2Muon;
        p2Met. SetMagPhi(object.pt, object.phi);
        p2Muon.SetMagPhi(0, 0);
        if (!muons.product()) clog << "ERROR: cannot find metNoMu because muons collection is not initialized." << endl;
        for (uint imuon = 0; imuon<muons->size(); imuon++) {
          double muonPt  = valueLookup(&muons->at(imuon), "pt");
          double muonPhi = valueLookup(&muons->at(imuon), "phi");
          TVector2 p2MuonTmp;
          p2MuonTmp.SetMagPhi(muonPt, muonPhi);
          p2Muon += p2MuonTmp;
        }
        TVector2 p2MetNoMu = p2Met + p2Muon;
        //    value = p2MetNoMu.Mod();
        value = getPtSingleObjectMatchesAnyTrigger(p2MetNoMu.Phi(), targetTriggers_, trigobjs.product());
      }*/
/*      else if(variable == "metNoElec") {
        // Calculate the MET, without including electrons in the sum of E_T
        TVector2 p2Met;
        TVector2 p2Elec;
        p2Met. SetMagPhi(object.pt, object.phi);
        p2Elec.SetMagPhi(0, 0);
        if (!electrons.product()) clog << "ERROR: cannot find metNoElec because electron collection is not initialized." << endl;
        for (uint ielec = 0; ielec<electrons->size(); ielec++) {
          double elecPt  = valueLookup(&electrons->at(ielec), "pt");
          double elecPhi = valueLookup(&electrons->at(ielec), "phi");
          TVector2 p2ElecTmp;
          p2ElecTmp.SetMagPhi(elecPt, elecPhi);
          p2Elec += p2ElecTmp;
        }
        TVector2 p2MetNoElec = p2Met + p2Elec;
        value =  p2MetNoElec.Mod();
      }*/
/*      else if(variable == "deltaPhiMin2Jets") {
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
          double jetPt  = valueLookup(&jets->at(ijet), "pt");
          double jetEta = valueLookup(&jets->at(ijet), "eta");
          double jetPhi = valueLookup(&jets->at(ijet), "phi");
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
          double dPhi = fabs(deltaPhi (phiJet1, object.phi));
          if (dPhi<deltaPhiMin2Jets) deltaPhiMin2Jets = dPhi;
        }
        if (ptJet2 >=0) {
          double dPhi = fabs(deltaPhi (phiJet2, object.phi));
          if (dPhi<deltaPhiMin2Jets) deltaPhiMin2Jets = dPhi;
        }
        value = deltaPhiMin2Jets;
        if (verbose_>4) clog  << "  Met phi==" << object.phi << endl;
        if (verbose_>4) clog << "  Calculated value:  deltaPhiMin2Jets=" << deltaPhiMin2Jets << endl;
      }*/

/*      else if(variable == "deltaPhiJet1") {
        // |deltaPhi| between met vector and leading jet
        // only consider jets with pt > 45 GeV and |eta|<2.8
        if (!jets.product()) clog << "ERROR:  cannot find deltaPhiJet1 because jets collection is not initialized." << endl;
        double ptJet1 = -99; // leading jet
        double phiJet1 = -99;
        if (verbose_>4) clog << "Debug:  calculating deltaPhiJet1" << endl;
        for (uint ijet = 0; ijet<jets->size(); ijet++) {
          // find indices of candidate jets
          double jetPt  = valueLookup(&jets->at(ijet), "pt");
          double jetEta = valueLookup(&jets->at(ijet), "eta");
          double jetPhi = valueLookup(&jets->at(ijet), "phi");
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
          double dPhi = fabs(deltaPhi (phiJet1, object.phi));
          deltaPhiJet1 = dPhi;
        }
        value = deltaPhiJet1;
      }*/

/*      else if(variable == "deltaPhiJet2") {
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
          double jetPt  = valueLookup(&jets->at(ijet), "pt");
          double jetEta = valueLookup(&jets->at(ijet), "eta");
          double jetPhi = valueLookup(&jets->at(ijet), "phi");
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
          double dPhi = fabs(deltaPhi (phiJet2, object.phi));
          deltaPhiJet2 = dPhi;
        }
        value = deltaPhiJet2;
      }*/

      else
        clog << "WARNING: invalid met variable '" << variable << "'\n";
    }

  delete obj;
  return value;
} // end met valueLookup
