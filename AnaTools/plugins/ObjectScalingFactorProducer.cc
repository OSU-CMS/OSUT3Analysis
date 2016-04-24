#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"
#include "OSUT3Analysis/AnaTools/plugins/ObjectScalingFactorProducer.h"

ObjectScalingFactorProducer::ObjectScalingFactorProducer(const edm::ParameterSet &cfg) :
   EventVariableProducer(cfg),
   muonFile_         (cfg.getParameter<string>("muonFile")),
   electronFile_     (cfg.getParameter<string>("electronFile")),
   electronWp_       (cfg.getParameter<string>("electronWp")),
   muonWp_           (cfg.getParameter<string>("muonWp")),
   doEleSF_          (cfg.getParameter<bool>("doEleSF")),
   doMuSF_           (cfg.getParameter<bool>("doMuSF"))
{
  if (doEleSF_)
    objectsToGet_.insert ("electrons");
  if (doMuSF_)
    objectsToGet_.insert ("muons");
  anatools::getAllTokens (collections_, consumesCollector (), tokens_);
}

ObjectScalingFactorProducer::~ObjectScalingFactorProducer() {}

void
ObjectScalingFactorProducer::AddVariables (const edm::Event &event) {
#if DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == MINI_AOD

  anatools::getRequiredCollections (objectsToGet_, handles_, event, tokens_);
  if (doEleSF_)
    {
      TFile *elesf = TFile::Open (electronFile_.c_str ());
      if (!elesf || elesf->IsZombie()) {
        clog << "ERROR [ObjectScalingFactorProducer]: Could not find file: " << electronFile_
    	 << "; will cause a seg fault." << endl;
        exit(1);
      }

      string eWp = electronWp_.substr(0,electronWp_.find("_"));
      string shift = electronWp_.substr(electronWp_.find("_")+1);
      TH2F *ele;
      elesf->GetObject(eWp.c_str(),ele);
      if(!elesf){
        clog << "ERROR [ObjectScalingFactorProducer]: Could not find histogram: " << eWp.c_str()
         << "; will cause a seg fault." << endl;
        exit(1);
      }

      double eleSF = 1.0;
      if(handles_.electrons->size()){
        for (const auto &electron1 : *handles_.electrons) {
          float eta = abs(electron1.eta()) > ele->GetXaxis()->GetBinCenter(ele->GetNbinsX()) ? ele->GetXaxis()->GetBinCenter(ele->GetNbinsX()) : abs(electron1.eta());
          float pt = electron1.pt() > ele->GetYaxis()->GetBinCenter(ele->GetNbinsY()) ? ele->GetYaxis()->GetBinCenter(ele->GetNbinsY()) : electron1.pt();
	  float sf = ele->GetBinContent(ele->FindBin(eta,pt));
	  if (shift == "up")
	    sf += ele->GetBinError(ele->FindBin(eta,pt));
	  else if (shift == "down")
	    sf -= ele->GetBinError(ele->FindBin(eta,pt));
	  eleSF = eleSF * sf;
	}
      }
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

      string muWp = muonWp_.substr(0,muonWp_.find("_"));
      string shift = muonWp_.substr(muonWp_.find("_")+1);
      TH2F *mu;
      musf->GetObject(muWp.c_str(),mu);
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
          float sf = mu->GetBinContent(mu->FindBin(pt,eta));
          if (shift == "up")
            sf += mu->GetBinError(mu->FindBin(pt,eta));
          else if (shift == "down")
            sf -= mu->GetBinError(mu->FindBin(pt,eta));
          muSF = muSF * sf;
	}
      }
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
