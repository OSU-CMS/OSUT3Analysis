#include "OSUT3Analysis/Collections/plugins/OSUPrimaryvertexProducer.h"

#if IS_VALID(primaryvertexs)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUPrimaryvertexProducer::OSUPrimaryvertexProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("primaryvertexs");

  produces<vector<osu::Primaryvertex> > (collection_.instance ());

  token_ = consumes<vector<TYPE(primaryvertexs)> > (collection_);
}

OSUPrimaryvertexProducer::~OSUPrimaryvertexProducer ()
{
}

void
OSUPrimaryvertexProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(primaryvertexs)> > collection;
  if (!event.getByToken (token_, collection))
    return;

  pl_ = auto_ptr<vector<osu::Primaryvertex> > (new vector<osu::Primaryvertex> ());
  for (const auto &object : *collection)
    pl_->emplace_back (object);

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUPrimaryvertexProducer);

#endif
