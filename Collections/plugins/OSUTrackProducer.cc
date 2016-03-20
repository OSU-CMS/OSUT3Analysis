#include "OSUT3Analysis/Collections/plugins/OSUTrackProducer.h"

#if IS_VALID(tracks)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUTrackProducer::OSUTrackProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_ (cfg)
{
  collection_ = collections_.getParameter<edm::InputTag> ("tracks");

  produces<vector<osu::Track> > (collection_.instance ());

  token_ = consumes<vector<TYPE(tracks)> > (collection_);
  mcparticleToken_ = consumes<vector<osu::Mcparticle> > (collections_.getParameter<edm::InputTag> ("mcparticles"));
  jetsToken_ = consumes<vector<TYPE(jets)> > (collections_.getParameter<edm::InputTag> ("jets"));  

}

OSUTrackProducer::~OSUTrackProducer ()
{
}

void
OSUTrackProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(tracks)> > collection;
  if (!event.getByToken (token_, collection))
    return;
  edm::Handle<vector<osu::Mcparticle> > particles;  
  event.getByToken (mcparticleToken_, particles);  
  edm::Handle<vector<TYPE(jets)> > jets;  
  if (!event.getByToken (jetsToken_, jets)) {
    clog << "ERROR:  Could not find jets collection." << endl; 
    return;
  } 


  pl_ = auto_ptr<vector<osu::Track> > (new vector<osu::Track> ());
  for (const auto &object : *collection)
    {
      osu::Track track (object, particles, cfg_);

      double dRMinJet = 999;  
      for (const auto &jet : *jets) {
	if (!(jet.pt() > 30))         continue; 
	if (!(fabs(jet.eta()) < 4.5)) continue; 
	double dR = deltaR(track, jet);
	if (dR < dRMinJet) {
	  dRMinJet = dR;
	}
      }

      track.set_dRMinJet(dRMinJet);  

      pl_->push_back (track);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUTrackProducer);

#endif
