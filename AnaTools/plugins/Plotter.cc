#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"
#include "OSUT3Analysis/AnaTools/plugins/Plotter.h"

#define EXIT_CODE 5

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
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),

  histogramSets_ (cfg.getParameter<vector<edm::ParameterSet> >("histogramSets")),
  verbose_ (cfg.getParameter<int> ("verbose")),

  firstEvent_ (true),
  vl_ (NULL)

{
  
  if (verbose_) clog << "Beginning Plotter::Plotter constructor." << endl;

  if  (collections_.exists  ("bxlumis"))         bxlumis_         =  collections_.getParameter<edm::InputTag>  ("bxlumis");
  if  (collections_.exists  ("electrons"))       electrons_       =  collections_.getParameter<edm::InputTag>  ("electrons");
  if  (collections_.exists  ("events"))          events_          =  collections_.getParameter<edm::InputTag>  ("events");
  if  (collections_.exists  ("genjets"))         genjets_         =  collections_.getParameter<edm::InputTag>  ("genjets");
  if  (collections_.exists  ("jets"))            jets_            =  collections_.getParameter<edm::InputTag>  ("jets");
  if  (collections_.exists  ("mcparticles"))     mcparticles_     =  collections_.getParameter<edm::InputTag>  ("mcparticles");
  if  (collections_.exists  ("mets"))            mets_            =  collections_.getParameter<edm::InputTag>  ("mets");
  if  (collections_.exists  ("muons"))           muons_           =  collections_.getParameter<edm::InputTag>  ("muons");
  if  (collections_.exists  ("photons"))         photons_         =  collections_.getParameter<edm::InputTag>  ("photons");
  if  (collections_.exists  ("primaryvertexs"))  primaryvertexs_  =  collections_.getParameter<edm::InputTag>  ("primaryvertexs");
  if  (collections_.exists  ("secMuons"))        secMuons_        =  collections_.getParameter<edm::InputTag>  ("secMuons");
  if  (collections_.exists  ("superclusters"))   superclusters_   =  collections_.getParameter<edm::InputTag>  ("superclusters");
  if  (collections_.exists  ("taus"))            taus_            =  collections_.getParameter<edm::InputTag>  ("taus");
  if  (collections_.exists  ("tracks"))          tracks_          =  collections_.getParameter<edm::InputTag>  ("tracks");
  if  (collections_.exists  ("triggers"))        triggers_        =  collections_.getParameter<edm::InputTag>  ("triggers");
  if  (collections_.exists  ("trigobjs"))        trigobjs_        =  collections_.getParameter<edm::InputTag>  ("trigobjs");
  if  (collections_.exists  ("userVariables"))   userVariables_   =  collections_.getParameter<edm::InputTag>  ("userVariables");

  TH1::SetDefaultSumw2();

  /////////////////////////////////////
  // parse the histogram definitions //
  /////////////////////////////////////
  
  // loop over each histogram set the user has included
  for(unsigned histoSet = 0; histoSet != histogramSets_.size(); histoSet++){

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
    
    // get the appropriate directory name
    string directoryName = getDirectoryName(inputCollection);

    // import all the histogram definitions for the current set
    vector<edm::ParameterSet> histogramList (histogramSets_.at(histoSet).getParameter<vector<edm::ParameterSet> >("histograms"));

    // loop over each histogram
    vector<edm::ParameterSet>::iterator histogram;
    for(histogram = histogramList.begin(); histogram != histogramList.end(); ++histogram){

      // parse the definition and save the relevant info
      histoDef histoDefinition = parseHistoDef(*histogram,inputCollection,directoryName);
      histogramDefinitions.push_back(histoDefinition);

    } // end loop on histograms in the set

  } // end loop on histogram sets


  // loop over each parsed histogram configuration
  vector<histoDef>::iterator histogram;
  for(histogram = histogramDefinitions.begin(); histogram != histogramDefinitions.end(); ++histogram){
    
    // book a TH1/TH2 in the appropriate folder
    bookHistogram(*histogram);

  } // end loop on parsed histograms

}      

////////////////////////////////////////////////////////////////////////

// in the analyze function, we retrieve all the needed collections from the event
// and then loop on over the histogram sets and fill each histogram

void
Plotter::analyze (const edm::Event &event, const edm::EventSetup &setup)
{

  // get the required collections from the event
  if (find(objectsToGet.begin(), objectsToGet.end(), "bxlumis") != objectsToGet.end()){
    event.getByLabel (bxlumis_, bxlumis);
    if (!bxlumis.product()) clog << "ERROR: could not get bxlumis input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "electrons") != objectsToGet.end()){
    event.getByLabel (electrons_, electrons);
    if (!electrons.product()) clog << "ERROR: could not get electrons input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "events") != objectsToGet.end()){
    event.getByLabel (events_, events);
    if (!events.product()) clog << "ERROR: could not get events input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "genjets") != objectsToGet.end()){
    event.getByLabel (genjets_, genjets);
    if (!genjets.product()) clog << "ERROR: could not get genjets input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "jets") != objectsToGet.end()){
    event.getByLabel (jets_, jets);
    if (!jets.product()) clog << "ERROR: could not get jets input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "mcparticles") != objectsToGet.end()){
    event.getByLabel (mcparticles_, mcparticles);
    if (!mcparticles.product()) clog << "ERROR: could not get mcparticles input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "mets") != objectsToGet.end()){
    event.getByLabel (mets_, mets);
    if (!mets.product()) clog << "ERROR: could not get mets input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "muons") != objectsToGet.end()){
    event.getByLabel (muons_, muons);
    if (!muons.product()) clog << "ERROR: could not get muons input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "photons") != objectsToGet.end()){
    event.getByLabel (photons_, photons);
    if (!photons.product()) clog << "ERROR: could not get photons input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "primaryvertexs") != objectsToGet.end()){
    event.getByLabel (primaryvertexs_, primaryvertexs);
    if (!primaryvertexs.product()) clog << "ERROR: could not get primaryvertexs input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "secMuons") != objectsToGet.end()){
    event.getByLabel (secMuons_, secMuons);
    if (!secMuons.product()) clog << "ERROR: could not get secMuons input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "superclusters") != objectsToGet.end()){
    event.getByLabel (superclusters_, superclusters);
    if (!superclusters.product()) clog << "ERROR: could not get superclusters input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "taus") != objectsToGet.end()){
    event.getByLabel (taus_, taus);
    if (!taus.product()) clog << "ERROR: could not get taus input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "tracks") != objectsToGet.end()){
    event.getByLabel (tracks_, tracks);
    if (!tracks.product()) clog << "ERROR: could not get tracks input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "triggers") != objectsToGet.end()){
    event.getByLabel (triggers_, triggers);
    if (!triggers.product()) clog << "ERROR: could not get triggers input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "trigobjs") != objectsToGet.end()){
    event.getByLabel (trigobjs_, trigobjs);
    if (!trigobjs.product()) clog << "ERROR: could not get trigobjs input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "userVariables") != objectsToGet.end()){
    event.getByLabel (userVariables_, userVariables);
    if (!userVariables.product()) clog << "ERROR: could not get userVariables input collection" << endl;
  }


  initializeValueLookup ();
  if (!initializeValueLookupTrees (histogramDefinitions))
    {
      clog << "ERROR: failed to parse input variables. Quitting..." << endl;
      exit (EXIT_CODE);
    }
 
  // now that we have all the required objects, we'll loop over the histograms, filling each one as we go

  vector<histoDef>::iterator histogram;
  for(histogram = histogramDefinitions.begin(); histogram != histogramDefinitions.end(); ++histogram){
    if        (histogram->inputCollection  ==  "bxlumis")                fillHistogram  (*histogram,  bxlumis.product         ());
    else  if  (histogram->inputCollection  ==  "electrons")              fillHistogram  (*histogram,  electrons.product       ());
    else  if  (histogram->inputCollection  ==  "events")                 fillHistogram  (*histogram,  events.product          ());
    else  if  (histogram->inputCollection  ==  "genjets")                fillHistogram  (*histogram,  genjets.product         ());
    else  if  (histogram->inputCollection  ==  "mcparticles")            fillHistogram  (*histogram,  mcparticles.product     ());
    else  if  (histogram->inputCollection  ==  "mets")                   fillHistogram  (*histogram,  mets.product            ());
    else  if  (histogram->inputCollection  ==  "muons")                  fillHistogram  (*histogram,  muons.product           ());
    else  if  (histogram->inputCollection  ==  "photons")                fillHistogram  (*histogram,  photons.product         ());
    else  if  (histogram->inputCollection  ==  "primaryvertexs")         fillHistogram  (*histogram,  primaryvertexs.product  ());
    else  if  (histogram->inputCollection  ==  "secondary electrons")    fillHistogram  (*histogram,  electrons.product       ());
    else  if  (histogram->inputCollection  ==  "secondary jets")         fillHistogram  (*histogram,  jets.product            ());
    else  if  (histogram->inputCollection  ==  "secondary mcparticles")  fillHistogram  (*histogram,  mcparticles.product     ());
    else  if  (histogram->inputCollection  ==  "secondary muons")        fillHistogram  (*histogram,  muons.product           ());
    else  if  (histogram->inputCollection  ==  "secondary photons")      fillHistogram  (*histogram,  photons.product         ());
    else  if  (histogram->inputCollection  ==  "superclusters")          fillHistogram  (*histogram,  superclusters.product   ());
    else  if  (histogram->inputCollection  ==  "taus")                   fillHistogram  (*histogram,  taus.product            ());
    else  if  (histogram->inputCollection  ==  "tracks")                 fillHistogram  (*histogram,  tracks.product          ());
    else  if  (histogram->inputCollection  ==  "trigobjs")               fillHistogram  (*histogram,  trigobjs.product        ());
    else  if  (histogram->inputCollection  ==  "jets")                   fillHistogram  (*histogram,  jets.product            ());
    else  if  (histogram->inputCollection  ==  "userVariables")          fillHistogram  (*histogram,  userVariables.product   ());

    else  if  (histogram->inputCollection  ==  "electron-electron pairs")                fillHistogram  (*histogram,  electrons.product    (),  electrons.product    ());
    else  if  (histogram->inputCollection  ==  "electron-event pairs")                   fillHistogram  (*histogram,  electrons.product    (),  events.product       ());
    else  if  (histogram->inputCollection  ==  "electron-jet pairs")                     fillHistogram  (*histogram,  electrons.product    (),  jets.product         ());
    else  if  (histogram->inputCollection  ==  "electron-mcparticle pairs")              fillHistogram  (*histogram,  electrons.product    (),  mcparticles.product  ());
    else  if  (histogram->inputCollection  ==  "electron-muon pairs")                    fillHistogram  (*histogram,  electrons.product    (),  muons.product        ());
    else  if  (histogram->inputCollection  ==  "electron-photon pairs")                  fillHistogram  (*histogram,  electrons.product    (),  photons.product      ());
    else  if  (histogram->inputCollection  ==  "electron-secondary electron pairs")      fillHistogram  (*histogram,  electrons.product    (),  electrons.product    ());
    else  if  (histogram->inputCollection  ==  "electron-secondary jet pairs")           fillHistogram  (*histogram,  electrons.product    (),  jets.product         ());
    else  if  (histogram->inputCollection  ==  "electron-track pairs")                   fillHistogram  (*histogram,  electrons.product    (),  tracks.product       ());
    else  if  (histogram->inputCollection  ==  "electron-trigobj pairs")                 fillHistogram  (*histogram,  electrons.product    (),  trigobjs.product     ());
    else  if  (histogram->inputCollection  ==  "jet-jet pairs")                          fillHistogram  (*histogram,  jets.product         (),  jets.product         ());
    else  if  (histogram->inputCollection  ==  "jet-mcparticle pairs")                   fillHistogram  (*histogram,  jets.product         (),  mcparticles.product  ());
    else  if  (histogram->inputCollection  ==  "jet-secondary jet pairs")                fillHistogram  (*histogram,  jets.product         (),  jets.product         ());
    else  if  (histogram->inputCollection  ==  "jet-secondary mcparticle pairs")         fillHistogram  (*histogram,  jets.product         (),  mcparticles.product  ());
    else  if  (histogram->inputCollection  ==  "jet-track pairs")                        fillHistogram  (*histogram,  jets.product         (),  tracks.product       ());
    else  if  (histogram->inputCollection  ==  "mcparticle-secondary mcparticle pairs")  fillHistogram  (*histogram,  mcparticles.product  (),  mcparticles.product  ());
    else  if  (histogram->inputCollection  ==  "met-jet pairs")                          fillHistogram  (*histogram,  mets.product         (),  jets.product         ());
    else  if  (histogram->inputCollection  ==  "met-mcparticle pairs")                   fillHistogram  (*histogram,  mets.product         (),  mcparticles.product  ());
    else  if  (histogram->inputCollection  ==  "muon-event pairs")                       fillHistogram  (*histogram,  muons.product        (),  events.product       ());
    else  if  (histogram->inputCollection  ==  "muon-jet pairs")                         fillHistogram  (*histogram,  muons.product        (),  jets.product         ());
    else  if  (histogram->inputCollection  ==  "muon-mcparticle pairs")                  fillHistogram  (*histogram,  muons.product        (),  mcparticles.product  ());
    else  if  (histogram->inputCollection  ==  "muon-muon pairs")                        fillHistogram  (*histogram,  muons.product        (),  muons.product        ());
    else  if  (histogram->inputCollection  ==  "muon-photon pairs")                      fillHistogram  (*histogram,  muons.product        (),  photons.product      ());
    else  if  (histogram->inputCollection  ==  "muon-secondary jet pairs")               fillHistogram  (*histogram,  muons.product        (),  jets.product         ());
    else  if  (histogram->inputCollection  ==  "muon-secondary mcparticle pairs")        fillHistogram  (*histogram,  muons.product        (),  mcparticles.product  ());
    else  if  (histogram->inputCollection  ==  "muon-secondary muon pairs")              fillHistogram  (*histogram,  muons.product        (),  secMuons.product     ());
    else  if  (histogram->inputCollection  ==  "muon-secondary photon pairs")            fillHistogram  (*histogram,  muons.product        (),  photons.product      ());
    else  if  (histogram->inputCollection  ==  "muon-tau pairs")                         fillHistogram  (*histogram,  muons.product        (),  taus.product         ());
    else  if  (histogram->inputCollection  ==  "muon-track pairs")                       fillHistogram  (*histogram,  muons.product        (),  tracks.product       ());
    else  if  (histogram->inputCollection  ==  "muon-trigobj pairs")                     fillHistogram  (*histogram,  muons.product        (),  trigobjs.product     ());
    else  if  (histogram->inputCollection  ==  "photon-jet pairs")                       fillHistogram  (*histogram,  photons.product      (),  jets.product         ());
    else  if  (histogram->inputCollection  ==  "photon-secondary jet pairs")             fillHistogram  (*histogram,  photons.product      (),  jets.product         ());
    else  if  (histogram->inputCollection  ==  "secondary electron-jet pairs")           fillHistogram  (*histogram,  electrons.product    (),  jets.product         ());
    else  if  (histogram->inputCollection  ==  "secondary muon-jet pairs")               fillHistogram  (*histogram,  muons.product        (),  jets.product         ());
    else  if  (histogram->inputCollection  ==  "secondary muon-track pairs")             fillHistogram  (*histogram,  secMuons.product     (),  tracks.product       ());
    else  if  (histogram->inputCollection  ==  "tau-tau pairs")                          fillHistogram  (*histogram,  taus.product         (),  taus.product         ());
    else  if  (histogram->inputCollection  ==  "tau-track pairs")                        fillHistogram  (*histogram,  taus.product         (),  tracks.product       ());
    else  if  (histogram->inputCollection  ==  "track-event pairs")                      fillHistogram  (*histogram,  tracks.product       (),  events.product       ());
    else  if  (histogram->inputCollection  ==  "track-jet pairs")                        fillHistogram  (*histogram,  tracks.product       (),  jets.product         ());
    else  if  (histogram->inputCollection  ==  "track-mcparticle pairs")                 fillHistogram  (*histogram,  tracks.product       (),  mcparticles.product  ());
  }

  firstEvent_ = false;
}

////////////////////////////////////////////////////////////////////////

Plotter::~Plotter ()
{
  //////////////////////////////////////////////////////////////////////////////
  // Destroy the ValueLookup object if it exists.
  //////////////////////////////////////////////////////////////////////////////
  if (vl_)
    delete vl_;
  //////////////////////////////////////////////////////////////////////////////
}

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

pair<string,string> Plotter::getVariableAndFunction(const string input){

  string inputVariable = "";
  string function = "";

  if(input.find("(")==string::npos){
    inputVariable = input;// variable to cut on                                                                                                                                                 
  }
  else{
    function = input.substr(0,input.find("("));//function comes before the "("                                                                                                          
    inputVariable = input.substr(input.find("(")+1);//get rest of string                                                                                                                
    inputVariable = inputVariable.substr(0,inputVariable.size()-1);//remove trailing ")"                                                                                                                
  }

  return make_pair(inputVariable,function);

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

// parses a histogram configuration and saves it in a C++ container
histoDef Plotter::parseHistoDef(const edm::ParameterSet definition, const string inputCollection, const string subDir){

  histoDef parsedDef;

  vector<double> defaults;
  defaults.push_back(-1.0);

  parsedDef.inputCollection = inputCollection;
  parsedDef.directory = subDir;
  parsedDef.name = definition.getParameter<string>("name");
  parsedDef.title = definition.getParameter<string>("title");
  parsedDef.binsX = definition.getUntrackedParameter<vector<double> >("binsX", defaults);
  parsedDef.binsY = definition.getUntrackedParameter<vector<double> >("binsY", defaults);
  parsedDef.hasVariableBinsX = parsedDef.binsX.size() > 3;
  parsedDef.hasVariableBinsY = parsedDef.binsY.size() > 3;
  parsedDef.inputVariables = definition.getParameter<vector<string> >("inputVariables");
  parsedDef.dimensions = parsedDef.inputVariables.size();

  // for 1D histograms, set the appropriate y-axis label
  parsedDef.title = setYaxisLabel(parsedDef);

  return parsedDef;

}

////////////////////////////////////////////////////////////////////////

// book TH1 or TH2 in appropriate directory with correct bin options
void Plotter::bookHistogram(const histoDef definition){

  // check for valid bins
  bool hasValidBinsX = definition.binsX.size() >= 3;
  bool hasValidBinsY = definition.binsY.size() >= 3 || (definition.binsY.size() == 1 && 
							definition.binsY.at(0) == -1);

  if(!hasValidBinsX || !hasValidBinsY){
    cout << "WARNING - invalid histogram bins" << endl;
    return;
  }

  TFileDirectory subdir = fs_->mkdir(definition.directory);


  // book 1D histogram
  if(definition.dimensions == 1){
    // equal X bins
    if(!definition.hasVariableBinsX){
      subdir.make<TH1D>(TString(definition.name),
			TString(definition.title),
			definition.binsX.at(0),
			definition.binsX.at(1),
			definition.binsX.at(2));
    }
    // variable X bins
    else{
      subdir.make<TH1D>(TString(definition.name),
			TString(definition.title),
			definition.binsX.size() - 1,
			definition.binsX.data());
    }
  }
  // book 2D histogram
  else if(definition.dimensions == 2){
    // equal X bins and equal Y bins
    if(!definition.hasVariableBinsX && !definition.hasVariableBinsY){
      subdir.make<TH2D>(TString(definition.name),
			TString(definition.title),
			definition.binsX.at(0),
			definition.binsX.at(1),
			definition.binsX.at(2),
			definition.binsY.at(0),
			definition.binsY.at(1),
			definition.binsY.at(2));
    }
    // variable X bins and equal Y bins
    else if(definition.hasVariableBinsX && !definition.hasVariableBinsY){
      subdir.make<TH2D>(TString(definition.name),
			TString(definition.title),
			definition.binsX.size() - 1,
			definition.binsX.data(),
			definition.binsY.at(0),
			definition.binsY.at(1),
			definition.binsY.at(2));
    }
    // equal X bins and variable Y bins
    else if(!definition.hasVariableBinsX && definition.hasVariableBinsY){
      subdir.make<TH2D>(TString(definition.name),
			TString(definition.title),
			definition.binsX.at(0),
			definition.binsX.at(1),
			definition.binsX.at(2),
			definition.binsY.size() - 1,
			definition.binsY.data());
    }
    // variable X bins and variable Y bins
    else if(definition.hasVariableBinsX && definition.hasVariableBinsY){
      subdir.make<TH2D>(TString(definition.name),
			TString(definition.title),
			definition.binsX.size() - 1,
			definition.binsX.data(),
			definition.binsY.size() - 1,
			definition.binsY.data());
    }
  }
  else{
    cout << "WARNING - invalid histogram dimension" << endl;
    return;
  }

}

////////////////////////////////////////////////////////////////////////

// fill TH1 or TH2 using one collection
template <class InputCollection> void Plotter::fillHistogram(const histoDef definition,
							     const InputCollection collection){

 if(definition.dimensions == 1){
   fill1DHistogram(definition, collection);
  }
  else if(definition.dimensions == 2){
    fill2DHistogram(definition, collection);
  }
  else{
    cout << "WARNING - Histogram dimension error" << endl;
  }

}

////////////////////////////////////////////////////////////////////////

// fill TH1 or TH2 using two collections
template <class InputCollection1, class InputCollection2> void Plotter::fillHistogram(const histoDef definition,
										      const InputCollection1 collection1, 
										      const InputCollection2 collection2){

  if(definition.dimensions == 1){
    fill1DHistogram(definition, collection1, collection2);
  }
  else if(definition.dimensions == 2){
    fill2DHistogram(definition, collection1, collection2);
  }
  else{
    cout << "WARNING - Histogram dimension error" << endl;
  }

}

////////////////////////////////////////////////////////////////////////

// fill TH1 using one collection
template <class InputCollection> void Plotter::fill1DHistogram(const histoDef definition,
							       const InputCollection collection){

  TH1D *histogram = fs_->getObject<TH1D>(definition.name, definition.directory);

  // loop over objets in input collection and fill histogram
  for(unsigned index = 0; index != collection->size(); index++){
    double value = definition.valueLookupTrees.at (0)->evaluate (collection->at(index));
    double weight = 1.0;
    if(definition.hasVariableBinsX){
      weight /= getBinSize(histogram,value);
    }
    histogram->Fill(value, weight);
  }
  
}

////////////////////////////////////////////////////////////////////////

// fill TH1 using two collections
template <class InputCollection1, class InputCollection2> void Plotter::fill1DHistogram(const histoDef definition,
											const InputCollection1 collection1,
											const InputCollection2 collection2){
  
  vector<string> objects = getInputTypes(definition.inputCollection);
  bool sameType = objects.at(0) == objects.at(1);

  TH1D *histogram = fs_->getObject<TH1D>(definition.name, definition.directory);

  // loop over objets in input collection and fill histogram
  for(unsigned index1 = 0; index1 != collection1->size(); index1++){
    for(unsigned index2 = 0; index2 != collection2->size(); index2++){

      //account for duplicate pairs if both collections are the same
      if(sameType && index1 >= index2) continue;

      double value = definition.valueLookupTrees.at (0)->evaluate (collection1->at(index1), collection2->at(index2));
      double weight = 1.0;
      if(definition.hasVariableBinsX){
	weight /= getBinSize(histogram,value);
      }
      histogram->Fill(value, weight);
    }
  }


}

////////////////////////////////////////////////////////////////////////

// fill TH2 using one collection
template <class InputCollection> void Plotter::fill2DHistogram(const histoDef definition,
							       const InputCollection collection){

  TH2D *histogram = fs_->getObject<TH2D>(definition.name, definition.directory);

  // loop over objets in input collection and fill histogram
  for(unsigned index = 0; index != collection->size(); index++){
    double valueX = definition.valueLookupTrees.at (0)->evaluate (collection->at(index));
    double valueY = definition.valueLookupTrees.at (1)->evaluate (collection->at(index));
    double weight = 1.0;
    if(definition.hasVariableBinsX){
      weight /= getBinSize(histogram,valueX, valueY).first;
    }
    if(definition.hasVariableBinsY){
      weight /= getBinSize(histogram,valueX, valueY).second;
    }
    histogram->Fill(valueX, valueY, weight);
  }

}

////////////////////////////////////////////////////////////////////////

// fill TH2 using two collections
template <class InputCollection1, class InputCollection2> void Plotter::fill2DHistogram(const histoDef definition,
											const InputCollection1 collection1,
											const InputCollection2 collection2){

  vector<string> objects = getInputTypes(definition.inputCollection);
  bool sameType = objects.at(0) == objects.at(1);

  TH2D *histogram = fs_->getObject<TH2D>(definition.name, definition.directory);

  // loop over objets in input collection and fill histogram
  for(unsigned index1 = 0; index1 != collection1->size(); index1++){
    for(unsigned index2 = 0; index2 != collection2->size(); index2++){

      //account for duplicate pairs if both collections are the same
      if(sameType && index1 >= index2) continue;

      double valueX = definition.valueLookupTrees.at (0)->evaluate (collection1->at(index1), collection2->at(index2));
      double valueY = definition.valueLookupTrees.at (1)->evaluate (collection1->at(index1), collection2->at(index2));
      double weight = 1.0;
      if(definition.hasVariableBinsX){
	weight /= getBinSize(histogram,valueX, valueY).first;
      }
      if(definition.hasVariableBinsY){
	weight /= getBinSize(histogram,valueX, valueY).second;
      }
      histogram->Fill(valueX, valueY, weight);

    }
  }

}

////////////////////////////////////////////////////////////////////////

double Plotter::getBinSize(TH1D *histogram,
			   const double value){
  
  int binIndex = histogram->FindBin(value);
  double binSize = histogram->GetBinWidth(binIndex);

  return binSize;

}

pair<double,double>  Plotter::getBinSize(TH2D *histogram,
					 const double valueX,
					 const double valueY){

  int binIndex = histogram->FindBin(valueX, valueY);
  double binSizeX = histogram->GetXaxis()->GetBinWidth(binIndex);
  double binSizeY = histogram->GetYaxis()->GetBinWidth(binIndex);
  
  return make_pair(binSizeX, binSizeY);

}

////////////////////////////////////////////////////////////////////////

string Plotter::setYaxisLabel(const histoDef definition){

  string title = definition.title;

  // this function is only valid for 1D histograms
  if(definition.dimensions > 1) return title;

  string binWidth = ""; // default in case there are variable bins
  string unit = "units"; // default if we don't find any unit

  // extract the x-axis unit (if there is one)

  std::size_t semiColonIndex = title.find_first_of(";");
  string axisLabels = title.substr(semiColonIndex+1);
  semiColonIndex = axisLabels.find_first_of(";");
  // if there is a second semicolon, the yaxis label is already defined
  if(semiColonIndex != string::npos) return title;
  std::size_t unitBeginIndex = axisLabels.find_first_of("[");
  std::size_t unitEndIndex   = axisLabels.find_first_of("]");
  std::size_t unitSize = unitEndIndex - unitBeginIndex;
  if(unitSize > 0){
    unit = axisLabels.substr(unitBeginIndex+1, unitSize-1);
  }

  // extract the bin width (if they are of fixed size)

  if(!definition.hasVariableBinsX){
    double range = definition.binsX.at(2) - definition.binsX.at(1);
    double width = range/definition.binsX.at(0);

    binWidth = to_string(width);
    // erase trailing zeros and decimal point
    if(binWidth.find('.') != string::npos){
      binWidth.erase(binWidth.find_last_not_of('0')+1, string::npos);
      if(binWidth.back() == '.'){
	binWidth.erase(binWidth.size()-1, string::npos);
      }
    }
    if(binWidth == "1"){
      binWidth = "";
      if(unit == "units"){
	unit = "unit";
      }
    }
    if(binWidth != ""){
      binWidth = binWidth + " ";
    }
  }

  // construct y-axis label from components

  string yAxisLabel = "Entries / " + binWidth + unit;
  if(definition.hasVariableBinsX){
    yAxisLabel = "< " + yAxisLabel + " >";
  }
  if(title.find_first_of(";") != string::npos){
    title = title + ";" + yAxisLabel;
  }
  else{
    title = title + ";;" + yAxisLabel;
  }
  return title;
}

bool
Plotter::initializeValueLookupTrees (vector<histoDef> &histograms)
{
  //////////////////////////////////////////////////////////////////////////////
  // Do nothing if it is not the first event.
  //////////////////////////////////////////////////////////////////////////////
  if (!firstEvent_)
    return true;
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // For each inputVariable of each histogram, parse it into a new
  // ValueLookupTree object which is stored in the histogram definition
  // structure.
  //////////////////////////////////////////////////////////////////////////////
  for (vector<histoDef>::iterator histogram = histograms.begin (); histogram != histograms.end (); histogram++)
    {
      for (vector<string>::const_iterator inputVariable = histogram->inputVariables.begin (); inputVariable != histogram->inputVariables.end (); inputVariable++)
        histogram->valueLookupTrees.push_back (new ValueLookupTree (*inputVariable, vl_));
      if (!histogram->valueLookupTrees.back ()->isValid ())
        return false;
    }
  return true;
  //////////////////////////////////////////////////////////////////////////////
}

void
Plotter::initializeValueLookup ()
{
  //////////////////////////////////////////////////////////////////////////////
  // Create a new ValueLookup object if it does not already exist.
  //////////////////////////////////////////////////////////////////////////////
  if (!vl_)
    vl_ = new ValueLookup ();
  //////////////////////////////////////////////////////////////////////////////
}

DEFINE_FWK_MODULE(Plotter);
