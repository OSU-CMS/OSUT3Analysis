#include <iostream>
#include <algorithm>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
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
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "bxlumis")         !=  objectsToGet_.end  ()  &&  collections_.exists  ("bxlumis"))         event.getByLabel  (collections_.getParameter<edm::InputTag>  ("bxlumis"),         handles_.bxlumis);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "electrons")       !=  objectsToGet_.end  ()  &&  collections_.exists  ("electrons"))       event.getByLabel  (collections_.getParameter<edm::InputTag>  ("electrons"),       handles_.electrons);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "events")          !=  objectsToGet_.end  ()  &&  collections_.exists  ("events"))          event.getByLabel  (collections_.getParameter<edm::InputTag>  ("events"),          handles_.events);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "genjets")         !=  objectsToGet_.end  ()  &&  collections_.exists  ("genjets"))         event.getByLabel  (collections_.getParameter<edm::InputTag>  ("genjets"),         handles_.genjets);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "jets")            !=  objectsToGet_.end  ()  &&  collections_.exists  ("jets"))            event.getByLabel  (collections_.getParameter<edm::InputTag>  ("jets"),            handles_.jets);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "mcparticles")     !=  objectsToGet_.end  ()  &&  collections_.exists  ("mcparticles"))     event.getByLabel  (collections_.getParameter<edm::InputTag>  ("mcparticles"),     handles_.mcparticles);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "mets")            !=  objectsToGet_.end  ()  &&  collections_.exists  ("mets"))            event.getByLabel  (collections_.getParameter<edm::InputTag>  ("mets"),            handles_.mets);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "muons")           !=  objectsToGet_.end  ()  &&  collections_.exists  ("muons"))           event.getByLabel  (collections_.getParameter<edm::InputTag>  ("muons"),           handles_.muons);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "photons")         !=  objectsToGet_.end  ()  &&  collections_.exists  ("photons"))         event.getByLabel  (collections_.getParameter<edm::InputTag>  ("photons"),         handles_.photons);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "primaryvertexs")  !=  objectsToGet_.end  ()  &&  collections_.exists  ("primaryvertexs"))  event.getByLabel  (collections_.getParameter<edm::InputTag>  ("primaryvertexs"),  handles_.primaryvertexs);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "superclusters")   !=  objectsToGet_.end  ()  &&  collections_.exists  ("superclusters"))   event.getByLabel  (collections_.getParameter<edm::InputTag>  ("superclusters"),   handles_.superclusters);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "taus")            !=  objectsToGet_.end  ()  &&  collections_.exists  ("taus"))            event.getByLabel  (collections_.getParameter<edm::InputTag>  ("taus"),            handles_.taus);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "tracks")          !=  objectsToGet_.end  ()  &&  collections_.exists  ("tracks"))          event.getByLabel  (collections_.getParameter<edm::InputTag>  ("tracks"),          handles_.tracks);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "triggers")        !=  objectsToGet_.end  ()  &&  collections_.exists  ("triggers"))        event.getByLabel  (collections_.getParameter<edm::InputTag>  ("triggers"),        handles_.triggers);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "trigobjs")        !=  objectsToGet_.end  ()  &&  collections_.exists  ("trigobjs"))        event.getByLabel  (collections_.getParameter<edm::InputTag>  ("trigobjs"),        handles_.trigobjs);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "userVariables")   !=  objectsToGet_.end  ()  &&  collections_.exists  ("userVariables"))   event.getByLabel  (collections_.getParameter<edm::InputTag>  ("userVariables"),   handles_.userVariables);

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
  if (!initializeValueLookupTrees (unpackedCuts_, &handles_))
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
      updateCrossTalk (pl_->objectFlags);
      updateCrossTalk (pl_->cumulativeObjectFlags);
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
CutCalculator::setObjectFlags (const Cut &currentCut, unsigned currentCutIndex)
{
  string inputType = currentCut.inputLabel;
  if (currentCutIndex >= pl_->objectFlags.size ())
    pl_->objectFlags.resize (currentCutIndex + 1);
  if (currentCutIndex >= pl_->cumulativeObjectFlags.size ())
    pl_->cumulativeObjectFlags.resize (currentCutIndex + 1);
  pl_->objectFlags.at (currentCutIndex)[inputType];
  pl_->cumulativeObjectFlags.at (currentCutIndex)[inputType];

  for (vector<Operand>::const_iterator cutDecision = currentCut.valueLookupTree->evaluate ().begin (); cutDecision != currentCut.valueLookupTree->evaluate ().end (); cutDecision++)
    {
      unsigned object = (cutDecision - currentCut.valueLookupTree->evaluate ().begin ());
      bool flag = boost::get<double> (*cutDecision);

      pl_->objectFlags.at (currentCutIndex).at (inputType).push_back (flag);
      if (currentCutIndex > 0)
        flag = flag && pl_->cumulativeObjectFlags.at (currentCutIndex - 1).at (inputType).at (object);
      pl_->cumulativeObjectFlags.at (currentCutIndex).at (inputType).push_back (flag);
    }

  return true;
}

void
CutCalculator::updateCrossTalk (FlagMap &flags)
{
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
      objectsToGet_.push_back ("triggers");
    }
  else
    clog << "WARNING: no triggers have been specified." << endl;
  if (cuts_.exists ("triggersToVeto"))
    {
      unpackedTriggersToVeto_ = cuts_.getParameter<vector<string> > ("triggersToVeto");
      objectsToGet_.push_back("triggers");
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
      objectsToGet_.insert (objectsToGet_.end (), tempInputCollection.begin (), tempInputCollection.end ());
      //////////////////////////////////////////////////////////////////////////

      string catInputCollection = "";
      for (vector<string>::const_iterator collection = tempInputCollection.begin (); collection != tempInputCollection.end (); collection++)
        {
          if (collection != tempInputCollection.begin ())
            catInputCollection += "-";
          catInputCollection += *collection;
        }
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
  sort (objectsToGet_.begin (), objectsToGet_.end ());
  objectsToGet_.erase (unique (objectsToGet_.begin (), objectsToGet_.end ()), objectsToGet_.end ());

  return true;
}

void
CutCalculator::getTwoObjs (string tempInputCollection, string &obj1, string &obj2)
{
  //////////////////////////////////////////////////////////////////////////////
  // Extracts the names of the two object types from the name of a pair.
  //////////////////////////////////////////////////////////////////////////////
  int dashIndex = tempInputCollection.find ("-");
  int spaceIndex = tempInputCollection.find_last_of (" ");
  int secondWordLength = spaceIndex - dashIndex;
  obj1 = tempInputCollection.substr (0, dashIndex) + "s";
  obj2 = tempInputCollection.substr (dashIndex + 1, secondWordLength - 1) + "s";
  //////////////////////////////////////////////////////////////////////////////
}

template<typename T> bool
CutCalculator::evaluateComparison (T testValue, string comparison, T cutValue)
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

string
CutCalculator::getObjToGet (string obj)
{
  //////////////////////////////////////////////////////////////////////////////
  // Strip the word "secondary" from the beginning of a string.
  //////////////////////////////////////////////////////////////////////////////
  if (obj.find ("secondary") == string::npos)
    return obj;
  int firstSpaceIndex = obj.find_first_of (" ");
  return obj.substr (firstSpaceIndex + 1, obj.length () - 1);
  //////////////////////////////////////////////////////////////////////////////
}

vector<string>
CutCalculator::splitString (string inputString)
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
CutCalculator::evaluateTriggers ()
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
      for (BNtriggerCollection::const_iterator trigger = handles_.triggers->begin (); trigger != handles_.triggers->end (); trigger++)
        {
          //////////////////////////////////////////////////////////////////////////
          // If the current trigger matches one of the triggers to veto, record its
          // decision. If any of these triggers is true, set the event-wide flag to
          // false;
          //////////////////////////////////////////////////////////////////////////
          for (unsigned triggerIndex = 0; triggerIndex != pl_->triggersToVeto.size (); triggerIndex++)
            {
              if (trigger->name.find (pl_->triggersToVeto.at (triggerIndex)) == 0)
                {
                  vetoTriggerDecision = vetoTriggerDecision && !trigger->pass;
                  pl_->vetoTriggerFlags.at (triggerIndex) = trigger->pass;
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
              if (trigger->name.find (pl_->triggers.at (triggerIndex)) == 0)
                {
                  triggerDecision = triggerDecision || trigger->pass;
                  pl_->triggerFlags.at (triggerIndex) = trigger->pass;
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
CutCalculator::setEventFlags ()
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
      for (unsigned object = 0; object != pl_->cumulativeObjectFlags.at (currentCutIndex).at (currentCut.inputLabel).size (); object++)
        {
          if (pl_->cumulativeObjectFlags.at (currentCutIndex).at (currentCut.inputLabel).at (object))
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
          for (unsigned object = 0; object < pl_->cumulativeObjectFlags.at (currentCutIndex).at (currentCut.inputLabel).size (); object++)
            {
              bool passesPrevCuts = true;
              if (currentCutIndex > 0)
                passesPrevCuts = pl_->cumulativeObjectFlags.at (currentCutIndex - 1).at (currentCut.inputLabel).at (object);
              passesPrevCuts && numberPassingPrev++;
            }
          int numberFailCut = numberPassingPrev - numberPassing;
          cutDecision = evaluateComparison (numberFailCut, currentCut.eventComparativeOperator, currentCut.numberRequired);
        }

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
CutCalculator::initializeValueLookupTrees (vector<Cut> &cuts, Collections *handles)
{
  //////////////////////////////////////////////////////////////////////////////
  // Do nothing if it is not the first event.
  //////////////////////////////////////////////////////////////////////////////
  if (!firstEvent_)
    {
      return true;
    }
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // For each cut, parse its cut string into a new ValueLookupTree object which
  // is stored in the cut structure.
  //////////////////////////////////////////////////////////////////////////////
  for (vector<Cut>::iterator cut = cuts.begin (); cut != cuts.end (); cut++)
    {
      if (firstEvent_)
        {
          cut->valueLookupTree = new ValueLookupTree (*cut);
          if (!cut->valueLookupTree->isValid ())
            return false;
        }
      cut->valueLookupTree->setCollections (handles);
    }
  return true;
  //////////////////////////////////////////////////////////////////////////////
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(CutCalculator);
