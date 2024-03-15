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

/*void getMiniAODFile(){

    std::string json;

    std::string fin = "Muon_2022E.json";
    ifstream t(fin);
    std::stringstream buffer;
    buffer << t.rdbuf();
    json = buffer.str();

    Json::Reader reader;
    Json::Value jsonRoot;
    bool parseSuccess = reader.parse(json, jsonRoot, false);
    if(parseSuccess){
        std::cout << "success" << std::endl;
        for (const auto& entry : jsonRoot.getMemberNames()) {
            std::string key = entry;
            std::cout << entry << std::endl;
            std::vector<runInfo> thisAOD = getEventsInFile(key);
            std::cout << "Entries: " << thisAOD.size() << std::endl;
            for(auto& entry: thisAOD){
                std::cout << entry.runNum << ", " << entry.event << ", "<< entry.lumiBlock << std::endl;
            }
        }

    }
    else{
        std::cout <<"failed" << std::endl;
    }
}*/

int getEvents(){

    gSystem->Load("libFWCoreFWLite.so");
    FWLiteEnabler::enable();
    gSystem->Load("libDataFormatsFWLite.so");

    //getMiniAODFile();

    return 0;
}
