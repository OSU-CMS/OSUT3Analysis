#include "OSUT3Analysis/AnaTools/interface/PUAnalyzer.h"

PUAnalyzer::PUAnalyzer (const edm::ParameterSet &cfg) :
  events_ (cfg.getParameter<edm::InputTag> ("events"))
{
  TH1::SetDefaultSumw2 ();

  masterCutFlow_ = new CutFlow (fs_);

  oneDHists_["pu"] = fs_->make<TH1D> ("pu",";pileup", 65, 0, 65);
}

PUAnalyzer::~PUAnalyzer ()
{
  delete masterCutFlow_;
}

void
PUAnalyzer::analyze (const edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<BNeventCollection> events;
  event.getByLabel (events_, events);

  masterCutFlow_->fillCutFlow ();

  oneDHists_["pu"]->Fill (events->at (0).numTruePV);
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PUAnalyzer);
