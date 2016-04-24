#include "OSUT3Analysis/Collections/plugins/OSUPhotonProducer.h"

#if IS_VALID(photons)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUPhotonProducer::OSUPhotonProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_ (cfg)
{
  collection_ = collections_.getParameter<edm::InputTag> ("photons");

  produces<vector<osu::Photon> > (collection_.instance ());

  token_ = consumes<vector<TYPE(photons)> > (collection_);
  mcparticleToken_ = consumes<vector<osu::Mcparticle> > (collections_.getParameter<edm::InputTag> ("mcparticles"));
}

OSUPhotonProducer::~OSUPhotonProducer ()
{
}

void
OSUPhotonProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(photons)> > collection;
  if (!event.getByToken (token_, collection))
    return;
  edm::Handle<vector<osu::Mcparticle> > particles;
  event.getByToken (mcparticleToken_, particles);

  pl_ = auto_ptr<vector<osu::Photon> > (new vector<osu::Photon> ());
  for (const auto &object : *collection)
    {
      const osu::Photon photon (object, particles, cfg_);
      pl_->push_back (photon);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUPhotonProducer);

#endif
