#include <iostream>

#include "TLorentzVector.h"
#include "TVector2.h"

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!event valueLookup
double
ValueLookup::valueLookup (const BNevent &object, string variable){

  double value = numeric_limits<int>::min ();
  BNevent *obj = new BNevent (object);

  try
    {
      value = getMember ("BNevent", obj, variable);
    }
  catch (...)
    {
      if(variable == "nExtraPartons") value = GetNumExtraPartons(mcparticles.product());
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
        } else value = numeric_limits<int>::min ();
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
        } else value = numeric_limits<int>::min ();
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
        } else value = numeric_limits<int>::min ();

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
      else if(variable == "totalJetPhiMinusTotalMcparticlePhi") {
        value = deltaPhi(valueLookup(object, "totalJetPhi"),
                         valueLookup(object, "totalMcparticlePhi"));
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

      else
        clog << "WARNING: invalid event variable '" << variable << "'\n";
    }

  delete obj;
  return value;
} // end event valueLookup
