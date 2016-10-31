#ifndef BJET_PRODUCER
#define BJET_PRODUCER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/Collections/interface/Bjet.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

class OSUBjetProducer : public edm::EDProducer
{
  public:
    OSUBjetProducer (const edm::ParameterSet &);
    ~OSUBjetProducer ();

    void produce (edm::Event &, const edm::EventSetup &);

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::ParameterSet  collections_;
    edm::InputTag      collection_;
    edm::InputTag      electrons_;
    edm::InputTag      muons_;
    edm::InputTag      genjets_;
    edm::InputTag      rho_;

    string jetResolutionPayload_;
    string jetResSFPayload_;
    bool jetResFromGlobalTag_;

    edm::EDGetTokenT<vector<TYPE(bjets)> > token_;
    edm::EDGetTokenT<vector<osu::Mcparticle> > mcparticleToken_;
    edm::EDGetTokenT<vector<TYPE(electrons)> > electronToken_;
    edm::EDGetTokenT<vector<TYPE(muons)> > muonToken_;
    edm::EDGetTokenT<vector<TYPE(genjets)> > genjetsToken_;
    edm::EDGetTokenT<double> rhoToken_;

    edm::ParameterSet  cfg_;
    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDFilter.
    auto_ptr<vector<TYPE (electrons)> > goodElectrons_;
    auto_ptr<vector<TYPE (muons)> > goodMuons_;
    auto_ptr<vector<osu::Bjet> > pl_;
};

#endif
