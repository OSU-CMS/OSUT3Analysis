#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/BeamspotProducer.h"

BeamspotProducer::BeamspotProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("beamspots");

  produces<vector<TYPE(beamspots)> > (collection_.instance ());
}

BeamspotProducer::~BeamspotProducer ()
{
}

void
BeamspotProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(beamspots)> > collection;
  anatools::getCollection (collection_, collection, event);

  pl_ = auto_ptr<vector<TYPE(beamspots)> > (new vector<TYPE(beamspots)> (*collection));

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}
