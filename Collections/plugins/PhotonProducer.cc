#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/PhotonProducer.h"

PhotonProducer::PhotonProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("photons");

  produces<vector<TYPE(photons)> > (collection_.instance ());
}

PhotonProducer::~PhotonProducer ()
{
}

void
PhotonProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(photons)> > collection;
  anatools::getCollection (collection_, collection, event);

  pl_ = auto_ptr<vector<TYPE(photons)> > (new vector<TYPE(photons)> (*collection));

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}
