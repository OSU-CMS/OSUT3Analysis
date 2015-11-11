#ifndef PU_ANALYZER

#define PU_ANALYZER

#include <string>
#include <vector>
#include <map>

#include "TH1D.h"
#include "TROOT.h"
#include "TStyle.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Common/interface/EventBase.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

class PUAnalyzer : public edm::EDAnalyzer
{
 public:
  PUAnalyzer (const edm::ParameterSet &);
  virtual ~PUAnalyzer ();
  void beginJob () {};
  void analyze (const edm::Event &, const edm::EventSetup &);
  void endJob () {};

 private:
  edm::InputTag pileUpInfo_;

  std::map<std::string, TH1D*> oneDHists_;
  edm::Service<TFileService> fs_;

};

#endif
