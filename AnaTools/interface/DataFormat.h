#ifndef DATA_FORMAT

#define  BEAN      0
#define  MINI_AOD  1

#define DATA_FORMAT BEAN

#define INVALID_TYPE void *

#if DATA_FORMAT == BEAN
  #define  bxlumis_TYPE         BNbxlumi
  #define  electrons_TYPE       BNelectron
  #define  events_TYPE          BNevent
  #define  genjets_TYPE         BNgenjet
  #define  jets_TYPE            BNjet
  #define  mcparticles_TYPE     BNmcparticle
  #define  mets_TYPE            BNmet
  #define  muons_TYPE           BNmuon
  #define  photons_TYPE         BNphoton
  #define  primaryvertexs_TYPE  BNprimaryvertex
  #define  superclusters_TYPE   BNsupercluster
  #define  taus_TYPE            BNtau
  #define  tracks_TYPE          BNtrack
  #define  trigobjs_TYPE        BNtrigobj
  #define  userVariables_TYPE   VariableProducerPayload

  #define  triggers_TYPE        BNtriggerCollection

  #include "BEAN/Collections/interface/BNbxlumi.h"
  #include "BEAN/Collections/interface/BNelectron.h"
  #include "BEAN/Collections/interface/BNevent.h"
  #include "BEAN/Collections/interface/BNgenjet.h"
  #include "BEAN/Collections/interface/BNjet.h"
  #include "BEAN/Collections/interface/BNmcparticle.h"
  #include "BEAN/Collections/interface/BNmet.h"
  #include "BEAN/Collections/interface/BNmuon.h"
  #include "BEAN/Collections/interface/BNphoton.h"
  #include "BEAN/Collections/interface/BNprimaryvertex.h"
  #include "BEAN/Collections/interface/BNsupercluster.h"
  #include "BEAN/Collections/interface/BNtau.h"
  #include "BEAN/Collections/interface/BNtrack.h"
  #include "BEAN/Collections/interface/BNtrigger.h"
  #include "BEAN/Collections/interface/BNtrigobj.h"

#elif DATA_FORMAT == MINI_AOD
  #define  bxlumis_TYPE         INVALID_TYPE
  #define  electrons_TYPE       pat::Electron
  #define  events_TYPE          INVALID_TYPE
  #define  genjets_TYPE         reco::GenJet
  #define  jets_TYPE            pat::Jet
  #define  mcparticles_TYPE     pat::PackedGenParticle
  #define  mets_TYPE            pat::MET
  #define  muons_TYPE           pat::Muon
  #define  photons_TYPE         pat::Photon
  #define  primaryvertexs_TYPE  reco::Vertex
  #define  superclusters_TYPE   reco::SuperCluster
  #define  taus_TYPE            pat::Tau
  #define  tracks_TYPE          INVALID_TYPE
  #define  trigobjs_TYPE        pat::TriggerObjectStandAlone
  #define  userVariables_TYPE   VariableProducerPayload

  #define  bxlumis_INVALID
  #define  events_INVALID
  #define  tracks_INVALID

  #define  triggers_TYPE        edm::TriggerResults

  #include "DataFormats/Common/interface/TriggerResults.h"
  #include "DataFormats/EgammaReco/interface/SuperCluster.h"
  #include "DataFormats/JetReco/interface/GenJet.h"
  #include "DataFormats/PatCandidates/interface/Electron.h"
  #include "DataFormats/PatCandidates/interface/Jet.h"
  #include "DataFormats/PatCandidates/interface/MET.h"
  #include "DataFormats/PatCandidates/interface/Muon.h"
  #include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
  #include "DataFormats/PatCandidates/interface/Photon.h"
  #include "DataFormats/PatCandidates/interface/Tau.h"
  #include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
  #include "DataFormats/VertexReco/interface/Vertex.h"

#else
  #error "Data format is not valid."
#endif

#define XSTR(x) STR(x)
#define STR(x) #x
#define IS_VALID(x) !(defined (x##_INVALID))
#define EQ_VALID(s,x) (string (XSTR(x##_TYPE)) != XSTR(INVALID_TYPE) && string (s) == XSTR(x))

#define TYPE(x) x##_TYPE
#define TYPE_STR(x) XSTR(x##_TYPE)

#include <string>
#include "RVersion.h"

#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
  #define ROOT6
#else
  #define ROOT5
#endif

#ifdef ROOT6
  #error "ROOT 6 is not yet supported."
#endif

using namespace std;

#endif
