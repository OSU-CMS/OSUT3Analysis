#include "OSUT3Analysis/Collections/plugins/OSUGenjetProducer.h"

#if IS_VALID(genjets)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUGenjetProducer::OSUGenjetProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_ (cfg)
{
  collection_ = collections_.getParameter<edm::InputTag> ("genjets");

  produces<vector<osu::Genjet> > (collection_.instance ());

  token_ = consumes<vector<TYPE(genjets)> > (collection_);
  mcparticleToken_ = consumes<vector<osu::Mcparticle> > (edm::InputTag ());
}

OSUGenjetProducer::~OSUGenjetProducer ()
{
}

void
OSUGenjetProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(genjets)> > collection;
  if (!event.getByToken (token_, collection))
    return;
  edm::Handle<vector<osu::Mcparticle> > particles;
  event.getByToken (mcparticleToken_, particles);

  pl_ = auto_ptr<vector<osu::Genjet> > (new vector<osu::Genjet> ());
  for (const auto &object : *collection)
    {
      const osu::Genjet genjet (object, particles, cfg_);
      pl_->push_back (genjet);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUGenjetProducer);

#endif
