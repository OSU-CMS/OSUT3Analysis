#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/McparticleProducer.h"

McparticleProducer::McparticleProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("mcparticles");

  produces<vector<TYPE(mcparticles)> > (collection_.instance ());
}

McparticleProducer::~McparticleProducer ()
{
}

void
McparticleProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(mcparticles)> > collection;
  anatools::getCollection (collection_, collection, event);

  pl_ = auto_ptr<vector<TYPE(mcparticles)> > (new vector<TYPE(mcparticles)> (*collection));

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}
