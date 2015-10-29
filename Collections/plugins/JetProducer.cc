#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/JetProducer.h"

JetProducer::JetProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("jets");

  produces<vector<TYPE(jets)> > (collection_.instance ());
}

JetProducer::~JetProducer ()
{
}

void
JetProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(jets)> > collection;
  anatools::getCollection (collection_, collection, event);

  pl_ = auto_ptr<vector<TYPE(jets)> > (new vector<TYPE(jets)> (*collection));

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}
