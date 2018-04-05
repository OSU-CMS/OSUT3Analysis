#define  beamspots_TYPE                   reco::BeamSpot
#define  bxlumis_TYPE                     INVALID_TYPE
#define  electrons_TYPE                   pat::Electron
#define  events_TYPE                      INVALID_TYPE
#define  genjets_TYPE                     reco::GenJet
#define  jets_TYPE                        pat::Jet
#define  bjets_TYPE                       pat::Jet
#define  mcparticles_TYPE                 pat::PackedGenParticle
#define  hardInteractionMcparticles_TYPE  reco::GenParticle
#define  mets_TYPE                        pat::MET
#define  muons_TYPE                       pat::Muon
#define  photons_TYPE                     pat::Photon
#define  primaryvertexs_TYPE              reco::Vertex
#define  superclusters_TYPE               reco::SuperCluster
#define  taus_TYPE                        pat::Tau
#define  tracks_TYPE                      INVALID_TYPE
#define  secondaryTracks_TYPE             INVALID_TYPE
#define  pileupinfos_TYPE                 PileupSummaryInfo
#define  trigobjs_TYPE                    pat::TriggerObjectStandAlone
#define  uservariables_TYPE               VariableProducerPayload
#define  eventvariables_TYPE              EventVariableProducerPayload

#define  cschits_TYPE           INVALID_TYPE
#define  cscsegs_TYPE           INVALID_TYPE
#define  dtsegs_TYPE            INVALID_TYPE
#define  rpchits_TYPE           INVALID_TYPE

#define  triggers_TYPE          edm::TriggerResults
#define  prescales_TYPE         pat::PackedTriggerPrescales
#define  generatorweights_TYPE  GenEventInfoProduct

#define  bxlumis_INVALID
#define  events_INVALID
#define  tracks_INVALID
#define  secondaryTracks_INVALID

#define  cschits_INVALID
#define  cscsegs_INVALID
#define  dtsegs_INVALID
#define  rpchits_INVALID

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
