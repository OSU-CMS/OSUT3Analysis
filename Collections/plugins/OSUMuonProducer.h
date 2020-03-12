#ifndef MUON_PRODUCER
#define MUON_PRODUCER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "OSUT3Analysis/Collections/interface/Muon.h"
#include "TRandom3.h"


class OSUMuonProducer : public edm::EDProducer
{
  public:
    OSUMuonProducer (const edm::ParameterSet &);
    ~OSUMuonProducer ();

    void produce (edm::Event &, const edm::EventSetup &);

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::ParameterSet  collections_;
    edm::InputTag      collection_;
    edm::EDGetTokenT<vector<TYPE(muons)> > token_;
    edm::EDGetTokenT<vector<osu::Mcparticle> > mcparticleToken_;
    edm::EDGetTokenT<TYPE(beamspots)> beamspotToken_;
    edm::EDGetTokenT<vector<reco::GenParticle> > prunedParticleToken_;
    edm::EDGetTokenT<vector<pat::PackedCandidate> > pfCandidateToken_;
    edm::EDGetTokenT<vector<TYPE(primaryvertexs)> > primaryvertexToken_;
    edm::EDGetTokenT<vector<osu::Met> > metToken_;
    edm::EDGetTokenT<double> rhoToken_;
    edm::EDGetTokenT<edm::TriggerResults> triggersToken_;
    edm::EDGetTokenT<vector<pat::TriggerObjectStandAlone> > trigobjsToken_;

    edm::ParameterSet  cfg_;
    edm::InputTag      pfCandidate_;
    edm::InputTag      rho_;
    double             d0SmearingWidth_;
    vector<edm::ParameterSet> hltMatchingInfo_;

    TRandom3* rng;

    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDFilter.
    unique_ptr<vector<osu::Muon> > pl_;
};

#endif
