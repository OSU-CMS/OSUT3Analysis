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
      if(!elesf){
        clog << "ERROR [ObjectScalingFactorProducer]: Could not find histogram: " << electronWp_.c_str()
         << "; will cause a seg fault." << endl;
        exit(1);
      }
      double eleSF = 1.0;
      if(handles_.electrons->size()){
        for (const auto &electron1 : *handles_.electrons) {
          float eta = abs(electron1.eta()) > ele->GetXaxis()->GetBinCenter(ele->GetNbinsX()) ? ele->GetXaxis()->GetBinCenter(ele->GetNbinsX()) : abs(electron1.eta());
          float pt = electron1.pt() > ele->GetYaxis()->GetBinCenter(ele->GetNbinsY()) ? ele->GetYaxis()->GetBinCenter(ele->GetNbinsY()) : electron1.pt();
          eleSF = eleSF * ele->GetBinContent(ele->FindBin(eta,pt));
       }}
      (*eventvariables)["electronScalingFactor"] = eleSF;
      delete ele; 
      elesf->Close(); 
   }
  if(doMuSF_)
    {
      TFile *musf = TFile::Open (muonFile_.c_str ());
      if (!musf || musf->IsZombie()) {
        clog << "ERROR [ObjectScalingFactorProducer]: Could not find file: " << muonFile_
         << "; will cause a seg fault." << endl;
        exit(1);
      }
      TH2F *mu;   
      musf->GetObject(muonWp_.c_str(),mu);
      if(!musf){
        clog << "ERROR [ObjectScalingFactorProducer]: Could not find histogram: " << muonWp_.c_str()
         << "; will cause a seg fault." << endl;
        exit(1);
      }
      double muSF = 1.0;
      if(handles_.muons->size()){
        for (const auto &muon1 : *handles_.muons) {
          float eta = abs(muon1.eta()) > mu->GetYaxis()->GetBinCenter(mu->GetNbinsY()) ? mu->GetYaxis()->GetBinCenter(mu->GetNbinsY()): abs(muon1.eta());
          float pt = muon1.pt() > mu->GetXaxis()->GetBinCenter(mu->GetNbinsX()) ? mu->GetXaxis()->GetBinCenter(mu->GetNbinsX()) : muon1.pt();
          muSF = muSF*mu->GetBinContent(mu->FindBin(pt,eta));
      }}
      (*eventvariables)["muonScalingFactor"] = muSF;
      delete mu; 
      musf->Close(); 
    }
#else
  (*eventvariables)["electronScalingFactor"] = 1; 
  (*eventvariables)["muonScalingFactor"] = 1;
# endif
}  

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(ObjectScalingFactorProducer);
