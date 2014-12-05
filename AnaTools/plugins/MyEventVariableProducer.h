#ifndef MY_EVENT_VARIABLE_PRODUCER

#define MY_EVENT_VARIABLE_PRODUCER

#include "OSUT3Analysis/AnaTools/plugins/EventVariableProducer.h"


class MyEventVariableProducer : public EventVariableProducer
  {
    public:
        MyEventVariableProducer (const edm::ParameterSet &);
	~MyEventVariableProducer ();

//    void produce (edm::Event &, const edm::EventSetup &);

    private:

      // Functions

	void AddVariables(const edm::Event &, map<string, double> &);



  };

#endif
