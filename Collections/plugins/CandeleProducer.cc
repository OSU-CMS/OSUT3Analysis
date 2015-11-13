#include "OSUT3Analysis/Collections/plugins/CandeleProducer.h"

#if IS_VALID(candeles)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

CandeleProducer::CandeleProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("candeles");

  produces<vector<osu::Candele> > (collection_.instance ());
}

CandeleProducer::~CandeleProducer ()
{
}

void
CandeleProducer::produce (edm::Event &event,  const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE (candeles)> > collection;
  if (!anatools::getCollection (collection_,  collection,  event))
    return;
  edm::Handle<vector<osu::Mcparticle> > particles;
  anatools::getCollection (edm::InputTag ("",  ""),  particles,  event);

  pl_ = auto_ptr<vector<osu::Candele> > (new vector<osu::Candele> ());
  for (const auto &object : *collection)
    {
      const osu::Candele candele (object,  particles);
      pl_->push_back (candele);
    }

  event.put (pl_,  collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(CandeleProducer);

#endif
