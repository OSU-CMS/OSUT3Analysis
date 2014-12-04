#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"
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

  firstEvent_ (true)

{

  if (verbose_) clog << "Beginning Plotter::Plotter constructor." << endl;

  TH1::SetDefaultSumw2();

  /////////////////////////////////////
  // parse the histogram definitions //
  /////////////////////////////////////

  // loop over each histogram set the user has included
  for(unsigned histoSet = 0; histoSet != histogramSets_.size(); histoSet++){

    vector<string> inputCollection = histogramSets_.at(histoSet).getParameter<vector<string> > ("inputCollection");
    sort (inputCollection.begin (), inputCollection.end ());
    string catInputCollection = concatenateInputCollection (inputCollection);

    objectsToGet_.insert (objectsToGet_.end (), inputCollection.begin (), inputCollection.end ());

    // get the appropriate directory name
    string directoryName = getDirectoryName(catInputCollection);

    // import all the histogram definitions for the current set
    vector<edm::ParameterSet> histogramList (histogramSets_.at(histoSet).getParameter<vector<edm::ParameterSet> >("histograms"));

    // loop over each histogram
    vector<edm::ParameterSet>::const_iterator histogram;
    for(histogram = histogramList.begin(); histogram != histogramList.end(); ++histogram){

      // parse the definition and save the relevant info
      HistoDef histoDefinition = parseHistoDef(*histogram,inputCollection,catInputCollection,directoryName);
      histogramDefinitions.push_back(histoDefinition);

    } // end loop on histograms in the set

  } // end loop on histogram sets

  sort (objectsToGet_.begin (), objectsToGet_.end ());
  objectsToGet_.erase (unique (objectsToGet_.begin (), objectsToGet_.end ()), objectsToGet_.end ());

  // loop over each parsed histogram configuration
  vector<HistoDef>::iterator histogram;
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
  if  (VEC_CONTAINS  (objectsToGet_,  "bxlumis")         &&  collections_.exists  ("bxlumis"))         getCollection  (collections_.getParameter<edm::InputTag>  ("bxlumis"),         handles_.bxlumis,         event);
  if  (VEC_CONTAINS  (objectsToGet_,  "electrons")       &&  collections_.exists  ("electrons"))       getCollection  (collections_.getParameter<edm::InputTag>  ("electrons"),       handles_.electrons,       event);
  if  (VEC_CONTAINS  (objectsToGet_,  "events")          &&  collections_.exists  ("events"))          getCollection  (collections_.getParameter<edm::InputTag>  ("events"),          handles_.events,          event);
  if  (VEC_CONTAINS  (objectsToGet_,  "genjets")         &&  collections_.exists  ("genjets"))         getCollection  (collections_.getParameter<edm::InputTag>  ("genjets"),         handles_.genjets,         event);
  if  (VEC_CONTAINS  (objectsToGet_,  "jets")            &&  collections_.exists  ("jets"))            getCollection  (collections_.getParameter<edm::InputTag>  ("jets"),            handles_.jets,            event);
  if  (VEC_CONTAINS  (objectsToGet_,  "mcparticles")     &&  collections_.exists  ("mcparticles"))     getCollection  (collections_.getParameter<edm::InputTag>  ("mcparticles"),     handles_.mcparticles,     event);
  if  (VEC_CONTAINS  (objectsToGet_,  "mets")            &&  collections_.exists  ("mets"))            getCollection  (collections_.getParameter<edm::InputTag>  ("mets"),            handles_.mets,            event);
  if  (VEC_CONTAINS  (objectsToGet_,  "muons")           &&  collections_.exists  ("muons"))           getCollection  (collections_.getParameter<edm::InputTag>  ("muons"),           handles_.muons,           event);
  if  (VEC_CONTAINS  (objectsToGet_,  "photons")         &&  collections_.exists  ("photons"))         getCollection  (collections_.getParameter<edm::InputTag>  ("photons"),         handles_.photons,         event);
  if  (VEC_CONTAINS  (objectsToGet_,  "primaryvertexs")  &&  collections_.exists  ("primaryvertexs"))  getCollection  (collections_.getParameter<edm::InputTag>  ("primaryvertexs"),  handles_.primaryvertexs,  event);
  if  (VEC_CONTAINS  (objectsToGet_,  "superclusters")   &&  collections_.exists  ("superclusters"))   getCollection  (collections_.getParameter<edm::InputTag>  ("superclusters"),   handles_.superclusters,   event);
  if  (VEC_CONTAINS  (objectsToGet_,  "taus")            &&  collections_.exists  ("taus"))            getCollection  (collections_.getParameter<edm::InputTag>  ("taus"),            handles_.taus,            event);
  if  (VEC_CONTAINS  (objectsToGet_,  "tracks")          &&  collections_.exists  ("tracks"))          getCollection  (collections_.getParameter<edm::InputTag>  ("tracks"),          handles_.tracks,          event);
  if  (VEC_CONTAINS  (objectsToGet_,  "triggers")        &&  collections_.exists  ("triggers"))        getCollection  (collections_.getParameter<edm::InputTag>  ("triggers"),        handles_.triggers,        event);
  if  (VEC_CONTAINS  (objectsToGet_,  "trigobjs")        &&  collections_.exists  ("trigobjs"))        getCollection  (collections_.getParameter<edm::InputTag>  ("trigobjs"),        handles_.trigobjs,        event);
  if  (VEC_CONTAINS  (objectsToGet_,  "userVariables")   &&  collections_.exists  ("userVariables"))   getCollection  (collections_.getParameter<edm::InputTag>  ("userVariables"),   handles_.userVariables,   event);

  if (!initializeValueLookupForest (histogramDefinitions, &handles_))
    {
      clog << "ERROR: failed to parse input variables. Quitting..." << endl;
      exit (EXIT_CODE);
    }

  // now that we have all the required objects, we'll loop over the histograms, filling each one as we go

  vector<HistoDef>::iterator histogram;
  for(histogram = histogramDefinitions.begin(); histogram != histogramDefinitions.end(); ++histogram)
    fillHistogram (*histogram);

  firstEvent_ = false;
}

////////////////////////////////////////////////////////////////////////

Plotter::~Plotter ()
{
}

////////////////////////////////////////////////////////////////////////

// function to convert an input collection into a directory name
string Plotter::getDirectoryName(string inputName){

  string parsedName = capitalize(singular(inputName));

  parsedName += " Plots";

  return parsedName;

}

////////////////////////////////////////////////////////////////////////

// parses a histogram configuration and saves it in a C++ container
HistoDef Plotter::parseHistoDef(const edm::ParameterSet &definition, const vector<string> &inputCollection, const string &catInputCollection, const string &subDir){

  HistoDef parsedDef;

  vector<double> defaults;
  defaults.push_back(-1.0);

  parsedDef.inputLabel = catInputCollection;
  parsedDef.inputCollections = inputCollection;
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
void Plotter::bookHistogram(const HistoDef definition){

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
void Plotter::fillHistogram(const HistoDef &definition){

 if(definition.dimensions == 1){
   fill1DHistogram(definition);
  }
  else if(definition.dimensions == 2){
    fill2DHistogram(definition);
  }
  else{
    cout << "WARNING - Histogram dimension error" << endl;
  }

}

////////////////////////////////////////////////////////////////////////

// fill TH1 using one collection
void Plotter::fill1DHistogram(const HistoDef &definition){

  TH1D *histogram = fs_->getObject<TH1D>(definition.name, definition.directory);

  // loop over objects in input collection and fill histogram
  for(vector<Leaf>::const_iterator value = definition.valueLookupTrees.at (0)->evaluate ().begin (); value != definition.valueLookupTrees.at (0)->evaluate ().end (); value++){
    double weight = 1.0;
    if(definition.hasVariableBinsX){
      weight /= getBinSize(histogram,boost::get<double> (*value));
    }
    histogram->Fill(boost::get<double> (*value), weight);
  }

}

////////////////////////////////////////////////////////////////////////

// fill TH2 using one collection
void Plotter::fill2DHistogram(const HistoDef &definition){

  TH2D *histogram = fs_->getObject<TH2D>(definition.name, definition.directory);

  // loop over objets in input collection and fill histogram
  for(vector<Leaf>::const_iterator valueX = definition.valueLookupTrees.at (0)->evaluate ().begin (); valueX != definition.valueLookupTrees.at (0)->evaluate ().end (); valueX++){
    for(vector<Leaf>::const_iterator valueY = definition.valueLookupTrees.at (1)->evaluate ().begin (); valueY != definition.valueLookupTrees.at (1)->evaluate ().end (); valueY++){
      double weight = 1.0;
      if(definition.hasVariableBinsX){
        weight /= getBinSize(histogram,boost::get<double> (*valueX), boost::get<double> (*valueY)).first;
      }
      if(definition.hasVariableBinsY){
        weight /= getBinSize(histogram,boost::get<double> (*valueX), boost::get<double> (*valueY)).second;
      }
      histogram->Fill(boost::get<double> (*valueX), boost::get<double> (*valueY), weight);
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

string Plotter::setYaxisLabel(const HistoDef definition){

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
Plotter::initializeValueLookupForest (vector<HistoDef> &histograms, Collections *handles)
{
  //////////////////////////////////////////////////////////////////////////////
  // For each inputVariable of each histogram, parse it into a new
  // ValueLookupTree object which is stored in the histogram definition
  // structure.
  //////////////////////////////////////////////////////////////////////////////
  for (vector<HistoDef>::iterator histogram = histograms.begin (); histogram != histograms.end (); histogram++)
    {
      if (firstEvent_)
        {
          for (vector<string>::const_iterator inputVariable = histogram->inputVariables.begin (); inputVariable != histogram->inputVariables.end (); inputVariable++)
            histogram->valueLookupTrees.push_back (new ValueLookupTree (*inputVariable, histogram->inputCollections));
          if (!histogram->valueLookupTrees.back ()->isValid ())
            return false;
        }
      for (vector<ValueLookupTree *>::iterator tree = histogram->valueLookupTrees.begin (); tree != histogram->valueLookupTrees.end (); tree++)
        (*tree)->setCollections (handles);
    }
  return true;
  //////////////////////////////////////////////////////////////////////////////
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(Plotter);
