import FWCore.ParameterSet.Config as cms
from OSUT3Analysis.Configuration.processingUtilities import *
import math
import os

################################################################################
##### Set up the 'process' object ##############################################
################################################################################

process = cms.Process ('OSUAnalysis')

# how often to print a log message
process.load ('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.source = cms.Source ('PoolSource',
  fileNames = cms.untracked.vstring (
#        'file:/data/users/lantonel/condor/DEC3__DisplacedControlRegionNoTriggerNoIsoSkim/TTJets_DiLept_MiniAOD/DisplacedControlRegionNoTriggerNoIso/skim_0.root'
#'file:/data/users/bing/condor/EMuSkim13TeV/DYJetsToLL_50_MiniAOD/EMuSKim13TeV/skim_0.root',
'file:/data/users/bing/condor/EMuSkim13TeV/MuonEG_2015D_v4/EMuSKim13TeV/skim_0.root',
#'file:/store/user/wulsin/SingleMuon/Run2015D-05Oct2015-v1/151103_163809/0000/miniAODWithCandidateTracks_1.root',

#    'root://cmsxrootd.fnal.gov//store/mc/RunIISpring15DR74/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v3/10000/009D49A5-7314-E511-84EF-0025905A605E.root',
#    'root://cmsxrootd.fnal.gov//store/mc/RunIISpring15MiniAODv2/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/50000/00759690-D16E-E511-B29E-00261894382D.root',
    #'file:/data/users/bing/condor/EMuSkim13TeV/TTJets_DiLept_MiniAOD/EMuSKim13TeV/skim_416.root',

  )
)


# output histogram file name when running interactively
process.TFileService = cms.Service ('TFileService',
    fileName = cms.string ('hist.root')
)

# number of events to process when running interactively
process.maxEvents = cms.untracked.PSet (
    input = cms.untracked.int32 (-1)
)

################################################################################
##### Set up the 'collections' map #############################################
################################################################################

# this PSet specifies which collections to get from the input files
miniAOD_collections = cms.PSet (
  genjets         =  cms.InputTag  ('slimmedGenJets',                 ''),
  mcparticles     =  cms.InputTag  ('packedGenParticles',             ''),
  mets            =  cms.InputTag  ('slimmedMETs',                    ''),
  muons           =  cms.InputTag  ('slimmedMuons',                   ''),
  jets            =  cms.InputTag  ('slimmedJets',                   ''),
  electrons       =  cms.InputTag  ('slimmedElectrons',               ''),
  photons         =  cms.InputTag  ('slimmedPhotons',                 ''),
  generatorweights = cms.InputTag  ('generator', ''),
  primaryvertexs  =  cms.InputTag  ('offlineSlimmedPrimaryVertices',  ''),
  #please notice this inputTag is different in miniAODv1 and v2.
  #pileupinfos     =  cms.InputTag  ("addPileupInfo",           ""),
  pileupinfos     =  cms.InputTag  ("slimmedAddPileupInfo",           ""),
  beamspots       =  cms.InputTag  ('offlineBeamSpot',                ''),
  superclusters   =  cms.InputTag  ('reducedEgamma',                  'reducedSuperClusters'),
  taus            =  cms.InputTag  ('slimmedTaus',                    ''),
  triggers        =  cms.InputTag  ('TriggerResults',                 '',  'HLT'),
  trigobjs        =  cms.InputTag  ('selectedPatTrigger',             ''),
)

collections = miniAOD_collections

################################################################################
##### Set up any user-defined variable producers ###############################
################################################################################

variableProducers = []

################################################################################
##### Import the channels to be run ############################################
################################################################################

GsfTrackTest = cms.PSet(
    name = cms.string("GsfTrackTest"),
    triggers = cms.vstring(),
    cuts = cms.VPSet (
        cms.PSet (
            inputCollection = cms.vstring("electrons"),
            cutString = cms.string("gsfTrack.pt > 0"),
            numberRequired = cms.string(">= 1")
        ),
    )
)
eventSelections = []
eventSelections.append(GsfTrackTest)

weights = cms.VPSet()

################################################################################
##### Import the histograms to be plotted ######################################
################################################################################

histograms = cms.VPSet()

################################################################################
##### Attach the channels and histograms to the process ########################
################################################################################

add_channels (process, eventSelections, histograms, weights, collections, variableProducers, False)

