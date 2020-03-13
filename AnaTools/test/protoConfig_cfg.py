import FWCore.ParameterSet.Config as cms
import os

################################################################################
########################### Set up the main process ############################
################################################################################

process = cms.Process ('ProtoAnalysis')
process.load ('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100

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

# specify input miniAOD collections to be used
myMuons = cms.InputTag ('PAT', 'slimmedMuons')
myMETs = cms.InputTag ('PAT', 'slimmedMETs')

################################################################################

# create module to define an event-wide variable
# e.g. deltaPhi between Met and dimuon system
# !!! needs to be written
process.DiMuonMetDeltaPhiDefinition = cms.EDProducer ('VariableProducer',
                                                      muons = myMuons,
                                                      mets = myMets
                                                      )

################################################################################

# create module to calculate muon correction SFs
# !!! needs to be written
process.MuonCorrections = cms.EDProducer ('CorrectionProducer',
                                          muons = myMuons
                                          )

################################################################################

# create module to calculate met corrections
# !!! needs to be written
process.MetCorrections = cms.EDProducer ('CorrectionProducer',
                                         mets = myMets
                                         )

################################################################################

# combine correction modules into one sequence
process.CorrectionSequence = cms.Sequence(process.MuonCorrections *
                                          process.MetCorrections
                                          )

################################################################################

# create module to flag all objects in all events
# according to the user-defined selections (channels)
process.EventSelections = cms.EDProducer ('EventProcessor',
                                          muons = myMuons
                                          )

# import the cut definitions to test
from OSUT3Analysis.AnaTools.MyProtoEventSelections import ZtoMuMu_Selection

# add desired channel(s) to the event processing module
process.EventSelections.channels.append(ZtoMuMu_Selection)

################################################################################

# import the standard sequence to filter events based on the cuts
# and produce the cutflow histogram in the output file
# !!! needs to be written
from OSUT3Analysis.Configuration.StandardSequenes import EventFiltering


################################################################################
######### Import the information about all the histograms to be filled #########
################################################################################

# import the desired sets of standard histograms and their definitions
from OSUT3Analysis.Configuration.histogramDefinitions import *
process.Plotting.histogramSets.append(ExampleMuonHistograms)
process.Plotting.histogramSets.append(ExampleDiMuonHistograms)

# import histogram for any special user-defined variables
# this one has the dimuon-met delta phi histogram
from OSUT3Analysis.AnaTools.MyProtoHistogramDefinitions import *
process.Plotting.histogramSets.append(MyProtoHistograms)

process.myPath = cms.Path (process.DiMuonMetDeltaPhiDefinition *
                           process.CorrectionSequence *
                           process.EventSelections *
                           process.EventFiltering *
                           process.Plotting
                           )
