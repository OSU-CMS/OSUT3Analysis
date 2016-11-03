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
  mcparticleToken_ = consumes<vector<osu::Mcparticle> > (collections_.getParameter<edm::InputTag> ("mcparticles"));
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

  pl_ = unique_ptr<vector<osu::Genjet> > (new vector<osu::Genjet> ());
  for (const auto &object : *collection)
    pl_->emplace_back (object, particles, cfg_);

  event.put (std::move (pl_), collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUGenjetProducer);

#endif
