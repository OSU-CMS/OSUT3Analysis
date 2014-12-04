#ifndef VARIABLE_PRODUCER

#define VARIABLE_PRODUCER

#include <iostream>
#include <map>
#include <string>
#include "TLorentzVector.h"

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "BEAN/Collections/interface/BNbxlumi.h"
#include "BEAN/Collections/interface/BNelectron.h"
#include "BEAN/Collections/interface/BNevent.h"
#include "BEAN/Collections/interface/BNjet.h"
#include "BEAN/Collections/interface/BNmcparticle.h"
#include "BEAN/Collections/interface/BNmet.h"
#include "BEAN/Collections/interface/BNmuon.h"
#include "BEAN/Collections/interface/BNphoton.h"
#include "BEAN/Collections/interface/BNprimaryvertex.h"
#include "BEAN/Collections/interface/BNskimbits.h"
#include "BEAN/Collections/interface/BNsupercluster.h"
#include "BEAN/Collections/interface/BNtrack.h"
#include "BEAN/Collections/interface/BNtrigger.h"
#include "BEAN/Collections/interface/BNtrigobj.h"
#include "BEAN/Collections/interface/BNtau.h"
#include "BEAN/Collections/interface/BNgenjet.h"

#include "DataFormats/Math/interface/deltaPhi.h"

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

using namespace std;

class VariableProducer : public edm::EDProducer
  {
    public:
      VariableProducer (const edm::ParameterSet &);
      ~VariableProducer ();

      void produce (edm::Event &, const edm::EventSetup &);

    private:

      edm::ParameterSet collectionMap_;

      // Parameters to be passed to the module

      edm::InputTag jets_;
      edm::InputTag muons_;
      edm::InputTag electrons_;
      edm::InputTag taus_;
      edm::InputTag mets_;
      edm::InputTag genjets_;
      edm::InputTag mcparticles_;
      edm::InputTag primaryvertexs_;
      edm::InputTag photons_;
      edm::InputTag triggers_;

      //Collections

      edm::Handle<BNtriggerCollection> triggers;
      edm::Handle<BNjetCollection> jets;
      edm::Handle<BNmuonCollection> muons;
      edm::Handle<BNelectronCollection> electrons;
      edm::Handle<BNtauCollection> taus;
      edm::Handle<BNmetCollection> mets;
      edm::Handle<BNgenjetCollection> genjets;
      edm::Handle<BNmcparticleCollection> mcparticles;
      edm::Handle<BNprimaryvertexCollection> primaryvertexs;
      edm::Handle<BNphotonCollection> photons;

      // Functions

      double setDiMuonMetDeltaPhi(const BNmuonCollection *muons, const BNmetCollection *mets);
      double getDiMuonInvMass    (const BNmuonCollection *muons);  


  };

#endif
