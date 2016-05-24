#include "OSUT3Analysis/Collections/plugins/OSUJetProducer.h"

#if IS_VALID(jets)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUJetProducer::OSUJetProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_ (cfg)
{
  collection_ = collections_.getParameter<edm::InputTag> ("jets");
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD
  electrons_ = collections_.getParameter<edm::InputTag> ("electrons");
  muons_ = collections_.getParameter<edm::InputTag> ("muons");
#endif
  produces<vector<osu::Jet> > (collection_.instance ());

  token_ = consumes<vector<TYPE(jets)> > (collection_);
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD
  electronToken_ = consumes<vector<TYPE(electrons)> > (electrons_);
  muonToken_ = consumes<vector<TYPE(muons)> > (muons_);
  mcparticleToken_ = consumes<vector<osu::Mcparticle> > (collections_.getParameter<edm::InputTag> ("mcparticles"));
#endif
}

OSUJetProducer::~OSUJetProducer ()
{
}

void
OSUJetProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(jets)> > collection;
  if (!event.getByToken (token_, collection))
    return;
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD
  edm::Handle<vector<osu::Mcparticle> > particles;
  event.getByToken (mcparticleToken_, particles);

#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM
  // get lepton collections for cross-cleaning
  edm::Handle<vector<TYPE (electrons)> > electrons;
  if (!event.getByToken (electronToken_, electrons))
    return;

  edm::Handle<vector<TYPE (muons)> > muons;
  if (!event.getByToken (muonToken_, muons))
    return;

  // construct vectors of "good" leptons (ID)
  goodElectrons_ = auto_ptr<vector<TYPE (electrons)> > (new vector<TYPE (electrons)> ());
  for (const auto &electron : *electrons){
    bool passElectronID = false;
    if ((electron.isEB() &&                                             \
         electron.gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS) <= 2 && \
         abs(electron.deltaEtaSuperClusterTrackAtVtx()) < 0.00926 &&     \
         abs(electron.deltaPhiSuperClusterTrackAtVtx()) < 0.0336 &&      \
         electron.full5x5_sigmaIetaIeta() < 0.0101 &&                    \
         electron.hadronicOverEm() < 0.0597 &&                           \
         abs(1.0/electron.ecalEnergy() - electron.eSuperClusterOverP()/electron.ecalEnergy()) < 0.012 && \
         electron.passConversionVeto()) ||                               \
        (electron.isEE() &&                                               \
         electron.gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS) <= 1 && \
         abs(electron.deltaEtaSuperClusterTrackAtVtx()) < 0.00724 &&      \
         abs(electron.deltaPhiSuperClusterTrackAtVtx()) < 0.0918 &&       \
         electron.full5x5_sigmaIetaIeta() < 0.0279 &&                     \
         electron.hadronicOverEm() < 0.0615 &&                            \
         abs(1/electron.ecalEnergy() - electron.eSuperClusterOverP()/electron.ecalEnergy()) < 0.00999 && \
         electron.passConversionVeto())){
      passElectronID = true;
    }
    if (passElectronID == false) continue;
    goodElectrons_->push_back(electron);
  }
  goodMuons_ = auto_ptr<vector<TYPE (muons)> > (new vector<TYPE (muons)> ());
  for (const auto &muon : *muons){
    bool passMuonID = false;
    if (muon.isGlobalMuon() == false ||  muon.isPFMuon() == false) continue;
    if (muon.globalTrack()->hitPattern().numberOfValidMuonHits() > 0 && \
        muon.numberOfMatchedStations() > 1 &&                           \
        muon.globalTrack()->normalizedChi2() < 10 &&                    \
        muon.innerTrack()->hitPattern().numberOfValidPixelHits() > 0 && \
        muon.innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5){
      passMuonID = true;
    }
    if (passMuonID == false) continue;
    goodMuons_->push_back(muon);
  }
#endif
#endif

  pl_ = auto_ptr<vector<osu::Jet> > (new vector<osu::Jet> ());
  for (const auto &object : *collection)
    {
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD
      osu::Jet jet (object, particles, cfg_);
#elif DATA_FORMAT == AOD_CUSTOM
      const osu::Jet jet (object);
#endif

#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM
      jet.set_pfCombinedInclusiveSecondaryVertexV2BJetTags(jet.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));
      jet.set_pfCombinedSecondaryVertexV2BJetTags(jet.bDiscriminator("pfCombinedSecondaryVertexV2BJetTags"));

      double maxDeltaR_ = 0.05;

      int isMatchedToElectron = 0;
      for (const auto &electron : *goodElectrons_){
        double dR = deltaR (electron, jet);
        if (dR < maxDeltaR_){
          isMatchedToElectron = true;
          break;
        }
      }

      bool isMatchedToMuon = false;
      for (const auto &muon : *goodMuons_){
        double dR = deltaR (muon, jet);
        if (dR < maxDeltaR_){
          isMatchedToMuon = true;
          break;
        }
      }

      if (isMatchedToElectron == 1 || isMatchedToMuon == 1)
        jet.set_matchedToLepton(1);
      else
        jet.set_matchedToLepton(0);

#endif

      pl_->push_back (jet);
    }
  event.put (pl_, collection_.instance ());
  pl_.reset ();
}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUJetProducer);

#endif
