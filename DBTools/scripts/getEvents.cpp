using namespace std;


std::vector<runInfo> getEventsInFile(std::string filename){
    std::cout << "Filename " << filename << std::endl;
    TString fileIn = "root://cmsxrootd.fnal.gov:/" + filename;
    TFile* myfile = TFile::Open(fileIn, "read");
    fwlite::Event ev(myfile);

    std::vector<runInfo> thisRun = {};

    for(ev.toBegin(); !ev.atEnd(); ++ev){
        runInfo thisInfo(ev.eventAuxiliary().run(), ev.eventAuxiliary().luminosityBlock(), ev.eventAuxiliary().event());
        thisRun.push_back(thisInfo);
    }

    return thisRun;
}

void getMiniAODFile(){

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
            //Json::Value miniAODList = jsonRoot[key];
            //cout << "\t Values: " << miniAODList << endl;
        }

    }
    else{
        std::cout <<"failed" << std::endl;
    }
}

int getEvents(){

    gSystem->Load("libFWCoreFWLite.so");
    FWLiteEnabler::enable();
    gSystem->Load("libDataFormatsFWLite.so");

    /*TFile* myfile = TFile::Open("97f3283e-1180-440b-bc86-7a8ae71cb5b6.root", "read");
    TTree* mytree = (TTree*)myfile->Get("Events");

    fwlite::Event ev(myfile);

    for (ev.toBegin(); ! ev.atEnd(); ++ev ){
        cout << "Run: " << ev.eventAuxiliary().run() << ", LumiBlock: " << ev.eventAuxiliary().luminosityBlock() << ", Event: " << ev.eventAuxiliary().event() <<std::endl; 
    }*/

    getMiniAODFile();

    return 0;
}