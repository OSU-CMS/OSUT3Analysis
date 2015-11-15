#define  beamspots_TYPE         reco::BeamSpot
#define  bxlumis_TYPE           INVALID_TYPE
#define  electrons_TYPE         reco::GsfElectron
#define  events_TYPE            INVALID_TYPE
#define  genjets_TYPE           INVALID_TYPE
#define  jets_TYPE              reco::PFJet
#define  bjets_TYPE             INVALID_TYPE
#define  candeles_TYPE          INVALID_TYPE
#define  candjets_TYPE          INVALID_TYPE
#define  basicjets_TYPE         reco::BasicJet
#define  mcparticles_TYPE       INVALID_TYPE
#define  mets_TYPE              reco::PFMET
#define  muons_TYPE             reco::Muon
#define  photons_TYPE           reco::Photon
#define  primaryvertexs_TYPE    reco::Vertex
#define  superclusters_TYPE     INVALID_TYPE
#define  taus_TYPE              reco::PFTau
#define  tracks_TYPE            reco::Track
#define  pileupinfos_TYPE       PileupSummaryInfo
#define  trigobjs_TYPE          INVALID_TYPE
#define  uservariables_TYPE     VariableProducerPayload
#define  eventvariables_TYPE    EventVariableProducerPayload

#define  triggers_TYPE        edm::TriggerResults
#define  prescales_TYPE       INVALID_TYPE
#define  generatorweights_TYPE  GenEventInfoProduct

#define  bxlumis_INVALID
#define  events_INVALID
#define  genjets_INVALID
#define  mcparticles_INVALID
#define  prescales_INVALID
#define  superclusters_INVALID
#define  trigobjs_INVALID
#define  bjets_INVALID
#define  candeles_INVALID
#define  candjets_INVALID


#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/BasicJet.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
