#include "OSUT3Analysis/Collections/plugins/BjetProducer.h"

#if IS_VALID(bjets)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

BjetProducer::BjetProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_ (cfg)
{
  collection_ = collections_.getParameter<edm::InputTag> ("bjets");

  produces<vector<osu::Bjet> > (collection_.instance ());
}

BjetProducer::~BjetProducer ()
{
}

void
BjetProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE (bjets)> > collection;
  if (!anatools::getCollection (collection_, collection, event, false))
    return;
  edm::Handle<vector<osu::Mcparticle> > particles;
  anatools::getCollection (edm::InputTag ("", ""), particles, event);

  pl_ = auto_ptr<vector<osu::Bjet> > (new vector<osu::Bjet> ());
  for (const auto &object : *collection)
    {
      const osu::Bjet bjet (object, particles, cfg_);
      pl_->push_back (bjet);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(BjetProducer);

#endif
