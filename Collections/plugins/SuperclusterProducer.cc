#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/SuperclusterProducer.h"

SuperclusterProducer::SuperclusterProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("superclusters");

  produces<vector<osu::Supercluster> > (collection_.instance ());
}

SuperclusterProducer::~SuperclusterProducer ()
{
}

void
SuperclusterProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(superclusters)> > collection;
  bool valid = anatools::getCollection (collection_, collection, event);
  if(!valid)
    return;

  pl_ = auto_ptr<vector<osu::Supercluster> > (new vector<osu::Supercluster> ());
  for (const auto &object : *collection)
    {
      osu::Supercluster supercluster(object);
      pl_->push_back (supercluster);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(SuperclusterProducer);
