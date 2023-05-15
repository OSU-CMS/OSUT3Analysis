#ifndef TAU_PRODUCER
#define TAU_PRODUCER

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/Collections/interface/Tau.h"

class OSUTauProducer : public edm::stream::EDProducer<>
{
  public:
    OSUTauProducer (const edm::ParameterSet &);
    ~OSUTauProducer ();

    void produce (edm::Event &, const edm::EventSetup &);

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::ParameterSet  collections_;
    edm::InputTag      collection_;
    edm::EDGetTokenT<vector<TYPE(taus)> > token_;
    edm::EDGetTokenT<vector<osu::Mcparticle> > mcparticleToken_;
    edm::EDGetTokenT<vector<osu::Met> > metToken_;
    edm::EDGetTokenT<edm::TriggerResults> triggersToken_;
#if IS_VALID(trigobjs)
    edm::EDGetTokenT<vector<pat::TriggerObjectStandAlone> > trigobjsToken_;
#endif
    edm::ParameterSet  cfg_;
    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDFilter.
    unique_ptr<vector<osu::Tau> > pl_;
};

#endif
