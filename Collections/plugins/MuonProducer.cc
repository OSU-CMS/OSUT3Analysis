#include "OSUT3Analysis/Collections/plugins/MuonProducer.h"

#if IS_VALID(muons)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

MuonProducer::MuonProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("muons");

  produces<vector<osu::Muon> > (collection_.instance ());
}

MuonProducer::~MuonProducer ()
{
}

void
MuonProducer::produce (edm::Event &event,  const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE (muons)> > collection;
  if (!anatools::getCollection (collection_,  collection,  event))
    return;
  edm::Handle<vector<osu::Mcparticle> > particles;
  anatools::getCollection (edm::InputTag ("",  ""),  particles,  event);

  pl_ = auto_ptr<vector<osu::Muon> > (new vector<osu::Muon> ());
  for (const auto &object : *collection)
    {
      const osu::Muon muon (object, particles);
      pl_->push_back (muon);
    }

  event.put (pl_,  collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(MuonProducer);

#endif
