#ifndef CUT_CALCULATOR
#define CUT_CALCULATOR

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/AnaTools/plugins/AnalysisPayloads.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

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
    template <class InputCollection> bool setObjectFlags (const cut &, uint, edm::Handle<InputCollection>, const string &);
    template <class InputCollection1, class InputCollection2> bool setObjectFlags (const cut &, uint, edm::Handle<InputCollection1>, edm::Handle<InputCollection2>, const string &);
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
    // Private InputTag variables set by unpacking the collections ParameterSet.
    ////////////////////////////////////////////////////////////////////////////
    edm::InputTag  bxlumis_;
    edm::InputTag  electrons_;
    edm::InputTag  events_;
    edm::InputTag  genjets_;
    edm::InputTag  jets_;
    edm::InputTag  mcparticles_;
    edm::InputTag  mets_;
    edm::InputTag  muons_;
    edm::InputTag  photons_;
    edm::InputTag  primaryvertexs_;
    edm::InputTag  secMuons_;
    edm::InputTag  superclusters_;
    edm::InputTag  taus_;
    edm::InputTag  tracks_;
    edm::InputTag  triggers_;
    edm::InputTag  trigobjs_;
    edm::InputTag  userVariables_;
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Private variables set after unpacking the cuts ParameterSet.
    ////////////////////////////////////////////////////////////////////////////
    vector<cut>     unpackedCuts_;
    vector<string>  unpackedTriggers_;
    vector<string>  unpackedTriggersToVeto_;
    vector<string>  objectsToFlag_;
    vector<string>  objectsToGet_;
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Object collections which can be gotten from the event.
    ////////////////////////////////////////////////////////////////////////////
    edm::Handle<BNbxlumiCollection>         bxlumis;
    edm::Handle<BNelectronCollection>       electrons;
    edm::Handle<BNeventCollection>          events;
    edm::Handle<BNgenjetCollection>         genjets;
    edm::Handle<BNjetCollection>            jets;
    edm::Handle<BNmcparticleCollection>     mcparticles;
    edm::Handle<BNmetCollection>            mets;
    edm::Handle<BNmuonCollection>           muons;
    edm::Handle<BNmuonCollection>           secMuons;
    edm::Handle<BNphotonCollection>         photons;
    edm::Handle<BNprimaryvertexCollection>  primaryvertexs;
    edm::Handle<BNsuperclusterCollection>   superclusters;
    edm::Handle<BNtauCollection>            taus;
    edm::Handle<BNtrackCollection>          tracks;
    edm::Handle<BNtriggerCollection>        triggers;
    edm::Handle<BNtrigobjCollection>        trigobjs;
    edm::Handle<vector< map<string,double> > > userVariables;  
    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDProducer.
    auto_ptr<CutCalculatorPayload>  pl_;

    ////////////////////////////////////////////////////////////////////////////
    // Private variables used by the valueLookup methods.
    ////////////////////////////////////////////////////////////////////////////
    PUWeight                       *puWeight_;
    bool                           NmissoutShiftDown_;
    bool                           NmissoutShiftUp_;
    bool                           doPileupReweighting_;
    bool                           flagJESJERCorr_;
    bool                           useTrackCaloRhoCorr_;
    double                         EcaloVaryScaleFactor_;
    double                         bTagScaleFactor_;
    double                         channelScaleFactor_;
    double                         electronScaleFactor_;
    double                         electronTrackScaleFactor_;
    double                         eventScaleFactor_;
    double                         globalScaleFactor_;
    double                         isrVaryScaleFactor_;
    double                         muonScaleFactor_;
    double                         muonTrackScaleFactor_;
    double                         stopCTauScaleFactor_;
    double                         topPtScaleFactor_;
    double                         trackNMissOutScaleFactor_;
    double                         triggerMetScaleFactor_;
    double                         triggerScaleFactor_;
    edm::Handle<BNstopCollection>  stops;
    edm::Handle<double>            rhokt6CaloJetsHandle_;
    flagMap                        cumulativeFlags;
    int                            verbose_;
    string                         badCSCFile_;
    string                         datasetType_;
    string                         deadEcalFile_;
    string                         jESJERCorr_;
    vector<BadCSC>                 BadCSCVec;
    vector<DeadEcal>               DeadEcalVec;
    vector<string>                 objectsToCut_;
    vector<string>                 targetTriggers_;
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ValueLookup object and the function for initializing its private
    // variables, which some valueLookup methods use.
    ////////////////////////////////////////////////////////////////////////////
    ValueLookup *vl_;
    void initializeValueLookup ();
    ////////////////////////////////////////////////////////////////////////////
};

#endif
