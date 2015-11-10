#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/EventvariableProducer.h"

EventvariableProducer::EventvariableProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("eventvariables");

  produces<osu::Eventvariable> (collection_.instance ());
}

EventvariableProducer::~EventvariableProducer ()
{
}

void
EventvariableProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<TYPE(eventvariables)> collection;
  bool valid = anatools::getCollection (collection_, collection, event, false);
  // Specify argument verbose = false to prevent error messages if collection is not found. 
  if(!valid)
    return;

  pl_ = auto_ptr<osu::Eventvariable> (new osu::Eventvariable ());
  osu::Eventvariable eventvariable(*collection); 
  *pl_ = eventvariable;
  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(EventvariableProducer);
