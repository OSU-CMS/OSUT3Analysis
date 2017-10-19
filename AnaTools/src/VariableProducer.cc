#include "OSUT3Analysis/AnaTools/interface/VariableProducer.h"

VariableProducer::VariableProducer(const edm::ParameterSet &cfg) :
  collections_  (cfg.getParameter<edm::ParameterSet>  ("collections"))
{
  produces<VariableProducerPayload> ("uservariables");
}

VariableProducer::~VariableProducer()
{
}

bool
VariableProducer::filter (edm::Event &event, const edm::EventSetup &setup)
{

  uservariables = unique_ptr<VariableProducerPayload> (new VariableProducerPayload);

  ////////////////////////////////////////////////////////////////////////

  AddVariables(event);

  bool filterDecision = true;

  // store all of our calculated quantities in the event
  event.put (std::move (uservariables), "uservariables");

  return filterDecision;
}


// implementation left up to the daughter class
void
VariableProducer::AddVariables (const edm::Event &event) {}
