#include <iostream>

#include "TVector2.h"

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!muon valueLookup
double
ValueLookup::valueLookup (const BNmuon &object, string variable){

  double value = 0.0;
  BNmuon *obj = new BNmuon (object);

  try
    {
      value = getMember ("BNmuon", obj, variable);
    }
  catch (...)
    {
      //user-defined variables
      if(variable == "relpfIsoR04SumExceptChargedHad") value = (object.pfIsoR04SumNeutralHadronEt + object.pfIsoR04SumPhotonEt - 0.5*object.pfIsoR04SumPUPt)/ object.pt;
      else if(variable == "time_between_ecal_hcal") value = object.hcal_time - object.ecal_time;
      else if(variable == "looseID") {
        value = object.pt > 10 &&
          (object.isGlobalMuon  > 0 ||
           object.isTrackerMuon > 0);
      }
      else if(variable == "looseIDGlobalMuon") {
        value = object.pt > 10 &&
          object.isGlobalMuon  > 0;
      }
      else if(variable == "vertexDistZ") value = object.vz - chosenVertex()->z;
      else if(variable == "correctedD0VertexErr") value =  hypot (object.tkD0err, hypot (chosenVertex ()->xError, chosenVertex ()->yError));
      else if(variable == "correctedD0VertexSig") value =  object.correctedD0Vertex / hypot (object.tkD0err, hypot (chosenVertex ()->xError, chosenVertex ()->yError));
      else if(variable == "correctedD0Err") value =  hypot (object.tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
      else if(variable == "correctedD0Sig") value =  object.correctedD0 / hypot (object.tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
      else if(variable == "detIso") value = (object.trackIsoDR03) / object.pt;
      else if(variable == "relPFdBetaIso") value = (object.pfIsoR04SumChargedHadronPt + max(0.0, object.pfIsoR04SumNeutralHadronEt + object.pfIsoR04SumPhotonEt - 0.5*object.pfIsoR04SumPUPt)) / object.pt;
      else if(variable == "PUInrelPFdBetaIso") value = (object.pfIsoR04SumPUPt)/ object.pt;
      else if(variable == "ChargedHadInrelPFdBetaIso") value = (object.pfIsoR04SumChargedHadronPt)/ object.pt;
      else if(variable == "NeutralHadInrelPFdBetaIso") value = (object.pfIsoR04SumNeutralHadronEt)/ object.pt;
      else if(variable == "PhotonInrelPFdBetaIso") value = (object.pfIsoR04SumPhotonEt)/ object.pt;
      else if(variable == "relPFdBetaIsoNoPUSubtracted") value = (object.pfIsoR04SumChargedHadronPt + max(0.0, object.pfIsoR04SumNeutralHadronEt + object.pfIsoR04SumPhotonEt)) / object.pt;
      else if(variable == "relPFdBetaIsoNew") value = (object.chargedHadronIsoDR04 + max(0.0, object.neutralHadronIsoDR04 + object.photonIsoDR04 - 0.5*object.puChargedHadronIsoDR04)) / object.pt;
      else if(variable == "relPFdBetaIsoPseudo") value = (object.pfIsoR04SumChargedHadronPt + object.pfIsoR04SumNeutralHadronEt + object.pfIsoR04SumPhotonEt - 0.5*object.pfIsoR04SumPUPt) / object.pt;
      else if(variable == "relPFrhoIso") value = ( object.chargedHadronIso + max(0.0, object.neutralHadronIso + object.photonIso - object.AEffDr03*object.rhoPrime) ) / object.pt;
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
        // Calculate the magnitude of the vector sum of the muon pT and the Met.
        if (const BNmet *met = chosenMET ())
          {
            TVector2 p2Met;
            TVector2 p2Muon;
            p2Met. SetMagPhi(   met->pt,    met->phi);
            p2Muon.SetMagPhi(object.pt, object.phi);
            TVector2 p2MetMuon = p2Met + p2Muon;
            value = p2MetMuon.Mod();
          }
        else
          value = numeric_limits<int>::min ();
      }

      else if(variable == "correctedD0VertexInEBPlus"){
        if(fabs(object.eta) < 0.8 && object.eta > 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0VertexOutEBPlus"){
        if(fabs(object.eta) < 1.479 && fabs(object.eta) > 0.8 && object.eta > 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0VertexEEPlus"){
        if(fabs(object.eta) > 1.479 && object.eta > 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }

      else if(variable == "correctedD0BeamspotInEBPlus"){
        if(fabs(object.eta) < 0.8 && object.eta > 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0BeamspotOutEBPlus"){
        if(fabs(object.eta) < 1.479 && fabs(object.eta) > 0.8 && object.eta > 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0BeamspotEEPlus"){
        if(fabs(object.eta) > 1.479 && object.eta > 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }

      else if(variable == "correctedD0VertexInEBMinus"){
        if(fabs(object.eta) < 0.8 && object.eta < 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0VertexOutEBMinus"){
        if(fabs(object.eta) < 1.479 && fabs(object.eta) > 0.8 && object.eta < 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0VertexEEMinus"){
        if(fabs(object.eta) > 1.479 && object.eta < 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }

      else if(variable == "correctedD0BeamspotInEBMinus"){
        if(fabs(object.eta) < 0.8 && object.eta < 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0BeamspotOutEBMinus"){
        if(fabs(object.eta) < 1.479 && fabs(object.eta) > 0.8 && object.eta < 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0BeamspotEEMinus"){
        if(fabs(object.eta) > 1.479 && object.eta < 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }


      else if(variable == "correctedD0VertexInEBPositiveCharge"){
        if(fabs(object.eta) < 0.8 && object.charge > 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0VertexOutEBPositiveCharge"){
        if(fabs(object.eta) < 1.479 && fabs(object.eta) > 0.8 && object.charge > 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0VertexEEPositiveCharge"){
        if(fabs(object.eta) > 1.479 && object.charge > 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }

      else if(variable == "correctedD0BeamspotInEBPositiveCharge"){
        if(fabs(object.eta) < 0.8 && object.charge > 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0BeamspotOutEBPositiveCharge"){
        if(fabs(object.eta) < 1.479 && fabs(object.eta) > 0.8 && object.charge > 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0BeamspotEEPositiveCharge"){
        if(fabs(object.eta) > 1.479 && object.charge > 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }

      else if(variable == "correctedD0VertexInEBNegativeCharge"){
        if(fabs(object.eta) < 0.8 && object.charge < 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0VertexOutEBNegativeCharge"){
        if(fabs(object.eta) < 1.479 && fabs(object.eta) > 0.8 && object.charge < 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0VertexEENegativeCharge"){
        if(fabs(object.eta) > 1.479 && object.charge < 0) value = object.correctedD0Vertex;
        else value = numeric_limits<int>::min ();
      }

      else if(variable == "correctedD0BeamspotInEBNegativeCharge"){
        if(fabs(object.eta) < 0.8 && object.charge < 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0BeamspotOutEBNegativeCharge"){
        if(fabs(object.eta) < 1.479 && fabs(object.eta) > 0.8 && object.charge < 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }
      else if(variable == "correctedD0BeamspotEENegativeCharge"){
        if(fabs(object.eta) > 1.479 && object.charge < 0) value = object.correctedD0;
        else value = numeric_limits<int>::min ();
      }


      else if(variable == "tightID") {
        // Defined in https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId#Tight_Muon
        value = object.isGlobalMuon > 0                \
          && object.isPFMuon > 0                        \
          && object.normalizedChi2 < 10                \
          && object.numberOfValidMuonHits > 0        \
          && object.numberOfMatchedStations > 1        \
          && fabs(object.correctedD0Vertex) < 0.2        \
          && fabs(object.correctedDZ) < 0.5        \
          && object.numberOfValidPixelHits > 0                \
          && object.numberOfLayersWithMeasurement > 5;
      }
      else if(variable == "tightIDdisplaced"){
        value = object.isGlobalMuon > 0                \
          && object.isPFMuon > 0                        \
          && object.normalizedChi2 < 10                \
          && object.numberOfValidMuonHits > 0        \
          && object.numberOfMatchedStations > 1        \
          && object.numberOfValidPixelHits > 0        \
          && object.numberOfLayersWithMeasurement > 5;
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
        else value = 24 - getPdgIdBinValue(mcparticles->at(index).motherId);
      }
      else if(variable == "genRecoChargeProduct"){
        int index = getGenMatchedParticleIndex(&object);
        if(index == -1) value = 2;
        else value = object.charge * mcparticles->at(index).charge;
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
      else if(variable == "pfMuonsFromVertex"){
        double d0Error, dzError;

        d0Error = hypot (object.tkD0err, hypot (chosenVertex ()->xError, chosenVertex ()->yError));
        dzError = hypot (object.tkDZerr, chosenVertex ()->zError);
        value = fabs (object.correctedD0Vertex) > 0.2 || fabs (object.correctedDZ) > 0.5
          || fabs (object.correctedD0Vertex / d0Error) > 99.0
          || fabs (object.correctedDZ / dzError) > 99.0;
        value = (object.isStandAloneMuon && !object.isTrackerMuon && !object.isGlobalMuon) || !value;
      }



      else{
        clog << "WARNING: invalid muon variable '" << variable << "'\n";
        value = numeric_limits<int>::min ();
      }
    }

  delete obj;
  return value;
} // end muon valueLookup
