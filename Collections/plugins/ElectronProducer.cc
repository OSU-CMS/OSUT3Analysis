#include "OSUT3Analysis/Collections/plugins/ElectronProducer.h"

#if IS_VALID(electrons)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

ElectronProducer::ElectronProducer (const edm::ParameterSet &cfg) :
  collections_    (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_ (cfg),
  beamSpot_       (cfg.getParameter<edm::InputTag> ("beamSpot")),
  conversions_    (cfg.getParameter<edm::InputTag> ("conversions")),
  rho_            (cfg.getParameter<edm::InputTag> ("rho")),
  gsfElectrons_   (cfg.getParameter<edm::InputTag> ("gsfElectrons"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("electrons");
  produces<vector<osu::Electron> > (collection_.instance ());
  gsfeElectronsToken_ = mayConsume<edm::View<reco::GsfElectron> >(gsfElectrons_);
}

ElectronProducer::~ElectronProducer ()
{
}

void
ElectronProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM  
  using namespace edm;
  using namespace reco;
  
  edm::Handle<edm::View<reco::GsfElectron> > electrons;
  edm::Handle<reco::ConversionCollection> conversions;
  edm::Handle<reco::BeamSpot> beamSpot;
  edm::Handle<double> rho;
  
  edm::Handle<vector<osu::Mcparticle> > particles;
  anatools::getCollection (edm::InputTag ("", ""), particles, event);
  if(!event.getByToken(gsfeElectronsToken_,electrons))
    return; 
  pl_ = auto_ptr<vector<osu::Electron> > (new vector<osu::Electron> ());
  for (const auto &object : *electrons)
    {
      osu::Electron electron (object, particles, cfg_);
      if(event.getByLabel (rho_, rho))
        electron.set_rho((float)(*rho)); 
      if(event.getByLabel (beamSpot_, beamSpot) && event.getByLabel (conversions_, conversions))
        electron.set_vtxFitConversion(ConversionTools::hasMatchedConversion(object, conversions, (*beamSpot).position()));
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

  event.put (pl_, gsfElectrons_.instance ());
  pl_.reset ();
#else
  edm::Handle<vector<TYPE (electrons)> > collection;
  if (!anatools::getCollection (collection_, collection, event, false))
    return;
  edm:Handle<vector<osu::Mcparticle> > particles;
  anatools::getCollection (edm::InputTag ("", ""), particles, event);

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
DEFINE_FWK_MODULE(ElectronProducer);

#endif
