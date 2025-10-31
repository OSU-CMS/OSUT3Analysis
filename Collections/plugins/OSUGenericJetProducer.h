#ifndef JET_PRODUCER
#define JET_PRODUCER

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TH2D.h" // Used for JEC/JER corrections
#include "TH3D.h" // Used for JEC/JER corrections
#include "TRandom3.h"

#include "OSUT3Analysis/Collections/interface/Jet.h"
#include "OSUT3Analysis/Collections/interface/JecApplication.hpp"
#include "OSUT3Analysis/Collections/interface/JecConfigReader.hpp"


template<class T>
class OSUGenericJetProducer : public edm::stream::EDProducer<>
{
 public:
  OSUGenericJetProducer (const edm::ParameterSet &);
  ~OSUGenericJetProducer ();

  void produce (edm::Event &, const edm::EventSetup &);

 private:
  bool getEventHandles(
    edm::Event &,
    edm::Handle<vector<TYPE(genjets)>> &,
    edm::Handle<double> &,
    edm::Handle<vector<TYPE(electrons)>> &,
    edm::Handle<vector<TYPE(muons)>> &,
    edm::Handle<vector<TYPE(primaryvertexs)>> &
  );

  void buildGoodLeptonCollections(
    edm::Handle<vector<TYPE(electrons)>>,
    edm::Handle<vector<TYPE(muons)>>,
    vector<const TYPE(electrons)*> &,
    vector<const TYPE(muons)*> &
  );

  void applyBTagDiscriminators(T &);

  void checkJetLeptonMatching(
    T &,
    vector<const TYPE(electrons)*> &,
    vector<const TYPE(muons)*> &
  );

  void applyJetEnergyCorrections(JecApplication::EvalContext, T &, edm::Event &);
  void setJERScaleFactors(T &, edm::Handle<double>);
  void calculateMedianIPSig(T &);
  void calculateAlphaMax(T &, edm::Handle<vector<TYPE(primaryvertexs)>>);
  bool tryGetMatchedGenJet(T &, edm::Handle<vector<TYPE(genjets)>>, TYPE(genjets) &);
  void smearJetPtMatched(T &, TYPE(genjets));
  void smearJetPtUnmatched(T &);
  double computeRawFactorFromMiniAOD(const T& jet);
  void setP4Scaled(T& jet, double scale);

  ////////////////////////////////////////////////////////////////////////////
  // Private variables initialized by the constructor.
  ////////////////////////////////////////////////////////////////////////////
  edm::ParameterSet  collections_;
  edm::InputTag      jets_;
  edm::InputTag      electrons_;
  edm::InputTag      muons_;
  edm::InputTag      genjets_;
  edm::InputTag      rho_;
  edm::InputTag      primaryvertexs_;

  bool jetResNewPrescription_;
  string jecjerFile_;
  string year_;
  string dataPeriod_;
  string dataEra_;
  bool isData_;

  edm::EDGetTokenT<vector<TYPE(jets)> > jetsToken_;
  edm::EDGetTokenT<vector<osu::Mcparticle> > mcparticleToken_;
  edm::EDGetTokenT<vector<TYPE(electrons)> > electronToken_;
  edm::EDGetTokenT<vector<TYPE(muons)> > muonToken_;
  edm::EDGetTokenT<vector<TYPE(genjets)> > genjetsToken_;
  edm::EDGetTokenT<double> rhoToken_;
  edm::EDGetTokenT<vector<TYPE(primaryvertexs)> > primaryvertexsToken_;

  TFile* f_jecjer_;
  // string jetEnergyScaleUncHistName_;
  string jetEnergyResSFNomHistName_;
  string jetEnergyResSFUpHistName_;
  string jetEnergyResSFDownHistName_;
  string jetEnergyResPtResHistName_;

  // TH2D* jetEnergyScaleUncHist_;
  TH2D* jetEnergyResSFNomHist_;
  TH2D* jetEnergyResSFUpHist_;
  TH2D* jetEnergyResSFDownHist_;
  TH3D* jetEnergyResPtResHist_;

  edm::FileInPath jecConfigFile_;
  JecConfigReader::CorrectionRefs jecRefs_;

  TRandom3* rng_;

  edm::ParameterSet  cfg_;

  unique_ptr<vector<T>> outputJets_;
};

#endif
