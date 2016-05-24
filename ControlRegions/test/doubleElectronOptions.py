#!/usr/bin/env python

config_file = "doubleElectronAnalyzer_cfg.py"
intLumi = 19709.  # DoubleElectron

datasets = [

####################
### data samples ###
####################

    #'DoubleElectron_22Jan2013',

##########################
### background samples ###
##########################

     'WNjets',
     'TTbar',
     'Diboson',
     'DY',
     'QCD_MuEnriched',
     'SingleTop',
    #'Wjets',
    #'WNjets',
    #'TTbar_Had',
    #'TTbar_SemiLep',
    #'TTbar_Lep',
    #'WW',
    #'WZ',
    #'ZZ',
    #'DYToEE_20',
    #'DYToMuMu_20',
    #'DYToTauTau_20',
    #'QCD_MuEnriched',

]
systematics_file = "DisplacedSUSY.Configuration.systematicsDefinitions"

external_systematics_directory = 'DisplacedSUSY/Configuration/data/'
