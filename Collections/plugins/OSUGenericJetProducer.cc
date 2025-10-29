#include "OSUT3Analysis/Collections/plugins/OSUGenericJetProducer.h"
#include "DataFormats/Math/interface/deltaR.h"

#if IS_VALID(jets)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

template<class T>
OSUGenericJetProducer<T>::OSUGenericJetProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  rho_         (cfg.getParameter<edm::InputTag>  ("rho")),
  jetResNewPrescription_ (cfg.getParameter<bool> ("jetResNewPrescription")),
  jecjerFile_ (cfg.getParameter<edm::FileInPath> ("jecjerFile").fullPath()),
  dataPeriod_ (cfg.getParameter<string> ("dataPeriod")),
  dataEra_ (cfg.getParameter<string> ("dataEra")),
  cfg_         (cfg)
{
  jets_ = collections_.getParameter<edm::InputTag> ("jets");
#ifndef STOPPPED_PTLS
  electrons_ = collections_.getParameter<edm::InputTag> ("electrons");
  muons_ = collections_.getParameter<edm::InputTag> ("muons");
  genjets_ = collections_.getParameter<edm::InputTag> ("genjets");
  primaryvertexs_ = collections_.getParameter<edm::InputTag> ("primaryvertexs");
#endif
  produces<vector<T> > (jets_.instance ());

  jetsToken_ = consumes<vector<TYPE(jets)> > (jets_);
#ifndef STOPPPED_PTLS
  electronToken_ = consumes<vector<TYPE(electrons)> > (electrons_);
  muonToken_ = consumes<vector<TYPE(muons)> > (muons_);
  mcparticleToken_ = consumes<vector<osu::Mcparticle> > (collections_.getParameter<edm::InputTag> ("mcparticles"));
  genjetsToken_ = consumes<vector<TYPE(genjets)> > (genjets_);
  rhoToken_ = consumes<double> (rho_);
  primaryvertexsToken_ = consumes<vector<TYPE(primaryvertexs)> > (primaryvertexs_);
#endif

  f_jecjer_ = TFile::Open(jecjerFile_.c_str(), "read");
  jetEnergyScaleMCL2RelativeHistName_ = "JECL2MC_" + dataPeriod_ + "_AK4PFPuppi";
  jetEnergyScaleMCL3AbsoluteHistName_ = "JECL3MC_" + dataPeriod_ + "_AK4PFPuppi";
  jetEnergyScaleDATAL2RelativeHistName_ = "JECL2DATA_" + dataPeriod_ + dataEra_ + "_AK4PFPuppi";
  jetEnergyScaleDATAL3AbsoluteHistName_ = "JECL3DATA_" + dataPeriod_ + dataEra_ + "_AK4PFPuppi";
  jetEnergyScaleL2L3HistName_ = "JECL2L3DATA_" + dataPeriod_ + dataEra_ + "_AK4PFPuppi";
  jetEnergyScaleUncHistName_ = "JECUnc_" + dataPeriod_ + "_AK4PFPuppi";
  jetEnergyResSFNomHistName_ = "JERSFNom_" + dataPeriod_ + "_AK4PFPuppi";
  jetEnergyResSFUpHistName_ = "JERSFUp_" + dataPeriod_ + "_AK4PFPuppi";
  jetEnergyResSFDownHistName_ = "JERSFDown_" + dataPeriod_ + "_AK4PFPuppi";
  jetEnergyResPtResHistName_ = "JERPtRes_" + dataPeriod_ + "_AK4PFPuppi";
  jetEnergyScaleMCL2RelativeHist_ = (TH2D*)f_jecjer_->Get(jetEnergyScaleMCL2RelativeHistName_.c_str());
  jetEnergyScaleMCL3AbsoluteHist_ = (TH2D*)f_jecjer_->Get(jetEnergyScaleMCL3AbsoluteHistName_.c_str());
  jetEnergyScaleDATAL2RelativeHist_ = (TH2D*)f_jecjer_->Get(jetEnergyScaleDATAL2RelativeHistName_.c_str());
  jetEnergyScaleDATAL3AbsoluteHist_ = (TH2D*)f_jecjer_->Get(jetEnergyScaleDATAL3AbsoluteHistName_.c_str());
  jetEnergyScaleL2L3Hist_ = (TH2D*)f_jecjer_->Get(jetEnergyScaleL2L3HistName_.c_str());
  jetEnergyScaleUncHist_ = (TH2D*)f_jecjer_->Get(jetEnergyScaleUncHistName_.c_str());
  jetEnergyResSFNomHist_ = (TH2D*)f_jecjer_->Get(jetEnergyResSFNomHistName_.c_str());
  jetEnergyResSFUpHist_ = (TH2D*)f_jecjer_->Get(jetEnergyResSFUpHistName_.c_str());
  jetEnergyResSFDownHist_ = (TH2D*)f_jecjer_->Get(jetEnergyResSFDownHistName_.c_str());
  jetEnergyResPtResHist_ = (TH3D*)f_jecjer_->Get(jetEnergyResPtResHistName_.c_str());

  jetEnergyScaleMCL2RelativeHist_->SetDirectory(0);
  jetEnergyScaleMCL3AbsoluteHist_->SetDirectory(0);
  jetEnergyScaleDATAL2RelativeHist_->SetDirectory(0);
  jetEnergyScaleDATAL3AbsoluteHist_->SetDirectory(0);
  jetEnergyScaleL2L3Hist_->SetDirectory(0);
  jetEnergyScaleUncHist_->SetDirectory(0);
  jetEnergyResSFNomHist_->SetDirectory(0);
  jetEnergyResSFUpHist_->SetDirectory(0);
  jetEnergyResSFDownHist_->SetDirectory(0);
  jetEnergyResPtResHist_->SetDirectory(0);
  f_jecjer_->Close();
  delete f_jecjer_;

#if DATA_FORMAT_FROM_MINIAOD
  rng_ = new TRandom3(0);
#endif
}

template<class T> OSUGenericJetProducer<T>::~OSUGenericJetProducer ()
{
  delete jetEnergyScaleMCL2RelativeHist_;
  delete jetEnergyScaleMCL3AbsoluteHist_;
  delete jetEnergyScaleDATAL2RelativeHist_;
  delete jetEnergyScaleDATAL3AbsoluteHist_;
  delete jetEnergyScaleL2L3Hist_;
  delete jetEnergyScaleUncHist_;
  delete jetEnergyResSFNomHist_;
  delete jetEnergyResSFUpHist_;
  delete jetEnergyResSFDownHist_;
  delete jetEnergyResPtResHist_;
#if DATA_FORMAT_FROM_MINIAOD
  delete rng_;
#endif
}

template<class T> void
OSUGenericJetProducer<T>::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(jets)> > jets;
  if (!event.getByToken(jetsToken_, jets))
    return;

#ifndef STOPPPED_PTLS
  edm::Handle<vector<osu::Mcparticle> > particles;
  event.getByToken(mcparticleToken_, particles);

#if DATA_FORMAT_FROM_MINIAOD
  edm::Handle<vector<TYPE(genjets)>> genjets;
  edm::Handle<double> rho;
  edm::Handle<vector<TYPE (electrons)>> electrons;
  edm::Handle<vector<TYPE (muons)>> muons;
  edm::Handle<vector<TYPE (primaryvertexs)>> primaryvertexs;

  if (!getEventHandles(event, genjets, rho, electrons, muons, primaryvertexs))
    return;

  bool hasGenJets = event.getByToken (genjetsToken_, genjets);

  vector<const TYPE(electrons) *> goodElectrons;
  vector<const TYPE(muons) *> goodMuons;
  buildGoodLeptonCollections(electrons, muons, goodElectrons, goodMuons);

#endif // DATA_FORMAT_FROM_MINIAOD
#endif // not STOPPPED_PTLS

  outputJets_ = unique_ptr<vector<T>>(new vector<T>());

  for (const auto &object : *jets)
  {
#ifndef STOPPPED_PTLS
      outputJets_->emplace_back(object, particles, cfg_);
#else
      outputJets_->emplace_back(object);
#endif

#if DATA_FORMAT_FROM_MINIAOD
      T &jet = outputJets_->back();

      calculateMedianIPSig(jet);
      calculateAlphaMax(jet, primaryvertexs);
      applyBTagDiscriminators(jet);
      applyJetEnergyCorrections(jet, event);
      setJERScaleFactors(jet, rho);

      // https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution#Smearing_procedures for Run 2
      // https://cms-jerc.web.cern.ch/JER/ for Run 3
      if(hasGenJets)
      {
        TYPE(genjets) matchedJet;
        if(tryGetMatchedGenJet(jet, genjets, matchedJet))
        {
          smearJetPtMatched(jet, matchedJet);
        }
        else
        {
          smearJetPtUnmatched(jet);
        }
      }
      else {
        jet.set_smearedPt(jet.pt());
        jet.set_smearedPtUp(jet.pt());
        jet.set_smearedPtDown(jet.pt());
      }

      checkJetLeptonMatching(jet, goodElectrons, goodMuons);

#endif
  }

  event.put(std::move(outputJets_), jets_.instance());
  outputJets_.reset();
}

#ifndef STOPPPED_PTLS

#if DATA_FORMAT_FROM_MINIAOD

template<class T>
bool OSUGenericJetProducer<T>::getEventHandles(
  edm::Event &event,
  edm::Handle<vector<TYPE(genjets)>> &genjets,
  edm::Handle<double> &rho,
  edm::Handle<vector<TYPE(electrons)>> &electrons,
  edm::Handle<vector<TYPE(muons)>> &muons,
  edm::Handle<vector<TYPE(primaryvertexs)>> &primaryvertexs
)
{
  event.getByToken(genjetsToken_, genjets);
  event.getByToken(rhoToken_, rho);

  if (!event.getByToken(electronToken_, electrons))
    return false;

  if (!event.getByToken(muonToken_, muons))
    return false;

  if (!event.getByToken(primaryvertexsToken_, primaryvertexs))
    return false;

  return true;
}

template<class T>
void OSUGenericJetProducer<T>::buildGoodLeptonCollections(
  edm::Handle<vector<TYPE(electrons)>> electrons,
  edm::Handle<vector<TYPE(muons)>> muons,
  vector<const TYPE(electrons)*> &goodElectrons,
  vector<const TYPE(muons)*> &goodMuons
)
{
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
}

template<class T>
void OSUGenericJetProducer<T>::applyBTagDiscriminators(T &jet)
{
  jet.set_pfCombinedInclusiveSecondaryVertexV2BJetTags(jet.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));
  jet.set_pfCombinedSecondaryVertexV2BJetTags(jet.bDiscriminator("pfCombinedSecondaryVertexV2BJetTags"));
}

template<class T>
void OSUGenericJetProducer<T>::checkJetLeptonMatching(
  T &jet,
  vector<const TYPE(electrons)*> &goodElectrons,
  vector<const TYPE(muons)*> &goodMuons
)
{
  double maxDeltaR_ = 0.4;

  int isMatchedToElectron = 0;
  for (const auto &electron : goodElectrons){
    double dR = deltaR(*electron, jet);
    if (dR < maxDeltaR_){
      isMatchedToElectron = true;
      break;
    }
  }

  bool isMatchedToMuon = false;
  for (const auto &muon : goodMuons){
    double dR = deltaR(*muon, jet);
    if (dR < maxDeltaR_){
      isMatchedToMuon = true;
      break;
    }
  }

  if (isMatchedToElectron == 1 || isMatchedToMuon == 1)
    jet.set_matchedToLepton(1);
  else
    jet.set_matchedToLepton(0);
}

template<class T>
void OSUGenericJetProducer<T>::applyJetEnergyCorrections(T &jet, edm::Event &event)
{
  Double_t JECMCL2Relative = jetEnergyScaleMCL2RelativeHist_->GetBinContent(jetEnergyScaleMCL2RelativeHist_->FindBin(jet.pt(), jet.eta()));
  Double_t JECMCL3Absolute = jetEnergyScaleMCL3AbsoluteHist_->GetBinContent(jetEnergyScaleMCL3AbsoluteHist_->FindBin(jet.pt(), jet.eta()));
  Double_t JECDATAL2Relative = jetEnergyScaleDATAL2RelativeHist_->GetBinContent(jetEnergyScaleDATAL2RelativeHist_->FindBin(jet.pt(), jet.eta()));
  Double_t JECDATAL3Absolute = jetEnergyScaleDATAL3AbsoluteHist_->GetBinContent(jetEnergyScaleDATAL3AbsoluteHist_->FindBin(jet.pt(), jet.eta()));
  Double_t JECDATAL2L3 = jetEnergyScaleL2L3Hist_->GetBinContent(jetEnergyScaleL2L3Hist_->FindBin(jet.pt(), jet.eta()));
  Double_t JECUnc = jetEnergyScaleUncHist_->GetBinContent(jetEnergyScaleUncHist_->FindBin(jet.pt(), jet.eta()));

  if (event.isRealData()) jet.scaleEnergy(JECDATAL2Relative * JECDATAL3Absolute * JECDATAL2L3);
  else jet.scaleEnergy(JECMCL2Relative * JECMCL3Absolute);
  jet.set_jecUncertainty(JECUnc);
}

template<class T>
void OSUGenericJetProducer<T>::setJERScaleFactors(T &jet, edm::Handle<double> rho)
{
  Double_t JERSFNom = jetEnergyResSFNomHist_->GetBinContent(jetEnergyResSFNomHist_->FindBin(jet.pt(), jet.eta()));
  Double_t JERSFUp = jetEnergyResSFUpHist_->GetBinContent(jetEnergyResSFUpHist_->FindBin(jet.pt(), jet.eta()));
  Double_t JERSFDown = jetEnergyResSFDownHist_->GetBinContent(jetEnergyResSFDownHist_->FindBin(jet.pt(), jet.eta()));
  Double_t JERPtRes = jetEnergyResPtResHist_->GetBinContent(jetEnergyResPtResHist_->FindBin(jet.pt(), jet.eta(), (float)(*rho)));

  jet.set_jetPtResolution(JERPtRes);
  jet.set_setJetPtResolutionSF(JERSFNom, JERSFUp, JERSFDown);
}

template<class T>
void OSUGenericJetProducer<T>::calculateMedianIPSig(T &jet)
{
  std::vector<double> ipsigVector;

  for(unsigned id = 0; id < jet.getJetConstituents().size(); id++) {

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
      edm::LogWarning("OSUGenericJetProducer (medianlog10ipsig)") << e.what();
    }
  }

  double medianipsig = -4;

  if(!ipsigVector.empty()) {
    std::sort(ipsigVector.begin(), ipsigVector.end());
    if(ipsigVector.size()%2 == 0) medianipsig = (ipsigVector[ipsigVector.size()/2 - 1] + ipsigVector[ipsigVector.size()/2])/2;
    if(ipsigVector.size()%2 == 1) medianipsig = ipsigVector[ipsigVector.size()/2];
  }

  if(!ipsigVector.empty()) jet.set_medianlog10ipsig(log10(medianipsig));
  else jet.set_medianlog10ipsig(-4);
}

template<class T>
void OSUGenericJetProducer<T>::calculateAlphaMax(T &jet, edm::Handle<vector<TYPE(primaryvertexs)>> primaryvertexs)
{
  double numerator = 0;
  double denominator = 0;

  double alpha = 0;
  double alphaMax = 0;

  for(unsigned vertex = 0; vertex < primaryvertexs.product()->size(); vertex++) {

    for(unsigned id = 0; id < jet.getJetConstituents().size(); id++) {

      const pat::PackedCandidate &packedCand = dynamic_cast<const pat::PackedCandidate &>(*jet.getJetConstituents().at(id));
      const edm::Ptr<reco::Candidate> recoCand = jet.getJetConstituents().at(id);

      try {
        if(packedCand.charge() != 0) {

          double candPT = recoCand->pt();

          if(packedCand.fromPV(vertex) > 1){
            numerator += candPT;
          }
          denominator += candPT;
        }
      }
      catch (cms::Exception &e) {
        edm::LogWarning("OSUGenericJetProducer (alphamax)") << e.what();
      }
    }

    if (denominator != 0) alpha = (numerator/denominator);
    if (alpha > alphaMax) alphaMax = alpha;

  }

  if(denominator != 0) jet.set_alphamax(alphaMax);
  else jet.set_alphamax(-1);
}

template<class T>
bool OSUGenericJetProducer<T>::tryGetMatchedGenJet(T &jet, edm::Handle<vector<TYPE(genjets)>> genjets, TYPE(genjets) &matchedGenJet)
{
  for (const auto &genjet : *genjets) {
    double dR = deltaR(genjet, jet);
    double dPt = jet.pt() - genjet.pt();

    if(jetResNewPrescription_) {
      if(dR < 0.2 && fabs(dPt) < 3.0 * jet.pt() * jet.jer()) {
        matchedGenJet = genjet;
        return true;
      }
    }
    else {
      if(dR < 0.2 && fabs(dPt) < 3.0 * jet.jer()) {
        matchedGenJet = genjet;
        return true;
      }
    }
  }
  return false;
}

template<class T>
void OSUGenericJetProducer<T>::smearJetPtMatched(T &jet, TYPE(genjets) genjet)
{
  double dPt = jet.pt() - genjet.pt();

  if(jetResNewPrescription_) {
    // https://github.com/cms-sw/cmssw/blob/CMSSW_8_0_25/PhysicsTools/PatUtils/interface/SmearedJetProducerT.h#L237 for Run 2
    // https://github.com/cms-sw/cmssw/blob/CMSSW_13_0_13/PhysicsTools/PatUtils/interface/SmearedJetProducerT.h#L260 for Run 3
    jet.set_smearedPt(    max(0.0, jet.pt() + (jet.jerSF()     - 1.) * dPt));
    jet.set_smearedPtUp(  max(0.0, jet.pt() + (jet.jerSFUp()   - 1.) * dPt));
    jet.set_smearedPtDown(max(0.0, jet.pt() + (jet.jerSFDown() - 1.) * dPt));
  }
  else {
    jet.set_smearedPt(    max(0.0, genjet.pt() + jet.jerSF()     * dPt));
    jet.set_smearedPtUp(  max(0.0, genjet.pt() + jet.jerSFUp()   * dPt));
    jet.set_smearedPtDown(max(0.0, genjet.pt() + jet.jerSFDown() * dPt));
  }
}

template<class T>
void OSUGenericJetProducer<T>::smearJetPtUnmatched(T &jet)
{
  if(jet.jerSF() > 1.0) {
    if(jetResNewPrescription_) {
      double smearedPt = rng_->Gaus(0.0, jet.jer()) * sqrt(max(jet.jerSF() * jet.jerSF() - 1.0, 0.0));
      smearedPt = jet.pt() * (1.0 + smearedPt);
      jet.set_smearedPt(    smearedPt);
      jet.set_smearedPtUp(  smearedPt * jet.jerSFUp() / jet.jerSF());
      jet.set_smearedPtDown(smearedPt * jet.jerSFDown() / jet.jerSF());
    }
    else {
      double smearedPt = rng_->Gaus(jet.pt(),
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
}

#endif // DATA_FORMAT_FROM_MINIAOD

#endif // STOPPPED_PTLS

#include "FWCore/Framework/interface/MakerMacros.h"
typedef OSUGenericJetProducer<osu::Jet> OSUJetProducer;
DEFINE_FWK_MODULE(OSUJetProducer);

#if IS_VALID(bjets)
typedef OSUGenericJetProducer<osu::Bjet> OSUBjetProducer;
DEFINE_FWK_MODULE(OSUBjetProducer);
#endif // IS_VALID(bjets)

#endif // IS_VALID(jets)
