#ifndef MCPARTICLE_PRODUCER
#define MCPARTICLE_PRODUCER

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/Collections/interface/Mcparticle.h"

class McparticleProducer : public edm::stream::EDProducer<>
{
  public:
    McparticleProducer (const edm::ParameterSet &);
    ~McparticleProducer ();

    void produce (edm::Event &, const edm::EventSetup &);

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::ParameterSet  collections_;
    edm::InputTag      collection_;
    edm::EDGetTokenT<vector<TYPE(mcparticles)> > token_;
    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDFilter.
    unique_ptr<vector<osu::Mcparticle> > pl_;

    const reco::Candidate * uniqueMother (const TYPE(mcparticles) &) const;
};

#endif
