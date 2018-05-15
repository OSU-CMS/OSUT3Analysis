#!/usr/bin/env python

# to be run with osusub.py (see https://twiki.cern.ch/twiki/bin/view/CMS/OSUT3AnalysisTutorial)

# import the definitions of all the datasets on the T3
from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.miniAODV2Samples import *

# specify which config file to pass to cmsRun
config_file = "pu_cfg.py"

# choose luminosity used for MC normalization
intLumi = -1 #from 13 TeV MuEG dataset 2015D Silver JSON

# create list of datasets to process
datasets = [
    ############################################################################
    #MiniAOD stored on T3.
    ############################################################################
    #DY
    'DYJetsToLL_50',
    'DYJetsToNuNu',
#
#    #WJets
    'WJetsToLNu',
#
#    #WJets_HT
    'WJetsToLNu_HT100to200',
    'WJetsToLNu_HT200to400',
    'WJetsToLNu_HT400to600',
    'WJetsToLNu_HT600toInf',
#
#    #WW
    'WWToLNuQQ',
    'WWToLNuLNu',
#
#    #WZ
    'WZToLNuQQ',
    'WZToLLQQ',
    'WZToLNuNuNu',
    'WZToLLLNu',
#
#    #ZZ
    'ZZToNuNuQQ',
    'ZZToLLQQ',
    'ZZToLLNuNu',
    'ZZToLLLL',
#
#    #VG
    'WG',
    'ZG',
#
#    #SingleTop
    'SingleTop_s_channel',
    'SingleTop_t_channel',
    'SingleTop_tW',
    'SingleTop_tbarW',

#    #TTJets
    'TTJets_DiLept',
    'TTJets_SingleLeptFromT',
    'TTJets_SingleLeptFromTbar',
#
#    #AMSB chargino
    'AMSB_chargino_100GeV_10cm',
    'AMSB_chargino_100GeV_100cm',
    'AMSB_chargino_100GeV_1000cm',

    'AMSB_chargino_300GeV_10cm',
    'AMSB_chargino_300GeV_100cm',
    'AMSB_chargino_300GeV_1000cm',

    'AMSB_chargino_500GeV_10cm',
    'AMSB_chargino_500GeV_100cm',
    'AMSB_chargino_500GeV_1000cm',

    'AMSB_chargino_700GeV_10cm',
    'AMSB_chargino_700GeV_100cm',
    'AMSB_chargino_700GeV_1000cm',
    ############################################################################

    ############################################################################
    #MiniAOD not stored on T3.
    ############################################################################
#    'DYJetsToLL_10to50',
    'TTJets',
    'ZZ',
    'WW',
    'WZ',
#
    'DYJetsToLL_50_50ns',
    'DYJetsToLL_10to50_50ns',
    'WJetsToLNu_50ns',
    'TTJets_50ns',
    'ZZ_50ns',
    'WW_50ns',
    'WZ_50ns',

#    #QCD MuEnriched
    'QCD_MuEnriched_20toInf',
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
    #QCD EMEnriched
    'QCD_EMEnriched_15to20',
    'QCD_EMEnriched_20to30',
    'QCD_EMEnriched_30to50',
    'QCD_EMEnriched_50to80',
    'QCD_EMEnriched_80to120',
    'QCD_EMEnriched_120to170',
    'QCD_EMEnriched_170to300',
    'QCD_EMEnriched_300toInf',
    #QCD bcToE
    'QCD_bcToE_15to20',
    'QCD_bcToE_20to30',
    'QCD_bcToE_30to80',
    'QCD_bcToE_80to170',
    'QCD_bcToE_170to250',
    'QCD_bcToE_250toInf',
    #DisplacedSUSY Signal MC MiniAOD
    'stop200_1mm',
    'stop200_10mm',
    'stop200_100mm',
    'stop200_1000mm',
    'stop300_1mm',
    'stop300_10mm',
    'stop300_100mm',
    'stop300_1000mm',
    'stop400_1mm',
    'stop400_10mm',
    'stop400_100mm',
    'stop400_1000mm',
    'stop500_1mm',
    'stop500_10mm',
    'stop500_100mm',
    'stop500_1000mm',
    'stop600_1mm',
    'stop600_10mm',
    'stop600_100mm',
    'stop600_1000mm',
    'stop700_1mm',
    'stop700_10mm',
    'stop700_100mm',
    'stop700_1000mm',
    'stop800_1mm',
    'stop800_10mm',
    'stop800_100mm',
    'stop800_1000mm',
    'stop900_1mm',
    'stop900_10mm',
    'stop900_100mm',
    'stop900_1000mm',
    'stop1000_1mm',
    'stop1000_10mm',
    'stop1000_100mm',
    'stop1000_1000mm',
    'stop1100_1mm',
    'stop1100_10mm',
    'stop1100_100mm',
    'stop1100_1000mm',
    'stop1200_1mm',
    'stop1200_10mm',
    'stop1200_100mm',
    'stop1200_1000mm',
]

InputCondorArguments = {'hold': 'true'}
