#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"  
#include "OSUT3Analysis/AnaTools/plugins/VariableProducer.h"

VariableProducer::VariableProducer(const edm::ParameterSet &cfg) :
  collectionMap_ (cfg.getParameter<edm::ParameterSet> ("inputsMap"))
{

  //  produces<map<string, double> > ("userVariables");
  produces<vector<map<string, double> > > ("userVariables");

  jets_ = collectionMap_.getParameter<edm::InputTag> ("jets");
  muons_ = collectionMap_.getParameter<edm::InputTag> ("muons");
  electrons_ = collectionMap_.getParameter<edm::InputTag> ("electrons");
  taus_ = collectionMap_.getParameter<edm::InputTag> ("taus");
  mets_ = collectionMap_.getParameter<edm::InputTag> ("mets");
  genjets_ = collectionMap_.getParameter<edm::InputTag> ("genjets");
  mcparticles_ = collectionMap_.getParameter<edm::InputTag> ("mcparticles");
  primaryvertexs_ = collectionMap_.getParameter<edm::InputTag> ("primaryvertexs");
  photons_ = collectionMap_.getParameter<edm::InputTag> ("photons");
  triggers_ = collectionMap_.getParameter<edm::InputTag> ("triggers");
}

VariableProducer::~VariableProducer()
{
}

void
VariableProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  // define structure that will be put into the event
  auto_ptr<vector<map<string, double> > > userVariables (new vector<map<string, double> >);  


  // get whatever we need from the event
  event.getByLabel (muons_, muons);
  event.getByLabel (mets_, mets);

  ////////////////////////////////////////////////////////////////////////
  map<string, double> myVars; 
  myVars["dimuonMetDeltaPhi"] = setDiMuonMetDeltaPhi(muons.product(), mets.product());
  myVars["dimuonInvMass"]     = getDiMuonInvMass(muons.product());  
  myVars["metPt"] = mets->at(0).pt; 

  userVariables->push_back(myVars);  


  ////////////////////////////////////////////////////////////////////////

  // store all of our calculated quantities in the event  
  event.put (userVariables, "userVariables");
  userVariables.reset();  


}


// calculate the invariant mass of muon pair in event 
double
VariableProducer::getDiMuonInvMass(const BNmuonCollection *muons) { 

  // if not exactly two muons, return a dummy value
  if (muons->size() != 2) { 
    return -99;
  } else {
    BNmuon muon1 = muons->at(0);
    BNmuon muon2 = muons->at(1);
    TLorentzVector muon1vector;  
    TLorentzVector muon2vector; 
    muon1vector.SetPxPyPzE(muon1.px, muon1.py, muon1.pz, muon1.energy);
    muon2vector.SetPxPyPzE(muon2.px, muon2.py, muon2.pz, muon2.energy);
    TLorentzVector dimuonVector = muon1vector + muon2vector;
    return dimuonVector.M();  
  }

}




// calculate the delta phi between the leading dimuon pair and the met
double
VariableProducer::setDiMuonMetDeltaPhi(const BNmuonCollection *muons, const BNmetCollection *mets){

  // if the right objects aren't in the event, just return a dummy value
  if(muons->size() < 2 or mets->size() < 1){
    return -99;
  }
  else {
    // get 4-vector of dimuon system
    BNmuon muon1 = muons->at(0);
    BNmuon muon2 = muons->at(1);
    TLorentzVector muon1vector;  
    TLorentzVector muon2vector; 
    muon1vector.SetPxPyPzE(muon1.px, muon1.py, muon1.pz, muon1.energy);
    muon2vector.SetPxPyPzE(muon2.px, muon2.py, muon2.pz, muon2.energy);
    TLorentzVector dimuonVector = muon1vector + muon2vector;
    // take delta phi with met
    BNmet met = mets->at(0);
    double deltaphi = deltaPhi(dimuonVector.Phi(),met.phi);
    return fabs(deltaphi);
  }

}

DEFINE_FWK_MODULE(VariableProducer);
