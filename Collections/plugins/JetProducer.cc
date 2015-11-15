#include "OSUT3Analysis/Collections/plugins/JetProducer.h"

#if IS_VALID(jets)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

JetProducer::JetProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_ (cfg)
{
  collection_ = collections_.getParameter<edm::InputTag> ("jets");

  produces<vector<osu::Jet> > (collection_.instance ());
}

JetProducer::~JetProducer ()
{
}

void
JetProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE (jets)> > collection;
  if (!anatools::getCollection (collection_, collection, event, false))
    return;
  edm::Handle<vector<osu::Mcparticle> > particles;
  anatools::getCollection (edm::InputTag ("", ""), particles, event);

  pl_ = auto_ptr<vector<osu::Jet> > (new vector<osu::Jet> ());
  for (const auto &object : *collection)
    {
      const osu::Jet jet (object, particles, cfg_);
      pl_->push_back (jet);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(JetProducer);

#endif
