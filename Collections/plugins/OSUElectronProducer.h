#ifndef ELECTRON_PRODUCER
#define ELECTRON_PRODUCER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "OSUT3Analysis/Collections/interface/Electron.h"
#include "RecoEgamma/EgammaTools/interface/EffectiveAreas.h"
#include "TRandom3.h"

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
    edm::EDGetTokenT<edm::View<TYPE(electrons)> > token_;
    edm::EDGetTokenT<vector<osu::Mcparticle> > mcparticleToken_;
    edm::EDGetTokenT<TYPE(beamspots)> beamspotToken_;
    edm::EDGetTokenT<vector<reco::GenParticle> > prunedParticleToken_;
    edm::EDGetTokenT<vector<pat::PackedCandidate> > pfCandidateToken_;
    edm::EDGetTokenT<vector<TYPE(primaryvertexs)> > primaryvertexToken_;
    edm::EDGetTokenT<vector<osu::Met> > metToken_;
    edm::EDGetTokenT<vector<reco::Conversion> > conversionsToken_;
    edm::EDGetTokenT<double> rhoToken_;
    edm::EDGetTokenT<edm::TriggerResults> triggersToken_;
    edm::EDGetTokenT<vector<pat::TriggerObjectStandAlone> > trigobjsToken_;

    edm::EDGetTokenT<edm::ValueMap<bool> > vidVetoIdMapToken_;
    edm::EDGetTokenT<edm::ValueMap<bool> > vidLooseIdMapToken_;
    edm::EDGetTokenT<edm::ValueMap<bool> > vidMediumIdMapToken_;
    edm::EDGetTokenT<edm::ValueMap<bool> > vidTightIdMapToken_;

    edm::ParameterSet  cfg_;
    edm::InputTag      pfCandidate_;
    edm::InputTag      conversions_;
    edm::InputTag      rho_;
    edm::InputTag      vidVetoIdMap_;
    edm::InputTag      vidLooseIdMap_;
    edm::InputTag      vidMediumIdMap_;
    edm::InputTag      vidTightIdMap_;
    EffectiveAreas     effectiveAreas_;

    TRandom3* random;

    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDFilter.
    unique_ptr<vector<osu::Electron> > pl_;
};

#endif
