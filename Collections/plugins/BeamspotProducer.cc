#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/BeamspotProducer.h"

BeamspotProducer::BeamspotProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("beamspots");

  produces<vector<osu::Beamspot> > (collection_.instance ());
}

BeamspotProducer::~BeamspotProducer ()
{
}

void
BeamspotProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(beamspots)> > collection;
  anatools::getCollection (collection_, collection, event);

  pl_ = auto_ptr<vector<osu::Beamspot> > (new vector<osu::Beamspot> ());
  for (const auto &object : *collection)
    {
      const osu::Beamspot * const beamspot = new osu::Beamspot (object);
      pl_->push_back (*beamspot);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(BeamspotProducer);
