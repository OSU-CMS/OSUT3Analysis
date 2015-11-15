#include "OSUT3Analysis/Collections/plugins/TrigobjProducer.h"

#if IS_VALID(trigobjs)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

TrigobjProducer::TrigobjProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_ (cfg)
{
  collection_ = collections_.getParameter<edm::InputTag> ("trigobjs");

  produces<vector<osu::Trigobj> > (collection_.instance ());
}

TrigobjProducer::~TrigobjProducer ()
{
}

void
TrigobjProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE (trigobjs)> > collection;
  if (!anatools::getCollection (collection_, collection, event, false))
    return;
  edm::Handle<vector<osu::Mcparticle> > particles;
  anatools::getCollection (edm::InputTag ("", ""), particles, event);

  pl_ = auto_ptr<vector<osu::Trigobj> > (new vector<osu::Trigobj> ());
  for (const auto &object : *collection)
    {
      const osu::Trigobj trigobj (object, particles, cfg_);
      pl_->push_back (trigobj);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TrigobjProducer);

#endif
