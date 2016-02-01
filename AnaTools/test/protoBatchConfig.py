#!/usr/bin/env python

# to be run with submitToCondor.py -l protoBatchConfig.py

# import the definitions of all the datasets on the T3
from OSUT3Analysis.Configuration.configurationOptions import *

# specify which config file to pass to cmsRun
config_file = "protoConfig_cfg.py"

# choose luminosity used for MC normalization
intLumi = 19700 # from 8 TeV MuEG dataset

# create list of datasets to process
datasets = [
    'MuonEG_2015D',
    'MuEG_2015D',
    #'MuEG_22Jan2013',
    #'WNjets',
    #'Diboson',
    #'SingleTop',
    #'DY',
    #'TTbar',
]

InputCondorArguments = {}
