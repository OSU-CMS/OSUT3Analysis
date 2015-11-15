#include "OSUT3Analysis/Collections/plugins/MetProducer.h"

#if IS_VALID(mets)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

MetProducer::MetProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("mets");

  produces<vector<osu::Met> > (collection_.instance ());
}

MetProducer::~MetProducer ()
{
}

void
MetProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE (mets)> > collection;
  if (!anatools::getCollection (collection_, collection, event, false))
    return;

  pl_ = auto_ptr<vector<osu::Met> > (new vector<osu::Met> ());
  for (const auto &object : *collection)
    {
      const osu::Met met (object);
      pl_->push_back (met);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(MetProducer);

#endif
