#include <vector>

#include "OSUT3Analysis/AnaTools/plugins/ISRWeightProducer.h"

ISRWeightProducer::ISRWeightProducer (const edm::ParameterSet &cfg) :
  EventVariableProducer(cfg),
  pdgIds_     (cfg.getParameter<vector<int> > ("pdgIds")),
  weightFile_ (cfg.getParameter<string> ("weightFile")),
  weightHist_ (cfg.getParameter<string> ("weightHist"))
{
  mcparticlesToken_ = consumes<vector<TYPE(hardInteractionMcparticles)> > (collections_.getParameter<edm::InputTag> ("hardInteractionMcparticles"));
}

ISRWeightProducer::~ISRWeightProducer() {}

void
ISRWeightProducer::AddVariables (const edm::Event &event) {

#if DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == MINI_AOD || DATA_FORMAT == AOD
  if(event.isRealData()) {
    (*eventvariables)["isrWeight"] = 1;
    return;
  }

  edm::Handle<vector<TYPE(hardInteractionMcparticles)> > mcparticles;
  if (!event.getByToken(mcparticlesToken_, mcparticles)) {
    (*eventvariables)["isrWeight"] = 1;
    return;
  }

  TFile * fin = TFile::Open(weightFile_.c_str());
  if(!fin || fin->IsZombie()) {
    clog << "ERROR [ISRWeightProducer]: Could not find file: " << weightFile_
         << "; would cause a seg fault." << endl;
    exit(1);
  }

  TH1D * weights = (TH1D*)fin->Get(weightHist_.c_str());
  if(!weights) {
    clog << "ERROR [ISRWeightProducer]: Could not find histogram: " << weightHist_
         << "; would cause a seg fault." << endl;
    exit(1);
  }
  weights->SetDirectory(0);
  delete fin;

  double px = 0.0;
  double py = 0.0;

  for(const auto &mcparticle : *mcparticles) {
    for(const auto &pdgId : pdgIds_) {
      if(abs(mcparticle.pdgId()) == abs(pdgId) && isOriginalParticle(mcparticle, mcparticle.pdgId())) {
        px += mcparticle.px();
        py += mcparticle.py();
      }
    }
  }

  double pt = sqrt(px*px + py*py);

  (*eventvariables)["isrWeight"] = weights->GetBinContent(weights->FindBin(pt));

  delete weights;

#else
  (*eventvariables)["isrWeight"] = 1;
#endif
}

bool
ISRWeightProducer::isOriginalParticle (const TYPE(hardInteractionMcparticles) &mcparticle, const int pdgId) const
{
#if DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == MINI_AOD || DATA_FORMAT == AOD
  if(!mcparticle.numberOfMothers () || mcparticle.motherRef ().isNull ()) return true;
  return(mcparticle.motherRef ()->pdgId () != pdgId) && isOriginalParticle ((TYPE(hardInteractionMcparticles)) *mcparticle.motherRef (), pdgId);
#else
  return false;
#endif
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(ISRWeightProducer);
