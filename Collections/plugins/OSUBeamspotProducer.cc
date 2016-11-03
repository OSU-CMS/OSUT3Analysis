#include "OSUT3Analysis/Collections/plugins/OSUBeamspotProducer.h"

#if IS_VALID(beamspots)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUBeamspotProducer::OSUBeamspotProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("beamspots");

  produces<osu::Beamspot> (collection_.instance ());

  token_ = consumes<TYPE(beamspots)> (collection_);
}

OSUBeamspotProducer::~OSUBeamspotProducer ()
{
}

void
OSUBeamspotProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<TYPE(beamspots)> collection;
  if (!event.getByToken (token_, collection))
    return;
  pl_ = unique_ptr<osu::Beamspot>  (new osu::Beamspot (*collection));
  event.put (std::move (pl_), collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUBeamspotProducer);

#endif
