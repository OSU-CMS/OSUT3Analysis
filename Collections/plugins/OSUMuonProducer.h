#ifndef MUON_PRODUCER
#define MUON_PRODUCER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/Collections/interface/Muon.h"

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
    edm::EDGetTokenT<vector<pat::PackedCandidate> > pfCandidateToken_;
    edm::EDGetTokenT<vector<TYPE(primaryvertexs)> > primaryvertexToken_;
    edm::InputTag      pfCandidate_;
    edm::ParameterSet  cfg_;
    edm::InputTag      collPrimaryvertexs_;
    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDFilter.
    auto_ptr<vector<osu::Muon> > pl_;
};

#endif
