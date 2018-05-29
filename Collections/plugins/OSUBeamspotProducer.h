#ifndef BEAMSPOT_PRODUCER
#define BEAMSPOT_PRODUCER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/Collections/interface/Beamspot.h"

class OSUBeamspotProducer : public edm::EDProducer
{
  public:
    OSUBeamspotProducer (const edm::ParameterSet &);
    ~OSUBeamspotProducer ();

    void produce (edm::Event &, const edm::EventSetup &);

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::ParameterSet  collections_;
    edm::InputTag      collection_;
    edm::EDGetTokenT<TYPE(beamspots)> token_;
    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDFilter.
    unique_ptr<osu::Beamspot> pl_;
};

#endif
