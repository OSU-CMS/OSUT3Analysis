#!/usr/bin/env python

config_file = "singleMuAnalyzer_cfg.py"
#intLumi = 19417.  # Full SingleMu
intLumi = 18533.  # HLT_IsoMu24_v*

datasets = [

####################
### data samples ###
####################

    'SingleMu',
    
##########################
### background samples ###
##########################

##     'Wjets',
##     'TTbar',
##     'Diboson', 
##     'DY',
##     'QCD_MuEnriched',

   
    'Wjets', 
    'TTbar_Had',
    'TTbar_SemiLep',
    'TTbar_Lep',
    'WW',
    'WZ',
    'ZZ',
    'DYToEE_20',
    'DYToMuMu_20',
    'DYToTauTau_20',
    'QCD_MuEnriched',

]

