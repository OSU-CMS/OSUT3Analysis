#include "OSUT3Analysis/AnaTools/interface/EventVariableProducer.h"

EventVariableProducer::EventVariableProducer(const edm::ParameterSet &cfg) :
  collections_  (cfg.getParameter<edm::ParameterSet>  ("collections"))
{
  assert (strcmp (PROJECT_VERSION, SUPPORTED_VERSION) == 0);

  produces<EventVariableProducerPayload> ("eventvariables");
}

EventVariableProducer::~EventVariableProducer()
{
}

void
EventVariableProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  // define structure that will be put into the event
  // string is the variable name
  // double is the value of the variable
  // it is a vector because  other collections are also vectors
  // the vector will have size=1 for each event

  eventvariables = auto_ptr<EventVariableProducerPayload> (new EventVariableProducerPayload);

  ////////////////////////////////////////////////////////////////////////
  AddVariables(event);

  // store all of our calculated quantities in the event
  event.put (eventvariables, "eventvariables");
  eventvariables.reset ();
}

// implementation left up to the daughter class
void
EventVariableProducer::AddVariables (const edm::Event &event) {}
