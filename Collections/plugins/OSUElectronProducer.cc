#include "OSUT3Analysis/Collections/plugins/OSUElectronProducer.h"

#if IS_VALID(electrons)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUElectronProducer::OSUElectronProducer (const edm::ParameterSet &cfg) :
  collections_    (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_ (cfg),
  beamSpot_       (cfg.getParameter<edm::InputTag> ("beamSpot")),
  conversions_    (cfg.getParameter<edm::InputTag> ("conversions")),
  rho_            (cfg.getParameter<edm::InputTag> ("rho")),
  vertices_            (cfg.getParameter<edm::InputTag> ("vertices"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("electrons");
  produces<vector<osu::Electron> > (collection_.instance ());

  token_ = consumes<vector<TYPE(electrons)> > (collection_);
  mcparticleToken_ = consumes<vector<osu::Mcparticle> > (collections_.getParameter<edm::InputTag> ("mcparticles"));
  rhoToken_ = consumes<double> (rho_);
}

OSUElectronProducer::~OSUElectronProducer ()
{
}

void
OSUElectronProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM  
  using namespace edm;
  using namespace reco;
  
  edm::Handle<vector<TYPE (electrons)> > collection;
  edm::Handle<reco::BeamSpot> beamSpot;
  edm::Handle<vector<reco::Conversion> > conversions;
  edm::Handle<double> rho;
  edm::Handle<vector<TYPE(primaryvertexs)> > vertices;
  
  edm::Handle<vector<osu::Mcparticle> > particles;
  event.getByToken (mcparticleToken_, particles);
  if (!event.getByToken (token_, collection))
    return;
  pl_ = auto_ptr<vector<osu::Electron> > (new vector<osu::Electron> ());
  for (const auto &object : *collection)
    {
      osu::Electron electron (object, particles, cfg_);
      if(event.getByToken (rhoToken_, rho))
        electron.set_rho((float)(*rho)); 
      if(event.getByLabel (beamSpot_, beamSpot) && event.getByLabel (conversions_, conversions) && event.getByLabel (vertices_, vertices) && vertices->size ())
        electron.set_passesTightID_noIsolation (*beamSpot, vertices->at (0), conversions);
      electron.set_missingInnerHits(object.gsfTrack()->hitPattern ().numberOfHits(reco::HitPattern::MISSING_INNER_HITS));
      float effectiveArea = 0;
      if(abs(object.superCluster()->eta()) >= 0.0000 && abs(object.superCluster()->eta()) < 1.0000)
        effectiveArea = 0.1752;
      if(abs(object.superCluster()->eta()) >= 1.0000 && abs(object.superCluster()->eta()) < 1.4790)
        effectiveArea = 0.1862;
      if(abs(object.superCluster()->eta()) >= 1.4790 && abs(object.superCluster()->eta()) < 2.0000)
        effectiveArea = 0.1411;
      if(abs(object.superCluster()->eta()) >= 2.0000 && abs(object.superCluster()->eta()) < 2.2000)
        effectiveArea = 0.1534;
      if(abs(object.superCluster()->eta()) >= 2.2000 && abs(object.superCluster()->eta()) < 2.3000)
        effectiveArea = 0.1903;
      if(abs(object.superCluster()->eta()) >= 2.3000 && abs(object.superCluster()->eta()) < 2.4000)
        effectiveArea = 0.2243;
      if(abs(object.superCluster()->eta()) >= 2.4000 && abs(object.superCluster()->eta()) < 5.0000)
        effectiveArea = 0.2687;
      electron.set_AEff(effectiveArea);
      
      pl_->push_back (electron);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
#else
  edm::Handle<vector<TYPE(electrons)> > collection;
  if (!event.getByToken (token_, collection))
    return;
  edm::Handle<vector<osu::Mcparticle> > particles;
  event.getByToken (mcparticleToken_, particles);

  pl_ = auto_ptr<vector<osu::Electron> > (new vector<osu::Electron> ());
  for (const auto &object : *collection)
    {
      const osu::Electron electron (object, particles, cfg_);
      pl_->push_back (electron);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
#endif
}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUElectronProducer);

#endif
