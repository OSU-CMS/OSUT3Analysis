#include "OSUT3Analysis/Collections/plugins/OSUTauProducer.h"

#if IS_VALID(taus)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUTauProducer::OSUTauProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_ (cfg)
{
  collection_ = collections_.getParameter<edm::InputTag> ("taus");

  produces<vector<osu::Tau> > (collection_.instance ());

  token_ = consumes<vector<TYPE(taus)> > (collection_);
  mcparticleToken_ = consumes<vector<osu::Mcparticle> > (edm::InputTag ());
}

OSUTauProducer::~OSUTauProducer ()
{
}

void
OSUTauProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(taus)> > collection;
  if (!event.getByToken (token_, collection))
    return;
  edm::Handle<vector<osu::Mcparticle> > particles;
  event.getByToken (mcparticleToken_, particles);

  pl_ = auto_ptr<vector<osu::Tau> > (new vector<osu::Tau> ());
  for (const auto &object : *collection)
    {
      const osu::Tau tau (object, particles, cfg_);
      pl_->push_back (tau);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUTauProducer);

#endif
