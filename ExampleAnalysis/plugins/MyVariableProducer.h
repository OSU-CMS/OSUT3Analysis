#ifndef MY_EVENT_VARIABLE_PRODUCER
#define MY_EVENT_VARIABLE_PRODUCER

#include "OSUT3Analysis/AnaTools/interface/EventVariableProducer.h"
#include "TLorentzVector.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

struct OriginalCollections
{

  edm::Handle<vector<reco::Vertex> >        primaryvertexs;
  edm::Handle<vector<PileupSummaryInfo>>    pileupinfos;
  edm::Handle<edm::TriggerResults>          triggers;

#if DATA_FORMAT == AOD
  edm::Handle<vector<reco::GsfElectron> >    electrons;
  edm::Handle<vector<reco::PFJet> >          jets;
  edm::Handle<vector<reco::Muon> >           muons;
#else
  edm::Handle<vector<pat::Electron> >       electrons;
  edm::Handle<vector<pat::Jet> >            jets;
  edm::Handle<vector<pat::Muon> >           muons;
#endif

};

struct OriginalTokens
{

  edm::EDGetTokenT<vector<reco::Vertex> >        primaryvertexs;
  edm::EDGetTokenT<vector<PileupSummaryInfo>>    pileupinfos;
  edm::EDGetTokenT<edm::TriggerResults>          triggers;

#if DATA_FORMAT == AOD
  edm::EDGetTokenT<vector<reco::GsfElectron> >    electrons;
  edm::EDGetTokenT<vector<reco::PFJet> >          jets;
  edm::EDGetTokenT<vector<reco::Muon> >           muons;
#else
  edm::EDGetTokenT<vector<pat::Electron> >       electrons;
  edm::EDGetTokenT<vector<pat::Jet> >            jets;
  edm::EDGetTokenT<vector<pat::Muon> >           muons;
#endif

};

class MyVariableProducer : public EventVariableProducer
  {
    public:
        MyVariableProducer (const edm::ParameterSet &);
        ~MyVariableProducer ();
        void getOriginalCollections (const unordered_set<string> &objectsToGet, const edm::Event &event);
        OriginalCollections handles_;
        OriginalTokens tokens_;

    private:

        // Functions
        void AddVariables(const edm::Event &, const edm::EventSetup &);
  };

#endif
