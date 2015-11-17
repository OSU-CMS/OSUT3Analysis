#ifndef PUSCALING_FACTOR_PRODUCER
#define PUSCALING_FACTOR_PRODUCER

#include "OSUT3Analysis/AnaTools/interface/EventVariableProducer.h"
#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"
#include "DataFormats/Math/interface/deltaR.h"
#include <string>
#include "TH1D.h"
#include "TFile.h"
struct OriginalCollections
{
  edm::Handle<vector<PileupSummaryInfo>>    pileupinfos;
};

class PFScalingFactorProducer : public EventVariableProducer
  {
    public:
        PFScalingFactorProducer (const edm::ParameterSet &);
        void getOriginalCollections (const unordered_set<string> &objectsToGet, const edm::ParameterSet &collections, OriginalCollections &handles, const edm::Event &event);
        bool passCleaning (double eta, double phi, OriginalCollections &handles);
	~PFScalingFactorProducer ();
        OriginalCollections handles_;

    private:
        string PU_;             
        string dataset_; 
        string type_; 
        void AddVariables(const edm::Event &);
};
#endif
