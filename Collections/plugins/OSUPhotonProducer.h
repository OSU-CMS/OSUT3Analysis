#ifndef PHOTON_PRODUCER
#define PHOTON_PRODUCER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "OSUT3Analysis/Collections/interface/Photon.h"

#if CMSSW_VERSION_CODE >= CMSSW_VERSION(11,2,0)
#include "CommonTools/Egamma/interface/EffectiveAreas.h"
#else
#include "RecoEgamma/EgammaTools/interface/EffectiveAreas.h"
#endif

class OSUPhotonProducer : public edm::EDProducer
{
  public:
    OSUPhotonProducer (const edm::ParameterSet &);
    ~OSUPhotonProducer ();

    void produce (edm::Event &, const edm::EventSetup &);

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::ParameterSet  collections_;
    edm::InputTag      collection_;
    edm::EDGetTokenT<edm::View<TYPE(photons)> > token_;
    edm::EDGetTokenT<vector<osu::Mcparticle> > mcparticleToken_;
    edm::EDGetTokenT<double> rhoToken_;
    edm::EDGetTokenT<edm::ValueMap<bool> > vidLooseIdMapToken_;
    edm::EDGetTokenT<edm::ValueMap<bool> > vidMediumIdMapToken_;
    edm::EDGetTokenT<edm::ValueMap<bool> > vidTightIdMapToken_;
    edm::ParameterSet  cfg_;
    edm::InputTag      rho_;
    edm::InputTag      vidLooseIdMap_;
    edm::InputTag      vidMediumIdMap_;
    edm::InputTag      vidTightIdMap_;
    EffectiveAreas     effectiveAreasChargedHadron_;
    EffectiveAreas     effectiveAreasNeutralHadron_;
    EffectiveAreas     effectiveAreasPhoton_;
    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDFilter.
    unique_ptr<vector<osu::Photon> > pl_;
};

#endif
