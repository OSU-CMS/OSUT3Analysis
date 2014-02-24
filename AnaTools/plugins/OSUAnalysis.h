#ifndef OSU_ANALYSIS

#define OSU_ANALYSIS

#include <map>
#include <string>
#include <vector>
#include <sstream>

#include <fstream>
#include <iostream>
#include <algorithm>
#include <regex>
#include "TH1.h"
#include "TH2.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TVector2.h"
#include "TTree.h"

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Common/interface/Handle.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/GeometrySurface/interface/Line.h"

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

#include "OSUT3Analysis/AnaTools/interface/BNstop.h"

#include "OSUT3Analysis/AnaTools/interface/CutFlow.h"
#include "OSUT3Analysis/AnaTools/interface/PUWeight.h"
#include "OSUT3Analysis/AnaTools/interface/SFWeight.h"
#include "OSUT3Analysis/AnaTools/interface/BtagSFWeight.h"
#include "OSUT3Analysis/AnaTools/interface/StopCTauWeight.h"

using namespace std;

class OSUAnalysis : public edm::EDProducer
  {
    public:
      OSUAnalysis (const edm::ParameterSet &);
      ~OSUAnalysis ();

      void produce (edm::Event &, const edm::EventSetup &);
      bool evaluateComparison (double, string, double);
      bool evaluateComparison (string, string, string);
      bool evaluateTriggers   (vector<string>, vector<string>, const BNtriggerCollection*);
      double applyFunction(string, double);

      double valueLookup (const BNjet* object, string variable, string function, string &stringValue);
      double valueLookup (const BNmuon* object, string variable, string function, string &stringValue);
      double valueLookup (const BNmuon* object1, const BNmuon* object2, string variable, string function, string &stringValue);

      double valueLookup (const BNelectron* object, string variable, string function, string &stringValue);
      double valueLookup (const BNelectron* object1, const BNelectron* object2, string variable, string function, string &stringValue);
      double valueLookup (const BNelectron* object1, const BNmuon* object2, string variable, string function, string &stringValue);
      double valueLookup (const BNelectron* object1, const BNjet* object2, string variable, string function, string &stringValue);
      double valueLookup (const BNelectron* object1, const BNphoton* object2, string variable, string function, string &stringValue);

      double valueLookup (const BNmuon* object1, const BNphoton* object2, string variable, string function, string &stringValue);
      double valueLookup (const BNmuon* object1, const BNevent* object2, string variable, string function, string &stringValue);
      double valueLookup (const BNelectron* object1, const BNevent* object2, string variable, string function, string &stringValue);

      double valueLookup (const BNphoton* object1, const BNjet* object2, string variable, string function, string &stringValue);

      double valueLookup (const BNelectron* object1, const BNtrack* object2, string variable, string function, string &stringValue);
      double valueLookup (const BNmuon* object1, const BNjet* object2, string variable, string function, string &stringValue);
      double valueLookup (const BNmet* object1, const BNjet* object2, string variable, string function, string &stringValue);  
      double valueLookup (const BNtrack* object1, const BNjet* object2, string variable, string function, string &stringValue);  
      double valueLookup (const BNmuon* object1, const BNtrack* object2, string variable, string function, string &stringValue);
      double valueLookup (const BNmuon* object1, const BNtau* object2, string variable, string function, string &stringValue);
      double valueLookup (const BNtau* object1, const BNtau* object2, string variable, string function, string &stringValue);
      double valueLookup (const BNtau* object1, const BNtrack* object2, string variable, string function, string &stringValue);
      double valueLookup (const BNjet* object1, const BNjet* object2, string variable, string function, string &stringValue);

      double valueLookup (const BNevent* object, string variable, string function, string &stringValue);
      double valueLookup (const BNtau* object, string variable, string function, string &stringValue);
      double valueLookup (const BNmet* object, string variable, string function, string &stringValue);
      double valueLookup (const BNtrack* object, string variable, string function, string &stringValue);

      double valueLookup (const BNtrack* object1, const BNevent* object2, string variable, string function, string &stringValue);
      double valueLookup (const BNgenjet* object, string variable, string function, string &stringValue);
      double valueLookup (const BNmcparticle* object, string variable, string function, string &stringValue);

      double valueLookup (const BNelectron* object1, const BNmcparticle* object, string variable, string function, string &stringValue);
      double valueLookup (const BNmuon* object1, const BNmcparticle* object, string variable, string function, string &stringValue);
      double valueLookup (const BNtrack* object1,   const BNmcparticle* object2, string variable, string function, string &stringValue);  


      double valueLookup (const BNprimaryvertex* object, string variable, string function, string &stringValue);
      double valueLookup (const BNbxlumi* object, string variable, string function, string &stringValue);
      double valueLookup (const BNphoton* object, string variable, string function, string &stringValue);
      double valueLookup (const BNsupercluster* object, string variable, string function, string &stringValue);
      double valueLookup (const BNtrigobj* object, string variable, string function, string &stringValue);
      double valueLookup (const BNelectron* object1, const BNtrigobj* object2, string variable, string function, string &stringValue);
      double valueLookup (const BNmuon* object1, const BNtrigobj* object2, string variable, string function, string &stringValue);
      double valueLookup (const BNstop* object, string variable, string function, string &stringValue);

      int getTrkIsIso (const BNtrack* track1, const BNtrackCollection* trackColl);
      double getTrkPtTrue (const BNtrack* track1, const BNmcparticleCollection* genPartColl);
      double getHt (const BNjetCollection* jetColl);
      unsigned int GetNumExtraPartons (const BNmcparticleCollection* genPartColl);
      double getSt (const BNelectronCollection* electronColl, const BNmuonCollection* muonColl, const BNjetCollection* jetColl);
      double getTrkPtRes (const BNtrack* track1);
      double getTrkIsIso (const BNtrack* track1);
      double getTrkCaloTotRhoCorr(const BNtrack* track);
      double getRhoCorr(const BNtrack* track);
      double getTrkDepTrkRp5RhoCorr(const BNtrack* track);
      double getVariableBinsWeights(vector<double> , double );
      double getTrkDepTrkRp3RhoCorr(const BNtrack* track);
      void WriteDeadEcal();
      void WriteBadCSC();
      int getTrkIsMatchedDeadEcal (const BNtrack* track1);
      double getTrkDeadEcalDeltaR (const BNtrack* track1);
      int getTrkIsMatchedBadCSC   (const BNtrack* track1);  

      //BNskimbits
      //BNtrigobj

      vector<string> splitString (string);
      void getTwoObjs(string tempInputCollection, string& obj1, string& obj2);
      string getObjToGet(string obj);

    private:

      //flagMap:
      //string       holds input collection type
      //outer vector corresponds to each cut
      //inner vector corresponds to each object in input collection
      //pair(bool) first bool counts towards the event passing, second bool determines whether to plot the object        
      typedef map<string, vector<vector<pair<bool,bool> > > > flagMap;

      //flagPair (corresponds to flagMap):  
      //vector corresponds to each object in input collection
      //pair(bool) first bool counts towards the event passing, second bool determines whether to plot the object        
      typedef vector<pair<bool,bool> > flagPair;

      //counterMap:
      //string holds input collection type
      //vector corresponds to each cut
      //int    holds number of objects passing all cuts up to that point
      typedef map<string ,vector <int> > counterMap;

      // Remember to define parameters to be retrieved from the configuration file.
      edm::InputTag jets_;
      edm::InputTag muons_;
      edm::InputTag secMuons_;
      edm::InputTag electrons_;
      edm::InputTag events_;
      edm::InputTag taus_;
      edm::InputTag mets_;
      edm::InputTag tracks_;
      edm::InputTag genjets_;
      edm::InputTag mcparticles_;
      edm::InputTag stops_;
      edm::InputTag primaryvertexs_;
      edm::InputTag bxlumis_;
      edm::InputTag photons_;
      edm::InputTag superclusters_;
      edm::InputTag triggers_;
      edm::InputTag trigobjs_;
      string puFile_;
      string deadEcalFile_;
      string badCSCFile_;
      string electronSFFile_;
      string muonSFFile_;
      string dataPU_;
      string electronSFID_;
      string electronSF_;
      string muonSF_;
      string dataset_;
      string datasetType_;
      vector<edm::ParameterSet> channels_;
      vector<edm::ParameterSet> histogramSets_;
      bool useEDMFormat_;
      vector<edm::ParameterSet> treeBranchSets_;  
      bool plotAllObjectsInPassingEvents_;
      bool doPileupReweighting_;
      bool doTopPtReweighting_;
      bool applyTriggerSF_;
      double triggerScaleFactor_;
      double trackingScaleFactor_;
      bool applyLeptonSF_;
      bool applyTrackingSF_;
      bool applyBtagSF_;
      int  minBtag_;
      string electronSFShift_;
      string muonSFShift_;
      string trackSFShift_;
      bool printEventInfo_;
      bool printAllTriggers_;
      bool useTrackCaloRhoCorr_;  // to use the calo-based rho correction for the by-hand calculation of the track isolation energy
      vector<double> stopCTau_;
      bool GetPlotsAfterEachCut_;
      int verbose_;
      
      struct DeadEcal {
        double etaEcal;
        double phiEcal;
      };
      vector<DeadEcal> DeadEcalVec;

      struct BadCSC {
        double etaCSC;
        double phiCSC;
      };
      vector<BadCSC> BadCSCVec;


      //Collections
      edm::Handle<BNtriggerCollection> triggers;
      edm::Handle<BNtrigobjCollection> trigobjs;
      edm::Handle<BNjetCollection> jets;
      edm::Handle<BNmuonCollection> muons;
      edm::Handle<BNmuonCollection> secMuons;
      edm::Handle<BNelectronCollection> electrons;
      edm::Handle<BNeventCollection> events;
      edm::Handle<BNtauCollection> taus;
      edm::Handle<BNmetCollection> mets;
      edm::Handle<BNtrackCollection> tracks;
      edm::Handle<BNgenjetCollection> genjets;
      edm::Handle<BNmcparticleCollection> mcparticles;
      edm::Handle<BNprimaryvertexCollection> primaryvertexs;
      edm::Handle<BNbxlumiCollection> bxlumis;
      edm::Handle<BNphotonCollection> photons;
      edm::Handle<BNsuperclusterCollection> superclusters;
      edm::Handle<BNstopCollection> stops;

      edm::Handle<double> rhokt6CaloJetsHandle_;

      flagMap cumulativeFlags;
      flagMap vetoFlags;

      vector<vector<map<string, TH1D*>>> oneDHists_;
      vector<vector<map<string, TH2D*>>> twoDHists_;
      vector<TTree*> BNTrees_;  // one tree per channel  
      map<string, vector<float>> BNTreeBranchVals_;  // data structure to hold the values of the branches to be stored in the BNTrees_; the string is the name of a variable  
      long BNTreeBranchVals_evtLong_;  // event number  
      int  BNTreeBranchVals_runInt_;   // run number  
      int  BNTreeBranchVals_lumiInt_;  // lumi number  

      edm::Service<TFileService> fs_;

      CutFlow *masterCutFlow_;
      vector<CutFlow *> cutFlows_;

      typedef struct {
        string name;
        string title;
        vector<double> bins;
        vector<double> variableBinsX;
        vector<double> variableBinsWeightsX;
        vector<double> variableBinsY;
        string inputCollection;
        vector<string> inputVariables;
      } histogram;

      typedef struct {
        string name;
        string inputCollection;
        string inputVariable;
      } BranchSpecs;


      struct cut {
        string inputCollection;
        vector<string> functions;
        vector<string> variables;
        vector<string> comparativeOperators;// >, <, =, etc.
        vector<double> cutValues;
        vector<string> cutStringValues;
        vector<string> logicalOperators;//and, or
        int    numSubcuts;
        int    numberRequired;
        string eventComparativeOperator;
        string name;
	bool   isVeto;
      };

      struct channel {
        string name;
	vector<int> inStatus3Outgoing;
	vector<int> notInStatus3Outgoing;
	vector<int> absInStatus3Outgoing;
	vector<int> absNotInStatus3Outgoing;
        string inStatus3OutgoingCutName;
        string notInStatus3OutgoingCutName;
        vector<string> triggers;
        vector<string> triggersToVeto;
        vector<cut> cuts;
      };

      vector<string>  objectsToGet;
      vector<string>  objectsToCut;
      vector<string>  objectsToPlot;
      vector<string>  objectsToFlag;

      vector<channel> channels;
      vector<histogram> histograms;
      vector<BranchSpecs> treeBranches_;

      PUWeight *puWeight_;
      MuonSFWeight *muonSFWeight_;
      double muonScaleFactor_;
      ElectronSFWeight *electronSFWeight_;
      double electronScaleFactor_;
      TrackSFWeight *trackSFWeight_;
      double muonTrackScaleFactor_ ;
      double electronTrackScaleFactor_ ;
      BtagSFWeight *bTagSFWeight_;
      double bTagScaleFactor_;     

      double topPtScaleFactor_;

      StopCTauWeight *stopCTauWeight_;
      double stopCTauScaleFactor_;

      double globalScaleFactor_;
      double channelScaleFactor_;
      double eventScaleFactor_;

      ofstream* findEventsLog; 
      bool isFirstEvent_;  

      template <class InputCollection> void setObjectFlags(cut &, uint, flagMap &, flagMap &, InputCollection, string);
      template <class InputCollection1, class InputCollection2> void setObjectFlags(cut &, uint, flagMap &, flagMap &, InputCollection1, InputCollection2, string);

      template <class InputCollection> void assignTreeBranch(BranchSpecs brSpecs, InputCollection inputCollection, flagPair flags);  
      template <class InputCollection> void fill1DHistogram(TH1*, histogram, InputCollection, flagPair, double);
      template <class InputCollection1, class InputCollection2> void fill1DHistogram(TH1*, histogram, InputCollection1, InputCollection2, flagPair, double);
      void fill1DStatus3Histogram(TH1* histo, const BNmcparticleCollection *inputCollection, double scaleFactor);
      template <class InputCollection> void fill2DHistogram(TH2*, histogram, InputCollection, flagPair, double);
      template <class InputCollection1, class InputCollection2> void fill2DHistogram(TH2*, histogram, InputCollection1, InputCollection2, flagPair, double);
      bool getPreviousCumulativeFlags(uint currentCutIndex, flagMap &individualFlags, string obj1Type, uint object1, string flagType);



      template <class InputObject> int getGenMatchedParticleIndex(InputObject);
      int getPdgIdBinValue(int);
      int findTauMotherIndex(const BNmcparticle*);

      template <class InputObject> double getGenDeltaRLowest(InputObject);


      const BNprimaryvertex *chosenVertex ();
      const BNmet *chosenMET ();
      const BNelectron *chosenElectron ();
      const BNmuon *chosenMuon ();
      template<typename T0, typename T1> const T1 *closest (const T0 *, const edm::Handle<vector<T1> > &, const string &);
      double chosenHT ();
      double chosenST ();

      pair<const BNmuon *, const BNmuon*> leadMuonPair ();
      pair<const BNelectron *, const BNelectron*> leadElectronPair ();

      double getTopPtWeight();
  };

#endif
