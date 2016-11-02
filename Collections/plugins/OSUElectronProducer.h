#ifndef ELECTRON_PRODUCER
#define ELECTRON_PRODUCER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "RecoEgamma/EgammaTools/interface/EffectiveAreas.h"
#include "OSUT3Analysis/Collections/interface/Electron.h"

class OSUElectronProducer : public edm::EDProducer
{
  public:
    OSUElectronProducer (const edm::ParameterSet &);
    ~OSUElectronProducer ();

    void produce (edm::Event &, const edm::EventSetup &);

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::ParameterSet  collections_;
    edm::InputTag      collection_;
    edm::EDGetTokenT<vector<TYPE(electrons)> > token_;
    edm::EDGetTokenT<vector<osu::Mcparticle> > mcparticleToken_;
    edm::EDGetTokenT<TYPE(beamspots)> beamSpotToken_;
    edm::EDGetTokenT<vector<reco::Conversion> > conversionsToken_;
    edm::EDGetTokenT<vector<pat::PackedCandidate> > pfCandidateToken_;
    edm::EDGetTokenT<double> rhoToken_;
    edm::EDGetTokenT<vector<TYPE(primaryvertexs)> > verticesToken_;
    edm::EDGetTokenT<vector<osu::Met> > metToken_;
    edm::ParameterSet  cfg_;
    edm::InputTag      beamSpot_;
    edm::InputTag      conversions_;
    edm::InputTag      pfCandidate_;
    edm::InputTag      rho_;
    edm::InputTag      vertices_;
    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDFilter.
    unique_ptr<vector<osu::Electron> > pl_;
};

#endif
