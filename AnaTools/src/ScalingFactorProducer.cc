#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"
#include "OSUT3Analysis/AnaTools/interface/ScalingFactorProducer.h"

ScalingFactorProducer::ScalingFactorProducer(const edm::ParameterSet &cfg) :
   EventVariableProducer(cfg),
   PU_               (cfg.getParameter<string>("PU")),
   dataset_          (cfg.getParameter<string>("dataset"))
{
}

ScalingFactorProducer::~ScalingFactorProducer() {}

void
ScalingFactorProducer::AddVariables (const edm::Event &event) {
#if DATA_FORMAT == MINI_AOD_CUSTOM
  TFile *fin = TFile::Open (PU_.c_str ());
  if (!fin || fin->IsZombie()) {
    clog << "ERROR [ScalingFactorProducer]: Could not find file: " << PU_
	 << "; will cause a seg fault." << endl;
    exit(1);
  }  
  TH1D *mc;
  TH1D *puWeight;
  fin->GetObject(dataset_.c_str(), mc);  
  fin->GetObject("MuonEG_2015D", puWeight);
  if (!mc) {
    clog << "ERROR [ScalingFactorProducer]: Could not find histogram: " << dataset_ << "; will cause a seg fault." << endl;
    exit(1);
  }
  if (!puWeight) {
    clog << "ERROR [ScalingFactorProducer]: Could not find histogram: MuonEG_2015D, will cause a seg fault." << endl;
    exit(1);
  }
  mc->SetDirectory (0);
  puWeight->SetDirectory (0);
  mc->Scale (puWeight->Integral () / mc->Integral ());
  TH1D *trimmedMC = new TH1D ("bla", "bla", puWeight->GetNbinsX(), 0, puWeight->GetNbinsX());
  for (int bin = 1; bin <= puWeight->GetNbinsX(); bin++)
    trimmedMC->SetBinContent (bin, mc->GetBinContent (bin));
  puWeight->Divide (trimmedMC);
  delete fin;
  delete mc;
  delete trimmedMC;
  objectsToGet_.insert ("pileupinfos");
  getOriginalCollections (objectsToGet_, collections_, handles_, event);
  double numTruePV = 0;
 for (const auto &pv1 : *handles_.pileupinfos) {
    if(pv1.getBunchCrossing() == 0)
      numTruePV = pv1.getTrueNumInteractions();
  }
  (*eventvariables)["puScalingFactor"] = puWeight->GetBinContent(puWeight->FindBin(numTruePV));
  delete puWeight; 
 # endif
 }  

void
ScalingFactorProducer::getOriginalCollections (const unordered_set<string> &objectsToGet, const edm::ParameterSet &collections, OriginalCollections &handles, const edm::Event &event)
{

  //////////////////////////////////////////////////////////////////////////////
  // Retrieve each object collection which we need and print a warning if it is
  // missing.
  //////////////////////////////////////////////////////////////////////////////
  if  (VEC_CONTAINS  (objectsToGet,  "beamspots")         &&  collections.exists  ("beamspots"))         anatools::getCollection  (collections.getParameter<edm::InputTag>  ("beamspots"),         handles.beamspots,         event);
  if  (VEC_CONTAINS  (objectsToGet,  "bxlumis")           &&  collections.exists  ("bxlumis"))           anatools::getCollection  (collections.getParameter<edm::InputTag>  ("bxlumis"),           handles.bxlumis,           event);
  if  (VEC_CONTAINS  (objectsToGet,  "electrons")         &&  collections.exists  ("electrons"))         anatools::getCollection  (collections.getParameter<edm::InputTag>  ("electrons"),         handles.electrons,         event);
  if  (VEC_CONTAINS  (objectsToGet,  "events")            &&  collections.exists  ("events"))            anatools::getCollection  (collections.getParameter<edm::InputTag>  ("events"),            handles.events,            event);
  if  (VEC_CONTAINS  (objectsToGet,  "genjets")           &&  collections.exists  ("genjets"))           anatools::getCollection  (collections.getParameter<edm::InputTag>  ("genjets"),           handles.genjets,           event);
  if  (VEC_CONTAINS  (objectsToGet,  "jets")              &&  collections.exists  ("jets"))              anatools::getCollection  (collections.getParameter<edm::InputTag>  ("jets"),              handles.jets,              event);
  if  (VEC_CONTAINS  (objectsToGet,  "basicjets")         &&  collections.exists  ("basicjets"))         anatools::getCollection  (collections.getParameter<edm::InputTag>  ("basicjets"),         handles.basicjets,         event);
  if  (VEC_CONTAINS  (objectsToGet,  "candjets")          &&  collections.exists  ("candjets"))          anatools::getCollection  (collections.getParameter<edm::InputTag>  ("candjets"),          handles.candjets,          event);
  if  (VEC_CONTAINS  (objectsToGet,  "candeles")          &&  collections.exists  ("candeles"))          anatools::getCollection  (collections.getParameter<edm::InputTag>  ("candeles"),          handles.candeles,          event);
  if  (VEC_CONTAINS  (objectsToGet,  "generatorweights")  &&  collections.exists  ("generatorweights"))  anatools::getCollection  (collections.getParameter<edm::InputTag>  ("generatorweights"),  handles.generatorweights,  event);
  if  (VEC_CONTAINS  (objectsToGet,  "mcparticles")       &&  collections.exists  ("mcparticles"))       anatools::getCollection  (collections.getParameter<edm::InputTag>  ("mcparticles"),       handles.mcparticles,       event);
  if  (VEC_CONTAINS  (objectsToGet,  "mets")              &&  collections.exists  ("mets"))              anatools::getCollection  (collections.getParameter<edm::InputTag>  ("mets"),              handles.mets,              event);
  if  (VEC_CONTAINS  (objectsToGet,  "muons")             &&  collections.exists  ("muons"))             anatools::getCollection  (collections.getParameter<edm::InputTag>  ("muons"),             handles.muons,             event);
  if  (VEC_CONTAINS  (objectsToGet,  "photons")           &&  collections.exists  ("photons"))           anatools::getCollection  (collections.getParameter<edm::InputTag>  ("photons"),           handles.photons,           event);
  if  (VEC_CONTAINS  (objectsToGet,  "prescales")         &&  collections.exists  ("prescales"))         anatools::getCollection  (collections.getParameter<edm::InputTag>  ("prescales"),         handles.prescales,         event);
  if  (VEC_CONTAINS  (objectsToGet,  "primaryvertexs")    &&  collections.exists  ("primaryvertexs"))    anatools::getCollection  (collections.getParameter<edm::InputTag>  ("primaryvertexs"),    handles.primaryvertexs,    event);
  if  (VEC_CONTAINS  (objectsToGet,  "superclusters")     &&  collections.exists  ("superclusters"))     anatools::getCollection  (collections.getParameter<edm::InputTag>  ("superclusters"),     handles.superclusters,     event);
  if  (VEC_CONTAINS  (objectsToGet,  "taus")              &&  collections.exists  ("taus"))              anatools::getCollection  (collections.getParameter<edm::InputTag>  ("taus"),              handles.taus,              event);
  if  (VEC_CONTAINS  (objectsToGet,  "tracks")            &&  collections.exists  ("tracks"))            anatools::getCollection  (collections.getParameter<edm::InputTag>  ("tracks"),            handles.tracks,            event);
  if  (VEC_CONTAINS  (objectsToGet,  "triggers")          &&  collections.exists  ("triggers"))          anatools::getCollection  (collections.getParameter<edm::InputTag>  ("triggers"),          handles.triggers,          event);
  if  (VEC_CONTAINS  (objectsToGet,  "trigobjs")          &&  collections.exists  ("trigobjs"))          anatools::getCollection  (collections.getParameter<edm::InputTag>  ("trigobjs"),          handles.trigobjs,          event);
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(ScalingFactorProducer);
