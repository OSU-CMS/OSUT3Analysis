#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"
#include "OSUT3Analysis/AnaTools/plugins/PUScalingFactorProducer.h"

PUScalingFactorProducer::PUScalingFactorProducer(const edm::ParameterSet &cfg) :
   EventVariableProducer(cfg),
   PU_               (cfg.getParameter<string>("PU")),
   dataset_          (cfg.getParameter<string>("dataset")),
   type_             (cfg.getParameter<string>("type"))
{
  if(type_.find("MC") < type_.length() && collections_.exists ("pileupinfos"))
    pileUpInfosToken_ = consumes<vector<TYPE(pileupinfos)> > (collections_.getParameter<edm::InputTag> ("pileupinfos"));
}

PUScalingFactorProducer::~PUScalingFactorProducer() {}

void
PUScalingFactorProducer::AddVariables (const edm::Event &event) {
#if DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == MINI_AOD
  TFile *fin = TFile::Open (PU_.c_str ());
  if (!fin || fin->IsZombie()) {
    clog << "ERROR [PUScalingFactorProducer]: Could not find file: " << PU_
	 << "; will cause a seg fault." << endl;
    exit(1);
  }  
  TH1D *mc;
  TH1D *puWeight;
  fin->GetObject(dataset_.c_str(), mc);  
  fin->GetObject("MuonEG_2015D", puWeight);
  if (!mc) {
    clog << "ERROR [PUScalingFactorProducer]: Could not find histogram: " << dataset_ << "; will cause a seg fault." << endl;
    exit(1);
  }
  if (!puWeight) {
    clog << "ERROR [PUScalingFactorProducer]: Could not find histogram: MuonEG_2015D, will cause a seg fault." << endl;
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
  if(type_.find("MC") < type_.length())
    {
      objectsToGet_.insert ("pileupinfos");
      getOriginalCollections (event);
      double numTruePV = 0;
      for (const auto &pv1 : *handles_.pileupinfos) {
      if(pv1.getBunchCrossing() == 0)
        numTruePV = pv1.getTrueNumInteractions();
      }
      (*eventvariables)["puScalingFactor"] = puWeight->GetBinContent(puWeight->FindBin(numTruePV));
    }
  else
    (*eventvariables)["puScalingFactor"] = 1;
  delete puWeight; 
#else
    (*eventvariables)["puScalingFactor"] = 1; 
# endif
}  

void
PUScalingFactorProducer::getOriginalCollections (const edm::Event &event)
{
  if (VEC_CONTAINS (objectsToGet_, "pileupinfos"))
    event.getByToken (pileUpInfosToken_, handles_.pileupinfos);
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PUScalingFactorProducer);
