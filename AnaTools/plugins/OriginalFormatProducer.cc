// -*- C++ -*-
//
// Package:    OSUT3Analysis/AnaTools 
// Class:      OriginalFormatProducer
//
/**\class OriginalFormatProducer OriginalFormatProducer.cc OSUT3Analysis/AnaTools/plugins/OriginalFormatProducer.cc

 Description: Casts osu::TYPE collections into new collections in the original format.  
   Currently only implemented for MINIAOD, but could be generalized to allow for RECO or AOD.  

 Implementation:
   Typically run this producer on a skim file produced by the OSU analysis framework. 


*/
//
// Original Author:  Wells Wulsin
//         Created:  22 Dec 2015 
//
//

// user include files
#include <typeinfo>  
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h" 



using namespace std;

//
// class declaration
//

class OriginalFormatProducer : public edm::EDProducer {
public:
  explicit OriginalFormatProducer (const edm::ParameterSet&);
  ~OriginalFormatProducer ();

private:
  virtual void produce (edm::Event&, const edm::EventSetup&) override;
  template<class Old, class New> void produceCollection (edm::Event& iEvent);  

  // ----------member data ---------------------------
  bool makeBeamspots_;  
  bool makeElectrons_;  
  bool makeGenjets_;  
  bool makeJets_; 
  bool makeMcparticles_; 
  bool makeMets_;  
  bool makeMuons_;  
  bool makePhotons_; 
  bool makePileUpInfos_; 
  bool makePrimaryvertexs_; 
  bool makeTaus_; 
  bool makeTracks_;  
  bool makeTrigobjs_; 
  bool makeSuperclusters_;

};


//
// constructors and destructor
//
OriginalFormatProducer::OriginalFormatProducer (const edm::ParameterSet& iConfig) 
{
  makeBeamspots_       = iConfig.getParameter<bool> ("makeBeamspots");        
  makeElectrons_       = iConfig.getParameter<bool> ("makeElectrons");        
  makeGenjets_         = iConfig.getParameter<bool> ("makeGenjets");   	     
  makeJets_ 	       = iConfig.getParameter<bool> ("makeJets");  	     
  makeMcparticles_     = iConfig.getParameter<bool> ("makeMcparticles");      
  makeMets_  	       = iConfig.getParameter<bool> ("makeMets");   	     
  makeMuons_  	       = iConfig.getParameter<bool> ("makeMuons");   	     
  makePhotons_ 	       = iConfig.getParameter<bool> ("makePhotons");  	     
  makePileUpInfos_     = iConfig.getParameter<bool> ("makePileUpInfos");      
  makePrimaryvertexs_  = iConfig.getParameter<bool> ("makePrimaryvertexs");   
  makeTaus_ 	       = iConfig.getParameter<bool> ("makeTaus");  	     
  makeTracks_  	       = iConfig.getParameter<bool> ("makeTracks");   	     
  makeTrigobjs_        = iConfig.getParameter<bool> ("makeTrigobjs");  	     
  makeSuperclusters_   = iConfig.getParameter<bool> ("makeSuperclusters");    


  if (makeBeamspots_)       produces<vector< reco::BeamSpot > > (); 
  if (makeElectrons_)       produces<vector< pat::Electron > > (); 
  if (makeGenjets_)         produces<vector< reco::GenJet > > (); 
  if (makeJets_)            produces<vector< pat::Jet > > (); 
  if (makeMcparticles_)     produces<vector< reco::GenParticle > > (); 
  if (makeMets_)   	    produces<vector< pat::MET > > (); 
  if (makeMuons_)  	    produces<vector< pat::Muon > > (); 
  if (makePhotons_) 	    produces<vector< pat::Photon > > (); 
  if (makePileUpInfos_)     produces<vector< PileupSummaryInfo > > (); 
  if (makePrimaryvertexs_)  produces<vector< reco::Vertex > > (); 
  if (makeTaus_) 	    produces<vector< pat::Tau > > (); 
  if (makeTracks_)  	    produces<vector< reco::Track > > (); 
  if (makeTrigobjs_)        produces<vector< pat::TriggerObjectStandAlone > > (); 
  if (makeSuperclusters_)   produces<vector< reco::SuperCluster > > (); 

}

OriginalFormatProducer::~OriginalFormatProducer ()
{
}

//
// member functions
//

template<typename Old, typename New> void
OriginalFormatProducer::produceCollection (edm::Event& iEvent) 
{ 
  vector<edm::Handle< vector < Old > > > oldObjVec;
  iEvent.getManyByType(oldObjVec);
  if (oldObjVec.size() == 0) {
    return;  
  } else if (oldObjVec.size() > 1) {
    cout << "ERROR:  size of objVec = " << oldObjVec.size() << ", only valid size is 1." << endl;  
    cout << "Will skip conversion to " << typeid(New).name() << endl;  
    return; 
  }
  // At this point we are sure that oldObjVec.size() == 1.  
  edm::Handle<vector<Old> > oldObjs = oldObjVec.at(0);  

  auto_ptr<vector< New > > newObjs (new vector< New > ());
  for (const auto &oldObj : *oldObjs) {
    New newObj(oldObj);  
    newObjs->push_back(newObj);
  }
  iEvent.put (newObjs);
  
}


// ------------ method called to produce the data  ------------
void
OriginalFormatProducer::produce (edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  //  if (makeBeamspots_)       produceCollection<osu::Beamspot,      reco::BeamSpot>               (iEvent); Doesn't work currently.  
  if (makeElectrons_)       produceCollection<osu::Electron,      pat::Electron>                (iEvent); 
  if (makeGenjets_)         produceCollection<osu::Genjet,        reco::GenJet>                 (iEvent); 
  if (makeJets_)            produceCollection<osu::Jet,           pat::Jet>                     (iEvent); 
  if (makeMcparticles_)     produceCollection<osu::Mcparticle,    pat::PackedGenParticle>       (iEvent); 
  if (makeMets_)   	    produceCollection<osu::Met,           pat::MET>                     (iEvent); 
  if (makeMuons_)  	    produceCollection<osu::Muon,          pat::Muon>                    (iEvent); 
  if (makePhotons_) 	    produceCollection<osu::Photon,        pat::Photon>                  (iEvent); 
  //  if (makePileUpInfos_)     produceCollection<osu::PileUpInfo,    PileupSummaryInfo>            (iEvent); // Doesn't work currently.  
  if (makePrimaryvertexs_)  produceCollection<osu::Primaryvertex, reco::Vertex>                 (iEvent); 
  if (makeTaus_) 	    produceCollection<osu::Tau,           pat::Tau>                     (iEvent); 
  if (makeTracks_)  	    produceCollection<osu::Track,         reco::Track>                  (iEvent); 
  if (makeTrigobjs_)        produceCollection<osu::Trigobj,       pat::TriggerObjectStandAlone> (iEvent); 
  if (makeSuperclusters_)   produceCollection<osu::Supercluster,  reco::SuperCluster>           (iEvent); 

} 

//define this as a plug-in
DEFINE_FWK_MODULE (OriginalFormatProducer);



