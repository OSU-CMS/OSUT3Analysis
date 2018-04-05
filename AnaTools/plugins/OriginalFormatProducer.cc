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
  bool makeSecondaryTracks_;
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
  makeJets_            = iConfig.getParameter<bool> ("makeJets");
  makeMcparticles_     = iConfig.getParameter<bool> ("makeMcparticles");
  makeMets_            = iConfig.getParameter<bool> ("makeMets");
  makeMuons_           = iConfig.getParameter<bool> ("makeMuons");
  makePhotons_         = iConfig.getParameter<bool> ("makePhotons");
  makePileUpInfos_     = iConfig.getParameter<bool> ("makePileUpInfos");
  makePrimaryvertexs_  = iConfig.getParameter<bool> ("makePrimaryvertexs");
  makeTaus_            = iConfig.getParameter<bool> ("makeTaus");
  makeTracks_          = iConfig.getParameter<bool> ("makeTracks");
  makeSecondaryTracks_ = iConfig.getParameter<bool> ("makeSecondaryTracks");
  makeTrigobjs_        = iConfig.getParameter<bool> ("makeTrigobjs");
  makeSuperclusters_   = iConfig.getParameter<bool> ("makeSuperclusters");


  if (makeBeamspots_)       produces<vector< TYPE(beamspots) > > ();
  if (makeElectrons_)       produces<vector< TYPE(electrons) > > ();
  if (makeGenjets_)         produces<vector< TYPE(genjets) > > ();
  if (makeJets_)            produces<vector< TYPE(jets) > > ();
  if (makeMcparticles_)     produces<vector< TYPE(mcparticles) > > ();
  if (makeMets_)            produces<vector< TYPE(mets) > > ();
  if (makeMuons_)           produces<vector< TYPE(muons) > > ();
  if (makePhotons_)         produces<vector< TYPE(photons) > > ();
  if (makePileUpInfos_)     produces<vector< TYPE(pileupinfos) > > ();
  if (makePrimaryvertexs_)  produces<vector< TYPE(primaryvertexs) > > ();
  if (makeTaus_)            produces<vector< TYPE(taus) > > ();
  if (makeTracks_)          produces<vector< TYPE(tracks) > > ();
  if (makeSecondaryTracks_) produces<vector< TYPE(secondaryTracks) > > ();
  if (makeTrigobjs_)        produces<vector< TYPE(trigobjs) > > ();
  if (makeSuperclusters_)   produces<vector< TYPE(superclusters) > > ();

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
  if (oldObjVec.empty()) {
    return;
  } else if (oldObjVec.size() > 1) {
    cout << "ERROR:  size of objVec = " << oldObjVec.size() << ", only valid size is 1." << endl;
    cout << "Will skip conversion to " << typeid(New).name() << endl;
    return;
  }
  // At this point we are sure that oldObjVec.size() == 1.
  edm::Handle<vector<Old> > oldObjs = oldObjVec.at(0);

  unique_ptr<vector< New > > newObjs (new vector< New > ());
  for (const auto &oldObj : *oldObjs)
    newObjs->emplace_back(oldObj);
  iEvent.put (std::move (newObjs));

}


// ------------ method called to produce the data  ------------
void
OriginalFormatProducer::produce (edm::Event& iEvent, const edm::EventSetup& iSetup)
{

#if IS_VALID(beamspots)
  //  if (makeBeamspots_)       produceCollection<osu::Beamspot,      TYPE(beamspots)>               (iEvent); Doesn't work currently.
#endif
#if IS_VALID(electrons)
  if (makeElectrons_)       produceCollection<osu::Electron,      TYPE(electrons)>                (iEvent);
#endif
#if IS_VALID(genjets)
  if (makeGenjets_)         produceCollection<osu::Genjet,        TYPE(genjets)>                 (iEvent);
#endif
#if IS_VALID(jets)
  if (makeJets_)            produceCollection<osu::Jet,           TYPE(jets)>                     (iEvent);
#endif
#if IS_VALID(mcparticles)
  if (makeMcparticles_)     produceCollection<osu::Mcparticle,    TYPE(mcparticles)>       (iEvent);
#endif
#if IS_VALID(mets)
  if (makeMets_)               produceCollection<osu::Met,           TYPE(mets)>                     (iEvent);
#endif
#if IS_VALID(muons)
  if (makeMuons_)              produceCollection<osu::Muon,          TYPE(muons)>                    (iEvent);
#endif
#if IS_VALID(photons)
  if (makePhotons_)             produceCollection<osu::Photon,        TYPE(photons)>                  (iEvent);
#endif
#if IS_VALID(pileupinfos)
  //  if (makePileUpInfos_)     produceCollection<osu::PileUpInfo,    TYPE(pileupinfos)>            (iEvent); // Doesn't work currently.
#endif
#if IS_VALID(primaryvertexs)
  if (makePrimaryvertexs_)  produceCollection<osu::Primaryvertex, TYPE(primaryvertexs)>                 (iEvent);
#endif
#if IS_VALID(taus)
  if (makeTaus_)             produceCollection<osu::Tau,           TYPE(taus)>                     (iEvent);
#endif
#if IS_VALID(tracks)
  if (makeTracks_)              produceCollection<osu::Track,         TYPE(tracks)>                  (iEvent);
#endif
#if IS_VALID(secondaryTracks)
  if (makeSecondaryTracks_)              produceCollection<osu::SecondaryTrack,         TYPE(secondaryTracks)>                  (iEvent);
#endif
#if IS_VALID(superclusters)
  if (makeSuperclusters_)   produceCollection<osu::Supercluster,  TYPE(superclusters)>           (iEvent);
#endif

}

//define this as a plug-in
DEFINE_FWK_MODULE (OriginalFormatProducer);



