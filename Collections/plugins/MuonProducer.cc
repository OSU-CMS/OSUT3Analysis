#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/MuonProducer.h"

MuonProducer::MuonProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("muons");

  produces<vector<TYPE(muons)> > (collection_.instance ());
}

MuonProducer::~MuonProducer ()
{
}

void
MuonProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(muons)> > collection;
  anatools::getCollection (collection_, collection, event);

  pl_ = auto_ptr<vector<TYPE(muons)> > (new vector<TYPE(muons)> (*collection));

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}
