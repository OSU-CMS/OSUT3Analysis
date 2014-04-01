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

#include "BEAN/Collections/interface/BNelectron.h"
#include "BEAN/Collections/interface/BNevent.h"
#include "BEAN/Collections/interface/BNjet.h"
#include "BEAN/Collections/interface/BNgenjet.h"
#include "BEAN/Collections/interface/BNmcparticle.h"
#include "BEAN/Collections/interface/BNmet.h"
#include "BEAN/Collections/interface/BNmuon.h"
#include "BEAN/Collections/interface/BNphoton.h"
#include "BEAN/Collections/interface/BNsupercluster.h"
#include "BEAN/Collections/interface/BNtau.h"
#include "BEAN/Collections/interface/BNtrack.h"
#include "BEAN/Collections/interface/BNtrigger.h"
#include "BEAN/Collections/interface/BNbxlumi.h"
#include "BEAN/Collections/interface/BNtrigobj.h"
#include "BEAN/Collections/interface/BNprimaryvertex.h"

#include "OSUT3Analysis/AnaTools/interface/BNstop.h"

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
