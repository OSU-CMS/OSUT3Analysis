#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"

// The object selector for beamspots is a special case because beamspots are
// not stored in a vector.

#if IS_VALID(beamspots)
  template<>
  ObjectSelector<osu::Beamspot, TYPE(beamspots)>::ObjectSelector (const edm::ParameterSet &cfg) :
    collections_         (cfg.getParameter<edm::ParameterSet>  ("collections")),
    collectionToFilter_  (cfg.getParameter<string>             ("collectionToFilter")),
    originalCollection_  (cfg.getParameter<edm::InputTag>      ("originalCollection")),
    cutDecisions_        (cfg.getParameter<edm::InputTag>      ("cutDecisions")),
    firstEvent_          (true)
  {
    // Retrieve the InputTag for the collection which is to be filtered.
    collection_ = collections_.getParameter<edm::InputTag> (collectionToFilter_);

    produces<osu::Beamspot> (collection_.instance ());
    produces<TYPE(beamspots)> (ORIGINAL_FORMAT);

    singletonToken_ = consumes<osu::Beamspot> (collection_);
    singletonOrigToken_ = consumes<TYPE(beamspots)> (originalCollection_);
    cutDecisionsToken_ = consumes<CutCalculatorPayload> (cutDecisions_);
  }

  template<> bool
  ObjectSelector<osu::Beamspot, TYPE(beamspots)>::filter (edm::Event &event, const edm::EventSetup &setup)
  {
    //////////////////////////////////////////////////////////////////////////////
    // Get the collection and cut decisions from the event and print a warning if
    // there is a problem.
    //////////////////////////////////////////////////////////////////////////////
    event.getByToken (singletonToken_, singleton);
    event.getByToken (singletonOrigToken_, singletonOrig);
    event.getByToken (cutDecisionsToken_, cutDecisions);
    if (firstEvent_ && !singleton.isValid ())
      clog << "WARNING: failed to retrieve requested collection from the event." << endl;
    if (firstEvent_ && !singletonOrig.isValid ())
      clog << "WARNING: failed to retrieve original collection from the event." << endl;
    if (firstEvent_ && !cutDecisions.isValid ())
      clog << "WARNING: failed to retrieve cut decisions from the event." << endl;
    //////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////
    // Fill the payload with the objects from the collection which pass all cuts.
    // If the collection could not be retrieved, the payload remains empty. If the
    // cut decisions could not be retrieved, no objects are cut.
    //////////////////////////////////////////////////////////////////////////////
    unique_ptr<osu::Beamspot> pl_ = unique_ptr<osu::Beamspot> (new osu::Beamspot ());
    unique_ptr<TYPE(beamspots)> plO_ = unique_ptr<TYPE(beamspots)> (new TYPE(beamspots) ());
    if (singleton.isValid () && singletonOrig.isValid())
      {
        const osu::Beamspot * const object = &(*singleton);
        const TYPE(beamspots) * const objOrig = &(*singletonOrig);
        unsigned iObject = 0;
        bool passes = true;

        if (cutDecisions.isValid ())
          {
            for (int iCut = cutDecisions->cumulativeObjectFlags.size () - 1; iCut >= 0; iCut--)
              {
                passes = (cutDecisions->cumulativeObjectFlags.at (iCut).at (collectionToFilter_).at (iObject).second ? cutDecisions->cumulativeObjectFlags.at (iCut).at (collectionToFilter_).at (iObject).first : false);
                break;
              }
          }
        if (passes)
          {
            *pl_ = *object;
            *plO_ = *objOrig;
          }
      }
    //////////////////////////////////////////////////////////////////////////////

    event.put (std::move (pl_),  collection_.instance ());
    event.put (std::move (plO_), ORIGINAL_FORMAT);
    pl_.reset ();
    plO_.reset ();
    firstEvent_ = false;

    // Return the global decision for the event. If the cut decisions could not
    // be retrieved, the event passes.
    return (cutDecisions.isValid () ? cutDecisions->eventDecision : true);
  }
#endif
