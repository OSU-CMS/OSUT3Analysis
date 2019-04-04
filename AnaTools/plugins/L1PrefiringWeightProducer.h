#ifndef L1_PREFIRING_WEIGHT_PRODUCER
#define L1_PREFIRING_WEIGHT_PRODUCER

#include "OSUT3Analysis/AnaTools/interface/EventVariableProducer.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

class L1PrefiringWeightProducer : public EventVariableProducer
  {
    public:
        L1PrefiringWeightProducer (const edm::ParameterSet &);
        ~L1PrefiringWeightProducer ();

    private:
        const std::string dataera_;

        edm::EDGetTokenT<double> tokenPrefWeight_;
        edm::EDGetTokenT<double> tokenPrefWeightUp_;
        edm::EDGetTokenT<double> tokenPrefWeightDown_;

        edm::EDGetTokenT<vector<TYPE(jets)> > tokenJets_;

        void AddVariables(const edm::Event &);
};
#endif
