#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

double
ValueLookup::applyFunction(string function, double value){

  if(function == "abs") value = fabs(value);
  else if(function == "fabs") value = fabs(value);
  else if(function == "log10") value = log10(value);
  else if(function == "log") value = log10(value);

  else if(function == "") value = value;
  else{clog << "WARNING: invalid function '" << function << "'\n";}

  return value;

}


double
ValueLookup::getRhoCorr(const BNtrack* track) {
  // Return the pile-up (rho) corrected isolation energy, i.e., the total calorimeter energy around the candidate track.                                                                                  
  if (!useTrackCaloRhoCorr_) return -99;
  // if (!rhokt6CaloJetsHandle_) {                                                                                                                                                                        
  //   clog << "ERROR [getTrkCaloTotRhoCorr]:  The collection rhokt6CaloJetsHandle is not available!" << endl;                                                                                            
  //   return -99;                                                                                                                                                                                        
  // }                                                                                                                                                                                                    
  double radDeltaRCone = 0.5;
  double rhoCorr_kt6CaloJets = *rhokt6CaloJetsHandle_ * TMath::Pi() * pow(radDeltaRCone, 2);  // Define effective area as pi*r^2, where r is radius of DeltaR cone.                                       
  return rhoCorr_kt6CaloJets;

}


double
ValueLookup::getTrkCaloTotRhoCorr(const BNtrack* track) {
  // Return the pile-up (rho) corrected isolation energy, i.e., the total calorimeter energy around the candidate track.
  if (!useTrackCaloRhoCorr_) return -99;
  // if (!rhokt6CaloJetsHandle_) {
  //   clog << "ERROR [getTrkCaloTotRhoCorr]:  The collection rhokt6CaloJetsHandle is not available!" << endl;
  //   return -99;
  // }
  double radDeltaRCone = 0.5;
  double rhoCorr_kt6CaloJets = *rhokt6CaloJetsHandle_ * TMath::Pi() * pow(radDeltaRCone, 2);  // Define effective area as pi*r^2, where r is radius of DeltaR cone.
  double rawCaloTot = track->caloHadDeltaRp5 + track->caloEMDeltaRp5;
  double caloTotRhoCorrCalo = TMath::Max(0., rawCaloTot - rhoCorr_kt6CaloJets);
  return caloTotRhoCorrCalo;

}


//calculate the scalar sum of Jet Pt in the event.
double
ValueLookup::getHt (const BNjetCollection* jetColl){
  double Ht = 0;
  for(BNjetCollection::const_iterator jet = jetColl->begin(); jet !=jetColl->end(); jet++){
    Ht += abs(jet->pt);
  }
  return Ht;
}


double
ValueLookup::getSumForwardJetPt (const BNjetCollection* jetColl){
  double sumForwardJetPt = 0;
  TLorentzVector hT (0, 0, 0, 0);
  for(BNjetCollection::const_iterator jet = jetColl->begin(); jet !=jetColl->end(); jet++){
    TLorentzVector hT_2(0, 0, 0, 0);
    if (fabs(jet->eta) < 3 ) continue;
    hT_2.SetPtEtaPhiM(jet->pt, jet->eta, jet->phi, jet->mass);
    hT += hT_2;
    sumForwardJetPt = hT.Pt();

  }

  return sumForwardJetPt;
}


const BNprimaryvertex *
ValueLookup::chosenVertex ()
{
  const BNprimaryvertex *chosenVertex = 0;
  if(cumulativeFlags.find ("primaryvertexs") != cumulativeFlags.end ()){
    flagPair vertexFlags;
    for (int i = cumulativeFlags.at("primaryvertexs").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("primaryvertexs").at(i).size()){
        vertexFlags = cumulativeFlags.at("primaryvertexs").at(i);
        break;
      }
    }
    for (uint vertexIndex = 0; vertexIndex != vertexFlags.size(); vertexIndex++){
      if(!vertexFlags.at(vertexIndex).first) continue;
      chosenVertex = & primaryvertexs->at(vertexIndex);
      break;
    }
  }
  else if (find (objectsToGet.begin (), objectsToGet.end (), "primaryvertexs") != objectsToGet.end ())
    chosenVertex = & primaryvertexs->at (0);

  return chosenVertex;
}


const BNmet *
ValueLookup::chosenMET ()
{
  const BNmet *chosenMET = 0;
  if(cumulativeFlags.find ("mets") != cumulativeFlags.end ()){
    flagPair metFlags;
    for (int i = cumulativeFlags.at("mets").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("mets").at(i).size()){
        metFlags = cumulativeFlags.at("mets").at(i);
        break;
      }
    }
    for (uint metIndex = 0; metIndex != metFlags.size(); metIndex++){
      if(!metFlags.at(metIndex).first) continue;
      chosenMET = & mets->at(metIndex);
      break;
    }
  }
  else if (find (objectsToGet.begin (), objectsToGet.end (), "mets") != objectsToGet.end ())
    chosenMET = & mets->at (0);

  return chosenMET;
}


const BNtrack *
ValueLookup::chosenTrack ()
{
  const BNtrack *chosenTrack = 0;
  if(cumulativeFlags.find ("tracks") != cumulativeFlags.end ()){
    flagPair trackFlags;
    for (int i = cumulativeFlags.at("tracks").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("tracks").at(i).size()){
        trackFlags = cumulativeFlags.at("tracks").at(i);
	break;
      }
    }
    for (uint trackIndex = 0; trackIndex != trackFlags.size(); trackIndex++){
      if(!trackFlags.at(trackIndex).first) continue;
      chosenTrack = & tracks->at(trackIndex);
      break;
    }
  }
  else if (find (objectsToGet.begin (), objectsToGet.end (), "tracks") != objectsToGet.end ())
    chosenTrack = & tracks->at (0);

  return chosenTrack;
}


const BNjet *
ValueLookup::chosenJet ()
{
  const BNjet *chosenJet = 0;
  if(cumulativeFlags.find ("jets") != cumulativeFlags.end ()){
    flagPair jetFlags;
    for (int i = cumulativeFlags.at("jets").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("jets").at(i).size()){
        jetFlags = cumulativeFlags.at("jets").at(i);
	break;
      }
    }
    for (uint jetIndex = 0; jetIndex != jetFlags.size(); jetIndex++){
      if(!jetFlags.at(jetIndex).first) continue;
      chosenJet = & jets->at(jetIndex);
      break;
    }
  }
  else if (find (objectsToGet.begin (), objectsToGet.end (), "jets") != objectsToGet.end ())
    chosenJet = & jets->at (0);

  return chosenJet;
}


const BNelectron *
ValueLookup::chosenElectron ()
{
  const BNelectron *chosenElectron = 0;
  if(cumulativeFlags.find ("electrons") != cumulativeFlags.end ()){
    flagPair electronFlags;
    for (int i = cumulativeFlags.at("electrons").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("electrons").at(i).size()){
        electronFlags = cumulativeFlags.at("electrons").at(i);
        break;
      }
    }
    for (uint electronIndex = 0; electronIndex != electronFlags.size(); electronIndex++){
      if(!electronFlags.at(electronIndex).first) continue;
      chosenElectron = & electrons->at(electronIndex);
      break;
    }
  }
  else if (find (objectsToGet.begin (), objectsToGet.end (), "electrons") != objectsToGet.end ())
    chosenElectron = & electrons->at (0);

  return chosenElectron;
}


const BNmuon *
ValueLookup::chosenMuon ()
{
  const BNmuon *chosenMuon = 0;
  if(cumulativeFlags.find ("muons") != cumulativeFlags.end ()){
    flagPair muonFlags;
    for (int i = cumulativeFlags.at("muons").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("muons").at(i).size()){
        muonFlags = cumulativeFlags.at("muons").at(i);
        break;
      }
    }
    for (uint muonIndex = 0; muonIndex != muonFlags.size(); muonIndex++){
      if(!muonFlags.at(muonIndex).first) continue;
      chosenMuon = & muons->at(muonIndex);
      break;
    }
  }
  else if (find (objectsToGet.begin (), objectsToGet.end (), "muons") != objectsToGet.end ())
    chosenMuon = & muons->at (0);

  return chosenMuon;
}


double
ValueLookup::chosenHT ()
{
  double chosenHT = 0.0;
  if(cumulativeFlags.find ("jets") != cumulativeFlags.end ()){
    flagPair jetFlags;
    for (int i = cumulativeFlags.at("jets").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("jets").at(i).size()){
        jetFlags = cumulativeFlags.at("jets").at(i);
        break;
      }
    }
    for (uint jetIndex = 0; jetIndex != jetFlags.size(); jetIndex++){
      if(!jetFlags.at(jetIndex).first) continue;
      chosenHT += jets->at(jetIndex).pt;
    }
  }

  return chosenHT;
}


double
ValueLookup::chosenST ()
{
  double chosenST = 0.0;
  if(cumulativeFlags.find ("electrons") != cumulativeFlags.end ()){
    flagPair electronFlags;
    for (int i = cumulativeFlags.at("electrons").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("electrons").at(i).size()){
        electronFlags = cumulativeFlags.at("electrons").at(i);
        break;
      }
    }
    for (uint electronIndex = 0; electronIndex != electronFlags.size(); electronIndex++){
      if(!electronFlags.at(electronIndex).first) continue;
      chosenST += electrons->at(electronIndex).pt;
    }
  }

  if(cumulativeFlags.find ("muons") != cumulativeFlags.end ()){
    flagPair muonFlags;
    for (int i = cumulativeFlags.at("muons").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("muons").at(i).size()){
        muonFlags = cumulativeFlags.at("muons").at(i);
        break;
      }
    }
    for (uint muonIndex = 0; muonIndex != muonFlags.size(); muonIndex++){
      if(!muonFlags.at(muonIndex).first) continue;
      chosenST += muons->at(muonIndex).pt;
    }
  }

  if(cumulativeFlags.find ("jets") != cumulativeFlags.end ()){
    flagPair jetFlags;
    for (int i = cumulativeFlags.at("jets").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("jets").at(i).size()){
        jetFlags = cumulativeFlags.at("jets").at(i);
        break;
      }
    }
    for (uint jetIndex = 0; jetIndex != jetFlags.size(); jetIndex++){
      if(!jetFlags.at(jetIndex).first) continue;
      chosenST += jets->at(jetIndex).pt;
    }
  }

  return chosenST;
}

// bin      particle type
// ---      -------------
//  0        unmatched
//  1        u
//  2        d
//  3        s
//  4        c
//  5        b
//  6        t
//  7        e
//  8        mu
//  9        tau
// 10        nu
// 11        g
// 12        gamma
// 13        Z
// 14        W
// 15        light meson
// 16        K meson
// 17        D meson
// 18        B meson
// 19        light baryon
// 20        strange baryon
// 21        charm baryon
// 22        bottom baryon
// 23        QCD string
// 24        other

int ValueLookup::getPdgIdBinValue(int pdgId){

  int binValue = -999;
  int absPdgId = fabs(pdgId);
  if(pdgId == -1) binValue = 0;
  else if(absPdgId <= 6 ) binValue = absPdgId;
  else if(absPdgId == 11 ) binValue = 7;
  else if(absPdgId == 13 ) binValue = 8;
  else if(absPdgId == 15 ) binValue = 9;
  else if(absPdgId == 12 || absPdgId == 14 || absPdgId == 16 ) binValue = 10;
  else if(absPdgId == 21 ) binValue = 11;
  else if(absPdgId == 22 ) binValue = 12;
  else if(absPdgId == 23 ) binValue = 13;
  else if(absPdgId == 24 ) binValue = 14;
  else if(absPdgId > 100 && absPdgId < 300 && absPdgId != 130  ) binValue = 15;
  else if( absPdgId == 130 || (absPdgId > 300 && absPdgId < 400)  ) binValue = 16;
  else if(absPdgId > 400 && absPdgId < 500  ) binValue = 17;
  else if(absPdgId > 500 && absPdgId < 600  ) binValue = 18;
  else if(absPdgId > 1000 && absPdgId < 3000  ) binValue = 19;
  else if(absPdgId > 3000 && absPdgId < 4000  ) binValue = 20;
  else if(absPdgId > 4000 && absPdgId < 5000  ) binValue = 21;
  else if(absPdgId > 5000 && absPdgId < 6000  ) binValue = 22;
  else if(absPdgId == 92  ) binValue = 23;

  else binValue = 24;

  return binValue;

}


int ValueLookup::findTauMotherIndex(const BNmcparticle* tau){

  int bestMatchIndex = -1;
  double bestMatchDeltaR = 999;

  for(BNmcparticleCollection::const_iterator mcparticle = mcparticles->begin (); mcparticle != mcparticles->end (); mcparticle++){

    if(fabs(mcparticle->id) != 15 || mcparticle->status !=3) continue;

    double currentDeltaR = deltaR(tau->eta,tau->phi,mcparticle->eta,mcparticle->phi);
    if(currentDeltaR > 0.05) continue;

    if(currentDeltaR < bestMatchDeltaR && mcparticles->at(mcparticle - mcparticles->begin()).id != mcparticles->at(mcparticle - mcparticles->begin()).motherId){
      bestMatchIndex = mcparticle - mcparticles->begin();
      bestMatchDeltaR = currentDeltaR;
    }

  }

  return bestMatchIndex;
}


unsigned int ValueLookup::GetNumExtraPartons(const BNmcparticleCollection* genPartColl){

  int nList = 0;
  unsigned int nPartons = 0;
  for( BNmcparticleCollection::const_iterator mcparticle = genPartColl->begin(); mcparticle != genPartColl->end(); mcparticle++)
   {
     nList ++;
    int id = mcparticle->id;
    int motherID = mcparticle->motherId;
    int status = mcparticle->status;
    int aid = abs(id);

    if(status == 3){//only status 3 particles (which are listed first)
      if(nList>6){//dont look at first 6 (incomming event)
        if( (aid>0 && aid<6) || aid ==21 || aid ==9){//udscb gluon
          if(abs(motherID) !=23 && abs(motherID) !=24 && abs(motherID) != 25 && abs(motherID)!=6 ){ //not from WZHt
            nPartons++;
          }
        }
      }
    }else{
      break; //break when you run out of stat==3
    }
  }

  return nPartons;

}


double
ValueLookup::getSumJetPt (const BNjetCollection* jetColl){
  double sumJetPt = 0;
  TLorentzVector hT (0, 0, 0, 0);
  for(BNjetCollection::const_iterator jet = jetColl->begin(); jet !=jetColl->end(); jet++){
    TLorentzVector hT_2(0, 0, 0, 0);
    hT_2.SetPtEtaPhiM(jet->pt, jet->eta, jet->phi, jet->mass);
    hT += hT_2;
    sumJetPt = hT.Pt();
    
  }
  return sumJetPt;
}


//calculate the scalar sum of Electron, Muon, and Jet Pt in the event.
double
ValueLookup::getSt (const BNelectronCollection* electronColl, const BNmuonCollection* muonColl, const BNjetCollection* jetColl){
  double St = 0;
  if (!electronColl || !muonColl || !jetColl) return -999.0;
  for(BNelectronCollection::const_iterator electron = electronColl->begin(); electron !=electronColl->end(); electron++){
    St += abs(electron->pt);
  }
  for(BNmuonCollection::const_iterator muon = muonColl->begin(); muon !=muonColl->end(); muon++){
    St += abs(muon->pt);
  }
  for(BNjetCollection::const_iterator jet = jetColl->begin(); jet !=jetColl->end(); jet++){
    St += abs(jet->pt);
  }

  return St;
}


pair<const BNmuon *, const BNmuon *>
ValueLookup::leadMuonPair ()
{
  pair<const BNmuon *, const BNmuon *> leadMuonPair;
  leadMuonPair.first = leadMuonPair.second = 0;

  if(cumulativeFlags.find ("muons") != cumulativeFlags.end ()){
    flagPair muonFlags;
    for (int i = cumulativeFlags.at("muons").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("muons").at(i).size()){
        muonFlags = cumulativeFlags.at("muons").at(i);
        break;
      }
    }
    for (uint muonIndex0 = 0; muonIndex0 != muonFlags.size(); muonIndex0++){
      if(!muonFlags.at(muonIndex0).first) continue;
      for (uint muonIndex1 = muonIndex0 + 1; muonIndex1 < muonFlags.size(); muonIndex1++){
        if(!muonFlags.at(muonIndex1).first) continue;
        const BNmuon *mu0 = & muons->at(muonIndex0);
        const BNmuon *mu1 = & muons->at(muonIndex1);
        if(leadMuonPair.first == 0 || leadMuonPair.second == 0){
          leadMuonPair.first = mu0;
          leadMuonPair.second = mu1;
        }
        else{
          TVector2 newPt0 (mu0->px, mu0->py),
                   newPt1 (mu1->px, mu1->py),
                   oldPt0 (leadMuonPair.first->px, leadMuonPair.first->py),
                   oldPt1 (leadMuonPair.second->px, leadMuonPair.second->py);
          if(newPt0.Mod () + newPt1.Mod () > oldPt0.Mod() + oldPt1.Mod ())
            {
              leadMuonPair.first = mu0;
              leadMuonPair.second = mu1;
            }
        }
      }
    }
  }

  return leadMuonPair;
}


pair<const BNelectron *, const BNelectron *>
ValueLookup::leadElectronPair ()
{
  pair<const BNelectron *, const BNelectron *> leadElectronPair;
  leadElectronPair.first = leadElectronPair.second = 0;
  if(cumulativeFlags.find ("electrons") != cumulativeFlags.end ()){
    flagPair electronFlags;
    for (int i = cumulativeFlags.at("electrons").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("electrons").at(i).size()){
        electronFlags = cumulativeFlags.at("electrons").at(i);
        break;
      }
    }
    for (uint electronIndex0 = 0; electronIndex0 != electronFlags.size(); electronIndex0++){
      if(!electronFlags.at(electronIndex0).first) continue;
      for (uint electronIndex1 = electronIndex0 + 1; electronIndex1 < electronFlags.size(); electronIndex1++){
        if(!electronFlags.at(electronIndex1).first) continue;
        const BNelectron *el0 = & electrons->at(electronIndex0);
        const BNelectron *el1 = & electrons->at(electronIndex1);
        if(leadElectronPair.first == 0 || leadElectronPair.second == 0){
          leadElectronPair.first = el0;
          leadElectronPair.second = el1;
        }
        else{
          TVector2 newPt0 (el0->px, el0->py),
                   newPt1 (el1->px, el1->py),
                   oldPt0 (leadElectronPair.first->px, leadElectronPair.first->py),
                   oldPt1 (leadElectronPair.second->px, leadElectronPair.second->py);
          if(newPt0.Mod () + newPt1.Mod () > oldPt0.Mod() + oldPt1.Mod ())
            {
              leadElectronPair.first = el0;
              leadElectronPair.second = el1;
            }
        }
      }
    }
  }

  return leadElectronPair;
}


//return corrected jet
BNjet ValueLookup::getCorrectedJet(const BNjet &iJet, string jERCase){

    if(datasetType_ == "data"){ return iJet; }
    double jetFactor = 1;

  // Make appopriate correction to the xy components of input jet
    if(jERCase == "JERup")   jetFactor *= getJERfactor(1, fabs(iJet.eta), iJet.genJetPT, iJet.pt);
    if(jERCase == "JERdown") jetFactor *= getJERfactor(-1, fabs(iJet.eta), iJet.genJetPT, iJet.pt);
    if(jERCase == "JESup") {
      jetFactor *= (1. + iJet.JESunc);
      jetFactor *= getJERfactor(0, fabs(iJet.eta), iJet.genJetPT, jetFactor * iJet.pt);
    }
    if(jERCase =="JESdown"){
      jetFactor *= (1. - iJet.JESunc);
      jetFactor *= getJERfactor(0, fabs(iJet.eta), iJet.genJetPT, jetFactor * iJet.pt);
    }
    else jetFactor *= getJERfactor(0, fabs(iJet.eta), iJet.genJetPT, iJet.pt);
  // Make a copy of the input jet for output and update 4-vector values
  BNjet result = iJet;
    
  result.px   *= jetFactor;
  result.py   *= jetFactor;
  result.pz   *= jetFactor;
  result.pt   *= jetFactor;
  result.et      *= jetFactor;
  result.energy *= jetFactor;
  result.mass     *= jetFactor;

  // Update CSV value (i.e. reshape it if applicable)
  // result.btagCombinedSecVertex = GetCSVvalue(result,iSysType);

  return result;


}


double
ValueLookup::getDetectorEta(double evntEta,double z0) {
  // Use algorithm from Sam Harper, https://hypernews.cern.ch/HyperNews/CMS/get/EXO-12-034/19.html  
  double thetaEvt = etaToTheta(evntEta);
  //   std::cout << "**************************************** " << std::endl;
  //   std::cout << "Event Theta: " << thetaEvt << std::endl;
  double z = 129.4 / tan(thetaEvt); //129.4 is the average barrel radius
  double zTot = z+z0;
  
  if(fabs(zTot)<269){ //269 is an emperically derived number which means that < its likely in th barrel
    return zTot !=0 ? thetaToEta(atan(129.4/zTot)) : 0.;
  }

  //otherwise endcap time
  double endcapZ = 319.2; //average z position of endcap
  if(evntEta<0) endcapZ*=-1;
  double rxy = tan(thetaEvt) * (endcapZ-z0);
  //   std::cout << "rxy: " << rxy << std::endl;
  //   std::cout << "endcapZ: " << endcapZ << std::endl;
  //   std::cout << "Event Eta: " << evntEta << std::endl;
  //   std::cout << "Result of getDetectorEta: " << thetaToEta(atan(rxy/endcapZ)) << std::endl;
  //  std::cout << "Result of getDetectorEta: " << -log(tan((atan(rxy/endcapZ))/2)) << std::endl;
  return thetaToEta(atan(rxy/endcapZ));
  //  return -log(tan((atan(rxy/endcapZ))/2));
  
}


double
ValueLookup::getTrkDepTrkRp5RhoCorr(const BNtrack* track) {
  // Return the pile-up (rho) corrected isolation energy, i.e., the total calorimeter energy around the candidate track.
  if (!useTrackCaloRhoCorr_) return -99;
  // if (!rhokt6CaloJetsHandle_) {
  //   clog << "ERROR [getTrkCaloTotRhoCorr]:  The collection rhokt6CaloJetsHandle is not available!" << endl;
  //   return -99;
  // }
  double radDeltaRCone = 0.5;
  double rhoCorr_kt6CaloJets = *rhokt6CaloJetsHandle_ * TMath::Pi() * pow(radDeltaRCone, 2);  // Define effective area as pi*r^2, where r is radius of DeltaR cone.
  double rawDepTrkRp5 = track->depTrkRp5;
 double depTrkRp5RhoCorr = TMath::Max(0., rawDepTrkRp5 - rhoCorr_kt6CaloJets);
 return depTrkRp5RhoCorr;

}


double
ValueLookup::getTrkDepTrkRp3RhoCorr(const BNtrack* track) {
  // Return the pile-up (rho) corrected isolation energy, i.e., the total calorimeter energy around the candidate track.
  if (!useTrackCaloRhoCorr_) return -99;
  // if (!rhokt6CaloJetsHandle_) {
  //   clog << "ERROR [getTrkCaloTotRhoCorr]:  The collection rhokt6CaloJetsHandle is not available!" << endl;
  //   return -99;
  // }
  double radDeltaRCone = 0.3;
// Define effective area as pi*r^2, where r is radius of DeltaR cone
  double rhoCorr_kt6CaloJets = *rhokt6CaloJetsHandle_ * TMath::Pi() * pow(radDeltaRCone, 2);
  double rawDepTrkRp3 = track->depTrkRp3;
  double depTrkRp3RhoCorr = TMath::Max(0., rawDepTrkRp3 - rhoCorr_kt6CaloJets);
  return depTrkRp3RhoCorr;

}


// Calculate the number of tracks in cone of DeltaR<0.5 around track1.
// Return true iff no other tracks are found in this cone.
int
ValueLookup::getTrkIsIso (const BNtrack* track1, const BNtrackCollection* trackColl){
  for(BNtrackCollection::const_iterator track2 = trackColl->begin(); track2 !=trackColl->end(); track2++){
    if(track1->eta == track2->eta && track1->phi == track2->phi) continue; // Do not compare the track to itself.
    double deltaRtrk = deltaR(track1->eta, track1->phi, track2->eta, track2->phi);
    if(deltaRtrk < 0.5) return 0;
  }
  return 1;

}


//if a track is found within dR<0.05 of a dead Ecal channel value = 1, otherwise value = 0
int
ValueLookup::getTrkIsMatchedDeadEcal (const BNtrack* track1){
  int value = 0;
  if (getTrkDeadEcalDeltaR(track1)<0.05) {value = 1;}
  else {value = 0;}
  return value;
}



int
ValueLookup::getTrkIsMatchedDeadEcalDet (const BNtrack* track1){
  int value = 0;
  if (getDetDeadEcalDeltaR(track1)<0.05) {value = 1;}
  else {value = 0;}
  return value;
}


double
ValueLookup::getTrkDeadEcalDeltaR (const BNtrack* track1){
  double deltaRLowest = 999;
  if (DeadEcalVec.size() == 0) WriteDeadEcal();
  for(vector<DeadEcal>::const_iterator ecal = DeadEcalVec.begin(); ecal != DeadEcalVec.end(); ++ecal){
    double eta = ecal->etaEcal;
    double phi = ecal->phiEcal;
    double deltaRtemp = deltaR(eta, phi, track1->eta, track1->phi);
    if(deltaRtemp < deltaRLowest) deltaRLowest = deltaRtemp;
  }
  if (verbose_) clog << deltaRLowest << endl;
  return deltaRLowest;
}


// If a track is found within dR<0.25 of a bad CSC chamber, value = 1, otherwise value = 0.  
// FIXME:  Instead of a deltaR cut, it would be better to use the boundaries of the chamber 
int
ValueLookup::getTrkIsMatchedBadCSC (const BNtrack* track1){
  double deltaRLowest = 999;
  int value = 0;
  if (BadCSCVec.size() == 0) WriteBadCSC();
  for(vector<BadCSC>::const_iterator csc = BadCSCVec.begin(); csc != BadCSCVec.end(); ++csc) {
    double eta = csc->etaCSC;
    double phi = csc->phiCSC;
    double deltaRtemp = deltaR(eta, phi, track1->eta, track1->phi);
    if(deltaRtemp < deltaRLowest) deltaRLowest = deltaRtemp;
  }
  if (deltaRLowest<0.25) { value = 1; }
  else                  { value = 0; } 
  return value;
}


double
ValueLookup::getTrkPtRes (const BNtrack* track1){

  double ptTrue = getTrkPtTrue(track1, mcparticles.product());
  double PtRes = (track1->pt - ptTrue) / ptTrue;

  return PtRes;

}


double
ValueLookup::getTrkPtTrue (const BNtrack* track1, const BNmcparticleCollection* genPartColl){
  double value = -99;
  double genDeltaRLowest = 999;

  for (BNmcparticleCollection::const_iterator genPart = genPartColl->begin(); genPart !=genPartColl->end(); genPart++){
    double genDeltaRtemp = deltaR(genPart->eta, genPart->phi,track1->eta, track1->phi);
    if (genDeltaRtemp < genDeltaRLowest) {
      genDeltaRLowest = genDeltaRtemp;
      if (genDeltaRLowest < 0.15) {   // Only consider it truth-matched if DeltaR<0.15.
        double ptTrue = genPart->pt;
        value = ptTrue;
      }
    }
  }

  return value;

}


double ValueLookup::getPtSingleObjectMatchesAnyTrigger(double recoPhi, const vector<string> & targetTriggers, const BNtrigobjCollection * triggerObjects){

  double matchPt = -99;

  for ( BNtrigobjCollection::const_iterator iObj = triggerObjects->begin();
        iObj != triggerObjects->end();
        iObj ++ ) {
    // look for a matching name again                                                                                                          
    for (vector<string>::const_iterator iTarget = targetTriggers.begin();
         iTarget != targetTriggers.end();
         iTarget++) {
      // if this is the right name                                                                                                             
      if ( iObj->filter.find((*iTarget)) != std::string::npos) {
        double deltaPhiCalc = fabs(deltaPhi(recoPhi, iObj->phi));

        if (deltaPhiCalc < 0.3) matchPt = iObj->pt;

      } 

    }// end for each target                                                                                                                    


  } // end for each object                                                                                                                     


  return matchPt;


}


double ValueLookup::getJERfactor(int returnType, double jetAbsETA, double genjetPT, double recojetPT){

  double factor = 1.;
  
  double scale_JER = 1., scale_JERup = 1., scale_JERdown = 1.;
  if( jetAbsETA<0.5 ){ 
    scale_JER = 1.052; scale_JERup = 1.052 + sqrt( 0.012*0.012 + 0.062*0.062 ); scale_JERdown = 1.052 - sqrt( 0.012*0.012 + 0.061*0.061 );
  }
  else if( jetAbsETA<1.1 ){ 
    scale_JER = 1.057; scale_JERup = 1.057 + sqrt( 0.012*0.012 + 0.056*0.056 ); scale_JERdown = 1.057 - sqrt( 0.012*0.012 + 0.055*0.055 );
  }
  else if( jetAbsETA<1.7 ){ 
    scale_JER = 1.096; scale_JERup = 1.096 + sqrt( 0.017*0.017 + 0.063*0.063 ); scale_JERdown = 1.096 - sqrt( 0.017*0.017 + 0.062*0.062 );
  }
  else if( jetAbsETA<2.3 ){ 
    scale_JER = 1.134; scale_JERup = 1.134 + sqrt( 0.035*0.035 + 0.087*0.087 ); scale_JERdown = 1.134 - sqrt( 0.035*0.035 + 0.085*0.085 );
  }
  else if( jetAbsETA<5.0 ){ 
    scale_JER = 1.288; scale_JERup = 1.288 + sqrt( 0.127*0.127 + 0.155*0.155 ); scale_JERdown = 1.288 - sqrt( 0.127*0.127 + 0.153*0.153 );
  }

  double jetPt_JER = recojetPT;
  double jetPt_JERup = recojetPT;
  double jetPt_JERdown = recojetPT;

  double diff_recojet_genjet = recojetPT - genjetPT;

  if( genjetPT>10. ){
    jetPt_JER = std::max( 0., genjetPT + scale_JER * ( diff_recojet_genjet ) );
    jetPt_JERup = std::max( 0., genjetPT + scale_JERup * ( diff_recojet_genjet ) );
    jetPt_JERdown = std::max( 0., genjetPT + scale_JERdown * ( diff_recojet_genjet ) );
  }

  if( returnType==1 )       factor = jetPt_JERup/recojetPT;
  else if( returnType==-1 ) factor = jetPt_JERdown/recojetPT;
  else                      factor = jetPt_JER/recojetPT;

  if( !(genjetPT>10.) ) factor = 1.;

  return factor;


}


double
ValueLookup::etaToTheta(double eta) {
  if(eta<0) return -2*atan(exp(eta));
  else return 2*atan(exp(-1*eta));
}


double
ValueLookup::getDetDeadEcalDeltaR (const BNtrack* track1){
  double deltaRLowest = 999;
  if (DeadEcalVec.size() == 0) WriteDeadEcal();
  for(vector<DeadEcal>::const_iterator ecal = DeadEcalVec.begin(); ecal != DeadEcalVec.end(); ++ecal){
    double eta = ecal->etaEcal;
    double phi = ecal->phiEcal;
    double deltaRtemp = deltaR(eta, phi, getDetectorEta(track1->eta, track1->dZ), track1->phi);
    if(deltaRtemp < deltaRLowest) deltaRLowest = deltaRtemp;
  }
  if (verbose_) clog << deltaRLowest << endl;
  return deltaRLowest;
}


//creates a map of the dead Ecal channels in the barrel and endcap
//to see how the map of dead Ecal channels is created look at function getChannelStatusMaps() here:
//http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/jbrinson/DisappTrk/OSUT3Analysis/AnaTools/src/OSUAnalysis.cc?revision=1.88&view=markup
void
ValueLookup::WriteDeadEcal (){
  double etaEcal, phiEcal;
  ifstream DeadEcalFile(deadEcalFile_);
  if(!DeadEcalFile) {
    clog << "Error: DeadEcalFile has not been found." << endl;
    return;
  }
  if(DeadEcalVec.size()!= 0){
    clog << "Error: DeadEcalVec has a nonzero size" << endl;
    return;
  }
  while(!DeadEcalFile.eof())
    {
      DeadEcalFile >> etaEcal >> phiEcal;
      DeadEcal newChan;
      newChan.etaEcal = etaEcal;
      newChan.phiEcal = phiEcal;
      DeadEcalVec.push_back(newChan);
    }
  if(DeadEcalVec.size() == 0) clog << "Warning: No dead Ecal channels have been found." << endl;
}


// Creates a map of the bad CSC chambers.  
// The list of bad CSC chambers is taken from:  
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/CSCDPGConditions#CSC_bad_chambers_in_2012_30_05_2
void
ValueLookup::WriteBadCSC() {
  double etaCSC, phiCSC;
  ifstream BadCSCFile(badCSCFile_);
  if (!BadCSCFile) {
    clog << "Error: BadCSCFile has not been found." << endl;
    return;
  }
  if (BadCSCVec.size()!= 0) {
    clog << "Error: BadCSCVec has a nonzero size" << endl;
    return;
  }
  while (!BadCSCFile.eof()) {
    BadCSCFile >> etaCSC >> phiCSC;
    BadCSC newChan;
    newChan.etaCSC = etaCSC;
    newChan.phiCSC = phiCSC;
    BadCSCVec.push_back(newChan);
    //    clog << "Debug:  Adding bad CSC with eta=" << etaCSC << ", phi=" << phiCSC << endl;  
  }
  if (BadCSCVec.size() == 0) clog << "Warning: No bad CSC chambers have been found." << endl;
}  


double
ValueLookup::thetaToEta(double theta) {
  if (theta < 0) theta += TMath::Pi();  // to calculate eta, theta must be in range (0,pi)
  return -log(tan(theta/2.));
}


flagPair ValueLookup::getLastValidFlags(string objType) {
  // get the last valid flags in the flag map
  for (int i = cumulativeFlags.at(objType).size() - 1; i >= 0; i--) {  // loop backwards over all the cuts
    if (cumulativeFlags.at(objType).at(i).size()){  	  // If all the flags have been filled, then the last cut will have a nonzero size
      return cumulativeFlags.at(objType).at(i);  
    }
  }
  // no valid flags have been found 
  flagPair empty;
  return empty;  
}  
