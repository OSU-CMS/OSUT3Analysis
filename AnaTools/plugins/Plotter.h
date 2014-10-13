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

#include <vector>
#include <string>

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



      // Remember to define parameters to be retrieved from the configuration file.
/*       edm::InputTag jets_; */
      edm::InputTag muons_;
/*       edm::InputTag secMuons_; */
      edm::InputTag electrons_;
/*       edm::InputTag taus_; */
/*       edm::InputTag mets_; */
/*       edm::InputTag tracks_; */
/*       edm::InputTag genjets_; */
/*       edm::InputTag mcparticles_; */
/*       edm::InputTag primaryvertexs_; */
/*       edm::InputTag bxlumis_; */
/*       edm::InputTag photons_; */
/*       edm::InputTag superclusters_; */
/*       edm::InputTag triggers_; */
/*       edm::InputTag trigobjs_; */


      //Collections
/*       edm::Handle<BNtriggerCollection> triggers; */
/*       edm::Handle<BNtrigobjCollection> trigobjs; */
/*       edm::Handle<BNjetCollection> jets; */
/*       edm::Handle<BNmuonCollection> muons; */
/*       edm::Handle<BNelectronCollection> electrons; */
/*       edm::Handle<BNeventCollection> events; */
/*       edm::Handle<BNtauCollection> taus; */
/*       edm::Handle<BNmetCollection> mets; */
/*       edm::Handle<BNtrackCollection> tracks; */
/*       edm::Handle<BNgenjetCollection> genjets; */
/*       edm::Handle<BNmcparticleCollection> mcparticles; */
/*       edm::Handle<BNprimaryvertexCollection> primaryvertexs; */
/*       edm::Handle<BNbxlumiCollection> bxlumis; */
/*       edm::Handle<BNphotonCollection> photons; */
/*       edm::Handle<BNsuperclusterCollection> superclusters; */

/*       vector<vector<map<string, TH1D*>>> oneDHists_; */
/*       vector<vector<map<string, TH2D*>>> twoDHists_; */

      vector<edm::ParameterSet> histogramSets_;

      int verbose_;

      edm::Service<TFileService> fs_;

      vector<string> objectsToGet;



      struct histoDef {
        string inputCollection;
        TFileDirectory *directory;
        string name;
        string title; // contains axis labels
        vector<double> binsX;
        vector<double> binsY;
        vector<string> inputVariables;
        int dimensions;
      };

      vector<histoDef> histogramDefinitions;

      string getDirectoryName(const string);
      vector<string> getInputTypes(const string);
      string fixOrdering(const string);
      histoDef parseHistoDef(const edm::ParameterSet, string, TFileDirectory*);
      void bookHistogram(Plotter::histoDef);



};


#endif
