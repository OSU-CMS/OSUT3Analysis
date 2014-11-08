#include "DataFormats/Common/interface/Wrapper.h"

//Add includes for your classes here
#include "OSUT3Analysis/AnaTools/interface/BNstop.h"
#include "OSUT3Analysis/AnaTools/interface/BNPFChgHad.h"
#include "OSUT3Analysis/AnaTools/plugins/AnalysisPayloads.h"
#include <utility>
#include <vector>
#include <map>
#include <string>

namespace {
   struct OSUT3Analysis_AnaTools {
      //add 'dummy' Wrapper variable for each class type you put into the Event
     BNstop stopdummy0;
     edm::Wrapper<BNstop> stopdummy1;
     std::vector<BNstop> stopdummy2;
     edm::Wrapper<std::vector<BNstop> > stopdummy3;

     BNPFChgHad PFChgHaddummy0;
     edm::Wrapper<BNPFChgHad> PFChgHaddummy1;
     std::vector<BNPFChgHad> PFChgHaddummy2;
     edm::Wrapper<std::vector<BNPFChgHad> > PFChgHaddummy3;

     std::map<std::string, double> newvarsdummy0;
     edm::Wrapper<std::map<std::string, double> > newvarsdummy1;
     std::vector<std::map<std::string, double> > newvarsdummy2;
     edm::Wrapper<std::vector<std::map<std::string, double> > > newvarsdummy3;

     CutCalculatorPayload CutCalculatorPayloadDummy0;
     std::vector<CutCalculatorPayload> CutCalculatorPayloadDummy1;
     edm::Wrapper<CutCalculatorPayload> CutCalculatorPayloadDummy2;
     edm::Wrapper<std::vector<CutCalculatorPayload> > CutCalculatorPayloadDummy3;

     Cut cutdummy0;
     edm::Wrapper<Cut> cutdummy1;
     std::vector<Cut> cutdummy2;
     edm::Wrapper<std::vector<Cut> > cutdummy3;

     std::map<std::basic_string<char>,std::vector<std::vector<bool> > > mapcharbooldummy0;
     edm::Wrapper<std::map<std::basic_string<char>,std::vector<std::vector<bool> > > > mapcharbooldummy1;
     std::vector<std::map<std::basic_string<char>,std::vector<std::vector<bool> > > > mapcharbooldummy2;
     edm::Wrapper<std::vector<std::map<std::basic_string<char>,std::vector<std::vector<bool> > > > > mapcharbooldummy3;

     std::vector<bool> booldummy0;
     edm::Wrapper<std::vector<bool> > booldummy1;
     std::vector<std::vector<bool> > booldummy2;
     edm::Wrapper<std::vector<std::vector<bool> > > booldummy3;


   };
}
