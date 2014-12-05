#ifndef PLOTTER

#define PLOTTER

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Common/interface/Handle.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "BEAN/Collections/interface/BNelectron.h"
#include "BEAN/Collections/interface/BNmuon.h"

#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"
#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

#include <vector>
#include <string>

#include "TH1.h"
#include "TH2.h"


using namespace std;


class Plotter : public edm::EDAnalyzer
{
    public:

      Plotter (const edm::ParameterSet &);
      ~Plotter ();
      void analyze(const edm::Event&, const edm::EventSetup&);

      string capitalize(const string);
      string singular(const string);
      string plural(const string);

    private:

      edm::ParameterSet collections_;

      // Remember to define parameters to be retrieved from the configuration file.
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
      
      
      //Collections
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


      vector<edm::ParameterSet> histogramSets_;

      int verbose_;

      bool firstEvent_;

      ValueLookup *vl_;
      bool initializeValueLookupTrees (vector<histoDef> &);
      void initializeValueLookup ();

      edm::Service<TFileService> fs_;

      vector<string> objectsToGet;

      vector<TFileDirectory> subDirs;

      vector<histoDef> histogramDefinitions;

      string getDirectoryName(const string);
      vector<string> getInputTypes(const string);
      string fixOrdering(const string);
      histoDef parseHistoDef(const edm::ParameterSet, const string, const string);
      void bookHistogram(const histoDef);
      pair<string,string> getVariableAndFunction(const string);

      template <class InputCollection> void fillHistogram(const histoDef, const InputCollection);
      template <class InputCollection1, class InputCollection2> void fillHistogram(const histoDef,
										   const InputCollection1,
										   const InputCollection2);

      template <class InputCollection> void fill1DHistogram(const histoDef, const InputCollection);
      template <class InputCollection1, class InputCollection2> void fill1DHistogram(const histoDef,
										     const InputCollection1,
										     const InputCollection2);

      template <class InputCollection> void fill2DHistogram(const histoDef, const InputCollection);
      template <class InputCollection1, class InputCollection2> void fill2DHistogram(const histoDef,
										     const InputCollection1,
										     const InputCollection2);

      double getBinSize(TH1D *, const double);
      pair<double,double> getBinSize(TH2D *, const double, const double);
      string setYaxisLabel(const histoDef);


};





#endif
