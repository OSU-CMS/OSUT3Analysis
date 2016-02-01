#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"
#include "OSUT3Analysis/ExampleAnalysis/plugins/MyVariableProducer.h"

MyVariableProducer::MyVariableProducer(const edm::ParameterSet &cfg) :
  EventVariableProducer(cfg) {}

MyVariableProducer::~MyVariableProducer() {}

void
MyVariableProducer::AddVariables (const edm::Event &event) {

  // Add all of the needed collections to objectsToGet_
  objectsToGet_.insert ("muons");

  // get all the needed collections from the event and put them into the "handles_" collection
  //anatools::getRequiredCollections (objectsToGet_, collections_, handles_, event);
  getOriginalCollections (objectsToGet_, collections_, handles_, event);

  // calculate whatever variables you'd like
  double muonPt = 0.;

  // simple case, just muonPt
  for (const auto &muon1 : *handles_.muons) {
    muonPt = muon1.pt();
    //double value = anatools::getMember(muon1, "pt");
    //addUserVar("muonPt", value, muon1);
  }
  (*eventvariables)["muonPt"] = muonPt;
}


void
MyVariableProducer::getOriginalCollections (const unordered_set<string> &objectsToGet, const edm::ParameterSet &collections, OriginalCollections &handles, const edm::Event &event)
{

  //////////////////////////////////////////////////////////////////////////////
  // Retrieve each object collection which we need and print a warning if it is
  // missing.
  //////////////////////////////////////////////////////////////////////////////
  if  (VEC_CONTAINS  (objectsToGet,  "electrons")         &&  collections.exists  ("electrons"))         anatools::getCollection  (collections.getParameter<edm::InputTag>  ("electrons"),         handles.electrons,         event);
  if  (VEC_CONTAINS  (objectsToGet,  "jets")              &&  collections.exists  ("jets"))              anatools::getCollection  (collections.getParameter<edm::InputTag>  ("jets"),              handles.jets,              event);
  if  (VEC_CONTAINS  (objectsToGet,  "muons")             &&  collections.exists  ("muons"))             anatools::getCollection  (collections.getParameter<edm::InputTag>  ("muons"),             handles.muons,             event);
  if  (VEC_CONTAINS  (objectsToGet,  "primaryvertexs")    &&  collections.exists  ("primaryvertexs"))    anatools::getCollection  (collections.getParameter<edm::InputTag>  ("primaryvertexs"),    handles.primaryvertexs,    event);
  if  (VEC_CONTAINS  (objectsToGet,  "pileupinfos")    &&  collections.exists  ("pileupinfos"))    anatools::getCollection  (collections.getParameter<edm::InputTag>  ("pileupinfos"),    handles.pileupinfos,    event);
  if  (VEC_CONTAINS  (objectsToGet,  "triggers")    &&  collections.exists  ("triggers"))    anatools::getCollection  (collections.getParameter<edm::InputTag>  ("triggers"),    handles.triggers,    event);
}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(MyVariableProducer);
