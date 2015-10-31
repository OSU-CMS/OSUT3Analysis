#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"

#if IS_VALID(beamspots)
  template<>
  ObjectSelector<osu::Beamspot>::ObjectSelector (const edm::ParameterSet &cfg) :
    collections_         (cfg.getParameter<edm::ParameterSet>  ("collections")),
    collectionToFilter_  (cfg.getParameter<string>             ("collectionToFilter")),
    cutDecisions_        (cfg.getParameter<edm::InputTag>      ("cutDecisions")),
    firstEvent_          (true)
  {
    assert (strcmp (PROJECT_VERSION, SUPPORTED_VERSION) == 0);

    // Retrieve the InputTag for the collection which is to be filtered.
    collection_ = collections_.getParameter<edm::InputTag> (collectionToFilter_);

    produces<osu::Beamspot> (collection_.instance ());
  }

  template<> bool
  ObjectSelector<osu::Beamspot>::filter (edm::Event &event, const edm::EventSetup &setup)
  {
    //////////////////////////////////////////////////////////////////////////////
    // Get the collection and cut decisions from the event and print a warning if
    // there is a problem.
    //////////////////////////////////////////////////////////////////////////////
    edm::Handle<osu::Beamspot> collection;
    anatools::getCollection (collection_, collection, event);
    event.getByLabel (cutDecisions_, cutDecisions);
    if (firstEvent_ && !collection.isValid ())
      clog << "WARNING: failed to retrieve requested collection from the event." << endl;
    if (firstEvent_ && !cutDecisions.isValid ())
      clog << "WARNING: failed to retrieve cut decisions from the event." << endl;
    //////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////
    // Fill the payload with the objects from the collection which pass all cuts.
    // If the collection could not be retrieved, the payload remains empty. If the
    // cut decisions could not be retrieved, no objects are cut.
    //////////////////////////////////////////////////////////////////////////////
    auto_ptr<osu::Beamspot> pl_ = auto_ptr<osu::Beamspot> (new osu::Beamspot ());
    if (collection.isValid ())
      {
        const osu::Beamspot * const object = &(*collection);
        unsigned iObject = 0,
                 nCuts;
        bool passes = true;

        if (cutDecisions.isValid ())
          {
            nCuts = cutDecisions->cumulativeObjectFlags.size ();
            if (nCuts > 0)
              cutDecisions->cumulativeObjectFlags.at (nCuts - 1).at (collectionToFilter_).at (iObject).second && (passes = cutDecisions->cumulativeObjectFlags.at (nCuts - 1).at (collectionToFilter_).at (iObject).first);
          }
        if (passes)
          *pl_ = *object;
      }
    //////////////////////////////////////////////////////////////////////////////

    event.put (pl_, collection_.instance ());
    firstEvent_ = false;

    // Return the global decision for the event. If the cut decisions could not
    // be retrieved, the event passes.
    return (cutDecisions.isValid () ? cutDecisions->eventDecision : true);
  }
#endif
