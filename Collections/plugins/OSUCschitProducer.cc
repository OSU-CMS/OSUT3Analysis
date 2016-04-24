#include "OSUT3Analysis/Collections/plugins/OSUCschitProducer.h"

#if IS_VALID(cschits)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUCschitProducer::OSUCschitProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("cschits");

  produces<vector<osu::Cschit> > (collection_.instance ());
}

OSUCschitProducer::~OSUCschitProducer ()
{
}

void
OSUCschitProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE (cschits)> > collection;
  if (!event.getByToken (token_, collection))
    return;

  pl_ = auto_ptr<vector<osu::Cschit> > (new vector<osu::Cschit> ());
  for (const auto &object : *collection)
    {
      const osu::Cschit cschit (object);
      pl_->push_back (cschit);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUCschitProducer);

#endif
