#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"
#include "OSUT3Analysis/AnaTools/plugins/ObjectScalingFactorProducer.h"

ObjectScalingFactorProducer::ObjectScalingFactorProducer(const edm::ParameterSet &cfg) :
   EventVariableProducer(cfg),
   muonFile_         (cfg.getParameter<string>("muonFile")),  
   electronFile_     (cfg.getParameter<string>("electronFile")),  
   electronWp_       (cfg.getParameter<string>("electronWp")),
   muonWp_           (cfg.getParameter<string>("muonWp")),
   doEleSF_             (cfg.getParameter<bool>("doEleSF")),
   doMuSF_             (cfg.getParameter<bool>("doMuSF"))
{
}

ObjectScalingFactorProducer::~ObjectScalingFactorProducer() {}

void
ObjectScalingFactorProducer::AddVariables (const edm::Event &event) {
#if DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == MINI_AOD
  
  if (doEleSF_)
    objectsToGet_.insert ("electrons");
  if (doMuSF_)
    objectsToGet_.insert ("muons");
  anatools::getRequiredCollections (objectsToGet_, collections_, handles_, event);
  if (doEleSF_)
    {
      TFile *elesf = TFile::Open (electronFile_.c_str ());
      if (!elesf || elesf->IsZombie()) {
        clog << "ERROR [ObjectScalingFactorProducer]: Could not find file: " << electronFile_
    	 << "; will cause a seg fault." << endl;
        exit(1);
      }
      TH2D *ele;   
      elesf->GetObject(electronWp_.c_str(),ele);
      double eleSF = 1.0;
      for (const auto &electron1 : *handles_.electrons) {
        eleSF = eleSF * ele->GetBinContent(ele->FindBin(abs(electron1.eta()),electron1.pt()));
      }
      (*eventvariables)["electronScalingFactor"] = eleSF;
      delete ele; 
    }
  if(doMuSF_)
    {
      TFile *musf = TFile::Open (muonFile_.c_str ());
      if (!musf || musf->IsZombie()) {
        clog << "ERROR [ObjectScalingFactorProducer]: Could not find file: " << muonFile_
         << "; will cause a seg fault." << endl;
        exit(1);
      }
      TH2D *mu;   
      musf->GetObject(muonWp_.c_str(),mu);
      double muSF = 1.0;
      for (const auto &muon1 : *handles_.muons) {
        muSF = muSF*mu->GetBinContent(mu->FindBin(muon1.pt(),abs(muon1.eta())));
      }
      (*eventvariables)["muonScalingFactor"] = muSF;
      delete mu; 
    }
#else
  (*eventvariables)["electronScalingFactor"] = 1; 
  (*eventvariables)["muonScalingFactor"] = 1;
# endif
}  

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(ObjectScalingFactorProducer);
