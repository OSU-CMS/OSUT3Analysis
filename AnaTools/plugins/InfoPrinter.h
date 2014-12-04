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

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

class InfoPrinter : public edm::EDAnalyzer
{
  public:
    InfoPrinter (const edm::ParameterSet &);
    ~InfoPrinter ();

    void analyze (const edm::Event &, const edm::EventSetup &);

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private methods for printing various information about an event to the
    // stringstream.
    ////////////////////////////////////////////////////////////////////////////
    bool printEventDecision ();
    bool printCutDecision ();
    bool printTriggerDecision ();
    bool printEventFlags ();
    bool printCumulativeObjectFlags ();
    bool printObjectFlags ();
    bool printTriggerFlags ();
    bool printVetoTriggerFlags ();
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Private methods for calculating how wide a column in a table needs to
    // be.
    ////////////////////////////////////////////////////////////////////////////
    unsigned getMaxWidth (const vector<string> &) const;
    unsigned getMaxWidth (const vector<Cut> &) const;
    ////////////////////////////////////////////////////////////////////////////

    // Outputs the time on the stopwatch to the stringstream.
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
    unsigned              counter_;
    ////////////////////////////////////////////////////////////////////////////

    // Stopwatch for timing the code.
    TStopwatch *sw_;

    // Stringstream which holds all the information to be printed until the
    // destructor is called, where it is printed to the screen.
    stringstream ss_;

    // Cut decisions which are gotten from the event.
    edm::Handle<CutCalculatorPayload> cutDecisions;

    ////////////////////////////////////////////////////////////////////////////
    // Variables for holding the widths of columns of cut names and trigger
    // names.
    ////////////////////////////////////////////////////////////////////////////
    unsigned maxCutWidth_;
    unsigned maxTriggerWidth_;
    unsigned maxVetoTriggerWidth_;
    ////////////////////////////////////////////////////////////////////////////
};

#endif
