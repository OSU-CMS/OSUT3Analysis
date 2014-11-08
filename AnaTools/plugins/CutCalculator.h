#ifndef CUT_CALCULATOR
#define CUT_CALCULATOR

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
    template <class InputCollection> bool setObjectFlags (const Cut &, unsigned);
    bool setObjectFlags (const Cut &, unsigned);
    void updateCrossTalk (FlagMap &);
    bool unpackCuts ();
    void getTwoObjs (string, string &, string &);
    template<typename T> bool evaluateComparison (T, string, T);
    string getObjToGet (string);
    vector<string> splitString (string);
    bool evaluateTriggers ();
    bool setEventFlags ();
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
    vector<Cut>     unpackedCuts_;
    vector<string>  unpackedTriggers_;
    vector<string>  unpackedTriggersToVeto_;
    vector<string>  objectsToFlag_;
    vector<string>  objectsToGet_;
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
    bool initializeValueLookupTrees (vector<Cut> &, Collections *);
    ////////////////////////////////////////////////////////////////////////////
};

#endif
