#ifndef CANDELE_PRODUCER
#define CANDELE_PRODUCER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/Collections/interface/Candele.h"

class CandeleProducer : public edm::EDProducer
{
  public:
    CandeleProducer (const edm::ParameterSet &);
    ~CandeleProducer ();

    void produce (edm::Event &, const edm::EventSetup &);

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::ParameterSet  collections_;
    edm::InputTag      collection_;
    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDFilter.
    auto_ptr<vector<TYPE(candeles)> > pl_;
};

#endif
