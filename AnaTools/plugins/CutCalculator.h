#ifndef CUT_CALCULATOR

#define CUT_CALCULATOR

#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

class edm::EventSetup;

class CutCalculator : public edm::EDProducer
{
  public:
    CutCalculator (const edm::ParameterSet &);
    ~CutCalculator ();

    void produce (edm::Event &, const edm::EventSetup &);

  private:
    template <class InputCollection> void setObjectFlags (cut &, uint, edm::Handle<InputCollection>, string);
    template <class InputCollection1, class InputCollection2> void setObjectFlags (cut &, uint, edm::Handle<InputCollection1>, edm::Handle<InputCollection2>, string);
    void unpackCuts ();
    void getTwoObjs (string, string &, string &);
    bool evaluateComparison (double, string, double);
    bool evaluateComparison (string, string, string);
    string getObjToGet (string);
    vector<string> splitString (string);
    bool evaluateTriggers (vector<string>, vector<string>, const edm::Handle<BNtriggerCollection>);

    edm::ParameterSet collections_;
    edm::ParameterSet cuts_;
    auto_ptr<flagMap> individualFlags_;

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

    vector<cut> unpackedCuts_;
    vector<string> unpackedTriggers_;
    vector<string> unpackedTriggersToVeto_;

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
    edm::Handle<BNstopCollection>           stops;
    edm::Handle<double>                     rhokt6CaloJetsHandle_;

    vector<string>  objectsToPlot;
    vector<string>  objectsToFlag;

    bool firstEvent_;

    ////////////////////////////////////////////////////////////////////////////
    // Private variables used by the valueLookup methods.
    ////////////////////////////////////////////////////////////////////////////
    PUWeight          *puWeight_;
    bool              NmissoutShiftDown_;
    bool              NmissoutShiftUp_;
    bool              doPileupReweighting_;
    bool              flagJESJERCorr_;
    bool              useTrackCaloRhoCorr_;
    double            EcaloVaryScaleFactor_;
    double            bTagScaleFactor_;
    double            channelScaleFactor_;
    double            electronScaleFactor_;
    double            electronTrackScaleFactor_;
    double            eventScaleFactor_;
    double            globalScaleFactor_;
    double            isrVaryScaleFactor_;
    double            muonScaleFactor_;
    double            muonTrackScaleFactor_;
    double            stopCTauScaleFactor_;
    double            topPtScaleFactor_;
    double            trackNMissOutScaleFactor_;
    double            triggerMetScaleFactor_;
    double            triggerScaleFactor_;
    flagMap           cumulativeFlags;
    int               verbose_;
    string            badCSCFile_;
    string            datasetType_;
    string            deadEcalFile_;
    string            jESJERCorr_;
    vector<BadCSC>    BadCSCVec;
    vector<DeadEcal>  DeadEcalVec;
    vector<string>    objectsToCut;
    vector<string>    objectsToGet;
    vector<string>    targetTriggers_;
  //////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    //ValueLookup object and the function for initializing its private
    //variables, which some valueLookup methods use.
    ////////////////////////////////////////////////////////////////////////////
    ValueLookup *vl_;
    void initializeValueLookup ();
    ////////////////////////////////////////////////////////////////////////////
};

#endif
