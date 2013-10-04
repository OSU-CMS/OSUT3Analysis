import FWCore.ParameterSet.Config as cms


###########################################################
##### Setup process #####
###########################################################

process = cms.Process ('SignalMC')
process.load ('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.source = cms.Source ('PoolSource',
    fileNames = cms.untracked.vstring ("file:/data/se/store/user/lannon/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12-PU_S6_START52_V9-v1_BEAN_53xOn52x_V01_CV01/b9130084bab6b1e2e321806968edb2ef/pat2bean_53x_100_1_Wj9.root")
)
process.TFileService = cms.Service ("TFileService",
    fileName = cms.string ('histograms.root')
)
process.maxEvents = cms.untracked.PSet (
    input = cms.untracked.int32 (1000)
)

###########################################################
##### Setup the analyzers #####
###########################################################

process.PUAnalyzer = cms.EDAnalyzer ('PUAnalyzer',
    events = cms.InputTag ('BNproducer', ''),
)

###########################################################
##### Setup paths #####
###########################################################

process.mypath = cms.Path (process.PUAnalyzer)
