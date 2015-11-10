#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#include "OSUT3Analysis/Collections/plugins/MuonProducer.h"

MuonProducer::MuonProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_         = collections_.getParameter<edm::InputTag> ("muons");
  collPrimaryvertexs_ = collections_.getParameter<edm::InputTag> ("primaryvertexs");

  produces<vector<osu::Muon> > (collection_.instance ());
}

MuonProducer::~MuonProducer ()
{
}

void
MuonProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(muons)> > collection;
  edm::Handle<vector<TYPE(primaryvertexs)> > collPrimaryvertexs;
  bool valid = anatools::getCollection (collection_, collection, event, false);
  // Specify argument verbose = false to prevent error messages if collection is not found. 
  if(!valid) {
    clog << "ERROR [MuonProducer::produce]:  could not get collection: " << collection_ << endl;  
    return;
  } 
  valid = anatools::getCollection (collPrimaryvertexs_, collPrimaryvertexs, event);
  if(!valid) {
    clog << "ERROR [MuonProducer::produce]:  could not get collection: " << collPrimaryvertexs_ << endl;  
    return;
  }
  pl_ = auto_ptr<vector<osu::Muon> > (new vector<osu::Muon> ());
  for (const auto &object : *collection)
    {
      osu::Muon muon(object);
      const reco::Vertex vtx = collPrimaryvertexs->at(0);  
      muon.set_isTightMuonWRTVtx(muon.isTightMuon(vtx));  
      pl_->push_back (muon);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(MuonProducer);
