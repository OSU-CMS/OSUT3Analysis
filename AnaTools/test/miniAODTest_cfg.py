import FWCore.ParameterSet.Config as cms
from OSUT3Analysis.Configuration.processingUtilities import *
import math
import os

###########################################################
##### Set up process #####
###########################################################

process = cms.Process ('OSUAnalysis')
process.load ('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.source = cms.Source ('PoolSource',
  fileNames = cms.untracked.vstring (
    'file:miniAODSample.root'
    #'root://cmsxrootd.fnal.gov//store/mc/Phys14DR/DYJetsToLL_M-50_13TeV-madgraph-pythia8/MINIAODSIM/PU4bx50_PHYS14_25_V1-v1/00000/080957A7-C36E-E411-A5BC-00266CF327C4.root',
  )
)

#output file name when running interactively
process.TFileService = cms.Service ('TFileService',
    fileName = cms.string ('hist.root')
)
process.maxEvents = cms.untracked.PSet (
    input = cms.untracked.int32 (-1)
)

#set_input(process,"/store/user/ahart/BN_stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_1/stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_10_2_Dzw.root")

#set_input(process,"/store/user/ahart/BN_stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_1/")

#set_input(process,"WW")


###########################################################
##### Set up the analyzer #####
###########################################################

miniAOD_collections = cms.PSet (
  electrons       =  cms.InputTag  ('slimmedElectrons',               ''),
  genjets         =  cms.InputTag  ('slimmedGenJets',                 ''),
  jets            =  cms.InputTag  ('slimmedJets',                    ''),
  mcparticles     =  cms.InputTag  ('packedGenParticles',             ''),
  mets            =  cms.InputTag  ('slimmedMETs',                    ''),
  muons           =  cms.InputTag  ('slimmedMuons',                   ''),
  photons         =  cms.InputTag  ('slimmedPhotons',                 ''),
  primaryvertexs  =  cms.InputTag  ('offlineSlimmedPrimaryVertices',  ''),
  superclusters   =  cms.InputTag  ('reducedEgamma',                  'reducedSuperClusters'),
  taus            =  cms.InputTag  ('slimmedTaus',                    ''),
  triggers        =  cms.InputTag  ('TriggerResults',                 '',  'HLT'),
  trigobjs        =  cms.InputTag  ('selectedPatTrigger',             ''),
)


eMu = cms.PSet(
    name = cms.string("EMu"),
    triggers = cms.vstring('HLT_Mu38NoFiltersNoVtx_Photon38_CaloIdL'), 
    cuts = cms.VPSet (
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("pt > 43.0"),
        numberRequired = cms.string(">= 1")
      ),
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("abs(eta) < 2.5"),
        numberRequired = cms.string(">= 1")
      ),
      #Muon Id
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("isGlobalMuon"),
        numberRequired = cms.string(">= 1"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("isPFMuon"),
        numberRequired = cms.string(">= 1"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("normChi2 < 10.0"),
        numberRequired = cms.string(">= 1"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("globalTrack.hitPattern_.numberOfValidMuonHits > 0"),
        numberRequired = cms.string(">= 1"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("innerTrack.hitPattern_.numberOfValidPixelHits > 0"),
        numberRequired = cms.string(">= 1"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("innerTrack.hitPattern_.trackerLayersWithMeasurement > 0"),
        numberRequired = cms.string(">= 1"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("numberOfMatchedStations > 1"),
        numberRequired = cms.string(">= 1"),
      ),
      # MUON ISOLATION
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("                \
              (pfIsolationR04_.sumChargedHadronPt \
            + max(0.0,                            \
            pfIsolationR04_.sumNeutralHadronEt                    \
            + pfIsolationR04_.sumPhotonEt                         \
            - 0.5*pfIsolationR04_.sumPUPt))                        \
            /pt < 0.12"),
        numberRequired = cms.string(">= 1"),
        alias = cms.string("muon isolation < 0.12")
      ),
      cms.PSet (
        inputCollection = cms.vstring("muons","primaryvertexs"),
        cutString = cms.string("abs(-(-(muon.vx - primaryvertex.x)*muon.py + (muon.vy - primaryvertex.y)*muon.px)/muon.pt) > 0.01"),
        numberRequired = cms.string(">= 2"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("electrons"),
        cutString = cms.string("pt > 43.0"),
        numberRequired = cms.string(">= 1")
      ),
      cms.PSet (
        inputCollection = cms.vstring("electrons"),
        cutString = cms.string("abs(eta) < 2.5"),
        numberRequired = cms.string(">= 1")
      ),
      # ELECTRON ID
      cms.PSet (
        inputCollection = cms.vstring("electrons"),
        cutString = cms.string("abs(deltaEtaSuperClusterTrackAtVtx) < 0.005"),
        numberRequired = cms.string(">= 1"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("electrons"),
        cutString = cms.string("abs(deltaPhiSuperClusterTrackAtVtx) < 0.02"),
        numberRequired = cms.string(">= 1"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("electrons"),
        cutString = cms.string("hadronicOverEm < 0.03"),
        numberRequired = cms.string(">= 1"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("electrons"),
        cutString = cms.string("isGap < 1"),
        numberRequired = cms.string(">= 1"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("electrons"),
        cutString = cms.string("abs(1/et - 1/pt ) < 0.1"),
        numberRequired = cms.string(">= 1"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("electrons"),
        cutString = cms.string("passConversionVeto > 0"),
        numberRequired = cms.string(">= 1"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("electrons"),
        cutString = cms.string("gsfTrack.numberOfLostHits < 1"),
        numberRequired = cms.string(">= 1"),
      ),
      # ELECTRON ISOLATION
      cms.PSet (
        inputCollection = cms.vstring("electrons"),
        cutString = cms.string("                \
              (pfIso_.sumChargedHadronPt \
            + max(0.0,                            \
            pfIso_.sumNeutralHadronEt                    \
            + pfIso_.sumPhotonEt                         \
            - 0.5*pfIso_.sumPUPt))                        \
            /pt < 0.10"),
        numberRequired = cms.string(">= 1"),
        alias = cms.string("electron isolation < 0.1")
      ),
      cms.PSet (
        inputCollection = cms.vstring("electrons","primaryvertexs"),
        cutString = cms.string("abs(-(-(electron.vx - primaryvertex.x)*electron.py + (electron.vy - primaryvertex.y)*electron.px)/electron.pt)  > 0.01"),
        numberRequired = cms.string(">= 1"),
      ),
    )
)


doubleMuon = cms.PSet(
    name = cms.string("DoubleMuon"),
    triggers = cms.vstring("HLT_DoubleMu33NoFiltersNoVtx"), 
    cuts = cms.VPSet (
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("pt > 38.0"),
        numberRequired = cms.string(">= 2")
      ),
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("abs(eta) < 2.5"),
        numberRequired = cms.string(">= 2")
      ),
      #Muon Id
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("isGlobalMuon"),
        numberRequired = cms.string(">= 2"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("isPFMuon"),
        numberRequired = cms.string(">= 2"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("normChi2 < 10.0"),
        numberRequired = cms.string(">= 2"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("globalTrack.hitPattern_.numberOfValidMuonHits > 0"),
        numberRequired = cms.string(">= 2"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("innerTrack.hitPattern_.numberOfValidPixelHits > 0"),
        numberRequired = cms.string(">= 2"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("innerTrack.hitPattern_.trackerLayersWithMeasurement > 0"),
        numberRequired = cms.string(">= 2"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("numberOfMatchedStations > 1"),
        numberRequired = cms.string(">= 2"),
      ),
      # MUON ISOLATION
      cms.PSet (
        inputCollection = cms.vstring("muons"),
        cutString = cms.string("                \
              (pfIsolationR04_.sumChargedHadronPt \
            + max(0.0,                            \
            pfIsolationR04_.sumNeutralHadronEt                    \
            + pfIsolationR04_.sumPhotonEt                         \
            - 0.5*pfIsolationR04_.sumPUPt))                        \
            /pt < 0.12"),
        numberRequired = cms.string(">= 2"),
        alias = cms.string("muon isolation < 0.12")
      ),
      cms.PSet (
        inputCollection = cms.vstring("muons","primaryvertexs"),
        cutString = cms.string("abs(-(-(muon.vx - primaryvertex.x)*muon.py + (muon.vy - primaryvertex.y)*muon.px)/muon.pt) > 0.01"),
        numberRequired = cms.string(">= 2"),
      ),
    )
)

doubleElectron = cms.PSet(
    name = cms.string("DoubleElectron"),
    triggers = cms.vstring("HLT_Photon42_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon22_AND_HE10_R9Id65"), # TRIGGER
    cuts = cms.VPSet (
      # EVENT CLEANING
      cms.PSet (
        inputCollection = cms.vstring("electrons"),
        cutString = cms.string("pt > 45.0"),
        numberRequired = cms.string(">= 2")
      ),
      cms.PSet (
        inputCollection = cms.vstring("electrons"),
        cutString = cms.string("abs(eta) < 2.5"),
        numberRequired = cms.string(">= 2")
      ),
      # ELECTRON ID
      cms.PSet (
        inputCollection = cms.vstring("electrons"),
        cutString = cms.string("abs(deltaEtaSuperClusterTrackAtVtx) < 0.005"),
        numberRequired = cms.string(">= 2"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("electrons"),
        cutString = cms.string("abs(deltaPhiSuperClusterTrackAtVtx) < 0.02"),
        numberRequired = cms.string(">= 2"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("electrons"),
        cutString = cms.string("hadronicOverEm < 0.03"),
        numberRequired = cms.string(">= 2"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("electrons"),
        cutString = cms.string("isGap < 1"),
        numberRequired = cms.string(">= 2"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("electrons"),
        cutString = cms.string("abs(1/et - 1/pt ) < 0.1"),
        numberRequired = cms.string(">= 2"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("electrons"),
        cutString = cms.string("passConversionVeto > 0"),
        numberRequired = cms.string(">= 2"),
      ),
      cms.PSet (
        inputCollection = cms.vstring("electrons"),
        cutString = cms.string("gsfTrack.numberOfLostHits < 1"),
        numberRequired = cms.string(">= 2"),
      ),
      # ELECTRON ISOLATION
      cms.PSet (
        inputCollection = cms.vstring("electrons"),
        cutString = cms.string("                \
              (pfIso_.sumChargedHadronPt \
            + max(0.0,                            \
            pfIso_.sumNeutralHadronEt                    \
            + pfIso_.sumPhotonEt                         \
            - 0.5*pfIso_.sumPUPt))                        \
            /pt < 0.10"),
        numberRequired = cms.string(">= 2"),
        alias = cms.string("electron isolation < 0.1")
      ),
      cms.PSet (
        inputCollection = cms.vstring("electrons","primaryvertexs"),
        cutString = cms.string("abs(-(-(electron.vx - primaryvertex.x)*electron.py + (electron.vy - primaryvertex.y)*electron.px)/electron.pt)  > 0.01"),
        numberRequired = cms.string(">= 2"),
      ),
    )
)

muonHistograms = cms.PSet(
    inputCollection = cms.vstring("muons","primaryvertexs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("muonPt"),
            title = cms.string("Muon Transverse Momentum;muon p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(1000, 0, 100),
            inputVariables = cms.vstring("muon.pt"),
        ),
        cms.PSet (
            name = cms.string("muonEta"),
            title = cms.string("Muon Pseudorapidity;muon #eta"),
            binsX = cms.untracked.vdouble(1000, -5.0, 5.0),
            inputVariables = cms.vstring("muon.eta"),
        ),
        cms.PSet (
            name = cms.string("muonDxy"),
            title = cms.string("Muon IP;muon d_{xy}"),
            binsX = cms.untracked.vdouble(1000, -0.5, 0.5),
            inputVariables = cms.vstring("-(-(muon.vx - primaryvertex.x)*muon.py + (muon.vy - primaryvertex.y)*muon.px)/muon.pt"),
        ),
  )
)
electronHistograms = cms.PSet(
    inputCollection = cms.vstring("electrons","primaryvertexs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("electronPt"),
            title = cms.string("Electron Transverse Momentum;electron p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(1000, 0, 100),
            inputVariables = cms.vstring("electron.pt"),
        ),
        cms.PSet (
            name = cms.string("electronEta"),
            title = cms.string("Electron Pseudorapidity;electron #eta"),
            binsX = cms.untracked.vdouble(1000, -5.0, 5.0),
            inputVariables = cms.vstring("electron.eta"),
        ),
        cms.PSet (
            name = cms.string("electronDxy"),
            title = cms.string("Electron IP;electron d_{xy}"),
            binsX = cms.untracked.vdouble(1000, 0.5, 0.5),
            inputVariables = cms.vstring("-(-(electron.vx - primaryvertex.x)*electron.py + (electron.vy - primaryvertex.y)*electron.px)/electron.pt"),
        ),
  )
)

add_channels  (process,  [doubleElectron, doubleMuon, eMu],  cms.VPSet  (electronHistograms, muonHistograms),  miniAOD_collections,[],False)

from Configuration.DataProcessing.Utils import addMonitoring
process = addMonitoring (process)

#outfile = open('dumpedConfig.py','w'); print >> outfile,process.dumpPython(); outfile.close()
