#include "OSUT3Analysis/Collections/plugins/OSUBxlumiProducer.h"

#if IS_VALID(bxlumis)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUBxlumiProducer::OSUBxlumiProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("bxlumis");

  produces<vector<osu::Bxlumi> > (collection_.instance ());

  token_ = consumes<vector<TYPE(bxlumis)> > (collection_);
}

OSUBxlumiProducer::~OSUBxlumiProducer ()
{
}

void
OSUBxlumiProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(bxlumis)> > collection;
  if (!event.getByToken (token_, collection))
    return;

  pl_ = auto_ptr<vector<osu::Bxlumi> > (new vector<osu::Bxlumi> ());
  for (const auto &object : *collection)
    pl_->emplace_back (object);

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUBxlumiProducer);

#endif
