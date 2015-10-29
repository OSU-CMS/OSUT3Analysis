#ifndef PRIMARYVERTEX_PRODUCER
#define PRIMARYVERTEX_PRODUCER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/Collections/interface/Primaryvertex.h"

class PrimaryvertexProducer : public edm::EDProducer
{
  public:
    PrimaryvertexProducer (const edm::ParameterSet &);
    ~PrimaryvertexProducer ();

    void produce (edm::Event &, const edm::EventSetup &);

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::ParameterSet  collections_;
    edm::InputTag      collection_;
    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDFilter.
    auto_ptr<vector<TYPE(primaryvertexs)> > pl_;
};

#endif
