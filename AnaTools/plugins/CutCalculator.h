#ifndef CUT_CALCULATOR
#define CUT_CALCULATOR

#include <unordered_set>

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

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

    bool setInputCollectionFlags (const Cut &, unsigned) const;
    bool arbitrateInputCollectionFlags (const Cut &, unsigned) const;
    bool propagateToCompositeCollections (const Cut &, unsigned) const;
    bool setOtherCollectionsFlags (const Cut &, unsigned, const vector<string> &) const;
    bool propagateFromSingleCollections (const Cut &, unsigned, const vector<string> &) const;
    bool propagateFromCompositeCollections (const Cut &, unsigned, const vector<string> &) const;
    bool unpackCuts ();
    bool evaluateComparison (int, const string &, int) const;
    vector<string> splitString (const string &) const;
    bool evaluateTriggers (const edm::Event &) const;
    bool evaluateTriggerFilters (const edm::Event &) const;
    bool setEventFlags () const;
    vector<string> getListOfObjects (const Cuts &);
    bool isUniqueCase (const Cut &, unsigned, string) const;

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
    Cuts                   unpackedCuts_;
    vector<string>         unpackedTriggersToVeto_;
    vector<string>         unpackedTriggers_;
    vector<string>         unpackedTriggerFilters_;
    vector<string>         unpackedTriggersInMenu_;
    ////////////////////////////////////////////////////////////////////////////

    // Object collections which can be gotten from the event.
    Collections handles_;
    Tokens tokens_;

    // Payload for this EDProducer.
    auto_ptr<CutCalculatorPayload>  pl_;

    // Function for initializing the ValueLookupTree objects, one for each cut.
    bool initializeValueLookupForest (Cuts &, Collections * const);
};

#endif
