#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"

// The object selector for beamspots is a special case because beamspots are
// not stored in a vector.

#if IS_VALID(beamspots)
  template<>
  ObjectSelector<osu::Beamspot, TYPE(beamspots)>::ObjectSelector (const edm::ParameterSet &cfg) :
    collections_         (cfg.getParameter<edm::ParameterSet>  ("collections")),
    collectionToFilter_  (cfg.getParameter<string>             ("collectionToFilter")),
    cutDecisions_        (cfg.getParameter<edm::InputTag>      ("cutDecisions")),
    firstEvent_          (true)
  {
    assert (strcmp (PROJECT_VERSION, SUPPORTED_VERSION) == 0);

    // Retrieve the InputTag for the collection which is to be filtered.
    collection_ = collections_.getParameter<edm::InputTag> (collectionToFilter_);

    produces<osu::Beamspot> (collection_.instance ());
    produces<TYPE(beamspots)> (ORIGINAL_FORMAT);
  }

  template<> bool
  ObjectSelector<osu::Beamspot, TYPE(beamspots)>::filter (edm::Event &event, const edm::EventSetup &setup)
  {
    //////////////////////////////////////////////////////////////////////////////
    // Get the collection and cut decisions from the event and print a warning if
    // there is a problem.
    //////////////////////////////////////////////////////////////////////////////
    edm::Handle<osu::Beamspot> collection;
    edm::Handle<TYPE(beamspots)> collectionOrig;
    anatools::getCollection (collection_, collection,     event);
    anatools::getCollection (collection_, collectionOrig, event);
    event.getByLabel (cutDecisions_, cutDecisions);  
    if (firstEvent_ && !collection.isValid ())
      clog << "WARNING: failed to retrieve requested collection from the event." << endl;
    if (firstEvent_ && !collectionOrig.isValid ())
      clog << "WARNING: failed to retrieve original collection from the event." << endl;
    if (firstEvent_ && !cutDecisions.isValid ())
      clog << "WARNING: failed to retrieve cut decisions from the event." << endl;
    //////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////
    // Fill the payload with the objects from the collection which pass all cuts.
    // If the collection could not be retrieved, the payload remains empty. If the
    // cut decisions could not be retrieved, no objects are cut.
    //////////////////////////////////////////////////////////////////////////////
    auto_ptr<osu::Beamspot> pl_ = auto_ptr<osu::Beamspot> (new osu::Beamspot ());
    auto_ptr<TYPE(beamspots)> plO_ = auto_ptr<TYPE(beamspots)> (new TYPE(beamspots) ());
    if (collection.isValid () && collectionOrig.isValid())
      {
        const osu::Beamspot * const object = &(*collection);
        const TYPE(beamspots) * const objOrig = &(*collectionOrig);
        unsigned iObject = 0;
        bool passes = true;

        if (cutDecisions.isValid ())
          {
            for (int iCut = cutDecisions->cumulativeObjectFlags.size () - 1; iCut >= 0; iCut--)
              {
                if (cutDecisions->cumulativeObjectFlags.at (iCut).at (collectionToFilter_).at (iObject).second)
                  {
                    passes = cutDecisions->cumulativeObjectFlags.at (iCut).at (collectionToFilter_).at (iObject).first;
                    break;
                  }
              }
          }
        if (passes)
          {
            *pl_ = *object;
            *plO_ = *objOrig;
          }
      }
    //////////////////////////////////////////////////////////////////////////////

    event.put (pl_,  collection_.instance ());
    event.put (plO_, ORIGINAL_FORMAT);  
    pl_.reset ();
    plO_.reset ();
    firstEvent_ = false;

    // Return the global decision for the event. If the cut decisions could not
    // be retrieved, the event passes.
    return (cutDecisions.isValid () ? cutDecisions->eventDecision : true);
  }
#endif
