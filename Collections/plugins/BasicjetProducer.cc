#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/BasicjetProducer.h"

BasicjetProducer::BasicjetProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("basicjets");

  produces<vector<TYPE(basicjets)> > (collection_.instance ());
}

BasicjetProducer::~BasicjetProducer ()
{
}

void
BasicjetProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(basicjets)> > collection;
  anatools::getCollection (collection_, collection, event);

  pl_ = auto_ptr<vector<TYPE(basicjets)> > (new vector<TYPE(basicjets)> (*collection));

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}
