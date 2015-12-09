#!/usr/bin/env python

# import the definitions of all the datasets on the T3
from OSUT3Analysis.Configuration.configurationOptions_13TeV import *

# specify which config file to pass to cmsRun
config_file = "TriggerEfficiencyAnalyzer_cfg.py"

# choose luminosity used for MC normalization
#intLumi = 19700 # from 8 TeV MuEG dataset
intLumi = 2538.43 # for 13 TeV 

# create list of datasets to process
datasets = [
    'Diboson_MiniAOD',
    'WJetsToLNu_MiniAOD',
    'DYJetsToLL_50_MiniAOD',
    'SingleTop_MiniAOD',
    'TTJets_Lept_MiniAOD',
    'QCD_MuEnriched_MiniAOD',
    'MuonEG_2015D',
#    'stop200_1mm_MiniAOD',
#    'stop200_10mm_MiniAOD',
#    'stop200_100mm_MiniAOD',
#    'stop200_1000mm_MiniAOD',
#    'stop600_1mm_MiniAOD',
#    'stop600_10mm_MiniAOD',
#    'stop600_100mm_MiniAOD',
#    'stop600_1000mm_MiniAOD',
#    'stop1000_1mm_MiniAOD',
#    'stop1000_10mm_MiniAOD',
#    'stop1000_100mm_MiniAOD',
#    'stop1000_1000mm_MiniAOD',
]

datasets_split = [
    'WWToLNuQQ_MiniAOD',
    'WWToLNuLNu_MiniAOD',
    'WZToLNuQQ_MiniAOD',
    'WZToLLQQ_MiniAOD',
    'WZToLNuNuNu_MiniAOD',
    'WZToLLLNu_MiniAOD',
    'ZZToNuNuQQ_MiniAOD',
    'ZZToLLQQ_MiniAOD',
    'ZZToLLNuNu_MiniAOD',
    'ZZToLLLL_MiniAOD',
    'WG_MiniAOD',
    'ZG_MiniAOD',

    'WJetsToLNu_MiniAOD',
    'DYJetsToLL_50_MiniAOD',

    'SingleTop_s_channel_MiniAOD',
    'SingleTop_t_channel_MiniAOD',
    'SingleTop_tW_MiniAOD',
    'SingleTop_tbarW_MiniAOD',

    'TTJets_DiLept_MiniAOD',
    'TTJets_SingleLeptFromT_MiniAOD',
    'TTJets_SingleLeptFromTbar_MiniAOD',

    'QCD_MuEnriched_15to20_MiniAOD',
    'QCD_MuEnriched_20to30_MiniAOD',
    'QCD_MuEnriched_30to50_MiniAOD',
    'QCD_MuEnriched_50to80_MiniAOD',
    'QCD_MuEnriched_80to120_MiniAOD',
    'QCD_MuEnriched_120to170_MiniAOD',
    'QCD_MuEnriched_170to300_MiniAOD',
    'QCD_MuEnriched_300to470_MiniAOD',
    'QCD_MuEnriched_470to600_MiniAOD',
    'QCD_MuEnriched_600to800_MiniAOD',
    'QCD_MuEnriched_800to1000_MiniAOD',
    'QCD_MuEnriched_1000toInf_MiniAOD',

    'MuonEG_2015D_v3',
    'MuonEG_2015D_v4',
]

InputCondorArguments = {'hold': 'true'}
