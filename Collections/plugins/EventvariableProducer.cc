#include "OSUT3Analysis/Collections/plugins/EventvariableProducer.h"

#if IS_VALID(eventvariables)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

EventvariableProducer::EventvariableProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  std::vector<edm::InputTag> eventvariablesVector = collections_.getParameter<std::vector<edm::InputTag>> ("eventvariables");
  collection_ = eventvariablesVector[0];

  produces<osu::Eventvariable> (collection_.instance ());

  token_ = consumes<TYPE(eventvariables)> (collection_);
}

EventvariableProducer::~EventvariableProducer ()
{
}

void
EventvariableProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<TYPE(eventvariables)> collection;
  if (!event.getByToken (token_, collection))
    return;

  pl_ = unique_ptr<osu::Eventvariable> (new osu::Eventvariable (*collection));
  event.put (std::move (pl_), collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(EventvariableProducer);

#endif
