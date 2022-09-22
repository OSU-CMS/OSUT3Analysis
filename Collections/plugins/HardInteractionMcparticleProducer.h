#ifndef HARDINTERACTIONMCPARTICLE_PRODUCER
#define HARDINTERACTIONMCPARTICLE_PRODUCER

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/Collections/interface/HardInteractionMcparticle.h"

#include <unordered_set>

class HardInteractionMcparticleProducer : public edm::stream::EDProducer<>
{
  public:
    HardInteractionMcparticleProducer (const edm::ParameterSet &);
    ~HardInteractionMcparticleProducer ();

    void produce (edm::Event &, const edm::EventSetup &);

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::ParameterSet  collections_;
    edm::InputTag      collection_;
    edm::EDGetTokenT<vector<TYPE(hardInteractionMcparticles)> > token_;
    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDFilter.
    unique_ptr<vector<osu::HardInteractionMcparticle> > pl_;

    const reco::Candidate * uniqueMother (const TYPE(hardInteractionMcparticles) &) const;
};

#endif
