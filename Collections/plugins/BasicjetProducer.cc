#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/BasicjetProducer.h"

BasicjetProducer::BasicjetProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
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
  edm::Handle<vector<TYPE(basicjets)> > collection;
  anatools::getCollection (collection_, collection, event);

  pl_ = auto_ptr<vector<osu::Basicjet> > (new vector<osu::Basicjet> ());
  for (const auto &object : *collection)
    {
      const osu::Basicjet * const basicjet = new osu::Basicjet (object);
      pl_->push_back (*basicjet);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(BasicjetProducer);
