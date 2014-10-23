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
    input = cms.untracked.int32 (5)
)

################################################################################
####################### Set up the individual analyzers ########################
################################################################################

# import the list of standard input collections 
#from OSUT3Analysis.AnaTools.osuAnalysis_cfi import collectionMap  # miniAOD
from OSUT3Analysis.AnaTools.osuAnalysis_cfi import collectionMapBEANs # BEANs


################################################################################

# create module to define an event-wide variable
# e.g. deltaPhi between Met and dimuon system 
process.UserVariableProduction = cms.EDProducer ('VariableProducer',
                                                 inputsMap = collectionMapBEANs
                                                 )


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


################################################################################
######### Import the information about all the histograms to be filled #########
################################################################################
from OSUT3Analysis.Configuration.histogramDefinitions import *
process.Plotting.histogramSets.append(MetHistograms)
process.Plotting.histogramSets.append(MuonHistograms)
process.Plotting.histogramSets.append(DiMuonHistograms)
process.Plotting.histogramSets.append(ElectronMuonHistograms)

# import histogram for any special user-defined variables
# this one has the dimuon-met delta phi histogram
from OSUT3Analysis.AnaTools.MyProtoHistogramDefinitions import *
process.Plotting.histogramSets.append(MyProtoHistograms)



################################################################################
#################### Put all desired modules into the path #####################
################################################################################

process.out0 = cms.OutputModule("PoolOutputModule",
                                dropMetaData = cms.untracked.string('ALL'),
                                outputCommands = cms.untracked.vstring('keep *',
                                                                       'drop BNtriggers_BNproducer_L1Talgo_BEANs'),
                                fileName = cms.untracked.string('bean.root'),
                                eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
                                splitLevel = cms.untracked.int32(0),
                                )


process.myPath = cms.Path (process.UserVariableProduction *
                           process.Plotting
                           )

# Include this to write out a skim:  
process.myEndPath0 = cms.EndPath(process.out0)


## Dump python config if wished
outfile = open('dumpedConfig.py','w'); print >> outfile,process.dumpPython(); outfile.close()


