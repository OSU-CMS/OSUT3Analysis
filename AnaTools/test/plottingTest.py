import FWCore.ParameterSet.Config as cms
import math
import os

from OSUT3Analysis.Configuration.processingUtilities import *

###########################################################
##### Set up process #####
###########################################################

process = cms.Process ('PlotTest')
process.load ('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.source = cms.Source ('PoolSource',
    fileNames = cms.untracked.vstring ()
)

#output file name when running interactively
process.TFileService = cms.Service ('TFileService',
    fileName = cms.string ('hist.root')
)
process.maxEvents = cms.untracked.PSet (
    input = cms.untracked.int32 (-1)
)

###########################################################
##### Set up the analyzer #####
###########################################################


process.Plotting = cms.EDAnalyzer ('Plotter',
    jets = cms.InputTag ('BNproducer', 'selectedPatJetsPFlow'),
    muons    = cms.InputTag ('BNproducer', 'selectedPatMuonsLoosePFlow'),
    electrons = cms.InputTag ('BNproducer', 'selectedPatElectronsLoosePFlow'),
    taus = cms.InputTag ('BNproducer', 'selectedPatTaus'),
    mets = cms.InputTag ('BNproducer', 'patMETsPFlow'),
    genjets = cms.InputTag ('BNproducer', 'ak5GenJets'),
    mcparticles = cms.InputTag ('BNproducer', 'MCstatus3'),
    primaryvertexs = cms.InputTag ('BNproducer', 'offlinePrimaryVertices'),
    photons = cms.InputTag ('BNproducer', 'none'),
    triggers = cms.InputTag('BNproducer','HLT'),

    histogramSets = cms.VPSet (),
    verbose = cms.int32(0),

)




###########################################################################################


###############################################################
##### Set Options for Running your Analyzer Interactively #####
###############################################################

dir = "/store/user/ahart/BN_DYToMuMu_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_1/"
for file in os.listdir(dir):
    if ".root" not in file:
        continue
    process.source.fileNames.extend(cms.untracked.vstring('file:' + dir + file))


#output file name when running interactively
#process.TFileService.fileName = 'hist.root'

#number of events to process when running interactively
#process.maxEvents.input = -1

########################################################################
##### Import the information about all the histograms to be filled #####
########################################################################

#import the desired sets of histograms from the standard python file which defines them
from OSUT3Analysis.Configuration.histogramDefinitions import *
process.Plotting.histogramSets.append(MuonHistograms)
process.Plotting.histogramSets.append(DiMuonHistograms)
process.Plotting.histogramSets.append(ElectronMuonHistograms)



process.myPath = cms.Path (process.Plotting)
