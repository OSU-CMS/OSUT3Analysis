#!/usr/bin/env python

# to be run with submitToCondor.py -l protoBatchConfig.py

# import the definitions of all the datasets on the T3
from OSUT3Analysis.Configuration.configurationOptions import *

# specify which config file to pass to cmsRun
config_file = "muonAnalyzer_cfg.py"

# choose luminosity used for MC normalization
intLumi = 19495 # from 8 TeV double muon dataset

# create list of datasets to process
datasets = [
    'DoubleMu',
    'DYToMuMu_20',
    'Wjets'
]

