#ifndef JET_PRODUCER
#define JET_PRODUCER

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/Collections/interface/Jet.h"

#if CMSSW_VERSION_CODE >= CMSSW_VERSION(11,2,0)
#include "JetMETCorrections/JetCorrector/interface/JetCorrector.h"
#else
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#endif
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

#if DATA_FORMAT_FROM_MINIAOD
#include "JetMETCorrections/Modules/interface/JetResolution.h"
#include "CondFormats/DataRecord/interface/JetResolutionRcd.h"
#include "CondFormats/DataRecord/interface/JetResolutionScaleFactorRcd.h"
#include "TRandom3.h"
#endif

template<class T>
class OSUGenericJetProducer : public edm::stream::EDProducer<>
{
 public:
  OSUGenericJetProducer (const edm::ParameterSet &);
  ~OSUGenericJetProducer ();

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
  edm::InputTag      primaryvertexs_;

  string jetCorrectionPayloadName_;
  string jetResolutionPayload_;
  string jetResSFPayload_;
  bool jetResFromGlobalTag_;
  bool jetResNewPrescription_;

  edm::EDGetTokenT<vector<TYPE(jets)> > token_;
  edm::EDGetTokenT<vector<osu::Mcparticle> > mcparticleToken_;
  edm::EDGetTokenT<vector<TYPE(electrons)> > electronToken_;
  edm::EDGetTokenT<vector<TYPE(muons)> > muonToken_;
  edm::EDGetTokenT<vector<TYPE(genjets)> > genjetsToken_;
  edm::EDGetTokenT<double> rhoToken_;
  edm::EDGetTokenT<vector<TYPE(primaryvertexs)> > primaryvertexsToken_;
  edm::ESGetToken<JetCorrectorParametersCollection, JetCorrectionsRecord> JetCorrParToken_;

  #if CMSSW_VERSION_CODE >= CMSSW_VERSION(12,4,0)
  JME::JetResolution::Token  jetResolutionToken_;
  JME::JetResolutionScaleFactor::Token jetResolutionSFToken_;
  #endif

  edm::ParameterSet  cfg_;
  ////////////////////////////////////////////////////////////////////////////

  // Payload for this EDFilter.
  unique_ptr<vector<T> > pl_;
};

#endif
