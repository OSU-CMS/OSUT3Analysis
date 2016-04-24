#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"
#include "OSUT3Analysis/AnaTools/plugins/PrimaryVtxVarProducer.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

PrimaryVtxVarProducer::PrimaryVtxVarProducer(const edm::ParameterSet &cfg) :
  EventVariableProducer(cfg)
{
  token_ = consumes<vector<TYPE(primaryvertexs)> > (collections_.getParameter<edm::InputTag> ("primaryvertexs"));
}

PrimaryVtxVarProducer::~PrimaryVtxVarProducer() {}

void
PrimaryVtxVarProducer::AddVariables (const edm::Event &event) {

  edm::Handle<vector<TYPE(primaryvertexs)> > primaryvertexs;
  if (!event.getByToken (token_, primaryvertexs)) {
    cout << "ERROR:  Could not find primaryvertexs collection." << endl; 
    return;
  } 

  int numPVReco = primaryvertexs->size(); 
  reco::Vertex pv = primaryvertexs->at(0); 
  double leadingPV_x = pv.x(); 
  double leadingPV_y = pv.y();
  double leadingPV_z = pv.z(); 

  (*eventvariables)["numPVReco"]       = numPVReco;
  (*eventvariables)["leadingPV_x"]     = leadingPV_x;
  (*eventvariables)["leadingPV_y"]     = leadingPV_y;
  (*eventvariables)["leadingPV_z"]     = leadingPV_z;

}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PrimaryVtxVarProducer);
