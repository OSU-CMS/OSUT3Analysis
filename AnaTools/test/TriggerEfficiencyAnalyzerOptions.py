#!/usr/bin/env python

# import the definitions of all the datasets on the T3
from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.miniAODV2Samples import *

# specify which config file to pass to cmsRun
config_file = "TriggerEfficiencyAnalyzer_cfg.py"

# choose luminosity used for MC normalization
#intLumi = 19700 # from 8 TeV MuEG dataset
intLumi = 2538.43 # for 13 TeV 

# create list of datasets to process
datasets = [
    'Diboson',
    'WJetsToLNu',
    'DYJetsToLL_50',
    'SingleTop',
    'TTJets_Lept',
    'QCD_MuEnriched',
    'MuonEG_2015D',
#    'stop200_1mm',
#    'stop200_10mm',
#    'stop200_100mm',
#    'stop200_1000mm',
#    'stop600_1mm',
#    'stop600_10mm',
#    'stop600_100mm',
#    'stop600_1000mm',
#    'stop1000_1mm',
#    'stop1000_10mm',
#    'stop1000_100mm',
#    'stop1000_1000mm',
]

datasets_split = [
    'WWToLNuQQ',
    'WWToLNuLNu',
    'WZToLNuQQ',
    'WZToLLQQ',
    'WZToLNuNuNu',
    'WZToLLLNu',
    'ZZToNuNuQQ',
    'ZZToLLQQ',
    'ZZToLLNuNu',
    'ZZToLLLL',
    'WG',
    'ZG',

    'WJetsToLNu',
    'DYJetsToLL_50',

    'SingleTop_s_channel',
    'SingleTop_t_channel',
    'SingleTop_tW',
    'SingleTop_tbarW',

    'TTJets_DiLept',
    'TTJets_SingleLeptFromT',
    'TTJets_SingleLeptFromTbar',

    'QCD_MuEnriched_15to20',
    'QCD_MuEnriched_20to30',
    'QCD_MuEnriched_30to50',
    'QCD_MuEnriched_50to80',
    'QCD_MuEnriched_80to120',
    'QCD_MuEnriched_120to170',
    'QCD_MuEnriched_170to300',
    'QCD_MuEnriched_300to470',
    'QCD_MuEnriched_470to600',
    'QCD_MuEnriched_600to800',
    'QCD_MuEnriched_800to1000',
    'QCD_MuEnriched_1000toInf',

    'MuonEG_2015D_v3',
    'MuonEG_2015D_v4',
]

InputCondorArguments = {'hold': 'true'}
