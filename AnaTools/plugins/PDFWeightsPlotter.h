#ifndef PDF_WEIGHTS_PLOTTER
#define PDF_WEIGHTS_PLOTTER

#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"
#include <string>
#include "TH1D.h"
#include "TFile.h"
class PDFWeightsPlotter : public edm::EDAnalyzer
  {
    public:
        PDFWeightsPlotter (const edm::ParameterSet &);
        void analyze (const edm::Event &, const edm::EventSetup &);;
        ~PDFWeightsPlotter ();
        edm::Service<TFileService> fs_;

    private:
        string type_;
        edm::EDGetTokenT<GenEventInfoProduct> genInfoProductToken_;
        edm::EDGetTokenT<LHEEventProduct> lheProductToken_;
        uint numPDFWeights_;
        uint pdfWeightsOffset_;
        bool firstEvent_;
};
#endif
