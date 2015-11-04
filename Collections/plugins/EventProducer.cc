#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/EventProducer.h"

EventProducer::EventProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("events");

  produces<vector<osu::Event> > (collection_.instance ());
}

EventProducer::~EventProducer ()
{
}

void
EventProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(events)> > collection;
  bool valid = anatools::getCollection (collection_, collection, event);
  if(!valid)
    return;

  pl_ = auto_ptr<vector<osu::Event> > (new vector<osu::Event> ());
  for (const auto &object : *collection)
    {
      osu::Event event(object);
      pl_->push_back (event);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(EventProducer);
