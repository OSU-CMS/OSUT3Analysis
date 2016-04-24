#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"
#include "OSUT3Analysis/ExampleAnalysis/plugins/MyVariableProducer.h"

MyVariableProducer::MyVariableProducer(const edm::ParameterSet &cfg) :
  EventVariableProducer(cfg)
{
  if (collections_.exists ("electrons"))
    tokens_.electrons = consumes<vector<pat::Electron> > (collections_.getParameter<edm::InputTag> ("electrons"));
  if (collections_.exists ("jets"))
    tokens_.jets = consumes<vector<pat::Jet> > (collections_.getParameter<edm::InputTag> ("jets"));
  if (collections_.exists ("muons"))
    tokens_.muons = consumes<vector<pat::Muon> > (collections_.getParameter<edm::InputTag> ("muons"));
  if (collections_.exists ("primaryvertexs"))
    tokens_.primaryvertexs = consumes<vector<reco::Vertex> > (collections_.getParameter<edm::InputTag> ("primaryvertexs"));
  if (collections_.exists ("pileupinfos"))
    tokens_.pileupinfos = consumes<vector<PileupSummaryInfo> > (collections_.getParameter<edm::InputTag> ("pileupinfos"));
  if (collections_.exists ("triggers"))
    tokens_.triggers = consumes<edm::TriggerResults> (collections_.getParameter<edm::InputTag> ("triggers"));
}

MyVariableProducer::~MyVariableProducer() {}

void
MyVariableProducer::AddVariables (const edm::Event &event) {

  // Add all of the needed collections to objectsToGet_
  objectsToGet_.insert ("muon");

  // get all the needed collections from the event and put them into the "handles_" collection
  //anatools::getRequiredCollections (objectsToGet_, collections_, handles_, event);
  getOriginalCollections (objectsToGet_, event);

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
MyVariableProducer::getOriginalCollections (const unordered_set<string> &objectsToGet, const edm::Event &event)
{

  //////////////////////////////////////////////////////////////////////////////
  // Retrieve each object collection which we need and print a warning if it is
  // missing.
  //////////////////////////////////////////////////////////////////////////////
  if  (VEC_CONTAINS  (objectsToGet,  "electrons"))       event.getByToken  (tokens_.electrons,       handles_.electrons);
  if  (VEC_CONTAINS  (objectsToGet,  "jets"))            event.getByToken  (tokens_.jets,            handles_.jets);
  if  (VEC_CONTAINS  (objectsToGet,  "muons"))           event.getByToken  (tokens_.muons,           handles_.muons);
  if  (VEC_CONTAINS  (objectsToGet,  "primaryvertexs"))  event.getByToken  (tokens_.primaryvertexs,  handles_.primaryvertexs);
  if  (VEC_CONTAINS  (objectsToGet,  "pileupinfos"))     event.getByToken  (tokens_.pileupinfos,     handles_.pileupinfos);
  if  (VEC_CONTAINS  (objectsToGet,  "triggers"))        event.getByToken  (tokens_.triggers,        handles_.triggers);
}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(MyVariableProducer);
