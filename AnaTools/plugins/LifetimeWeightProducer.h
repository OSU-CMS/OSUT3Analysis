#ifndef LIFETIME_WEIGHT_PRODUCER
#define LIFETIME_WEIGHT_PRODUCER

#include "OSUT3Analysis/AnaTools/interface/EventVariableProducer.h"

class LifetimeWeightProducer : public EventVariableProducer
  {
    public:
        LifetimeWeightProducer (const edm::ParameterSet &);
        ~LifetimeWeightProducer ();

    private:
        edm::EDGetTokenT<vector<TYPE(hardInteractionMcparticles)> > mcparticlesToken_;

        vector<double> srcCTau_;
        vector<double> dstCTau_;
        vector<int> pdgIds_;

        bool isOriginalParticle (const TYPE(hardInteractionMcparticles) &, const int) const;
        double getCTau (const TYPE(hardInteractionMcparticles) &) const;
        void getFinalPosition (const reco::Candidate &, const int, math::XYZPoint &) const;

        void AddVariables(const edm::Event &);
};
#endif
