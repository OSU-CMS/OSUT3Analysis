#ifndef BASICJET_PRODUCER
#define BASICJET_PRODUCER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/Collections/interface/Basicjet.h"

class OSUBasicjetProducer : public edm::EDProducer
{
  public:
    OSUBasicjetProducer (const edm::ParameterSet &);
    ~OSUBasicjetProducer ();

    void produce (edm::Event &, const edm::EventSetup &);

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::ParameterSet  collections_;
    edm::InputTag      collection_;
    edm::EDGetTokenT<vector<TYPE(basicjets)> > token_;
    edm::EDGetTokenT<vector<osu::Mcparticle> > mcparticleToken_;
    edm::ParameterSet  cfg_;
    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDFilter.
    unique_ptr<vector<osu::Basicjet> > pl_;
};

#endif
