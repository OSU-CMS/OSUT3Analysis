#include "OSUT3Analysis/Collections/plugins/BasicjetProducer.h"

#if IS_VALID(basicjets)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

BasicjetProducer::BasicjetProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_ (cfg)
{
  collection_ = collections_.getParameter<edm::InputTag> ("basicjets");

  produces<vector<osu::Basicjet> > (collection_.instance ());
}

BasicjetProducer::~BasicjetProducer ()
{
}

void
BasicjetProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE (basicjets)> > collection;
  if (!anatools::getCollection (collection_, collection, event, false))
    return;
  edm::Handle<vector<osu::Mcparticle> > particles;
  anatools::getCollection (edm::InputTag ("", ""), particles, event);

  pl_ = auto_ptr<vector<osu::Basicjet> > (new vector<osu::Basicjet> ());
  for (const auto &object : *collection)
    {
      const osu::Basicjet basicjet (object, particles, cfg_);
      pl_->push_back (basicjet);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(BasicjetProducer);

#endif
