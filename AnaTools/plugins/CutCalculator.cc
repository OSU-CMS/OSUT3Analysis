#include <iostream>
#include <algorithm>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/plugins/CutCalculator.h"

CutCalculator::CutCalculator (const edm::ParameterSet &cfg) :
  collections_  (cfg.getParameter<edm::ParameterSet>   ("collections")),
  cuts_         (cfg.getParameter<edm::ParameterSet>  ("cuts")),
  firstEvent_   (true),
  vl_           (NULL)
{
  //////////////////////////////////////////////////////////////////////////////
  // Retrieve the object collection names from the collections ParameterSet.
  //////////////////////////////////////////////////////////////////////////////
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
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // Try to unpack the cuts ParameterSet and quit if there is a problem.
  //////////////////////////////////////////////////////////////////////////////
  if (!unpackCuts ())
    {
      clog << "ERROR: failed to interpret cuts PSet. Quitting..." << endl;
      exit (1);
    }
  //////////////////////////////////////////////////////////////////////////////

  produces<CutCalculatorPayload> ("cutDecisions");
}

CutCalculator::~CutCalculator ()
{
  //////////////////////////////////////////////////////////////////////////////
  // Destroy the ValueLookup object if it exists.
  //////////////////////////////////////////////////////////////////////////////
  if (vl_)
    delete vl_;
  //////////////////////////////////////////////////////////////////////////////
}

void
CutCalculator::produce (edm::Event &event, const edm::EventSetup &setup)
{
  //////////////////////////////////////////////////////////////////////////////
  // Retrieve each object collection which we need and print a warning if it is
  // missing.
  //////////////////////////////////////////////////////////////////////////////
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "bxlumis")         !=  objectsToGet_.end  ())  event.getByLabel  (bxlumis_,         bxlumis);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "electrons")       !=  objectsToGet_.end  ())  event.getByLabel  (electrons_,       electrons);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "events")          !=  objectsToGet_.end  ())  event.getByLabel  (events_,          events);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "genjets")         !=  objectsToGet_.end  ())  event.getByLabel  (genjets_,         genjets);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "jets")            !=  objectsToGet_.end  ())  event.getByLabel  (jets_,            jets);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "mcparticles")     !=  objectsToGet_.end  ())  event.getByLabel  (mcparticles_,     mcparticles);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "mets")            !=  objectsToGet_.end  ())  event.getByLabel  (mets_,            mets);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "muons")           !=  objectsToGet_.end  ())  event.getByLabel  (muons_,           muons);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "photons")         !=  objectsToGet_.end  ())  event.getByLabel  (photons_,         photons);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "primaryvertexs")  !=  objectsToGet_.end  ())  event.getByLabel  (primaryvertexs_,  primaryvertexs);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "secMuons")        !=  objectsToGet_.end  ())  event.getByLabel  (secMuons_,        secMuons);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "superclusters")   !=  objectsToGet_.end  ())  event.getByLabel  (superclusters_,   superclusters);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "taus")            !=  objectsToGet_.end  ())  event.getByLabel  (taus_,            taus);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "tracks")          !=  objectsToGet_.end  ())  event.getByLabel  (tracks_,          tracks);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "triggers")        !=  objectsToGet_.end  ())  event.getByLabel  (triggers_,        triggers);
  if  (find  (objectsToGet_.begin  (),  objectsToGet_.end  (),  "trigobjs")        !=  objectsToGet_.end  ())  event.getByLabel  (trigobjs_,        trigobjs);

  if (firstEvent_ && !bxlumis.isValid ())
    clog << "INFO: did not retrieve bxlumis collection from the event." << endl;
  if (firstEvent_ && !electrons.isValid ())
    clog << "INFO: did not retrieve electrons collection from the event." << endl;
  if (firstEvent_ && !events.isValid ())
    clog << "INFO: did not retrieve events collection from the event." << endl;
  if (firstEvent_ && !genjets.isValid ())
    clog << "INFO: did not retrieve genjets collection from the event." << endl;
  if (firstEvent_ && !jets.isValid ())
    clog << "INFO: did not retrieve jets collection from the event." << endl;
  if (firstEvent_ && !mcparticles.isValid ())
    clog << "INFO: did not retrieve mcparticles collection from the event." << endl;
  if (firstEvent_ && !mets.isValid ())
    clog << "INFO: did not retrieve mets collection from the event." << endl;
  if (firstEvent_ && !muons.isValid ())
    clog << "INFO: did not retrieve muons collection from the event." << endl;
  if (firstEvent_ && !photons.isValid ())
    clog << "INFO: did not retrieve photons collection from the event." << endl;
  if (firstEvent_ && !primaryvertexs.isValid ())
    clog << "INFO: did not retrieve primaryvertexs collection from the event." << endl;
  if (firstEvent_ && !secMuons.isValid ())
    clog << "INFO: did not retrieve secMuons collection from the event." << endl;
  if (firstEvent_ && !superclusters.isValid ())
    clog << "INFO: did not retrieve superclusters collection from the event." << endl;
  if (firstEvent_ && !taus.isValid ())
    clog << "INFO: did not retrieve taus collection from the event." << endl;
  if (firstEvent_ && !tracks.isValid ())
    clog << "INFO: did not retrieve tracks collection from the event." << endl;
  if (firstEvent_ && !triggers.isValid ())
    clog << "INFO: did not retrieve triggers collection from the event." << endl;
  if (firstEvent_ && !trigobjs.isValid ())
    clog << "INFO: did not retrieve trigobjs collection from the event." << endl;
  //////////////////////////////////////////////////////////////////////////////

  // Set all the private variables in the ValueLookup object before using it.
  initializeValueLookup ();

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
      cut currentCut = pl_->cuts.at (currentCutIndex);

      //////////////////////////////////////////////////////////////////////////
      // Initialize the object flags for this cut in the payload with blank
      // vectors.
      //////////////////////////////////////////////////////////////////////////
      for (unsigned currentObjectIndex = 0; currentObjectIndex != objectsToFlag_.size (); currentObjectIndex++)
        {
          string currentObject = objectsToFlag_.at (currentObjectIndex);

          if (!pl_->objectFlags.count (currentObject))
            pl_->objectFlags[currentObject] = vector<vector<bool> > ();
          pl_->objectFlags[currentObject].push_back (vector<bool> ());
        }
      //////////////////////////////////////////////////////////////////////////

      // Loop over each object type to set flags for each object indicating
      // whether it passed the current cut.
      for (int currentObjectIndex = -1; pl_->isValid && currentObjectIndex != (int) objectsToFlag_.size (); currentObjectIndex++)
        {
          //////////////////////////////////////////////////////////////////////
          // The first iteration sets the flags for the current cut so that
          // other collections can use this flag.
          //////////////////////////////////////////////////////////////////////
          string currentObject;
          if (currentObjectIndex < 0)
            currentObject = currentCut.inputCollection;
          else
            currentObject = objectsToFlag_.at (currentObjectIndex);
          if (currentObjectIndex >= 0 && currentObject == currentCut.inputCollection)
            continue;

          //////////////////////////////////////////////////////////////////////
          // Set the flags for each object in single object collections.
          //////////////////////////////////////////////////////////////////////
          if        (currentObject  ==  "bxlumis")                pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  bxlumis,         "bxlumis");
          else  if  (currentObject  ==  "electrons")              pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  electrons,       "electrons");
          else  if  (currentObject  ==  "events")                 pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  events,          "events");
          else  if  (currentObject  ==  "genjets")                pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  genjets,         "genjets");
          else  if  (currentObject  ==  "mcparticles")            pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  mcparticles,     "mcparticles");
          else  if  (currentObject  ==  "mets")                   pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  mets,            "mets");
          else  if  (currentObject  ==  "muons")                  pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  muons,           "muons");
          else  if  (currentObject  ==  "photons")                pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  photons,         "photons");
          else  if  (currentObject  ==  "primaryvertexs")         pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  primaryvertexs,  "primaryvertexs");
          else  if  (currentObject  ==  "secondary electrons")    pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  electrons,       "secondary electrons");
          else  if  (currentObject  ==  "secondary jets")         pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  jets,            "secondary jets");
          else  if  (currentObject  ==  "secondary mcparticles")  pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  mcparticles,     "secondary mcparticles");
          else  if  (currentObject  ==  "secondary muons")        pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  secMuons,        "secondary muons");
          else  if  (currentObject  ==  "secondary photons")      pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  photons,         "secondary photons");
          else  if  (currentObject  ==  "superclusters")          pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  superclusters,   "superclusters");
          else  if  (currentObject  ==  "taus")                   pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  taus,            "taus");
          else  if  (currentObject  ==  "tracks")                 pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  tracks,          "tracks");
          else  if  (currentObject  ==  "trigobjs")               pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  trigobjs,        "trigobjs");
          else  if  (currentObject  ==  "jets")                   pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  jets,            "jets");
          //////////////////////////////////////////////////////////////////////

          //////////////////////////////////////////////////////////////////////
          // Set the flags for each object in paired object collections.
          //////////////////////////////////////////////////////////////////////
          else  if  (currentObject  ==  "electron-electron pairs")                pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  electrons,    electrons,    "electron-electron pairs");
          else  if  (currentObject  ==  "electron-event pairs")                   pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  electrons,    events,       "electron-event pairs");
          else  if  (currentObject  ==  "electron-jet pairs")                     pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  electrons,    jets,         "electron-jet pairs");
          else  if  (currentObject  ==  "electron-mcparticle pairs")              pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  electrons,    mcparticles,  "electron-mcparticle pairs");
          else  if  (currentObject  ==  "electron-muon pairs")                    pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  electrons,    muons,        "electron-muon pairs");
          else  if  (currentObject  ==  "electron-photon pairs")                  pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  electrons,    photons,      "electron-photon pairs");
          else  if  (currentObject  ==  "electron-secondary electron pairs")      pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  electrons,    electrons,    "electron-secondary electron pairs");
          else  if  (currentObject  ==  "electron-secondary jet pairs")           pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  electrons,    jets,         "electron-secondary jet pairs");
          else  if  (currentObject  ==  "electron-track pairs")                   pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  electrons,    tracks,       "electron-track pairs");
          else  if  (currentObject  ==  "electron-trigobj pairs")                 pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  electrons,    trigobjs,     "electron-trigobj pairs");
          else  if  (currentObject  ==  "jet-jet pairs")                          pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  jets,         jets,         "jet-jet pairs");
          else  if  (currentObject  ==  "jet-mcparticle pairs")                   pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  jets,         mcparticles,  "jet-mcparticle pairs");
          else  if  (currentObject  ==  "jet-secondary jet pairs")                pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  jets,         jets,         "jet-secondary jet pairs");
          else  if  (currentObject  ==  "jet-secondary mcparticle pairs")         pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  jets,         mcparticles,  "jet-secondary mcparticle pairs");
          else  if  (currentObject  ==  "jet-track pairs")                        pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  jets,         tracks,       "jet-track pairs");
          else  if  (currentObject  ==  "mcparticle-secondary mcparticle pairs")  pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  mcparticles,  mcparticles,  "mcparticle-secondary mcparticle pairs");
          else  if  (currentObject  ==  "met-jet pairs")                          pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  mets,         jets,         "met-jet pairs");
          else  if  (currentObject  ==  "met-mcparticle pairs")                   pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  mets,         mcparticles,  "met-mcparticle pairs");
          else  if  (currentObject  ==  "muon-event pairs")                       pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  muons,        events,       "muon-event pairs");
          else  if  (currentObject  ==  "muon-jet pairs")                         pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  muons,        jets,         "muon-jet pairs");
          else  if  (currentObject  ==  "muon-mcparticle pairs")                  pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  muons,        mcparticles,  "muon-mcparticle pairs");
          else  if  (currentObject  ==  "muon-muon pairs")                        pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  muons,        muons,        "muon-muon pairs");
          else  if  (currentObject  ==  "muon-photon pairs")                      pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  muons,        photons,      "muon-photon pairs");
          else  if  (currentObject  ==  "muon-secondary jet pairs")               pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  muons,        jets,         "muon-secondary jet pairs");
          else  if  (currentObject  ==  "muon-secondary mcparticle pairs")        pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  muons,        mcparticles,  "muon-secondary mcparticle pairs");
          else  if  (currentObject  ==  "muon-secondary muon pairs")              pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  muons,        secMuons,     "muon-secondary muon pairs");
          else  if  (currentObject  ==  "muon-secondary photon pairs")            pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  muons,        photons,      "muon-secondary photon pairs");
          else  if  (currentObject  ==  "muon-tau pairs")                         pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  muons,        taus,         "muon-tau pairs");
          else  if  (currentObject  ==  "muon-track pairs")                       pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  muons,        tracks,       "muon-track pairs");
          else  if  (currentObject  ==  "muon-trigobj pairs")                     pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  muons,        trigobjs,     "muon-trigobj pairs");
          else  if  (currentObject  ==  "photon-jet pairs")                       pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  photons,      jets,         "photon-jet pairs");
          else  if  (currentObject  ==  "photon-secondary jet pairs")             pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  photons,      jets,         "photon-secondary jet pairs");
          else  if  (currentObject  ==  "secondary electron-jet pairs")           pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  electrons,    jets,         "secondary electron-jet pairs");
          else  if  (currentObject  ==  "secondary muon-jet pairs")               pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  muons,        jets,         "secondary muon-jet pairs");
          else  if  (currentObject  ==  "secondary muon-track pairs")             pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  secMuons,     tracks,       "secondary muon-track pairs");
          else  if  (currentObject  ==  "tau-tau pairs")                          pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  taus,         taus,         "tau-tau pairs");
          else  if  (currentObject  ==  "tau-track pairs")                        pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  taus,         tracks,       "tau-track pairs");
          else  if  (currentObject  ==  "track-event pairs")                      pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  tracks,       events,       "track-event pairs");
          else  if  (currentObject  ==  "track-jet pairs")                        pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  tracks,       jets,         "track-jet pairs");
          else  if  (currentObject  ==  "track-mcparticle pairs")                 pl_->isValid  =  setObjectFlags  (currentCut,  currentCutIndex,  tracks,       mcparticles,  "track-mcparticle pairs");
          //////////////////////////////////////////////////////////////////////
        }
    }

  //////////////////////////////////////////////////////////////////////////////
  // Quit if there was a problem setting the flags for any of the objects.
  //////////////////////////////////////////////////////////////////////////////
  if (!pl_->isValid)
    {
      clog << "ERROR: failed to set flags. Quitting..." <<  endl;
      exit (1);
    }
  //////////////////////////////////////////////////////////////////////////////

  // Decide whether the event passes the triggers specified by the user and
  // store the decision in the payload.
  evaluateTriggers ();
  setEventFlags ();

  event.put (pl_, "cutDecisions");
  firstEvent_ = false;
}

template <class InputCollection> bool
CutCalculator::setObjectFlags (const cut &currentCut, unsigned currentCutIndex, edm::Handle<InputCollection> inputCollection, const string &inputType)
{
  //////////////////////////////////////////////////////////////////////////////
  // Return false and print an error if the input collection is invalid.
  //////////////////////////////////////////////////////////////////////////////
  if (!inputCollection.isValid ())
    {
      firstEvent_ && clog << "ERROR: failed to set flags for invalid " << inputType << " collection." <<  endl;
      return false;
    }
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // If the cut is on a pair which includes the object collection whose flags
  // are being set, return. The flags for this object collection will be set
  // instead when setObjectFlags() is called for the pair.
  //////////////////////////////////////////////////////////////////////////////
  if (currentCut.inputCollection.find ("pair") != string::npos)
    {
      string obj1, obj2;
      getTwoObjs (currentCut.inputCollection, obj1, obj2);
      if (inputType == obj1 || inputType == obj2)
        return true;
    }
  //////////////////////////////////////////////////////////////////////////////

  // Set the flag for each object in the collection.
  for (unsigned object = 0; object != inputCollection->size (); object++)
    {
      // The flag is true unless the cut acts on this collection.
      bool cutDecision = true;

      if (currentCut.inputCollection == inputType)
        {
          //////////////////////////////////////////////////////////////////////
          // Calculate the flag for each subcut.
          //////////////////////////////////////////////////////////////////////
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
          //////////////////////////////////////////////////////////////////////

          //////////////////////////////////////////////////////////////////////
          // Combine the subcut flags using the logical operators given by the
          // user.
          //////////////////////////////////////////////////////////////////////
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
          //////////////////////////////////////////////////////////////////////

          //////////////////////////////////////////////////////////////////////
          // Invert the flag if the cut is a veto.
          //////////////////////////////////////////////////////////////////////
          if (currentCut.isVeto)
            cutDecision = !cutDecision;
          //////////////////////////////////////////////////////////////////////
        }

      pl_->objectFlags.at (inputType).at (currentCutIndex).push_back (cutDecision);
    }

  return true;
}

template <class InputCollection1, class InputCollection2> bool
CutCalculator::setObjectFlags (const cut &currentCut, unsigned currentCutIndex, edm::Handle<InputCollection1> inputCollection1, edm::Handle<InputCollection2> inputCollection2, const string &inputType)
{
  //////////////////////////////////////////////////////////////////////////////
  // Return false and print an error if either input collection is invalid.
  //////////////////////////////////////////////////////////////////////////////
  if (!inputCollection1.isValid () || !inputCollection2.isValid ())
    {
      firstEvent_ && clog << "ERROR: failed to set flags for invalid " << inputType << " collection." <<  endl;
      return false;
    }
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // Decide if the two objects in the pair are of different types.
  //////////////////////////////////////////////////////////////////////////////
  string obj1Type, obj2Type;
  getTwoObjs (inputType, obj1Type, obj2Type);
  bool isTwoTypesOfObject = true;
  if (obj1Type == obj2Type)
    isTwoTypesOfObject = false;
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // Initialize all the flags for the individual collections with false.
  //////////////////////////////////////////////////////////////////////////////
  if (currentCut.inputCollection == inputType)
    {
      for (unsigned object1 = 0; object1 != inputCollection1->size (); object1++)
        pl_->objectFlags.at (obj1Type).at (currentCutIndex).push_back (false);
      if (isTwoTypesOfObject)
        {
          for (unsigned object2 = 0; object2 != inputCollection2->size (); object2++)
            pl_->objectFlags.at (obj2Type).at (currentCutIndex).push_back (false);
        }
    }
  //////////////////////////////////////////////////////////////////////////////

  // Set the flag for each pair of objects.
  for (unsigned object1 = 0; object1 != inputCollection1->size (); object1++)
    {
      for (unsigned object2 = 0; object2 != inputCollection2->size (); object2++)
        {
          //////////////////////////////////////////////////////////////////////
          // Avoid double counting pairs when the two collections are the same
          // type.
          //////////////////////////////////////////////////////////////////////
          if (!isTwoTypesOfObject && object1 >= object2)
            continue;
          //////////////////////////////////////////////////////////////////////

          // The flag is true unless the cut acts on this pair.
          bool cutDecision = true;

          if (currentCut.inputCollection == inputType)
            {
              //////////////////////////////////////////////////////////////////
              // Calculate the flag for each subcut.
              //////////////////////////////////////////////////////////////////
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
              //////////////////////////////////////////////////////////////////

              //////////////////////////////////////////////////////////////////
              // Combine the subcut flags using the logical operators given by
              // the user.
              //////////////////////////////////////////////////////////////////
              if (currentCut.numSubcuts == 1)
                cutDecision = subcutDecisions.at (0);
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
              //////////////////////////////////////////////////////////////////

              //////////////////////////////////////////////////////////////////
              // Invert the flag if the cut is a veto.
              //////////////////////////////////////////////////////////////////
              if (currentCut.isVeto)
                cutDecision = !cutDecision;
              //////////////////////////////////////////////////////////////////

              // Set the flags for the individual objects.
              pl_->objectFlags.at (obj1Type).at (currentCutIndex).at (object1) = pl_->objectFlags.at (obj2Type).at (currentCutIndex).at (object2) = cutDecision;
            }

          pl_->objectFlags.at (inputType).at (currentCutIndex).push_back (cutDecision);
        }
    }

  return true;
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

  //////////////////////////////////////////////////////////////////////////////
  // Retrieve the cuts and clear the vector in which they will be stored after
  // parsing.
  //////////////////////////////////////////////////////////////////////////////
  edm::VParameterSet cuts = cuts_.getParameter<edm::VParameterSet> ("cuts");
  unpackedCuts_.clear ();
  //////////////////////////////////////////////////////////////////////////////

  // Loop over the cuts, parsing each one and storing it in a vector.
  for (unsigned currentCut = 0; currentCut != cuts.size (); currentCut++)
    {
      cut tempCut;
      string tempInputCollection = cuts.at (currentCut).getParameter<string> ("inputCollection");

      //////////////////////////////////////////////////////////////////////////
      // Swap the names of the objects in pairs to match what is used in the
      // rest of the code.
      //////////////////////////////////////////////////////////////////////////
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
      //////////////////////////////////////////////////////////////////////////

      //////////////////////////////////////////////////////////////////////////
      // Store the name(s) of the collection(s) to get from the event and for
      // which to set flags.
      //////////////////////////////////////////////////////////////////////////
      tempCut.inputCollection = tempInputCollection;
      if (tempInputCollection.find ("pairs") == string::npos)
        {
          if (tempInputCollection.find ("secondary muons") != string::npos)
            objectsToGet_.push_back ("secondary muons");
          else
            objectsToGet_.push_back (getObjToGet (tempInputCollection));
          objectsToFlag_.push_back (tempInputCollection);
        }
      else
        {
          string obj1;
          string obj2;
          getTwoObjs (tempInputCollection, obj1, obj2);
          string obj2ToGet = getObjToGet (obj2);
          objectsToFlag_.push_back (tempInputCollection);
          objectsToFlag_.push_back (obj1);
          objectsToFlag_.push_back (obj2);
          objectsToGet_.push_back (tempInputCollection);
          objectsToGet_.push_back (obj1);
          objectsToGet_.push_back (obj2ToGet);
        }
      //////////////////////////////////////////////////////////////////////////

      //////////////////////////////////////////////////////////////////////////
      // Separate the cut into subcuts, each of which is separated by a logical
      // operator.
      //////////////////////////////////////////////////////////////////////////
      string cutString = cuts.at (currentCut).getParameter<string> ("cutString");
      vector<string> cutStringVector = splitString (cutString);
      if (cutStringVector.size () != 3 && cutStringVector.size () % 4 != 3)
        {
          clog << "ERROR: malformed cut: \"" << cutString << "\"." << endl;
          return false;
        }
      tempCut.numSubcuts = (cutStringVector.size () + 1) / 4;
      for (int subcutIndex = 0; subcutIndex != tempCut.numSubcuts; subcutIndex++)
        {
          int indexOffset = 4 * subcutIndex;
          string currentVariableString = cutStringVector.at (indexOffset);
          if (currentVariableString.find ("(") == string::npos)
            {
              tempCut.functions.push_back ("");
              tempCut.variables.push_back (currentVariableString);
            }
          else
            {
              tempCut.functions.push_back (currentVariableString.substr (0, currentVariableString.find ("(")));
              string tempVariable = currentVariableString.substr (currentVariableString.find ("(") + 1);
              tempCut.variables.push_back (tempVariable.substr (0, tempVariable.size () - 1));
            }
          tempCut.comparativeOperators.push_back (cutStringVector.at (indexOffset + 1));
          tempCut.cutValues.push_back (atof (cutStringVector.at (indexOffset + 2).c_str ()));
          tempCut.cutStringValues.push_back (cutStringVector.at (indexOffset + 2));
          if (subcutIndex != 0)
            tempCut.logicalOperators.push_back (cutStringVector.at (indexOffset - 1));
        }
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
          bool plural = numberRequiredInt != 1;
          string collectionString = plural ? tempInputCollection : tempInputCollection.substr (0, tempInputCollection.size () - 1);
          string cutName =  numberRequiredString + " " + collectionString + " with " + cutString;
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
  sort (objectsToFlag_.begin (), objectsToFlag_.end ());
  objectsToFlag_.erase (unique (objectsToFlag_.begin (), objectsToFlag_.end ()), objectsToFlag_.end ());

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

  if (triggers.isValid ())
    {
      for (BNtriggerCollection::const_iterator trigger = triggers->begin (); trigger != triggers->end (); trigger++)
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
      cut currentCut = pl_->cuts.at (currentCutIndex);
      int numberPassing = 0;
      int numberPassingPrev = 0;

      //////////////////////////////////////////////////////////////////////////
      // Count the number of objects passing the current cut in the collection
      // on which the cut acts.
      //////////////////////////////////////////////////////////////////////////
      for (unsigned object = 0; object != pl_->objectFlags.at (currentCut.inputCollection).at (currentCutIndex).size (); object++)
        if (pl_->objectFlags.at (currentCut.inputCollection).at (currentCutIndex).at (object))
          numberPassing++;
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
          for (unsigned object = 0; object < pl_->objectFlags.at (currentCut.inputCollection).at (currentCutIndex).size (); object++)
            {
              bool passesPrevCuts = true;
              for (int cut = currentCutIndex - 1; cut >= 0; cut--)
                passesPrevCuts = passesPrevCuts && pl_->objectFlags.at (currentCut.inputCollection).at (cut).at (object);
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
  vl_->setObjectsToCut              (objectsToCut_);
  vl_->setObjectsToGet              (objectsToGet_);
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

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(CutCalculator);
