#ifndef MY_EVENT_VARIABLE_PRODUCER
#define MY_EVENT_VARIABLE_PRODUCER

#include "OSUT3Analysis/AnaTools/interface/EventVariableProducer.h"
#include "TLorentzVector.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

struct OriginalCollections
{
  edm::Handle<vector<pat::Electron> >       electrons;
  edm::Handle<vector<pat::Jet> >            jets;
  edm::Handle<vector<pat::Muon> >           muons;
  edm::Handle<vector<reco::Vertex> >        primaryvertexs;
  edm::Handle<vector<PileupSummaryInfo>>    pileupinfos;
  edm::Handle<edm::TriggerResults>          triggers;
};

struct OriginalTokens
{
  edm::EDGetTokenT<vector<pat::Electron> >       electrons;
  edm::EDGetTokenT<vector<pat::Jet> >            jets;
  edm::EDGetTokenT<vector<pat::Muon> >           muons;
  edm::EDGetTokenT<vector<reco::Vertex> >        primaryvertexs;
  edm::EDGetTokenT<vector<PileupSummaryInfo>>    pileupinfos;
  edm::EDGetTokenT<edm::TriggerResults>          triggers;
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
