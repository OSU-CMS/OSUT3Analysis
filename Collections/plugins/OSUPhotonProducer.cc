#include "OSUT3Analysis/Collections/plugins/OSUPhotonProducer.h"

#if IS_VALID(photons)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUPhotonProducer::OSUPhotonProducer (const edm::ParameterSet &cfg) :
  collections_                 (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_                         (cfg),
  rho_                         (cfg.getParameter<edm::InputTag>     ("rho")),
  vidLooseIdMap_               (cfg.getParameter<edm::InputTag>     ("vidLooseIdMap")),
  vidMediumIdMap_              (cfg.getParameter<edm::InputTag>     ("vidMediumIdMap")),
  vidTightIdMap_               (cfg.getParameter<edm::InputTag>     ("vidTightIdMap")),
  effectiveAreasChargedHadron_ ((cfg.getParameter<edm::FileInPath>  ("effAreasChargedHadronPayload")).fullPath()),
  effectiveAreasNeutralHadron_ ((cfg.getParameter<edm::FileInPath>  ("effAreasNeutralHadronPayload")).fullPath()),
  effectiveAreasPhoton_        ((cfg.getParameter<edm::FileInPath>  ("effAreasPhotonPayload")).fullPath())
{
  collection_ = collections_.getParameter<edm::InputTag> ("photons");

  produces<vector<osu::Photon> > (collection_.instance ());

  token_ = consumes<edm::View<TYPE(photons)> > (collection_);
  mcparticleToken_ = consumes<vector<osu::Mcparticle> > (collections_.getParameter<edm::InputTag> ("mcparticles"));
  rhoToken_ = consumes<double>(rho_);

  vidLooseIdMapToken_  = consumes<edm::ValueMap<bool> > (vidLooseIdMap_);
  vidMediumIdMapToken_ = consumes<edm::ValueMap<bool> > (vidMediumIdMap_);
  vidTightIdMapToken_  = consumes<edm::ValueMap<bool> > (vidTightIdMap_);
}

OSUPhotonProducer::~OSUPhotonProducer ()
{
}

void
OSUPhotonProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<edm::View<TYPE(photons)> > collection;
  if (!event.getByToken (token_, collection))
    return;
  edm::Handle<vector<osu::Mcparticle> > particles;
  event.getByToken (mcparticleToken_, particles);

  edm::Handle<double> rho;

  edm::Handle<edm::ValueMap<bool> > vidLooseIdMap;
  event.getByToken(vidLooseIdMapToken_, vidLooseIdMap);

  edm::Handle<edm::ValueMap<bool> > vidMediumIdMap;
  event.getByToken(vidMediumIdMapToken_, vidMediumIdMap);

  edm::Handle<edm::ValueMap<bool> > vidTightIdMap;
  event.getByToken(vidTightIdMapToken_, vidTightIdMap);

  pl_ = unique_ptr<vector<osu::Photon> > (new vector<osu::Photon> ());

  unsigned iPho = -1;

  for (const auto &object : *collection)
    {
      ++iPho;

      pl_->emplace_back (object, particles, cfg_);
      osu::Photon &photon = pl_->back ();

      if(event.getByToken(rhoToken_, rho)) photon.set_rho((float)(*rho));

      if(vidLooseIdMap.isValid())
        photon.set_passesVID_looseID ( (*vidLooseIdMap)[(*collection).refAt(iPho)] );

      if(vidMediumIdMap.isValid())
        photon.set_passesVID_mediumID ( (*vidMediumIdMap)[(*collection).refAt(iPho)] );

      if(vidTightIdMap.isValid())
        photon.set_passesVID_tightID ( (*vidTightIdMap)[(*collection).refAt(iPho)] );

      float Aeff_chargedHadron = 0.;
      float Aeff_neutralHadron = 0.;
      float Aeff_photon = 0.;
      float Aeff_eta = abs(object.superCluster()->eta());
      Aeff_chargedHadron = effectiveAreasChargedHadron_.getEffectiveArea(Aeff_eta);
      Aeff_neutralHadron = effectiveAreasNeutralHadron_.getEffectiveArea(Aeff_eta);
      Aeff_photon = effectiveAreasPhoton_.getEffectiveArea(Aeff_eta);
      photon.set_AEff_neutralHadron(Aeff_neutralHadron);
      photon.set_Aeff_chargedHadron(Aeff_chargedHadron);
      photon.set_Aeff_photon(Aeff_photon);
    }

  event.put (std::move (pl_), collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUPhotonProducer);

#endif
