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
process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring (
        #'root://cmsxrootd.fnal.gov//store/mc/RunIISummer16MiniAODv2/DisplacedSUSY_StopToBL_M-500_CTau-10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/80000/8CCDDA83-8CC8-E611-96A9-0CC47AD98B94.root',
        'root://cmsxrootd-site1.fnal.gov:1092//store/mc/RunIISpring16MiniAODv2/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/00000/428CC6A9-591B-E611-A8A7-0CC47A4F1D16.root',
        ),
)

# FIXME:  set_input does not work (because of error with /usr/bin/file) in CMSSW_7_4_5_ROOT5
# argument can be a ROOT file, directory, or dataset name*
# *registered dataset names are listed in 'datasets' in:
#    https://github.com/OSU-CMS/OSUT3Analysis/blob/master/Configuration/python/configurationOptions.py

# sample direcotory
# set_input(process, "/store/user/ahart/BN_stopToBottom_M_800_10mm_Tune4C_8TeV_pythia8_lantonel-Summer12_DR53X-PU_S10_START53_V19-v1-ab45720b22c4f98257a2f100c39d504b_USER_1/")

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

from OSUT3Analysis.AnaTools.osuAnalysis_cfi import collectionMap  # miniAOD

################################################################################
##### Set up weights to be used in plotting and cutflows  ######################
################################################################################

weights = cms.VPSet (
    cms.PSet (
        inputCollections = cms.vstring("muons"),
        inputVariable = cms.string("pt")
    ),
    #cms.PSet (
    #    inputCollections = cms.vstring("eventvariables"),
    #    inputVariable = cms.string("electronScalingFactor")
    #),
)

scalingfactorproducers = []
#ObjectScalingFactorProducer = {}
#ObjectScalingFactorProducer['name'] = 'ObjectScalingFactorProducer'
#ObjectScalingFactorProducer['muonFile'] = cms.string(os.environ['CMSSW_BASE'] + '/src/OSUT3Analy
#sis/AnaTools/data/muonSF.root')
#ObjectScalingFactorProducer['electronFile'] = cms.string(os.environ['CMSSW_BASE'] + '/src/OSUT3A
#nalysis/AnaTools/data/electronSF.root')
######Here the muonWp and electronWp refer to the histograms you want to use. #########
#ObjectScalingFactorProducer['muonWp'] = cms.string('NUM_TightIDandIPCut_DEN_genTracks_PAR_pt_spl
#iteta_bin1/abseta_vs_pt')ObjectScalingFactorProducer['electronWp'] = cms.string('GlobalSF')
#ObjectScalingFactorProducer['doEleSF'] = cms.bool(True)
#ObjectScalingFactorProducer['doMuSF'] = cms.bool(False)
#scalingfactorproducers.append(ObjectScalingFactorProducer)
################################################################################
##### Set up any user-defined variable producers ###############################
################################################################################

variableProducers = []
#variableProducers.append("MyVariableProducer")

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

add_channels (process, [eMuMinimal], cms.VPSet (histograms), weights,scalingfactorproducers, collectionMap, variableProducers, False)

# uncomment to produce a full python configuration log file
#outfile = open('dumpedConfig.py','w'); print >> outfile,process.dumpPython(); outfile.close()
