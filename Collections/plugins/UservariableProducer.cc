#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/UservariableProducer.h"

UservariableProducer::UservariableProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("uservariables");

  produces<vector<osu::Uservariable> > (collection_.instance ());
}

UservariableProducer::~UservariableProducer ()
{
}

void
UservariableProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(uservariables)> > collection;
  bool valid = anatools::getCollection (collection_, collection, event, false);
  // Specify argument verbose = false to prevent error messages if collection is not found. 
  if(!valid)
    return;

  pl_ = auto_ptr<vector<osu::Uservariable> > (new vector<osu::Uservariable> ());
  for (const auto &object : *collection)
    {
      osu::Uservariable uservariable(object);
      pl_->push_back (uservariable);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(UservariableProducer);
