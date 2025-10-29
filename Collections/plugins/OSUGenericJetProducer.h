#ifndef JET_PRODUCER
#define JET_PRODUCER

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TH2D.h" // Used for JEC/JER corrections
#include "TH3D.h" // Used for JEC/JER corrections

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

  bool jetResNewPrescription_;
  string jecjerFile_;
  string dataPeriod_;
  string dataEra_;

  edm::EDGetTokenT<vector<TYPE(jets)> > token_;
  edm::EDGetTokenT<vector<osu::Mcparticle> > mcparticleToken_;
  edm::EDGetTokenT<vector<TYPE(electrons)> > electronToken_;
  edm::EDGetTokenT<vector<TYPE(muons)> > muonToken_;
  edm::EDGetTokenT<vector<TYPE(genjets)> > genjetsToken_;
  edm::EDGetTokenT<double> rhoToken_;
  edm::EDGetTokenT<vector<TYPE(primaryvertexs)> > primaryvertexsToken_;

  TFile* f_jecjer_;
  string jetEnergyScaleMCL2RelativeHistName_;
  string jetEnergyScaleMCL3AbsoluteHistName_;
  string jetEnergyScaleDATAL2RelativeHistName_;
  string jetEnergyScaleDATAL3AbsoluteHistName_;
  string jetEnergyScaleL2L3HistName_;
  string jetEnergyScaleUncHistName_;
  string jetEnergyResSFNomHistName_;
  string jetEnergyResSFUpHistName_;
  string jetEnergyResSFDownHistName_;
  string jetEnergyResPtResHistName_;

  TH2D* jetEnergyScaleMCL2RelativeHist_;
  TH2D* jetEnergyScaleMCL3AbsoluteHist_;
  TH2D* jetEnergyScaleDATAL2RelativeHist_;
  TH2D* jetEnergyScaleDATAL3AbsoluteHist_;
  TH2D* jetEnergyScaleL2L3Hist_;
  TH2D* jetEnergyScaleUncHist_;
  TH2D* jetEnergyResSFNomHist_;
  TH2D* jetEnergyResSFUpHist_;
  TH2D* jetEnergyResSFDownHist_;
  TH3D* jetEnergyResPtResHist_;



  edm::ParameterSet  cfg_;
  ////////////////////////////////////////////////////////////////////////////

  // Payload for this EDFilter.
  unique_ptr<vector<T> > pl_;
};

#endif
