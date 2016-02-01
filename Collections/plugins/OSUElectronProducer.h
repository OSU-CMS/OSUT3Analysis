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
    edm::ParameterSet  cfg_;
    edm::InputTag      beamSpot_;
    edm::InputTag      rho_;
    edm::InputTag      vertices_;
    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDFilter.
    auto_ptr<vector<osu::Electron> > pl_;
};

#endif
