#ifndef JET_PRODUCER
#define JET_PRODUCER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/Collections/interface/Jet.h"

class OSUJetProducer : public edm::EDProducer
{
  public:
    OSUJetProducer (const edm::ParameterSet &);
    ~OSUJetProducer ();

    void produce (edm::Event &, const edm::EventSetup &);

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::ParameterSet  collections_;
    edm::InputTag      collection_;
    edm::InputTag      electrons_;
    edm::InputTag      muons_;
    edm::EDGetTokenT<vector<TYPE(jets)> > token_;
    edm::EDGetTokenT<vector<osu::Mcparticle> > mcparticleToken_;
    edm::EDGetTokenT<vector<TYPE(electrons)> > electronToken_;
    edm::EDGetTokenT<vector<TYPE(muons)> > muonToken_;
    edm::ParameterSet  cfg_;
    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDFilter.
    auto_ptr<vector<osu::Jet> > pl_;
};

#endif
