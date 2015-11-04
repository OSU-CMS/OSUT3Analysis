#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/TrigobjProducer.h"

TrigobjProducer::TrigobjProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
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
  edm::Handle<vector<TYPE(trigobjs)> > collection;
  anatools::getCollection (collection_, collection, event);

  pl_ = auto_ptr<vector<osu::Trigobj> > (new vector<osu::Trigobj> ());
  for (const auto &object : *collection)
    {
      osu::Trigobj trigobj(object);
      pl_->push_back (trigobj);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TrigobjProducer);
