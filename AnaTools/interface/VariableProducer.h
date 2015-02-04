#ifndef VARIABLE_PRODUCER

#define VARIABLE_PRODUCER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"
#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"
using anatools::objectHash;

using namespace std;



class VariableProducer : public edm::EDProducer
  {
    public:
      VariableProducer (const edm::ParameterSet &);
      ~VariableProducer ();

      void produce (edm::Event &, const edm::EventSetup &);

    protected:

      edm::ParameterSet collections_;

    private:

      // Methods

      virtual void AddVariables(const edm::Event &, auto_ptr<VariableProducerPayload> &);

  };

#endif
