#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/PileUpInfoProducer.h"

PileUpInfoProducer::PileUpInfoProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("pileupinfos");

  produces<vector<osu::PileUpInfo> > (collection_.instance ());

  token_ = consumes<vector<TYPE(pileupinfos)> > (collection_);
}

PileUpInfoProducer::~PileUpInfoProducer ()
{
}

void
PileUpInfoProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(pileupinfos)> > collection;
  if (!event.getByToken (token_, collection))
    return;

  pl_ = unique_ptr<vector<osu::PileUpInfo> > (new vector<osu::PileUpInfo> ());
  for (const auto &object : *collection)
    pl_->emplace_back (object);

  event.put (std::move (pl_), collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PileUpInfoProducer);
