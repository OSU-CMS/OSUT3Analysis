#include "OSUT3Analysis/Collections/plugins/OSUBjetProducer.h"

#if IS_VALID(bjets)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM
#include "JetMETCorrections/Modules/interface/JetResolution.h"
#include "CondFormats/DataRecord/interface/JetResolutionRcd.h"
#include "CondFormats/DataRecord/interface/JetResolutionScaleFactorRcd.h"
#include "TRandom3.h"
#endif

OSUBjetProducer::OSUBjetProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  rho_         (cfg.getParameter<edm::InputTag>  ("rho")),
  jetResolutionPayload_ (cfg.getParameter<string> ("jetResolutionPayload")),
  jetResSFPayload_      (cfg.getParameter<string> ("jetResSFPayload")),
  jetResFromGlobalTag_  (cfg.getParameter<bool> ("jetResFromGlobalTag")),
  cfg_         (cfg)
{
  collection_ = collections_.getParameter<edm::InputTag> ("bjets");
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD
  electrons_ = collections_.getParameter<edm::InputTag> ("electrons");
  muons_ = collections_.getParameter<edm::InputTag> ("muons");
  genjets_ = collections_.getParameter<edm::InputTag> ("genjets");
#endif
  produces<vector<osu::Bjet> > (collection_.instance ());

  token_ = consumes<vector<TYPE(bjets)> > (collection_);
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD
  electronToken_ = consumes<vector<TYPE(electrons)> > (electrons_);
  muonToken_ = consumes<vector<TYPE(muons)> > (muons_);
  mcparticleToken_ = consumes<vector<osu::Mcparticle> > (collections_.getParameter<edm::InputTag> ("mcparticles"));
  genjetsToken_ = consumes<vector<TYPE(genjets)> > (genjets_);
  rhoToken_ = consumes<double> (rho_);
#endif
}

OSUBjetProducer::~OSUBjetProducer ()
{
}

void
OSUBjetProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(bjets)> > collection;
  if (!event.getByToken (token_, collection))
    return;
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD
  edm::Handle<vector<osu::Mcparticle> > particles;
  event.getByToken (mcparticleToken_, particles);

#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM
  // get JetCorrector parameters to get the jec uncertainty
  edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
  setup.get<JetCorrectionsRecord>().get("AK4PFchs", JetCorParColl);
  JetCorrectorParameters const & JetCorPar = (*JetCorParColl)["Uncertainty"];
  JetCorrectionUncertainty * jecUnc = new JetCorrectionUncertainty(JetCorPar);

  // Get jet energy resolution and scale factors
  // These are in the Global Tag for 80X but not for 76X,
  // Configuration/python/collectionProducer_cff.py looks at $CMSSW_BASE to set this choice
  JME::JetResolution jetEnergyResolution = (jetResFromGlobalTag_) ?
                                              JME::JetResolution::get(setup, "AK4PFchs_pt") :
                                              JME::JetResolution(jetResolutionPayload_);

  JME::JetResolutionScaleFactor jetEnergyResolutionSFs = (jetResFromGlobalTag_) ?
                                              JME::JetResolutionScaleFactor::get(setup, "AK4PFchs") :
                                              JME::JetResolutionScaleFactor(jetResSFPayload_);

  JME::JetParameters jetResParams;

  // get genjets for JER smearing matching
  edm::Handle<vector<TYPE(genjets)> > genjets;
  bool hasGenJets = event.getByToken (genjetsToken_, genjets);

  // get rho for JER smearing
  edm::Handle<double> rho;
  event.getByToken (rhoToken_, rho);

  // RNG for gaussian JER smearing (when there is no genjet match)
  TRandom3 * rng = new TRandom3(0);

  // get lepton collections for cross-cleaning
  edm::Handle<vector<TYPE (electrons)> > electrons;
  if (!event.getByToken (electronToken_, electrons))
    return;

  edm::Handle<vector<TYPE (muons)> > muons;
  if (!event.getByToken (muonToken_, muons))
    return;

  // construct vectors of "good" leptons (ID+iso)
  vector<const TYPE(electrons) *> goodElectrons;
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
	         electron.passConversionVeto())) {
      passElectronID = true;
    }
    if (passElectronID == false) continue;
    goodElectrons.push_back(&electron);
  }
  vector<const TYPE(muons) *> goodMuons;
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
    goodMuons.push_back(&muon);
  }
#endif
#endif

  pl_ = unique_ptr<vector<osu::Bjet> > (new vector<osu::Bjet> ());
  for (const auto &object : *collection)
    {
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD
      pl_->emplace_back (object, particles, cfg_);
      osu::Bjet &bjet = pl_->back ();
#elif DATA_FORMAT == AOD_CUSTOM
      pl_->emplace_back (object);
#endif

#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM
      bjet.set_pfCombinedInclusiveSecondaryVertexV2BJetTags(bjet.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));
      bjet.set_pfCombinedSecondaryVertexV2BJetTags(bjet.bDiscriminator("pfCombinedSecondaryVertexV2BJetTags"));
      bjet.set_pileupJetId(bjet.userFloat("pileupJetId:fullDiscriminant"));

      jecUnc->setJetEta(bjet.eta());
      jecUnc->setJetPt(bjet.pt());
      bjet.set_jecUncertainty(jecUnc->getUncertainty(true));

      jetResParams.setJetPt(bjet.pt());
      jetResParams.setJetEta(bjet.pt());
      jetResParams.setRho((float)(*rho));
      bjet.set_jetPtResolution(jetEnergyResolution.getResolution(jetResParams));
      bjet.set_setJetPtResolutionSF(jetEnergyResolutionSFs.getScaleFactor(jetResParams),
                                    jetEnergyResolutionSFs.getScaleFactor(jetResParams, Variation::UP),
                                    jetEnergyResolutionSFs.getScaleFactor(jetResParams, Variation::DOWN));

      // https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution#Smearing_procedures
      if(hasGenJets) {
        bool isMatchedToGenJet = false;
        for (const auto &genjet : *genjets) {
          double dR = deltaR (genjet, bjet);
          double dPt = bjet.pt() - genjet.pt();

          if(dR < 0.2 && fabs(dPt) < 3.0 * bjet.jer()) {
            bjet.set_smearedPt(max(0.0, genjet.pt() + bjet.jerSF() * dPt));
            bjet.set_smearedPtUp(max(0.0, genjet.pt() + bjet.jerSFUp() * dPt));
            bjet.set_smearedPtDown(max(0.0, genjet.pt() + bjet.jerSFDown() * dPt));

            isMatchedToGenJet = true;
            break;
          }
        }
        if(!isMatchedToGenJet) {

          if(bjet.jerSF() > 1.0) {
            double smearedPt = rng->Gaus(bjet.pt(),
                                         sqrt(bjet.jerSF() * bjet.jerSF() - 1) * bjet.jer());

            bjet.set_smearedPt(smearedPt);
            bjet.set_smearedPtUp(smearedPt * bjet.jerSFUp() / bjet.jerSF());
            bjet.set_smearedPtDown(smearedPt * bjet.jerSFDown() / bjet.jerSF());
          }
          else {
            bjet.set_smearedPt(INVALID_VALUE);
            bjet.set_smearedPtUp(INVALID_VALUE);
            bjet.set_smearedPtDown(INVALID_VALUE);
          }

        }

      } // if(hasGenjets)
      else {
        bjet.set_smearedPt(INVALID_VALUE);
        bjet.set_smearedPtUp(INVALID_VALUE);
        bjet.set_smearedPtDown(INVALID_VALUE);
      }

      double maxDeltaR_ = 0.3;

      int isMatchedToElectron = 0;
      for (const auto &electron : goodElectrons){
	       double dR = deltaR (*electron, bjet);
	        if (dR < maxDeltaR_) {
            isMatchedToElectron = true;
            break;
          }
      }

      bool isMatchedToMuon = false;
      for (const auto &muon : goodMuons){
	       double dR = deltaR (*muon, bjet);
         if (dR < maxDeltaR_) {
          isMatchedToMuon = true;
          break;
         }
      }

      if (isMatchedToElectron == 1 || isMatchedToMuon == 1)
        bjet.set_matchedToLepton(1);
      else
        bjet.set_matchedToLepton(0);
#endif
    }

  event.put (std::move (pl_), collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUBjetProducer);

#endif
