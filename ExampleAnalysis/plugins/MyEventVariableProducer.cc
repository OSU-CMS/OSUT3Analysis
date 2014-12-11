#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"
#include "OSUT3Analysis/ExampleAnalysis/plugins/MyEventVariableProducer.h"

MyEventVariableProducer::MyEventVariableProducer(const edm::ParameterSet &cfg) :
  EventVariableProducer(cfg) {}

MyEventVariableProducer::~MyEventVariableProducer() {}

void
MyEventVariableProducer::AddVariables (const edm::Event &event, auto_ptr<EventVariableProducerPayload> &myVars) {

  edm::Handle<BNmuonCollection> muons;
  if (collections_.exists ("muons")) anatools::getCollection (collections_.getParameter<edm::InputTag> ("muons"), muons, event);

  (*myVars)["numMuons"] = muons->size();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(MyEventVariableProducer);
