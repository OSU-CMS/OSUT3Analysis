#include "OSUT3Analysis/AnaTools/plugins/CutCalculator.h"

CutCalculator::CutCalculator (const edm::ParameterSet &cfg) :
  collections_  (cfg.getParameter<edm::ParameterSet>   ("collections")),
  cuts_         (cfg.getParameter<edm::ParameterSet>  ("cuts")),
  firstEvent_   (true)
{
  bxlumis_         =  collections_.getParameter<edm::InputTag>  ("bxlumis");
  electrons_       =  collections_.getParameter<edm::InputTag>  ("electrons");
  events_          =  collections_.getParameter<edm::InputTag>  ("events");
  genjets_         =  collections_.getParameter<edm::InputTag>  ("genjets");
  jets_            =  collections_.getParameter<edm::InputTag>  ("jets");
  mcparticles_     =  collections_.getParameter<edm::InputTag>  ("mcparticles");
  mets_            =  collections_.getParameter<edm::InputTag>  ("mets");
  muons_           =  collections_.getParameter<edm::InputTag>  ("muons");
  photons_         =  collections_.getParameter<edm::InputTag>  ("photons");
  primaryvertexs_  =  collections_.getParameter<edm::InputTag>  ("primaryvertexs");
  secMuons_        =  collections_.getParameter<edm::InputTag>  ("secMuons");
  superclusters_   =  collections_.getParameter<edm::InputTag>  ("superclusters");
  taus_            =  collections_.getParameter<edm::InputTag>  ("taus");
  tracks_          =  collections_.getParameter<edm::InputTag>  ("tracks");
  triggers_        =  collections_.getParameter<edm::InputTag>  ("triggers");
  trigobjs_        =  collections_.getParameter<edm::InputTag>  ("trigobjs");

  unpackCuts ();
  produces<flagMap> ("cutDecisions");
}

CutCalculator::~CutCalculator ()
{
}

void
CutCalculator::produce (edm::Event &event, const edm::EventSetup &setup)
{
  event.getByLabel  (bxlumis_,         bxlumis);
  event.getByLabel  (electrons_,       electrons);
  event.getByLabel  (events_,          events);
  event.getByLabel  (genjets_,         genjets);
  event.getByLabel  (jets_,            jets);
  event.getByLabel  (mcparticles_,     mcparticles);
  event.getByLabel  (mets_,            mets);
  event.getByLabel  (muons_,           muons);
  event.getByLabel  (photons_,         photons);
  event.getByLabel  (primaryvertexs_,  primaryvertexs);
  event.getByLabel  (secMuons_,        secMuons);
  event.getByLabel  (superclusters_,   superclusters);
  event.getByLabel  (taus_,            taus);
  event.getByLabel  (tracks_,          tracks);
  event.getByLabel  (triggers_,        triggers);
  event.getByLabel  (trigobjs_,        trigobjs);

  if (!bxlumis.isValid () && firstEvent_)
    clog << "ERROR: failed to retrieve bxlumis collection from the event" << endl;
  if (!electrons.isValid () && firstEvent_)
    clog << "ERROR: failed to retrieve electrons collection from the event" << endl;
  if (!events.isValid () && firstEvent_)
    clog << "ERROR: failed to retrieve events collection from the event" << endl;
  if (!genjets.isValid () && firstEvent_)
    clog << "ERROR: failed to retrieve genjets collection from the event" << endl;
  if (!jets.isValid () && firstEvent_)
    clog << "ERROR: failed to retrieve jets collection from the event" << endl;
  if (!mcparticles.isValid () && firstEvent_)
    clog << "ERROR: failed to retrieve mcparticles collection from the event" << endl;
  if (!mets.isValid () && firstEvent_)
    clog << "ERROR: failed to retrieve mets collection from the event" << endl;
  if (!muons.isValid () && firstEvent_)
    clog << "ERROR: failed to retrieve muons collection from the event" << endl;
  if (!photons.isValid () && firstEvent_)
    clog << "ERROR: failed to retrieve photons collection from the event" << endl;
  if (!primaryvertexs.isValid () && firstEvent_)
    clog << "ERROR: failed to retrieve primaryvertexs collection from the event" << endl;
  if (!secMuons.isValid () && firstEvent_)
    clog << "ERROR: failed to retrieve secMuons collection from the event" << endl;
  if (!superclusters.isValid () && firstEvent_)
    clog << "ERROR: failed to retrieve superclusters collection from the event" << endl;
  if (!taus.isValid () && firstEvent_)
    clog << "ERROR: failed to retrieve taus collection from the event" << endl;
  if (!tracks.isValid () && firstEvent_)
    clog << "ERROR: failed to retrieve tracks collection from the event" << endl;
  if (!triggers.isValid () && firstEvent_)
    clog << "ERROR: failed to retrieve triggers collection from the event" << endl;
  if (!trigobjs.isValid () && firstEvent_)
    clog << "ERROR: failed to retrieve trigobjs collection from the event" << endl;

  initializeValueLookup ();

  individualFlags_ = auto_ptr<flagMap> (new flagMap);

  //loop over all cuts
  for (uint currentCutIndex = 0; currentCutIndex != unpackedCuts_.size (); currentCutIndex++)
    {
      cut currentCut = unpackedCuts_.at (currentCutIndex);

      for (uint currentObjectIndex = 0; currentObjectIndex != objectsToFlag.size (); currentObjectIndex++)
        {
          string currentObject = objectsToFlag.at (currentObjectIndex);

          //initialize maps to get ready to set cuts
          if (!individualFlags_->count (currentObject))
            (*individualFlags_)[currentObject] = vector<vector<bool> > ();
          (*individualFlags_)[currentObject].push_back (vector<bool> ());
        }

      //set flags for all relevant objects
      for (int currentObjectIndex = -1; currentObjectIndex != int (objectsToFlag.size ()); currentObjectIndex++)
        {
          string currentObject;
          if (currentObjectIndex < 0) currentObject = currentCut.inputCollection;  // In the first loop, set the flags for the collection that the cut is acting on.
                                                                                   // That way other paired collections can access the correct flag for the current cut.
          else currentObject = objectsToFlag.at (currentObjectIndex);
          if (currentObjectIndex >= 0 && currentObject == currentCut.inputCollection) continue;  // Flags have already been set for the inputCollection object, so should not be set again.

          // single object collections
          if        (currentObject  ==  "jets")                   setObjectFlags  (currentCut,  currentCutIndex,  jets,            "jets");
          else  if  (currentObject  ==  "secondary jets")         setObjectFlags  (currentCut,  currentCutIndex,  jets,            "secondary jets");
          else  if  (currentObject  ==  "secondary photons")      setObjectFlags  (currentCut,  currentCutIndex,  photons,         "secondary photons");
          else  if  (currentObject  ==  "muons")                  setObjectFlags  (currentCut,  currentCutIndex,  muons,           "muons");
          else  if  (currentObject  ==  "secondary muons")        setObjectFlags  (currentCut,  currentCutIndex,  secMuons,        "secondary muons");
          else  if  (currentObject  ==  "secondary electrons")    setObjectFlags  (currentCut,  currentCutIndex,  electrons,       "secondary electrons");
          else  if  (currentObject  ==  "electrons")              setObjectFlags  (currentCut,  currentCutIndex,  electrons,       "electrons");
          else  if  (currentObject  ==  "events")                 setObjectFlags  (currentCut,  currentCutIndex,  events,          "events");
          else  if  (currentObject  ==  "taus")                   setObjectFlags  (currentCut,  currentCutIndex,  taus,            "taus");
          else  if  (currentObject  ==  "mets")                   setObjectFlags  (currentCut,  currentCutIndex,  mets,            "mets");
          else  if  (currentObject  ==  "tracks")                 setObjectFlags  (currentCut,  currentCutIndex,  tracks,          "tracks");
          else  if  (currentObject  ==  "genjets")                setObjectFlags  (currentCut,  currentCutIndex,  genjets,         "genjets");
          else  if  (currentObject  ==  "mcparticles")            setObjectFlags  (currentCut,  currentCutIndex,  mcparticles,     "mcparticles");
          else  if  (currentObject  ==  "secondary mcparticles")  setObjectFlags  (currentCut,  currentCutIndex,  mcparticles,     "secondary mcparticles");
          else  if  (currentObject  ==  "primaryvertexs")         setObjectFlags  (currentCut,  currentCutIndex,  primaryvertexs,  "primaryvertexs");
          else  if  (currentObject  ==  "bxlumis")                setObjectFlags  (currentCut,  currentCutIndex,  bxlumis,         "bxlumis");
          else  if  (currentObject  ==  "photons")                setObjectFlags  (currentCut,  currentCutIndex,  photons,         "photons");
          else  if  (currentObject  ==  "superclusters")          setObjectFlags  (currentCut,  currentCutIndex,  superclusters,   "superclusters");
          else  if  (currentObject  ==  "trigobjs")               setObjectFlags  (currentCut,  currentCutIndex,  trigobjs,        "trigobjs");

          // paired object collections
          else  if  (currentObject  ==  "muon-muon pairs")                        setObjectFlags  (currentCut,  currentCutIndex,  muons,        muons,        "muon-muon pairs");
          else  if  (currentObject  ==  "muon-secondary muon pairs")              setObjectFlags  (currentCut,  currentCutIndex,  muons,        secMuons,     "muon-secondary muon pairs");
          else  if  (currentObject  ==  "muon-secondary photon pairs")            setObjectFlags  (currentCut,  currentCutIndex,  muons,        photons,      "muon-secondary photon pairs");
          else  if  (currentObject  ==  "muon-secondary jet pairs")               setObjectFlags  (currentCut,  currentCutIndex,  muons,        jets,         "muon-secondary jet pairs");
          else  if  (currentObject  ==  "photon-secondary jet pairs")             setObjectFlags  (currentCut,  currentCutIndex,  photons,      jets,         "photon-secondary jet pairs");
          else  if  (currentObject  ==  "electron-secondary jet pairs")           setObjectFlags  (currentCut,  currentCutIndex,  electrons,    jets,         "electron-secondary jet pairs");
          else  if  (currentObject  ==  "electron-secondary electron pairs")      setObjectFlags  (currentCut,  currentCutIndex,  electrons,    electrons,    "electron-secondary electron pairs");
          else  if  (currentObject  ==  "electron-electron pairs")                setObjectFlags  (currentCut,  currentCutIndex,  electrons,    electrons,    "electron-electron pairs");
          else  if  (currentObject  ==  "electron-muon pairs")                    setObjectFlags  (currentCut,  currentCutIndex,  electrons,    muons,        "electron-muon pairs");
          else  if  (currentObject  ==  "jet-secondary jet pairs")                setObjectFlags  (currentCut,  currentCutIndex,  jets,         jets,         "jet-secondary jet pairs");
          else  if  (currentObject  ==  "electron-jet pairs")                     setObjectFlags  (currentCut,  currentCutIndex,  electrons,    jets,         "electron-jet pairs");
          else  if  (currentObject  ==  "secondary electron-jet pairs")           setObjectFlags  (currentCut,  currentCutIndex,  electrons,    jets,         "secondary electron-jet pairs");
          else  if  (currentObject  ==  "secondary muon-jet pairs")               setObjectFlags  (currentCut,  currentCutIndex,  muons,        jets,         "secondary muon-jet pairs");
          else  if  (currentObject  ==  "electron-photon pairs")                  setObjectFlags  (currentCut,  currentCutIndex,  electrons,    photons,      "electron-photon pairs");
          else  if  (currentObject  ==  "photon-jet pairs")                       setObjectFlags  (currentCut,  currentCutIndex,  photons,      jets,         "photon-jet pairs");
          else  if  (currentObject  ==  "muon-jet pairs")                         setObjectFlags  (currentCut,  currentCutIndex,  muons,        jets,         "muon-jet pairs");
          else  if  (currentObject  ==  "muon-event pairs")                       setObjectFlags  (currentCut,  currentCutIndex,  muons,        events,       "muon-event pairs");
          else  if  (currentObject  ==  "electron-event pairs")                   setObjectFlags  (currentCut,  currentCutIndex,  electrons,    events,       "electron-event pairs");
          else  if  (currentObject  ==  "met-jet pairs")                          setObjectFlags  (currentCut,  currentCutIndex,  mets,         jets,         "met-jet pairs");
          else  if  (currentObject  ==  "met-mcparticle pairs")                   setObjectFlags  (currentCut,  currentCutIndex,  mets,         mcparticles,  "met-mcparticle pairs");
          else  if  (currentObject  ==  "jet-mcparticle pairs")                   setObjectFlags  (currentCut,  currentCutIndex,  jets,         mcparticles,  "jet-mcparticle pairs");
          else  if  (currentObject  ==  "jet-secondary mcparticle pairs")         setObjectFlags  (currentCut,  currentCutIndex,  jets,         mcparticles,  "jet-secondary mcparticle pairs");
          else  if  (currentObject  ==  "mcparticle-secondary mcparticle pairs")  setObjectFlags  (currentCut,  currentCutIndex,  mcparticles,  mcparticles,  "mcparticle-secondary mcparticle pairs");
          else  if  (currentObject  ==  "track-jet pairs")                        setObjectFlags  (currentCut,  currentCutIndex,  tracks,       jets,         "track-jet pairs");
          else  if  (currentObject  ==  "muon-photon pairs")                      setObjectFlags  (currentCut,  currentCutIndex,  muons,        photons,      "muon-photon pairs");
          else  if  (currentObject  ==  "track-event pairs")                      setObjectFlags  (currentCut,  currentCutIndex,  tracks,       events,       "track-event pairs");
          else  if  (currentObject  ==  "electron-track pairs")                   setObjectFlags  (currentCut,  currentCutIndex,  electrons,    tracks,       "electron-track pairs");
          else  if  (currentObject  ==  "muon-track pairs")                       setObjectFlags  (currentCut,  currentCutIndex,  muons,        tracks,       "muon-track pairs");
          else  if  (currentObject  ==  "jet-jet pairs")                          setObjectFlags  (currentCut,  currentCutIndex,  jets,         jets,         "jet-jet pairs");
          else  if  (currentObject  ==  "jet-track pairs")                        setObjectFlags  (currentCut,  currentCutIndex,  jets,         tracks,       "jet-track pairs");
          else  if  (currentObject  ==  "secondary muon-track pairs")             setObjectFlags  (currentCut,  currentCutIndex,  secMuons,     tracks,       "secondary muon-track pairs");
          else  if  (currentObject  ==  "muon-tau pairs")                         setObjectFlags  (currentCut,  currentCutIndex,  muons,        taus,         "muon-tau pairs");
          else  if  (currentObject  ==  "tau-tau pairs")                          setObjectFlags  (currentCut,  currentCutIndex,  taus,         taus,         "tau-tau pairs");
          else  if  (currentObject  ==  "tau-track pairs")                        setObjectFlags  (currentCut,  currentCutIndex,  taus,         tracks,       "tau-track pairs");
          else  if  (currentObject  ==  "electron-trigobj pairs")                 setObjectFlags  (currentCut,  currentCutIndex,  electrons,    trigobjs,     "electron-trigobj pairs");
          else  if  (currentObject  ==  "muon-trigobj pairs")                     setObjectFlags  (currentCut,  currentCutIndex,  muons,        trigobjs,     "muon-trigobj pairs");
          else  if  (currentObject  ==  "electron-mcparticle pairs")              setObjectFlags  (currentCut,  currentCutIndex,  electrons,    mcparticles,  "electron-mcparticle pairs");
          else  if  (currentObject  ==  "muon-secondary mcparticle pairs")        setObjectFlags  (currentCut,  currentCutIndex,  muons,        mcparticles,  "muon-secondary mcparticle pairs");
          else  if  (currentObject  ==  "muon-mcparticle pairs")                  setObjectFlags  (currentCut,  currentCutIndex,  muons,        mcparticles,  "muon-mcparticle pairs");
          else  if  (currentObject  ==  "track-mcparticle pairs")                 setObjectFlags  (currentCut,  currentCutIndex,  tracks,       mcparticles,  "track-mcparticle pairs");
        }
    }//end loop over all cuts

/*  bool triggerDecision = true;
  if (unpackedTriggers_.size () != 0 || unpackedTriggersToVeto_.size () != 0)  //triggers specified
    triggerDecision = evaluateTriggers (unpackedTriggers_, unpackedTriggersToVeto_, triggers);*/

  event.put (individualFlags_, "cutDecisions");
  firstEvent_ = false;
}

template <class InputCollection>
void CutCalculator::setObjectFlags (cut &currentCut, uint currentCutIndex, edm::Handle<InputCollection> inputCollection, string inputType)
{
  if (!inputCollection.isValid ())
    {
      clog << "ERROR:  invalid input collection for inputType=" << inputType << endl;
      return;
    }

  if (currentCut.inputCollection.find ("pair") != string::npos)
    {
      string obj1, obj2;
      getTwoObjs (currentCut.inputCollection, obj1, obj2);
      if (inputType==obj1 || inputType==obj2)
        {
          // Do not add a cut to individualFlags_ if the cut is on a paired
          // collection, and the inputType is a member of the pair.  The cut
          // will instead be applied when the setObjectFlags() is called for
          // the paired collection.  For example, if
          // currentCut.inputCollection==electron-muon pairs, then the flags
          // should not be set here when inputType==muons or
          // inputType==electrons.
          return;
        }
    }

  for (uint object = 0; object != inputCollection->size (); object++)
    {
      bool cutDecision = true;//object passes if this cut doesn't cut on that type of object

      if (currentCut.inputCollection == inputType)
        {
          vector<bool> subcutDecisions;
          for (int subcutIndex = 0; subcutIndex != currentCut.numSubcuts; subcutIndex++)
            {
              string stringValue = "";
              double value = vl_->valueLookup (&inputCollection->at (object), currentCut.variables.at (subcutIndex), currentCut.functions.at (subcutIndex), stringValue);
              if (stringValue == "")
                subcutDecisions.push_back (evaluateComparison (value, currentCut.comparativeOperators.at (subcutIndex), currentCut.cutValues.at (subcutIndex)));
              else
                subcutDecisions.push_back (evaluateComparison (stringValue, currentCut.comparativeOperators.at (subcutIndex), currentCut.cutStringValues.at (subcutIndex)));
            }
          if (currentCut.numSubcuts == 1)
            cutDecision = subcutDecisions.at (0);
          else
            {
              bool tempDecision = subcutDecisions.at (0);
              for (int subcutIndex = 1; subcutIndex < currentCut.numSubcuts; subcutIndex++)
                {
                  if (currentCut.logicalOperators.at (subcutIndex - 1) == "&" || currentCut.logicalOperators.at (subcutIndex - 1) == "&&")
                    tempDecision = tempDecision && subcutDecisions.at (subcutIndex);
                  else if (currentCut.logicalOperators.at (subcutIndex - 1) == "|"|| currentCut.logicalOperators.at (subcutIndex - 1) == "||")
                    tempDecision = tempDecision || subcutDecisions.at (subcutIndex);
                }
              cutDecision = tempDecision;
            }
          //invert the cut if this cut is a veto
          if (currentCut.isVeto)
            cutDecision = !cutDecision;
        }

      individualFlags_->at (inputType).at (currentCutIndex).push_back (cutDecision);
    }  //   for (uint object = 0; object != inputCollection->size (); object++)
} // end void CutCalculator::setObjectFlags


template <class InputCollection1, class InputCollection2>
void CutCalculator::setObjectFlags (cut &currentCut, uint currentCutIndex, edm::Handle<InputCollection1> inputCollection1, edm::Handle<InputCollection2> inputCollection2, string inputType)
{
  // This function sets the flags for the paired object collection.
  // If the cut is applying on the given paired object collection, then the flags for the single object collections are also set.
  // If not, then the flags for the paired object collection are taken as the AND of the flags for each single object collection.
  //
  if (!inputCollection1.isValid () || !inputCollection2.isValid ())
    {
      clog << "ERROR:  invalid input collection for inputType=" << inputType << endl;
      return;
    }

  // Get the strings for the two objects that make up the pair.
  string obj1Type, obj2Type;
  getTwoObjs (inputType, obj1Type, obj2Type);
  bool isTwoTypesOfObject = true;
  if (obj1Type==obj2Type)
    isTwoTypesOfObject = false;

  // Initialize the flags for individual objects to all be false, if the cut is on the pair.
  // Set them to true later, if any paired object passes (in which case both of its constituents should pass).
  if (currentCut.inputCollection == inputType)
    {
      for (uint object1 = 0; object1 != inputCollection1->size (); object1++)
        individualFlags_->at (obj1Type).at (currentCutIndex).push_back (false);
      if (isTwoTypesOfObject) // Only initialize the second object if it is different from the first.
        {
          for (uint object2 = 0; object2 != inputCollection2->size (); object2++)
            individualFlags_->at (obj2Type).at (currentCutIndex).push_back (false);
        }
    }

  int counter = 0;

  for (uint object1 = 0; object1 != inputCollection1->size (); object1++)
    {
      for (uint object2 = 0; object2 != inputCollection2->size (); object2++)
        {
          if (!isTwoTypesOfObject && object1 >= object2)//account for duplicate pairs if both collections are the same
            continue;

          bool cutDecision  = true;//object passes if this cut doesn't cut on that type of object
          bool plotDecision = true;

          // Determine whether each pair passes the cut, only if inputCollection is the same as the inputType.
          if (currentCut.inputCollection == inputType)
            {
              vector<bool> subcutDecisions;
              for (int subcutIndex = 0; subcutIndex != currentCut.numSubcuts; subcutIndex++)
                {
                  string stringValue = "";
                  double value = vl_->valueLookup (&inputCollection1->at (object1), &inputCollection2->at (object2), currentCut.variables.at (subcutIndex), currentCut.functions.at (subcutIndex), stringValue);
                  if (stringValue == "")
                    subcutDecisions.push_back (evaluateComparison (value, currentCut.comparativeOperators.at (subcutIndex), currentCut.cutValues.at (subcutIndex)));
                  else
                    subcutDecisions.push_back (evaluateComparison (stringValue, currentCut.comparativeOperators.at (subcutIndex), currentCut.cutStringValues.at (subcutIndex)));
                }

              if (currentCut.numSubcuts == 1) cutDecision = subcutDecisions.at (0);
              else
                {
                  bool tempDecision = subcutDecisions.at (0);
                  for (int subcutIndex = 1; subcutIndex < currentCut.numSubcuts; subcutIndex++)
                    {
                      if (currentCut.logicalOperators.at (subcutIndex-1) == "&" || currentCut.logicalOperators.at (subcutIndex-1) == "&&")
                        tempDecision = tempDecision && subcutDecisions.at (subcutIndex);
                      else if (currentCut.logicalOperators.at (subcutIndex-1) == "|"|| currentCut.logicalOperators.at (subcutIndex-1) == "||")
                        tempDecision = tempDecision || subcutDecisions.at (subcutIndex);
                    }
                  cutDecision = tempDecision;
                }
              //invert the cut if this cut is a veto
              if (currentCut.isVeto)
                cutDecision = !cutDecision;
              plotDecision = cutDecision;

              if (cutDecision)  // only set the flags for the individual objects if the pair object is being cut on
                {
                  individualFlags_->at (obj1Type).at (currentCutIndex).at (object1) = true;
                  individualFlags_->at (obj2Type).at (currentCutIndex).at (object2) = true;
                }
              if (plotDecision)  // only set the flags for the individual objects if the pair object is being cut on
                {
                  individualFlags_->at (obj1Type).at (currentCutIndex).at (object1) = true;
                  individualFlags_->at (obj2Type).at (currentCutIndex).at (object2) = true;
                }
            } // if (currentCut.inputCollection == inputType)

          // The individualFlags_ will be true if the inputCollection is not the same as the inputType.
          // They are also independent of the previous flags on the single objects.
          individualFlags_->at (inputType).at (currentCutIndex).push_back (cutDecision);

          counter++;

        } // end   for (uint object2 = 0; object2 != inputCollection2->size (); object2++)
    }  // end   for (uint object1 = 0; object1 != inputCollection1->size (); object1++)

} // end void CutCalculator::setObjectFlags

void
CutCalculator::unpackCuts ()
{
  //set triggers for this channel
  if (cuts_.exists ("triggers"))
    {
      unpackedTriggers_ = cuts_.getParameter<vector<string> > ("triggers");
      objectsToGet.push_back ("triggers");
    }
  if (cuts_.exists ("triggersToVeto"))
    {
      unpackedTriggersToVeto_ = cuts_.getParameter<vector<string> > ("triggersToVeto");
      objectsToGet.push_back("triggers");
    }

  vector<string> subSubDirNames;//subdirectories in each channel.
  edm::VParameterSet cuts = cuts_.getParameter<edm::VParameterSet> ("cuts");

  for (uint currentCut = 0; currentCut != cuts.size (); currentCut++)
    {
      cut tempCut;
      //store input collection for cut
      string tempInputCollection = cuts.at (currentCut).getParameter<string> ("inputCollection");

      if  (tempInputCollection  ==  "muon-electron pairs")                    tempInputCollection  =  "electron-muon pairs";
      if  (tempInputCollection  ==  "photon-electron pairs")                  tempInputCollection  =  "electron-photon pairs";
      if  (tempInputCollection  ==  "photon-muon pairs")                      tempInputCollection  =  "muon-photon pairs";
      if  (tempInputCollection  ==  "jet-electron pairs")                     tempInputCollection  =  "electron-jet pairs";
      if  (tempInputCollection  ==  "jet-muon pairs")                         tempInputCollection  =  "muon-jet pairs";
      if  (tempInputCollection  ==  "event-muon pairs")                       tempInputCollection  =  "muon-event pairs";
      if  (tempInputCollection  ==  "event-electron pairs")                   tempInputCollection  =  "electron-event pairs";
      if  (tempInputCollection  ==  "jet-met pairs")                          tempInputCollection  =  "met-jet pairs";
      if  (tempInputCollection  ==  "mcparticle-met pairs")                   tempInputCollection  =  "met-mcparticle pairs";
      if  (tempInputCollection  ==  "secondary mcparticle-jet pairs")         tempInputCollection  =  "jet-secondary mcparticle pairs";
      if  (tempInputCollection  ==  "mcparticle-jet pairs")                   tempInputCollection  =  "jet-mcparticle pairs";
      if  (tempInputCollection  ==  "secondary mcparticle-mcparticle pairs")  tempInputCollection  =  "mcparticle-secondary mcparticle pairs";
      if  (tempInputCollection  ==  "track-jet pairs")                        tempInputCollection  =  "track-jet pairs";
      if  (tempInputCollection  ==  "jet-photon pairs")                       tempInputCollection  =  "photon-jet pairs";
      if  (tempInputCollection  ==  "event-track pairs")                      tempInputCollection  =  "track-event pairs";
      if  (tempInputCollection  ==  "secondary muon-muon pairs")              tempInputCollection  =  "muon-secondary muon pairs";
      if  (tempInputCollection  ==  "secondary jet-jet pairs")                tempInputCollection  =  "jet-secondary jet pairs";
      if  (tempInputCollection  ==  "secondary jet-muon pairs")               tempInputCollection  =  "muon-secondary jet pairs";
      if  (tempInputCollection  ==  "secondary photon-muon pairs")            tempInputCollection  =  "muon-secondary photon pairs";
      if  (tempInputCollection  ==  "secondary jet-photon pairs")             tempInputCollection  =  "photon-secondary jet pairs";
      if  (tempInputCollection  ==  "secondary jet-electron pairs")           tempInputCollection  =  "electron-secondary jet pairs";
      if  (tempInputCollection  ==  "jet-secondary muon pairs")               tempInputCollection  =  "secondary muon-jet pairs";
      if  (tempInputCollection  ==  "jet-secondary electron pairs")           tempInputCollection  =  "secondary electron-jet pairs";
      if  (tempInputCollection  ==  "secondary electron-electron pairs")      tempInputCollection  =  "electron-secondary electron pairs";
      if  (tempInputCollection  ==  "trigobj-electron pairs")                 tempInputCollection  =  "electron-trigobj pairs";
      if  (tempInputCollection  ==  "trigobj-muon pairs")                     tempInputCollection  =  "muon-trigobj pairs";
      if  (tempInputCollection  ==  "mcparticle-electron pairs")              tempInputCollection  =  "electron-mcparticle pairs";
      if  (tempInputCollection  ==  "mcparticle-muon pairs")                  tempInputCollection  =  "muon-mcparticle pairs";
      if  (tempInputCollection  ==  "secondary mcparticle-muon pairs")        tempInputCollection  =  "muon-secondary mcparticle pairs";
      if  (tempInputCollection  ==  "mcparticle-track pairs")                 tempInputCollection  =  "track-mcparticle pairs";

      tempCut.inputCollection = tempInputCollection;
      if (tempInputCollection.find ("pairs")==string::npos) //just a single object
        {
          if (tempInputCollection.find ("secondary")!=string::npos)//secondary object
            {
              if (tempInputCollection.find ("secondary muons")!=string::npos)//treat secondary muons differently; allow for a different input collection
                objectsToGet.push_back ("secondary muons");
              else
                {
                  int spaceIndex = tempInputCollection.find (" ");
                  int secondWordLength = tempInputCollection.size () - spaceIndex;
                  objectsToGet.push_back (tempInputCollection.substr (spaceIndex + 1, secondWordLength));
                }
            }
          else
            objectsToGet.push_back (tempInputCollection);
          objectsToCut.push_back (tempInputCollection);
          objectsToFlag.push_back (tempInputCollection);
        }
      else//pair of objects, need to add them both to objectsToGet
        {
          string obj1;
          string obj2;
          getTwoObjs (tempInputCollection, obj1, obj2);
          string obj2ToGet = getObjToGet (obj2);
          objectsToCut.push_back (tempInputCollection);
          objectsToCut.push_back (obj1);
          objectsToCut.push_back (obj2);
          objectsToFlag.push_back (tempInputCollection);
          objectsToFlag.push_back (obj1);
          objectsToFlag.push_back (obj2);
          objectsToGet.push_back (tempInputCollection);
          objectsToGet.push_back (obj1);
          objectsToGet.push_back (obj2ToGet);
        }

      //split cut string into parts and store them
      string cutString = cuts.at (currentCut).getParameter<string> ("cutString");
      vector<string> cutStringVector = splitString (cutString);
      if (cutStringVector.size () != 3 && cutStringVector.size () % 4 != 3)
        {
          clog << "Error: Didn't find the expected number elements in the following cut string: '" << cutString << "'\n";
          exit (0);
        }
      tempCut.numSubcuts = (cutStringVector.size () + 1) / 4;
      for (int subcutIndex = 0; subcutIndex != tempCut.numSubcuts; subcutIndex++)//loop over all the pieces of the cut combined using &,|
        {
          int indexOffset = 4 * subcutIndex;
          string currentVariableString = cutStringVector.at (indexOffset);
          if (currentVariableString.find ("(") == string::npos)
            {
              tempCut.functions.push_back ("");//no function was specified
              tempCut.variables.push_back (currentVariableString);// variable to cut on
            }
          else
            {
              tempCut.functions.push_back (currentVariableString.substr (0, currentVariableString.find ("(")));//function comes before the "("
              string tempVariable = currentVariableString.substr (currentVariableString.find ("(") + 1);//get rest of string
              tempCut.variables.push_back (tempVariable.substr (0, tempVariable.size () - 1));//remove trailing ")"
            }
          tempCut.comparativeOperators.push_back (cutStringVector.at (indexOffset + 1));// comparison to make
          tempCut.cutValues.push_back (atof (cutStringVector.at (indexOffset + 2).c_str ()));// threshold value to pass cut
          tempCut.cutStringValues.push_back (cutStringVector.at (indexOffset + 2));// string value to pass cut
          if (subcutIndex != 0)
            tempCut.logicalOperators.push_back (cutStringVector.at (indexOffset - 1)); // logical comparison (and, or)
        }

      //get number of objects required to pass cut for event to pass
      string numberRequiredString = cuts.at (currentCut).getParameter<string> ("numberRequired");
      vector<string> numberRequiredVector = splitString (numberRequiredString);
      if (numberRequiredVector.size () != 2)
        {
          clog << "Error: Didn't find two elements in the following number requirement string: '" << numberRequiredString << "'\n";
          exit (0);
        }

      int numberRequiredInt = atoi (numberRequiredVector.at (1).c_str ());
      tempCut.numberRequired = numberRequiredInt;// number of objects required to pass the cut
      tempCut.eventComparativeOperator = numberRequiredVector.at (0);// comparison to make

      //Set up vectors to store the directories and subDirectories for each channel.
      string subSubDirName;
      string tempCutName;
      if (cuts.at (currentCut).exists ("alias"))
        {
          tempCutName = cuts.at (currentCut).getParameter<string> ("alias");
          subSubDirName = "After " + tempCutName + " Cut Applied";
        }
      else
        {
          //construct string for cutflow table
          bool plural = numberRequiredInt != 1;
          string collectionString = plural ? tempInputCollection : tempInputCollection.substr (0, tempInputCollection.size () - 1);
          string cutName =  numberRequiredString + " " + collectionString + " with " + cutString;
          tempCutName = cutName;
          subSubDirName = "After " + numberRequiredString + " " + collectionString + " with " + cutString + " Cut Applied";
        }

      tempCut.isVeto = false;
      if (cuts.at (currentCut).exists ("isVeto"))
        tempCut.isVeto = cuts.at (currentCut).getParameter<bool> ("isVeto");
      subSubDirNames.push_back (subSubDirName);
      tempCut.name = tempCutName;

      unpackedCuts_.push_back (tempCut);

    }//end loop over cuts
}

void
CutCalculator::getTwoObjs (string tempInputCollection, string &obj1, string &obj2)
{
  // Set two object strings from the tempInputCollection string,
  // For example, if tempInputCollection is "electron-muon pairs",
  // then obj1 = "electrons" and obj2 = "muons".
  // Note that the objects have an "s" appended.

  int dashIndex = tempInputCollection.find ("-");
  int spaceIndex = tempInputCollection.find_last_of (" ");
  int secondWordLength = spaceIndex - dashIndex;
  obj1 = tempInputCollection.substr (0, dashIndex) + "s";
  obj2 = tempInputCollection.substr (dashIndex + 1, secondWordLength - 1) + "s";
}

bool
CutCalculator::evaluateComparison (double testValue, string comparison, double cutValue)
{
  bool returnValue;

  if (comparison == ">")
    returnValue = testValue >  cutValue;
  else if (comparison == ">=")
    returnValue = testValue >= cutValue;
  else if (comparison == "<")
    returnValue = testValue <  cutValue;
  else if (comparison == "<=")
    returnValue = testValue <= cutValue;
  else if (comparison == "==")
    returnValue = testValue == cutValue;
  else if (comparison == "=")
    returnValue = testValue == cutValue;
  else if (comparison == "!=")
    returnValue = testValue != cutValue;
  else
    {
      clog << "WARNING: invalid comparison operator '" << comparison << "'\n";
      returnValue = false;
    }

  return returnValue;
}

bool
CutCalculator::evaluateComparison (string testValue, string comparison, string cutValue)
{
  bool returnValue;

  if (comparison == ">")
    returnValue = testValue >  cutValue;
  else if (comparison == ">=")
    returnValue = testValue >= cutValue;
  else if (comparison == "<")
    returnValue = testValue <  cutValue;
  else if (comparison == "<=")
    returnValue = testValue <= cutValue;
  else if (comparison == "==")
    returnValue = testValue == cutValue;
  else if (comparison == "=")
    returnValue = testValue == cutValue;
  else if (comparison == "!=")
    returnValue = testValue != cutValue;
  else
    {
      clog << "WARNING: invalid comparison operator '" << comparison << "'\n";
      returnValue = false;
    }

  return returnValue;
}

string
CutCalculator::getObjToGet (string obj)
{
  // Return the string corresponding to the object to get for the given obj string.
  // Right now this only handles the case in which obj contains "secondary",
  // e.g, "secondary muons".
  // Note that "s" is NOT appended.

  if (obj.find ("secondary") == string::npos)  // "secondary" is not found
    return obj;
  int firstSpaceIndex = obj.find_first_of (" ");
  return obj.substr (firstSpaceIndex+1, obj.length () - 1);
}

vector<string>
CutCalculator::splitString (string inputString)
{
  stringstream stringStream(inputString);
  istream_iterator<string> begin(stringStream);
  istream_iterator<string> end;
  vector<string> stringVector(begin, end);
  return stringVector;
}

bool
CutCalculator::evaluateTriggers (vector<string> triggersToTest, vector<string> triggersToVeto, const edm::Handle<BNtriggerCollection> triggerCollection)
{
  //initialize to false until a chosen trigger is passed
  bool triggerDecision = false;

  //loop over all triggers defined in the event
  for (BNtriggerCollection::const_iterator trigger = triggerCollection->begin (); trigger != triggerCollection->end (); trigger++)
    {
      //we're only interested in triggers that actually passed
      if (trigger->pass != 1)
        continue;

      //if the event passes one of the veto triggers, exit and return false
      for (uint triggerName = 0; triggerName != triggersToVeto.size (); triggerName++)
        {
          if (trigger->name.find (triggersToVeto.at (triggerName)) != string::npos)
            return false;
        }
      //if the event passes one of the chosen triggers, set triggerDecision to true
      for (uint triggerName = 0; triggerName != triggersToTest.size (); triggerName++)
        {
          if (trigger->name.find (triggersToTest.at (triggerName)) != string::npos)
            triggerDecision = true;
        }
    }

  //if none of the veto triggers fired:
  //return the OR of all the chosen triggers
  if (triggersToTest.size () != 0)
    return triggerDecision;
  //or if no triggers were defined return true
  else
    return true;
}

void
CutCalculator::initializeValueLookup ()
{
  //////////////////////////////////////////////////////////////////////////////
  // Create a new ValueLookup object if it does not already exist.
  //////////////////////////////////////////////////////////////////////////////
  if (!vl_)
    vl_ = new ValueLookup ();
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // Set all the private object collections of the ValueLookup object to the
  // values in this OSUAnalysis object.
  //////////////////////////////////////////////////////////////////////////////
  vl_->setBxlumis                   (bxlumis);
  vl_->setElectrons                 (electrons);
  vl_->setEvents                    (events);
  vl_->setGenjets                   (genjets);
  vl_->setJets                      (jets);
  vl_->setMcparticles               (mcparticles);
  vl_->setMets                      (mets);
  vl_->setMuons                     (muons);
  vl_->setPhotons                   (photons);
  vl_->setPrimaryvertexs            (primaryvertexs);
  vl_->setRhokt6CaloJetsHandle      (rhokt6CaloJetsHandle_);
  vl_->setSecMuons                  (secMuons);
  vl_->setStops                     (stops);
  vl_->setSuperclusters             (superclusters);
  vl_->setTaus                      (taus);
  vl_->setTracks                    (tracks);
  vl_->setTriggers                  (triggers);
  vl_->setTrigobjs                  (trigobjs);
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // Set all the private variables of the ValueLookup object to the
  // corresponding values in this OSUAnalysis object.
  //////////////////////////////////////////////////////////////////////////////
  vl_->setBTagScaleFactor           (bTagScaleFactor_);
  vl_->setBadCSCFile                (badCSCFile_);
  vl_->setBadCSCVec                 (BadCSCVec);
  vl_->setChannelScaleFactor        (channelScaleFactor_);
  vl_->setCumulativeFlags           (cumulativeFlags);
  vl_->setDeadEcalFile              (deadEcalFile_);
  vl_->setDeadEcalVec               (DeadEcalVec);
  vl_->setDoPileupReweighting       (doPileupReweighting_);
  vl_->setEcaloVaryScaleFactor      (EcaloVaryScaleFactor_);
  vl_->setElectronScaleFactor       (electronScaleFactor_);
  vl_->setElectronTrackScaleFactor  (electronTrackScaleFactor_);
  vl_->setEventScaleFactor          (eventScaleFactor_);
  vl_->setFlagJESJERCorr            (flagJESJERCorr_);
  vl_->setGlobalScaleFactor         (globalScaleFactor_);
  vl_->setIsrVaryScaleFactor        (isrVaryScaleFactor_);
  vl_->setJESJERCorr                (jESJERCorr_);
  vl_->setMuonScaleFactor           (muonScaleFactor_);
  vl_->setMuonTrackScaleFactor      (muonTrackScaleFactor_);
  vl_->setNmissoutShiftDown         (NmissoutShiftDown_);
  vl_->setNmissoutShiftUp           (NmissoutShiftUp_);
  vl_->setObjectsToCut              (objectsToCut);
  vl_->setObjectsToGet              (objectsToGet);
  vl_->setPuWeight                  (puWeight_);
  vl_->setStopCTauScaleFactor       (stopCTauScaleFactor_);
  vl_->setTargetTriggers            (targetTriggers_);
  vl_->setTopPtScaleFactor          (topPtScaleFactor_);
  vl_->setTrackNMissOutScaleFactor  (trackNMissOutScaleFactor_);
  vl_->setTriggerMetScaleFactor     (triggerMetScaleFactor_);
  vl_->setTriggerScaleFactor        (triggerScaleFactor_);
  vl_->setDatasetType               (datasetType_);
  vl_->setUseTrackCaloRhoCorr       (useTrackCaloRhoCorr_);
  vl_->setVerbose                   (verbose_);
  //////////////////////////////////////////////////////////////////////////////
}

DEFINE_FWK_MODULE(CutCalculator);
