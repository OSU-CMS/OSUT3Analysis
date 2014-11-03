#ifndef OBJECT_SELECTOR
#define OBJECT_SELECTOR

#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/AnaTools/plugins/AnalysisPayloads.h"

template<class T>
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
    edm::Handle<CutCalculatorPayload>  cutDecisions;
    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDFilter.
    auto_ptr<vector<T> > pl_;
};

#endif
