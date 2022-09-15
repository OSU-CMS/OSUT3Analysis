#ifndef CUT_FLOW_PLOTTER
#define CUT_FLOW_PLOTTER

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/stream/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TH1D.h"

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

class CutFlowPlotter : public edm::stream::EDAnalyzer<>
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
    edm::ParameterSet  collections_;
    edm::InputTag      cutDecisions_;
    string             module_type_;
    string             module_label_;
    bool               firstEvent_;
    vector<string>     triggers_;
    vector<string>     triggersToVeto_;
    vector<string>     triggerFilters_;
    vector<string>     metFilters_;
    ////////////////////////////////////////////////////////////////////////////

    // Objects which can be gotten from the event.
    edm::Handle<CutCalculatorPayload> cutDecisions;
    edm::Handle<TYPE(generatorweights)> generatorweights;
    edm::EDGetTokenT<CutCalculatorPayload> cutDecisionsToken_;
    edm::EDGetTokenT<TYPE(generatorweights)> generatorweightsToken_;

    ////////////////////////////////////////////////////////////////////////////
    // TFileService object used for interacting with the ROOT file of
    // histograms and a map for storing the histogram objects.
    ////////////////////////////////////////////////////////////////////////////
    edm::Service<TFileService> fs_;
    map<string, TH1D *> oneDHists_;
    ////////////////////////////////////////////////////////////////////////////
};

#endif
