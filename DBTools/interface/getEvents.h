#include "TFile.h"
#include "TTree.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "FWCore/FWLite/interface/FWLiteEnabler.h"
#include <iostream>
#include <fstream>
#include <TSystem.h>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

struct runInfo {
    runInfo(unsigned int runNum_, unsigned int lumiBlock_, unsigned int event_){
        runNum = runNum_;
        lumiBlock = lumiBlock_;
        event = event_;
    }
    unsigned int runNum;
    unsigned int lumiBlock;
    unsigned int event;
};


std::vector<runInfo> getEventsInFile(std::string filename);

int getEvents();
