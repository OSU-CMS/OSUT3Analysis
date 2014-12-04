#include <algorithm>
#include <iostream>
#include <set>
#include <unordered_map>

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"
#include "OSUT3Analysis/AnaTools/plugins/CutCalculator.h"


#define EXIT_CODE 1

CutCalculator::CutCalculator (const edm::ParameterSet &cfg) :
  collections_  (cfg.getParameter<edm::ParameterSet>  ("collections")),
  cuts_         (cfg.getParameter<edm::ParameterSet>  ("cuts")),
  firstEvent_   (true)
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

  produces<CutCalculatorPayload> ("cutDecisions");
}

CutCalculator::~CutCalculator ()
{
}

void
CutCalculator::produce (edm::Event &event, const edm::EventSetup &setup)
{
  //////////////////////////////////////////////////////////////////////////////
  // Retrieve each object collection which we need and print a warning if it is
  // missing.
  //////////////////////////////////////////////////////////////////////////////
  if  (objectsToGet_.find  ("bxlumis")         !=  objectsToGet_.end  ()  &&  collections_.exists  ("bxlumis"))         event.getByLabel  (collections_.getParameter<edm::InputTag>  ("bxlumis"),         handles_.bxlumis);
  if  (objectsToGet_.find  ("electrons")       !=  objectsToGet_.end  ()  &&  collections_.exists  ("electrons"))       event.getByLabel  (collections_.getParameter<edm::InputTag>  ("electrons"),       handles_.electrons);
  if  (objectsToGet_.find  ("events")          !=  objectsToGet_.end  ()  &&  collections_.exists  ("events"))          event.getByLabel  (collections_.getParameter<edm::InputTag>  ("events"),          handles_.events);
  if  (objectsToGet_.find  ("genjets")         !=  objectsToGet_.end  ()  &&  collections_.exists  ("genjets"))         event.getByLabel  (collections_.getParameter<edm::InputTag>  ("genjets"),         handles_.genjets);
  if  (objectsToGet_.find  ("jets")            !=  objectsToGet_.end  ()  &&  collections_.exists  ("jets"))            event.getByLabel  (collections_.getParameter<edm::InputTag>  ("jets"),            handles_.jets);
  if  (objectsToGet_.find  ("mcparticles")     !=  objectsToGet_.end  ()  &&  collections_.exists  ("mcparticles"))     event.getByLabel  (collections_.getParameter<edm::InputTag>  ("mcparticles"),     handles_.mcparticles);
  if  (objectsToGet_.find  ("mets")            !=  objectsToGet_.end  ()  &&  collections_.exists  ("mets"))            event.getByLabel  (collections_.getParameter<edm::InputTag>  ("mets"),            handles_.mets);
  if  (objectsToGet_.find  ("muons")           !=  objectsToGet_.end  ()  &&  collections_.exists  ("muons"))           event.getByLabel  (collections_.getParameter<edm::InputTag>  ("muons"),           handles_.muons);
  if  (objectsToGet_.find  ("photons")         !=  objectsToGet_.end  ()  &&  collections_.exists  ("photons"))         event.getByLabel  (collections_.getParameter<edm::InputTag>  ("photons"),         handles_.photons);
  if  (objectsToGet_.find  ("primaryvertexs")  !=  objectsToGet_.end  ()  &&  collections_.exists  ("primaryvertexs"))  event.getByLabel  (collections_.getParameter<edm::InputTag>  ("primaryvertexs"),  handles_.primaryvertexs);
  if  (objectsToGet_.find  ("superclusters")   !=  objectsToGet_.end  ()  &&  collections_.exists  ("superclusters"))   event.getByLabel  (collections_.getParameter<edm::InputTag>  ("superclusters"),   handles_.superclusters);
  if  (objectsToGet_.find  ("taus")            !=  objectsToGet_.end  ()  &&  collections_.exists  ("taus"))            event.getByLabel  (collections_.getParameter<edm::InputTag>  ("taus"),            handles_.taus);
  if  (objectsToGet_.find  ("tracks")          !=  objectsToGet_.end  ()  &&  collections_.exists  ("tracks"))          event.getByLabel  (collections_.getParameter<edm::InputTag>  ("tracks"),          handles_.tracks);
  if  (objectsToGet_.find  ("triggers")        !=  objectsToGet_.end  ()  &&  collections_.exists  ("triggers"))        event.getByLabel  (collections_.getParameter<edm::InputTag>  ("triggers"),        handles_.triggers);
  if  (objectsToGet_.find  ("trigobjs")        !=  objectsToGet_.end  ()  &&  collections_.exists  ("trigobjs"))        event.getByLabel  (collections_.getParameter<edm::InputTag>  ("trigobjs"),        handles_.trigobjs);

  if (firstEvent_ && !handles_.bxlumis.isValid ())
    clog << "INFO: did not retrieve bxlumis collection from the event." << endl;
  if (firstEvent_ && !handles_.electrons.isValid ())
    clog << "INFO: did not retrieve electrons collection from the event." << endl;
  if (firstEvent_ && !handles_.events.isValid ())
    clog << "INFO: did not retrieve events collection from the event." << endl;
  if (firstEvent_ && !handles_.genjets.isValid ())
    clog << "INFO: did not retrieve genjets collection from the event." << endl;
  if (firstEvent_ && !handles_.jets.isValid ())
    clog << "INFO: did not retrieve jets collection from the event." << endl;
  if (firstEvent_ && !handles_.mcparticles.isValid ())
    clog << "INFO: did not retrieve mcparticles collection from the event." << endl;
  if (firstEvent_ && !handles_.mets.isValid ())
    clog << "INFO: did not retrieve mets collection from the event." << endl;
  if (firstEvent_ && !handles_.muons.isValid ())
    clog << "INFO: did not retrieve muons collection from the event." << endl;
  if (firstEvent_ && !handles_.photons.isValid ())
    clog << "INFO: did not retrieve photons collection from the event." << endl;
  if (firstEvent_ && !handles_.primaryvertexs.isValid ())
    clog << "INFO: did not retrieve primaryvertexs collection from the event." << endl;
  if (firstEvent_ && !handles_.superclusters.isValid ())
    clog << "INFO: did not retrieve superclusters collection from the event." << endl;
  if (firstEvent_ && !handles_.taus.isValid ())
    clog << "INFO: did not retrieve taus collection from the event." << endl;
  if (firstEvent_ && !handles_.tracks.isValid ())
    clog << "INFO: did not retrieve tracks collection from the event." << endl;
  if (firstEvent_ && !handles_.triggers.isValid ())
    clog << "INFO: did not retrieve triggers collection from the event." << endl;
  if (firstEvent_ && !handles_.trigobjs.isValid ())
    clog << "INFO: did not retrieve trigobjs collection from the event." << endl;
  if (firstEvent_ && !handles_.userVariables.isValid ())
    clog << "INFO: did not retrieve userVariables collection from the event." << endl;
  //////////////////////////////////////////////////////////////////////////////

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
  pl_ = auto_ptr<CutCalculatorPayload> (new CutCalculatorPayload);
  pl_->isValid = true;
  pl_->cuts = unpackedCuts_;
  pl_->triggers = unpackedTriggers_;
  pl_->triggersToVeto = unpackedTriggersToVeto_;
  //////////////////////////////////////////////////////////////////////////////

  // Loop over cuts to set flags for each object indicating whether it passed
  // the cut.
  for (unsigned currentCutIndex = 0; pl_->isValid && currentCutIndex != pl_->cuts.size (); currentCutIndex++)
    {
      Cut currentCut = pl_->cuts.at (currentCutIndex);
      pl_->isValid = setObjectFlags (currentCut, currentCutIndex);

      //////////////////////////////////////////////////////////////////////////
      // Update the flags for the paired object collections with those of the
      // constituent object collections.
      //////////////////////////////////////////////////////////////////////////
      updateCrossTalk (currentCut, currentCutIndex);
      //////////////////////////////////////////////////////////////////////////
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
  evaluateTriggers ();
  setEventFlags ();

  event.put (pl_, "cutDecisions");
  firstEvent_ = false;
}

bool
CutCalculator::setObjectFlags (const Cut &currentCut, unsigned currentCutIndex) const
{
  string inputType = currentCut.inputLabel;
  if (currentCutIndex >= pl_->objectFlags.size ())
    pl_->objectFlags.resize (currentCutIndex + 1);
  if (currentCutIndex >= pl_->cumulativeObjectFlags.size ())
    pl_->cumulativeObjectFlags.resize (currentCutIndex + 1);
  pl_->objectFlags.at (currentCutIndex)[inputType];
  pl_->cumulativeObjectFlags.at (currentCutIndex)[inputType];

  for (auto cutDecision = currentCut.valueLookupTree->evaluate ().begin (); cutDecision != currentCut.valueLookupTree->evaluate ().end (); cutDecision++)
    {
      unsigned object = (cutDecision - currentCut.valueLookupTree->evaluate ().begin ());
      double value = boost::get<double> (*cutDecision);
      pair<bool, bool> flag = make_pair (value, value > numeric_limits<int>::min () + 1);

      if (currentCut.isVeto)
        flag.first = !flag.first;

      pl_->objectFlags.at (currentCutIndex).at (inputType).push_back (flag);
      if (currentCutIndex > 0 && pl_->cumulativeObjectFlags.at (currentCutIndex - 1).count (inputType))
        flag.first = flag.first && pl_->cumulativeObjectFlags.at (currentCutIndex - 1).at (inputType).at (object).first;
      pl_->cumulativeObjectFlags.at (currentCutIndex).at (inputType).push_back (flag);
    }

  return true;
}

void
CutCalculator::updateCrossTalk (const Cut &currentCut, unsigned currentCutIndex) const
{
  string inputType = currentCut.inputLabel;
  vector<string> singleObjects = ValueLookupTree::getSingleObjects (inputType);
  if (currentCutIndex > 0)
    {
      for (const auto &collection : pl_->objectFlags.at (currentCutIndex - 1))
        {
          if (collection.first == inputType)
            continue;
          pl_->objectFlags.at (currentCutIndex)[collection.first] = pl_->objectFlags.at (currentCutIndex - 1).at (collection.first);
          pl_->cumulativeObjectFlags.at (currentCutIndex)[collection.first] = pl_->cumulativeObjectFlags.at (currentCutIndex - 1).at (collection.first);
          unordered_map<unsigned, bool> otherCumulativeFlags;
          for (auto singleObject = singleObjects.begin (); singleObject != singleObjects.end (); singleObject++)
            {
              for (auto flag = pl_->objectFlags.at (currentCutIndex).at (inputType).begin (); flag != pl_->objectFlags.at (currentCutIndex).at (inputType).end (); flag++)
                {
                  unsigned iFlag = flag - pl_->objectFlags.at (currentCutIndex).at (inputType).begin ();
                  unsigned localIndex = currentCut.valueLookupTree->getLocalIndex (iFlag, singleObject - singleObjects.begin ());
                  set<unsigned> globalIndices = currentCut.valueLookupTree->getGlobalIndices (localIndex, *singleObject, collection.first);
                  if (!globalIndices.size ())
                    break;
                  pair<bool, bool> currentFlag = pl_->cumulativeObjectFlags.at (currentCutIndex).at (inputType).at (iFlag), otherFlag;
                  bool cumulativeFlag = false;
                  for (const auto &globalIndex : globalIndices)
                    {
                      flag->second && (pl_->objectFlags.at (currentCutIndex).at (collection.first).at (globalIndex).first = flag->first);
                      otherFlag = pl_->cumulativeObjectFlags.at (currentCutIndex).at (collection.first).at (globalIndex);
                      otherFlag.second && (cumulativeFlag = cumulativeFlag || otherFlag.first);
                      if (!otherCumulativeFlags.count (globalIndex))
                        otherCumulativeFlags[globalIndex] = false;
                      currentFlag.second && (otherCumulativeFlags.at (globalIndex) = otherCumulativeFlags.at (globalIndex) || currentFlag.first);
                    }
                  currentFlag.second && (pl_->cumulativeObjectFlags.at (currentCutIndex).at (inputType).at (iFlag).first = currentFlag.first && cumulativeFlag);
                }
            }
          for (const auto &flag : otherCumulativeFlags)
            {
              pair<bool, bool> otherFlag = pl_->cumulativeObjectFlags.at (currentCutIndex).at (collection.first).at (flag.first);
              otherFlag.second && (pl_->cumulativeObjectFlags.at (currentCutIndex).at (collection.first).at (flag.first).first = otherFlag.first && flag.second);
            }
        }
    }
  if (singleObjects.size () > 1)
    {
      for (auto singleObject = singleObjects.begin (); singleObject != singleObjects.end (); singleObject++)
        {
          if (pl_->objectFlags.at (currentCutIndex).count (*singleObject))
            continue;
          pl_->objectFlags.at (currentCutIndex)[*singleObject] = vector<pair<bool, bool> > (currentCut.valueLookupTree->getCollectionSize (*singleObject), make_pair (false, true));
          pl_->cumulativeObjectFlags.at (currentCutIndex)[*singleObject] = vector<pair<bool, bool> > (currentCut.valueLookupTree->getCollectionSize (*singleObject), make_pair (false, true));
          for (auto flag = pl_->objectFlags.at (currentCutIndex).at (inputType).begin (); flag != pl_->objectFlags.at (currentCutIndex).at (inputType).end (); flag++)
            {
              unsigned localIndex = currentCut.valueLookupTree->getLocalIndex (flag - pl_->objectFlags.at (currentCutIndex).at (inputType).begin (), singleObject - singleObjects.begin ());
              flag->second && (pl_->objectFlags.at (currentCutIndex).at (*singleObject).at (localIndex).first = pl_->objectFlags.at (currentCutIndex).at (*singleObject).at (localIndex).first || flag->first);
              flag->second && (pl_->cumulativeObjectFlags.at (currentCutIndex).at (*singleObject).at (localIndex).first = pl_->cumulativeObjectFlags.at (currentCutIndex).at (*singleObject).at (localIndex).first || flag->first);
            }
        }
    }
  if (currentCutIndex > 0)
    {
      for (const auto &collection : pl_->objectFlags.at (currentCutIndex))
        {
          if (pl_->objectFlags.at (currentCutIndex - 1).count (collection.first))
            continue;
          singleObjects = ValueLookupTree::getSingleObjects (inputType);
          for (unsigned i = 0; i < currentCutIndex; i++)
            {
              vector<pair<bool, bool> > objectFlags (collection.second.size (), make_pair (true, true)),
                                        cumulativeObjectFlags (collection.second.size (), make_pair (true, true));
              for (const auto &singleObject : singleObjects)
                {
                  if (!pl_->objectFlags.at (i).count (singleObject))
                    continue;
                  for (auto flag = pl_->objectFlags.at (i).at (singleObject).begin (); flag != pl_->objectFlags.at (i).at (singleObject).end (); flag++)
                    {
                      unsigned localIndex = flag - pl_->objectFlags.at (i).at (singleObject).begin ();
                      set<unsigned> globalIndices = currentCut.valueLookupTree->getGlobalIndices (localIndex, singleObject, collection.first);
                      for (const auto &globalIndex : globalIndices)
                        {
                          objectFlags.at (globalIndex).second = pl_->objectFlags.at (currentCutIndex).at (collection.first).at (globalIndex).second;
                          objectFlags.at (globalIndex).second && (objectFlags.at (globalIndex).first = objectFlags.at (globalIndex).first && flag->first);
                          cumulativeObjectFlags.at (globalIndex).second = pl_->cumulativeObjectFlags.at (currentCutIndex).at (collection.first).at (globalIndex).second;
                          cumulativeObjectFlags.at (globalIndex).second && (cumulativeObjectFlags.at (globalIndex).first = cumulativeObjectFlags.at (globalIndex).first && pl_->cumulativeObjectFlags.at (i).at (singleObject).at (localIndex).first);
                        }
                    }
                }
              pl_->objectFlags.at (i)[collection.first] = objectFlags;
              pl_->cumulativeObjectFlags.at (i)[collection.first] = cumulativeObjectFlags;
            }
        }
    }
}

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

      string catInputCollection = ValueLookupTree::catInputCollection (tempInputCollection);
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

      // Store the temporary cut variable into the vector of unpacked cuts.
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
CutCalculator::evaluateTriggers () const
{
  //////////////////////////////////////////////////////////////////////////////
  // Initialize the flags for each trigger which is required and each trigger
  // which is to be vetoed, as well as the event-wide flags for each of these.
  //////////////////////////////////////////////////////////////////////////////
  bool triggerDecision = !pl_->triggers.size (), vetoTriggerDecision = true;
  pl_->triggerFlags.resize (pl_->triggers.size (), false);
  pl_->vetoTriggerFlags.resize (pl_->triggersToVeto.size (), true);
  //////////////////////////////////////////////////////////////////////////////

  if (handles_.triggers.isValid ())
    {
      for (const auto &trigger : *handles_.triggers)
        {
          //////////////////////////////////////////////////////////////////////////
          // If the current trigger matches one of the triggers to veto, record its
          // decision. If any of these triggers is true, set the event-wide flag to
          // false;
          //////////////////////////////////////////////////////////////////////////
          for (unsigned triggerIndex = 0; triggerIndex != pl_->triggersToVeto.size (); triggerIndex++)
            {
              if (trigger.name.find (pl_->triggersToVeto.at (triggerIndex)) == 0)
                {
                  vetoTriggerDecision = vetoTriggerDecision && !trigger.pass;
                  pl_->vetoTriggerFlags.at (triggerIndex) = trigger.pass;
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
              if (trigger.name.find (pl_->triggers.at (triggerIndex)) == 0)
                {
                  triggerDecision = triggerDecision || trigger.pass;
                  pl_->triggerFlags.at (triggerIndex) = trigger.pass;
                }
            }
          //////////////////////////////////////////////////////////////////////////
        }
    }

  // Store the logical AND of the two event-wide flags as the event-wide
  // trigger decision in the payload and return it.
  return (pl_->triggerDecision = (triggerDecision && vetoTriggerDecision));
}

bool
CutCalculator::setEventFlags () const
{
  pl_->cutDecision = true;

  // Loop over the cuts, storing the event-wide decision for each in the
  // payload.
  for (unsigned currentCutIndex = 0; currentCutIndex != pl_->cuts.size (); currentCutIndex++)
    {
      Cut currentCut = pl_->cuts.at (currentCutIndex);
      int numberPassing = 0;
      int numberPassingPrev = 0;

      //////////////////////////////////////////////////////////////////////////
      // Count the number of objects passing the current cut and all previous
      // cuts in the collection on which the cut acts.
      //////////////////////////////////////////////////////////////////////////
      for (const auto &flag : pl_->cumulativeObjectFlags.at (currentCutIndex).at (currentCut.inputLabel))
        {
          if (flag.second && flag.first)
            numberPassing++;
        }
      //////////////////////////////////////////////////////////////////////////

      //////////////////////////////////////////////////////////////////////////
      // Decide if the event passes this cut. If the cut is a veto, we have to
      // test the number of objects which failed this cut but which passed all
      // previous cuts. Remember, the object flags are inverted in the case of a
      // veto.
      //////////////////////////////////////////////////////////////////////////
      bool cutDecision;
      if (!currentCut.isVeto)
        cutDecision = evaluateComparison (numberPassing, currentCut.eventComparativeOperator, currentCut.numberRequired);
      else
        {
          if (currentCutIndex > 0)
            {
              for (const auto &flag : pl_->cumulativeObjectFlags.at (currentCutIndex - 1).at (currentCut.inputLabel))
                (flag.second && flag.first) && numberPassingPrev++;
            }
          int numberFailCut = numberPassingPrev - numberPassing;
          cutDecision = evaluateComparison (numberFailCut, currentCut.eventComparativeOperator, currentCut.numberRequired);
        }
      //////////////////////////////////////////////////////////////////////////

      //////////////////////////////////////////////////////////////////////////
      // Store the decision for this cut in the payload and update the global
      // cut decision flag.
      //////////////////////////////////////////////////////////////////////////
      pl_->eventFlags.push_back (cutDecision);
      pl_->cutDecision = pl_->cutDecision && cutDecision;
      //////////////////////////////////////////////////////////////////////////
    }

  // Store the logical AND of the trigger decision and the global cut decision
  // as the global event decision in the payload and return it.
  return (pl_->eventDecision = (pl_->triggerDecision && pl_->cutDecision));
}

bool
CutCalculator::initializeValueLookupForest (vector<Cut> &cuts, Collections * const handles)
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
          if (!cut.valueLookupTree->isValid ())
            return false;
        }
      cut.valueLookupTree->setCollections (handles);
    }
  return true;
  //////////////////////////////////////////////////////////////////////////////
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(CutCalculator);
