#include "OSUT3Analysis/Collections/plugins/TauProducer.h"

#if IS_VALID(taus)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

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
  edm::Handle<vector<TYPE (taus)> > collection;
  if (!anatools::getCollection (collection_, collection, event, false))
    return;
  edm::Handle<vector<osu::Mcparticle> > particles;
  anatools::getCollection (edm::InputTag ("", ""), particles, event);

  pl_ = auto_ptr<vector<osu::Tau> > (new vector<osu::Tau> ());
  for (const auto &object : *collection)
    {
      const osu::Tau tau (object, particles);
      pl_->push_back (tau);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TauProducer);

#endif
