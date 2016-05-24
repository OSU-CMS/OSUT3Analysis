#!/usr/bin/env python

config_file = "singleElectronAnalyzer_cfg.py"
#intLumi = 19183. # Full SingleElectron
intLumi = 19183. # same for HLT_Ele27_WP80_v*

datasets = [

####################
### data samples ###
####################

    'SingleElectron',

##########################
### background samples ###
##########################

##     'Wjets',
##     'TTbar',
##     'Diboson',
##     'DY',
##     'QCD_ElectronEnriched',


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
    'QCD_ElectronEnriched',

]

