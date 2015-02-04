#include "DataFormats/Common/interface/Wrapper.h"

//Add includes for your classes here
#include "OSUT3Analysis/AnaTools/interface/BNstop.h"
#include "OSUT3Analysis/AnaTools/interface/BNPFChgHad.h"
#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

namespace {
   struct OSUT3Analysis_AnaTools {
      //add 'dummy' Wrapper variable for each class type you put into the Event
     BNstop stopdummy0;
     edm::Wrapper<BNstop> stopdummy1;
     vector<BNstop> stopdummy2;
     edm::Wrapper<vector<BNstop> > stopdummy3;

     BNPFChgHad PFChgHaddummy0;
     edm::Wrapper<BNPFChgHad> PFChgHaddummy1;
     vector<BNPFChgHad> PFChgHaddummy2;
     edm::Wrapper<vector<BNPFChgHad> > PFChgHaddummy3;

     std::map<std::string, double> newvarsdummy0;
     edm::Wrapper<std::map<std::string, double> > newvarsdummy1;
     vector<std::map<std::string, double> > newvarsdummy2;
     edm::Wrapper<vector<std::map<std::string, double> > > newvarsdummy3;

     CutCalculatorPayload CutCalculatorPayloadDummy0;
     vector<CutCalculatorPayload> CutCalculatorPayloadDummy1;
     edm::Wrapper<CutCalculatorPayload> CutCalculatorPayloadDummy2;
     edm::Wrapper<vector<CutCalculatorPayload> > CutCalculatorPayloadDummy3;

     Cut cutdummy0;
     edm::Wrapper<Cut> cutdummy1;
     vector<Cut> cutdummy2;
     edm::Wrapper<vector<Cut> > cutdummy3;

     map<string, vector<vector<bool> > > mapcharbooldummy0;
     edm::Wrapper<map<string, vector<vector<bool> > > > mapcharbooldummy1;
     vector<map<string, vector<vector<bool> > > > mapcharbooldummy2;
     edm::Wrapper<vector<map<string, vector<vector<bool> > > > > mapcharbooldummy3;

     vector<bool> booldummy0;
     edm::Wrapper<vector<bool> > booldummy1;
     vector<vector<bool> > booldummy2;
     edm::Wrapper<vector<vector<bool> > > booldummy3;
   };
}
