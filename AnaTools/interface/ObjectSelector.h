#ifndef OBJECT_SELECTOR
#define OBJECT_SELECTOR

#include <iostream>

#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

#define EXIT_CODE 2

template<class T, class TO>
class ObjectSelector : public edm::EDFilter
{
  public:
    ObjectSelector (const edm::ParameterSet &);
    ~ObjectSelector ();

    bool filter (edm::Event &, const edm::EventSetup &);

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::ParameterSet  collections_;
    string             collectionToFilter_;
    edm::InputTag      cutDecisions_;
    bool               firstEvent_;
    ////////////////////////////////////////////////////////////////////////////

    // InputTag for the collection which is to be filtered.
    edm::InputTag            collection_;

    ////////////////////////////////////////////////////////////////////////////
    // Objects which can be gotten from the event.
    ////////////////////////////////////////////////////////////////////////////
    edm::Handle<vector<T> >            collection;
    edm::Handle<vector<TO> >           collectionOrig;  // original format collection
    edm::Handle<CutCalculatorPayload>  cutDecisions;
    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDFilter.
    auto_ptr<vector<T> >  pl_;  
    auto_ptr<vector<TO> > plO_; // original format  
};

template<class T, class TO>
  ObjectSelector<T, TO>::ObjectSelector (const edm::ParameterSet &cfg) :
  collections_         (cfg.getParameter<edm::ParameterSet>  ("collections")),
  collectionToFilter_  (cfg.getParameter<string>             ("collectionToFilter")),
  cutDecisions_        (cfg.getParameter<edm::InputTag>      ("cutDecisions")),
  firstEvent_          (true)
{
  assert (strcmp (PROJECT_VERSION, SUPPORTED_VERSION) == 0);

  // Retrieve the InputTag for the collection which is to be filtered.
  collection_ = collections_.getParameter<edm::InputTag> (collectionToFilter_);

  produces<vector<T> >  (collection_.instance ());
  produces<vector<TO> > (ORIGINAL_FORMAT);  
}

template<class T, class TO>
    ObjectSelector<T, TO>::~ObjectSelector ()
{
}

template<class T, class TO> bool
  ObjectSelector<T, TO>::filter (edm::Event &event, const edm::EventSetup &setup)
{
  //////////////////////////////////////////////////////////////////////////////
  // Get the collection and cut decisions from the event and print a warning if
  // there is a problem.
  //////////////////////////////////////////////////////////////////////////////
  anatools::getCollection (collection_, collection,     event);
  anatools::getCollection (collection_, collectionOrig, event);
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
  pl_  = auto_ptr<vector<T> >  (new vector<T>  ());
  plO_ = auto_ptr<vector<TO> > (new vector<TO> ());
  if (collection.isValid () && collectionOrig.isValid())
    {
      auto objOrig = collectionOrig->begin();  
      for (auto object = collection->begin (); object != collection->end (); object++)
        {
          unsigned iObject = object - collection->begin ();
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
            pl_ ->push_back (*object);
	  if (objOrig == collectionOrig->end()) {  
	    clog << "ERROR:  Invalid original object found for iObject = " << iObject 
		 << " in collection:  " << collectionToFilter_ << endl;  
	  } else {
            plO_->push_back (*objOrig);
	  }  
        }
      objOrig++;  
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
