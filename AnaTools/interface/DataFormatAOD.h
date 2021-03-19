#define  beamspots_TYPE         reco::BeamSpot
#define  bxlumis_TYPE           INVALID_TYPE
#define  electrons_TYPE         reco::GsfElectron
#define  events_TYPE            INVALID_TYPE
#define  genjets_TYPE           INVALID_TYPE
#define  jets_TYPE              reco::PFJet
#define  bjets_TYPE             INVALID_TYPE
#define  mcparticles_TYPE       reco::GenParticle
#define  hardInteractionMcparticles_TYPE reco::GenParticle
#define  mets_TYPE              reco::PFMET
#define  muons_TYPE             reco::Muon
#define  photons_TYPE           reco::Photon
#define  primaryvertexs_TYPE    reco::Vertex
#define  superclusters_TYPE     INVALID_TYPE
#define  taus_TYPE              reco::PFTau
#define  tracks_TYPE            reco::Track
#define  secondaryTracks_TYPE   reco::Track
#define  pileupinfos_TYPE       PileupSummaryInfo
#define  trigobjs_TYPE          INVALID_TYPE
#define  uservariables_TYPE     VariableProducerPayload
#define  eventvariables_TYPE    EventVariableProducerPayload

#define  cschits_TYPE           INVALID_TYPE
#define  cscsegs_TYPE           INVALID_TYPE
#define  dtsegs_TYPE            INVALID_TYPE
#define  rpchits_TYPE           INVALID_TYPE

#define  triggers_TYPE        edm::TriggerResults
#define  prescales_TYPE       INVALID_TYPE
#define  generatorweights_TYPE  GenEventInfoProduct

#define  bxlumis_INVALID       1
#define  events_INVALID        1
#define  genjets_INVALID       1
#define  prescales_INVALID     1
#define  superclusters_INVALID 1
#define  trigobjs_INVALID      1
#define  bjets_INVALID         1
#define  cschits_INVALID       1
#define  cscsegs_INVALID       1
#define  dtsegs_INVALID        1
#define  rpchits_INVALID       1


#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
