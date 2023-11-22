#include "TFile.h"
#include "TTree.h"
//#include "EventAuxiliary.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "FWCore/FWLite/interface/FWLiteEnabler.h"
#include "OSUT3Analysis/DBTools/interface/json.h"
#include <iostream>
#include <fstream>
#include <TSystem.h>
#include <vector>
#include <string>
#include <fstream>

struct runInfo {
    runInfo(int runNum_, int lumiBlock_, int event_){
        runNum = runNum_;
        lumiBlock = lumiBlock_;
        event = event_;
    }
    int runNum;
    int lumiBlock;
    int event;
};


std::vector<runInfo> getEventsInFile(std::string filename);

void getMiniAODFile();

int getEvents();
