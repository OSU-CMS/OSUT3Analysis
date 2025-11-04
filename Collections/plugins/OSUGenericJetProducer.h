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
  void applySmearedJetEnergy(JecApplication::EvalContext, T &, edm::Event &);
  void setJERScaleFactors(T &, edm::Handle<double>);
  void calculateMedianIPSig(T &);
  void calculateAlphaMax(T &, edm::Handle<vector<TYPE(primaryvertexs)>>);
  bool tryGetMatchedGenJet(T &, edm::Handle<vector<TYPE(genjets)>>, TYPE(genjets) &);
  double computeRawFactorFromMiniAOD(const T& jet);
  void setP4Scaled(T& jet, double scale);
  double getJercFactor(JecApplication::EvalContext, T &, edm::Event &, JecConfigReader::SystKind, std::string);

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

  string year_;
  string dataEra_;
  bool isData_;

  edm::EDGetTokenT<vector<TYPE(jets)> > jetsToken_;
  edm::EDGetTokenT<vector<osu::Mcparticle> > mcparticleToken_;
  edm::EDGetTokenT<vector<TYPE(electrons)> > electronToken_;
  edm::EDGetTokenT<vector<TYPE(muons)> > muonToken_;
  edm::EDGetTokenT<vector<TYPE(genjets)> > genjetsToken_;
  edm::EDGetTokenT<double> rhoToken_;
  edm::EDGetTokenT<vector<TYPE(primaryvertexs)> > primaryvertexsToken_;

  edm::FileInPath jecConfigFile_;
  JecConfigReader::CorrectionRefs jecRefs_;
  string jesUncTag_;

  edm::ParameterSet  cfg_;

  unique_ptr<vector<T>> outputJets_;
};

#endif
