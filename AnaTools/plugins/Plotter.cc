#include "OSUT3Analysis/AnaTools/plugins/Plotter.h"

// The Plotter class handles user-defined histograms
// As input, it takes:
//   1. histogram definitions
//   2. names of miniAOD collections to be used
// It outputs a root file with corresponding histograms

Plotter::Plotter (const edm::ParameterSet &cfg) :

  // In the constructor, we parse the input histogram definitions
  // We create the appropriate directory structure in the output file
  // Then we book the TH1/TH2 objects in these directories

  /// Retrieve parameters from the configuration file.
//   jets_ (cfg.getParameter<edm::InputTag> ("jets")),
  muons_ (cfg.getParameter<edm::InputTag> ("muons")),
  electrons_ (cfg.getParameter<edm::InputTag> ("electrons")),
//   taus_ (cfg.getParameter<edm::InputTag> ("taus")),
//   mets_ (cfg.getParameter<edm::InputTag> ("mets")),
//   tracks_ (cfg.getParameter<edm::InputTag> ("tracks")),
//   genjets_ (cfg.getParameter<edm::InputTag> ("genjets")),
//   mcparticles_ (cfg.getParameter<edm::InputTag> ("mcparticles")),
//   primaryvertexs_ (cfg.getParameter<edm::InputTag> ("primaryvertexs")),
//   photons_ (cfg.getParameter<edm::InputTag> ("photons")),
//   triggers_ (cfg.getParameter<edm::InputTag> ("triggers")),
//   trigobjs_ (cfg.getParameter<edm::InputTag> ("trigobjs")),
  histogramSets_ (cfg.getParameter<vector<edm::ParameterSet> >("histogramSets")),
  verbose_ (cfg.getParameter<int> ("verbose"))

{
  
  if (verbose_) clog << "Beginning Plotter::Plotter constructor." << endl;

  TH1::SetDefaultSumw2();

  /////////////////////////////////////
  // parse the histogram definitions //
  /////////////////////////////////////
  
  vector<TFileDirectory> directories; //vector of directories in the output file.                                                                                                                       
  
  // loop over each histogram set the user has included
  for(uint histoSet = 0; histoSet != histogramSets_.size(); histoSet++){

    string inputCollection = histogramSets_.at(histoSet).getParameter<string> ("inputCollection");

    // parse the input collection to see what objects it contains
    vector<string> inputObjects = getInputTypes(inputCollection);

    // if we haven't already included the object(s), do so now
    vector<string>::iterator inputObject;
    for(inputObject = inputObjects.begin(); inputObject != inputObjects.end(); ++inputObject){

      if (find(objectsToGet.begin(), objectsToGet.end(), *inputObject) == objectsToGet.end()){
	objectsToGet.push_back(*inputObject);
      }

    }

    // take care of cases in which the user puts a pair in the wrong order
    inputCollection = fixOrdering(inputCollection);
    
    // create a directory in the output file to hold the histogram set
    string directoryName = getDirectoryName(inputCollection);
    TFileDirectory subDir = fs_->mkdir(directoryName);

    vector<edm::ParameterSet> histogramList (histogramSets_.at(histoSet).getParameter<vector<edm::ParameterSet> >("histograms"));

    // loop over each histogram
    vector<edm::ParameterSet>::iterator histogram;
    for(histogram = histogramList.begin(); histogram != histogramList.end(); ++histogram){

      // parse the definition and save the relevant info
      Plotter::histoDef histoDefinition = parseHistoDef(*histogram,inputCollection,&subDir);
      histogramDefinitions.push_back(histoDefinition);

    } // end loop on histograms in the set
  } // end loop on histogram sets


  // loop over each histogram configuration
  vector<Plotter::histoDef>::iterator histogram;
  for(histogram = histogramDefinitions.begin(); histogram != histogramDefinitions.end(); ++histogram){
    
    // book a TH1/TH2 in the appropriate folder
    bookHistogram(*histogram);


  }    // end loop on parsed histograms



  vector<string>::iterator inputObject;
  cout << "list of objects to get: " << endl;
  for(inputObject = objectsToGet.begin(); inputObject != objectsToGet.end(); ++inputObject){
    cout << *inputObject << endl;
  }
}      

////////////////////////////////////////////////////////////////////////

// in the analyze function, we retrieve all the needed collections from the event
// and then loop on over the histogram sets and fill each histogram

void
Plotter::analyze (const edm::Event &event, const edm::EventSetup &setup)
{


}

////////////////////////////////////////////////////////////////////////

Plotter::~Plotter ()
{}

////////////////////////////////////////////////////////////////////////


// function to convert an input collection into a directory name
string Plotter::getDirectoryName(string inputName){
  string parsedName;

  bool isPair = inputName.find("pair") !=string::npos;

  if(isPair){
    vector<string> objects = getInputTypes(inputName);
    if(objects.at(0) == objects.at(1)){  // identical objects
      parsedName = "Di" + singular(objects.at(0));
    }
    else{
      parsedName = capitalize(singular(objects.at(0))) + "-" + capitalize(singular(objects.at(1)));
    }
  }
  else{
    parsedName = capitalize(singular(inputName));
  }

  parsedName += " Plots";

  return parsedName;

}

////////////////////////////////////////////////////////////////////////

vector<string> Plotter::getInputTypes(string inputName){
  
  // Return the object type(s) from the inputName string,
  // e.g. "muons" returns ["muons"],
  // and "electron-muon pairs" returns:
  // ["electrons","muons"]


  vector<string> inputTypes;

  bool isPair = inputName.find("pair")!=string::npos;

  if(isPair){
    int dashIndex = inputName.find("-");
    int spaceIndex = inputName.find_last_of(" ");
    int secondWordLength = spaceIndex - dashIndex;
    string obj1 = plural(inputName.substr(0,dashIndex));
    string obj2 = plural(inputName.substr(dashIndex+1,secondWordLength-1));
    
    inputTypes.push_back(obj1);
    inputTypes.push_back(obj2);
  }
  else{
    inputTypes.push_back(inputName);
  }

  return inputTypes;

}

////////////////////////////////////////////////////////////////////////

string Plotter::capitalize(string input){
  
  input[0] = toupper(input[0]);
  return input;

}

////////////////////////////////////////////////////////////////////////

string Plotter::singular(string input){

  if(strcmp(&input.back(), "s") == 0){ // remove trailing "s"
    return input.substr(0, input.size()-1);
  }
  else{ 
    return input;
  }

}

////////////////////////////////////////////////////////////////////////

string Plotter::plural(string input){

  if(strcmp(&input.back(), "s") == 0){ 
    return input;
  }
  else{
    return input + "s"; // add trailing "s"
  }

}

////////////////////////////////////////////////////////////////////////

string Plotter::fixOrdering(string input){

  if(input == "muon-electron pairs")                    return "electron-muon pairs";
  if(input == "photon-muon pairs")                      return "muon-photon pairs";
  if(input == "photon-electron pairs")                  return "electron-photon pairs";
  if(input == "jet-electron pairs")                     return "electron-jet pairs";
  if(input == "jet-photon pairs")                       return "photon-jet pairs";
  if(input == "jet-muon pairs")                         return "muon-jet pairs";
  if(input == "event-muon pairs")                       return "muon-event pairs";
  if(input == "event-electron pairs")                   return "electron-event pairs";
  if(input == "jet-met pairs")                          return "met-jet pairs";
  if(input == "mcparticle-met pairs")                   return "met-mcparticle pairs";
  if(input == "secondary mcparticle-jet pairs")         return "jet-secondary mcparticle pairs";
  if(input == "mcparticle-jet pairs")                   return "jet-mcparticle pairs";
  if(input == "secondary mcparticle-mcparticle pairs")  return "mcparticle-secondary mcparticle pairs";
  if(input == "track-jet pairs")                        return "track-jet pairs";
  if(input == "event-track pairs")                      return "track-event pairs";
  if(input == "secondary muon-muon pairs")              return "muon-secondary muon pairs";
  if(input == "secondary jet-muon pairs")               return "muon-secondary jet pairs";
  if(input == "jet-secondary muon pairs")               return "secondary muon-jet pairs";
  if(input == "secondary photon-muon pairs")            return "muon-secondary photon pairs";
  if(input == "secondary jet-electron pairs")           return "electron-secondary jet pairs";
  if(input == "jet-secondary electron pairs")           return "secondary electron-jet pairs";
  if(input == "secondary jet-photon pairs")             return "photon-secondary jet pairs";
  if(input == "secondary jet-jet pairs")                return "jet-secondary jet pairs";
  if(input == "secondary electron-electron pairs")      return "electron-secondary electron pairs";
  if(input == "trigobj-electron pairs")                 return "electron-trigobj pairs";
  if(input == "trigobj-muon pairs")                     return "muon-trigobj pairs";
  if(input == "mcparticle-electron pairs")              return "electron-mcparticle pairs";
  if(input == "mcparticle-muon pairs")                  return "muon-mcparticle pairs";
  if(input == "secondary mcparticle-muon pairs")        return "muon-secondary mcparticle pairs";
  if(input == "mcparticle-track pairs")                 return "track-mcparticle pairs";
  if(input == "mcparticle-stop pairs")                  return "stop-mcparticle pairs";

  return input;

}

////////////////////////////////////////////////////////////////////////

// copies a histogram configuration into a C++ container
Plotter::histoDef Plotter::parseHistoDef(const edm::ParameterSet definition, string inputCollection, TFileDirectory *subDir){

  Plotter::histoDef parsedDef;

  vector<double> defaults;
  defaults.push_back(-1.0);

  parsedDef.inputCollection = inputCollection;
  parsedDef.directory = subDir;
  parsedDef.name = definition.getParameter<string>("name");
  parsedDef.title = definition.getParameter<string>("title");
  parsedDef.binsX = definition.getUntrackedParameter<vector<double> >("variableBinsX", defaults);
  parsedDef.binsY = definition.getUntrackedParameter<vector<double> >("variableBinsY", defaults);
  parsedDef.inputVariables = definition.getParameter<vector<string> >("inputVariables");
  parsedDef.dimensions = parsedDef.inputVariables.size();

  return parsedDef;

}

void Plotter::bookHistogram(Plotter::histoDef definition){

  string name = definition.name;
  string title = definition.title;

  // check for variable bins
  bool hasVariableBinsX = definition.binsX.size() > 3;
  cout << "hasVariableBinsX: " << hasVariableBinsX << endl;

  // decide what type of histogram to book (TH1 or TH2)


  //  directories.at(currentDir).make<TH1D> (TString(currentHistogram.name),channelLabel+" channel: "+currentHistogram.title, \
    //  currentHistogram.bins.at(0), currentHistogram.bins.at(1), currentHistogram.bins.at(2));

}


DEFINE_FWK_MODULE(Plotter);
