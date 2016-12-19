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

class PUScalingFactorProducer : public EventVariableProducer
  {
    public:
        PUScalingFactorProducer (const edm::ParameterSet &);
        void getOriginalCollections (const edm::Event &event);
        ~PUScalingFactorProducer ();
        OriginalCollections handles_;

    private:
        string PU_;
        string dataset_;
        string target_;
        TH1D *puWeight_;
        void AddVariables(const edm::Event &);

        edm::EDGetTokenT<vector<TYPE(pileupinfos)> > pileUpInfosToken_;
};
#endif
