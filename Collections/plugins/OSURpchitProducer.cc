#include "OSUT3Analysis/Collections/plugins/OSURpchitProducer.h"

#if IS_VALID(rpchits)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSURpchitProducer::OSURpchitProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("rpchits");

  produces<vector<osu::Rpchit> > (collection_.instance ());

  token_ = consumes<vector<TYPE(rpchits)> > (collection_);
}

OSURpchitProducer::~OSURpchitProducer ()
{
}

void
OSURpchitProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE (rpchits)> > collection;
  if (!event.getByToken (token_, collection))
    return;

  pl_ = unique_ptr<vector<osu::Rpchit> > (new vector<osu::Rpchit> ());
  for (const auto &object : *collection)
    pl_->emplace_back (object);

  event.put (std::move (pl_), collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSURpchitProducer);

#endif
