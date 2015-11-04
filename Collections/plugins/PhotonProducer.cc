#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/PhotonProducer.h"

PhotonProducer::PhotonProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("photons");

  produces<vector<osu::Photon> > (collection_.instance ());
}

PhotonProducer::~PhotonProducer ()
{
}

void
PhotonProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(photons)> > collection;
  bool valid = anatools::getCollection (collection_, collection, event);
  if(!valid)
    return;

  pl_ = auto_ptr<vector<osu::Photon> > (new vector<osu::Photon> ());
  for (const auto &object : *collection)
    {
      osu::Photon photon(object);
      pl_->push_back (photon);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PhotonProducer);
