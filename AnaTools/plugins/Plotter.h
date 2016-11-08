#ifndef PLOTTER
#define PLOTTER

#include <tuple>
#include <unordered_set>

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

#include "TH1.h"
#include "TH2.h"
#include "TH3.h"

class Plotter : public edm::EDAnalyzer
{
    public:

      Plotter (const edm::ParameterSet &);
      ~Plotter ();
      void analyze(const edm::Event&, const edm::EventSetup&);

    private:

      edm::ParameterSet collections_;
      vector<edm::ParameterSet> weightDefs_;
      vector<edm::ParameterSet> histogramSets_;
      int verbose_;
      bool firstEvent_;

      //Collections
      Collections handles_;
      Tokens tokens_;

      bool initializeValueLookupForest (vector<HistoDef> &, Collections * const);
      bool initializeValueLookupForest (vector<Weight> &, Collections * const);

      edm::Service<TFileService> fs_;

      unordered_set<string> objectsToGet_;

      vector<TFileDirectory> subDirs;

      vector<HistoDef> histogramDefinitions;

      vector<Weight> weights;

      string getDirectoryName(const string);
      HistoDef parseHistoDef(const edm::ParameterSet &, const vector<string> &, const string &, const string &);
      void bookHistogram(const HistoDef &);

      void fillHistogram(const HistoDef &);
      void fill1DHistogram(const HistoDef &);
      void fill2DHistogram(const HistoDef &);
      void fill2DHistogram(const HistoDef & definition, double valueX, double valueY, double weight);
      void fill3DHistogram(const HistoDef &);
      void fill3DHistogram(const HistoDef & definition, double valueX, double valueY, double valueZ, double weight);

      double getBinSize(TH1D * const, const double);
      pair<double,double> getBinSize(TH2D * const, const double, const double);
      tuple<double,double,double>  getBinSize(TH3D * const, const double, const double, const double);
      string setYaxisLabel(const HistoDef &);



};

#endif




