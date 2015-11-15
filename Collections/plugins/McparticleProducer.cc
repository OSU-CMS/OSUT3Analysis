#include "OSUT3Analysis/Collections/plugins/McparticleProducer.h"

#if IS_VALID(mcparticles)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

McparticleProducer::McparticleProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("mcparticles");

  produces<vector<osu::Mcparticle> > (collection_.instance ());
}

McparticleProducer::~McparticleProducer ()
{
}

void
McparticleProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE (mcparticles)> > collection;
  if (!anatools::getCollection (collection_, collection, event, false))
    return;

  pl_ = auto_ptr<vector<osu::Mcparticle> > (new vector<osu::Mcparticle> ());
  for (const auto &object : *collection)
    {
      const osu::Mcparticle mcparticle (object);
      pl_->push_back (mcparticle);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(McparticleProducer);

#endif
