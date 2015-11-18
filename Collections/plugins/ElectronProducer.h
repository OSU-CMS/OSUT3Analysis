#ifndef ELECTRON_PRODUCER
#define ELECTRON_PRODUCER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"  
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "RecoEgamma/EgammaTools/interface/EffectiveAreas.h"
#include "OSUT3Analysis/Collections/interface/Electron.h"

class ElectronProducer : public edm::EDProducer
{
  public:
    ElectronProducer (const edm::ParameterSet &);
    ~ElectronProducer ();

    void produce (edm::Event &, const edm::EventSetup &);

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::ParameterSet  collections_;
    edm::InputTag      collection_;
    edm::ParameterSet  cfg_;
    edm::EDGetToken    gsfeElectronsToken_;
    edm::InputTag      beamSpot_;
    edm::InputTag      conversions_;
    edm::InputTag      rho_;
    edm::InputTag      gsfElectrons_;
    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDFilter.
    auto_ptr<vector<osu::Electron> > pl_;
};

#endif
