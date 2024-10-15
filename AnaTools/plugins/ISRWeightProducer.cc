#include <vector>

#include "OSUT3Analysis/AnaTools/plugins/ISRWeightProducer.h"

ISRWeightProducer::ISRWeightProducer (const edm::ParameterSet &cfg) :
  EventVariableProducer(cfg),
  pdgIds_                  (cfg.getParameter<vector<int> >    ("pdgIds")),
  motherIdsToReject_       (cfg.getParameter<vector<int> >    ("motherIdsToReject")),
  requireLastNotFirstCopy_ (cfg.getParameter<bool>            ("requireLastNotFirstCopy")),
  weightFile_              (cfg.getParameter<edm::FileInPath>          ("weightFile").fullPath()),
  weightHist_              (cfg.getParameter<vector<string> > ("weightHist")),
  weights_                 ({})
{
  mcparticlesToken_ = consumes<vector<TYPE(hardInteractionMcparticles)> > (collections_.getParameter<edm::InputTag> ("hardInteractionMcparticles"));
}

ISRWeightProducer::~ISRWeightProducer() {
  for (const auto &weight : weights_)
    if (weight)
      delete weight;
}

void
ISRWeightProducer::AddVariables (const edm::Event &event, const edm::EventSetup &setup) {

#ifndef STOPPPED_PTLS
  if(event.isRealData() || weightFile_.empty () || weightHist_.empty ()) {
    (*eventvariables)["isrPt"] = -1;
    (*eventvariables)["isrWeight"] = 1;
    (*eventvariables)["isrWeightUp"] = 1;
    (*eventvariables)["isrWeightDown"] = 1;
    return;
  }

  edm::Handle<vector<TYPE(hardInteractionMcparticles)> > mcparticles;
  if (!event.getByToken(mcparticlesToken_, mcparticles)) {
    (*eventvariables)["isrPt"] = -1;
    (*eventvariables)["isrWeight"] = 1;
    (*eventvariables)["isrWeightUp"] = 1;
    (*eventvariables)["isrWeightDown"] = 1;
    return;
  }

  if (weights_.empty ())
    {
      TFile * fin = TFile::Open(weightFile_.c_str());
      if(!fin || fin->IsZombie()) {
        clog << "ERROR [ISRWeightProducer]: Could not find file: " << weightFile_
             << "; would cause a seg fault." << endl;
        exit(1);
      }

      for (const auto &weightHist : weightHist_)
        {
          TH1D *w = (TH1D*)fin->Get(weightHist.c_str());
          if(!w) {
            clog << "ERROR [ISRWeightProducer]: Could not find histogram: " << weightHist
                 << "; would cause a seg fault." << endl;
            exit(1);
          }
          clog << "[ISRWeightProducer]: Producing ISR weights with histogram: " << weightHist << endl;
          w->SetDirectory(0);
          weights_.push_back (w);
        }
      fin->Close ();
      delete fin;
    }

  double px = 0.0;
  double py = 0.0;

  for(const auto &mcparticle : *mcparticles) {

    // Pythia8 first creates particles in the frame of the interaction, and only boosts them
    // for ISR recoil in later steps. So only the last copy is desired. A particle that's both
    // a last and first copy means it is a decay product created after the boost is applied,
    // e.g. a neutralino from a chargino decay, and we've already added the mother chargino.
    if(requireLastNotFirstCopy_) {
      if(!mcparticle.isLastCopy()) continue;
      if(mcparticle.statusFlags().isFirstCopy()) continue;
    }

    for(const auto &pdgId : pdgIds_) {
      if(mcparticle.numberOfMothers() &&
         !mcparticle.motherRef().isNull() &&
         find(motherIdsToReject_.begin(), motherIdsToReject_.end(), abs(mcparticle.motherRef()->pdgId())) != motherIdsToReject_.end()) {
        continue;
      }

      if(abs(mcparticle.pdgId()) == abs(pdgId) &&
         (requireLastNotFirstCopy_ || isOriginalParticle(mcparticle, mcparticle.pdgId()))) {
        px += mcparticle.px();
        py += mcparticle.py();
      }
    }

  }

  double pt = sqrt(px*px + py*py);
  double isrWeight = 1.0, isrWeightUp = 1.0, isrWeightDown = 1.0;

  for (const auto &weight : weights_) {
    double content = weight->GetBinContent (min (weight->FindBin (pt), weight->GetNbinsX ()));
    double error = weight->GetBinError (min (weight->FindBin (pt), weight->GetNbinsX ()));

    isrWeight *= content;
    isrWeightUp *= content + error;
    isrWeightDown *= max(content - error, 0.0); // max just in case error > content
  }

  (*eventvariables)["isrPt"] = pt;
  (*eventvariables)["isrWeight"] = isrWeight;
  (*eventvariables)["isrWeightUp"] = isrWeightUp;
  (*eventvariables)["isrWeightDown"] = isrWeightDown;

#else // if STOPPPED_PTLS
  (*eventvariables)["isrPt"] = -1;
  (*eventvariables)["isrWeight"] = 1;
  (*eventvariables)["isrWeightUp"] = 1;
  (*eventvariables)["isrWeightDown"] = 1;
#endif
}

// isOriginalParticle: particle is "original" if its mother has a different pdgId
bool
ISRWeightProducer::isOriginalParticle (const TYPE(hardInteractionMcparticles) &mcparticle, const int pdgId) const
{
#ifndef STOPPPED_PTLS
  if(!mcparticle.numberOfMothers () || mcparticle.motherRef ().isNull ()) return true;
  return (mcparticle.motherRef ()->pdgId () != pdgId);
#else
  return false;
#endif
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(ISRWeightProducer);
