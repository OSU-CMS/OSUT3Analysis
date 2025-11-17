#include "OSUT3Analysis/Collections/plugins/OSUGenericJetProducer.h"
#include "DataFormats/Math/interface/deltaR.h"

#if IS_VALID(jets)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"


template<class T>
OSUGenericJetProducer<T>::OSUGenericJetProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  rho_         (cfg.getParameter<edm::InputTag>  ("rho")),
  year_(cfg.getParameter<string>("year")),
  dataEra_ (cfg.getParameter<string> ("eraTag")),
  isData_(cfg.getParameter<bool>("isData")),
  jecConfigFile_(cfg.getParameter<edm::FileInPath>("jecConfigFile")),
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

  JecConfigReader::ConfigPaths paths;
  paths.ak4 = jecConfigFile_.fullPath();
  JecConfigReader::setConfigPaths(paths);

  const auto jecTags = JecConfigReader::getTagsAK4(year_, isData_, dataEra_);
  jecRefs_ = JecConfigReader::CorrectionRefs(jecTags);

  // The total JES uncertainty tag has a name specific to the year/era,
  // i.e. Summer19UL16APV_V7_MC_Total_AK4PFchs.
  // We can get that tag without needing to build it from scratch via
  // the following procedure.
  const auto& cfgAK4 = JecConfigReader::getCfgAK4();
  JecConfigReader::SystSetMapJES systTagNames = JecConfigReader::getSystTagNames(cfgAK4, year_);
  jesUncTag_ = systTagNames["ForUncertaintyJESTotal"][0].first;
}

template<class T> void
OSUGenericJetProducer<T>::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(jets)>> jets;
  if (!event.getByToken(jetsToken_, jets))
    return;

#ifndef STOPPPED_PTLS
  edm::Handle<vector<osu::Mcparticle> > particles;
  event.getByToken(mcparticleToken_, particles);

#if DATA_FORMAT_FROM_MINIAOD
  edm::Handle<vector<TYPE(genjets)>> genjets;
  edm::Handle<double> rho;
  edm::Handle<vector<TYPE(electrons)>> electrons;
  edm::Handle<vector<TYPE(muons)>> muons;
  edm::Handle<vector<TYPE(primaryvertexs)>> primaryvertexs;

  if (!getEventHandles(event, genjets, rho, electrons, muons, primaryvertexs))
    return;

  vector<const TYPE(electrons) *> goodElectrons;
  vector<const TYPE(muons) *> goodMuons;
  buildGoodLeptonCollections(electrons, muons, goodElectrons, goodMuons);

  JecApplication::EvalContext jecCtx{
    .year = year_,
    .refs = jecRefs_,
    .isData = isData_,
    .runNumber = std::nullopt,
    .isDebug = false
  };
  if (isData_ && JecApplication::requiresRunBasedResidual(year_))
  {
    jecCtx.runNumber = static_cast<double>(event.id().run());
  }

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

    applyJetEnergyCorrections(jecCtx, jet, event);
    if (!isData_) applySmearedJetEnergy(jecCtx, jet, event);

    checkJetLeptonMatching(jet, goodElectrons, goodMuons);
#endif
  }

  event.put(std::move(outputJets_), jets_.instance());
  outputJets_.reset();
}

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
void OSUGenericJetProducer<T>::applyJetEnergyCorrections(JecApplication::EvalContext ctx, T &jet, edm::Event &event)
{
  edm::Handle<double> hRho;
  event.getByToken(rhoToken_, hRho);
  const double rho = hRho.isValid() ? *hRho : 0.0;

  const double rawFactor = computeRawFactorFromMiniAOD(jet);
  JecApplication::JesInputs inputs{
    jet.pt(),
    jet.eta(),
    jet.phi(),
    jet.jetArea(),
    rho,
    rawFactor
  };

  const double jesFactor = JecApplication::getJesCorrectionNom(ctx, inputs);
  setP4Scaled(jet, jesFactor);
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
double OSUGenericJetProducer<T>::getJercFactor(
  JecApplication::EvalContext ctx,
  T &jet,
  edm::Event & event,
  JecConfigReader::SystKind systKind,
  std::string systVar
)
{
  edm::Handle<double> hRho;
  event.getByToken(rhoToken_, hRho);
  const double rho = hRho.isValid() ? *hRho : 0.0;

  double jesSystFactor = 1.0;
  if (systKind == JecConfigReader::SystKind::JES)
  {
    jesSystFactor = JecApplication::getJesCorrectionSyst(jecRefs_, jesUncTag_, systVar, jet.eta(), jet.pt(), false);
  }

  // Use pt after JES systematic corrections as input to JER corrections
  JecApplication::JesInputs jesInputs{
    jesSystFactor * jet.pt(),
    jet.eta(),
    jet.phi(),
    jet.jetArea(),
    rho,
    0.0
  };

  JecApplication::JerInputs jerInputs{};
  jerInputs.event = event.id().event();
  jerInputs.run = event.id().run();
  jerInputs.lumi = event.id().luminosityBlock();

  if (const auto* gj = jet.genJet())
  {
    jerInputs.hasGen = true;
    jerInputs.genPt = gj->pt();
    jerInputs.genEta = gj->eta();
    jerInputs.genPhi = gj->phi();
    jerInputs.maxDr = 0.2;
  }

  JecApplication::SystematicOptions options{};
  options.jerVar = (systKind == JecConfigReader::SystKind::JER) ? systVar : "nom"; // Use nominal when doing JES syst

  double jerFactor = JecApplication::getJerCorrectionNomOrSyst(ctx, jesInputs, jerInputs, options);
  return jerFactor * jesSystFactor;
}

template<class T>
void OSUGenericJetProducer<T>::applySmearedJetEnergy(JecApplication::EvalContext ctx, T &jet, edm::Event &event)
{
  double jerFactor = getJercFactor(ctx, jet, event, JecConfigReader::SystKind::Nominal, "");

  // JecApplication expects capital Up/Down for JES
  double jesSystUpFactor = getJercFactor(ctx, jet, event, JecConfigReader::SystKind::JES, "Up");
  double jesSystDownFactor = getJercFactor(ctx, jet, event, JecConfigReader::SystKind::JES, "Down");

  // JecApplication expects lowercase up/down for JER
  double jerSystUpFactor = getJercFactor(ctx, jet, event, JecConfigReader::SystKind::JER, "up");
  double jerSystDownFactor = getJercFactor(ctx, jet, event, JecConfigReader::SystKind::JER, "down");

  jet.set_smearedPt(jerFactor * jet.pt());
  jet.set_smearedPtJesSystUp(jesSystUpFactor * jet.pt());
  jet.set_smearedPtJesSystDown(jesSystDownFactor * jet.pt());
  jet.set_smearedPtJerSystUp(jerSystUpFactor * jet.pt());
  jet.set_smearedPtJerSystDown(jerSystDownFactor * jet.pt());

  /* TODO: do we still need this?
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
  */
}

template<class T>
double OSUGenericJetProducer<T>::computeRawFactorFromMiniAOD(const T& jet)
{
  const double ptCorr = jet.pt();
  const double ptRaw = jet.correctedP4("Uncorrected").pt();
  if (ptCorr <= 0.0) return 0.0;
  const double rf = 1.0 - (ptRaw / ptCorr);
  return std::min(std::max(rf, 0.0), 1.0);
}

template<class T>
void OSUGenericJetProducer<T>::setP4Scaled(T& jet, double scale)
{
  const auto p4 = jet.p4();
  const auto p4Scaled = reco::Particle::LorentzVector(
    p4.px() * scale, p4.py() * scale, p4.pz() * scale, p4.energy() * scale);
  jet.setP4(p4Scaled);
}

#include "FWCore/Framework/interface/MakerMacros.h"
typedef OSUGenericJetProducer<osu::Jet> OSUJetProducer;
DEFINE_FWK_MODULE(OSUJetProducer);

#if IS_VALID(bjets)
typedef OSUGenericJetProducer<osu::Bjet> OSUBjetProducer;
DEFINE_FWK_MODULE(OSUBjetProducer);
#endif // IS_VALID(bjets)

#endif // IS_VALID(jets)
