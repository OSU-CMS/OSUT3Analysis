#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"
#include "OSUT3Analysis/AnaTools/plugins/PUScalingFactorProducer.h"

PUScalingFactorProducer::PUScalingFactorProducer(const edm::ParameterSet &cfg) :
   EventVariableProducer(cfg),
   PU_               (cfg.getParameter<string>("PU")),
   dataset_          (cfg.getParameter<string>("dataset")),
   target_           (cfg.getParameter<string>("target")),
   targetUp_         ((cfg.exists("targetUp"))   ? cfg.getParameter<string>("targetUp")   : ""),
   targetDown_       ((cfg.exists("targetDown")) ? cfg.getParameter<string>("targetDown") : ""),
   puWeight_         (NULL),
   puWeightUp_       (NULL),
   puWeightDown_     (NULL),
   isFirstEvent_     (true)
{
  if(collections_.exists ("pileupinfos"))
    pileUpInfosToken_ = consumes<vector<TYPE(pileupinfos)> > (collections_.getParameter<edm::InputTag> ("pileupinfos"));

}

PUScalingFactorProducer::~PUScalingFactorProducer() {
  if (puWeight_)     delete puWeight_;
  if (puWeightUp_)   delete puWeightUp_;
  if (puWeightDown_) delete puWeightDown_;
}

void
PUScalingFactorProducer::AddVariables (const edm::Event &event) {
#if DATA_FORMAT_FROM_MINIAOD
  if (!event.isRealData () && PU_ != "" && dataset_ != "" && target_ != "")
    {
      // if the weights histogram hasn't been defined yet, calculate it from target/mc
      if (!puWeight_) {
        TFile * fin = new TFile(PU_.c_str());
        if(!fin || fin->IsZombie()) {
          edm::LogError ("PUScalingFactorProducer") << "ERROR [PUScalingFactorProducer]: Could not find file: " << PU_ << "; will cause a seg fault." << endl;
          exit(1);
        }

        TH1D * mc = (TH1D*)fin->Get(dataset_.c_str());
        puWeight_ = (TH1D*)fin->Get(target_.c_str());
        if(targetUp_ != "")   puWeightUp_   = (TH1D*)fin->Get(targetUp_.c_str());
        if(targetDown_ != "") puWeightDown_ = (TH1D*)fin->Get(targetDown_.c_str());

        if (!mc) {
          edm::LogError ("PUScalingFactorProducer") << "ERROR [PUScalingFactorProducer]: Could not find histogram: " << dataset_ << "; will cause a seg fault." << endl;
          exit(1);
        }
        if (!puWeight_) {
          edm::LogError ("PUScalingFactorProducer") << "ERROR [PUScalingFactorProducer]: Could not find histogram: " << target_ <<", will cause a seg fault." << endl;
          exit(1);
        }
        if (targetUp_ != "" && !puWeightUp_) {
          edm::LogError ("PUScalingFactorProducer") << "ERROR [PUScalingFactorProducer]: Could not find histogram: " << targetUp_ <<", will cause a seg fault." << endl;
          exit(1);
        }
        if (targetDown_ != "" && !puWeightDown_) {
          edm::LogError ("PUScalingFactorProducer") << "ERROR [PUScalingFactorProducer]: Could not find histogram: " << targetDown_ <<", will cause a seg fault." << endl;
          exit(1);
        }

        if(puWeightUp_) {
          if(puWeight_->GetNbinsX() != puWeightUp_->GetNbinsX() ||
             puWeight_->GetBinLowEdge(1) != puWeightUp_->GetBinLowEdge(1) ||
             puWeight_->GetBinLowEdge(puWeight_->GetNbinsX()+1) != puWeightUp_->GetBinLowEdge(puWeightUp_->GetNbinsX()+1)) {
               edm::LogError ("PUScalingFactorProducer") << "ERROR [PUScalingFactorProducer]: targetUp ("
                                                         << targetUp_
                                                         << ") requested with different binning than target ("
                                                         << target_
                                                         << ") -- will not behave as expected!"
                                                         << endl;
               exit(1);
          }
        }

        if(puWeightDown_) {
          if(puWeight_->GetNbinsX() != puWeightDown_->GetNbinsX() ||
             puWeight_->GetBinLowEdge(1) != puWeightDown_->GetBinLowEdge(1) ||
             puWeight_->GetBinLowEdge(puWeight_->GetNbinsX()+1) != puWeightDown_->GetBinLowEdge(puWeightDown_->GetNbinsX()+1)) {
               edm::LogError ("PUScalingFactorProducer") << "ERROR [PUScalingFactorProducer]: targetUp ("
                                                         << targetDown_
                                                         << ") requested with different binning than target ("
                                                         << target_
                                                         << ") -- will not behave as expected!"
                                                         << endl;
               exit(1);
          }
        }

        mc->SetDirectory(0);
        puWeight_->SetDirectory(0);
	mc->Scale (puWeight_->Integral () / mc->Integral ());
        if(puWeightUp_) puWeightUp_->SetDirectory(0);
        if(puWeightDown_) puWeightDown_->SetDirectory(0);

        TH1D *trimmedMC = new TH1D ("bla", "bla", puWeight_->GetNbinsX(), 0, puWeight_->GetNbinsX());
        for (int bin = 1; bin <= puWeight_->GetNbinsX(); bin++)
          trimmedMC->SetBinContent (bin, mc->GetBinContent (bin));
        puWeight_->Divide (trimmedMC);

        if(puWeightUp_)   puWeightUp_->Divide(trimmedMC);
        if(puWeightDown_) puWeightDown_->Divide(trimmedMC);

        fin->Close();
        delete fin;
        delete mc;
        delete trimmedMC;
      }

      // get the pileup info from the event
      edm::Handle<vector<PileupSummaryInfo> > pileupinfos;
      event.getByToken (pileUpInfosToken_, pileupinfos);

      double numTruePV = 0;
      for (const auto &pv1 : *pileupinfos) {
        if(pv1.getBunchCrossing() == 0) {
          numTruePV = pv1.getTrueNumInteractions();
        }
      }

      if (isFirstEvent_)
        clog << "[PUScalingFactorProducer] Applying PU reweighting (isRealData: "
             << (event.isRealData () ? "true" : "false")
             << ", PU_: \"" << PU_
             << "\", dataset_: \"" << dataset_
             << "\", target_: \"" << target_
             << "\", targetUp_: \"" << (targetUp_ != "" ? targetUp_ : "n/a")
             << "\", targetDown_: \"" << (targetDown_ != "" ? targetDown_ : "n/a")
             << "\")" << endl;
      (*eventvariables)["puScalingFactor"]     = puWeight_->GetBinContent(puWeight_->FindBin(numTruePV));
      (*eventvariables)["puScalingFactorUp"]   = puWeightUp_ ? puWeightUp_->GetBinContent(puWeightUp_->FindBin(numTruePV)) : 1;
      (*eventvariables)["puScalingFactorDown"] = puWeightDown_ ? puWeightDown_->GetBinContent(puWeightDown_->FindBin(numTruePV)) : 1;
    }
  else
    {
      if (isFirstEvent_)
        clog << "[PUScalingFactorProducer] NOT applying PU reweighting (isRealData: "
             << (event.isRealData () ? "true" : "false")
             << ", PU_: \"" << PU_
             << "\", dataset_: \"" << dataset_
             << "\", target_: \"" << target_
             << "\", targetUp_: \"" << (targetUp_ != "" ? targetUp_ : "n/a")
             << "\", targetDown_: \"" << (targetDown_ != "" ? targetDown_ : "n/a")
             << "\")" << endl;
             (*eventvariables)["puScalingFactor"]     = 1;
             (*eventvariables)["puScalingFactorUp"]   = 1;
             (*eventvariables)["puScalingFactorDown"] = 1;
    }
#else
    (*eventvariables)["puScalingFactor"]     = 1;
    (*eventvariables)["puScalingFactorUp"]   = 1;
    (*eventvariables)["puScalingFactorDown"] = 1;
# endif
    isFirstEvent_ = false;
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PUScalingFactorProducer);
