#ifndef EVENT_VARIABLE_PRODUCER

#define EVENT_VARIABLE_PRODUCER

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"
#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"


class EventVariableProducer : public edm::EDFilter
  {
    public:
      EventVariableProducer (const edm::ParameterSet &);
      ~EventVariableProducer ();

      // Methods

      bool filter (edm::Event &, const edm::EventSetup &);

    protected:

      // Variables

      edm::ParameterSet collections_;
      unordered_set<string> objectsToGet_;
      unique_ptr<EventVariableProducerPayload> eventvariables;

    private:

      // Methods

      virtual void AddVariables(const edm::Event &) = 0;

  };

#endif
