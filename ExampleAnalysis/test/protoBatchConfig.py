#!/usr/bin/env python3

# to be run with osusub.py (see https://twiki.cern.ch/twiki/bin/view/CMS/OSUT3AnalysisTutorial)

# import the definitions of all the datasets on the T3
from OSUT3Analysis.Configuration.configurationOptions import *
#from OSUT3Analysis.Configuration.miniAODV2Samples import *
from OSUT3Analysis.Configuration.miniAODV2_80X_Samples import *

# specify which config file to pass to cmsRun
config_file = "protoConfig_cfg.py"

# choose luminosity used for MC normalization
#intLumi = 19700 # from 8 TeV MuEG dataset
intLumi = 2460 # from 13 TeV silver json file

# create list of datasets to process
datasets = [
    'MuonEG_2016B',
    #'WJetsToLNu',
    #'SingleTop_tW',
    'DYJetsToLL_50',
    'TTJets_DiLept'
]

InputCondorArguments = {}
