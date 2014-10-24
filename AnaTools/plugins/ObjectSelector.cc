#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/plugins/ObjectSelector.h"

template<class T>
ObjectSelector<T>::ObjectSelector (const edm::ParameterSet &cfg) :
  collections_         (cfg.getParameter<edm::ParameterSet>  ("collections")),
  collectionToFilter_  (cfg.getParameter<string>             ("collectionToFilter")),
  cutDecisions_        (cfg.getParameter<edm::InputTag>      ("cutDecisions")),
  firstEvent_          (true)
{
  // Retrieve the InputTag for the collection which is to be filtered.
  collection_ = collections_.getParameter<edm::InputTag> (collectionToFilter_);

  produces<vector<T> > ("selectedObjects");
}

template<class T>
ObjectSelector<T>::~ObjectSelector ()
{
}

template<class T> bool
ObjectSelector<T>::filter (edm::Event &event, const edm::EventSetup &setup)
{
  //////////////////////////////////////////////////////////////////////////////
  // Get the collection and cut decisions from the event and print a warning if
  // there is a problem.
  //////////////////////////////////////////////////////////////////////////////
  event.getByLabel  (collection_, collection);
  event.getByLabel  (cutDecisions_, cutDecisions);
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
  pl_ = auto_ptr<vector<T> > (new vector<T> ());
  if (collection.isValid ())
    {
      for (typename vector<T>::const_iterator object = collection->begin (); object != collection->end (); object++)
        {
          bool passes = true;
          unsigned iObject = object - collection->begin ();

          if (cutDecisions.isValid ())
            {
              for (vector<vector<bool> >::const_iterator cut = cutDecisions->objectFlags.at (collectionToFilter_).begin (); cut != cutDecisions->objectFlags.at (collectionToFilter_).end (); cut++)
                passes = passes && cut->at (iObject);
            }
          if (passes)
            pl_->push_back (*object);
        }
    }
  //////////////////////////////////////////////////////////////////////////////

  event.put (pl_, "selectedObjects");
  firstEvent_ = false;

  // Return the global decision for the event. If the cut decisions could not
  // be retrieved, the event passes.
  return (cutDecisions.isValid () ? cutDecisions->eventDecision : true);
}

typedef  ObjectSelector<BNbxlumi>         BxlumiObjectSelector;
typedef  ObjectSelector<BNelectron>       ElectronObjectSelector;
typedef  ObjectSelector<BNevent>          EventObjectSelector;
typedef  ObjectSelector<BNgenjet>         GenjetObjectSelector;
typedef  ObjectSelector<BNjet>            JetObjectSelector;
typedef  ObjectSelector<BNmcparticle>     McparticleObjectSelector;
typedef  ObjectSelector<BNmet>            MetObjectSelector;
typedef  ObjectSelector<BNmuon>           MuonObjectSelector;
typedef  ObjectSelector<BNphoton>         PhotonObjectSelector;
typedef  ObjectSelector<BNprimaryvertex>  PrimaryvertexObjectSelector;
typedef  ObjectSelector<BNsupercluster>   SuperclusterObjectSelector;
typedef  ObjectSelector<BNtau>            TauObjectSelector;
typedef  ObjectSelector<BNtrack>          TrackObjectSelector;
typedef  ObjectSelector<BNtrigger>        TriggerObjectSelector;
typedef  ObjectSelector<BNtrigobj>        TrigobjObjectSelector;
typedef  ObjectSelector<map<string, double> > UserVarObjectSelector;

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(BxlumiObjectSelector);
DEFINE_FWK_MODULE(ElectronObjectSelector);
DEFINE_FWK_MODULE(EventObjectSelector);
DEFINE_FWK_MODULE(GenjetObjectSelector);
DEFINE_FWK_MODULE(JetObjectSelector);
DEFINE_FWK_MODULE(McparticleObjectSelector);
DEFINE_FWK_MODULE(MetObjectSelector);
DEFINE_FWK_MODULE(MuonObjectSelector);
DEFINE_FWK_MODULE(PhotonObjectSelector);
DEFINE_FWK_MODULE(PrimaryvertexObjectSelector);
DEFINE_FWK_MODULE(SuperclusterObjectSelector);
DEFINE_FWK_MODULE(TauObjectSelector);
DEFINE_FWK_MODULE(TrackObjectSelector);
DEFINE_FWK_MODULE(TriggerObjectSelector);
DEFINE_FWK_MODULE(TrigobjObjectSelector);
DEFINE_FWK_MODULE(UserVarObjectSelector);
