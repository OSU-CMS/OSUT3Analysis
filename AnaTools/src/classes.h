#include "DataFormats/Common/interface/Wrapper.h"

//Add includes for your classes here
#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"
namespace {
   struct OSUT3Analysis_AnaTools {
      //add 'dummy' Wrapper variable for each class type you put into the Event
     std::map<std::string, std::vector<std::pair<std::vector<int>, double> > > newvarsdummy0;
     edm::Wrapper<std::map<std::string, std::vector<std::pair<std::vector<int>, double> > > > newvarsdummy1;
     vector<std::map<std::string, std::vector<std::pair<std::vector<int>, double> > > > newvarsdummy2;
     edm::Wrapper<vector<std::map<std::string, std::vector<std::pair<std::vector<int>, double> > > > > newvarsdummy3;

     CutCalculatorPayload CutCalculatorPayloadDummy0;
     vector<CutCalculatorPayload> CutCalculatorPayloadDummy1;
     edm::Wrapper<CutCalculatorPayload> CutCalculatorPayloadDummy2;
     edm::Wrapper<vector<CutCalculatorPayload> > CutCalculatorPayloadDummy3;

     Cut cutdummy0;
     edm::Wrapper<Cut> cutdummy1;
     vector<Cut> cutdummy2;
     edm::Wrapper<vector<Cut> > cutdummy3;

     VariableProducerPayload VariableProducerPayloadDummy0;
     vector<VariableProducerPayload> VariableProducerPayloadDummy1;
     edm::Wrapper<VariableProducerPayload> VariableProducerPayloadDummy2;
     edm::Wrapper<vector<VariableProducerPayload> > VariableProducerPayloadDummy3;

     EventVariableProducerPayload EventVariableProducerPayloadDummy0;
     vector<EventVariableProducerPayload> EventVariableProducerPayloadDummy1;
     edm::Wrapper<EventVariableProducerPayload> EventVariableProducerPayloadDummy2;
     edm::Wrapper<vector<EventVariableProducerPayload> > EventVariableProducerPayloadDummy3;

     map<string, vector<vector<bool> > > mapcharbooldummy0;
     edm::Wrapper<map<string, vector<vector<bool> > > > mapcharbooldummy1;
     vector<map<string, vector<vector<bool> > > > mapcharbooldummy2;
     edm::Wrapper<vector<map<string, vector<vector<bool> > > > > mapcharbooldummy3;

     vector<bool> booldummy0;
     edm::Wrapper<vector<bool> > booldummy1;
     vector<vector<bool> > booldummy2;
     edm::Wrapper<vector<vector<bool> > > booldummy3;

     pair<const string, vector<UserVariable> > uservariabledummy0;
     pair<const string, double > eventvariabledummy0;
   };
}
