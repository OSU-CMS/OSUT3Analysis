#ifndef PRIMARY_VTX_VAR_PRODUCER
#define PRIMARY_VTX_VAR_PRODUCER

#include "OSUT3Analysis/AnaTools/interface/EventVariableProducer.h"
#include "TLorentzVector.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

class PrimaryVtxVarProducer : public EventVariableProducer
   {
    public:
        PrimaryVtxVarProducer (const edm::ParameterSet &);
        ~PrimaryVtxVarProducer ();

    private:
        void AddVariables(const edm::Event &, const edm::EventSetup &);
        edm::EDGetTokenT<vector<TYPE(primaryvertexs)> > token_;
  };

#endif
