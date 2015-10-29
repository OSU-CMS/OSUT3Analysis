#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/EventProducer.h"

EventProducer::EventProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("events");

  produces<vector<TYPE(events)> > (collection_.instance ());
}

EventProducer::~EventProducer ()
{
}

void
EventProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(events)> > collection;
  anatools::getCollection (collection_, collection, event);

  pl_ = auto_ptr<vector<TYPE(events)> > (new vector<TYPE(events)> (*collection));

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}
