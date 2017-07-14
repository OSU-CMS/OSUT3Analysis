#include "OSUT3Analysis/Collections/plugins/OSUJetProducer.h"
#include "DataFormats/Math/interface/deltaR.h"

#if IS_VALID(jets)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM
#include "JetMETCorrections/Modules/interface/JetResolution.h"
#include "CondFormats/DataRecord/interface/JetResolutionRcd.h"
#include "CondFormats/DataRecord/interface/JetResolutionScaleFactorRcd.h"
#include "TRandom3.h"
#endif

OSUJetProducer::OSUJetProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  rho_         (cfg.getParameter<edm::InputTag>  ("rho")),
  jetResolutionPayload_ (cfg.getParameter<string> ("jetResolutionPayload")),
  jetResSFPayload_      (cfg.getParameter<string> ("jetResSFPayload")),
  jetResFromGlobalTag_  (cfg.getParameter<bool> ("jetResFromGlobalTag")),
  cfg_         (cfg)
{
  collection_ = collections_.getParameter<edm::InputTag> ("jets");
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD
  electrons_ = collections_.getParameter<edm::InputTag> ("electrons");
  muons_ = collections_.getParameter<edm::InputTag> ("muons");
  genjets_ = collections_.getParameter<edm::InputTag> ("genjets");
  primaryvertexs_ = collections_.getParameter<edm::InputTag> ("primaryvertexs");
#endif
  produces<vector<osu::Jet> > (collection_.instance ());

  token_ = consumes<vector<TYPE(jets)> > (collection_);
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD
  electronToken_ = consumes<vector<TYPE(electrons)> > (electrons_);
  muonToken_ = consumes<vector<TYPE(muons)> > (muons_);
  mcparticleToken_ = consumes<vector<osu::Mcparticle> > (collections_.getParameter<edm::InputTag> ("mcparticles"));
  genjetsToken_ = consumes<vector<TYPE(genjets)> > (genjets_);
  rhoToken_ = consumes<double> (rho_);
  primaryvertexsToken_ = consumes<vector<TYPE(primaryvertexs)> > (primaryvertexs_);
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

  // get vertex collections
  edm::Handle<vector<TYPE (primaryvertexs)> > primaryvertexs;
  if (!event.getByToken (primaryvertexsToken_, primaryvertexs))
    return;

  // construct vectors of "good" leptons (ID)
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
         electron.passConversionVeto())){
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

  pl_ = unique_ptr<vector<osu::Jet> > (new vector<osu::Jet> ());

  for (const auto &object : *collection)
    {
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD
      pl_->emplace_back (object, particles, cfg_);
      osu::Jet &jet = pl_->back ();
#elif DATA_FORMAT == AOD_CUSTOM
      pl_->emplace_back (object);
#endif

#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM

      // medianLog10(ipsig) CALC
      std::vector<double> ipsigVector;

      for(unsigned id =0; id < jet.getJetConstituents().size(); id++) {

	const pat::PackedCandidate &packedCand = dynamic_cast<const pat::PackedCandidate &>(*jet.getJetConstituents().at(id));
	const edm::Ptr<reco::Candidate> recoCand = jet.getJetConstituents().at(id);

	if (recoCand->charge() != 0){
	  double dxy = fabs(packedCand.dxy());
	  double dxyerr = packedCand.dxyError();
	  if(dxyerr>0){
	    double dxySig = dxy/dxyerr;
	    ipsigVector.push_back(dxySig);
	  }
	}
      } // end of loop over candidates

      double medianipsig = -4;

      if(ipsigVector.size() != 0){
	std::sort (ipsigVector.begin(), ipsigVector.end());
	if(ipsigVector.size()%2 == 0) medianipsig = (ipsigVector[ ipsigVector.size()/2 - 1] + ipsigVector[ ipsigVector.size() / 2 ]) / 2;
	if(ipsigVector.size()%2 == 1) medianipsig = ipsigVector[ ipsigVector.size()/2 ];
      }

      if(ipsigVector.size() != 0) jet.set_medianlog10ipsig( log10(medianipsig) );
      else jet.set_medianlog10ipsig( -4 );

      // ALPHA MAX CALC
      double numerator = 0;
      double denominator = 0;

      double alpha = 0;
      double alphaMax = 0;

      for(unsigned vertex = 0; vertex < primaryvertexs.product()->size(); vertex++) {

	for(unsigned id = 0; id < jet.getJetConstituents().size(); id++) {

	  const pat::PackedCandidate &packedCand = dynamic_cast<const pat::PackedCandidate &>(*jet.getJetConstituents().at(id));
	  const edm::Ptr<reco::Candidate> recoCand = jet.getJetConstituents().at(id);

	  if(packedCand.charge() != 0) {

	    double candPT = recoCand->pt();

	    if(packedCand.fromPV(vertex) > 1){
	      numerator += candPT;
	    }
	    denominator += candPT;
	  }
	} // end of loop over candidates

	if ( denominator != 0 ) alpha = (numerator / denominator);
	if ( alpha > alphaMax ) alphaMax = alpha;

      } // end of loop over vertices


      if( denominator != 0) jet.set_alphamax( alphaMax );
      else jet.set_alphamax(-1);

      jet.set_pfCombinedInclusiveSecondaryVertexV2BJetTags(jet.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));
      jet.set_pfCombinedSecondaryVertexV2BJetTags(jet.bDiscriminator("pfCombinedSecondaryVertexV2BJetTags"));
      jet.set_pileupJetId(jet.userFloat("pileupJetId:fullDiscriminant"));

      jecUnc->setJetEta(jet.eta());
      jecUnc->setJetPt(jet.pt());
      jet.set_jecUncertainty(jecUnc->getUncertainty(true));

      jetResParams.setJetPt(jet.pt());
      jetResParams.setJetEta(jet.eta());
      jetResParams.setRho((float)(*rho));
      jet.set_jetPtResolution(jetEnergyResolution.getResolution(jetResParams));
      jet.set_setJetPtResolutionSF(jetEnergyResolutionSFs.getScaleFactor(jetResParams),
                                   jetEnergyResolutionSFs.getScaleFactor(jetResParams, Variation::UP),
                                   jetEnergyResolutionSFs.getScaleFactor(jetResParams, Variation::DOWN));

      // https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution#Smearing_procedures
      if(hasGenJets) {
        bool isMatchedToGenJet = false;
        for (const auto &genjet : *genjets) {
          double dR = deltaR (genjet, jet);
          double dPt = jet.pt() - genjet.pt();

          if(dR < 0.2 && fabs(dPt) < 3.0 * jet.jer()) {
            jet.set_smearedPt(max(0.0, genjet.pt() + jet.jerSF() * dPt));
            jet.set_smearedPtUp(max(0.0, genjet.pt() + jet.jerSFUp() * dPt));
            jet.set_smearedPtDown(max(0.0, genjet.pt() + jet.jerSFDown() * dPt));

            isMatchedToGenJet = true;
            break;
          }
        }
        if(!isMatchedToGenJet) {

          if(jet.jerSF() > 1.0) {
            double smearedPt = rng->Gaus(jet.pt(),
                                         sqrt(jet.jerSF() * jet.jerSF() - 1) * jet.jer());

            jet.set_smearedPt(smearedPt);
            jet.set_smearedPtUp(smearedPt * jet.jerSFUp() / jet.jerSF());
            jet.set_smearedPtDown(smearedPt * jet.jerSFDown() / jet.jerSF());
          }
          else {
            jet.set_smearedPt(INVALID_VALUE);
            jet.set_smearedPtUp(INVALID_VALUE);
            jet.set_smearedPtDown(INVALID_VALUE);
          }

        }

      } // if(hasGenjets)
      else {
        jet.set_smearedPt(INVALID_VALUE);
        jet.set_smearedPtUp(INVALID_VALUE);
        jet.set_smearedPtDown(INVALID_VALUE);
      }

      double maxDeltaR_ = 0.3;

      int isMatchedToElectron = 0;
      for (const auto &electron : goodElectrons){
        double dR = deltaR (*electron, jet);
        if (dR < maxDeltaR_){
          isMatchedToElectron = true;
          break;
        }
      }

      bool isMatchedToMuon = false;
      for (const auto &muon : goodMuons){
        double dR = deltaR (*muon, jet);
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
    }
  event.put (std::move (pl_), collection_.instance ());
  pl_.reset ();

#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM
  delete jecUnc;
  delete rng;
#endif
}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUJetProducer);

#endif
