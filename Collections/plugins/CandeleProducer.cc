#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/CandeleProducer.h"

CandeleProducer::CandeleProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("candeles");

  produces<vector<TYPE(candeles)> > (collection_.instance ());
}

CandeleProducer::~CandeleProducer ()
{
}

void
CandeleProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(candeles)> > collection;
  anatools::getCollection (collection_, collection, event);

  pl_ = auto_ptr<vector<TYPE(candeles)> > (new vector<TYPE(candeles)> (*collection));

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}
