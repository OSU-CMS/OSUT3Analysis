#include <vector>

#include "OSUT3Analysis/AnaTools/plugins/ISRWeightProducer.h"

ISRWeightProducer::ISRWeightProducer (const edm::ParameterSet &cfg) :
  EventVariableProducer(cfg),
  pdgIds_     (cfg.getParameter<vector<int> > ("pdgIds")),
  weightFile_ (cfg.getParameter<string> ("weightFile")),
  weightHist_ (cfg.getParameter<string> ("weightHist")),
  weights_    (NULL)
{
  mcparticlesToken_ = consumes<vector<TYPE(hardInteractionMcparticles)> > (collections_.getParameter<edm::InputTag> ("hardInteractionMcparticles"));
}

ISRWeightProducer::~ISRWeightProducer() {
  if (weights_)
    delete weights_;
}

void
ISRWeightProducer::AddVariables (const edm::Event &event) {

#if DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_2017 || DATA_FORMAT == MINI_AOD_2017 || DATA_FORMAT == AOD
  if(event.isRealData()) {
    (*eventvariables)["isrWeight"] = 1;
    return;
  }

  edm::Handle<vector<TYPE(hardInteractionMcparticles)> > mcparticles;
  if (!event.getByToken(mcparticlesToken_, mcparticles)) {
    (*eventvariables)["isrWeight"] = 1;
    return;
  }

  if (!weights_)
    {
      TFile * fin = TFile::Open(weightFile_.c_str());
      if(!fin || fin->IsZombie()) {
        clog << "ERROR [ISRWeightProducer]: Could not find file: " << weightFile_
             << "; would cause a seg fault." << endl;
        exit(1);
      }

      weights_ = (TH1D*)fin->Get(weightHist_.c_str());
      if(!weights_) {
        clog << "ERROR [ISRWeightProducer]: Could not find histogram: " << weightHist_
             << "; would cause a seg fault." << endl;
        exit(1);
      }
      weights_->SetDirectory(0);
      fin->Close ();
      delete fin;
    }

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

  (*eventvariables)["isrWeight"] = weights_->GetBinContent(weights_->FindBin(pt));

#else
  (*eventvariables)["isrWeight"] = 1;
#endif
}

bool
ISRWeightProducer::isOriginalParticle (const TYPE(hardInteractionMcparticles) &mcparticle, const int pdgId) const
{
#if DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_2017 || DATA_FORMAT == AOD
  if(!mcparticle.numberOfMothers () || mcparticle.motherRef ().isNull ()) return true;
  return(mcparticle.motherRef ()->pdgId () != pdgId) && isOriginalParticle ((TYPE(hardInteractionMcparticles)) *mcparticle.motherRef (), pdgId);
#else
  return false;
#endif
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(ISRWeightProducer);
