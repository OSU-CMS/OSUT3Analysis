#ifndef CUT_CALCULATOR
#define CUT_CALCULATOR

#include <unordered_set>

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/AnaTools/plugins/AnalysisPayloads.h"

// Declaration of the CutCalculator EDProducer which produces various flags
// indicating whether the event and each object passed the user-defined cuts.
class CutCalculator : public edm::EDProducer
{
  public:
    CutCalculator (const edm::ParameterSet &);
    ~CutCalculator ();

    void produce (edm::Event &, const edm::EventSetup &);

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private methods used in calculating the cut decisions.
    ////////////////////////////////////////////////////////////////////////////
    bool setObjectFlags (const Cut &, unsigned) const;
    void updateCrossTalk (const Cut &, unsigned) const;
    bool unpackCuts ();
    void getTwoObjs (string, string &, string &);
    bool evaluateComparison (int, const string &, int) const;
    string getObjToGet (string);
    vector<string> splitString (const string &) const;
    bool evaluateTriggers () const;
    bool setEventFlags () const;
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::ParameterSet  collections_;
    edm::ParameterSet  cuts_;
    bool               firstEvent_;
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Private variables set after unpacking the cuts ParameterSet.
    ////////////////////////////////////////////////////////////////////////////
    unordered_set<string>  objectsToGet_;
    vector<Cut>            unpackedCuts_;
    vector<string>         unpackedTriggersToVeto_;
    vector<string>         unpackedTriggers_;
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Object collections which can be gotten from the event.
    ////////////////////////////////////////////////////////////////////////////
    Collections handles_;
    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDProducer.
    auto_ptr<CutCalculatorPayload>  pl_;

    ////////////////////////////////////////////////////////////////////////////
    // ValueLookup object, the function for initializing its private variables,
    // and the function for initializing ValueLookupTree objects.
    ////////////////////////////////////////////////////////////////////////////
    bool initializeValueLookupForest (vector<Cut> &, Collections * const);
    ////////////////////////////////////////////////////////////////////////////
};

#endif
