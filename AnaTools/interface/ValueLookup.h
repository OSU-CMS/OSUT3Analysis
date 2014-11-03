#ifndef VALUE_LOOKUP
#define VALUE_LOOKUP

#include "DataFormats/Common/interface/Handle.h"

#include "BEAN/Collections/interface/BNbxlumi.h"
#include "BEAN/Collections/interface/BNelectron.h"
#include "BEAN/Collections/interface/BNevent.h"
#include "BEAN/Collections/interface/BNjet.h"
#include "BEAN/Collections/interface/BNmcparticle.h"
#include "BEAN/Collections/interface/BNmet.h"
#include "BEAN/Collections/interface/BNmuon.h"
#include "BEAN/Collections/interface/BNphoton.h"
#include "BEAN/Collections/interface/BNprimaryvertex.h"
#include "BEAN/Collections/interface/BNskimbits.h"
#include "BEAN/Collections/interface/BNsupercluster.h"
#include "BEAN/Collections/interface/BNtrack.h"
#include "BEAN/Collections/interface/BNtrigger.h"
#include "BEAN/Collections/interface/BNtrigobj.h"
#include "BEAN/Collections/interface/BNtau.h"
#include "BEAN/Collections/interface/BNgenjet.h"

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"
#include "OSUT3Analysis/AnaTools/interface/PUWeight.h"
#include "OSUT3Analysis/AnaTools/interface/BNstop.h"

// Declaration of the ValueLookup class, containing all the valueLookup methods
// and the variables on which they depend.
class ValueLookup {
  public:
    ValueLookup () {};
    ~ValueLookup () {};

  public:
    ////////////////////////////////////////////////////////////////////////////
    // valueLookup methods for each type of object.
    ////////////////////////////////////////////////////////////////////////////
    double  valueLookup  (const  BNbxlumi             &object,  string  variable);
    double  valueLookup  (const  BNelectron           &object,  string  variable);
    double  valueLookup  (const  BNevent              &object,  string  variable);
    double  valueLookup  (const  BNgenjet             &object,  string  variable);
    double  valueLookup  (const  BNjet                &object,  string  variable);
    double  valueLookup  (const  BNmcparticle         &object,  string  variable);
    double  valueLookup  (const  BNmet                &object,  string  variable);
    double  valueLookup  (const  BNmuon               &object,  string  variable);
    double  valueLookup  (const  BNphoton             &object,  string  variable);
    double  valueLookup  (const  BNprimaryvertex      &object,  string  variable);
    double  valueLookup  (const  BNstop               &object,  string  variable);
    double  valueLookup  (const  BNsupercluster       &object,  string  variable);
    double  valueLookup  (const  BNtau                &object,  string  variable);
    double  valueLookup  (const  BNtrack              &object,  string  variable);
    double  valueLookup  (const  BNtrigobj            &object,  string  variable);
    double  valueLookup  (const  map<string, double>  &object,  string  variable);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // valueLookup methods for each pair of objects.
    ////////////////////////////////////////////////////////////////////////////
    double  valueLookup  (const  BNelectron    &object1,  const  BNelectron    &object2,  string  variable);
    double  valueLookup  (const  BNelectron    &object1,  const  BNevent       &object2,  string  variable);
    double  valueLookup  (const  BNelectron    &object1,  const  BNjet         &object2,  string  variable);
    double  valueLookup  (const  BNelectron    &object1,  const  BNmcparticle  &object2,  string  variable);
    double  valueLookup  (const  BNelectron    &object1,  const  BNmuon        &object2,  string  variable);
    double  valueLookup  (const  BNelectron    &object1,  const  BNphoton      &object2,  string  variable);
    double  valueLookup  (const  BNelectron    &object1,  const  BNtrack       &object2,  string  variable);
    double  valueLookup  (const  BNelectron    &object1,  const  BNtrigobj     &object2,  string  variable);
    double  valueLookup  (const  BNjet         &object1,  const  BNjet         &object2,  string  variable);
    double  valueLookup  (const  BNjet         &object1,  const  BNmcparticle  &object2,  string  variable);
    double  valueLookup  (const  BNjet         &object1,  const  BNtrack       &object2,  string  variable);
    double  valueLookup  (const  BNmcparticle  &object1,  const  BNmcparticle  &object2,  string  variable);
    double  valueLookup  (const  BNmet         &object1,  const  BNjet         &object2,  string  variable);
    double  valueLookup  (const  BNmet         &object1,  const  BNmcparticle  &object2,  string  variable);
    double  valueLookup  (const  BNmuon        &object1,  const  BNevent       &object2,  string  variable);
    double  valueLookup  (const  BNmuon        &object1,  const  BNjet         &object2,  string  variable);
    double  valueLookup  (const  BNmuon        &object1,  const  BNmcparticle  &object2,  string  variable);
    double  valueLookup  (const  BNmuon        &object1,  const  BNmuon        &object2,  string  variable);
    double  valueLookup  (const  BNmuon        &object1,  const  BNphoton      &object2,  string  variable);
    double  valueLookup  (const  BNmuon        &object1,  const  BNtau         &object2,  string  variable);
    double  valueLookup  (const  BNmuon        &object1,  const  BNtrack       &object2,  string  variable);
    double  valueLookup  (const  BNmuon        &object1,  const  BNtrigobj     &object2,  string  variable);
    double  valueLookup  (const  BNphoton      &object1,  const  BNjet         &object2,  string  variable);
    double  valueLookup  (const  BNstop        &object1,  const  BNmcparticle  &object2,  string  variable);
    double  valueLookup  (const  BNtau         &object1,  const  BNtau         &object2,  string  variable);
    double  valueLookup  (const  BNtau         &object1,  const  BNtrack       &object2,  string  variable);
    double  valueLookup  (const  BNtrack       &object1,  const  BNevent       &object2,  string  variable);
    double  valueLookup  (const  BNtrack       &object1,  const  BNjet         &object2,  string  variable);
    double  valueLookup  (const  BNtrack       &object1,  const  BNmcparticle  &object2,  string  variable);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Public methods to assign values to each of the private object
    // collections.
    ////////////////////////////////////////////////////////////////////////////
    edm::Handle<BNbxlumiCollection>         setBxlumis                   (edm::Handle<BNbxlumiCollection>         &x)  {  return  (bxlumis                    =  x);  };
    edm::Handle<BNelectronCollection>       setElectrons                 (edm::Handle<BNelectronCollection>       &x)  {  return  (electrons                  =  x);  };
    edm::Handle<BNeventCollection>          setEvents                    (edm::Handle<BNeventCollection>          &x)  {  return  (events                     =  x);  };
    edm::Handle<BNgenjetCollection>         setGenjets                   (edm::Handle<BNgenjetCollection>         &x)  {  return  (genjets                    =  x);  };
    edm::Handle<BNjetCollection>            setJets                      (edm::Handle<BNjetCollection>            &x)  {  return  (jets                       =  x);  };
    edm::Handle<BNmcparticleCollection>     setMcparticles               (edm::Handle<BNmcparticleCollection>     &x)  {  return  (mcparticles                =  x);  };
    edm::Handle<BNmetCollection>            setMets                      (edm::Handle<BNmetCollection>            &x)  {  return  (mets                       =  x);  };
    edm::Handle<BNmuonCollection>           setMuons                     (edm::Handle<BNmuonCollection>           &x)  {  return  (muons                      =  x);  };
    edm::Handle<BNmuonCollection>           setSecMuons                  (edm::Handle<BNmuonCollection>           &x)  {  return  (secMuons                   =  x);  };
    edm::Handle<BNphotonCollection>         setPhotons                   (edm::Handle<BNphotonCollection>         &x)  {  return  (photons                    =  x);  };
    edm::Handle<BNprimaryvertexCollection>  setPrimaryvertexs            (edm::Handle<BNprimaryvertexCollection>  &x)  {  return  (primaryvertexs             =  x);  };
    edm::Handle<BNstopCollection>           setStops                     (edm::Handle<BNstopCollection>           &x)  {  return  (stops                      =  x);  };
    edm::Handle<BNsuperclusterCollection>   setSuperclusters             (edm::Handle<BNsuperclusterCollection>   &x)  {  return  (superclusters              =  x);  };
    edm::Handle<BNtauCollection>            setTaus                      (edm::Handle<BNtauCollection>            &x)  {  return  (taus                       =  x);  };
    edm::Handle<BNtrackCollection>          setTracks                    (edm::Handle<BNtrackCollection>          &x)  {  return  (tracks                     =  x);  };
    edm::Handle<BNtriggerCollection>        setTriggers                  (edm::Handle<BNtriggerCollection>        &x)  {  return  (triggers                   =  x);  };
    edm::Handle<BNtrigobjCollection>        setTrigobjs                  (edm::Handle<BNtrigobjCollection>        &x)  {  return  (trigobjs                   =  x);  };
    edm::Handle<double>                     setRhokt6CaloJetsHandle      (edm::Handle<double>                     &x)  {  return  (rhokt6CaloJetsHandle_      =  x);  };
    edm::Handle<vector< map<string,double> > > setUserVariables          (edm::Handle<vector< map<string,double> > > &x) { return  (userVariables             =  x);  };
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Public methods to assign values to each of the private variables.
    ////////////////////////////////////////////////////////////////////////////
    PUWeight*                               setPuWeight                  (PUWeight*                               x)   {  return  (puWeight_                  =  x);  };
    bool                                    setDoPileupReweighting       (bool                                    &x)  {  return  (doPileupReweighting_       =  x);  };
    bool                                    setFlagJESJERCorr            (bool                                    &x)  {  return  (flagJESJERCorr_            =  x);  };
    bool                                    setNmissoutShiftDown         (bool                                    &x)  {  return  (NmissoutShiftDown_         =  x);  };
    bool                                    setNmissoutShiftUp           (bool                                    &x)  {  return  (NmissoutShiftUp_           =  x);  };
    bool                                    setUseTrackCaloRhoCorr       (bool                                    &x)  {  return  (useTrackCaloRhoCorr_       =  x);  };
    double                                  setBTagScaleFactor           (double                                  &x)  {  return  (bTagScaleFactor_           =  x);  };
    double                                  setChannelScaleFactor        (double                                  &x)  {  return  (channelScaleFactor_        =  x);  };
    double                                  setEcaloVaryScaleFactor      (double                                  &x)  {  return  (EcaloVaryScaleFactor_      =  x);  };
    double                                  setElectronScaleFactor       (double                                  &x)  {  return  (electronScaleFactor_       =  x);  };
    double                                  setElectronTrackScaleFactor  (double                                  &x)  {  return  (electronTrackScaleFactor_  =  x);  };
    double                                  setEventScaleFactor          (double                                  &x)  {  return  (eventScaleFactor_          =  x);  };
    double                                  setGlobalScaleFactor         (double                                  &x)  {  return  (globalScaleFactor_         =  x);  };
    double                                  setIsrVaryScaleFactor        (double                                  &x)  {  return  (isrVaryScaleFactor_        =  x);  };
    double                                  setMuonScaleFactor           (double                                  &x)  {  return  (muonScaleFactor_           =  x);  };
    double                                  setMuonTrackScaleFactor      (double                                  &x)  {  return  (muonTrackScaleFactor_      =  x);  };
    double                                  setStopCTauScaleFactor       (double                                  &x)  {  return  (stopCTauScaleFactor_       =  x);  };
    double                                  setTopPtScaleFactor          (double                                  &x)  {  return  (topPtScaleFactor_          =  x);  };
    double                                  setTrackNMissOutScaleFactor  (double                                  &x)  {  return  (trackNMissOutScaleFactor_  =  x);  };
    double                                  setTriggerMetScaleFactor     (double                                  &x)  {  return  (triggerMetScaleFactor_     =  x);  };
    double                                  setTriggerScaleFactor        (double                                  &x)  {  return  (triggerScaleFactor_        =  x);  };
    flagMap                                 setCumulativeFlags           (flagMap                                 &x)  {  return  (cumulativeFlags            =  x);  };
    int                                     setVerbose                   (int                                     &x)  {  return  (verbose_                   =  x);  };
    string                                  setBadCSCFile                (string                                  &x)  {  return  (badCSCFile_                =  x);  };
    string                                  setDatasetType               (string                                  &x)  {  return  (datasetType_               =  x);  };
    string                                  setDeadEcalFile              (string                                  &x)  {  return  (deadEcalFile_              =  x);  };
    string                                  setJESJERCorr                (string                                  &x)  {  return  (jESJERCorr_                =  x);  };
    vector<BadCSC>                          setBadCSCVec                 (vector<BadCSC>                          &x)  {  return  (BadCSCVec                  =  x);  };
    vector<DeadEcal>                        setDeadEcalVec               (vector<DeadEcal>                        &x)  {  return  (DeadEcalVec                =  x);  };
    vector<string>                          setObjectsToCut              (vector<string>                          &x)  {  return  (objectsToCut               =  x);  };
    vector<string>                          setObjectsToGet              (vector<string>                          &x)  {  return  (objectsToGet               =  x);  };
    vector<string>                          setTargetTriggers            (vector<string>                          &x)  {  return  (targetTriggers_            =  x);  };
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Public methods which are used explicitly within OSUAnalysis.cc.
    ////////////////////////////////////////////////////////////////////////////
    const BNmet * chosenMET ();
    int getPdgIdBinValue(int pdgId);
    ////////////////////////////////////////////////////////////////////////////

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private utilitiy methods used by the valueLookup methods.
    ////////////////////////////////////////////////////////////////////////////
    BNjet getCorrectedJet(const BNjet &iJet, string jERCase);
    const BNelectron * chosenElectron ();
    const BNjet * chosenJet ();
    const BNmuon * chosenMuon ();
    const BNprimaryvertex * chosenVertex ();
    const BNtrack * chosenTrack ();
    double chosenHT ();
    double chosenST ();
    double etaToTheta(double eta);
    double getDetDeadEcalDeltaR (const BNtrack* track1);
    double getDetectorEta(double evntEta,double z0);
    double getHt (const BNjetCollection* jetColl);
    double getJERfactor(int returnType, double jetAbsETA, double genjetPT, double recojetPT);
    double getPtSingleObjectMatchesAnyTrigger(double recoPhi, const vector<string> & targetTriggers, const BNtrigobjCollection * triggerObjects);
    double getRhoCorr();
    double getSt (const BNelectronCollection* electronColl, const BNmuonCollection* muonColl, const BNjetCollection* jetColl);
    double getSumForwardJetPt (const BNjetCollection* jetColl);
    double getSumJetPt (const BNjetCollection* jetColl);
    double getTrkCaloTotRhoCorr(const BNtrack* track);
    double getTrkDeadEcalDeltaR (const BNtrack* track1);
    double getTrkDepTrkRp3RhoCorr(const BNtrack* track);
    double getTrkDepTrkRp5RhoCorr(const BNtrack* track);
    double getTrkPtRes (const BNtrack* track1);
    double getTrkPtTrue (const BNtrack* track1, const BNmcparticleCollection* genPartColl);
    double thetaToEta(double theta);
    vector<bool> getLastValidFlags(string objType);
    int findTauMotherIndex(const BNmcparticle* tau);
    int getTrkIsIso (const BNtrack* track1, const BNtrackCollection* trackColl);
    int getTrkIsMatchedBadCSC (const BNtrack* track1);
    int getTrkIsMatchedDeadEcal (const BNtrack* track1);
    int getTrkIsMatchedDeadEcalDet (const BNtrack* track1);
    pair<const BNelectron *, const BNelectron *> leadElectronPair ();
    pair<const BNmuon *, const BNmuon *> leadMuonPair ();
    template <class InputObject> double getGenDeltaRLowest(InputObject object);
    template <class InputObject> int getGenMatchedParticleIndex(InputObject object);
    template<typename T0, typename T1> const T1 * closest (const T0 *object0, const edm::Handle<vector<T1> > &object1Collection, const string &name);
    unsigned int GetNumExtraPartons(const BNmcparticleCollection* genPartColl);
    void WriteBadCSC();
    void WriteDeadEcal ();
    double getMember (const string, void *, const string);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Private object collections used by the valueLookup methods.
    ////////////////////////////////////////////////////////////////////////////
    edm::Handle<BNtriggerCollection>        triggers;
    edm::Handle<BNtrigobjCollection>        trigobjs;
    edm::Handle<BNjetCollection>            jets;
    edm::Handle<BNmuonCollection>           muons;
    edm::Handle<BNmuonCollection>           secMuons;
    edm::Handle<BNelectronCollection>       electrons;
    edm::Handle<BNeventCollection>          events;
    edm::Handle<BNtauCollection>            taus;
    edm::Handle<BNmetCollection>            mets;
    edm::Handle<BNtrackCollection>          tracks;
    edm::Handle<BNgenjetCollection>         genjets;
    edm::Handle<BNmcparticleCollection>     mcparticles;
    edm::Handle<BNprimaryvertexCollection>  primaryvertexs;
    edm::Handle<BNbxlumiCollection>         bxlumis;
    edm::Handle<BNphotonCollection>         photons;
    edm::Handle<BNsuperclusterCollection>   superclusters;
    edm::Handle<BNstopCollection>           stops;
    edm::Handle<double>                     rhokt6CaloJetsHandle_;
    edm::Handle<vector< map<string,double> > > userVariables;

    ////////////////////////////////////////////////////////////////////////////

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
    ////////////////////////////////////////////////////////////////////////////
};

// Definitions of the template methods.
#include "OSUT3Analysis/AnaTools/interface/ValueLookupTemplates.h"

#endif
