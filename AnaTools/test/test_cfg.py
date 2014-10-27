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
        '/store/user/ahart/BN_stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_1/stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_1_2_iaE.root',
        '/store/user/ahart/BN_stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_1/stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_2_2_eod.root',
        '/store/user/ahart/BN_stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_1/stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_3_2_brE.root',
        '/store/user/ahart/BN_stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_1/stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_4_2_NBg.root',
        '/store/user/ahart/BN_stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_1/stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_5_1_LlD.root',
        '/store/user/ahart/BN_stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_1/stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_6_2_bHx.root',
        '/store/user/ahart/BN_stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_1/stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_7_2_fo3.root',
        '/store/user/ahart/BN_stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_1/stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_8_2_qbH.root',
        '/store/user/ahart/BN_stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_1/stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_9_1_spy.root',
        '/store/user/ahart/BN_stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_1/stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_10_2_Dzw.root',
    )
)

#output file name when running interactively
process.TFileService = cms.Service ('TFileService',
    fileName = cms.string ('hist.root')
)
process.maxEvents = cms.untracked.PSet (
    input = cms.untracked.int32 (1000)
)

###########################################################
##### Set up the analyzer #####
###########################################################

collections = cms.PSet (
  bxlumis         =  cms.InputTag  ('BNproducer',  'BXlumi'),
  electrons       =  cms.InputTag  ('BNproducer',  'selectedPatElectronsLoosePFlow'),
  events          =  cms.InputTag  ('BNproducer',  ''),
  genjets         =  cms.InputTag  ('BNproducer',  'ak5GenJets'),
  jets            =  cms.InputTag  ('BNproducer',  'selectedPatJetsPFlow'),
  mcparticles     =  cms.InputTag  ('BNproducer',  'MCstatus3'),
  mets            =  cms.InputTag  ('BNproducer',  'patMETsPFlow'),
  muons           =  cms.InputTag  ('BNproducer',  'selectedPatMuonsLoosePFlow'),
  photons         =  cms.InputTag  ('BNproducer',  'none'),
  primaryvertexs  =  cms.InputTag  ('BNproducer',  'offlinePrimaryVertices'),
  secMuons        =  cms.InputTag  ('BNproducer',  'selectedPatMuonsLoosePFlow'),
  stops           =  cms.InputTag  ('BNproducer',  'MCstop'),
  superclusters   =  cms.InputTag  ('BNproducer',  'corHybridSCandMulti5x5WithPreshower'),
  taus            =  cms.InputTag  ('BNproducer',  'selectedPatTaus'),
  tracks          =  cms.InputTag  ('BNproducer',  'generalTracks'),
  triggers        =  cms.InputTag  ('BNproducer',  'HLT'),
  trigobjs        =  cms.InputTag  ('BNproducer',  'HLT'),
)

eMuMinimal = cms.PSet(
    name = cms.string("EMuMinimal"),
    triggers = cms.vstring("HLT_Mu22_Photon22_CaloIdL_v"), # TRIGGER
    cuts = cms.VPSet (
      # EVENT CLEANING
      cms.PSet (
        inputCollection = cms.string("events"),
        cutString = cms.string("FilterOutScraping > 0"),
        numberRequired = cms.string(">= 1")
      ),
      # EVENT HAS GOOD PV
      cms.PSet (
        inputCollection = cms.string("primaryvertexs"),
        cutString = cms.string("isGood > 0"),
        numberRequired = cms.string(">= 1")
      ),
      # ELECTRON ETA CUT
      cms.PSet (
        inputCollection = cms.string("electrons"),
        cutString = cms.string("abs(eta) < 2.5"),
        numberRequired = cms.string(">= 1")
      ),
      # ELECTRON PT CUT
      cms.PSet (
        inputCollection = cms.string("electrons"),
        cutString = cms.string("pt > 25"),
        numberRequired = cms.string(">= 1")
      ),
      # MUON ETA CUT
      cms.PSet (
        inputCollection = cms.string("muons"),
        cutString = cms.string("abs(eta) < 2.5"),
        numberRequired = cms.string(">= 1")
      ),
      # MUON PT CUT
      cms.PSet (
        inputCollection = cms.string("muons"),
        cutString = cms.string("pt > 25"),
        numberRequired = cms.string(">= 1")
      ),
    )
)

preselection = cms.PSet(
    name = cms.string("Preselection"),
    triggers = cms.vstring("HLT_Mu22_Photon22_CaloIdL_v"), # TRIGGER
    cuts = cms.VPSet (
      # EVENT CLEANING
      cms.PSet (
        inputCollection = cms.string("events"),
        cutString = cms.string("FilterOutScraping > 0"),
        numberRequired = cms.string(">= 1")
      ),
      # EVENT HAS GOOD PV
      cms.PSet (
        inputCollection = cms.string("primaryvertexs"),
        cutString = cms.string("isGood > 0"),
        numberRequired = cms.string(">= 1")
      ),
      # ELECTRON ETA CUT
      cms.PSet (
        inputCollection = cms.string("electrons"),
        cutString = cms.string("abs(eta) < 2.5"),
        numberRequired = cms.string(">= 1")
      ),
      # ELECTRON CRACK VETO
      cms.PSet (
        inputCollection = cms.string("electrons"),
        cutString = cms.string("isEBEEGap == 0"),
        numberRequired = cms.string(">= 1"),
        alias = cms.string("electron ECAL crack veto")
      ),
      # ELECTRON PT CUT
      cms.PSet (
        inputCollection = cms.string("electrons"),
        cutString = cms.string("pt > 25"),
        numberRequired = cms.string(">= 1")
      ),
      # ELECTRON ID
      cms.PSet (
        inputCollection = cms.string("electrons"),
        cutString = cms.string("mvaNonTrig_HtoZZto4l > 0"),
        numberRequired = cms.string(">= 1")
      ),
      # PHOTON CONVERSION VETO
      cms.PSet (
        inputCollection = cms.string("electrons"),
        cutString = cms.string("passConvVeto > 0 & numberOfLostHits = 0"),
        numberRequired = cms.string(">= 1"),
        alias = cms.string("electron conversion rejection")
      ),
      # ELECTRON ISOLATION
      cms.PSet (
        inputCollection = cms.string("electrons"),
        cutString = cms.string("relPFrhoIso < 0.1"),
        numberRequired = cms.string(">= 1")
      ),
      # MUON ETA CUT
      cms.PSet (
        inputCollection = cms.string("muons"),
        cutString = cms.string("abs(eta) < 2.5"),
        numberRequired = cms.string(">= 1")
      ),
      # MUON PT CUT
      cms.PSet (
        inputCollection = cms.string("muons"),
        cutString = cms.string("pt > 25"),
        numberRequired = cms.string(">= 1")
      ),
      # MUON ID
      cms.PSet (
        inputCollection = cms.string("muons"),
        cutString = cms.string("tightIDdisplaced > 0"),
        numberRequired = cms.string(">= 1")
      ),
      # MUON ISOLATION
      cms.PSet (
        inputCollection = cms.string("muons"),
        cutString = cms.string("relPFdBetaIso < 0.12"),
        numberRequired = cms.string(">= 1")
      ),
      # VETO EVENTS WITH EXTRA ELECTRON
      cms.PSet (
        inputCollection = cms.string("electrons"),
        cutString = cms.string("pt > -1"),
        numberRequired = cms.string("== 1"),
        alias = cms.string("extra electron veto")
      ),
      # VETO EVENTS WITH EXTRA MUON
      cms.PSet (
        inputCollection = cms.string("muons"),
        cutString = cms.string("pt > -1"),
        numberRequired = cms.string("== 1"),
        alias = cms.string("extra muon veto")
      ),
      # OPPOSITE SIGN E-MU PAIR
      cms.PSet (
        inputCollection = cms.string("electron-muon pairs"),
        cutString = cms.string("chargeProduct < 0"),
        numberRequired = cms.string("== 1")
      ),
      # ELECTRON AND MUON ARE NOT OVERLAPPING
      cms.PSet (
        inputCollection = cms.string("electron-muon pairs"),
        cutString = cms.string("deltaR > 0.5"),
        numberRequired = cms.string("== 1")
      ),

      #########START OF ADDITIONAL CUTS TO REQUIRE LEPTON IS NOT IN A JET
      # ONLY CONSIDER 30 GEV JETS
      cms.PSet (
        inputCollection = cms.string("jets"),
        cutString = cms.string("pt > 10"),
        numberRequired = cms.string(">= 0")
      ),
      # ELECTRON NOT OVERLAPPING WITH JET
      cms.PSet (
        inputCollection = cms.string("electron-jet pairs"),
        cutString = cms.string("deltaR < 0.5"),
        numberRequired = cms.string("== 0"),
        isVeto = cms.bool(True),
        alias = cms.string("electron near jet veto"),
      ),
      # MUON NOT OVERLAPPING WITH JET
      cms.PSet (
        inputCollection = cms.string("muon-jet pairs"),
        cutString = cms.string("deltaR < 0.5"),
        numberRequired = cms.string("== 0"),
        isVeto = cms.bool(True),
        alias = cms.string("muon near jet veto"),        
      ),
      ########### END OF ADDITIONAL CUTS TO REQUIRE LEPTON IS NOT IN A JET      

      # RESTRICT ELECTRONS TO RECONSTRUCTION ACCEPTANCE
      cms.PSet (
        inputCollection = cms.string("electrons"),
        cutString = cms.string("abs(correctedD0) < 2"),
        numberRequired = cms.string("== 1")
      ),
      # RESTRICT MUONS TO TRIGGER ACCEPTANCE
      cms.PSet (
        inputCollection = cms.string("muons"),
        cutString = cms.string("abs(correctedD0) < 2"),
        numberRequired = cms.string("== 1")
      ),
   )
)

histograms = cms.PSet(
    inputCollection = cms.string("muons"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("muonPt"),
            title = cms.string("Muon Transverse Momentum; muon p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
  )
)

add_channels  (process,  [eMuMinimal],    cms.VPSet  (histograms),  collections,  False)
add_channels  (process,  [preselection],  cms.VPSet  (histograms),  collections)

outfile = open('dumpedConfig.py','w'); print >> outfile,process.dumpPython(); outfile.close()
