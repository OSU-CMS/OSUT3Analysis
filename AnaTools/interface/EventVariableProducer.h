#ifndef EVENT_VARIABLE_PRODUCER

#define EVENT_VARIABLE_PRODUCER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

using namespace std;

class EventVariableProducer : public edm::EDProducer
  {
    public:
      EventVariableProducer (const edm::ParameterSet &);
      ~EventVariableProducer ();

      void produce (edm::Event &, const edm::EventSetup &);

    protected:

      edm::ParameterSet collections_;

    private:

      // Methods

      virtual void AddVariables(const edm::Event &, auto_ptr<EventVariableProducerPayload> &);

  };

#endif
