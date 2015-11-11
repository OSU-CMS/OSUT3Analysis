#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/CandjetProducer.h"

CandjetProducer::CandjetProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("candjets");

  produces<vector<osu::Candjet> > (collection_.instance ());
}

CandjetProducer::~CandjetProducer ()
{
}

void
CandjetProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(candjets)> > collection;
  bool valid = anatools::getCollection (collection_, collection, event, false);
  // Specify argument verbose = false to prevent error messages if collection is not found. 
  if(!valid)
    return;
  pl_ = auto_ptr<vector<osu::Candjet> > (new vector<osu::Candjet> ());
  for (const auto &object : *collection)
    {
      osu::Candjet candjet(object);
      pl_->push_back (candjet);
    }
  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(CandjetProducer);
