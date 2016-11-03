#include "OSUT3Analysis/Collections/plugins/OSUDtsegProducer.h"

#if IS_VALID(dtsegs)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUDtsegProducer::OSUDtsegProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("dtsegs");

  produces<vector<osu::Dtseg> > (collection_.instance ());
  std::cout << collection_.instance() << std::endl;
  token_ = consumes<vector<TYPE(dtsegs)> > (collection_);
}

OSUDtsegProducer::~OSUDtsegProducer ()
{
}

void
OSUDtsegProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE (dtsegs)> > collection;
  if (!event.getByToken (token_, collection))
    return;

  pl_ = unique_ptr<vector<osu::Dtseg> > (new vector<osu::Dtseg> ());
  for (const auto &object : *collection)
    pl_->emplace_back (object);

  event.put (std::move (pl_), collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUDtsegProducer);

#endif
