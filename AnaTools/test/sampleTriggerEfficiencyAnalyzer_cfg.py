# Usage:  
# cmsRun triggerEfficiencyAnalyzer_cfg.py | tee triggerEfficiencyAnalyzer_cfg.log
#
import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import os

process = cms.Process ('TriggerEfficiencyAnalyzer')
process.load ('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100


process.source = cms.Source ('PoolSource',
    fileNames = cms.untracked.vstring ()
)
#uncomment this line to add a single file to be processed
#process.source.fileNames.append('file:'+os.environ['CMSSW_BASE']+'/src/OSUT3Analysis/AnaTools/test/stop600toBottom_100mm.root')

#uncomment these 3 lines to add all files in a given directory to be processed
dir = "/store/user/abrinke1/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/894bf83260076a22df0c97ce24c6bb58/"
for file in os.listdir(dir):
        process.source.fileNames.extend(cms.untracked.vstring('file:' + dir + file))

process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

process.TFileService = cms.Service ('TFileService',
                                    fileName = cms.string ('trigger.root')
    )

process.maxEvents = cms.untracked.PSet (
    input = cms.untracked.int32 (1000)
)

process.TriggerEfficiencyAnalyzer = cms.EDAnalyzer ('TriggerEfficiencyAnalyzer',
    Trigger = cms.InputTag ('BNproducer', 'HLT'),
    triggers = cms.VPSet(),
)


#####################################################################
##### Define and Add the Desired Triggers to the List to be Run #####
##### One histogram will be made for each trigger type          #####  
#####################################################################
PFJet = cms.PSet(
    trigType = cms.string("PFJet"),
    trigs = cms.vstring(
        "HLT_PFJet80_v5",
        "HLT_PFJet140_v5",
        "HLT_PFJet200_v5",
        "HLT_PFJet260_v5",
        'HLT_PFJet320_v5',
        'HLT_PFJet400_v5', 
        ),
    )
process.TriggerEfficiencyAnalyzer.triggers.append(PFJet);

process.myPath = cms.Path (process.TriggerEfficiencyAnalyzer)


