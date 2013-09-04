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

#include "ProductArea/BNcollections/interface/BNelectron.h"
#include "ProductArea/BNcollections/interface/BNevent.h"
#include "ProductArea/BNcollections/interface/BNjet.h"
#include "ProductArea/BNcollections/interface/BNgenjet.h"
#include "ProductArea/BNcollections/interface/BNmcparticle.h"
#include "ProductArea/BNcollections/interface/BNmet.h"
#include "ProductArea/BNcollections/interface/BNmuon.h"
#include "ProductArea/BNcollections/interface/BNphoton.h"
#include "ProductArea/BNcollections/interface/BNsupercluster.h"
#include "ProductArea/BNcollections/interface/BNtau.h"
#include "ProductArea/BNcollections/interface/BNtrack.h"
#include "ProductArea/BNcollections/interface/BNtrigger.h"
#include "ProductArea/BNcollections/interface/BNbxlumi.h"
#include "ProductArea/BNcollections/interface/BNtrigobj.h"
#include "ProductArea/BNcollections/interface/BNprimaryvertex.h"

#include "OSUT3Analysis/AnaTools/interface/CutFlow.h"
#include "OSUT3Analysis/AnaTools/interface/PUWeight.h"

class PUAnalyzer : public edm::EDAnalyzer
{
 public:
  PUAnalyzer (const edm::ParameterSet &);
  virtual ~PUAnalyzer ();
  void beginJob () {};
  void analyze (const edm::Event &, const edm::EventSetup &);
  void endJob () {};

 private:
  edm::InputTag events_;

  CutFlow *masterCutFlow_;

  std::map<std::string, TH1D*> oneDHists_;
  edm::Service<TFileService> fs_;

};

#endif
