#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/GenjetProducer.h"

GenjetProducer::GenjetProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("genjets");

  produces<vector<osu::Genjet> > (collection_.instance ());
}

GenjetProducer::~GenjetProducer ()
{
}

void
GenjetProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(genjets)> > collection;
  anatools::getCollection (collection_, collection, event);

  pl_ = auto_ptr<vector<osu::Genjet> > (new vector<osu::Genjet> ());
  for (const auto &object : *collection)
    {
      osu::Genjet genjet(object);
      pl_->push_back (genjet);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(GenjetProducer);
