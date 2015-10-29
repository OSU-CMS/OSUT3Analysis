#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/SuperclusterProducer.h"

SuperclusterProducer::SuperclusterProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("superclusters");

  produces<vector<TYPE(superclusters)> > (collection_.instance ());
}

SuperclusterProducer::~SuperclusterProducer ()
{
}

void
SuperclusterProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(superclusters)> > collection;
  anatools::getCollection (collection_, collection, event);

  pl_ = auto_ptr<vector<TYPE(superclusters)> > (new vector<TYPE(superclusters)> (*collection));

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}
