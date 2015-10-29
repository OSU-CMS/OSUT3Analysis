#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/PrimaryvertexProducer.h"

PrimaryvertexProducer::PrimaryvertexProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("primaryvertexs");

  produces<vector<TYPE(primaryvertexs)> > (collection_.instance ());
}

PrimaryvertexProducer::~PrimaryvertexProducer ()
{
}

void
PrimaryvertexProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(primaryvertexs)> > collection;
  anatools::getCollection (collection_, collection, event);

  pl_ = auto_ptr<vector<TYPE(primaryvertexs)> > (new vector<TYPE(primaryvertexs)> (*collection));

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}
