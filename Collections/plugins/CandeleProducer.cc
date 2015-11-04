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
  bool valid = anatools::getCollection (collection_, collection, event);
  if(!valid)
    return;
  pl_ = auto_ptr<vector<osu::Candele> > (new vector<osu::Candele> ());
  for (const auto &object : *collection)
    {
      osu::Candele candele(object); 
      pl_->push_back (candele);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(CandeleProducer);
