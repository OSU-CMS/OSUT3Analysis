import FWCore.ParameterSet.Config as cms
import copy

###########################################################
##### Set up the event selections (channels) #####
###########################################################

##### List of valid input collections #####
# jets, muons, electrons, taus, photons, mets,
# events, tracks, primaryvertexs,
# genjets, mcparticles,
# bxlumis, superclusters

Event_Preselection = cms.PSet(
    cuts = cms.VPSet (
      cms.PSet (
        inputCollection = cms.vstring("events"),
        cutString = cms.string("FilterOutScraping > 0"),
        numberRequired = cms.string(">= 1")
      ),
      cms.PSet (
        inputCollection = cms.vstring("primaryvertexs"),
        cutString = cms.string("isGood > 0"),
        numberRequired = cms.string(">= 1")
      ),
   )
)

Muon_Preselection = cms.PSet(
    cuts = cms.VPSet (
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("abs(eta) < 2.5"),
        numberRequired = cms.string(">= 1")
      ),
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("pt > 25"),
        numberRequired = cms.string(">= 1")
      ),
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("\
        globalTrack.hitPattern_.numberOfValidMuonHits > 0 & \
        globalTrack.normalizedChi2 < 10 & \
        numberOfMatchedStations > 1 & \
        innerTrack.hitPattern_.numberOfValidPixelHits > 0 & \
        innerTrack.hitPattern_.trackerLayersWithMeasurement > 5"),
        numberRequired = cms.string(">= 1"),
        alias = cms.string("muon ID")
        ),
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("                \
        (pfIsolationR04_.sumChargedHadronPt \
        + max(0.0,                          \
        pfIsolationR04_.sumNeutralHadronEt  \
        + pfIsolationR04_.sumPhotonEt       \
        - 0.5*pfIsolationR04_.sumPUPt))     \
        /pt <= 0.15                         \
       "),
        numberRequired = cms.string(">= 1"),
        alias = cms.string("muon isolation")
      ),
   )
)


Electron_Preselection = cms.PSet(
    cuts = cms.VPSet (
      cms.PSet (
        inputCollection = cms.vstring("electrons"),
        cutString = cms.string("abs(eta) < 2.5"),
        numberRequired = cms.string(">= 1")
      ),
      cms.PSet (
        inputCollection = cms.vstring("electrons"),
        cutString = cms.string("pt > 25"),
        numberRequired = cms.string(">= 1")
      ),
      # ELECTRON CRACK VETO
      cms.PSet (
        inputCollection = cms.vstring("electrons"),
        cutString = cms.string("abs(eta) < 1.444 | abs(eta) > 1.566 "),
        numberRequired = cms.string(">= 1"),
        alias = cms.string("electron ECAL crack veto")
      ),
   )
)

## Preselection_EMu_PF_withTrigger = copy.deepcopy(Preselection_EMu_PF)
## Preselection_EMu_PF_withTrigger.name = "Preselection_EMu_PF_withTrigger"
## Preselection_EMu_PF_withTrigger.triggers = cms.vstring("HLT_Mu22_Photon22_CaloIdL_v")
