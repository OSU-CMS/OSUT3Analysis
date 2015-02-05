#ifndef MY_EVENT_VARIABLE_PRODUCER
#define MY_EVENT_VARIABLE_PRODUCER

#include "OSUT3Analysis/AnaTools/interface/VariableProducer.h"
#include "TLorentzVector.h"
#include "DataFormats/Math/interface/deltaPhi.h"

class MyVariableProducer : public VariableProducer
  {
    public:
        MyVariableProducer (const edm::ParameterSet &);
	~MyVariableProducer ();

    private:

	// Functions
	void AddVariables(const edm::Event &, auto_ptr<VariableProducerPayload> &);
  };

#endif
