#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"
#include "OSUT3Analysis/AnaTools/plugins/TreeMaker.h"

#define EXIT_CODE 5

// The TreeMaker class handles user-defined trees
// As input it takes "branch definitions", which are each composed of:
//   1. names of collections to be used
//   2. expression for the calcualtion of the branch
// All individual weights and their product will be included as another branch
// The output is a root file containing the trees for each channel

// Note: a "branch definition" is a subset of a "histogram definition" for the Plotter,
//       so it is possible to include those in the TreeMaker output. It will ignore any
//       2d/3d histograms and take only the title and x-axis variables as its input

// Important note: the tree is filled once per event, which means that for example, a
//                 branch "muonPt" will overwrite itself if there are multiple muons
//                 in the event. Use the "index" requirement judiciously to avoid this!

TreeMaker::TreeMaker(const edm::ParameterSet &cfg) :

  // In the constructor, we parse the input branch definitions
  // We book a TTree object named after the channel for which it's created

  /// Retrieve parameters from the configuration file.
  collections_(cfg.getParameter<edm::ParameterSet>("collections")),
  weightDefs_ (cfg.getParameter<vector<edm::ParameterSet> >("weights")),
  branchSets_ (cfg.getParameter<vector<edm::ParameterSet> >("branchSets")),
  verbose_    (cfg.getParameter<int>("verbose")),
  firstEvent_ (true)
{
  if(verbose_) clog << "Beginning TreeMaker::TreeMaker constructor." << endl;

  //////////////////////////////////
  // parse the branch definitions //
  //////////////////////////////////

  // loop over each branch set the user has included
  for(unsigned branchSet = 0; branchSet != branchSets_.size(); branchSet++) {

    vector<string> inputCollection = branchSets_.at(branchSet).getParameter<vector<string> >("inputCollection");
    
    string collectionPrefix = "";
    for(auto collection = inputCollection.begin(); collection != inputCollection.end(); collection++) {
      collectionPrefix += anatools::singular (*collection) + "_";
    }

    objectsToGet_.insert(inputCollection.begin(), inputCollection.end());
#if DATA_FORMAT_FROM_MINIAOD
    objectsToGet_.insert("generatorweights");
#endif

    bool isHistoDef = branchSets_.at(branchSet).exists("histograms");

    // import all the branch definitions for the current set
    // to support trees made from histogramSets, accept either "histograms" or "branches"
    vector<edm::ParameterSet> branchList(
      isHistoDef ? 
        branchSets_.at(branchSet).getParameter<vector<edm::ParameterSet> >("histograms") :
        branchSets_.at(branchSet).getParameter<vector<edm::ParameterSet> >("branches")
      );

    // loop over each branch
    vector<edm::ParameterSet>::const_iterator branch;
    for(branch = branchList.begin(); branch != branchList.end(); ++branch) {

      BranchDef branchDefinition = isHistoDef ? 
        parseHistoDef(*branch, inputCollection, collectionPrefix) :
        parseBranchDef(*branch, inputCollection, collectionPrefix);

      // check whether a branch of the same name already exists; if not, add to the master list
      bool alreadyExists = false;
      for(auto &branch : branchDefinitions) {
        if(branch.branchName == branchDefinition.branchName) {
          alreadyExists = true;
          break;
        }
      }

      if(alreadyExists) cerr << "WARNING: Found duplicate branch with name " << branchDefinition.branchName
                             << "; will only keep the first instance." << endl;
      else branchDefinitions.push_back(branchDefinition);
    } // end loop on branches in the set

  } // end loop on branch sets

  //////////////////////////////////
  // parse the weight definitions //
  //////////////////////////////////

  for(unsigned weightDef = 0; weightDef != weightDefs_.size(); weightDef++){
    vector<string> inputCollections = weightDefs_.at(weightDef).getParameter<vector<string> >("inputCollections");
    vector<string>::iterator inputCollection;
    for(inputCollection = inputCollections.begin(); inputCollection != inputCollections.end(); ++inputCollection){
      objectsToGet_.insert(*inputCollection);
    }
    string inputVariable = weightDefs_.at(weightDef).getParameter<string>("inputVariable");
    Weight weight;
    weight.inputCollections = inputCollections;
    weight.inputVariable = inputVariable;
    weight.valueLookupTree = NULL;
    weight.product = 1.0;
    weights.push_back(weight);
  }

  // book a single tree with all of these branches and the weights
  bookTree(branchDefinitions, weights);

  anatools::getAllTokens(collections_, consumesCollector(), tokens_);
}

////////////////////////////////////////////////////////////////////////

// in the analyze function, we retrieve all the needed collections from the event
// and then loop on over the histogram sets and fill each histogram

void
TreeMaker::analyze(const edm::Event &event, const edm::EventSetup &setup)
{
  // get the required collections from the event
  anatools::getRequiredCollections(objectsToGet_, handles_, event, tokens_);

  if(!initializeValueLookupForest(branchDefinitions, &handles_)) {
    clog << "ERROR: failed to parse input variables. Quitting..." << endl;
    exit(EXIT_CODE);
  }

  // first we'll calculate all the weights for this event
  if(!initializeValueLookupForest(weights, &handles_)) {
    clog << "ERROR: failed to parse weight definitions. Quitting..." << endl;
    exit(EXIT_CODE);
  }

  for(vector<Weight>::iterator weight = weights.begin(); weight != weights.end(); weight++) {
    weight->product = 1.0;
    for(vector<Leaf>::const_iterator leaf = weight->valueLookupTree->evaluate().begin(); leaf != weight->valueLookupTree->evaluate().end(); leaf++) {
      double value = boost::get<double>(*leaf);
      if(IS_INVALID(value)) continue;
      weight->product *= value;
    }
  }

  // now fill the tree
  fillTree(branchDefinitions, weights);

  firstEvent_ = false;
}

////////////////////////////////////////////////////////////////////////

TreeMaker::~TreeMaker()
{
  for(auto &branch : branchDefinitions) {
    for(auto &valueLookupTree : branch.valueLookupTrees)
      delete valueLookupTree;
  }

  for(auto &weight : weights) {
      if(weight.valueLookupTree)
        delete weight.valueLookupTree;
  }
}

////////////////////////////////////////////////////////////////////////

// parses a branch configuration and saves it in a C++ container
BranchDef TreeMaker::parseBranchDef(const edm::ParameterSet &definition, const vector<string> &inputCollections, const string &collectionPrefix) {

  BranchDef parsedDef;

  parsedDef.inputCollections = inputCollections;
  parsedDef.branchName = collectionPrefix + definition.getParameter<string>("name");
  parsedDef.index = definition.getUntrackedParameter<int>("index", INVALID_VALUE);
  parsedDef.inputVariables = definition.getParameter<vector<string> >("inputVariables");
  parsedDef.value = INVALID_VALUE;

  return parsedDef;  
}

// parses a branch configuration and saves it in a C++ container
BranchDef TreeMaker::parseHistoDef(const edm::ParameterSet &definition, const vector<string> &inputCollections, const string &collectionPrefix){

  BranchDef parsedDef;

  parsedDef.inputCollections = inputCollections;
  parsedDef.branchName = collectionPrefix + definition.getParameter<string>("name");
  parsedDef.index = definition.getUntrackedParameter<int>("indexX", INVALID_VALUE);
  parsedDef.inputVariables = definition.getParameter<vector<string> >("inputVariables");
  parsedDef.value = INVALID_VALUE;

  return parsedDef;
}

////////////////////////////////////////////////////////////////////////

// book the tree with all of the declared branches (and weights)
void TreeMaker::bookTree(vector<BranchDef> &branches, vector<Weight> &weights) {

  TTree * tree = fs_->make<TTree>("Tree", "Tree for analysis");

  for(auto &b : branches) tree->Branch(TString(b.branchName), &(b.value));
  for(auto &w : weights)  tree->Branch(TString("weights_") + TString(w.inputVariable), &(w.product));

  if(handles_.generatorweights.isValid()) tree->Branch("weights_generatorWeight", &generatorWeight_);

  if(weights.size() > 0) tree->Branch("weights_weightProduct", &weightProduct_);
}

////////////////////////////////////////////////////////////////////////

void TreeMaker::fillTree(vector<BranchDef> & branches, vector<Weight> &weights) {

  TTree * tree = fs_->getObject<TTree>("Tree");

  // set values for branches
  for(auto &b : branches) {

    // set to an invalid value first, in case no such object is present
    b.value = INVALID_VALUE;

    // loop over objects and set their values
    for(vector<Leaf>::const_iterator leaf = b.valueLookupTrees.at(0)->evaluate().begin(); leaf != b.valueLookupTrees.at(0)->evaluate().end(); leaf++) {

      // if an index (pt-ordered) is required, check this      
      if(!IS_INVALID(b.index) && leaf - b.valueLookupTrees.at(0)->evaluate().begin() != b.index) continue;

      // set the value
      b.value = boost::get<double>(*leaf);
    }

  } // for branches

  if(handles_.generatorweights.isValid()) generatorWeight_ = anatools::getGeneratorWeight(*handles_.generatorweights);

  if(weights.size() > 0) {
    weightProduct_ = 1.0;
    for(auto &w : weights) weightProduct_ *= w.product;
  }

  tree->Fill();

}

////////////////////////////////////////////////////////////////////////

bool
TreeMaker::initializeValueLookupForest(vector<BranchDef> &branches, Collections * const handles)
{
  //////////////////////////////////////////////////////////////////////////////
  // For each inputVariable of each branch, parse it into a new
  // ValueLookupTree object which is stored in the branch definition
  // structure.
  //////////////////////////////////////////////////////////////////////////////
  for(vector<BranchDef>::iterator branch = branches.begin(); branch != branches.end(); branch++) {
    if(firstEvent_) {
      for(vector<string>::const_iterator inputVariable = branch->inputVariables.begin(); inputVariable != branch->inputVariables.end(); inputVariable++)
        branch->valueLookupTrees.push_back(new ValueLookupTree(*inputVariable, branch->inputCollections));
      if(!branch->valueLookupTrees.back()->isValid())
        return false;
    }
    for(vector<ValueLookupTree *>::iterator lookupTree = branch->valueLookupTrees.begin(); lookupTree != branch->valueLookupTrees.end(); lookupTree++)
     (*lookupTree)->setCollections(handles);
  }
  return true;
  //////////////////////////////////////////////////////////////////////////////
}


bool
TreeMaker::initializeValueLookupForest(vector<Weight> &weights, Collections * const handles)
{
  //////////////////////////////////////////////////////////////////////////////
  // For each inputVariable of each weight, parse it into a new
  // ValueLookupTree object which is stored in the weight definition
  // structure.
  //////////////////////////////////////////////////////////////////////////////
  for(vector<Weight>::iterator weight = weights.begin(); weight != weights.end(); weight++) {
    if(firstEvent_) {
      weight->valueLookupTree = new ValueLookupTree(weight->inputVariable, weight->inputCollections);
      if(!weight->valueLookupTree->isValid())
        return false;
    }
    weight->valueLookupTree->setCollections(handles);
  }
  return true;
  //////////////////////////////////////////////////////////////////////////////
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TreeMaker);
