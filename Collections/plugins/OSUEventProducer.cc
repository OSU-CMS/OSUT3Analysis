#include "OSUT3Analysis/Collections/plugins/OSUEventProducer.h"

#if IS_VALID(events)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUEventProducer::OSUEventProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("events");

  produces<vector<osu::Event> > (collection_.instance ());

  token_ = consumes<vector<TYPE(events)> > (collection_);
}

OSUEventProducer::~OSUEventProducer ()
{
}

void
OSUEventProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(events)> > collection;
  if (!event.getByToken (token_, collection))
    return;

  pl_ = auto_ptr<vector<osu::Event> > (new vector<osu::Event> ());
  for (const auto &object : *collection)
    {
      const osu::Event event (object);
      pl_->push_back (event);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUEventProducer);

#endif
