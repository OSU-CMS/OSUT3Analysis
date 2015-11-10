#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/JetProducer.h"

JetProducer::JetProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("jets");

  produces<vector<osu::Jet> > (collection_.instance ());
}

JetProducer::~JetProducer ()
{
}

void
JetProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(jets)> > collection;
  bool valid = anatools::getCollection (collection_, collection, event, false);
  // Specify argument verbose = false to prevent error messages if collection is not found. 
  if(!valid)
    return;

  pl_ = auto_ptr<vector<osu::Jet> > (new vector<osu::Jet> ());
  for (const auto &object : *collection)
    {
      osu::Jet jet(object);
      pl_->push_back (jet);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(JetProducer);
