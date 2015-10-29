#ifndef EVENTVARIABLE_PRODUCER
#define EVENTVARIABLE_PRODUCER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/Collections/interface/Eventvariable.h"

class EventvariableProducer : public edm::EDProducer
{
  public:
    EventvariableProducer (const edm::ParameterSet &);
    ~EventvariableProducer ();

    void produce (edm::Event &, const edm::EventSetup &);

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::ParameterSet  collections_;
    edm::InputTag      collection_;
    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDFilter.
    auto_ptr<vector<TYPE(eventvariables)> > pl_;
};

#endif
