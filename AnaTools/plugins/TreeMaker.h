#ifndef TREEMAKER
#define TREEMAKER

#include <tuple>
#include <unordered_set>

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

#include "TTree.h"

class TreeMaker : public edm::EDAnalyzer
{
    public:

      TreeMaker (const edm::ParameterSet &);
      ~TreeMaker ();
      void analyze(const edm::Event&, const edm::EventSetup&);

    private:

      edm::ParameterSet collections_;
      vector<edm::ParameterSet> weightDefs_;
      vector<edm::ParameterSet> branchSets_;
      int verbose_;
      bool firstEvent_;

      //Collections
      Collections handles_;
      Tokens tokens_;

      bool initializeValueLookupForest (vector<BranchDef> &, Collections * const);
      bool initializeValueLookupForest (vector<Weight> &, Collections * const);

      edm::Service<TFileService> fs_;

      unordered_set<string> objectsToGet_;

      vector<TFileDirectory> subDirs;

      vector<BranchDef> branchDefinitions;
      vector<Weight> weights;
      double generatorWeight_;
      double weightProduct_;

      BranchDef parseBranchDef(const edm::ParameterSet &, const vector<string> &, const string &);
      BranchDef parseHistoDef(const edm::ParameterSet &, const vector<string> &, const string &);

      void bookTree(vector<BranchDef> &, vector<Weight> &);
      void fillTree(vector<BranchDef> &, vector<Weight> &);

};

#endif
