#include "OSUT3Analysis/AnaTools/plugins/PDFWeightsPlotter.h"
#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

PDFWeightsPlotter::PDFWeightsPlotter(const edm::ParameterSet &cfg) :
   type_             (cfg.getParameter<string>("type")),
   lheProduct_       (cfg.getParameter<edm::InputTag>("LHEProduct")), 
   firstEvent_       (true)
{
  TH1::SetDefaultSumw2();
  
  if(firstEvent_)
    { 
      string directoryName = "Generatorweights Plots";
      TFileDirectory subdir = fs_->mkdir(directoryName);
      subdir.make<TH1D>("PDF Weights", "PDF Weights", 5000, 0, 5000);
    }
}
PDFWeightsPlotter::~PDFWeightsPlotter() {}

void
PDFWeightsPlotter::analyze(const edm::Event &event, const edm::EventSetup &setup) {
#if DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == MINI_AOD
  edm::Handle<LHEEventProduct> lheProduct;
  event.getByLabel(lheProduct_, lheProduct);
  TH1D *pdfweights = fs_->getObject<TH1D>("PDF Weights", "Generatorweights Plots"); 
  for (uint i = 0; i < lheProduct->weights().size(); i++) {
    double oldWeight = pdfweights->GetBinContent(i);
    pdfweights->SetBinContent(i, oldWeight + lheProduct->weights()[i].wgt/lheProduct->originalXWGTUP());
  }
  firstEvent_ = false; 
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PDFWeightsPlotter);
#endif
