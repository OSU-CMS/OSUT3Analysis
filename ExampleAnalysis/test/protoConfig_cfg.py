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

# input source when running interactively
# ---------------------------------------
# argument can be a ROOT file, directory, or dataset name*
# *registered dataset names are listed in 'datasets' in:
#    https://github.com/OSU-CMS/OSUT3Analysis/blob/master/Configuration/python/configurationOptions.py

# sample direcotory
set_input(process, "/store/user/ahart/BN_stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_1/")

# sample ROOT file
#set_input(process, "/store/user/ahart/BN_stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_1/stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_10_2_Dzw.root")

# sample dataset nickname
#set_input(process, "DYToTauTau_20")
#set_input(process, "DYToMuMu_20")

# output histogram file name when running interactively
process.TFileService = cms.Service ('TFileService',
    fileName = cms.string ('hist.root')
)

# number of events to process when running interactively
process.maxEvents = cms.untracked.PSet (
    input = cms.untracked.int32 (1000)
)

################################################################################
##### Set up the 'collections' map #############################################
################################################################################

# this PSet specifies which collections to get from the input files
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

################################################################################
##### Set up any user-defined variable producers ###############################
################################################################################

variableProducers = []
variableProducers.append("MyVariableProducer")

################################################################################
##### Import the channels to be run ############################################
################################################################################

from OSUT3Analysis.ExampleAnalysis.MyProtoEventSelections import *

################################################################################
##### Import the histograms to be plotted ######################################
################################################################################

from OSUT3Analysis.ExampleAnalysis.MyProtoHistogramDefinitions import *

################################################################################
##### Attach the channels and histograms to the process ########################
################################################################################

add_channels (process, [eMuMinimal], cms.VPSet (histograms), collections, variableProducers, False)

# uncomment to produce a full python configuration log file
#outfile = open('dumpedConfig.py','w'); print >> outfile,process.dumpPython(); outfile.close()
