#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/BxlumiProducer.h"

BxlumiProducer::BxlumiProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("bxlumis");

  produces<vector<osu::Bxlumi> > (collection_.instance ());
}

BxlumiProducer::~BxlumiProducer ()
{
}

void
BxlumiProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(bxlumis)> > collection;
  anatools::getCollection (collection_, collection, event);

  pl_ = auto_ptr<vector<osu::Bxlumi> > (new vector<osu::Bxlumi> ());
  for (const auto &object : *collection)
    {
      osu::Bxlumi bxlumi(object);
      pl_->push_back (bxlumi);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(BxlumiProducer);
