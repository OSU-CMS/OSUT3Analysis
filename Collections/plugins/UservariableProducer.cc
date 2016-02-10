#include "OSUT3Analysis/Collections/plugins/UservariableProducer.h"

#if IS_VALID(uservariables)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

UservariableProducer::UservariableProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("uservariables");

  produces<vector<osu::Uservariable> > (collection_.instance ());

  token_ = consumes<vector<TYPE(uservariables)> > (collection_);
}

UservariableProducer::~UservariableProducer ()
{
}

void
UservariableProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(uservariables)> > collection;
  if (!event.getByToken (token_, collection))
    return;

  pl_ = auto_ptr<vector<osu::Uservariable> > (new vector<osu::Uservariable> ());
  for (const auto &object : *collection)
    {
      const osu::Uservariable uservariable (object);
      pl_->push_back (uservariable);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(UservariableProducer);

#endif
