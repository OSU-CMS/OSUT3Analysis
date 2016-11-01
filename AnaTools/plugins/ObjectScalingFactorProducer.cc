#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"
#include "OSUT3Analysis/AnaTools/plugins/ObjectScalingFactorProducer.h"

ObjectScalingFactorProducer::ObjectScalingFactorProducer(const edm::ParameterSet &cfg) :
  EventVariableProducer(cfg),
  doEleSF_          (false),
  doMuSF_           (false),
  doTrackSF_        (false),
{
  if (cfg.exists ("doEleSF"))
    doEleSF_ = cfg.getParameter<bool>("doEleSF");
  if (cfg.exists ("doMuSF"))
    doMuSF_ = cfg.getParameter<bool>("doMuSF");
  if (cfg.exists ("doTrackSF"))
    doTrackSF_ = cfg.getParameter<bool>("doTrackSF");

  if (doEleSF_)
    {
      electronFile_ = cfg.getParameter<string>("electronFile");
      electronWp_ = cfg.getParameter<string>("electronWp");
    }
  if (doEleSF_)
    {
      muonFile_ = cfg.getParameter<string>("muonFile");
      muonWp_ = cfg.getParameter<string>("muonWp");
    }
  if (doTrackSF_)
    trackFile_ = cfg.getParameter<string>("trackFile");

  if (doEleSF_)
    objectsToGet_.insert ("electrons");
  if (doMuSF_)
    objectsToGet_.insert ("muons");
  if (doTrackSF_)
    objectsToGet_.insert ("tracks");
  anatools::getAllTokens (collections_, consumesCollector (), tokens_);
}

ObjectScalingFactorProducer::~ObjectScalingFactorProducer() {}

void
ObjectScalingFactorProducer::AddVariables (const edm::Event &event) {
#if DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == MINI_AOD

  if (event.isRealData ())
    {
      doEleSF_ = doMuSF_ = doTrackSF_ = false;
      objectsToGet_.clear ();
    }

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
      for (const auto &electron1 : *handles_.electrons) {
        float eta = abs(electron1.eta()) > ele->GetXaxis()->GetBinCenter(ele->GetNbinsX()) ? ele->GetXaxis()->GetBinCenter(ele->GetNbinsX()) : abs(electron1.eta());
        float pt = electron1.pt() > ele->GetYaxis()->GetBinCenter(ele->GetNbinsY()) ? ele->GetYaxis()->GetBinCenter(ele->GetNbinsY()) : electron1.pt();
        if (ele->GetXaxis()->FindBin(electron1.eta()))
            eta = electron1.eta() > 0 ? eta : -eta;
        float sf = ele->GetBinContent(ele->FindBin(eta,pt));
        if (shift == "up")
          sf += ele->GetBinError(ele->FindBin(eta,pt));
        else if (shift == "down")
          sf -= ele->GetBinError(ele->FindBin(eta,pt));
        eleSF = eleSF * sf;
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
      (*eventvariables)["muonScalingFactor"] = muSF;
      delete mu;
      musf->Close();
    }
  if (doTrackSF_)
    {
      double sf = 1.0;
#if IS_VALID(tracks)
      TFile *trackSF = TFile::Open (trackFile_.c_str ());
      if (!trackSF || trackSF->IsZombie()) {
        clog << "ERROR [ObjectScalingFactorProducer]: Could not find file: " << trackFile_
         << "; will cause a seg fault." << endl;
        exit(1);
      }

      TH1D *data = (TH1D *) trackSF->Get ("missingOuterHits_data");
      data->SetDirectory (0);
      TH1D *mc = (TH1D *) trackSF->Get ("missingOuterHits_mc");
      mc->SetDirectory (0);
      trackSF->Close ();
      delete trackSF;

      data->Scale (1.0 / data->Integral ());
      mc->Scale (1.0 / mc->Integral ());
      data->Divide (mc);

      for (const auto &track : *handles_.tracks)
        {
          double missingOuterHits = track.hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::MISSING_OUTER_HITS);
          sf *= data->GetBinContent (data->FindBin (missingOuterHits));
        }
      delete data;
      delete mc;
#endif
      (*eventvariables)["trackScalingFactor"] = sf;
    }
#else
  (*eventvariables)["electronScalingFactor"] = 1;
  (*eventvariables)["muonScalingFactor"] = 1;
  (*eventvariables)["trackScalingFactor"] = 1;
# endif
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(ObjectScalingFactorProducer);
