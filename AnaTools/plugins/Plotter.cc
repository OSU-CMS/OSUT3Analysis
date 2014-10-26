#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
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
  jets_ (cfg.getParameter<edm::InputTag> ("jets")),
  muons_ (cfg.getParameter<edm::InputTag> ("muons")),
  electrons_ (cfg.getParameter<edm::InputTag> ("electrons")),
  taus_ (cfg.getParameter<edm::InputTag> ("taus")),
  mets_ (cfg.getParameter<edm::InputTag> ("mets")),
  genjets_ (cfg.getParameter<edm::InputTag> ("genjets")),
  mcparticles_ (cfg.getParameter<edm::InputTag> ("mcparticles")),
  primaryvertexs_ (cfg.getParameter<edm::InputTag> ("primaryvertexs")),
  photons_ (cfg.getParameter<edm::InputTag> ("photons")),
  triggers_ (cfg.getParameter<edm::InputTag> ("triggers")),

  histogramSets_ (cfg.getParameter<vector<edm::ParameterSet> >("histogramSets")),
  verbose_ (cfg.getParameter<int> ("verbose"))

{
  
  if (verbose_) clog << "Beginning Plotter::Plotter constructor." << endl;

  TH1::SetDefaultSumw2();

  /////////////////////////////////////
  // parse the histogram definitions //
  /////////////////////////////////////
  
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

  if (find(objectsToGet.begin(), objectsToGet.end(), "triggers") != objectsToGet.end()) {
    event.getByLabel (triggers_, triggers);
    if (!triggers.product()) clog << "ERROR: could not get triggers input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "jets") != objectsToGet.end()) {
    event.getByLabel (jets_, jets);
    if (!jets.product()) clog << "ERROR: could not get jets input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "muons") != objectsToGet.end()) {
    event.getByLabel (muons_, muons);
    if (!muons.product()) clog << "ERROR: could not get muons input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "electrons") != objectsToGet.end()) {
    event.getByLabel (electrons_, electrons);
    if (!electrons.product()) clog << "ERROR: could not get electrons input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "taus") != objectsToGet.end()) {
    event.getByLabel (taus_, taus);
    if (!taus.product()) clog << "ERROR: could not get taus input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "mets") != objectsToGet.end()) {
    event.getByLabel (mets_, mets);
    if (!mets.product()) clog << "ERROR: could not get mets input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "genjets") != objectsToGet.end()) {
    event.getByLabel (genjets_, genjets);
    if (!genjets.product()) clog << "ERROR: could not get genjets input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "mcparticles") != objectsToGet.end()) {
    event.getByLabel (mcparticles_, mcparticles);
    if (!mcparticles.product()) clog << "ERROR: could not get mcparticles input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "primaryvertexs") != objectsToGet.end()) {
    event.getByLabel (primaryvertexs_, primaryvertexs);
    if (!primaryvertexs.product()) clog << "ERROR: could not get primaryvertexs input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "photons") != objectsToGet.end()) {
    event.getByLabel (photons_, photons);
    if (!photons.product()) clog << "ERROR: could not get photons input collection" << endl;
  }
  if (find(objectsToGet.begin(), objectsToGet.end(), "userVariables") != objectsToGet.end()) {
    event.getByLabel ("UserVariableProduction", "userVariables", userVariables);
    if (!userVariables.product()) clog << "ERROR: could not get userVariables input collection" << endl;
  }
 
  // now that we have all the required objects, we'll loop over the histograms, filling each one as we go

  vector<histoDef>::iterator histogram;
  for(histogram = histogramDefinitions.begin(); histogram != histogramDefinitions.end(); ++histogram){

    if (histogram->inputCollection == "userVariables") {
      fillHistogram(*histogram, userVariables.product());
    } 
    else if (histogram->inputCollection == "mets") {
      fillHistogram(*histogram, mets.product());
    }
    else if(histogram->inputCollection == "muons"){
      fillHistogram(*histogram, muons.product());
    }
    else if(histogram->inputCollection == "muon-muon pairs"){
      fillHistogram(*histogram, muons.product(), muons.product());
    }
    else if(histogram->inputCollection == "electron-muon pairs"){
      fillHistogram(*histogram, electrons.product(), muons.product());
    }
  }

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

  // collect information for filling                                                                                                                                                                      
  pair<string,string> variableAndFunction = getVariableAndFunction(definition.inputVariables.at(0));
  string variable = variableAndFunction.first;
  string function = variableAndFunction.second;

  string dummy = "";

  TH1D *histogram = fs_->getObject<TH1D>(definition.name, definition.directory);

  // loop over objets in input collection and fill histogram
  for(uint index = 0; index != collection->size(); index++){
    double value = vl_->valueLookup(&collection->at(index), variable, function, dummy);
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
  
  // collect information for filling

  pair<string,string> variableAndFunction = getVariableAndFunction(definition.inputVariables.at(0));
  string variable = variableAndFunction.first;
  string function = variableAndFunction.second;

  string dummy = "";

  vector<string> objects = getInputTypes(definition.inputCollection);
  bool sameType = objects.at(0) == objects.at(1);

  TH1D *histogram = fs_->getObject<TH1D>(definition.name, definition.directory);

  // loop over objets in input collection and fill histogram
  for(uint index1 = 0; index1 != collection1->size(); index1++){
    for(uint index2 = 0; index2 != collection2->size(); index2++){

      //account for duplicate pairs if both collections are the same
      if(sameType && index1 >= index2) continue;

      double value = vl_->valueLookup(&collection1->at(index1), &collection2->at(index2), variable, function, dummy);
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

  // collect information for filling

  pair<string,string> variableAndFunctionX = getVariableAndFunction(definition.inputVariables.at(0));
  string variableX = variableAndFunctionX.first;
  string functionX = variableAndFunctionX.second;

  pair<string,string> variableAndFunctionY = getVariableAndFunction(definition.inputVariables.at(1));
  string variableY = variableAndFunctionY.first;
  string functionY = variableAndFunctionY.second;

  string dummy = "";

  TH2D *histogram = fs_->getObject<TH2D>(definition.name, definition.directory);

  // loop over objets in input collection and fill histogram
  for(uint index = 0; index != collection->size(); index++){
    double valueX = vl_->valueLookup(&collection->at(index), variableX, functionX, dummy);
    double valueY = vl_->valueLookup(&collection->at(index), variableY, functionY, dummy);
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

  // collect information for filling

  pair<string,string> variableAndFunctionX = getVariableAndFunction(definition.inputVariables.at(0));
  string variableX = variableAndFunctionX.first;
  string functionX = variableAndFunctionX.second;

  pair<string,string> variableAndFunctionY = getVariableAndFunction(definition.inputVariables.at(1));
  string variableY = variableAndFunctionY.first;
  string functionY = variableAndFunctionY.second;

  string dummy = "";

  vector<string> objects = getInputTypes(definition.inputCollection);
  bool sameType = objects.at(0) == objects.at(1);

  TH2D *histogram = fs_->getObject<TH2D>(definition.name, definition.directory);

  // loop over objets in input collection and fill histogram
  for(uint index1 = 0; index1 != collection1->size(); index1++){
    for(uint index2 = 0; index2 != collection2->size(); index2++){

      //account for duplicate pairs if both collections are the same
      if(sameType && index1 >= index2) continue;

      double valueX = vl_->valueLookup(&collection1->at(index1), &collection2->at(index2), variableX, functionX, dummy);
      double valueY = vl_->valueLookup(&collection1->at(index1), &collection2->at(index2), variableY, functionY, dummy);
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


DEFINE_FWK_MODULE(Plotter);
