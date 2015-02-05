#include "OSUT3Analysis/AnaTools/interface/VariableProducer.h"

VariableProducer::VariableProducer(const edm::ParameterSet &cfg) :
  collections_  (cfg.getParameter<edm::ParameterSet>  ("collections"))
{
  produces<VariableProducerPayload> ("userVariables");
}

VariableProducer::~VariableProducer()
{
}

void
VariableProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{

  auto_ptr<VariableProducerPayload> userVariables (new VariableProducerPayload);

  ////////////////////////////////////////////////////////////////////////

  AddVariables(event, userVariables);

  // store all of our calculated quantities in the event
  event.put (userVariables, "userVariables");
}


// implementation left up to the daughter class
void
VariableProducer::AddVariables (const edm::Event &event, auto_ptr<VariableProducerPayload> &myVars) {}
