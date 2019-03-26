#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"
#include "OSUT3Analysis/AnaTools/plugins/L1PrefiringWeightProducer.h"

L1PrefiringWeightProducer::L1PrefiringWeightProducer(const edm::ParameterSet &cfg) :
   EventVariableProducer(cfg)
{
  tokenPrefWeight_     = consumes< double >(edm::InputTag("prefiringweight:NonPrefiringProb"));
  tokenPrefWeightUp_   = consumes< double >(edm::InputTag("prefiringweight:NonPrefiringProbUp"));
  tokenPrefWeightDown_ = consumes< double >(edm::InputTag("prefiringweight:NonPrefiringProbDown"));

  tokenJets_ = consumes<vector<TYPE(jets)> >(collections_.getParameter<edm::InputTag>("jets"));
}

L1PrefiringWeightProducer::~L1PrefiringWeightProducer() {
}

void
L1PrefiringWeightProducer::AddVariables(const edm::Event &event) {
  double w = 1.0, wUp = 1.0, wDown = 1.0;
  bool hasPrefiredJets = false;

  if(!event.isRealData()) {
    edm::Handle<double> theprefweight;
    edm::Handle<double> theprefweightup;
    edm::Handle<double> theprefweightdown;

    event.getByToken(tokenPrefWeight_,     theprefweight);
    event.getByToken(tokenPrefWeightUp_,   theprefweightup);
    event.getByToken(tokenPrefWeightDown_, theprefweightdown);

    w     = (*theprefweight);
    wUp   = (*theprefweightup);
    wDown = (*theprefweightdown);

    edm::Handle<vector<TYPE(jets)> > jets;
    if (!event.getByToken(tokenJets_, jets)) {
      clog << "ERROR:  Could not find jets collection." << endl;
      return;
    }

    for(const auto &jet : *jets) {
      if(jet.pt() > 100.0 && fabs(jet.eta()) > 2.25 && fabs(jet.eta()) < 3.0) {
        hasPrefiredJets = true;
        break;
      }
    }

  }

  (*eventvariables)["L1ECALPrefiringWeight"]     = w;
  (*eventvariables)["L1ECALPrefiringWeightUp"]   = wUp;
  (*eventvariables)["L1ECALPrefiringWeightDown"] = wDown;

  (*eventvariables)["hasPrefiredJets"] = hasPrefiredJets;
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1PrefiringWeightProducer);
