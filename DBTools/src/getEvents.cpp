#include "OSUT3Analysis/DBTools/interface/getEvents.h"

using namespace std;

std::vector<runInfo> getEventsInFile(std::string filename){

    std::vector<runInfo> thisRun = {};

    TString fileIn = filename;
    //std::cout << "Opening file " << filename << std::endl;

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

    // This tells the user that the given dataset has some faulty root file;
    // These files names are written so that the user can decide what to do about them
    std::string errMesg = "Failed to open file after 5 attempts";
    if(success==false) std::cout << errMesg << std::endl;
    return thisRun;

}

int getEvents(){

    gSystem->Load("libFWCoreFWLite.so");
    FWLiteEnabler::enable();
    gSystem->Load("libDataFormatsFWLite.so");

    //getMiniAODFile();

    return 0;
}
