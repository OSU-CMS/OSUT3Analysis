#include "OSUT3Analysis/AnaTools/plugins/MyEventVariableProducer.h"

MyEventVariableProducer::MyEventVariableProducer(const edm::ParameterSet &cfg) :
  EventVariableProducer(cfg) {}

MyEventVariableProducer::~MyEventVariableProducer() {}

void
MyEventVariableProducer::AddVariables (const edm::Event &event, map<string, double> &myVars) {
  
  event.getByLabel (muons_, muons);
  const BNmuonCollection *Muons = muons.product();
  
  myVars["numMuons"] = Muons->size();

}

DEFINE_FWK_MODULE(MyEventVariableProducer);
