#include "OSUT3Analysis/AnaTools/interface/EventVariableProducer.h"

EventVariableProducer::EventVariableProducer(const edm::ParameterSet &cfg) :
  collections_  (cfg.getParameter<edm::ParameterSet>  ("collections"))
{
  produces<EventVariableProducerPayload> ("eventvariables");
}

EventVariableProducer::~EventVariableProducer()
{
}

bool
EventVariableProducer::filter (edm::Event &event, const edm::EventSetup &setup)
{
  // define structure that will be put into the event
  // string is the variable name
  // double is the value of the variable
  // it is a vector because  other collections are also vectors
  // the vector will have size=1 for each event

  eventvariables = unique_ptr<EventVariableProducerPayload> (new EventVariableProducerPayload);

  ////////////////////////////////////////////////////////////////////////
  AddVariables(event);

  bool filterDecision = true;
  if (eventvariables->count ("EventVariableProducerFilterDecision"))
    filterDecision = eventvariables->at ("EventVariableProducerFilterDecision");

  // store all of our calculated quantities in the event
  event.put (std::move (eventvariables), "eventvariables");
  eventvariables.reset ();

  return filterDecision;
}
