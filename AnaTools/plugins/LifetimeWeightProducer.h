#ifndef LIFETIME_WEIGHT_PRODUCER
#define LIFETIME_WEIGHT_PRODUCER

#include <assert.h>

#include "OSUT3Analysis/AnaTools/interface/EventVariableProducer.h"

class LifetimeWeightProducer : public EventVariableProducer {
    public:
        LifetimeWeightProducer(const edm::ParameterSet &);
        ~LifetimeWeightProducer();

    private:
        edm::EDGetTokenT<vector<TYPE(hardInteractionMcparticles)> > mcparticlesToken_;

        edm::VParameterSet reweightingRules_;
        bool requireLastNotFirstCopy_;

        vector<vector<double> > srcCTau_;
        vector<vector<double> > dstCTau_;
        vector<vector<int> >    pdgIds_;

        vector<double> weights_;
        vector<string> weightNames_;

        bool isOriginalParticle(const TYPE(hardInteractionMcparticles) &, const int) const;
        double getCTau(const TYPE(hardInteractionMcparticles) &) const;
        void getFinalPosition(const reco::Candidate &, const int, bool, math::XYZPoint &) const;

        void AddVariables(const edm::Event &);
};
#endif
