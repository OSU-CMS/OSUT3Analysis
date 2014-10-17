#include "DataFormats/Common/interface/Wrapper.h"

//Add includes for your classes here
#include "OSUT3Analysis/AnaTools/interface/BNstop.h"
#include "OSUT3Analysis/AnaTools/interface/BNPFChgHad.h"
#include "OSUT3Analysis/AnaTools/interface/UserVars.h"


#include <vector>

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

     UserVars uservarsdummy0;
     edm::Wrapper<UserVars> uservarsdummy1;
     std::vector<UserVars> uservarsdummy2;
     edm::Wrapper<std::vector<UserVars> > uservarsdummy3;

     std::map<std::string, double> newvarsdummy0;
     edm::Wrapper<std::map<std::string, double> > newvarsdummy1;
     std::vector<std::map<std::string, double> > newvarsdummy2;
     edm::Wrapper<std::vector<std::map<std::string, double> > > newvarsdummy3;

   };
}
