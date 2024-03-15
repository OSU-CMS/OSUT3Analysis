#include "OSUT3Analysis/DBTools/interface/getEvents.h"

using namespace std;

std::vector<runInfo> getEventsInFile(std::string filename){

    std::vector<runInfo> thisRun = {};

    TString fileIn = filename;
    //std::cout << "Opening file " << fileIn << std::endl;

    int attempts=0;
    bool success=false;

    while (attempts < 5 && success==false){
        attempts++;
        try {
            TFile* myfile = TFile::Open(fileIn, "read");
                        
            fwlite::Event ev(myfile);

            for(ev.toBegin(); !ev.atEnd(); ++ev){
                runInfo thisInfo((unsigned int)ev.eventAuxiliary().run(), (unsigned int)ev.eventAuxiliary().luminosityBlock(), (unsigned int)ev.eventAuxiliary().event());
                thisRun.push_back(thisInfo);
            }

            myfile->Close();

            return thisRun;
        }
        catch (...) {
            std::cout << "Failed to open file " << fileIn << ", attempt number " << attempts << std::endl;
        }
    }

    std::string errMesg = "Failed to open file after 5 attempts";
    if(success==false) throw std::runtime_error(errMesg);
    return thisRun;

}

int getEvents(){

    gSystem->Load("libFWCoreFWLite.so");
    FWLiteEnabler::enable();
    gSystem->Load("libDataFormatsFWLite.so");

    //getMiniAODFile();

    return 0;
}
