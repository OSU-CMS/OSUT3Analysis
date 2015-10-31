#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/TauProducer.h"

TauProducer::TauProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("taus");

  produces<vector<osu::Tau> > (collection_.instance ());
}

TauProducer::~TauProducer ()
{
}

void
TauProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(taus)> > collection;
  anatools::getCollection (collection_, collection, event);

  pl_ = auto_ptr<vector<osu::Tau> > (new vector<osu::Tau> ());
  for (const auto &object : *collection)
    {
      const osu::Tau * const tau = new osu::Tau (object);
      pl_->push_back (*tau);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TauProducer);
