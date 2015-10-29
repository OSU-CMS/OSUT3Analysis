#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/BxlumiProducer.h"

BxlumiProducer::BxlumiProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("bxlumis");

  produces<vector<TYPE(bxlumis)> > (collection_.instance ());
}

BxlumiProducer::~BxlumiProducer ()
{
}

void
BxlumiProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(bxlumis)> > collection;
  anatools::getCollection (collection_, collection, event);

  pl_ = auto_ptr<vector<TYPE(bxlumis)> > (new vector<TYPE(bxlumis)> (*collection));

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}
