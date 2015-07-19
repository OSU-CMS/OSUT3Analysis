#include "OSUT3Analysis/AnaTools/interface/VariableProducer.h"

VariableProducer::VariableProducer(const edm::ParameterSet &cfg) :
  collections_  (cfg.getParameter<edm::ParameterSet>  ("collections"))
{
  assert (strcmp (PROJECT_VERSION, SUPPORTED_VERSION) == 0);

  produces<VariableProducerPayload> ("uservariables");
}

VariableProducer::~VariableProducer()
{
}

void
VariableProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{

  uservariables = auto_ptr<VariableProducerPayload> (new VariableProducerPayload);

  ////////////////////////////////////////////////////////////////////////

  AddVariables(event);

  // store all of our calculated quantities in the event
  event.put (uservariables, "uservariables");
}


// implementation left up to the daughter class
void
VariableProducer::AddVariables (const edm::Event &event) {}
