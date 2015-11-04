#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/TrackProducer.h"

TrackProducer::TrackProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("tracks");

  produces<vector<osu::Track> > (collection_.instance ());
}

TrackProducer::~TrackProducer ()
{
}

void
TrackProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(tracks)> > collection;
  bool valid = anatools::getCollection (collection_, collection, event);
  if(!valid)
    return;

  pl_ = auto_ptr<vector<osu::Track> > (new vector<osu::Track> ());
  for (const auto &object : *collection)
    {
      osu::Track track(object);
      pl_->push_back (track);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TrackProducer);
