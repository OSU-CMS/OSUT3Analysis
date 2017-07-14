#ifndef PUSCALING_FACTOR_PRODUCER
#define PUSCALING_FACTOR_PRODUCER

#include "OSUT3Analysis/AnaTools/interface/EventVariableProducer.h"
#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"
#include "DataFormats/Math/interface/deltaR.h"
#include <string>
#include "TH1D.h"
#include "TFile.h"

class PUScalingFactorProducer : public EventVariableProducer
  {
    public:
        PUScalingFactorProducer (const edm::ParameterSet &);
        ~PUScalingFactorProducer ();

    private:
        string PU_;
        string dataset_;
        string target_;
        string targetUp_;
        string targetDown_;
        TH1D *puWeight_;
        TH1D *puWeightUp_;
        TH1D *puWeightDown_;
        bool isFirstEvent_;
        void AddVariables(const edm::Event &);

        edm::EDGetTokenT<vector<TYPE(pileupinfos)> > pileUpInfosToken_;
};
#endif
