#include <algorithm>
#include <iostream>
#include <set>
#include <unordered_map>

#include "FWCore/Common/interface/TriggerNames.h"

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"
#include "OSUT3Analysis/AnaTools/plugins/CutCalculator.h"

#define EXIT_CODE 1

CutCalculator::CutCalculator (const edm::ParameterSet &cfg) :
  collections_    (cfg.getParameter<edm::ParameterSet>  ("collections")),
  cuts_           (cfg.getParameter<edm::ParameterSet>  ("cuts")),
  triggersInMenu_ (true),
  firstEvent_     (true)
{

  //////////////////////////////////////////////////////////////////////////////
  // Try to unpack the cuts ParameterSet and quit if there is a problem.
  //////////////////////////////////////////////////////////////////////////////
  if (!unpackCuts ())
    {
      clog << "ERROR: failed to interpret cuts PSet. Quitting..." << endl;
      exit (EXIT_CODE);
    }
  //////////////////////////////////////////////////////////////////////////////

  triggerNamesPSetID_.reset ();
  triggerIndices_.clear ();

  anatools::getAllTokens (collections_, consumesCollector (), tokens_);

  produces<CutCalculatorPayload> ("cutDecisions");
}

CutCalculator::~CutCalculator ()
{

   for (auto &cut : unpackedCuts_)
     {
       if (cut.valueLookupTree)
         delete cut.valueLookupTree;
       if (cut.arbitrationTree)
         delete cut.arbitrationTree;
     }
}

void
CutCalculator::produce (edm::Event &event, const edm::EventSetup &setup)
{
  anatools::getRequiredCollections (objectsToGet_, handles_, event, tokens_);
  //////////////////////////////////////////////////////////////////////////////
  // Set all the private variables in the ValueLookup object before using it,
  // and parse the cut strings in the unpacked cuts into ValueLookupTree
  // objects.
  //////////////////////////////////////////////////////////////////////////////
  if (!initializeValueLookupForest (unpackedCuts_, &handles_))
    {
      clog << "ERROR: failed to parse all cut strings. Quitting..." << endl;
      exit (EXIT_CODE);
    }
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // Create the payload for this EDProducer and initialize some of its members.
  //////////////////////////////////////////////////////////////////////////////
  pl_ = unique_ptr<CutCalculatorPayload> (new CutCalculatorPayload);
  pl_->isValid = true;
  pl_->cuts = unpackedCuts_;
  pl_->triggers = unpackedTriggers_;
  pl_->triggersToVeto = unpackedTriggersToVeto_;
  pl_->triggerFilters = unpackedTriggerFilters_;
  pl_->triggersInMenu = unpackedTriggersInMenu_;
  pl_->metFilters = unpackedMETFilters_;
  //////////////////////////////////////////////////////////////////////////////

  // getListOfObjects
  // for each cut:
  //   setInputCollectionFlags
  //   arbitrateInputCollectionFlags
  //   propagateFromSingleCollections
  //   propagateFromCompositeCollections
  //   setOtherCollectionsFlags

  vector<string> listOfObjects = getListOfObjects(pl_->cuts);

  // Loop over cuts to set flags for each object indicating whether it passed
  // the cut.
  for (unsigned currentCutIndex = 0; pl_->isValid && currentCutIndex != pl_->cuts.size (); currentCutIndex++)
    {
      Cut currentCut = pl_->cuts.at (currentCutIndex);

      // Sets the flags for the current cut only for the objects which are
      // being cut on.
      pl_->isValid = setInputCollectionFlags (currentCut, currentCutIndex);

      // If the cut has an arbitration parameter, adjust flags accordingly
      pl_->isValid = arbitrateInputCollectionFlags (currentCut, currentCutIndex);

      // Copy flags to any composite collections containing the inputCollection, e.g. muons -> muon-jets
      pl_->isValid = propagateFromSingleCollections (currentCut, currentCutIndex, listOfObjects);

      // Copy flags to any component collections contained in the inputCollection, e.g. muon-jets -> muons, jets, muon-muons, etc.
      pl_->isValid = propagateFromCompositeCollections (currentCut, currentCutIndex, listOfObjects);

      // Set flags for all collections unrelated to the cut equal to true
      pl_->isValid = setOtherCollectionsFlags (currentCut, currentCutIndex, listOfObjects);
    }

  //////////////////////////////////////////////////////////////////////////////
  // Quit if there was a problem setting the flags for any of the objects.
  //////////////////////////////////////////////////////////////////////////////
  if (!pl_->isValid)
    {
      clog << "ERROR: failed to set flags. Quitting..." <<  endl;
      exit (EXIT_CODE);
    }
  //////////////////////////////////////////////////////////////////////////////

  // Decide whether the event passes the triggers specified by the user and
  // store the decision in the payload.
  evaluateTriggers (event);
  evaluateTriggerFilters (event);
  evaluateMETFilters (event);

  // Decide whether the event passes each cut
  // by counting the number of objects passing the cut
  // also AND together cut and trigger decision
  setEventFlags ();

  event.put (std::move (pl_), "cutDecisions");
  pl_.reset ();
  firstEvent_ = false;
}

bool
CutCalculator::setInputCollectionFlags (const Cut &currentCut, unsigned currentCutIndex) const
{
  ////////////////////////////////////////////////////////////////////////////////
  // Prepare the flag maps for the new cut by increasing the size of the vector
  // and adding the input label as a key to the map.
  ////////////////////////////////////////////////////////////////////////////////

  if (currentCutIndex >= pl_->individualObjectFlags.size ()){
    pl_->individualObjectFlags.resize (currentCutIndex + 1);
  }
  pl_->individualObjectFlags.at (currentCutIndex)[currentCut.inputLabel];

  if (currentCutIndex >= pl_->cumulativeObjectFlags.size ()){
    pl_->cumulativeObjectFlags.resize (currentCutIndex + 1);
  }
  pl_->cumulativeObjectFlags.at (currentCutIndex)[currentCut.inputLabel];

  ////////////////////////////////////////////////////////////////////////////////
  // extract decision from valueLookupTree and store in corresponding flag
  ////////////////////////////////////////////////////////////////////////////////

  for (auto cutDecision = currentCut.valueLookupTree->evaluate ().begin (); cutDecision != currentCut.valueLookupTree->evaluate ().end (); cutDecision++)
    {
      double value = boost::get<double> (*cutDecision);
      pair<bool, bool> flag = make_pair (value, !IS_INVALID(value));

      // invert flags if this cut is a veto
      if (currentCut.isVeto)
        flag.first = !flag.first;

      pl_->individualObjectFlags.at (currentCutIndex).at (currentCut.inputLabel).push_back (flag);
      pl_->cumulativeObjectFlags.at (currentCutIndex).at (currentCut.inputLabel).push_back (flag);

    }

  //!!!
  for (unsigned index = 0; index != pl_->cumulativeObjectFlags.at(currentCutIndex).at(currentCut.inputLabel).size(); index++) {

    // AND together cumulative flags from previous cuts with the one for the current cut
    for (unsigned cutIndex = 0; cutIndex != currentCutIndex; cutIndex++){
      pl_->cumulativeObjectFlags.at (currentCutIndex).at (currentCut.inputLabel).at (index).first = pl_->cumulativeObjectFlags.at (currentCutIndex).at (currentCut.inputLabel).at (index).first && pl_->cumulativeObjectFlags.at (cutIndex).at (currentCut.inputLabel).at (index).first;
    }

  }
  return true;
}


bool
CutCalculator::arbitrateInputCollectionFlags (const Cut &currentCut, unsigned currentCutIndex) const
{
  ////////////////////////////////////////////////////////////////////////////////
  // If the user has given an expression for the arbitration, use it to pick
  // one of the passing objects. For example, if arbitration == "pt", the
  // passing object with the largest pt is chosen, and the flags for all others
  // are changed to false. The special case of arbitration == "random" is used
  // to chose a random object.
  ////////////////////////////////////////////////////////////////////////////////
  if (currentCut.arbitration != "")
    {
      vector<pair<unsigned, double> > indicesToArbitrate, otherIndices;
      indicesToArbitrate.clear ();
      otherIndices.clear ();
      for (auto arbitrationValue = currentCut.arbitrationTree->evaluate ().begin (); arbitrationValue != currentCut.arbitrationTree->evaluate ().end (); arbitrationValue++)
        {
          unsigned object = (arbitrationValue - currentCut.arbitrationTree->evaluate ().begin ());
          double value = boost::get<double> (*arbitrationValue);
          pair<bool, bool> flag = make_pair (value, !IS_INVALID(value));

          if (pl_->cumulativeObjectFlags.at (currentCutIndex).at (currentCut.inputLabel).at (object).first
           && pl_->cumulativeObjectFlags.at (currentCutIndex).at (currentCut.inputLabel).at (object).second
           && flag.second)
            indicesToArbitrate.emplace_back (object, value);
          else
            otherIndices.emplace_back (object, value);
        }
      if (currentCut.arbitration != "random")
        sort (indicesToArbitrate.begin (), indicesToArbitrate.end (), [](pair<unsigned, double> a, pair<unsigned, double> b) -> bool { return a.second > b.second; });
      else
        random_shuffle (indicesToArbitrate.begin (), indicesToArbitrate.end ());

      bool isChosen = (indicesToArbitrate.size () ? true : false);
      for (const auto &index : indicesToArbitrate)
        {
          pl_->individualObjectFlags.at (currentCutIndex).at (currentCut.inputLabel).at (index.first).first = isChosen;
          pl_->cumulativeObjectFlags.at (currentCutIndex).at (currentCut.inputLabel).at (index.first).first = isChosen;
          isChosen = false;
        }
      for (const auto &index : otherIndices)
        {
          pl_->individualObjectFlags.at (currentCutIndex).at (currentCut.inputLabel).at (index.first).first = isChosen;
          pl_->cumulativeObjectFlags.at (currentCutIndex).at (currentCut.inputLabel).at (index.first).first = isChosen;
        }
    }
  ////////////////////////////////////////////////////////////////////////////////

  return true;
}

bool
CutCalculator::propagateFromSingleCollections (const Cut &currentCut, unsigned currentCutIndex, const vector<string> &listOfObjects) const
{
  ////////////////////////////////////////////////////////////////////////////////
  // Propagates flags for single object input collections to all related composite collections.
  // Initialize flags for all relevant composite collections to true.
  // Then, for every object in the input collection failing the current cut,
  // propagate the flag to every instance of that object in composite collections
  ////////////////////////////////////////////////////////////////////////////////

  // ignore composite input collections, as those are handled by propagateFromCompositeCollections function
  vector<string> singleObjects = anatools::getSingleObjects (currentCut.inputLabel);
  if (singleObjects.size() > 1){
    return true;
  }

  // loop over all the other collections containing these items
  for (auto &inputType : listOfObjects)
    {
      // don't reset flags for the input collection for this cut
      if (inputType == currentCut.inputLabel){
        continue;
      }
      // skip irrelevant collections
      vector<string> components = anatools::getSingleObjects (inputType);
      if (find(components.begin(), components.end(), currentCut.inputLabel) == components.end())
        continue;

      // determine total size of collection, since it's composed of multiple single objects
      int totalSize = 1;
      for (const auto &component : components){
        totalSize *= currentCut.valueLookupTree->getCollectionSize (component);
      }

      // by default all composite objects pass
      pl_->individualObjectFlags.at (currentCutIndex)[inputType] = vector<pair<bool, bool> > (totalSize, make_pair (true, true));

      // mark non-unique combinations as invalid
      for (unsigned index = 0; index != pl_->individualObjectFlags.at(currentCutIndex).at(inputType).size(); index++) {
        if (isUniqueCase(currentCut, index, inputType))
          continue;
        pl_->individualObjectFlags.at(currentCutIndex).at(inputType).at(index) = make_pair (false, false);
      }

      // loop over objects in input collection for current cut
      for (unsigned index = 0; index != pl_->individualObjectFlags.at(currentCutIndex).at(currentCut.inputLabel).size(); index++) {
        bool flag = pl_->individualObjectFlags.at(currentCutIndex).at(currentCut.inputLabel).at(index).first;

        // nothing to be done for good objects
        if (flag){
          continue;
        }

        // get the list of global indices for the composite collection containing the object in question
        set<unsigned> globalIndices = currentCut.valueLookupTree->getGlobalIndices (index, currentCut.inputLabel, inputType);
        for (const auto &globalIndex : globalIndices){
          // set flags to false for any composite object containing the bad individual object
          pl_->individualObjectFlags.at(currentCutIndex).at(inputType).at(globalIndex).first = false;
        }
      }
      pl_->cumulativeObjectFlags.at (currentCutIndex)[inputType] = pl_->individualObjectFlags.at(currentCutIndex).at(inputType);

      // for each object, AND together cumulative flags from previous cuts with the one for the current cut
      for (unsigned index = 0; index != pl_->cumulativeObjectFlags.at(currentCutIndex).at(inputType).size(); index++) {
        for (unsigned cutIndex = 0; cutIndex != currentCutIndex; cutIndex++){
          pl_->cumulativeObjectFlags.at (currentCutIndex).at (inputType).at (index).first = pl_->cumulativeObjectFlags.at (currentCutIndex).at (inputType).at (index).first && pl_->cumulativeObjectFlags.at (cutIndex).at (inputType).at (index).first;
        }
      }
    }

  ////////////////////////////////////////////////////////////////////////////////
  return true;
}

bool
CutCalculator::propagateFromCompositeCollections (const Cut &currentCut, unsigned currentCutIndex, const vector<string> &listOfObjects) const
{
  ////////////////////////////////////////////////////////////////////////////////
  // Propagates flags for input collections with multiple inputs to all other related collections.
  // Proceeds in 3 steps:
  // 1. Initialize all flags to false
  // 2. Find all single objects which are considered "good"
  // - non-veto case: "good" => part of any object PASSING current cut criteria
  // - veto case: "good" for individual flag => NOT part of any object FAILING current cut criteria
  // - veto case: "good" for cumulative flag => NOT part of any object FAILING current cut criteria but passing all previous cuts
  // 3. For every object in the list generated in the previous step and
  //    propagate the 'true' flag to every instance of that object in found elsewhere
  ////////////////////////////////////////////////////////////////////////////////

  // ignore single object input collections, as those are handled by propagateFromSingleCollections function
  vector<string> singleObjects = anatools::getSingleObjects (currentCut.inputLabel);
  if (singleObjects.size() <= 1){
    return true;
  }
  // filter out duplicate collections, e.g. muon-muons has 1 unique collection (i.e. muons)
  vector<string> uniqueSingleObjects;
  for (const auto &singleObject : singleObjects){
    if (find(uniqueSingleObjects.begin(), uniqueSingleObjects.end(), singleObject) == uniqueSingleObjects.end())
      uniqueSingleObjects.push_back(singleObject);
  }

  // loop over all the individual collections in the input collection
  for (const auto &singleObject : uniqueSingleObjects){

    // generate list of bad object indices in the individual object collection

    vector<bool> individualFlags;
    vector<bool> cumulativeFlags;

    // non-veto case
    if (!currentCut.isVeto){
      for (unsigned index = 0; index != currentCut.valueLookupTree->getCollectionSize (singleObject); index++){
        // initialize flags to false, reset them to true once we find a good composite object containing them
        individualFlags.push_back(false);
        cumulativeFlags.push_back(false);

        // get the list of global indices for the composite collection containing the object in question
        set<unsigned> globalIndices = currentCut.valueLookupTree->getGlobalIndices (index, singleObject, currentCut.inputLabel);
        for (const auto &globalIndex : globalIndices){
          // if we find a "true" flag for any composite object, set the individual object flag to true
          if (pl_->individualObjectFlags.at(currentCutIndex).at(currentCut.inputLabel).at(globalIndex).first){
            individualFlags.at(index) = true;
            if (currentCutIndex > 0){
              if(pl_->cumulativeObjectFlags.at(currentCutIndex-1).at(currentCut.inputLabel).at(globalIndex).first){
                cumulativeFlags.at(index) = true;
                break;
              }
            }
            else{
              cumulativeFlags.at(index) = true;
              break;
            }
          }
        }
      }
    }
    // veto case
    else {
      for (unsigned index = 0; index != currentCut.valueLookupTree->getCollectionSize (singleObject); index++){
        // initialize flags to true, reset them to false once we find a bad composite object containing them
        individualFlags.push_back(true);
        cumulativeFlags.push_back(true);

        // get the list of global indices for the composite collection containing the object in question
        set<unsigned> globalIndices = currentCut.valueLookupTree->getGlobalIndices (index, singleObject, currentCut.inputLabel);
        for (const auto &globalIndex : globalIndices){
          // if we find a "false" flag for any composite object, set the individual object flag to false
          if (!pl_->individualObjectFlags.at(currentCutIndex).at(currentCut.inputLabel).at(globalIndex).first){
            individualFlags.at(index) = false;

            // for calculating the cumulative flags, only consider composite objects passing all previous cuts
            if (currentCutIndex > 0){
              if(pl_->cumulativeObjectFlags.at(currentCutIndex-1).at(currentCut.inputLabel).at(globalIndex).first){
                cumulativeFlags.at(index) = false;
                break;
              }
            }
            else{
              cumulativeFlags.at(index) = false;
              break;
            }
          }
        }
      }
    }

    // loop over all the other collections containing these items
    for (auto &inputType : listOfObjects){
      // don't reset flags for the input collection for this cut
      if (inputType == currentCut.inputLabel){
        continue;
      }
      // skip irrelevant collections
      vector<string> components = anatools::getSingleObjects (inputType);
      if (find(components.begin(), components.end(), singleObject) == components.end())
        continue;


      // determine total size of collection, since it's potentially composed of multiple single objects
      int totalSize = 1;
      for (const auto &component : components){
        totalSize *= currentCut.valueLookupTree->getCollectionSize (component);
      }

      //////////////////////////////////////////////////////////////////////////////////////////
      // set individual and cumulative flags seperately (since for vetoes they're not identical)
      //////////////////////////////////////////////////////////////////////////////////////////

      ///////////////////////
      // set individual flags
      ///////////////////////

      // dy default all objects fail
      pl_->individualObjectFlags.at (currentCutIndex)[inputType] = vector<pair<bool, bool> > (totalSize, make_pair (false, true));

      // loop over flags for objects in the current inputType collection
      for (unsigned index = 0; index != individualFlags.size(); index++) {

        // nothing to be done for bad objects
        if (!individualFlags.at(index)){
          continue;
        }

        // get the list of global indices containing the object in question
        set<unsigned> globalIndices = currentCut.valueLookupTree->getGlobalIndices (index, singleObject, inputType);
        for (const auto &globalIndex : globalIndices){
          // set flags to true for any (potentially composite) object containing the good individual object
          pl_->individualObjectFlags.at(currentCutIndex).at(inputType).at(globalIndex).first = true;
        }

        // mark non-unique combinations as invalid
        for (unsigned index = 0; index != pl_->individualObjectFlags.at(currentCutIndex).at(inputType).size(); index++) {
          if (isUniqueCase(currentCut, index, inputType))
            continue;
          pl_->individualObjectFlags.at(currentCutIndex).at(inputType).at(index) = make_pair (false, false);
        }
      }

      ///////////////////////
      // set cumulative flags
      ///////////////////////

      // dy default all objects fail
      pl_->cumulativeObjectFlags.at (currentCutIndex)[inputType] = vector<pair<bool, bool> > (totalSize, make_pair (false, true));

      // loop over flags for objects in the current inputType collection
      for (unsigned index = 0; index != cumulativeFlags.size(); index++) {

        // nothing to be done for good objects
        if (!cumulativeFlags.at(index)){
          continue;
        }

        // get the list of global indices containing the object in question
        set<unsigned> globalIndices = currentCut.valueLookupTree->getGlobalIndices (index, singleObject, inputType);
        for (const auto &globalIndex : globalIndices){
          // set flags to true for any (potentially composite) object containing the good cumulative object
          pl_->cumulativeObjectFlags.at(currentCutIndex).at(inputType).at(globalIndex).first = true;
        }

        // mark non-unique combinations as invalid
        for (unsigned index = 0; index != pl_->cumulativeObjectFlags.at(currentCutIndex).at(inputType).size(); index++) {
          if (isUniqueCase(currentCut, index, inputType))
            continue;
          pl_->cumulativeObjectFlags.at(currentCutIndex).at(inputType).at(index) = make_pair (false, false);
        }

        // for each object, AND together cumulative flags from previous cuts with the one for the current cut
        for (unsigned index = 0; index != pl_->cumulativeObjectFlags.at(currentCutIndex).at(inputType).size(); index++) {
          for (unsigned cutIndex = 0; cutIndex != currentCutIndex; cutIndex++){
            pl_->cumulativeObjectFlags.at (currentCutIndex).at (inputType).at (index).first = pl_->cumulativeObjectFlags.at (currentCutIndex).at (inputType).at (index).first && pl_->cumulativeObjectFlags.at (cutIndex).at (inputType).at (index).first;
          }
        }
      }
    }
  }
  return true;
}

bool
CutCalculator::setOtherCollectionsFlags (const Cut &currentCut, unsigned currentCutIndex, const vector<string> &listOfObjects) const
{
  ////////////////////////////////////////////////////////////////////////////////
  // Sets flags for all irrelevant collections to true
  ////////////////////////////////////////////////////////////////////////////////

   for (auto &inputType : listOfObjects)
     {
       // skip if flags for this object already exist
       if (pl_->individualObjectFlags.at (currentCutIndex).find(inputType) != pl_->individualObjectFlags.at (currentCutIndex).end())
         continue;

       // determine total size of collection, since it may be composed of multiple single objects
       vector<string> singleObjects = anatools::getSingleObjects (inputType);
       int totalSize = 1;
       for (auto singleObject = singleObjects.begin (); singleObject != singleObjects.end (); singleObject++){
         totalSize *= currentCut.valueLookupTree->getCollectionSize (*singleObject);
       }

       // since these collections don't pertain to the current cut, they all pass by default
       pl_->individualObjectFlags.at (currentCutIndex)[inputType] = vector<pair<bool, bool> > (totalSize, make_pair (true, true));

       // mark non-unique combinations as invalid
       for (unsigned index = 0; index != pl_->individualObjectFlags.at(currentCutIndex).at(inputType).size(); index++) {
         if (isUniqueCase(currentCut, index, inputType))
           continue;
         pl_->individualObjectFlags.at(currentCutIndex).at(inputType).at(index) = make_pair (false, false);
       }

       pl_->cumulativeObjectFlags.at (currentCutIndex)[inputType] = pl_->individualObjectFlags.at(currentCutIndex).at(inputType);

       // for each object, AND together cumulative flags from previous cuts with the one for the current cut
       for (unsigned index = 0; index != pl_->cumulativeObjectFlags.at(currentCutIndex).at(inputType).size(); index++) {
         for (unsigned cutIndex = 0; cutIndex != currentCutIndex; cutIndex++){
           pl_->cumulativeObjectFlags.at (currentCutIndex).at (inputType).at (index).first = pl_->cumulativeObjectFlags.at (currentCutIndex).at (inputType).at (index).first && pl_->cumulativeObjectFlags.at (cutIndex).at (inputType).at (index).first;
         }
       }
     }
  return true;
}

////////////////////////////////////////////////////////////////////////////////

bool
CutCalculator::unpackCuts ()
{
  //////////////////////////////////////////////////////////////////////////////
  // If triggers are given, retrieve them.
  //////////////////////////////////////////////////////////////////////////////
  if (cuts_.exists ("triggers"))
    {
      unpackedTriggers_ = cuts_.getParameter<vector<string> > ("triggers");
      objectsToGet_.insert ("triggers");
    }
  else
    clog << "WARNING: no triggers have been specified." << endl;
  if (cuts_.exists ("triggersToVeto"))
    {
      unpackedTriggersToVeto_ = cuts_.getParameter<vector<string> > ("triggersToVeto");
      objectsToGet_.insert ("triggers");
    }
  if (cuts_.exists ("triggerFilters"))
    {
      unpackedTriggerFilters_ = cuts_.getParameter<vector<string> > ("triggerFilters");
      objectsToGet_.insert ("triggers");
      objectsToGet_.insert ("trigobjs");
    }
  if (cuts_.exists ("triggersInMenu"))
    {
      unpackedTriggersInMenu_ = cuts_.getParameter<vector<string> > ("triggersInMenu");
      objectsToGet_.insert ("triggers");
    }
  if (cuts_.exists ("metFilters"))
    {
      unpackedMETFilters_ = cuts_.getParameter<vector<string> > ("metFilters");
      objectsToGet_.insert ("metFilters");
    }
  //////////////////////////////////////////////////////////////////////////////

  // Retrieve the cuts and clear the vector in which they will be stored after
  // parsing.
  edm::VParameterSet cuts = cuts_.getParameter<edm::VParameterSet> ("cuts");

  // Loop over the cuts, parsing each one and storing it in a vector.
  for (unsigned currentCut = 0; currentCut != cuts.size (); currentCut++)
    {
      Cut tempCut;
      vector<string> tempInputCollection = cuts.at (currentCut).getParameter<vector<string> > ("inputCollection");
      sort (tempInputCollection.begin (), tempInputCollection.end ());

      //////////////////////////////////////////////////////////////////////////
      // Store the name(s) of the collection(s) to get from the event and for
      // which to set flags.
      //////////////////////////////////////////////////////////////////////////
      objectsToGet_.insert (tempInputCollection.begin (), tempInputCollection.end ());
      //////////////////////////////////////////////////////////////////////////

      string catInputCollection = anatools::concatenateInputCollection (tempInputCollection);
      tempCut.inputCollections = tempInputCollection;
      tempCut.inputLabel = catInputCollection;

      //////////////////////////////////////////////////////////////////////////
      // Separate the cut into subcuts, each of which is separated by a logical
      // operator.
      //////////////////////////////////////////////////////////////////////////
      tempCut.cutString = cuts.at (currentCut).getParameter<string> ("cutString");
      //////////////////////////////////////////////////////////////////////////

      //////////////////////////////////////////////////////////////////////////
      // Extract the number of objects required by the cut.
      //////////////////////////////////////////////////////////////////////////
      string numberRequiredString = cuts.at (currentCut).getParameter<string> ("numberRequired");
      vector<string> numberRequiredVector = splitString (numberRequiredString);
      if (numberRequiredVector.size () != 2)
        {
          clog << "ERROR: malformed number required: \"" << numberRequiredString << "\"." << endl;
          return false;
        }
      int numberRequiredInt = atoi (numberRequiredVector.at (1).c_str ());
      tempCut.numberRequired = numberRequiredInt;
      tempCut.eventComparativeOperator = numberRequiredVector.at (0);
      //////////////////////////////////////////////////////////////////////////

      //////////////////////////////////////////////////////////////////////////
      // Give the cut a name to be used in cut flows. If the user specifies an
      // alias, this is used as the name.
      //////////////////////////////////////////////////////////////////////////
      string tempCutName;
      if (cuts.at (currentCut).exists ("alias"))
        tempCutName = cuts.at (currentCut).getParameter<string> ("alias");
      else
        {
          //bool plural = numberRequiredInt != 1;
          string collectionString = catInputCollection;
          string cutName =  numberRequiredString + " " + collectionString + " with " + tempCut.cutString;
          tempCutName = cutName;
        }
      tempCut.name = tempCutName;
      //////////////////////////////////////////////////////////////////////////

      //////////////////////////////////////////////////////////////////////////
      // Extract "isVeto" which instructs the code to drop events which pass
      // this cut.
      //////////////////////////////////////////////////////////////////////////
      tempCut.isVeto = false;
      if (cuts.at (currentCut).exists ("isVeto"))
        tempCut.isVeto = cuts.at (currentCut).getParameter<bool> ("isVeto");
      //////////////////////////////////////////////////////////////////////////

      //////////////////////////////////////////////////////////////////////////
      // Set the arbitration method.
      //////////////////////////////////////////////////////////////////////////
      tempCut.arbitration = "";
      if (cuts.at (currentCut).exists ("arbitration"))
        tempCut.arbitration = cuts.at (currentCut).getParameter<string> ("arbitration");
      //////////////////////////////////////////////////////////////////////////

      // Store the temporary cut variable into the vector of unpacked cuts, and
      // initialize the valueLookupTree pointers to be NULL.
      tempCut.valueLookupTree = NULL;
      tempCut.arbitrationTree = NULL;
      unpackedCuts_.push_back (tempCut);
    }

  return true;
}

bool
CutCalculator::evaluateComparison (int testValue, const string &comparison, int cutValue) const
{
  //////////////////////////////////////////////////////////////////////////////
  // Return the result of the comparison of two values, returning false if the
  // comparison operator is not recognized.
  //////////////////////////////////////////////////////////////////////////////
  bool returnValue;

  if        (comparison  ==  "!=")  returnValue  =  (testValue  !=  cutValue);
  else  if  (comparison  ==  "<")   returnValue  =  (testValue  <   cutValue);
  else  if  (comparison  ==  "<=")  returnValue  =  (testValue  <=  cutValue);
  else  if  (comparison  ==  "=")   returnValue  =  (testValue  ==  cutValue);
  else  if  (comparison  ==  "==")  returnValue  =  (testValue  ==  cutValue);
  else  if  (comparison  ==  ">=")  returnValue  =  (testValue  >=  cutValue);
  else  if  (comparison  ==  ">")   returnValue  =  (testValue  >   cutValue);
  else
    {
      clog << "WARNING: invalid comparison operator \"" << comparison << "\"." << endl;
      returnValue = false;
    }

  return returnValue;
  //////////////////////////////////////////////////////////////////////////////
}

vector<string>
CutCalculator::splitString (const string &inputString) const
{
  //////////////////////////////////////////////////////////////////////////////
  // Split the input string into words separated by whitespace, with each word
  // stored in a vector.
  //////////////////////////////////////////////////////////////////////////////
  stringstream ss (inputString);
  istream_iterator<string> begin (ss);
  istream_iterator<string> end;
  vector<string> stringVector (begin, end);
  return stringVector;
  //////////////////////////////////////////////////////////////////////////////
}

bool
CutCalculator::evaluateTriggers (const edm::Event &event)
{
  //////////////////////////////////////////////////////////////////////////////
  // Initialize the flags for each trigger which is required to pass, each
  // trigger which is to be vetoed if it passed, and each trigger which is
  // required to exist in the HLT menu, as well as the event-wide flags for
  // each of these.
  //////////////////////////////////////////////////////////////////////////////
  bool triggerDecision = !pl_->triggers.size (), vetoTriggerDecision = true;
  pl_->triggerFlags.resize (pl_->triggers.size (), false);
  pl_->vetoTriggerFlags.resize (pl_->triggersToVeto.size (), true);
  pl_->triggerInMenuFlags.resize (pl_->triggersInMenu.size (), false);
  //////////////////////////////////////////////////////////////////////////////

  if (handles_.triggers.isValid ())
    {
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == AOD || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD_CUSTOM
      const edm::TriggerNames &triggerNames = event.triggerNames (*handles_.triggers);
      if (triggerNamesPSetID_ != triggerNames.parameterSetID ())
        {
          triggerIndices_.clear ();
          triggerNamesPSetID_ = triggerNames.parameterSetID ();
          triggersInMenu_ = true;
        }
      if (!triggerIndices_.size ())
        {
          for (unsigned i = 0; i < triggerNames.size (); i++)
            {
              string name = triggerNames.triggerName (i);
              bool pass = handles_.triggers->accept (i);
#else
      #error "Data format is not valid."
#endif
              //////////////////////////////////////////////////////////////////////////
              // If the current trigger matches one of the triggers to veto, record its
              // decision. If any of these triggers is true, set the event-wide flag to
              // false;
              //////////////////////////////////////////////////////////////////////////
              for (unsigned triggerIndex = 0; triggerIndex != pl_->triggersToVeto.size (); triggerIndex++)
                {
                  if (name.find (pl_->triggersToVeto.at (triggerIndex)) == 0)
                    {
                      triggerIndices_[pl_->triggersToVeto.at (triggerIndex)];
                      triggerIndices_.at (pl_->triggersToVeto.at (triggerIndex)).insert (i);
                      vetoTriggerDecision = vetoTriggerDecision && !pass;
                      pl_->vetoTriggerFlags.at (triggerIndex) = pass;
                    }
                }
              //////////////////////////////////////////////////////////////////////////

              //////////////////////////////////////////////////////////////////////////
              // If the current trigger matches one of the required triggers, record its
              // decision. If any of these triggers is true, set the event-wide flag to
              // true.
              //////////////////////////////////////////////////////////////////////////
              for (unsigned triggerIndex = 0; triggerIndex != pl_->triggers.size (); triggerIndex++)
                {
                  if (name.find (pl_->triggers.at (triggerIndex)) == 0)
                    {
                      triggerIndices_[pl_->triggers.at (triggerIndex)];
                      triggerIndices_.at (pl_->triggers.at (triggerIndex)).insert (i);
                      triggerDecision = triggerDecision || pass;
                      pl_->triggerFlags.at (triggerIndex) = pass;
                    }
                }
              //////////////////////////////////////////////////////////////////////////

              //////////////////////////////////////////////////////////////////////////
              // If the current trigger name exactly matches one of the triggers
              // required to exist in the HLT menu, set the corresponding flag to
              // true.
              //////////////////////////////////////////////////////////////////////////
              for (unsigned triggerIndex = 0; triggerIndex != pl_->triggersInMenu.size (); triggerIndex++)
                {
                  if (name == pl_->triggersInMenu.at (triggerIndex))
                    pl_->triggerInMenuFlags.at (triggerIndex) = true;
                }
              //////////////////////////////////////////////////////////////////////////
            }
        }
      else
        {
          for (unsigned triggerIndex = 0; triggerIndex != pl_->triggersToVeto.size (); triggerIndex++)
            {
              if (!triggerIndices_.count (pl_->triggersToVeto.at (triggerIndex)))
                continue;
              for (const auto &i : triggerIndices_.at (pl_->triggersToVeto.at (triggerIndex)))
                {
                  bool pass = handles_.triggers->accept (i);
                  vetoTriggerDecision = vetoTriggerDecision && !pass;
                  pl_->vetoTriggerFlags.at (triggerIndex) = pass;
                }
            }
          for (unsigned triggerIndex = 0; triggerIndex != pl_->triggers.size (); triggerIndex++)
            {
              if (!triggerIndices_.count (pl_->triggers.at (triggerIndex)))
                continue;
              for (const auto &i : triggerIndices_.at (pl_->triggers.at (triggerIndex)))
                {
                  bool pass = handles_.triggers->accept (i);
                  triggerDecision = triggerDecision || pass;
                  pl_->triggerFlags.at (triggerIndex) = pass;
                }
            }
        }
    }

  //////////////////////////////////////////////////////////////////////////
  // All of the triggers required to exist in the HLT menu must exist for the
  // event to pass.
  //////////////////////////////////////////////////////////////////////////
  for (const auto &flag : pl_->triggerInMenuFlags)
    triggersInMenu_ = triggersInMenu_ && flag;
  //////////////////////////////////////////////////////////////////////////

  // Store the logical AND of the three event-wide flags as the event-wide
  // trigger decision in the payload and return it.
  return (pl_->triggerDecision = (triggerDecision && vetoTriggerDecision && triggersInMenu_));
}

bool
CutCalculator::evaluateTriggerFilters (const edm::Event &event) const
{
  bool triggerFilterDecision = !pl_->triggerFilters.size ();
  pl_->triggerFilterFlags.resize (pl_->triggerFilters.size (), false);

  if (handles_.triggers.isValid () && handles_.trigobjs.isValid ())
    {
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM
      const edm::TriggerNames &triggerNames = event.triggerNames (*handles_.triggers);
#endif
      for (unsigned i = 0; i < pl_->triggerFilters.size (); i++)
        {
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM
          for (auto trigobj : *handles_.trigobjs)
            {
              trigobj.unpackPathNames (triggerNames);
              for (const auto &filter : trigobj.filterLabels ())
                {
                  pl_->triggerFilterFlags.at (i) = (pl_->triggerFilters.at (i) == filter);
                  if (pl_->triggerFilterFlags.at (i))
                    break;
                }
              if (pl_->triggerFilterFlags.at (i))
                break;
            }
#endif
          triggerFilterDecision = triggerFilterDecision || pl_->triggerFilterFlags.at (i);
        }
    }

  return (pl_->triggerFilterDecision = triggerFilterDecision);
}

bool
CutCalculator::evaluateMETFilters (const edm::Event &event)
{
  // The MET filter decisions are stored in an edm::TriggerResults object (for
  // some reason). As such, this code is just a copypasta of the code in
  // CutCalculator::evaluateTriggers above. The only significant difference is
  // that the MET filter decision is the AND of several booleans, instead of
  // the OR as in the case of the trigger decision.
  bool metFilterDecision = pl_->metFilters.size ();
  pl_->metFilterFlags.resize (pl_->metFilters.size (), false);

  if (handles_.metFilters.isValid ())
    {
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == AOD || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD_CUSTOM
      const edm::TriggerNames &metFilterNames = event.triggerNames (*handles_.metFilters);
      if (metFilterNamesPSetID_ != metFilterNames.parameterSetID ())
        {
          metFilterIndices_.clear ();
          metFilterNamesPSetID_ = metFilterNames.parameterSetID ();
        }
      if (!metFilterIndices_.size ())
        {
          for (unsigned i = 0; i < metFilterNames.size (); i++)
            {
              string name = metFilterNames.triggerName (i);
              bool pass = handles_.metFilters->accept (i);
#else
      #error "Data format is not valid."
#endif
              for (unsigned metFilterIndex = 0; metFilterIndex != pl_->metFilters.size (); metFilterIndex++)
                {
                  if (name.find (pl_->metFilters.at (metFilterIndex)) == 0)
                    {
                      metFilterIndices_[pl_->metFilters.at (metFilterIndex)];
                      metFilterIndices_.at (pl_->metFilters.at (metFilterIndex)).insert (i);
                      metFilterDecision = metFilterDecision && pass;
                      pl_->metFilterFlags.at (metFilterIndex) = pass;
                    }
                }
            }
        }
      else
        {
          for (unsigned metFilterIndex = 0; metFilterIndex != pl_->metFilters.size (); metFilterIndex++)
            {
              if (!metFilterIndices_.count (pl_->metFilters.at (metFilterIndex)))
                continue;
              for (const auto &i : metFilterIndices_.at (pl_->metFilters.at (metFilterIndex)))
                {
                  bool pass = handles_.metFilters->accept (i);
                  metFilterDecision = metFilterDecision && pass;
                  pl_->metFilterFlags.at (metFilterIndex) = pass;
                }
            }
        }
    }

  return (pl_->metFilterDecision = metFilterDecision);
}

bool
CutCalculator::setEventFlags () const
{
  pl_->cutsDecision = true;

  // Loop over the cuts, storing the event-wide decision for each in the
  // payload.
  for (unsigned currentCutIndex = 0; currentCutIndex != pl_->cuts.size (); currentCutIndex++)
    {
      Cut currentCut = pl_->cuts.at (currentCutIndex);
      int numberPassing = 0;
      int numberPassingPrev = 0;
      int numberPassingIndividual = 0;

      //////////////////////////////////////////////////////////////////////////
      // Count the number of objects passing the current cut and all previous
      // cuts in the collection on which this cut acts.
      //////////////////////////////////////////////////////////////////////////
      for (const auto &flag : pl_->cumulativeObjectFlags.at (currentCutIndex).at (currentCut.inputLabel))
        {
          if (flag.second && flag.first)
            numberPassing++;
        }
      //////////////////////////////////////////////////////////////////////////

      //////////////////////////////////////////////////////////////////////////
      // Count the number of objects passing the current cut independently.
      //////////////////////////////////////////////////////////////////////////
      for (const auto &flag : pl_->individualObjectFlags.at (currentCutIndex).at (currentCut.inputLabel))
        {
          if (flag.second && flag.first)
            numberPassingIndividual++;
        }
      //////////////////////////////////////////////////////////////////////////

      //////////////////////////////////////////////////////////////////////////
      // Decide if the event passes this cut. If the cut is a veto, we have to
      // test the number of objects which failed this cut but which passed all
      // previous cuts. Remember, the object flags are inverted in the case of a
      // veto.
      //////////////////////////////////////////////////////////////////////////
      bool cutDecision;
      bool cutDecisionIndividual;
      if (!currentCut.isVeto)
        {
        cutDecision = evaluateComparison (numberPassing, currentCut.eventComparativeOperator, currentCut.numberRequired);
        cutDecisionIndividual = evaluateComparison (numberPassingIndividual, currentCut.eventComparativeOperator, currentCut.numberRequired);
        }
      else
        {
          int numberTotalObjects = pl_->cumulativeObjectFlags.at (currentCutIndex).at (currentCut.inputLabel).size();
          if (currentCutIndex > 0)
            {
              for (const auto &flag : pl_->cumulativeObjectFlags.at (currentCutIndex - 1).at (currentCut.inputLabel))
                (flag.second && flag.first) && numberPassingPrev++;
            }
          else
            {
              numberPassingPrev = numberTotalObjects;
            }
          int numberFailIndividual = numberTotalObjects - numberPassingIndividual;
          int numberFailCumulative = numberPassingPrev - numberPassing;
          //          cout << "numberFailIndividual: " <<  numberFailIndividual << endl;
          //          cout << "numberFailCumulative: " << numberFailCumulative << endl;
          cutDecision = evaluateComparison (numberFailCumulative, currentCut.eventComparativeOperator, currentCut.numberRequired);
          cutDecisionIndividual = evaluateComparison (numberFailIndividual, currentCut.eventComparativeOperator, currentCut.numberRequired);
        }

      // cout << "cutDecision: " << cutDecision << endl;
      // cout << "cutDecisionIndividual: " << cutDecisionIndividual << endl;
      //////////////////////////////////////////////////////////////////////////

      //////////////////////////////////////////////////////////////////////////
      // Store the decision for this cut in the payload and update the global
      // cut decision flag.
      //////////////////////////////////////////////////////////////////////////
      pl_->cumulativeEventFlags.push_back (cutDecision);
      pl_->cutsDecision = pl_->cutsDecision && cutDecision;
      pl_->individualEventFlags.push_back (cutDecisionIndividual);
      //////////////////////////////////////////////////////////////////////////
    }

  // Store the logical AND of the trigger decision and the global cut decision
  // as the global event decision in the payload and return it.
  return (pl_->eventDecision = (pl_->triggerDecision && pl_->triggerFilterDecision && pl_->metFilterDecision && pl_->cutsDecision));
}

bool
CutCalculator::initializeValueLookupForest (Cuts &cuts, Collections * const handles)
{
  //////////////////////////////////////////////////////////////////////////////
  // For each cut, parse its cut string into a new ValueLookupTree object which
  // is stored in the cut structure.
  //////////////////////////////////////////////////////////////////////////////
  for (auto &cut : cuts)
    {
      if (firstEvent_)
        {
          cut.valueLookupTree = new ValueLookupTree (cut);
          if (cut.arbitration != "")
            cut.arbitrationTree = new ValueLookupTree (cut.arbitration != "random" ? cut.arbitration : "0.0", cut.inputCollections);
          if (!cut.valueLookupTree->isValid ())
            return false;
        }
      cut.valueLookupTree->setCollections (handles);
      if (cut.arbitration != "")
        cut.arbitrationTree->setCollections (handles);
    }
  return true;
  //////////////////////////////////////////////////////////////////////////////
}


vector<string>
CutCalculator::getListOfObjects (const Cuts &cuts)
{
  //////////////////////////////////////////////////////////////////////////////
  // Create unique list of objects for which flags should be set
  // For each cut, append the input collection and any component collections
  // to the list and return the final, unique list.
  //////////////////////////////////////////////////////////////////////////////
  vector<string> objects;
  for (const auto &cut : cuts)
    {
      if (find(objects.begin(), objects.end(), cut.inputLabel) == objects.end())
        objects.push_back(cut.inputLabel);
      for (const auto &collection : cut.inputCollections){
        if (find(objects.begin(), objects.end(), collection) == objects.end())
          objects.push_back(collection);
      }
    }

  return objects;

}

bool
  CutCalculator::isUniqueCase (const Cut &currentCut, unsigned globalIndex, string inputType) const
{
  ////////////////////////////////////////////////////////////////////////////////
  // Determine whether the composite object in index 'globalIndex' in the collection 'inputType'
  // corresponds to a unique case
  // Unique cases have ascending local indices for any objects in the same collection

  // Example1:  invMass(muon1,muon2).  In an event with 3 muons, there would be
  // 9 combinations:
  // Global index:                 0  1  2  3  4  5  6  7  8
  // Local index for collection 0: 0  0  0  1  1  1  2  2  2
  // Local index for collection 1: 0  1  2  0  1  2  0  1  2
  // globalIndex = 1, 2, 5 meet the criteria for being unique
  //
  // Example2:  invMass(muon1,muon2,electron).  In an event with 2 muons & 1 electron, there would be
  // 9 combinations:
  // Global index:                 0  1  2  3  4  5  6  7
  // Local index for collection 0: 0  0  0  0  1  1  1  1
  // Local index for collection 1: 0  0  1  1  0  0  1  1
  // Local index for collection 2: 0  1  0  1  0  1  0  1
  // globalIndex = 2, 3 meet the criteria for being unique

  // Example3:  invMass(muon1,muon2,electron1,electron2).  In an event with 2 muons & 2 electrons, there would be
  // 9 combinations:
  // Global index:                 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
  // Local index for collection 0: 0  0  0  0  0  0  0  0  1  1  1  1  1  1  1  1
  // Local index for collection 1: 0  0  0  0  1  1  1  1  0  0  0  0  1  1  1  1
  // Local index for collection 2: 0  0  1  1  0  0  1  1  0  0  1  1  0  0  1  1
  // Local index for collection 3: 0  1  0  1  0  1  0  1  0  1  0  1  0  1  0  1
  // globalIndex = 5 meets the criteria for being unique

  ////////////////////////////////////////////////////////////////////////////////

  vector<string> singleObjects = anatools::getSingleObjects (inputType);
  // always unique for single object collections
  if (singleObjects.size() <= 1){
    return true;
  }

  vector<string> uniqueSingleObjects;
  for (const auto &singleObject : singleObjects){
    if (find(uniqueSingleObjects.begin(), uniqueSingleObjects.end(), singleObject) == uniqueSingleObjects.end())
      uniqueSingleObjects.push_back(singleObject);
  }
  // always unique if no collection is used more than once
  if (singleObjects.size() == uniqueSingleObjects.size()){
    return true;
  }

  // for each single input object, store the type and the local index of the object used
  vector<pair<string,unsigned> > objectIndexMap;

  int nCombinations = 1;
  for (unsigned collectionIndex = 0; collectionIndex != singleObjects.size(); collectionIndex++){
    unsigned localIndex;
    int collectionSize = currentCut.valueLookupTree->getCollectionSize (singleObjects.at(collectionIndex));
    if (collectionIndex + 1 != singleObjects.size ()){
      nCombinations *= currentCut.valueLookupTree->getCollectionSize (singleObjects.at(collectionIndex));
      localIndex = ((globalIndex / nCombinations) % currentCut.valueLookupTree->getCollectionSize (singleObjects.at(collectionIndex)));
    }
    else{
      localIndex = (globalIndex % collectionSize);
    }
    objectIndexMap.emplace_back(singleObjects.at(collectionIndex),localIndex);
  }

  bool pass = true;
  for (const auto &singleObject : uniqueSingleObjects){
    int prevIndex = -1;
    for (const auto &objectIndex : objectIndexMap){

      if (objectIndex.first != singleObject)
        continue;

      // pass remains true as long as the objects are in order
      pass = pass && ((int) objectIndex.second > prevIndex);
      prevIndex = objectIndex.second;
    }
  }
  return pass;
}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(CutCalculator);
