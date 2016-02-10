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

  token_ = consumes<vector<TYPE(muons)> > (collection_);
  mcparticleToken_ = consumes<vector<osu::Mcparticle> > (edm::InputTag ());
  primaryvertexToken_ = consumes<vector<TYPE(primaryvertexs)> > (collPrimaryvertexs_);
}

OSUMuonProducer::~OSUMuonProducer ()
{
}

void
OSUMuonProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(muons)> > collection;
  edm::Handle<vector<TYPE(primaryvertexs)> > collPrimaryvertexs;
  if (!event.getByToken (token_, collection))
    return;
  if (!event.getByToken (primaryvertexToken_, collPrimaryvertexs)) {
    clog << "ERROR [OSUMuonProducer::produce]:  could not get collection: " << collPrimaryvertexs_ << endl;
    return;
  }
  edm::Handle<vector<osu::Mcparticle> > particles;
  event.getByToken (mcparticleToken_, particles);

  pl_ = auto_ptr<vector<osu::Muon> > (new vector<osu::Muon> ());
  for (const auto &object : *collection)
    {
      osu::Muon muon (object, particles, cfg_);
      if (collPrimaryvertexs->size ())
        {
          const reco::Vertex &vtx = collPrimaryvertexs->at (0);
          muon.set_isTightMuonWRTVtx(muon.isTightMuon(vtx));
        }
      else
          muon.set_isTightMuonWRTVtx(false);
      pl_->push_back (muon);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUMuonProducer);

#endif
