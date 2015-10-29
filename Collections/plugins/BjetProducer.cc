#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/BjetProducer.h"

BjetProducer::BjetProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("bjets");

  produces<vector<TYPE(bjets)> > (collection_.instance ());
}

BjetProducer::~BjetProducer ()
{
}

void
BjetProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(bjets)> > collection;
  anatools::getCollection (collection_, collection, event);

  pl_ = auto_ptr<vector<TYPE(bjets)> > (new vector<TYPE(bjets)> (*collection));

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}
