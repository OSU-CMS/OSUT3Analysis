#include <iostream>

#include "TLorentzVector.h"
#include "TVector3.h"

#include "DataFormats/GeometrySurface/interface/Line.h"

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!muon-muon pair valueLookup
double
ValueLookup::valueLookup (const BNmuon &object1, const BNmuon &object2, string variable){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1.phi,object2.phi));
  else if(variable == "deltaEta") value = fabs(object1.eta - object2.eta);
  else if(variable == "deltaPt") value = fabs(object1.pt - object2.pt);
  else if(variable == "deltaR") value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1.px, object1.py, object1.pz, object1.energy);
    TLorentzVector fourVector2(object2.px, object2.py, object2.pz, object2.energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1.px, object1.py, object1.pz, object1.energy);
    TLorentzVector fourVector2(object2.px, object2.py, object2.pz, object2.energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1.px, object1.py, object1.pz);
      TVector3 threeVector2(object2.px, object2.py, object2.pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "alpha")
    {
      static const double pi = 3.1415926535897932384626433832795028841971693993751058;
      TVector3 threeVector1(object1.px, object1.py, object1.pz);
      TVector3 threeVector2(object2.px, object2.py, object2.pz);
      value = (pi-threeVector1.Angle(threeVector2));
    }
  else if(variable == "deltaCorrectedD0Vertex") value = object1.correctedD0Vertex - object2.correctedD0Vertex;
  else if(variable == "deltaAbsCorrectedD0Vertex") value = fabs(object1.correctedD0Vertex) - fabs(object2.correctedD0Vertex);
  else if(variable == "d0Sign"){
    double d0Sign = (object1.correctedD0Vertex*object2.correctedD0Vertex)/fabs(object1.correctedD0Vertex*object2.correctedD0Vertex);
    if(d0Sign < 0) value = -0.5;
    else if (d0Sign > 0) value = 0.5;
    else value = numeric_limits<int>::min ();
  }
  else if(variable == "chargeProduct"){
    value = object1.charge*object2.charge;
  }
  else if(variable == "muon1CorrectedD0Vertex"){
    value = object1.correctedD0Vertex;
  }
  else if(variable == "muon2CorrectedD0Vertex"){
    value = object2.correctedD0Vertex;
  }
  else if(variable == "muon1timeAtIpInOut"){
    value = object1.timeAtIpInOut;
  }
  else if(variable == "muon2timeAtIpInOut"){
    value = object2.timeAtIpInOut;
  }
  else if(variable == "muon1EcalTime"){ value = object1.ecal_time;
  }
  else if(variable == "muon2EcalTime"){ value = object2.ecal_time;
  }
  else if(variable == "muon1HcalTime"){ value = object1.hcal_time;
  }
  else if(variable == "muon2HcalTime"){ value = object2.hcal_time;
  }
  else if(variable == "muon1CorrectedD0")
    {
      value = object1.correctedD0;
    }
  else if(variable == "muon2CorrectedD0")
    {
      value = object2.correctedD0;
    }

  else{clog << "WARNING: invalid muon-muon pair variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}

  return value;
} // end muon-muon pair valueLookup


//!muon-photon pair valueLookup
double
ValueLookup::valueLookup (const BNmuon &object1, const BNphoton &object2, string variable){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1.phi,object2.phi));
  else if(variable == "deltaEta") value = fabs(object1.eta - object2.eta);
  else if(variable == "photonEta") value = object2.eta;
  else if(variable == "photonPt") value = object2.pt;
  else if(variable == "muonEta") value = object1.eta;
  else if(variable == "photonPhi") value = object2.phi;
  else if(variable == "muonPhi") value = object1.phi;
  else if(variable == "deltaR") value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
  else if(variable == "photonGenMotherId") value = object2.genMotherId;
  else if(variable == "muonRelPFdBetaIso") value = (object1.pfIsoR04SumChargedHadronPt + max(0.0, object1.pfIsoR04SumNeutralHadronEt + object1.pfIsoR04SumPhotonEt - 0.5*object1.pfIsoR04SumPUPt)) / object1.pt;
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1.px, object1.py, object1.pz, object1.energy);
    TLorentzVector fourVector2(object2.px, object2.py, object2.pz, object2.energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1.px, object1.py, object1.pz, object1.energy);
    TLorentzVector fourVector2(object2.px, object2.py, object2.pz, object2.energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1.px, object1.py, object1.pz);
      TVector3 threeVector2(object2.px, object2.py, object2.pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "alpha")
    {
      static const double pi = 3.1415926535897932384626433832795028841971693993751058;
      TVector3 threeVector1(object1.px, object1.py, object1.pz);
      TVector3 threeVector2(object2.px, object2.py, object2.pz);
      value = (pi-threeVector1.Angle(threeVector2));
    }
  else{clog << "WARNING: invalid muon-photon pair variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}

  return value;
} // end muon-photon pair valueLookup


//!electron-photon pair valueLookup
double
ValueLookup::valueLookup (const BNelectron &object1, const BNphoton &object2, string variable){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1.phi,object2.phi));
  else if(variable == "deltaEta") value = fabs(object1.eta - object2.eta);
  else if(variable == "photonEta") value = object2.eta;
  else if(variable == "photonPt") value = object2.pt;
  else if(variable == "electronEta") value = object1.eta;
  else if(variable == "photonPhi") value = object2.phi;
  else if(variable == "electronPhi") value = object1.phi;
  else if(variable == "deltaR") value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
  else if(variable == "photonGenMotherId") value = object2.genMotherId;
  else if(variable == "electronRelPFrhoIso") value = ( object1.chargedHadronIsoDR03 + max(0.0, object1.neutralHadronIsoDR03 + object1.photonIsoDR03 - object1.AEffDr03*object1.rhoPrime) ) / object1.pt;
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1.px, object1.py, object1.pz, object1.energy);
    TLorentzVector fourVector2(object2.px, object2.py, object2.pz, object2.energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1.px, object1.py, object1.pz, object1.energy);
    TLorentzVector fourVector2(object2.px, object2.py, object2.pz, object2.energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1.px, object1.py, object1.pz);
      TVector3 threeVector2(object2.px, object2.py, object2.pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "alpha")
    {
      static const double pi = 3.1415926535897932384626433832795028841971693993751058;
      TVector3 threeVector1(object1.px, object1.py, object1.pz);
      TVector3 threeVector2(object2.px, object2.py, object2.pz);
      value = (pi-threeVector1.Angle(threeVector2));
    }
  else{clog << "WARNING: invalid electron-photon pair variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}

  return value;
} // end electron-photon pair valueLookup


//!electron-electron pair valueLookup
double
ValueLookup::valueLookup (const BNelectron &object1, const BNelectron &object2, string variable){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1.phi,object2.phi));
  else if(variable == "deltaEta") value = fabs(object1.eta - object2.eta);
  else if(variable == "deltaR") value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1.px, object1.py, object1.pz, object1.energy);
    TLorentzVector fourVector2(object2.px, object2.py, object2.pz, object2.energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1.px, object1.py, object1.pz, object1.energy);
    TLorentzVector fourVector2(object2.px, object2.py, object2.pz, object2.energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1.px, object1.py, object1.pz);
      TVector3 threeVector2(object2.px, object2.py, object2.pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "deltaCorrectedD0Vertex") value = object1.correctedD0Vertex - object2.correctedD0Vertex;
  else if(variable == "deltaAbsCorrectedD0Vertex") value = fabs(object1.correctedD0Vertex) - fabs(object2.correctedD0Vertex);
  else if(variable == "d0Sign"){
    double d0Sign = (object1.correctedD0Vertex*object2.correctedD0Vertex)/fabs(object1.correctedD0Vertex*object2.correctedD0Vertex);
    if(d0Sign < 0) value = -0.5;
    else if (d0Sign > 0) value = 0.5;
    else value = numeric_limits<int>::min ();
  }
  else if(variable == "chargeProduct"){
    value = object1.charge*object2.charge;
  }
  else if(variable == "electron1CorrectedD0Vertex"){
    value = object1.correctedD0Vertex;
  }
  else if(variable == "electron2CorrectedD0Vertex"){
    value = object2.correctedD0Vertex;
  }
  else if(variable == "electron1CorrectedD0"){
    value = object1.correctedD0;
  }
  else if(variable == "electron2CorrectedD0"){
    value = object2.correctedD0;
  }

  else{clog << "WARNING: invalid electron-electron pair variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}

  return value;
} // end electron-electron pair valueLookup


//!electron-muon pair valueLookup
double
ValueLookup::valueLookup (const BNelectron &object1, const BNmuon &object2, string variable){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1.phi,object2.phi));
  else if(variable == "deltaEta") value = fabs(object1.eta - object2.eta);
  else if(variable == "deltaR") value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1.px, object1.py, object1.pz, object1.energy);
    TLorentzVector fourVector2(object2.px, object2.py, object2.pz, object2.energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1.px, object1.py, object1.pz, object1.energy);
    TLorentzVector fourVector2(object2.px, object2.py, object2.pz, object2.energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1.px, object1.py, object1.pz);
      TVector3 threeVector2(object2.px, object2.py, object2.pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "deltaCorrectedD0Vertex") value = object1.correctedD0Vertex - object2.correctedD0Vertex;
  else if(variable == "deltaAbsCorrectedD0Vertex") value = fabs(object1.correctedD0Vertex) - fabs(object2.correctedD0Vertex);
  else if(variable == "d0Sign"){
    double d0Sign = (object1.correctedD0Vertex*object2.correctedD0Vertex)/fabs(object1.correctedD0Vertex*object2.correctedD0Vertex);
    if(d0Sign < 0) value = -0.5;
    else if (d0Sign > 0) value = 0.5;
    else value = numeric_limits<int>::min ();
  }
  else if(variable == "chargeProduct"){
    value = object1.charge*object2.charge;
  }
  else if(variable == "electronCorrectedD0Vertex"){
    value = object1.correctedD0Vertex;
  }
  else if(variable == "muonCorrectedD0Vertex"){
    value = object2.correctedD0Vertex;
  }
  else if(variable == "electronCorrectedD0"){
    value = object1.correctedD0;
  }
  else if(variable == "muonCorrectedD0"){
    value = object2.correctedD0;
  }
  else if(variable == "electronCorrectedD0Err") value =  hypot (object1.tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
  else if(variable == "electronCorrectedD0Sig") value =  object1.correctedD0 / hypot (object1.tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
  else if(variable == "muonCorrectedD0Err") value =  hypot (object2.tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
  else if(variable == "muonCorrectedD0Sig") value =  object2.correctedD0 / hypot (object2.tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
  else if(variable == "electronDetIso"){
    value = (object1.trackIso) / object1.pt;
  }
  else if(variable == "muonDetIso"){
    value = (object2.trackIsoDR03) / object2.pt;
  }
  else if(variable == "electronRelPFrhoIso"){
    value = ( object1.chargedHadronIsoDR03 + max(0.0, object1.neutralHadronIsoDR03 + object1.photonIsoDR03 - object1.AEffDr03*object1.rhoPrime) ) / object1.pt;
  }
  else if(variable == "muonRelPFdBetaIso"){
    value = (object2.pfIsoR04SumChargedHadronPt + max(0.0, object2.pfIsoR04SumNeutralHadronEt + object2.pfIsoR04SumPhotonEt - 0.5*object2.pfIsoR04SumPUPt)) / object2.pt;
  }
  else if(variable == "electronPt"){
    value = object1.pt;
  }
  else if(variable == "muonPt"){
    value = object2.pt;
  }
  else if(variable == "electronEta"){
    value = object1.eta;
  }
  else if(variable == "muonEta"){
    value = object2.eta;
  }
/*  else if(variable == "electronMetMT"){
    value = valueLookup(object1,string("metMT"));
  }*/
/*  else if(variable == "muonMetMT"){
    value = valueLookup(object2,string("metMT"));
  }*/
  else if(variable == "dz"){
    // based on definition of "dz" in DataFormats/TrackReco/interface/TrackBase.h
    value = object1.vz - object2.vz;
    value -= (object1.vx * object1.px + object1.vy * object1.py) / object1.pt * (object1.pz / object1.pt);
    value += (object2.vx * object2.px + object2.vy * object2.py) / object2.pt * (object2.pz / object2.pt);
  }
  else if(variable == "dzSig"){
    // based on definition of "dz" in DataFormats/TrackReco/interface/TrackBase.h
    value = object1.vz - object2.vz;
    value -= (object1.vx * object1.px + object1.vy * object1.py) / object1.pt * (object1.pz / object1.pt);
    value += (object2.vx * object2.px + object2.vy * object2.py) / object2.pt * (object2.pz / object2.pt);
    value /= hypot (object1.tkDZerr, object2.tkDZerr);
  }




  else{clog << "WARNING: invalid electron-muon pair variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}

  return value;
} // end electron-muon pair valueLookup


//!electron-jet pair valueLookup
double
ValueLookup::valueLookup (const BNelectron &object1, const BNjet &object2, string variable){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1.phi,object2.phi));
  else if(variable == "deltaEta") value = fabs(object1.eta - object2.eta);
  else if(variable == "distance")
   {
      Line::PositionType pos(GlobalPoint(object1.vx, object1.vy, object1.vz));
      Line::DirectionType dir(GlobalVector(object1.px, object1.py, object1.pz).unit());
      Line electron(pos, dir);
      Line::PositionType pos2(GlobalPoint(chosenVertex()->x, chosenVertex()->y, chosenVertex()->z));
      Line::DirectionType dir2(GlobalVector(object2.px, object2.py, object2.pz).unit());
      Line jet(pos2, dir2);
      value = (jet.distance(electron)).mag();
   }
  else if(variable == "jetEta") value = object2.eta;
  else if(variable == "jetPhi") value = object2.phi;
  else if(variable == "jetPt") value = object2.pt;
  else if(variable == "electronEta") value = object1.eta;
  else if(variable == "electronPhi") value = object1.phi;
  else if(variable == "electronPt") value = object1.pt;
  else if(variable == "deltaR") value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
  else if(variable == "deltaZ") value = object1.vz - object2.leadCandVz;
  else if(variable == "relPFrhoIso") value = ( object1.chargedHadronIsoDR03 + max(0.0, object1.neutralHadronIsoDR03 + object1.photonIsoDR03 - object1.AEffDr03*object1.rhoPrime) ) / object1.pt;
  else if(variable == "relPFrhoIsoNoPUSbtractedJet") value = (object2.chargedHadronEnergyFraction*object2.pt + max(0.0, object2.neutralHadronEnergyFraction*object2.pt + object2.neutralEmEnergyFraction*object2.pt)) / object1.pt;
  else if(variable == "relPFrhoIsoJet") value = (object2.chargedHadronEnergyFraction*object2.pt + max(0.0, object2.neutralHadronEnergyFraction*object2.pt + object2.neutralEmEnergyFraction*object2.pt - object1.AEffDr03*object1.rhoPrime)) / object1.pt;
  else if(variable == "PUInrelPFrhoIso") value = (object1.AEffDr03*object1.rhoPrime) / object1.pt;
  else if(variable == "ChargedHadInrelPFrhoIsoDiff") value = (object2.chargedHadronEnergyFraction*object2.pt - object1.chargedHadronIsoDR03) / object1.pt;
  else if(variable == "NeutralHadInrelPFrhoIsoDiff") value = (object2.neutralHadronEnergyFraction*object2.pt - object1.neutralHadronIsoDR03) / object1.pt;
  else if(variable == "PhotonInrelPFrhoIsoDiff") value = (object2.neutralEmEnergyFraction*object2.pt - object1.photonIsoDR03) / object1.pt;
  else if(variable == "ChargedHadInrelPFrhoIsoJet") value = (object2.chargedHadronEnergyFraction*object2.pt) / object1.pt;
  else if(variable == "NeutralHadInrelPFrhoIsoJet") value = (object2.neutralHadronEnergyFraction*object2.pt) / object1.pt;
  else if(variable == "PhotonInrelPFrhoIsoJet") value = (object2.neutralEmEnergyFraction*object2.pt) / object1.pt;
  else if(variable == "relPFrhoIsoDiff") value =  ( object1.chargedHadronIsoDR03 + max(0.0, object1.neutralHadronIsoDR03 + object1.photonIsoDR03 - object1.AEffDr03*object1.rhoPrime) ) / object1.pt - (object2.chargedHadronEnergyFraction*object2.pt + max(0.0, object2.neutralHadronEnergyFraction*object2.pt + object2.neutralEmEnergyFraction*object2.pt - object1.AEffDr03*object1.rhoPrime)) / object1.pt;
  else if(variable == "relPFrhoIsoNoPUSubtractedDiff") value = ( object1.chargedHadronIsoDR03 + max(0.0, object1.neutralHadronIsoDR03 + object1.photonIsoDR03) ) / object1.pt - (object2.chargedHadronEnergyFraction*object2.pt + max(0.0, object2.neutralHadronEnergyFraction*object2.pt + object2.neutralEmEnergyFraction*object2.pt)) / object1.pt;
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1.px, object1.py, object1.pz, object1.energy);
    TLorentzVector fourVector2(object2.px, object2.py, object2.pz, object2.energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1.px, object1.py, object1.pz, object1.energy);
    TLorentzVector fourVector2(object2.px, object2.py, object2.pz, object2.energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1.px, object1.py, object1.pz);
      TVector3 threeVector2(object2.px, object2.py, object2.pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "chargeProduct"){
    value = object1.charge*object2.charge;
  }
  else if(variable == "ptOfClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNelectron, BNjet> (&object1, jets, "jets");
    if (!closestJet) value = numeric_limits<int>::min ();
    else value = closestJet->pt;
  }
  else if(variable == "massOfClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNelectron, BNjet> (&object1, jets, "jets");
    if (!closestJet) value = numeric_limits<int>::min ();
    else value = closestJet->mass;
  }
  else if(variable == "areaOfClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNelectron, BNjet> (&object1, jets, "jets");
    if (!closestJet) value = numeric_limits<int>::min ();
    else value = closestJet->area;
  }
  else if(variable == "pTransverseToClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNelectron, BNjet> (&object1, jets, "jets");
    if (!closestJet) value = numeric_limits<int>::min ();
    else{
      TVector3 jetP (closestJet->px, closestJet->py, closestJet->pz),
               leptonP (object1.px, object1.py, object1.pz),
               projection, rejection;
      projection = jetP.Unit ();
      projection *= (leptonP * projection);
      rejection = leptonP - projection;
      value = rejection.Mag ();
    }
  }

  else{clog << "WARNING: invalid electron-jet pair variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}

  return value;
}

//!electron-mcparticle pair valueLookup
double
ValueLookup::valueLookup (const BNelectron &object1, const BNmcparticle &object2, string variable){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1.phi,object2.phi));
  else if(variable == "deltaEta") value = fabs(object1.eta - object2.eta);
  else if(variable == "id") value = fabs(object2.id);
  else if(variable == "deltaR") value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1.px, object1.py, object1.pz);
      TVector3 threeVector2(object2.px, object2.py, object2.pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "chargeProduct"){
    value = object1.charge*object2.charge;
  }
  else if(variable == "chargeProductById"){
    value = object1.charge*object2.id;
  }

  else{clog << "WARNING: invalid electron-mcparticle variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}

  return value;
}

//!muon-mcparticle pair valueLookup
double
ValueLookup::valueLookup (const BNmuon &object1, const BNmcparticle &object2, string variable){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1.phi,object2.phi));
  else if(variable == "deltaEta") value = fabs(object1.eta - object2.eta);
  else if(variable == "id") value = fabs(object2.id);
  else if(variable == "deltaR") value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1.px, object1.py, object1.pz);
      TVector3 threeVector2(object2.px, object2.py, object2.pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "chargeProduct"){
    value = object1.charge*object2.charge;
  }
  else if(variable == "chargeProductById"){
    value = object1.charge*object2.id;
  }

  else{clog << "WARNING: invalid muon-mcparticle variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}

  return value;
}


//!mcparticle-mcparticle pair valueLookup
double
ValueLookup::valueLookup (const BNmcparticle &object1, const BNmcparticle &object2, string variable){

  double value = 0.0;

  if(variable == "deltaR") value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
  else if(variable == "chargeProduct"){
    value = object1.charge*object2.charge;
  }
  else{clog << "WARNING: invalid mcparticle-mcparticle variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}

  return value;
}


//!jet-mcparticle pair valueLookup
double
ValueLookup::valueLookup (const BNjet &object1, const BNmcparticle &object2, string variable){

  double value = 0.0;

  if(variable == "deltaR") value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);


  else{clog << "WARNING: invalid jet-mcparticle variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}

  return value;
}



//!track-mcparticle pair valueLookup
double
ValueLookup::valueLookup (const BNtrack &object1, const BNmcparticle &object2, string variable){

  double value = 0.0;

  if     (variable == "deltaPhi") value = fabs(deltaPhi(object1.phi,object2.phi));
  else if(variable == "deltaEta") value = fabs(object1.eta - object2.eta);
  else if(variable == "deltaR")   value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1.px, object1.py, object1.pz);
      TVector3 threeVector2(object2.px, object2.py, object2.pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "chargeProduct"){
    value = object1.charge*object2.charge;
  }

  else{clog << "WARNING: invalid electron-mcparticle variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}

  return value;
}



//!stop-mcparticle pair valueLookup
double
ValueLookup::valueLookup (const BNstop &object1, const BNmcparticle &object2, string variable){

  double value = 0.0;

  if     (variable == "deltaPhi") value = fabs(deltaPhi(object1.phi,object2.phi));
  else if(variable == "deltaEta") value = fabs(object1.eta - object2.eta);
  else if(variable == "deltaR")   value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1.px, object1.py, object1.pz);
      TVector3 threeVector2(object2.px, object2.py, object2.pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "chargeProduct"){
    value = object1.charge*object2.charge;
  }

  else{clog << "WARNING: invalid stop-mcparticle variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}

  return value;
}




//!photon-jet pair valueLookup
double
ValueLookup::valueLookup (const BNphoton &object1, const BNjet &object2, string variable){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1.phi,object2.phi));
  else if(variable == "deltaEta") value = fabs(object1.eta - object2.eta);
  else if(variable == "jetEta") value = object2.eta;
  else if(variable == "jetPhi") value = object2.phi;
  else if(variable == "photonEta") value = object1.eta;
  else if(variable == "photonPhi") value = object1.phi;
  else if(variable == "photonGenMotherId") value = object1.genMotherId;
  else if(variable == "deltaR") value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1.px, object1.py, object1.pz, object1.energy);
    TLorentzVector fourVector2(object2.px, object2.py, object2.pz, object2.energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1.px, object1.py, object1.pz, object1.energy);
    TLorentzVector fourVector2(object2.px, object2.py, object2.pz, object2.energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1.px, object1.py, object1.pz);
      TVector3 threeVector2(object2.px, object2.py, object2.pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else{clog << "WARNING: invalid photon-jet pair variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}

  return value;
}

// track-jet pair valueLookup
double
ValueLookup::valueLookup (const BNtrack &object1, const BNjet &object2, string variable){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1.phi,object2.phi));
  else if(variable == "deltaR") value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);

  else{clog << "WARNING: invalid track-jet pair variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}

  return value;

}



// met-jet pair valueLookup
double
ValueLookup::valueLookup (const BNmet &object1, const BNjet &object2, string variable){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1.phi,object2.phi));

  else{clog << "WARNING: invalid met-jet pair variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}

  return value;

}

double
ValueLookup::valueLookup (const BNmet &object1, const BNmcparticle &object2, string variable){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1.phi,object2.phi));
  else{clog << "WARNING: invalid met-mcparticle pair variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}

  return value;

}




//!muon-jet pair valueLookup
double
ValueLookup::valueLookup (const BNmuon &object1, const BNjet &object2, string variable){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1.phi,object2.phi));
  else if(variable == "jetEta") value = object2.eta;
  else if(variable == "distance")
   {
      Line::PositionType pos(GlobalPoint(object1.vx, object1.vy, object1.vz));
      Line::DirectionType dir(GlobalVector(object1.px, object1.py, object1.pz).unit());
      Line muon(pos, dir);
      Line::PositionType pos2(GlobalPoint(chosenVertex()->x, chosenVertex()->y, chosenVertex()->z));
      Line::DirectionType dir2(GlobalVector(object2.px, object2.py, object2.pz).unit());
      Line jet(pos2, dir2);
      value = (jet.distance(muon)).mag();
   }
  else if(variable == "relPFdBetaIso") value = (object1.pfIsoR04SumChargedHadronPt + max(0.0, object1.pfIsoR04SumNeutralHadronEt + object1.pfIsoR04SumPhotonEt - 0.5*object1.pfIsoR04SumPUPt)) / object1.pt;
  else if(variable == "relPFdBetaIsoNoPUSbtractedJet") value = (object2.chargedHadronEnergyFraction*object2.pt + max(0.0, object2.neutralHadronEnergyFraction*object2.pt + object2.neutralEmEnergyFraction*object2.pt)) / object1.pt;
  else if(variable == "relPFdBetaIsoJet") value = (object2.chargedHadronEnergyFraction*object2.pt + max(0.0, object2.neutralHadronEnergyFraction*object2.pt + object2.neutralEmEnergyFraction*object2.pt - 0.5*object1.pfIsoR04SumPUPt)) / object1.pt;
  else if(variable == "PUInrelPFdBetaIso") value = (0.5*object1.pfIsoR04SumPUPt) / object1.pt;
  else if(variable == "ChargedHadInrelPFdBetaIsoDiff") value = (object2.chargedHadronEnergyFraction*object2.pt - object1.pfIsoR04SumChargedHadronPt) / object1.pt;
  else if(variable == "NeutralHadInrelPFdBetaIsoDiff") value = (object2.neutralHadronEnergyFraction*object2.pt - object1.pfIsoR04SumNeutralHadronEt) / object1.pt;
  else if(variable == "PhotonInrelPFdBetaIsoDiff") value = (object2.neutralEmEnergyFraction*object2.pt - object1.pfIsoR04SumPhotonEt) / object1.pt;
  else if(variable == "ChargedHadInrelPFdBetaIsoJet") value = (object2.chargedHadronEnergyFraction*object2.pt) / object1.pt;
  else if(variable == "NeutralHadInrelPFdBetaIsoJet") value = (object2.neutralHadronEnergyFraction*object2.pt) / object1.pt;
  else if(variable == "PhotonInrelPFdBetaIsoJet") value = (object2.neutralEmEnergyFraction*object2.pt) / object1.pt;
  else if(variable == "relPFdBetaIsoDiff") value = (object1.pfIsoR04SumChargedHadronPt + max(0.0, object1.pfIsoR04SumNeutralHadronEt + object1.pfIsoR04SumPhotonEt - 0.5*object1.pfIsoR04SumPUPt))/ object1.pt - (object2.chargedHadronEnergyFraction*object2.pt + max(0.0, object2.neutralHadronEnergyFraction*object2.pt + object2.neutralEmEnergyFraction*object2.pt- 0.5*object1.pfIsoR04SumPUPt)) / object1.pt;
  else if(variable == "relPFdBetaIsoNoPUSubtractedDiff") value =(object1.pfIsoR04SumChargedHadronPt + max(0.0, object1.pfIsoR04SumNeutralHadronEt + object1.pfIsoR04SumPhotonEt))/ object1.pt - (object2.chargedHadronEnergyFraction*object2.pt + max(0.0, object2.neutralHadronEnergyFraction*object2.pt + object2.neutralEmEnergyFraction*object2.pt)) / object1.pt;
  else if(variable == "jetPt") value = object2.pt;
  else if(variable == "jetPhi") value = object2.phi;
  else if(variable == "deltaPt") value = object1.pt - object2.pt;
  else if(variable == "muonEta") value = object1.eta;
  else if(variable == "muonPt") value = object1.pt;
  else if(variable == "muonPhi") value = object1.phi;
  else if(variable == "deltaEta") value = fabs(object1.eta - object2.eta);
  else if(variable == "deltaR") value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
  else if(variable == "deltaZ") value = object1.vz - object2.leadCandVz;
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1.px, object1.py, object1.pz, object1.energy);
    TLorentzVector fourVector2(object2.px, object2.py, object2.pz, object2.energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1.px, object1.py, object1.pz, object1.energy);
    TLorentzVector fourVector2(object2.px, object2.py, object2.pz, object2.energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1.px, object1.py, object1.pz);
      TVector3 threeVector2(object2.px, object2.py, object2.pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "chargeProduct"){
    value = object1.charge*object2.charge;
  }
  else if(variable == "ptOfClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNmuon, BNjet> (&object1, jets, "jets");
    if (!closestJet) value = numeric_limits<int>::min ();
    else value = closestJet->pt;
  }
  else if(variable == "massOfClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNmuon, BNjet> (&object1, jets, "jets");
    if (!closestJet) value = numeric_limits<int>::min ();
    else value = closestJet->mass;
  }
  else if(variable == "areaOfClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNmuon, BNjet> (&object1, jets, "jets");
    if (!closestJet) value = numeric_limits<int>::min ();
    else value = closestJet->area;
  }
  else if(variable == "pTransverseToClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNmuon, BNjet> (&object1, jets, "jets");
    if (!closestJet) value = numeric_limits<int>::min ();
    else{
      TVector3 jetP (closestJet->px, closestJet->py, closestJet->pz),
               leptonP (object1.px, object1.py, object1.pz),
               projection, rejection;
      projection = jetP.Unit ();
      projection *= (leptonP * projection);
      rejection = leptonP - projection;
      value = rejection.Mag ();
    }
  }

  else{clog << "WARNING: invalid muon-jet pair variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}

  return value;
} // end muon-jet pair valueLookup


//!muon-event valueLookup
double
ValueLookup::valueLookup (const BNmuon &object1, const BNevent &object2, string variable){

  double value = 0.0;

  if(variable == "muonEta") value = object1.eta;
  else if(variable == "muonPt") value = object1.pt;
  else if(variable == "muonPhi") value = object1.phi;
  else if(variable == "Ht") value = getHt(jets.product());
  else if(variable == "pthat")   value = object2.pthat;
  else if(variable == "numPV")   value = object2.numPV;
  else if(variable == "correctedD0")   value = object1.correctedD0;
  else if(variable == "relPFdBetaIso") value = (object1.pfIsoR04SumChargedHadronPt + max(0.0, object1.pfIsoR04SumNeutralHadronEt + object1.pfIsoR04SumPhotonEt - 0.5*object1.pfIsoR04SumPUPt)) / object1.pt;
  else{clog << "WARNING: invalid muon-event pair variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}

  return value;
}
//!electron-event valueLookup
double
ValueLookup::valueLookup (const BNelectron &object1, const BNevent &object2, string variable){

  double value = 0.0;

  if     (variable == "numPV")       value = object2.numPV;
  else if(variable == "correctedD0") value = object1.correctedD0;
  else{clog << "WARNING: invalid electron-event pair variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}

  return value;
}
//!jet-jet pair valueLookup
double
ValueLookup::valueLookup (const BNjet &object1, const BNjet &object2, string variable){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1.phi,object2.phi));
  else if(variable == "deltaEta") value = fabs(object1.eta - object2.eta);
  else if(variable == "deltaPt") value = object1.pt - object2.pt;
  else if(variable == "deltaR") value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1.px, object1.py, object1.pz, object1.energy);
    TLorentzVector fourVector2(object2.px, object2.py, object2.pz, object2.energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1.px, object1.py, object1.pz, object1.energy);
    TLorentzVector fourVector2(object2.px, object2.py, object2.pz, object2.energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1.px, object1.py, object1.pz);
      TVector3 threeVector2(object2.px, object2.py, object2.pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "chargeProduct"){
    value = object1.charge*object2.charge;
  }

  else{clog << "WARNING: invalid jet-jet pair variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}

  return value;
}

//!electron-track pair valueLookup
double
ValueLookup::valueLookup (const BNelectron &object1, const BNtrack &object2, string variable){
  double electronMass = 0.000511;
  double value = 0.0;
  TLorentzVector fourVector1(0, 0, 0, 0);
  TLorentzVector fourVector2(0, 0, 0, 0);
  if(variable == "deltaPhi") value = fabs(deltaPhi(object1.phi,object2.phi));
  else if(variable == "deltaEta") value = fabs(object1.eta - object2.eta);
  else if(variable == "deltaR") value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
  else if(variable == "deltaRLooseMvaId") {
    // only consider electrons that pass "loose MVA id", i.e., mvaNonTrigV0 > 0;
    // otherwise return very large value (99)
    if (object1.mvaNonTrigV0>0) value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
    else                         value = 99.;
  }
  else if(variable == "invMass"){
    fourVector1.SetPtEtaPhiM(object1.pt, object1.eta, object1.phi, electronMass);
    fourVector2.SetPtEtaPhiM(object2.pt, object2.eta, object2.phi, electronMass );

    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "chargeProduct"){
    value = object1.charge*object2.charge;
  }
  else{clog << "WARNING: invalid electron-track pair variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}
  return value;

}


//!muon-track pair valueLookup
double
ValueLookup::valueLookup (const BNmuon &object1, const BNtrack &object2, string variable){
  double value = 0.0;
  if(variable == "deltaPhi") value = fabs(deltaPhi(object1.phi,object2.phi));
  else if(variable == "deltaEta") value = fabs(object1.eta - object2.eta);
  else if(variable == "alpha")
    {
      static const double pi = 3.1415926535897932384626433832795028841971693993751058;
      TVector3 threeVector1(object1.px, object1.py, object1.pz);
      TVector3 threeVector2(object2.px, object2.py, object2.pz);
      value = (pi-threeVector1.Angle(threeVector2));
    }
  else if(variable == "deltaPt")  value = fabs(object1.pt - object2.pt);
  else if(variable == "deltaR") value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
/*  else if(variable == "deltaRLooseID") {
    // only consider muons that pass looseId criteria; otherwise return very large value (99.)
    double isLooseId = valueLookup(object1, "looseID");
    if (isLooseId) value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
    else           value = 99.;
  }*/
  else if(variable == "deltaRGlobalMuon") {
    // only consider muons that pass looseId criteria; otherwise return very large value (99.)
    if (object1.isGlobalMuon) value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
    else                       value = 99.;
  }
  else if(variable == "invMass"){
    double pionMass = 0.140;
    double muonMass = 0.106;
    TLorentzVector fourVector1(0, 0, 0, 0);
    TLorentzVector fourVector2(0, 0, 0, 0);
    fourVector1.SetPtEtaPhiM(object1.pt, object1.eta, object1.phi, muonMass);
    fourVector2.SetPtEtaPhiM(object2.pt, object2.eta, object2.phi, pionMass );
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "chargeProduct"){
    value = object1.charge*object2.charge;
  }

  else{clog << "WARNING: invalid muon-track pair variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}
  return value;
}

//!jet-track pair valueLookup
double
ValueLookup::valueLookup (const BNjet &object1, const BNtrack &object2, string variable){
  double value = 0.0;
  if(variable == "deltaPhi") value = fabs(deltaPhi(object1.phi,object2.phi));
  else if(variable == "deltaEta") value = fabs(object1.eta - object2.eta);
  else if(variable == "deltaPt")  value = fabs(object1.pt - object2.pt);
  else if(variable == "deltaR") value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
  else{clog << "WARNING: invalid jet-track pair variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}
  return value;
}

//!tau-tau pair valueLookup
double
ValueLookup::valueLookup (const BNtau &object1, const BNtau &object2, string variable){
  double value = 0.0;
  if(variable == "deltaPhi") value = fabs(deltaPhi(object1.phi,object2.phi));
  else if(variable == "deltaEta") value = fabs(object1.eta - object2.eta);
  else if(variable == "deltaR") value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1.px, object1.py, object1.pz, object1.energy);
    TLorentzVector fourVector2(object2.px, object2.py, object2.pz, object2.energy);
    value = (fourVector1 + fourVector2).M();
  }

  else if(variable == "chargeProduct"){
    value = object1.charge*object2.charge;
  }

  else{clog << "WARNING: invalid tau-tau pair variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}
  return value;
}

//!muon-tau pair valueLookup
double
ValueLookup::valueLookup (const BNmuon &object1, const BNtau &object2, string variable){
  double value = 0.0;
  if(variable == "deltaPhi") value = fabs(deltaPhi(object1.phi,object2.phi));
  else if(variable == "deltaEta") value = fabs(object1.eta - object2.eta);
  else if(variable == "deltaR") value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1.px, object1.py, object1.pz, object1.energy);
    TLorentzVector fourVector2(object2.px, object2.py, object2.pz, object2.energy);
    value = (fourVector1 + fourVector2).M();
  }

  else if(variable == "chargeProduct"){
    value = object1.charge*object2.charge;
  }

  else{clog << "WARNING: invalid muon-tau pair variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}
  return value;
}

//!tau-track pair valueLookup
double
ValueLookup::valueLookup (const BNtau &object1, const BNtrack &object2, string variable){
  double value = 0.0;
  if(variable == "deltaPhi") value = fabs(deltaPhi(object1.phi,object2.phi));
  else if(variable == "deltaR") value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
/*  else if(variable == "deltaRLooseHadronicID") {
    // only consider tau's that pass the loose hadronic ID criteria; otherwise return very large value (99)
    double isLooseHadronicID = valueLookup(object1, "looseHadronicID");

    if (isLooseHadronicID) value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
    else                   value = 99.;
  }*/
  else if(variable == "deltaEta") value = fabs(object1.eta - object2.eta);
  else if(variable == "invMass"){
    double tauMass = 1.777;  // PDG 2010
    double pionMass = 0.140; // PDG 2010
    TLorentzVector fourVector1(0, 0, 0, 0);
    TLorentzVector fourVector2(0, 0, 0, 0);
    fourVector1.SetPtEtaPhiM(object1.pt, object1.eta, object1.phi, tauMass);
    fourVector2.SetPtEtaPhiM(object2.pt, object2.eta, object2.phi, pionMass );
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "chargeProduct"){
    value = object1.charge*object2.charge;
  }

  else{clog << "WARNING: invalid tau-track pair variable '" << variable << "'\n"; value = numeric_limits<int>::min ();}
  return value;
}


//!track-event pair valueLookup
double
ValueLookup::valueLookup (const BNtrack &object1, const BNevent &object2, string variable){

  double value = 0.0;
  double pMag = sqrt(object1.pt * object1.pt +
                     object1.pz * object1.pz);

  if      (variable == "numPV")                      value = object2.numPV;
  else if (variable == "rhoCorrRp5")                 value = getRhoCorr();
  else if (variable == "caloTotDeltaRp5")            value =  (object1.caloHadDeltaRp5 + object1.caloEMDeltaRp5);
  else if (variable == "caloTotDeltaRp5ByP")         value = ((object1.caloHadDeltaRp5 + object1.caloEMDeltaRp5)/pMag);
  else if (variable == "caloTotDeltaRp5_RhoCorr")    value = getTrkCaloTotRhoCorr(&object1);
  else if (variable == "caloTotDeltaRp5ByP_RhoCorr") value = getTrkCaloTotRhoCorr(&object1) / pMag;

  else { clog << "WARNING: invalid track-event pair variable '" << variable << "'\n"; value = numeric_limits<int>::min (); }


  return value;

}

//!electron-trigobj pair valueLookup
double
ValueLookup::valueLookup (const BNelectron &object1, const BNtrigobj &object2, string variable){

  double value = 0.0;

  if (variable == "deltaR") value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);
  /*else if (variable == "match"){
    if (deltaR(object1.eta,object1.phi,object2.eta,object2.phi) < 0.2 && abs(object2.id) == 11)
      stringValue = object2.filter;
    else
      stringValue = "none";
  }*/

  else { clog << "WARNING: invalid electron-trigobj variable '" << variable << "'\n"; value = numeric_limits<int>::min (); }

  return value;

}

//!muon-trigobj pair valueLookup
double
ValueLookup::valueLookup (const BNmuon &object1, const BNtrigobj &object2, string variable){

  double value = 0.0;

  if (variable == "deltaR") value = deltaR(object1.eta,object1.phi,object2.eta,object2.phi);

  else { clog << "WARNING: invalid muon-trigobj variable '" << variable << "'\n"; value = numeric_limits<int>::min (); }

  return value;

}
