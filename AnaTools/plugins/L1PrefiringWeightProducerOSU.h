#ifndef L1_PREFIRING_WEIGHT_PRODUCER_OSU
#define L1_PREFIRING_WEIGHT_PRODUCER_OSU

#include "OSUT3Analysis/AnaTools/interface/EventVariableProducer.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

class L1PrefiringWeightProducerOSU : public EventVariableProducer
  {
    public:
        L1PrefiringWeightProducerOSU (const edm::ParameterSet &);
        ~L1PrefiringWeightProducerOSU ();

    private:
        const std::string dataera_;

        edm::EDGetTokenT<double> tokenPrefWeight_;
        edm::EDGetTokenT<double> tokenPrefWeightUp_;
        edm::EDGetTokenT<double> tokenPrefWeightDown_;

        edm::EDGetTokenT<vector<TYPE(jets)> > tokenJets_;

        void AddVariables(const edm::Event &, const edm::EventSetup &);
};
#endif
