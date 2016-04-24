#ifndef TRIGGER_EFFICIENCY_ANALYZER
#include <map>
#include <string>
#include <vector>

#include "TH1D.h"
#include "TH2D.h"
#include "TString.h"
#include "TStopwatch.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Common/interface/Handle.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

using namespace std;

class TriggerEfficiencyAnalyzer : public edm::EDAnalyzer
  {
    public:
      TriggerEfficiencyAnalyzer (const edm::ParameterSet &);
      ~TriggerEfficiencyAnalyzer ();

      std::vector<string> TriggerTypes;
      std::map< string, std::vector<string> > TriggerNameMap;
      std::map< string, TH1D* > TriggerHistogramMap;
      std::map< string, TH1D* > TriggerHistEffMap;
      std::map< string, bool > InclusiveORMap;

      void analyze (const edm::Event &, const edm::EventSetup &);
      const edm::Service<TFileService> fs;

    private:
      edm::Handle<edm::TriggerResults> TriggerCollection;
      edm::InputTag Trigger_;
      edm::EDGetTokenT<edm::TriggerResults> TriggerToken_;
      vector<edm::ParameterSet> triggers_;
      TStopwatch* timer;

  };

#endif
