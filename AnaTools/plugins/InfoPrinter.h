#ifndef INFO_PRINTER
#define INFO_PRINTER

#include <sstream>
#include <unordered_set>

#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Common/interface/TriggerNames.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

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
    bool getEventDecision ();
    bool printCutDecision ();
    bool printTriggerDecision ();
    bool printMETFilterDecision ();
    bool printCumulativeEventFlags ();
    bool printIndividualEventFlags ();
    bool printCumulativeObjectFlags ();
    bool printIndividualObjectFlags ();
    bool printTriggerFlags ();
    bool printVetoTriggerFlags ();
    bool printTriggerFilterFlags ();
    bool printTriggerInMenuFlags ();
    bool printMETFilterFlags ();
    bool printValuesToPrint ();
    bool printAllTriggers (const edm::Event &);
    bool printAllTriggerFilters (const edm::Event &);
    bool printAllMETFilters (const edm::Event &);
    void flushPassingEvents (const unsigned = 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Private methods for calculating how wide a column in a table needs to
    // be.
    ////////////////////////////////////////////////////////////////////////////
    unsigned getMaxWidth (const vector<string> &) const;
    unsigned getMaxWidth (const map<string, bool> &) const;
    unsigned getMaxWidth (const map<string, pair<bool, unsigned> > &) const;
    unsigned getMaxWidth (const Cuts &) const;
    unsigned getMaxWidth (const ValuesToPrint &) const;
    ////////////////////////////////////////////////////////////////////////////

    // Outputs the time on the stopwatch to the stringstream.
    void outputTime ();

    void unpackValuesToPrint ();

    bool initializeValueLookupForest (ValuesToPrint &, Collections * const);

    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::ParameterSet     collections_;
    edm::InputTag         cutDecisions_;
    vector<edm::EventID>  eventsToPrint_;
    bool                  printAllEvents_;
    bool                  printPassedEvents_;
    bool                  printCumulativeObjectFlags_;
    bool                  printCutDecision_;
    bool                  printEventDecision_;
    bool                  printCumulativeEventFlags_;
    bool                  printIndividualEventFlags_;
    bool                  printIndividualObjectFlags_;
    bool                  printTriggerDecision_;
    bool                  printMETFilterDecision_;
    bool                  printTriggerFlags_;
    bool                  printVetoTriggerFlags_;
    bool                  printTriggerFilterFlags_;
    bool                  printTriggerInMenuFlags_;
    bool                  printMETFilterFlags_;
    bool                  printAllTriggers_;
    bool                  printAllTriggerFilters_;
    bool                  printAllMETFilters_;
    edm::VParameterSet    valuesToPrint_;
    bool                  firstEvent_;
    unsigned              counter_;
    ////////////////////////////////////////////////////////////////////////////

    // Object collections which can be gotten from the event.
    Collections handles_;
    Tokens tokens_;

    // Stopwatch for timing the code.
    TStopwatch *sw_;

    // Stringstream which holds all the information to be printed until the
    // destructor is called, where it is printed to the screen.
    stringstream ss_;

    // Stringstream which acts as a buffer to hold event numbers for passing
    // events.
    stringstream passingEvents_;

    // Cut decisions which are gotten from the event.
    edm::Handle<CutCalculatorPayload> cutDecisions;
    edm::EDGetTokenT<CutCalculatorPayload> cutDecisionsToken_;

    ValuesToPrint valuesToPrint;

    unordered_set<string>  objectsToGet_;

    ////////////////////////////////////////////////////////////////////////////
    // Variables for holding the widths of columns of cut names and trigger
    // names.
    ////////////////////////////////////////////////////////////////////////////
    unsigned maxCutWidth_;
    unsigned maxTriggerWidth_;
    unsigned maxVetoTriggerWidth_;
    unsigned maxValueWidth_;
    unsigned maxAllTriggerWidth_;
    unsigned maxAllTriggerFilterWidth_;
    unsigned maxAllTriggerCollectionWidth_;
    unsigned maxMETFilterWidth_;
    unsigned maxAllMETFilterWidth_;
    ////////////////////////////////////////////////////////////////////////////
};

#endif
