#ifndef GENJET_PRODUCER
#define GENJET_PRODUCER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/Collections/interface/Genjet.h"

class OSUGenjetProducer : public edm::EDProducer
{
  public:
    OSUGenjetProducer (const edm::ParameterSet &);
    ~OSUGenjetProducer ();

    void produce (edm::Event &, const edm::EventSetup &);

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::ParameterSet  collections_;
    edm::InputTag      collection_;
    edm::ParameterSet  cfg_;
    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDFilter.
    auto_ptr<vector<osu::Genjet> > pl_;
};

#endif
