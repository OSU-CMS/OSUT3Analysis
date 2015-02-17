#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"
#include "OSUT3Analysis/ExampleAnalysis/plugins/MyVariableProducer.h"

MyVariableProducer::MyVariableProducer(const edm::ParameterSet &cfg) :
  VariableProducer(cfg) {}

MyVariableProducer::~MyVariableProducer() {}

void
MyVariableProducer::AddVariables (const edm::Event &event, auto_ptr<VariableProducerPayload> &myVars) {

  // Add all of the needed collections to objectsToGet_  
  objectsToGet_.insert ("muons");

  anatools::getRequiredCollections (objectsToGet_, collections_, handles_, event);


#if DATA_FORMAT == BEAN
  if (!collections_.exists ("muons")) {cout << "no muons" << endl; return;}

  edm::Handle<BNmuonCollection> muonHandle;
  anatools::getCollection (collections_.getParameter<edm::InputTag> ("muons"), muonHandle, event);

  edm::Handle<BNeventCollection> eventHandle;
  anatools::getCollection (collections_.getParameter<edm::InputTag> ("events"), eventHandle, event);

  edm::Handle<BNmetCollection> metHandle;
  anatools::getCollection (collections_.getParameter<edm::InputTag> ("mets"), metHandle, event);

  edm::Handle<BNprimaryvertexCollection> primaryvertexHandle;
  anatools::getCollection (collections_.getParameter<edm::InputTag> ("primaryvertexs"), primaryvertexHandle, event);

  const BNmuonCollection* muons = muonHandle.product();
  //const BNeventCollection* events = eventHandle.product();
  const BNmetCollection* mets = metHandle.product();
  //const BNprimaryvertexCollection* primaryvertexs = primaryvertexHandle.product();

  // for (BNmuonCollection::const_iterator muon = muons->begin(); muon != muons->end(); muon++){
  //   cout << "muon px: " << muon->px << endl;
  //   cout << "muon py: " << muon->py << endl;
  //   cout << "muon pz: " << muon->pz << endl;
  //   cout << "muon hash: " << objectHash(*muon) << endl;
  // }

  // for (BNprimaryvertexCollection::const_iterator primaryvertex = primaryvertexs->begin(); primaryvertex != primaryvertexs->end(); primaryvertex++){
  //   cout << "primaryvertex x: " << primaryvertex->x << endl;
  //   cout << "primaryvertex y: " << primaryvertex->y << endl;
  //   cout << "primaryvertex z: " << primaryvertex->z << endl;
  //   cout << "primaryvertex hash: " << objectHash(*primaryvertex) << endl;
  // }

  // cout << "event run: " << events->at(0).run << endl;
  // cout << "event lumi: " << events->at(0).lumi << endl;
  // cout << "event evt: " << events->at(0).evt << endl;
  // cout << "event hash: " << objectHash(events->at(0)) << endl;

  // cout << "met px: " << mets->at(0).px << endl;
  // cout << "met py: " << mets->at(0).py << endl;
  // cout << "met hash: " << objectHash(mets->at(0)) << endl;

  //calculate and store dimuon-met deltaPhi

  for (BNmuonCollection::const_iterator muon1 = muons->begin(); muon1 != muons->end(); muon1++){
    for (BNmuonCollection::const_iterator muon2 = muons->begin(); muon2 != muons->end(); muon2++){
      if (muon1 >= muon2) continue; // account for duplicates
      
      // add the 4-vectors of the two muons
      TLorentzVector muon1vector;
      TLorentzVector muon2vector;
      muon1vector.SetPxPyPzE(muon1->px, muon1->py, muon1->pz, muon1->energy);
      muon2vector.SetPxPyPzE(muon2->px, muon2->py, muon2->pz, muon2->energy);
      TLorentzVector dimuonVector = muon1vector + muon2vector;

      // take delta phi with met
      BNmet met = mets->at(0);
      double deltaphi = deltaPhi(dimuonVector.Phi(),met.phi);

      // add objects to the list of hashes
      vector<int> hashes;
      hashes.push_back(objectHash(*muon1));
      hashes.push_back(objectHash(*muon2));
      hashes.push_back(objectHash(met));
      
      UserVar dimuonMetDeltaPhi = make_pair(hashes,deltaphi);
      (*myVars)["dimuonMetDeltaPhi"].push_back(dimuonMetDeltaPhi);

    }
  }

  
#endif

  for (auto muon1 = handles_.muons->begin(); muon1 != handles_.muons->end(); muon1++) {  
    // TYPE_STR(handles_.muons) will be something like "handles_.BNmuon", so we use: 
    // string(TYPE_STR(handles_.muons)).substr(9) to remove the first 9 characters 
    // and get something like "BNmuon".  
    double value = anatools::getMember(string(TYPE_STR(handles_.muons)).substr(9), &(*muon1), "pt"); 
    addUserVar(myVars, "muonSF", value, muon1);  
  }



}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(MyVariableProducer);
