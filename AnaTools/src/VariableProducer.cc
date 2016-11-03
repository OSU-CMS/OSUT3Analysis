#include "OSUT3Analysis/AnaTools/interface/VariableProducer.h"

VariableProducer::VariableProducer(const edm::ParameterSet &cfg) :
  collections_  (cfg.getParameter<edm::ParameterSet>  ("collections"))
{
  produces<VariableProducerPayload> ("uservariables");
}

VariableProducer::~VariableProducer()
{
}

void
VariableProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{

  uservariables = unique_ptr<VariableProducerPayload> (new VariableProducerPayload);

  ////////////////////////////////////////////////////////////////////////

  AddVariables(event);

  // store all of our calculated quantities in the event
  event.put (std::move (uservariables), "uservariables");
}


// implementation left up to the daughter class
void
VariableProducer::AddVariables (const edm::Event &event) {}
