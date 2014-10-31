#include <iostream>

#include "TVector2.h"

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!electron valueLookup
double
ValueLookup::valueLookup (const BNelectron &object, string variable){

  double value = 0.0;
  double pMag = sqrt(object.pt * object.pt +
                     object.pz * object.pz);
  BNelectron *obj = new BNelectron (object);

  try
    {
      value = getMember ("BNelectron", obj, variable);
    }
  catch (...)
    {
      if(variable == "scEnergyRes") value = (object.scEnergy-object.scRawEnergy)/object.scRawEnergy;
      else if(variable == "scRawOverCorrectedEnergy") value = (object.scRawEnergy)/object.scEnergy;

      //user-defined variables
      else if(variable == "EOverP") value = (object.energy)/pMag;
      else if(variable == "vertexDistZ") value = object.vz - chosenVertex()->z;
      else if(variable == "correctedD0VertexErr") value =  hypot (object.tkD0err, hypot (chosenVertex ()->xError, chosenVertex ()->yError));
      else if(variable == "correctedD0VertexSig") value =  object.correctedD0Vertex / hypot (object.tkD0err, hypot (chosenVertex ()->xError, chosenVertex ()->yError));
      else if(variable == "correctedD0Err") value =  hypot (object.tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
      else if(variable == "correctedD0Sig") value =  object.correctedD0 / hypot (object.tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
      else if(variable == "detIso") value = (object.trackIso) / object.pt;
      else if(variable == "relPFrhoIso") value = ( object.chargedHadronIsoDR03 + max(0.0, object.neutralHadronIsoDR03 + object.photonIsoDR03 - object.AEffDr03*object.rhoPrime) ) / object.pt;
      else if(variable == "relPFrhoIsoNoPUSubtracted") value = ( object.chargedHadronIsoDR03 + max(0.0, object.neutralHadronIsoDR03 + object.photonIsoDR03) ) / object.pt;
      else if(variable == "PUInrelPFrhoIso") value = (object.AEffDr03*object.rhoPrime)/ object.pt;
      else if(variable == "ChargedHadInrelPFrhoIso") value = (object.chargedHadronIsoDR03)/ object.pt;
      else if(variable == "NeutralHadInrelPFrhoIso") value = (object.neutralHadronIsoDR03)/ object.pt;
      else if(variable == "PhotonInrelPFrhoIso") value = (object.photonIsoDR03)/ object.pt;
      else if(variable == "relPFrhoIsoEB") value = object.isEB ? ( object.chargedHadronIsoDR03 + max(0.0, object.neutralHadronIsoDR03 + object.photonIsoDR03 - object.AEffDr03*object.rhoPrime) ) / object.pt : numeric_limits<int>::min ();
      else if(variable == "relPFrhoIsoEE") value = object.isEE ? ( object.chargedHadronIsoDR03 + max(0.0, object.neutralHadronIsoDR03 + object.photonIsoDR03 - object.AEffDr03*object.rhoPrime) ) / object.pt : numeric_limits<int>::min ();
      else if(variable == "metMT") {
        if (const BNmet *met = chosenMET ())
          {
            double dPhi = deltaPhi (object.phi, met->phi);
            value = sqrt (2 * object.pt * met->pt * (1 - cos (dPhi)));
          }
        else
          value = numeric_limits<int>::min ();
      }
      else if(variable == "metDeltaPhi") {
        if (const BNmet *met = chosenMET ())
          value = deltaPhi (object.phi, met->phi);
        else
          value = numeric_limits<int>::min ();
      }
      else if(variable == "ptPlusMet") {
        // Calculate the magnitude of the vector sum of the electron pT and the Met.
        if (const BNmet *met = chosenMET ())
          {
            TVector2 p2Met;
            TVector2 p2Elec;
            p2Met. SetMagPhi(   met->pt,    met->phi);
            p2Elec.SetMagPhi(object.pt, object.phi);
            TVector2 p2MetElec = p2Met + p2Elec;
            value = p2MetElec.Mod();
          }
        else
          value = numeric_limits<int>::min ();
      }

      else if(variable == "correctedD0VertexEEPositiveChargeLowPt"){
        if(fabs(object.eta) > 1.479 && object.charge > 0 && object.pt > 45) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0VertexEEPositiveChargeHighPt"){
        if(fabs(object.eta) > 1.479 && object.charge > 0 && object.pt < 45) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }

      else if(variable == "correctedD0VertexInEBPlus"){
        if(fabs(object.eta) < 0.8 && object.eta > 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0VertexOutEBPlus"){
        if(object.isEB && fabs(object.eta) > 0.8 && object.eta > 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0VertexEEPlus"){
        if(object.isEE && object.eta > 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }

      else if(variable == "correctedD0BeamspotInEBPlus"){
        if(fabs(object.eta) < 0.8 && object.eta > 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0BeamspotOutEBPlus"){
        if(object.isEB && fabs(object.eta) > 0.8 && object.eta > 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0BeamspotEEPlus"){
        if(object.isEE && object.eta > 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }

      else if(variable == "correctedD0VertexInEBMinus"){
        if(fabs(object.eta) < 0.8 && object.eta < 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0VertexOutEBMinus"){
        if(object.isEB && fabs(object.eta) > 0.8 && object.eta < 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0VertexEEMinus"){
        if(object.isEE && object.eta < 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }

      else if(variable == "correctedD0BeamspotInEBMinus"){
        if(fabs(object.eta) < 0.8 && object.eta < 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0BeamspotOutEBMinus"){
        if(object.isEB && fabs(object.eta) > 0.8 && object.eta < 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0BeamspotEEMinus"){
        if(object.isEE && object.eta < 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }

      else if(variable == "looseID"){
        if (object.isEB)
          {
            value = fabs(object.delEtaIn) < 0.007 \
              && fabs (object.delPhiIn) < 0.15 \
              && object.scSigmaIEtaIEta < 0.01 \
              && object.hadOverEm < 0.12 \
              && fabs (object.correctedD0Vertex) < 0.02 \
              && fabs (object.correctedDZ) < 0.2 \
              && object.absInvEMinusInvPin < 0.05 \
              && object.passConvVeto;
          }
        else
          {
            value = fabs(object.delEtaIn) < 0.009 \
              && fabs (object.delPhiIn) < 0.10 \
              && object.scSigmaIEtaIEta < 0.03 \
              && object.hadOverEm < 0.10 \
              && fabs (object.correctedD0Vertex) < 0.02 \
              && fabs (object.correctedDZ) < 0.2 \
              && object.absInvEMinusInvPin < 0.05 \
              && object.passConvVeto;
          }
      }

      else if(variable == "tightID"){
        if (object.isEB)
          {
            value = fabs(object.delEtaIn) < 0.004 \
              && fabs (object.delPhiIn) < 0.03 \
              && object.scSigmaIEtaIEta < 0.01 \
              && object.hadOverEm < 0.12 \
              && fabs (object.correctedD0Vertex) < 0.02 \
              && fabs (object.correctedDZ) < 0.1 \
              && object.absInvEMinusInvPin < 0.05 \
              && object.passConvVeto;
          }
        else
          {
            value = fabs(object.delEtaIn) < 0.005 \
              && fabs (object.delPhiIn) < 0.02 \
              && object.scSigmaIEtaIEta < 0.03 \
              && object.hadOverEm < 0.10 \
              && fabs (object.correctedD0Vertex) < 0.02 \
              && fabs (object.correctedDZ) < 0.1 \
              && object.absInvEMinusInvPin < 0.05 \
              && object.passConvVeto;
          }
      }

      else if(variable == "mvaNonTrig_HtoZZto4l"){ // https://twiki.cern.ch/twiki/bin/view/CMS/MultivariateElectronIdentification#Non_triggering_MVA
        value = false;
        if (object.pt > 7.0 && object.pt < 10.0)
          {
            if (abs (object.scEta) < 0.8 && object.mvaNonTrigV0 > 0.47)
              value = true;
            else if (abs (object.scEta) < 1.479 && object.mvaNonTrigV0 > 0.004)
              value = true;
            else if (abs (object.scEta) < 2.5 && object.mvaNonTrigV0 > 0.295)
              value = true;
          }
        else if (object.pt >= 10.0)
          {
            if (abs (object.scEta) < 0.8 && object.mvaNonTrigV0 > -0.34)
              value = true;
            else if (abs (object.scEta) < 1.479 && object.mvaNonTrigV0 > -0.65)
              value = true;
            else if (abs (object.scEta) < 2.5 && object.mvaNonTrigV0 > 0.60)
              value = true;
          }
      }
      else if(variable == "mvaTrig_HtoWWto2l2nu"){ // https://twiki.cern.ch/twiki/bin/view/CMS/MultivariateElectronIdentification#Triggering_MVA
        value = false;
        if (object.pt > 10.0 && object.pt < 20.0)
          {
            if (abs (object.scEta) < 0.8 && object.mvaTrigV0 > 0.00)
              value = true;
            else if (abs (object.scEta) < 1.479 && object.mvaTrigV0 > 0.10)
              value = true;
            else if (abs (object.scEta) < 2.5 && object.mvaTrigV0 > 0.62)
              value = true;
          }
        else if (object.pt >= 20.0)
          {
            if (abs (object.scEta) < 0.8 && object.mvaTrigV0 > 0.94)
              value = true;
            else if (abs (object.scEta) < 1.479 && object.mvaTrigV0 > 0.85)
              value = true;
            else if (abs (object.scEta) < 2.5 && object.mvaTrigV0 > 0.92)
              value = true;
          }
      }

      else if(variable == "looseID_MVA"){
        value = object.pt > 10
          && object.mvaNonTrigV0 > 0;
          }
      else if(variable == "correctedD0VertexInEBPositiveCharge"){
        if(fabs(object.eta) < 0.8 && object.charge > 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0VertexOutEBPositiveCharge"){
        if(object.isEB && fabs(object.eta) > 0.8 && object.charge > 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0VertexEEPositiveCharge"){
        if(object.isEE && object.charge > 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }

      else if(variable == "correctedD0BeamspotInEBPositiveCharge"){
        if(fabs(object.eta) < 0.8 && object.charge > 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0BeamspotOutEBPositiveCharge"){
        if(object.isEB && fabs(object.eta) > 0.8 && object.charge > 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0BeamspotEEPositiveCharge"){
        if(object.isEE && object.charge > 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }

      else if(variable == "correctedD0VertexInEBNegativeCharge"){
        if(fabs(object.eta) < 0.8 && object.charge < 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0VertexOutEBNegativeCharge"){
        if(object.isEB && fabs(object.eta) > 0.8 && object.charge < 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0VertexEENegativeCharge"){
        if(object.isEE && object.charge < 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }

      else if(variable == "correctedD0BeamspotInEBNegativeCharge"){
        if(fabs(object.eta) < 0.8 && object.charge < 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0BeamspotOutEBNegativeCharge"){
        if(object.isEB && fabs(object.eta) > 0.8 && object.charge < 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0BeamspotEENegativeCharge"){
        if(object.isEE && object.charge < 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }


      else if(variable == "tightIDdisplaced"){
        if (object.isEB)
          {
            value = fabs(object.delEtaIn) < 0.004 \
              && fabs (object.delPhiIn) < 0.03 \
              && object.scSigmaIEtaIEta < 0.01 \
              && object.hadOverEm < 0.12 \
              && object.absInvEMinusInvPin < 0.05;
          }
        else
          {
            value = fabs (object.delEtaIn) < 0.005 \
              && fabs (object.delPhiIn) < 0.02 \
              && object.scSigmaIEtaIEta < 0.03 \
              && object.hadOverEm < 0.10 \
              && object.absInvEMinusInvPin < 0.05;
          }
      }


      else if(variable == "genDeltaRLowest") value = getGenDeltaRLowest(&object);

      else if(variable == "genMatchedPdgId"){
        int index = getGenMatchedParticleIndex(&object);
        if(index == -1) value = 0;
        else value = mcparticles->at(index).id;
      }

      else if(variable == "genRecoChargeProduct"){
        int index = getGenMatchedParticleIndex(&object);
        if(index == -1) value = 2;
        else value = object.charge * mcparticles->at(index).charge;
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
      else if(variable == "pfElectronsFromVertex"){
        double d0Error, dzError;

        d0Error = hypot (object.tkD0err, hypot (chosenVertex ()->xError, chosenVertex ()->yError));
        dzError = hypot (object.tkDZerr, chosenVertex ()->zError);
        value = fabs (object.correctedD0Vertex) > 0.2 || fabs (object.correctedDZ) > 0.5
          || fabs (object.correctedD0Vertex / d0Error) > 99.0
          || fabs (object.correctedDZ / dzError) > 99.0;
        value = !value;
      }

      else{
        clog << "WARNING: invalid electron variable '" << variable << "'\n";
        value = numeric_limits<int>::min ();
      }
    }

  delete obj;
  return value;
} // end electron valueLookup
