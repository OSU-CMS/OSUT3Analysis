#include "OSUT3Analysis/Collections/plugins/OSUGenericJetProducer.h"
#include "DataFormats/Math/interface/deltaR.h"

#if IS_VALID(jets)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

template<class T>
OSUGenericJetProducer<T>::OSUGenericJetProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  rho_         (cfg.getParameter<edm::InputTag>  ("rho")),
  jetCorrectionPayloadName_ (cfg.getParameter<string> ("jetCorrectionPayload")),
  jetResolutionPayload_ (cfg.getParameter<edm::FileInPath> ("jetResolutionPayload").fullPath()),
  jetResSFPayload_      (cfg.getParameter<edm::FileInPath> ("jetResSFPayload").fullPath()),
  jetResFromGlobalTag_  (cfg.getParameter<bool> ("jetResFromGlobalTag")),
  jetResNewPrescription_ (cfg.getParameter<bool> ("jetResNewPrescription")),

  cfg_         (cfg)
{
  collection_ = collections_.getParameter<edm::InputTag> ("jets");
#ifndef STOPPPED_PTLS
  electrons_ = collections_.getParameter<edm::InputTag> ("electrons");
  muons_ = collections_.getParameter<edm::InputTag> ("muons");
  genjets_ = collections_.getParameter<edm::InputTag> ("genjets");
  primaryvertexs_ = collections_.getParameter<edm::InputTag> ("primaryvertexs");
#endif
  produces<vector<T> > (collection_.instance ());

  token_ = consumes<vector<TYPE(jets)> > (collection_);
#ifndef STOPPPED_PTLS
  electronToken_ = consumes<vector<TYPE(electrons)> > (electrons_);
  muonToken_ = consumes<vector<TYPE(muons)> > (muons_);
  mcparticleToken_ = consumes<vector<osu::Mcparticle> > (collections_.getParameter<edm::InputTag> ("mcparticles"));
  genjetsToken_ = consumes<vector<TYPE(genjets)> > (genjets_);
  rhoToken_ = consumes<double> (rho_);
  primaryvertexsToken_ = consumes<vector<TYPE(primaryvertexs)> > (primaryvertexs_);
#endif

#if CMSSW_VERSION_CODE >= CMSSW_VERSION(12,4,0)
  JetCorrParToken_ = esConsumes(edm::ESInputTag("", jetCorrectionPayloadName_));
  jetResolutionToken_ = esConsumes(edm::ESInputTag("", "AK4PFchs_pt"));
  jetResolutionSFToken_ = esConsumes(edm::ESInputTag("", "AK4PFchs"));
#endif

}

template<class T> OSUGenericJetProducer<T>::~OSUGenericJetProducer ()
{
}

template<class T> void
OSUGenericJetProducer<T>::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(jets)> > collection;
  if (!event.getByToken (token_, collection))
    return;
#ifndef STOPPPED_PTLS
  edm::Handle<vector<osu::Mcparticle> > particles;
  event.getByToken (mcparticleToken_, particles);

#if DATA_FORMAT_FROM_MINIAOD
  // get JetCorrector parameters to get the jec uncertainty
  //edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
  //setup.get<JetCorrectionsRecord>().get("AK4PFchs", JetCorParColl);
  const JetCorrectorParametersCollection &JetCorParColl = setup.getData(JetCorrParToken_);

  JetCorrectorParameters const & JetCorPar = JetCorParColl["Uncertainty"];
  JetCorrectionUncertainty * jecUnc = new JetCorrectionUncertainty(JetCorPar);

  // Get jet energy resolution and scale factors
  // These are in the Global Tag for 80X but not for 76X,
  // Configuration/python/collectionProducer_cff.py looks at $CMSSW_BASE to set this choice

#if CMSSW_VERSION_CODE < CMSSW_VERSION(12,4,0)
  JME::JetResolution jetEnergyResolution = (jetResFromGlobalTag_) ?
    JME::JetResolution::get(setup, "AK4PFchs_pt") :
    JME::JetResolution(jetResolutionPayload_);

  JME::JetResolutionScaleFactor jetEnergyResolutionSFs = (jetResFromGlobalTag_) ?
    JME::JetResolutionScaleFactor::get(setup, "AK4PFchs") :
    JME::JetResolutionScaleFactor(jetResSFPayload_);
#else
  // Un-verified: Modification made in CMSSW for Jet Energy Resolution, DB file used: https://github.com/cms-sw/cmssw/tree/CMSSW_12_4_X/JetMETCorrections/Modules/test (June 14, 2022)
  JME::JetResolution jetEnergyResolution = JME::JetResolution::get(setup, jetResolutionToken_);
  JME::JetResolutionScaleFactor jetEnergyResolutionSFs = JME::JetResolutionScaleFactor::get(setup, jetResolutionSFToken_);
#endif

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
    if ((electron.isEB() &&
#if CMSSW_VERSION_CODE >= CMSSW_VERSION(9,4,0)
         electron.gsfTrack()->hitPattern().numberOfAllHits(reco::HitPattern::MISSING_INNER_HITS) <= 1 &&
#else
         electron.gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS)    <= 1 &&
#endif
         abs(electron.deltaEtaSuperClusterTrackAtVtx()) < 0.00308 &&
         abs(electron.deltaPhiSuperClusterTrackAtVtx()) < 0.0816 &&
         electron.full5x5_sigmaIetaIeta() < 0.00998 &&
         electron.hadronicOverEm() < 0.0414 &&
         abs(1.0/electron.ecalEnergy() - electron.eSuperClusterOverP()/electron.ecalEnergy()) < 0.0129 &&
         electron.passConversionVeto()) ||
        (electron.isEE() &&
#if CMSSW_VERSION_CODE >= CMSSW_VERSION(9,4,0)
         electron.gsfTrack()->hitPattern().numberOfAllHits(reco::HitPattern::MISSING_INNER_HITS) <= 1 &&
#else
         electron.gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS)    <= 1 &&
#endif
         abs(electron.deltaEtaSuperClusterTrackAtVtx()) < 0.00605 &&
         abs(electron.deltaPhiSuperClusterTrackAtVtx()) < 0.0394 &&
         electron.full5x5_sigmaIetaIeta() < 0.0292 &&
         electron.hadronicOverEm() < 0.0641 &&
         abs(1/electron.ecalEnergy() - electron.eSuperClusterOverP()/electron.ecalEnergy()) < 0.0129 &&
         electron.passConversionVeto()))
    {
      passElectronID = true;
    }
    if (passElectronID == false) continue;
    goodElectrons.push_back(&electron);
  }
  vector<const TYPE(muons) *> goodMuons;
  for (const auto &muon : *muons){
    bool passMuonID = false;
    if (muon.isGlobalMuon() == false ||  muon.isPFMuon() == false) continue;
    if (muon.globalTrack()->hitPattern().numberOfValidMuonHits() > 0 &&
        muon.numberOfMatchedStations() > 1 &&
        muon.globalTrack()->normalizedChi2() < 10 &&
        muon.innerTrack()->hitPattern().numberOfValidPixelHits() > 0 &&
        muon.innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5){
      passMuonID = true;
    }
    if (passMuonID == false) continue;
    goodMuons.push_back(&muon);
  }
#endif // DATA_FORMAT_FROM_MINIAOD
#endif // not STOPPPED_PTLS

  pl_ = unique_ptr<vector<T> > (new vector<T> ());

  for (const auto &object : *collection)
    {
#ifndef STOPPPED_PTLS
      pl_->emplace_back (object, particles, cfg_);

#else // STOPPPED_PTLS
      pl_->emplace_back (object);
#endif

#if DATA_FORMAT_FROM_MINIAOD

      T &jet = pl_->back ();

      // medianLog10(ipsig) CALC
      std::vector<double> ipsigVector;

      for(unsigned id =0; id < jet.getJetConstituents().size(); id++) {

	const pat::PackedCandidate &packedCand = dynamic_cast<const pat::PackedCandidate &>(*jet.getJetConstituents().at(id));
	const edm::Ptr<reco::Candidate> recoCand = jet.getJetConstituents().at(id);

        try {
#if CMSSW_VERSION_CODE >= CMSSW_VERSION(9,1,1)
          if(packedCand.hasTrackDetails() && recoCand->charge() != 0) {
#else
          if(recoCand->charge() != 0) {
#endif
            double dxy = fabs(packedCand.dxy());
            double dxyerr = packedCand.dxyError();
            if(dxyerr>0) {
              double dxySig = dxy/dxyerr;
              ipsigVector.push_back(dxySig);
            }
          }
        }
        catch (cms::Exception &e) {
          edm::LogWarning ("OSUGenericJetProducer (medianlog10ipsig)") << e.what ();
        }
      } // end of loop over candidates

      double medianipsig = -4;

      if(!ipsigVector.empty()) {
        std::sort (ipsigVector.begin(), ipsigVector.end());
        if(ipsigVector.size()%2 == 0) medianipsig = (ipsigVector[ ipsigVector.size()/2 - 1] + ipsigVector[ ipsigVector.size() / 2 ]) / 2;
        if(ipsigVector.size()%2 == 1) medianipsig = ipsigVector[ ipsigVector.size()/2 ];
      }

      if(!ipsigVector.empty()) jet.set_medianlog10ipsig( log10(medianipsig) );
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

          try
            {
              if(packedCand.charge() != 0) {

                double candPT = recoCand->pt();

                if(packedCand.fromPV(vertex) > 1){
                  numerator += candPT;
                }
                denominator += candPT;
              }
            }
          catch (cms::Exception &e)
            {
              edm::LogWarning ("OSUGenericJetProducer (alphamax)") << e.what ();
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

          if(jetResNewPrescription_) {
            // https://github.com/cms-sw/cmssw/blob/CMSSW_8_0_25/PhysicsTools/PatUtils/interface/SmearedJetProducerT.h#L237
            if(dR < 0.2 && fabs(dPt) < 3.0 * jet.pt() * jet.jer()) {
              // smearFactor = 1 + (jet.jerSF() - 1.) * dPt / jet.pt();
              jet.set_smearedPt(    max(0.0, jet.pt() + (jet.jerSF()     - 1.) * dPt));
              jet.set_smearedPtUp(  max(0.0, jet.pt() + (jet.jerSFUp()   - 1.) * dPt));
              jet.set_smearedPtDown(max(0.0, jet.pt() + (jet.jerSFDown() - 1.) * dPt));
              isMatchedToGenJet = true;
              break;
            }
          }
          else {
            if(dR < 0.2 && fabs(dPt) < 3.0 * jet.jer()) {
              jet.set_smearedPt(    max(0.0, genjet.pt() + jet.jerSF()     * dPt));
              jet.set_smearedPtUp(  max(0.0, genjet.pt() + jet.jerSFUp()   * dPt));
              jet.set_smearedPtDown(max(0.0, genjet.pt() + jet.jerSFDown() * dPt));
              isMatchedToGenJet = true;
              break;
            }
          }

        } // for genjets

        if(!isMatchedToGenJet) {

          if(jet.jerSF() > 1.0) {
            if(jetResNewPrescription_) {
              double smearedPt = rng->Gaus(0.0, jet.jer()) * sqrt(max(jet.jerSF() * jet.jerSF() - 1.0, 0.0));
              smearedPt = jet.pt() * (1.0 + smearedPt);
              jet.set_smearedPt(    smearedPt);
              jet.set_smearedPtUp(  smearedPt * jet.jerSFUp() / jet.jerSF());
              jet.set_smearedPtDown(smearedPt * jet.jerSFDown() / jet.jerSF());
            }
            else {
              double smearedPt = rng->Gaus(jet.pt(),
                                           sqrt(jet.jerSF() * jet.jerSF() - 1) * jet.jer());

              jet.set_smearedPt(smearedPt);
              jet.set_smearedPtUp(smearedPt * jet.jerSFUp() / jet.jerSF());
              jet.set_smearedPtDown(smearedPt * jet.jerSFDown() / jet.jerSF());
            }
          }
          else {
            // if the SF < 1 the method doesn't work, so we just don't smear this jet
            jet.set_smearedPt(    jet.pt());
            jet.set_smearedPtUp(  jet.pt());
            jet.set_smearedPtDown(jet.pt());
          }

        } // if no matched genJet

        // https://github.com/cms-sw/cmssw/blob/CMSSW_8_0_25/PhysicsTools/PatUtils/interface/SmearedJetProducerT.h#L255
        if(jetResNewPrescription_ && jet.energy() * jet.smearedPt() / jet.pt() < 1.e-2) {
          // Negative or too small smearFactor. We would change direction of the jet
          // and this is not what we want.
          // Recompute the smearing factor in order to have jet.energy() == MIN_JET_ENERGY (1.e-2)
          double newSmearFactor = 1.e-2 / jet.energy();
          jet.set_smearedPt(    newSmearFactor * jet.pt());
          jet.set_smearedPtUp(  newSmearFactor * jet.pt() * jet.jerSFUp()   / jet.jerSF());
          jet.set_smearedPtDown(newSmearFactor * jet.pt() * jet.jerSFDown() / jet.jerSF());
        }

      } // if(hasGenjets)
      else {
        // if genJets don't exist the method doesn't work, so we just don't smear this jet
        jet.set_smearedPt(    jet.pt());
        jet.set_smearedPtUp(  jet.pt());
        jet.set_smearedPtDown(jet.pt());
      }

      double maxDeltaR_ = 0.4;

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

#if DATA_FORMAT_FROM_MINIAOD
  delete jecUnc;
  delete rng;
#endif
}

#include "FWCore/Framework/interface/MakerMacros.h"
typedef OSUGenericJetProducer<osu::Jet> OSUJetProducer;
DEFINE_FWK_MODULE(OSUJetProducer);

#if IS_VALID(bjets)
typedef OSUGenericJetProducer<osu::Bjet> OSUBjetProducer;
DEFINE_FWK_MODULE(OSUBjetProducer);
#endif // IS_VALID(bjets)

#endif // IS_VALID(jets)
