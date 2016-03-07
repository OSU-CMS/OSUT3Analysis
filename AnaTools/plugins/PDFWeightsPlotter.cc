#include "OSUT3Analysis/AnaTools/plugins/PDFWeightsPlotter.h"
#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

PDFWeightsPlotter::PDFWeightsPlotter(const edm::ParameterSet &cfg) :
   type_             (cfg.getParameter<string>("type")),
   lheProduct_       (cfg.getParameter<edm::InputTag>("LHEProduct")), 
   genInfoProduct_   (cfg.getParameter<edm::InputTag>("GenInfoProduct")), 
   numPDFWeights_    (cfg.getParameter<uint>("NumPDFWeights")), 
   pdfWeightsOffset_ (cfg.getParameter<uint>("PDFWeightsOffset")), 
   firstEvent_       (true)
{
  TH1::SetDefaultSumw2();
  
  if(firstEvent_)
    { 
      string directoryName = "Generatorweights Plots";
      TFileDirectory subdir = fs_->mkdir(directoryName);
      subdir.make<TH1D>("PDF Weights", "PDF Weights", numPDFWeights_ + 1, 0, numPDFWeights_ + 1);
    }
}
PDFWeightsPlotter::~PDFWeightsPlotter() {}

void
PDFWeightsPlotter::analyze(const edm::Event &event, const edm::EventSetup &setup) {
#if DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == MINI_AOD
  edm::Handle<LHEEventProduct> lheProduct;
  event.getByLabel(lheProduct_, lheProduct);
  edm::Handle<GenEventInfoProduct> genInfoProduct;
  event.getByLabel(genInfoProduct_, genInfoProduct);
  double generatorWeightSign = genInfoProduct->weight()/fabs(genInfoProduct->weight());
  TH1D *pdfweights = fs_->getObject<TH1D>("PDF Weights", "Generatorweights Plots");
  if (numPDFWeights_ + pdfWeightsOffset_ > lheProduct->weights().size())
    numPDFWeights_ = lheProduct->weights().size() - pdfWeightsOffset_;
  pdfweights->SetBinContent(1, pdfweights->GetBinContent(1) + lheProduct->weights()[0].wgt*generatorWeightSign/lheProduct->originalXWGTUP());
  for (uint i = pdfWeightsOffset_; i < numPDFWeights_ + pdfWeightsOffset_; i++) {
    double oldWeight = pdfweights->GetBinContent(i - pdfWeightsOffset_ + 2);
    pdfweights->SetBinContent(i - pdfWeightsOffset_ + 2, oldWeight + lheProduct->weights()[i].wgt*generatorWeightSign/lheProduct->originalXWGTUP());
  }
  firstEvent_ = false; 
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PDFWeightsPlotter);
#endif
