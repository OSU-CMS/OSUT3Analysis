#include "OSUT3Analysis/Collections/plugins/OSUSuperclusterProducer.h"

#if IS_VALID(superclusters)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUSuperclusterProducer::OSUSuperclusterProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("superclusters");

  produces<vector<osu::Supercluster> > (collection_.instance ());

  token_ = consumes<vector<TYPE(superclusters)> > (collection_);
}

OSUSuperclusterProducer::~OSUSuperclusterProducer ()
{
}

void
OSUSuperclusterProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(superclusters)> > collection;
  if (!event.getByToken (token_, collection))
    return;

  pl_ = unique_ptr<vector<osu::Supercluster> > (new vector<osu::Supercluster> ());
  for (const auto &object : *collection)
    pl_->emplace_back (object);

  event.put (std::move (pl_), collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUSuperclusterProducer);

#endif
