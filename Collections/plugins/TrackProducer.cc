#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/TrackProducer.h"

TrackProducer::TrackProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("tracks");

  produces<vector<TYPE(tracks)> > (collection_.instance ());
}

TrackProducer::~TrackProducer ()
{
}

void
TrackProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(tracks)> > collection;
  anatools::getCollection (collection_, collection, event);

  pl_ = auto_ptr<vector<TYPE(tracks)> > (new vector<TYPE(tracks)> (*collection));

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}
