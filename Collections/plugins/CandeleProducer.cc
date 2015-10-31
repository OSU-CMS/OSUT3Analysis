#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/CandeleProducer.h"

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
CandeleProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(candeles)> > collection;
  anatools::getCollection (collection_, collection, event);

  pl_ = auto_ptr<vector<osu::Candele> > (new vector<osu::Candele> ());
  for (const auto &object : *collection)
    {
      const osu::Candele * const candele = new osu::Candele (object);
      pl_->push_back (*candele);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(CandeleProducer);
