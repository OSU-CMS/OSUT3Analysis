import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import os
from OSUT3Analysis.Configuration.processingUtilities import *

process = cms.Process ('TriggerEfficiencyAnalyzer')
process.load ('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.source = cms.Source ('PoolSource',
  fileNames = cms.untracked.vstring (
#        'file:/data/users/lantonel/condor/DEC3__DisplacedControlRegionNoTriggerNoIsoSkim/TTJets_DiLept_MiniAOD/DisplacedControlRegionNoTriggerNoIso/skim_0.root'
        'file:/data/users/lantonel/condor/DEC2__EMUTRIGGERSTUDYSKIM/TTJets_DiLept_MiniAOD/EMuTriggerStudySkimSelection/skim_0.root'


#        'file:/data/users/lantonel/condor/DEC3__DisplacedControlRegionNoTriggerNoIsoSkim/MuonEG_2015D_v4/DisplacedControlRegionNoTriggerNoIso/skim_0.root'
#    'root://cmsxrootd.fnal.gov//store/mc/RunIISpring15DR74/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v3/10000/009D49A5-7314-E511-84EF-0025905A605E.root',
  )
)

#set_input(process,'/data/users/lantonel/condor/DEC3__DisplacedControlRegionNoTriggerNoIsoSkim/MuonEG_2015D_v3/DisplacedControlRegionNoTriggerNoIso/')
#process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

process.TFileService = cms.Service ('TFileService',
                                    fileName = cms.string ('hist.root')
    )

process.maxEvents = cms.untracked.PSet (
    input = cms.untracked.int32 (10000)
)

process.TriggerEfficiencyAnalyzer = cms.EDAnalyzer ('TriggerEfficiencyAnalyzer',
    Trigger = cms.InputTag ('TriggerResults'),
    triggers = cms.VPSet(),
)

#####################################################################
##### Define and Add the Desired Triggers to the List to be Run #####
##### One histogram will be made for each trigger type          #####
#####################################################################
EMu = cms.PSet(
    trigType = cms.string("EMu"),
    trigs = cms.vstring(
        "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v",
        "HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v",
        "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v",
        "HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v",
        "HLT_Mu30_Ele30_CaloIdL_GsfTrkIdVL_v",
        "HLT_Mu12_Photon25_CaloIdL_v",
        "HLT_Mu12_Photon25_CaloIdL_L1ISO_v",
        "HLT_Mu12_Photon25_CaloIdL_L1OR_v",
        "HLT_Mu17_Photon30_CaloIdL_L1ISO_v",
        "HLT_Mu17_Photon35_CaloIdL_L1ISO_v",
        "HLT_Mu38NoFiltersNoVtx_Photon38_CaloIdL_v",
        "HLT_Mu42NoFiltersNoVtx_Photon42_CaloIdL_v",
        "HLT_Mu28NoFiltersNoVtxDisplaced_Photon28_CaloIdL_v",
        "HLT_Mu33NoFiltersNoVtxDisplaced_Photon33_CaloIdL_v",
        "HLT_Mu23NoFiltersNoVtx_Photon23_CaloIdL_v",
        ),
    )
process.TriggerEfficiencyAnalyzer.triggers.append(EMu)

process.myPath = cms.Path (process.TriggerEfficiencyAnalyzer)
