#ifndef PUSCALING_FACTOR_PRODUCER
#define PUSCALING_FACTOR_PRODUCER

#include "OSUT3Analysis/AnaTools/interface/EventVariableProducer.h"
#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"
#include "DataFormats/Math/interface/deltaR.h"
#include <string>
#include "TH2D.h"
#include "TH2F.h"
#include "TGraphAsymmErrors.h"
#include "TFile.h"

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

class ObjectScalingFactorProducer : public EventVariableProducer
  {
    public:
        ObjectScalingFactorProducer (const edm::ParameterSet &);
        ~ObjectScalingFactorProducer ();
        Collections handles_;
        Tokens tokens_;

    private:
        string muonFile_;
        string electronFile_;
        string trackFile_;
        string electronWp_;
        string muonWp_;
        bool doEleSF_;
        bool doMuSF_;
        bool doTrackSF_;
        void AddVariables(const edm::Event &);
              vector<ScaleFactor> scaleFactors_;

};
#endif
