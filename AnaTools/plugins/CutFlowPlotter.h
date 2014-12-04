#ifndef CUT_FLOW_PLOTTER
#define CUT_FLOW_PLOTTER

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TH1D.h"

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

class CutFlowPlotter : public edm::EDAnalyzer
{
  public:
    CutFlowPlotter (const edm::ParameterSet &);
    ~CutFlowPlotter ();

    void analyze (const edm::Event &, const edm::EventSetup &);

  private:
    bool initializeCutFlow ();
    bool fillCutFlow (double = 1.0);

    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::InputTag  cutDecisions_;
    bool           firstEvent_;
    ////////////////////////////////////////////////////////////////////////////

    // Objects which can be gotten from the event.
    edm::Handle<CutCalculatorPayload> cutDecisions;

    ////////////////////////////////////////////////////////////////////////////
    // TFileService object used for interacting with the ROOT file of
    // histograms and a map for storing the histogram objects.
    ////////////////////////////////////////////////////////////////////////////
    edm::Service<TFileService> fs_;
    map<string, TH1D *> oneDHists_;
    ////////////////////////////////////////////////////////////////////////////
};

#endif
