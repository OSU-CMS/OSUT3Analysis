#ifndef INFO_PRINTER
#define INFO_PRINTER

#include <sstream>

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TStopwatch.h"

#include "OSUT3Analysis/AnaTools/plugins/AnalysisPayloads.h"

class InfoPrinter : public edm::EDAnalyzer
{
  public:
    InfoPrinter (const edm::ParameterSet &);
    ~InfoPrinter ();

    void analyze (const edm::Event &, const edm::EventSetup &);

  private:
    bool printEventDecision ();
    bool printCutDecision ();
    bool printTriggerDecision ();
    bool printEventFlags ();
    bool printCumulativeObjectFlags ();
    bool printObjectFlags ();
    bool printTriggerFlags ();
    bool printVetoTriggerFlags ();
    unsigned getMaxWidth (const vector<string> &);
    unsigned getMaxWidth (const vector<Cut> &);
    void outputTime ();

    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::InputTag         cutDecisions_;
    vector<edm::EventID>  eventsToPrint_;
    bool                  printAllEvents_;
    bool                  printCumulativeObjectFlags_;
    bool                  printCutDecision_;
    bool                  printEventDecision_;
    bool                  printEventFlags_;
    bool                  printObjectFlags_;
    bool                  printTriggerDecision_;
    bool                  printTriggerFlags_;
    bool                  printVetoTriggerFlags_;
    bool                  firstEvent_;
    ////////////////////////////////////////////////////////////////////////////

    TStopwatch *sw_;
    stringstream ss_;
    edm::Handle<CutCalculatorPayload> cutDecisions;
    unsigned maxCutWidth_;
    unsigned maxTriggerWidth_;
    unsigned maxVetoTriggerWidth_;
};

#endif
