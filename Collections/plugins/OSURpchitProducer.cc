#include "OSUT3Analysis/Collections/plugins/OSURpchitProducer.h"

#if IS_VALID(rpchits)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSURpchitProducer::OSURpchitProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("rpchits");

  produces<vector<osu::Rpchit> > (collection_.instance ());
}

OSURpchitProducer::~OSURpchitProducer ()
{
}

void
OSURpchitProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE (rpchits)> > collection;
  if (!anatools::getCollection (collection_, collection, event, false))
    return;

  pl_ = auto_ptr<vector<osu::Rpchit> > (new vector<osu::Rpchit> ());
  for (const auto &object : *collection)
    {
      const osu::Rpchit rpchit (object);
      pl_->push_back (rpchit);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSURpchitProducer);

#endif
