#include "OSUT3Analysis/Collections/plugins/OSUMuonProducer.h"

#if IS_VALID(muons)

#include "OSUT3Analysis/Collections/interface/Primaryvertex.h"

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUMuonProducer::OSUMuonProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_ (cfg)
{
  collection_         = collections_.getParameter<edm::InputTag> ("muons");
  collPrimaryvertexs_ = collections_.getParameter<edm::InputTag> ("primaryvertexs");

  produces<vector<osu::Muon> > (collection_.instance ());
}

OSUMuonProducer::~OSUMuonProducer ()
{
}

void
OSUMuonProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE (muons)> > collection;
  edm::Handle<vector<TYPE(primaryvertexs)> > collPrimaryvertexs;
  edm::Handle<vector<osu::Primaryvertex> > collOSUPrimaryvertexs;
  if (!anatools::getCollection (collection_, collection, event, false))
    return;
  if (!anatools::getCollection (collPrimaryvertexs_, collPrimaryvertexs, event) && !anatools::getCollection (collPrimaryvertexs_, collOSUPrimaryvertexs, event)) {
    clog << "ERROR [OSUMuonProducer::produce]:  could not get collection: " << collPrimaryvertexs_ << endl;
    return;
  }
  edm::Handle<vector<osu::Mcparticle> > particles;
  anatools::getCollection (edm::InputTag ("", ""), particles, event);

  pl_ = auto_ptr<vector<osu::Muon> > (new vector<osu::Muon> ());
  for (const auto &object : *collection)
    {
      osu::Muon muon (object, particles, cfg_);
      const reco::Vertex &vtx = collPrimaryvertexs.isValid () ? collPrimaryvertexs->at (0) : collOSUPrimaryvertexs->at (0);
      muon.set_isTightMuonWRTVtx(muon.isTightMuon(vtx));
      pl_->push_back (muon);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUMuonProducer);

#endif
