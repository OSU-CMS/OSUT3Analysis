#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/GenjetProducer.h"

GenjetProducer::GenjetProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("genjets");

  produces<vector<TYPE(genjets)> > (collection_.instance ());
}

GenjetProducer::~GenjetProducer ()
{
}

void
GenjetProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(genjets)> > collection;
  anatools::getCollection (collection_, collection, event);

  pl_ = auto_ptr<vector<TYPE(genjets)> > (new vector<TYPE(genjets)> (*collection));

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}
