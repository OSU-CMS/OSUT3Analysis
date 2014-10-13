import FWCore.ParameterSet.Config as cms
import os

################################################################################
########################### Set up the main process ############################
################################################################################

process = cms.Process ('ProtoInvestigation')

# configure input files
process.source = cms.Source ('PoolSource',
    fileNames = cms.untracked.vstring ()
)
dir = "/store/user/ahart/BN_DYToMuMu_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_1/"
for file in os.listdir(dir):
    if ".root" not in file:
        continue
    process.source.fileNames.extend(cms.untracked.vstring('file:' + dir + file))

# configure output file 
process.TFileService = cms.Service ('TFileService',
    fileName = cms.string ('hist.root')
)

# number of events to process
# (mainly for testing, overwritten when running in batch mode)
process.maxEvents = cms.untracked.PSet (
    input = cms.untracked.int32 (10)
)

################################################################################
####################### Set up the individual analyzers ########################
################################################################################

# import the list of standard input collections in the miniAOD
from OSUT3Analysis.AnaTools.osuAnalysis_cfi import collectionsMap

################################################################################

# create module to define an event-wide variable
# e.g. deltaPhi between Met and dimuon system 
process.UserVariableProduction = cms.EDProducer ('VariableProducer',
                                                 inputsMap = collectionsMap
                                                 )

################################################################################
######### Import the information about all the histograms to be filled #########
################################################################################

# import histogram for any special user-defined variables
# this one has the dimuon-met delta phi histogram
from OSUT3Analysis.AnaTools.MyProtoHistogramDefinitions import *
process.Plotting.histogramSets.append(MyProtoHistograms)

################################################################################
#################### Put all desired modules into the path #####################
################################################################################

process.myPath = cms.Path (process.UserVariableProduction *
                           process.Plotting
                           )
