#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/PileUpInfoProducer.h"

PileUpInfoProducer::PileUpInfoProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("pileupinfos");

  produces<vector<osu::PileUpInfo> > (collection_.instance ());
}

PileUpInfoProducer::~PileUpInfoProducer ()
{
}

void
PileUpInfoProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(pileupinfos)> > collection;
  bool valid = anatools::getCollection (collection_, collection, event, false);
  // Specify argument verbose = false to prevent error messages if collection is not found. 
  if(!valid)
    return;

  pl_ = auto_ptr<vector<osu::PileUpInfo> > (new vector<osu::PileUpInfo> ());
  for (const auto &object : *collection)
    {
      osu::PileUpInfo pileupinfo(object);
      pl_->push_back (pileupinfo);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PileUpInfoProducer);
