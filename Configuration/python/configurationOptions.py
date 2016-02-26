#!/usr/bin/env python

config_file = "standardConfig_cfg.py"

###### 2015A = 961 pb-1
###### 2012B = 4.9 fb-1
###### 2012B = 4.9 fb-1
###### 2012B = 4.9 fb-1

intLumi = 2538.43  #2015D Silver Json

datasets = [

   #data
   'DoubleMu_2015',
   'SingleMu_2015',
   'SingleEle_2015',
   'SinglePhoton_2015',
   'DoubleEG_2015',
   'MuonEG_2015',
   'MuonEG_2015D',
   'NoBPTX_2015D',
   'StpPtls_controlSample_2015',

   #MiniAOD not stored on T3.
   'DYJetsToLL_50',
   'DYJetsToLL_10to50',
   'WJetsToLNu',
   'WJetsToLNu_HT',  # HT bins, starting with HT>100 GeV
   'TTJets',
   'Diboson',  
   
   'DYJetsToLL_50_50ns',
   'DYJetsToLL_10to50_50ns',
   'WJetsToLNu_50ns',
   'TTJets_50ns',
   'Diboson_50ns',  
 
    #QCD MuEnriched
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

   #Sum of all backgrounds
   'Background',
   
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

composite_dataset_definitions = {
    'DYJetsToLL' : [
        'DYJetsToLL_50',
        'DYJetsToLL_10to50',
    ],
    'DYJetsToLL_50ns' : [
        'DYJetsToLL_50_50ns',
        'DYJetsToLL_10to50_50ns',
    ],
    'WJetsToLNu_HT' : [
        'WJetsToLNu_HT100to200',
        'WJetsToLNu_HT200to400',
        'WJetsToLNu_HT400to600',
        'WJetsToLNu_HT600toInf',
    ],
    'Diboson' : [
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
        'ZG'
    ],
    'Diboson_50ns' : [
        'WW_50ns',
        'WZ_50ns',
        'ZZ_50ns',
    ],
    'SingleTop' : [
        'SingleTop_s_channel',
        'SingleTop_t_channel',
        'SingleTop_tW',
        'SingleTop_tbarW',
    ],
    'TTJets_Lept' : [
        'TTJets_DiLept',
        'TTJets_SingleLeptFromT',
        'TTJets_SingleLeptFromTbar',
    ],
    'QCD_MuEnriched' : [
        #'QCD_MuEnriched_20toInf', 
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
    ],  
   
    'QCD_EMEnriched' : [
        'QCD_EMEnriched_15to20',    
        'QCD_EMEnriched_20to30',    
        'QCD_EMEnriched_30to50',    
        'QCD_EMEnriched_50to80',    
        'QCD_EMEnriched_80to120',   
        'QCD_EMEnriched_120to170',  
        'QCD_EMEnriched_170to300',  
        'QCD_EMEnriched_300toInf', 
    ],  
    
    'QCD_bcToE' : [
        'QCD_bcToE_15to20',       
        'QCD_bcToE_20to30',       
        'QCD_bcToE_30to80',       
        'QCD_bcToE_80to170',      
        'QCD_bcToE_170to250',     
        'QCD_bcToE_250toInf',     
    ],
    'DoubleMu_2015' : [
        'DoubleMu_2015B',
        'DoubleMu_2015C',
        'DoubleMu_2015D_v3',
        'DoubleMu_2015D_v4',
    ],
    'SingleMu_2015' : [
        'SingleMu_2015B',
        'SingleMu_2015C',
        'SingleMu_2015D_v3',
        'SingleMu_2015D_v4',
    ],
    'SingleMu_2015D' : [
        'SingleMu_2015D_v3',
        'SingleMu_2015D_v4',
    ],
    'MET_2015D' : [
        'MET_2015D_v3',
        'MET_2015D_v4',
    ],
    'SingleEle_2015' : [
        'SingleEle_2015B',
        'SingleEle_2015C',
        'SingleEle_2015D_v3',
        'SingleEle_2015D_v4',
    ],
    'SingleEle_2015D' : [
        'SingleEle_2015D_v3',
        'SingleEle_2015D_v4',
    ],
    'SinglePhoton_2015' : [
        'SinglePhoton_2015B',
        'SinglePhoton_2015C',
        'SinglePhoton_2015D',
    ],
    'DoubleEG_2015' : [
        'DoubleEG_2015B',
        'DoubleEG_2015C',
        'DoubleEG_2015D_v3',
        'DoubleEG_2015D_v4',
    ],
    'MuonEG_2015' : [
        'MuonEG_2015B',
        'MuonEG_2015C',
        'MuonEG_2015D_v3',
        'MuonEG_2015D_v4',
    ],
    'MuonEG_2015D' : [
        'MuonEG_2015D_v3',
        'MuonEG_2015D_v4',
    ],
    'NoBPTX_2015D' : [
        'NoBPTX_2015D_v3',
        'NoBPTX_2015D_v4',
    ],
   'StpPtls_controlSample_2015' : [
        'Commissioning2015_controlSample',
        'NoBPTX_2015B_controlSample',
        'NoBPTX_2015C_controlSample',
        ],
    'DisplacedSUSYSignal' : [
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
    ],
}


############################################################################################################
#########  DATASET-SPECIFIC OPTIONS  ########################################################################
############################################################################################################

#### NOTE: dataset_names dictionary has been moved outside this file, please import the desired list separately

nJobs = {
    ############################################################################
    #MiniAOD stored on T3.
    ############################################################################
    #DY
    'DYJetsToLL_50'  :  999,
    'DYJetsToNuNu'   :  999,

    #WJets
    'WJetsToLNu'  :  999,
    'WJetsToLNu_HT100to200'  :  999,
    'WJetsToLNu_HT200to400'  :  999,
    'WJetsToLNu_HT400to600'  :  999,
    'WJetsToLNu_HT600toInf'  :  999,

    #WW
    'WWToLNuQQ'   :  999,
    'WWToLNuLNu'  :  999,

    #WZ
    'WZToLNuQQ'    :  999,
    'WZToLLQQ'     :  999,
    'WZToLNuNuNu'  :  999,
    'WZToLLLNu'    :  999,

    #ZZ
    'ZZToNuNuQQ'  :  999,
    'ZZToLLQQ'    :  999,
    'ZZToLLNuNu'  :  999,
    'ZZToLLLL'    :  999,

    #VG
    'WG'  :  999,
    'ZG'  :  999,

    #SingleTop
    'SingleTop_s_channel'  :  999,
    'SingleTop_t_channel'  :  999,
    'SingleTop_tW'         :  999,
    'SingleTop_tbarW'      :  999,

    #TTJets
    'TTJets_DiLept'              :  300,
    'TTJets_SingleLeptFromT'     :  300,
    'TTJets_SingleLeptFromTbar'  :  300,

    #MET data
    'MET_2015D_v3' : 999, 
    'MET_2015D_v4' : 999,

    #AMSB chargino
    'AMSB_chargino_100GeV_10cm'    :  99,
    'AMSB_chargino_100GeV_100cm'   :  99,
    'AMSB_chargino_100GeV_1000cm'  :  99,

    'AMSB_chargino_300GeV_10cm'    :  99,
    'AMSB_chargino_300GeV_100cm'   :  99,
    'AMSB_chargino_300GeV_1000cm'  :  99,

    'AMSB_chargino_500GeV_10cm'    :  99,
    'AMSB_chargino_500GeV_100cm'   :  99,
    'AMSB_chargino_500GeV_1000cm'  :  99,

    'AMSB_chargino_700GeV_10cm'    :  99,
    'AMSB_chargino_700GeV_100cm'   :  99,
    'AMSB_chargino_700GeV_1000cm'  :  99,
    ############################################################################

    ############################################################################
    #MiniAOD not stored on T3.
    ############################################################################
    'DYJetsToLL_10to50'    : 999,
    'TTJets'        : 325,
    'WW'        : 44,
    'ZZ'        : 36,
    'WZ'        : 45,
    
    'DYJetsToLL_50_50ns'        : 491,
    'DYJetsToLL_10to50_50ns'    : 396,
    'WJetsToLNu_50ns'        : 634,
    'TTJets_50ns'        : 150,
    'WW_50ns'        : 44,
    'ZZ_50ns'        : 36,
    'WZ_50ns'        : 45,
    
    #QCD MuEnriched
    'QCD_MuEnriched_20toInf'        : 999, 
    'QCD_MuEnriched_15to20'         : 999,
    'QCD_MuEnriched_20to30'         : 999,
    'QCD_MuEnriched_30to50'         : 999,
    'QCD_MuEnriched_50to80'         : 999,
    'QCD_MuEnriched_80to120'        : 999,
    'QCD_MuEnriched_120to170'       : 999,
    'QCD_MuEnriched_170to300'       : 999,
    'QCD_MuEnriched_300to470'       : 999,
    'QCD_MuEnriched_470to600'       : 999,
    'QCD_MuEnriched_600to800'       : 999,
    'QCD_MuEnriched_800to1000'      : 999,
    'QCD_MuEnriched_1000toInf'      : 999,
    
    #QCD EMEnriched
    'QCD_EMEnriched_15to20'         : 69,
    'QCD_EMEnriched_20to30'         : 261,
    'QCD_EMEnriched_30to50'         : 135,
    'QCD_EMEnriched_50to80'         : 145,
    'QCD_EMEnriched_80to120'        : 201,
    'QCD_EMEnriched_120to170'       : 233,
    'QCD_EMEnriched_170to300'       : 130,
    'QCD_EMEnriched_300toInf'       : 114,
    
    #QCD bcToE
    'QCD_bcToE_15to20'              : 21, 
    'QCD_bcToE_20to30'              : 123,
    'QCD_bcToE_30to80'              : 129,
    'QCD_bcToE_80to170'             : 153,
    'QCD_bcToE_170to250'            : 98,
    'QCD_bcToE_250toInf'            : 99,
    
     #Signal MC MiniAOD
    'stop200_1mm'     :  99,     
    'stop200_10mm'     :  99,   
    'stop200_100mm'     :  99,   
    'stop200_1000mm'     :  99,  
    'stop300_1mm'     :  99,
    'stop300_10mm'     :  99,
    'stop300_100mm'     :  99,
    'stop300_1000mm'     :  99,
    'stop400_1mm'     :  99,
    'stop400_10mm'     :  99,
    'stop400_100mm'     :  99,
    'stop400_1000mm'     :  99,
    'stop500_1mm'     :  99,
    'stop500_10mm'     :  99,
    'stop500_100mm'     :  99,
    'stop500_1000mm'     :  99,
    'stop600_1mm'     :  99,
    'stop600_10mm'     :  99,
    'stop600_100mm'     :  99,
    'stop600_1000mm'     :  99,
    'stop700_1mm'     :  99,
    'stop700_10mm'     :  99,
    'stop700_100mm'     :  99,
    'stop700_1000mm'     :  99,
    'stop800_1mm'     :  99,
    'stop800_10mm'     :  99,
    'stop800_100mm'     :  99,
    'stop800_1000mm'     :  99,
    'stop900_1mm'     :  99,
    'stop900_10mm'     :  99,
    'stop900_100mm'     :  99,
    'stop900_1000mm'     :  99,
    'stop1000_1mm'     :  99,
    'stop1000_10mm'     :  99,
    'stop1000_100mm'     :  99,
    'stop1000_1000mm'     :  99,
    'stop1100_1mm'     :  99,
    'stop1100_10mm'     :  99,
    'stop1100_100mm'     :  99,
    'stop1100_1000mm'     :  99,
    'stop1200_1mm'     :  99,
    'stop1200_10mm'     :  99,
    'stop1200_100mm'     :  99,
    'stop1200_1000mm'     :  99,
    
 
    'DoubleMu_2015B'   :  40,
    'DoubleMu_2015C'   : 187,
    'DoubleMu_2015D_v3': 1000,
    'DoubleMu_2015D_v4': 1000, 
    
    'SingleMu_2015B'      :  57,
    'SingleMu_2015C'      : 220,
    'SingleMu_2015D_v3'   : 1000,
    'SingleMu_2015D_v4'   : 1000,
   
    'SingleEle_2015B'    :  77,
    'SingleEle_2015C'    : 137,
    'SingleEle_2015D_v3' : 1000,
    'SingleEle_2015D_v4' : 1000,
   
    'SinglePhoton_2015B'   :  55,
    'SinglePhoton_2015C'   : 130,
    'SinglePhoton_2015D'   : 195,
    
    'DoubleEG_2015B'      :  100,
    'DoubleEG_2015C'      :  298,
    'DoubleEG_2015D_v3'   :  1000,
    'DoubleEG_2015D_v4'   :  1000,
    
    'MuonEG_2015B'   :  30,
    'MuonEG_2015C'   :  82,
    'MuonEG_2015D_v3'   :  1000,
    'MuonEG_2015D_v4'   :  1000,
    
    ############################################################################

    ###########################################################################
    #RECO stored on T3
    ###########################################################################
    #stopped particles stage2 signals
    'GluinoToUUbarChi0_1200_200' : 1,
    'GluinoToUUbarChi0_1200_600' : 3,
    'GluinoToUUbarChi0_1200_1000': 5,
    'GluinoToGChi0_1200_200'  : 2,
    'GluinoToGChi0_1200_400'  : 5,
    'GluinoToGChi0_1200_600'  : 4,
    'GluinoToGChi0_1200_800'  : 5,
    'GluinoToGChi0_1200_1000'  : 5,

    #stopped particles cosmic events
    'cosmic_preselection' : 300,

    ###########################################################################
    #RECO not stored on T3
    ###########################################################################
    #NoBPTX data
    'NoBPTX_2015D_v3' : 999,
    'NoBPTX_2015D_v4' : 999,

    #control sample data
    'Commissioning2015_controlSample' : 999,
    'NoBPTX_2015B_controlSample' : 999,
    'NoBPTX_2015C_controlSample' : 999,

}

maxEvents = {
    ############################################################################
    #MiniAOD stored on T3.
    ############################################################################
    #DY
    'DYJetsToLL_50'  :  -1,
    'DYJetsToNuNu'   :  -1,

    #WJets
    'WJetsToLNu'  :  -1,
    'WJetsToLNu_HT100to200'  :  -1,
    'WJetsToLNu_HT200to400'  :  -1,
    'WJetsToLNu_HT400to600'  :  -1,
    'WJetsToLNu_HT600toInf'  :  -1,

    #WW
    'WWToLNuQQ'   :  -1,
    'WWToLNuLNu'  :  -1,

    #WZ
    'WZToLNuQQ'    :  -1,
    'WZToLLQQ'     :  -1,
    'WZToLNuNuNu'  :  -1,
    'WZToLLLNu'    :  -1,

    #ZZ
    'ZZToNuNuQQ'  :  -1,
    'ZZToLLQQ'    :  -1,
    'ZZToLLNuNu'  :  -1,
    'ZZToLLLL'    :  -1,

    #VG
    'WG'  :  -1,
    'ZG'  :  -1,

    #SingleTop
    'SingleTop_s_channel'  :  -1,
    'SingleTop_t_channel'  :  -1,
    'SingleTop_tW'         :  -1,
    'SingleTop_tbarW'      :  -1,

    #TTJets
    'TTJets_DiLept'              :  -1,
    'TTJets_SingleLeptFromT'     :  -1,
    'TTJets_SingleLeptFromTbar'  :  -1,

    #MET data
    'MET_2015D_v3' : -1,
    'MET_2015D_v4' : -1,

    #AMSB chargino
    'AMSB_chargino_100GeV_10cm'    :  -1,
    'AMSB_chargino_100GeV_100cm'   :  -1,
    'AMSB_chargino_100GeV_1000cm'  :  -1,

    'AMSB_chargino_300GeV_10cm'    :  -1,
    'AMSB_chargino_300GeV_100cm'   :  -1,
    'AMSB_chargino_300GeV_1000cm'  :  -1,

    'AMSB_chargino_500GeV_10cm'    :  -1,
    'AMSB_chargino_500GeV_100cm'   :  -1,
    'AMSB_chargino_500GeV_1000cm'  :  -1,

    'AMSB_chargino_700GeV_10cm'    :  -1,
    'AMSB_chargino_700GeV_100cm'   :  -1,
    'AMSB_chargino_700GeV_1000cm'  :  -1,
    ############################################################################

    ############################################################################
    #MiniAOD not stored on T3.
    ############################################################################
    'DYJetsToLL_10to50'        : -1,
    'TTJets'            : -1,
    'WW'            : -1,
    'ZZ'            : -1,
    'WZ'            : -1,
    
    'DYJetsToLL_50_50ns'        : -1,
    'DYJetsToLL_10to50_50ns'        : -1,
    'WJetsToLNu_50ns'        : -1,
    'TTJets_50ns'            : -1,
    'WW_50ns'            : -1,
    'ZZ_50ns'            : -1,
    'WZ_50ns'            : -1,
    #QCD MuEnriched
    'QCD_MuEnriched_20toInf'        : -1, 
    'QCD_MuEnriched_15to20'         : -1,
    'QCD_MuEnriched_20to30'         : -1,
    'QCD_MuEnriched_30to50'         : -1,
    'QCD_MuEnriched_50to80'         : -1,
    'QCD_MuEnriched_80to120'        : -1,
    'QCD_MuEnriched_120to170'       : -1,
    'QCD_MuEnriched_170to300'       : -1,
    'QCD_MuEnriched_300to470'       : -1,
    'QCD_MuEnriched_470to600'       : -1,
    'QCD_MuEnriched_600to800'       : -1,
    'QCD_MuEnriched_800to1000'      : -1,
    'QCD_MuEnriched_1000toInf'      : -1,
    
    #QCD EMEnriched
    'QCD_EMEnriched_15to20'         : -1,
    'QCD_EMEnriched_20to30'         : -1,
    'QCD_EMEnriched_30to50'         : -1,
    'QCD_EMEnriched_50to80'         : -1,
    'QCD_EMEnriched_80to120'        : -1,
    'QCD_EMEnriched_120to170'       : -1,
    'QCD_EMEnriched_170to300'       : -1,
    'QCD_EMEnriched_300toInf'       : -1,
   
    #QCD bcToE
    'QCD_bcToE_15to20'              : -1, 
    'QCD_bcToE_20to30'              : -1,
    'QCD_bcToE_30to80'              : -1,
    'QCD_bcToE_80to170'             : -1,
    'QCD_bcToE_170to250'            : -1,
    'QCD_bcToE_250toInf'            : -1,
    
    #DisplacedSUSY Signal MC MiniAOD
    'stop200_1mm'     :  -1,     
    'stop200_10mm'     :  -1,   
    'stop200_100mm'     :  -1,   
    'stop200_1000mm'     :  -1,  
    'stop300_1mm'     :  -1,
    'stop300_10mm'     :  -1,
    'stop300_100mm'     :  -1,
    'stop300_1000mm'     :  -1,
    'stop400_1mm'     :  -1,
    'stop400_10mm'     :  -1,
    'stop400_100mm'     :  -1,
    'stop400_1000mm'     :  -1,
    'stop500_1mm'     :  -1,
    'stop500_10mm'     :  -1,
    'stop500_100mm'     :  -1,
    'stop500_1000mm'     :  -1,
    'stop600_1mm'     :  -1,
    'stop600_10mm'     :  -1,
    'stop600_100mm'     :  -1,
    'stop600_1000mm'     :  -1,
    'stop700_1mm'     :  -1,
    'stop700_10mm'     :  -1,
    'stop700_100mm'     :  -1,
    'stop700_1000mm'     :  -1,
    'stop800_1mm'     :  -1,
    'stop800_10mm'     :  -1,
    'stop800_100mm'     :  -1,
    'stop800_1000mm'     :  -1,
    'stop900_1mm'     :  -1,
    'stop900_10mm'     :  -1,
    'stop900_100mm'     :  -1,
    'stop900_1000mm'     :  -1,
    'stop1000_1mm'     :  -1,
    'stop1000_10mm'     :  -1,
    'stop1000_100mm'     :  -1,
    'stop1000_1000mm'     :  -1,
    'stop1100_1mm'     :  -1,
    'stop1100_10mm'     :  -1,
    'stop1100_100mm'     :  -1,
    'stop1100_1000mm'     :  -1,
    'stop1200_1mm'     :  -1,
    'stop1200_10mm'     :  -1,
    'stop1200_100mm'     :  -1,
    'stop1200_1000mm'     :  -1,
 
    'DoubleMu_2015B'       : -1,
    'DoubleMu_2015C'       : -1,
    'DoubleMu_2015D_v3'    : -1,
    'DoubleMu_2015D_v4'    : -1,
    
    'SingleMu_2015B'       : -1,
    'SingleMu_2015C'       : -1,
    'SingleMu_2015D_v3'       : -1,
    'SingleMu_2015D_v4'       : -1,
   
    'SingleEle_2015B'       : -1,
    'SingleEle_2015C'       : -1,
    'SingleEle_2015D_v3'       : -1,
    'SingleEle_2015D_v4'       : -1,
   
    'SinglePhoton_2015B'       : -1,
    'SinglePhoton_2015C'       : -1,
    'SinglePhoton_2015D'       : -1,
    
    'DoubleEG_2015B'       : -1,
    'DoubleEG_2015C'       : -1,
    'DoubleEG_2015D_v3'       : -1,
    'DoubleEG_2015D_v4'       : -1,
    
    'MuonEG_2015B'       : -1,
    'MuonEG_2015C'       : -1,
    'MuonEG_2015D_v3'       : -1,
    'MuonEG_2015D_v4'       : -1,
    
    ############################################################################
    ###########################################################################
    #RECO stored on T3
    ###########################################################################
    #stopped particles stage2 signals
    'GluinoToUUbarChi0_1200_200' : -1,
    'GluinoToUUbarChi0_1200_600' : -1,
    'GluinoToUUbarChi0_1200_1000': -1,
    'GluinoToGChi0_1200_200'  : -1,
    'GluinoToGChi0_1200_400'  : -1,
    'GluinoToGChi0_1200_600'  : -1,
    'GluinoToGChi0_1200_800'  : -1,
    'GluinoToGChi0_1200_1000'  : -1,
    #stopped particles cosmic events
    'cosmic_preselection' : -1,

    ###########################################################################
    #RECO not stored on T3
    ###########################################################################
    #NoBPTX data
    'NoBPTX_2015D_v3' : -1,
    'NoBPTX_2015D_v4' : -1,

    #control sample data
    'Commissioning2015_controlSample' : -1,
    'NoBPTX_2015B_controlSample' : -1,
    'NoBPTX_2015C_controlSample' : -1,

}

# bgMC => background MC process
# signalMC => signal MC process
# data => data
types = {
    ############################################################################
    #MiniAOD stored on T3.
    ############################################################################
    #DY
    'DYJetsToLL_50'  :  "bgMC",
    'DYJetsToNuNu'   :  "bgMC",

    #WJets
    'WJetsToLNu'  :  "bgMC",
    'WJetsToLNu_HT'          :  "bgMC",
    'WJetsToLNu_HT100to200'  :  "bgMC",
    'WJetsToLNu_HT200to400'  :  "bgMC",
    'WJetsToLNu_HT400to600'  :  "bgMC",
    'WJetsToLNu_HT600toInf'  :  "bgMC",

    #WW
    'WWToLNuQQ'   :  "bgMC",
    'WWToLNuLNu'  :  "bgMC",

    #WZ
    'WZToLNuQQ'    :  "bgMC",
    'WZToLLQQ'     :  "bgMC",
    'WZToLNuNuNu'  :  "bgMC",
    'WZToLLLNu'    :  "bgMC",

    #ZZ
    'ZZToNuNuQQ'  :  "bgMC",
    'ZZToLLQQ'    :  "bgMC",
    'ZZToLLNuNu'  :  "bgMC",
    'ZZToLLLL'    :  "bgMC",

    #VG
    'WG'  :  "bgMC",
    'ZG'  :  "bgMC",

    #SingleTop
    'SingleTop'  :  "bgMC",
    'SingleTop_s_channel'  :  "bgMC",
    'SingleTop_t_channel'  :  "bgMC",
    'SingleTop_tW'         :  "bgMC",
    'SingleTop_tbarW'      :  "bgMC",

    #TTJets
    'TTJets_Lept'                :  "bgMC",
    'TTJets_DiLept'              :  "bgMC",
    'TTJets_SingleLeptFromT'     :  "bgMC",
    'TTJets_SingleLeptFromTbar'  :  "bgMC",

    #MET data
    'MET_2015D_v3' : "data",
    'MET_2015D_v4' : "data",
    'MET_2015D' : "data",

    #AMSB chargino
    'AMSB_chargino_100GeV_10cm'    :  "signalMC",
    'AMSB_chargino_100GeV_100cm'   :  "signalMC",
    'AMSB_chargino_100GeV_1000cm'  :  "signalMC",

    'AMSB_chargino_300GeV_10cm'    :  "signalMC",
    'AMSB_chargino_300GeV_100cm'   :  "signalMC",
    'AMSB_chargino_300GeV_1000cm'  :  "signalMC",

    'AMSB_chargino_500GeV_10cm'    :  "signalMC",
    'AMSB_chargino_500GeV_100cm'   :  "signalMC",
    'AMSB_chargino_500GeV_1000cm'  :  "signalMC",

    'AMSB_chargino_700GeV_10cm'    :  "signalMC",
    'AMSB_chargino_700GeV_100cm'   :  "signalMC",
    'AMSB_chargino_700GeV_1000cm'  :  "signalMC",
    ############################################################################

    ############################################################################
    #MiniAOD not stored on T3.
    ############################################################################
    'DYJetsToLL'                : "bgMC",
    'DYJetsToLL_10to50'     : "bgMC",   
    'TTJets'            : "bgMC",
    'Diboson'            : "bgMC",
    'WW'            : "bgMC",
    'ZZ'            : "bgMC",
    'WZ'            : "bgMC",
   
    'DYJetsToLL_50ns'                : "bgMC",
    'DYJetsToLL_50_50ns'     : "bgMC",   
    'DYJetsToLL_10to50_50ns'     : "bgMC",   
    'WJetsToLNu_50ns'        : "bgMC",   
    'TTJets_50ns'            : "bgMC",
    'Diboson_50ns'            : "bgMC",
    'WW_50ns'            : "bgMC",
    'ZZ_50ns'            : "bgMC",
    'WZ_50ns'            : "bgMC",
    #QCD MuEnriched
    'QCD_MuEnriched_20toInf'        : "bgMC", 
    'QCD_MuEnriched'        : "bgMC", 
    'QCD_MuEnriched_15to20'         : "bgMC",
    'QCD_MuEnriched_20to30'         : "bgMC",
    'QCD_MuEnriched_30to50'         : "bgMC",
    'QCD_MuEnriched_50to80'         : "bgMC",
    'QCD_MuEnriched_80to120'        : "bgMC",
    'QCD_MuEnriched_120to170'       : "bgMC",
    'QCD_MuEnriched_170to300'       : "bgMC",
    'QCD_MuEnriched_300to470'       : "bgMC",
    'QCD_MuEnriched_470to600'       : "bgMC",
    'QCD_MuEnriched_600to800'       : "bgMC",
    'QCD_MuEnriched_800to1000'      : "bgMC",
    'QCD_MuEnriched_1000toInf'      : "bgMC",
    
    #QCD EMEnriched
    'QCD_EMEnriched_15to20'         : "bgMC",
    'QCD_EMEnriched'                : "bgMC",
    'QCD_EMEnriched_20to30'         : "bgMC",
    'QCD_EMEnriched_30to50'         : "bgMC",
    'QCD_EMEnriched_50to80'         : "bgMC",
    'QCD_EMEnriched_80to120'        : "bgMC",
    'QCD_EMEnriched_120to170'       : "bgMC",
    'QCD_EMEnriched_170to300'       : "bgMC",
    'QCD_EMEnriched_300toInf'       : "bgMC",
   
    #QCD bcToE
    'QCD_bcToE'                     : "bgMC", 
    'QCD_bcToE_15to20'              : "bgMC", 
    'QCD_bcToE_20to30'              : "bgMC",
    'QCD_bcToE_30to80'              : "bgMC",
    'QCD_bcToE_80to170'             : "bgMC",
    'QCD_bcToE_170to250'            : "bgMC",
    'QCD_bcToE_250toInf'            : "bgMC",

    'QCDFromData' : "bgMC",
     
    #DisplacedSUSY Signal MC MiniAOD
    'stop200_1mm'     :  "signalMC",     
    'stop200_10mm'     :  "signalMC",   
    'stop200_100mm'     :  "signalMC",   
    'stop200_1000mm'     :  "signalMC",  
    'stop300_1mm'     :  "signalMC",
    'stop300_10mm'     :  "signalMC",
    'stop300_100mm'     :  "signalMC",
    'stop300_1000mm'     :  "signalMC",
    'stop400_1mm'     :  "signalMC",
    'stop400_10mm'     :  "signalMC",
    'stop400_100mm'     :  "signalMC",
    'stop400_1000mm'     :  "signalMC",
    'stop500_1mm'     :  "signalMC",
    'stop500_10mm'     :  "signalMC",
    'stop500_100mm'     :  "signalMC",
    'stop500_1000mm'     :  "signalMC",
    'stop600_1mm'     :  "signalMC",
    'stop600_10mm'     :  "signalMC",
    'stop600_100mm'     :  "signalMC",
    'stop600_1000mm'     :  "signalMC",
    'stop700_1mm'     :  "signalMC",
    'stop700_10mm'     :  "signalMC",
    'stop700_100mm'     :  "signalMC",
    'stop700_1000mm'     :  "signalMC",
    'stop800_1mm'     :  "signalMC",
    'stop800_10mm'     :  "signalMC",
    'stop800_100mm'     :  "signalMC",
    'stop800_1000mm'     :  "signalMC",
    'stop900_1mm'     :  "signalMC",
    'stop900_10mm'     :  "signalMC",
    'stop900_100mm'     :  "signalMC",
    'stop900_1000mm'     :  "signalMC",
    'stop1000_1mm'     :  "signalMC",
    'stop1000_10mm'     :  "signalMC",
    'stop1000_100mm'     :  "signalMC",
    'stop1000_1000mm'     :  "signalMC",
    'stop1100_1mm'     :  "signalMC",
    'stop1100_10mm'     :  "signalMC",
    'stop1100_100mm'     :  "signalMC",
    'stop1100_1000mm'     :  "signalMC",
    'stop1200_1mm'     :  "signalMC",
    'stop1200_10mm'     :  "signalMC",
    'stop1200_100mm'     :  "signalMC",
    'stop1200_1000mm'     :  "signalMC",
 
 
    'DoubleMu_2015B'   : "data",
    'DoubleMu_2015C'   : "data",
    'DoubleMu_2015D'   : "data",
    'DoubleMu_2015D_v3'   : "data",
    'DoubleMu_2015D_v4'   : "data",
    'DoubleMu_2015'    : "data",
    
    'SingleMu_2015B'    : "data",
    'SingleMu_2015C'    : "data",
    'SingleMu_2015D'    : "data",
    'SingleMu_2015D_v3' : "data",
    'SingleMu_2015D_v4' : "data",

    'SingleMu_2015'     : "data",
   
    'SingleEle_2015B'   : "data",
    'SingleEle_2015C'   : "data",
    'SingleEle_2015D_v3': "data",
    'SingleEle_2015D_v4': "data",
    'SingleEle_2015D'   : "data",
   
    'SinglePhoton_2015B'   : "data",
    'SinglePhoton_2015C'   : "data",
    'SinglePhoton_2015D'   : "data",
    'SinglePhoton_2015'    : "data",
    
    'DoubleEG_2015B'   : "data",
    'DoubleEG_2015C'   : "data",
    'DoubleEG_2015D_v3': "data",
    'DoubleEG_2015D_v4': "data",
    'DoubleEG_2015'    : "data",
    
    'MuonEG_2015B'   : "data",
    'MuonEG_2015C'   : "data",
    'MuonEG_2015D'   : "data",
    'MuonEG_2015D_v3'   : "data",
    'MuonEG_2015D_v4'   : "data",
    'MuonEG_2015'    : "data",
    
    ###########################################################################
    #RECO stored on T3
    ###########################################################################
    #stopped particles stage2 signals
    'GluinoToUUbarChi0_1200_200' : "signalMC",
    'GluinoToUUbarChi0_1200_600' : "signalMC",
    'GluinoToUUbarChi0_1200_1000' : "signalMC",
    'GluinoToGChi0_1200_200'  : "signalMC",
    'GluinoToGChi0_1200_400'  : "signalMC",
    'GluinoToGChi0_1200_600'  : "signalMC",
    'GluinoToGChi0_1200_800'  : "signalMC",
    'GluinoToGChi0_1200_1000'  : "signalMC",

    #stopped particles cosmic events
    'cosmic_preselection' : "bgMC",

    ###########################################################################
    #RECO not stored on T3
    ###########################################################################
    #NoBPTX data
    'NoBPTX_2015D_v3' : 'data',
    'NoBPTX_2015D_v4' : 'data',
    'NoBPTX_2015D' : 'data',
    ############################################################################

    #control sample data
    'Commissioning2015_controlSample' : 'data',
    'NoBPTX_2015B_controlSample' : 'data',
    'NoBPTX_2015C_controlSample' : 'data',
    'StpPtls_controlSample_2015' : 'data',
}

colors = {
    ############################################################################
    #MiniAOD stored on T3.
    ############################################################################
    #DY
    'DYJetsToLL_50'  :  410,
    'DYJetsToNuNu'   :  831,

    #WJets
    'WJetsToLNu'             :  852,
    'WJetsToLNu_HT'          :  852,
    'WJetsToLNu_HT100to200'  :  852,
    'WJetsToLNu_HT200to400'  :  852,
    'WJetsToLNu_HT400to600'  :  852,
    'WJetsToLNu_HT600toInf'  :  852,

    #WW
    'WWToLNuQQ'   :  390,
    'WWToLNuLNu'  :  390,

    #WZ
    'WZToLNuQQ'    :  393,
    'WZToLLQQ'     :  393,
    'WZToLNuNuNu'  :  393,
    'WZToLLLNu'    :  393,

    #ZZ
    'ZZToNuNuQQ'  :  397,
    'ZZToLLQQ'    :  397,
    'ZZToLLNuNu'  :  397,
    'ZZToLLLL'    :  397,

    #VG
    'WG'  :  399,
    'ZG'  :  398,

    #SingleTop
    'SingleTop'            :  607,
    'SingleTop_s_channel'  :  905,
    'SingleTop_t_channel'  :  907,
    'SingleTop_tW'         :  909,
    'SingleTop_tbarW'      :  910,

    #TTJets
    'TTJets_Lept'                :  872,
    'TTJets_DiLept'              :  873,
    'TTJets_SingleLeptFromT'     :  872,
    'TTJets_SingleLeptFromTbar'  :  872,

    #MET data
    'MET_2015D_v3' : 1,
    'MET_2015D_v4' : 1,
    'MET_2015D' : 1,

    #AMSB chargino
    'AMSB_chargino_100GeV_10cm'    :  628,
    'AMSB_chargino_100GeV_100cm'   :  596,
    'AMSB_chargino_100GeV_1000cm'  :  923,

    'AMSB_chargino_300GeV_10cm'    :  628,
    'AMSB_chargino_300GeV_100cm'   :  596,
    'AMSB_chargino_300GeV_1000cm'  :  923,

    'AMSB_chargino_500GeV_10cm'    :  628,
    'AMSB_chargino_500GeV_100cm'   :  596,
    'AMSB_chargino_500GeV_1000cm'  :  923,

    'AMSB_chargino_700GeV_10cm'    :  628,
    'AMSB_chargino_700GeV_100cm'   :  596,
    'AMSB_chargino_700GeV_1000cm'  :  923,
    ############################################################################

    ############################################################################
    #MiniAOD not stored on T3.
    ############################################################################
    'DYJetsToLL'          : 410,
    'DYJetsToLL_10to50'   : 410,
    'TTJets'              : 872,
    'Diboson'             : 393,
    'WZ'                  : 393,
    'ZZ'                  : 397,
    'WW'                  : 390,
    
    'DYJetsToLL_50ns'          : 410,
    'DYJetsToLL_50_50ns'       : 410,
    'DYJetsToLL_10to50_50ns'   : 410,
    'WJetsToLNu_50ns'          : 852,
    'TTJets_50ns'              : 872,
    'Diboson_50ns'             : 393,
    'WZ_50ns'                  : 393,
    'ZZ_50ns'                  : 397,
    'WW_50ns'                  : 390,
    #QCD MuEnriched
    'QCD_MuEnriched_20toInf'        : 623, 
    'QCD_MuEnriched'                : 623, 
    'QCD_MuEnriched_15to20'         : 623,
    'QCD_MuEnriched_20to30'         : 623,
    'QCD_MuEnriched_30to50'         : 623,
    'QCD_MuEnriched_50to80'         : 623,
    'QCD_MuEnriched_80to120'        : 623,
    'QCD_MuEnriched_120to170'       : 623,
    'QCD_MuEnriched_170to300'       : 623,
    'QCD_MuEnriched_300to470'       : 623,
    'QCD_MuEnriched_470to600'       : 623,
    'QCD_MuEnriched_600to800'       : 623,
    'QCD_MuEnriched_800to1000'      : 623,
    'QCD_MuEnriched_1000toInf'      : 623,
    
    #QCD EMEnriched
    'QCD_EMEnriched'                : 791,
    'QCD_EMEnriched_15to20'         : 791,
    'QCD_EMEnriched_20to30'         : 791,
    'QCD_EMEnriched_30to50'         : 791,
    'QCD_EMEnriched_50to80'         : 791,
    'QCD_EMEnriched_80to120'        : 791,
    'QCD_EMEnriched_120to170'       : 791,
    'QCD_EMEnriched_170to300'       : 791,
    'QCD_EMEnriched_300toInf'       : 791,
   
    #QCD bcToE
    'QCD_bcToE'                     : 794, 
    'QCD_bcToE_15to20'              : 794, 
    'QCD_bcToE_20to30'              : 794,
    'QCD_bcToE_30to80'              : 794,
    'QCD_bcToE_80to170'             : 794,
    'QCD_bcToE_170to250'            : 794,
    'QCD_bcToE_250toInf'            : 794,

    'QCDFromData' : 791,
     
    #Signal MC MiniAOD
    'stop200_1mm'     :  20,     
    'stop200_10mm'     :  21,   
    'stop200_100mm'     :  22,   
    'stop200_1000mm'     :  23,  
    'stop300_1mm'     :  30,
    'stop300_10mm'     :  31,
    'stop300_100mm'     :  32,
    'stop300_1000mm'     :  33,
    'stop400_1mm'     :  40,
    'stop400_10mm'     :  41,
    'stop400_100mm'     :  42,
    'stop400_1000mm'     :  43,
    'stop500_1mm'     :  50,
    'stop500_10mm'     :  51,
    'stop500_100mm'     :  52,
    'stop500_1000mm'     :  53,
    'stop600_1mm'     :  60,
    'stop600_10mm'     :  61,
    'stop600_100mm'     :  62,
    'stop600_1000mm'     :  63,
    'stop700_1mm'     :  70,
    'stop700_10mm'     :  71,
    'stop700_100mm'     :  72,
    'stop700_1000mm'     :  73,
    'stop800_1mm'     :  80,
    'stop800_10mm'     :  81,
    'stop800_100mm'     :  82,
    'stop800_1000mm'     :  83,
    'stop900_1mm'     :  90,
    'stop900_10mm'     :  91,
    'stop900_100mm'     :  92,
    'stop900_1000mm'     :  93,
    'stop1000_1mm'     :  100,
    'stop1000_10mm'     :  101,
    'stop1000_100mm'     :  102,
    'stop1000_1000mm'     :  103,
    'stop1100_1mm'     :  110,
    'stop1100_10mm'     :  111,
    'stop1100_100mm'     :  112,
    'stop1100_1000mm'     :  113,
    'stop1200_1mm'     :  120,
    'stop1200_10mm'     :  121,
    'stop1200_100mm'     :  122,
    'stop1200_1000mm'     :  123,
    
    
    'DoubleMu_2015B'   : 1,
    'DoubleMu_2015C'   : 1,
    'DoubleMu_2015D_v3'   : 1,
    'DoubleMu_2015D_v4'   : 1,
    'DoubleMu_2015'   : 1,
    
    'SingleMu_2015B'   : 1,
    'SingleMu_2015C'   : 1,
    'SingleMu_2015D_v4'   : 1,
    'SingleMu_2015D_v4'   : 1,
    'SingleMu_2015D'   : 1,
    'SingleMu_2015'   : 1,
   
    'SingleEle_2015B'   : 1,
    'SingleEle_2015C'   : 1,
    'SingleEle_2015D'   : 1,
    'SingleEle_2015D_v3'   : 1,
    'SingleEle_2015D_v4'   : 1,
    'SingleEle_2015'   : 1,
   
    'SinglePhoton_2015B'   : 1,
    'SinglePhoton_2015C'   : 1,
    'SinglePhoton_2015D'   : 1,
    'SinglePhoton_2015'   : 1,
    
    'DoubleEG_2015B'   : 1,
    'DoubleEG_2015C'   : 1,
    'DoubleEG_2015D_v3': 1,
    'DoubleEG_2015D_v4': 1,
    'DoubleEG_2015'   : 1,
    
    'MuonEG_2015B'   : 1,
    'MuonEG_2015C'   : 1,
    'MuonEG_2015D'   : 1,
    'MuonEG_2015D_v3'   : 3,
    'MuonEG_2015D_v4'   : 4,
    'MuonEG_2015'    : 1,

    ###########################################################################
    #RECO stored on T3
    ###########################################################################
    #stopped particles stage2 signals
    'GluinoToUUbarChi0_1200_200' : 4,
    'GluinoToUUbarChi0_1200_600' : 22,
    'GluinoToUUbarChi0_1200_1000' : 2,
    'GluinoToGChi0_1200_200'  : 2,
    'GluinoToGChi0_1200_400'  : 3,
    'GluinoToGChi0_1200_600'  : 6,
    'GluinoToGChi0_1200_800'  : 7,
    'GluinoToGChi0_1200_1000'  : 4,

    #stopped particles cosmic events
    'cosmic_preselection' : 2,

    ############################################################################
   
    #NoBPTX data
    'NoBPTX_2015D_v3' : 1,
    'NoBPTX_2015D_v4' : 1,
    'NoBPTX_2015D' : 1,
    ############################################################################

    #control sample data
    'Commissioning2015_controlSample' : 1,
    'NoBPTX_2015B_controlSample' : 1,
    'NoBPTX_2015C_controlSample' : 1,
    'StpPtls_controlSample_2015' : 1,
}

labels = {
    ############################################################################
    #MiniAOD stored on T3.
    ############################################################################
    #DY
    'DYJetsToLL_50'  :  "Z#rightarrowl^{+}l^{-} M(50) ",
    'DYJetsToLL'     :  "Z#rightarrowl^{+}l^{-} M(10) ",
    'DYJetsToLL_10to50'     :  "Z#rightarrowl^{+}l^{-} M(10-50) ",
    'DYJetsToNuNu'   :  "Z#rightarrow#nu#bar{#nu} ",

    #WJets
    'WJetsToLNu'            :  "W#rightarrowl#nu ",
    'WJetsToLNu_HT'         :  "W#rightarrowl#nu (H_{T}>100 GeV) ",
    'WJetsToLNu_HT100to200' :  "W#rightarrowl#nu (100<H_{T}<200 GeV) ",
    'WJetsToLNu_HT200to400' :  "W#rightarrowl#nu (200<H_{T}<400 GeV) ",
    'WJetsToLNu_HT400to600' :  "W#rightarrowl#nu (400<H_{T}<600 GeV) ",
    'WJetsToLNu_HT600toInf' :  "W#rightarrowl#nu (H_{T}>600 GeV) ",

    #WW
    'WWToLNuQQ'   :  "WW#rightarrowl#nuqq",
    'WWToLNuLNu'  :  "WW#rightarrowl#nul#nu",

    #WZ
    'WZToLNuQQ'    :  "WZ#rightarrowl#nuqq",
    'WZToLLQQ'     :  "WZ#rightarrowqqll",
    'WZToLNuNuNu'  :  "WZ#rightarrowl#nu#nu#nu",
    'WZToLLLNu'    :  "WZ#rightarrowl#null",

    #ZZ
    'ZZToNuNuQQ'  :  "ZZ#rightarrow#nu#nuqq",
    'ZZToLLQQ'    :  "ZZ#rightarrowllqq",
    'ZZToLLNuNu'  :  "ZZ#rightarrowll#nu#nu",
    'ZZToLLLL'    :  "ZZ#rightarrowllll",

    #VG
    'WG'  :  "W#gamma#rightarrowl#nu#gamma",
    'ZG'  :  "Z#gamma#rightarrowll#gamma",

    #SingleTop
    'SingleTop'            :  "Single top",
    'SingleTop_s_channel'  :  "Single top (s-channel)",
    'SingleTop_t_channel'  :  "Single top (t-channel)",
    'SingleTop_tW'         :  "Single top (tW)",
    'SingleTop_tbarW'      :  "Single top (#bar{t}W)",

    #TTJets
    'TTJets_Lept'                :  "t#bar{t}",
    'TTJets_DiLept'              :  "t#bar{t} (fully leptonic)",
    'TTJets_SingleLeptFromT'     :  "t#bar{t} (single lepton from t)",
    'TTJets_SingleLeptFromTbar'  :  "t#bar{t} (single lepton from #bar{t})",

    #MET data
    'MET_2015D_v3' : "MET 2015D-v3 data",
    'MET_2015D_v4' : "MET 2015D-v4 data",
    'MET_2015D' : "MET 2015D data",

    #AMSB chargino
    'AMSB_chargino_100GeV_10cm'    :  "AMSB #tilde{#chi}^{#pm} (100 GeV, 10cm)",
    'AMSB_chargino_100GeV_100cm'   :  "AMSB #tilde{#chi}^{#pm} (100 GeV, 100cm)",
    'AMSB_chargino_100GeV_1000cm'  :  "AMSB #tilde{#chi}^{#pm} (100 GeV, 1000cm)",

    'AMSB_chargino_300GeV_10cm'    :  "AMSB #tilde{#chi}^{#pm} (300 GeV, 10cm)",
    'AMSB_chargino_300GeV_100cm'   :  "AMSB #tilde{#chi}^{#pm} (300 GeV, 100cm)",
    'AMSB_chargino_300GeV_1000cm'  :  "AMSB #tilde{#chi}^{#pm} (300 GeV, 1000cm)",

    'AMSB_chargino_500GeV_10cm'    :  "AMSB #tilde{#chi}^{#pm} (500 GeV, 10cm)",
    'AMSB_chargino_500GeV_100cm'   :  "AMSB #tilde{#chi}^{#pm} (500 GeV, 100cm)",
    'AMSB_chargino_500GeV_1000cm'  :  "AMSB #tilde{#chi}^{#pm} (500 GeV, 1000cm)",

    'AMSB_chargino_700GeV_10cm'    :  "AMSB #tilde{#chi}^{#pm} (700 GeV, 10cm)",
    'AMSB_chargino_700GeV_100cm'   :  "AMSB #tilde{#chi}^{#pm} (700 GeV, 100cm)",
    'AMSB_chargino_700GeV_1000cm'  :  "AMSB #tilde{#chi}^{#pm} (700 GeV, 1000cm)",
    ############################################################################

    ############################################################################
    #MiniAOD not stored on T3.
    ############################################################################
    'DYJetsToLL_10to50'             : "Z#rightarrowll M(10to50) ",
    'TTJets'                        : "t#bar{t} ",
    'Diboson'                   : "diboson ",
    'WW'                        : "WW ",
    'WZ'                        : "WZ ",
    'ZZ'                        : "ZZ ",
    
    'DYJetsToLL_50_50ns'                 : "Z#rightarrowll M(50) 50ns ",
    'DYJetsToLL_10to50_50ns'             : "Z#rightarrowll M(10to50) 50ns ",
    'WJetsToLNu_50ns'                    : "W#rightarrowl#nu 50ns ",
    'TTJets_50ns'                        : "t#bar{t} 50ns ",
    'Diboson_50ns'                   : "diboson 50ns ",
    'WW_50ns'                        : "WW 50ns ",
    'WZ_50ns'                        : "WZ 50ns ",
    'ZZ_50ns'                        : "ZZ 50ns ",
    #QCD MuEnriched
    'QCD_MuEnriched_20toInf'        : "QCD MuEnriched Pt 20-Inf ", 
    'QCD_MuEnriched'                : "QCD MuEnriched ", 
    'QCD_MuEnriched_15to20'         : "QCD MuEnriched Pt 15-20 ",
    'QCD_MuEnriched_20to30'         : "QCD MuEnriched Pt 20-30 ",
    'QCD_MuEnriched_30to50'         : "QCD MuEnriched Pt 30-50 ",
    'QCD_MuEnriched_50to80'         : "QCD MuEnriched Pt 50-80 ",
    'QCD_MuEnriched_80to120'        : "QCD MuEnriched Pt 80-120 ",
    'QCD_MuEnriched_120to170'       : "QCD MuEnriched Pt 120-170 ",
    'QCD_MuEnriched_170to300'       : "QCD MuEnriched Pt 170-300 ",
    'QCD_MuEnriched_300to470'       : "QCD MuEnriched Pt 300-470 ",
    'QCD_MuEnriched_470to600'       : "QCD MuEnriched Pt 470-600 ",
    'QCD_MuEnriched_600to800'       : "QCD MuEnriched Pt 600-800 ",
    'QCD_MuEnriched_800to1000'      : "QCD MuEnriched Pt 800-1000 ",
    'QCD_MuEnriched_1000toInf'      : "QCD MuEnriched Pt 1000-Inf ",
    
    #QCD EMEnriched
    'QCD_EMEnriched'                : "QCD EMEnriched",
    'QCD_EMEnriched_15to20'         : "QCD EMEnriched Pt 15-20 ",
    'QCD_EMEnriched_20to30'         : "QCD EMEnriched Pt 20-30 ",
    'QCD_EMEnriched_30to50'         : "QCD EMEnriched Pt 30-50 ",
    'QCD_EMEnriched_50to80'         : "QCD EMEnriched Pt 50-80 ",
    'QCD_EMEnriched_80to120'        : "QCD EMEnriched Pt 80-120 ",
    'QCD_EMEnriched_120to170'       : "QCD EMEnriched Pt 120-170 ",
    'QCD_EMEnriched_170to300'       : "QCD EMEnriched Pt 170-300 ",
    'QCD_EMEnriched_300toInf'       : "QCD EMEnriched Pt 300-Inf ",
   
    #QCD bcToE
    'QCD_bcToE'                     : "QCD bcToE", 
    'QCD_bcToE_15to20'              : "QCD bcToE Pt 15-20 ", 
    'QCD_bcToE_20to30'              : "QCD bcToE Pt 20-30 ",
    'QCD_bcToE_30to80'              : "QCD bcToE Pt 30-80 ",
    'QCD_bcToE_80to170'             : "QCD bcToE Pt 80-170 ",
    'QCD_bcToE_170to250'            : "QCD bcToE Pt 170-250 ",
    'QCD_bcToE_250toInf'            : "QCD bcToE Pt 250-Inf ",

    'QCDFromData'     : "data-driven QCD",
     
    #DisplacedSUSY Signal MC MiniAOD
    'stop200_1mm'    : "#tilde{t}#tilde{t} M(200) c#tau(1mm)", 
    'stop200_10mm'   : "#tilde{t}#tilde{t} M(200) c#tau(10mm)",
    'stop200_100mm'  : "#tilde{t}#tilde{t} M(200) c#tau(100mm)",
    'stop200_1000mm' : "#tilde{t}#tilde{t} M(200) c#tau(1000mm)",
    'stop300_1mm'    : "#tilde{t}#tilde{t} M(300) c#tau(1mm)", 
    'stop300_10mm'   : "#tilde{t}#tilde{t} M(300) c#tau(10mm)",
    'stop300_100mm'  : "#tilde{t}#tilde{t} M(300) c#tau(100mm)",
    'stop300_1000mm' : "#tilde{t}#tilde{t} M(300) c#tau(1000mm)",
    'stop400_1mm'    : "#tilde{t}#tilde{t} M(400) c#tau(1mm)", 
    'stop400_10mm'   : "#tilde{t}#tilde{t} M(400) c#tau(10mm)",
    'stop400_100mm'  : "#tilde{t}#tilde{t} M(400) c#tau(100mm)",
    'stop400_1000mm' : "#tilde{t}#tilde{t} M(400) c#tau(1000mm)",
    'stop500_1mm'    : "#tilde{t}#tilde{t} M(500) c#tau(1mm)", 
    'stop500_10mm'   : "#tilde{t}#tilde{t} M(500) c#tau(10mm)",
    'stop500_100mm'  : "#tilde{t}#tilde{t} M(500) c#tau(100mm)",
    'stop500_1000mm' : "#tilde{t}#tilde{t} M(500) c#tau(1000mm)",
    'stop600_1mm'    : "#tilde{t}#tilde{t} M(600) c#tau(1mm)", 
    'stop600_10mm'   : "#tilde{t}#tilde{t} M(600) c#tau(10mm)",
    'stop600_100mm'  : "#tilde{t}#tilde{t} M(600) c#tau(100mm)",
    'stop600_1000mm' : "#tilde{t}#tilde{t} M(600) c#tau(1000mm)",
    'stop700_1mm'    : "#tilde{t}#tilde{t} M(700) c#tau(1mm)", 
    'stop700_10mm'   : "#tilde{t}#tilde{t} M(700) c#tau(10mm)",
    'stop700_100mm'  : "#tilde{t}#tilde{t} M(700) c#tau(100mm)",
    'stop700_1000mm' : "#tilde{t}#tilde{t} M(700) c#tau(1000mm)",
    'stop800_1mm'    : "#tilde{t}#tilde{t} M(800) c#tau(1mm)", 
    'stop800_10mm'   : "#tilde{t}#tilde{t} M(800) c#tau(10mm)",
    'stop800_100mm'  : "#tilde{t}#tilde{t} M(800) c#tau(100mm)",
    'stop800_1000mm' : "#tilde{t}#tilde{t} M(800) c#tau(1000mm)",
    'stop900_1mm'    : "#tilde{t}#tilde{t} M(900) c#tau(1mm)", 
    'stop900_10mm'   : "#tilde{t}#tilde{t} M(900) c#tau(10mm)",
    'stop900_100mm'  : "#tilde{t}#tilde{t} M(900) c#tau(100mm)",
    'stop900_1000mm' : "#tilde{t}#tilde{t} M(900) c#tau(1000mm)",
    'stop1000_1mm'    : "#tilde{t}#tilde{t} M(1000) c#tau(1mm)", 
    'stop1000_10mm'   : "#tilde{t}#tilde{t} M(1000) c#tau(10mm)",
    'stop1000_100mm'  : "#tilde{t}#tilde{t} M(1000) c#tau(100mm)",
    'stop1000_1000mm' : "#tilde{t}#tilde{t} M(1000) c#tau(1000mm)",
    'stop1100_1mm'    : "#tilde{t}#tilde{t} M(1100) c#tau(1mm)", 
    'stop1100_10mm'   : "#tilde{t}#tilde{t} M(1100) c#tau(10mm)",
    'stop1100_100mm'  : "#tilde{t}#tilde{t} M(1100) c#tau(100mm)",
    'stop1100_1000mm' : "#tilde{t}#tilde{t} M(1100) c#tau(1000mm)",
    'stop1200_1mm'    : "#tilde{t}#tilde{t} M(1200) c#tau(1mm)", 
    'stop1200_10mm'   : "#tilde{t}#tilde{t} M(1200) c#tau(10mm)",
    'stop1200_100mm'  : "#tilde{t}#tilde{t} M(1200) c#tau(100mm)",
    'stop1200_1000mm' : "#tilde{t}#tilde{t} M(1200) c#tau(1000mm)",
    
 
    'DoubleMu_2015'      : "Double Muon 2015 data",
    'DoubleMu_2015B'     : "DoubleMu 2015B data",
    'DoubleMu_2015C'     : "DoubleMu 2015C data",
    'DoubleMu_2015D_v3'  : "DoubleMu 2015D_v3 data",
    'DoubleMu_2015D_v4'  : "DoubleMu 2015D_v4 data",
    
    'SingleMu_2015'      : "Single Muon 2015 data",
    'SingleMu_2015B'     : "SingleMu 2015B data",
    'SingleMu_2015C'     : "SingleMu 2015C data",
    'SingleMu_2015D'     : "SingleMu 2015D data",
    'SingleMu_2015D_v3'     : "SingleMu 2015D_v3 data",
    'SingleMu_2015D_v4'     : "SingleMu 2015D_v4 data",
   
    'SingleEle_2015'      : "Single Electron 2015 data",
    'SingleEle_2015B'     : "SingleEle 2015B data",
    'SingleEle_2015C'     : "SingleEle 2015C data",
    'SingleEle_2015D'     : "SingleEle 2015D data",
    'SingleEle_2015D_v3'     : "SingleEle 2015D_v3 data",
    'SingleEle_2015D_v4'     : "SingleEle 2015D_v4 data",
   
    'SinglePhoton_2015'      : "Single Photonctron 2015 data",
    'SinglePhoton_2015B'     : "SinglePhoton 2015B data",
    'SinglePhoton_2015C'     : "SinglePhoton 2015C data",
    'SinglePhoton_2015D'     : "SinglePhoton 2015D data",
    
    'DoubleEG_2015'      : "DoubleEG 2015 data",
    'DoubleEG_2015B'     : "DoubleEG 2015B data",
    'DoubleEG_2015C'     : "DoubleEG 2015C data",
    'DoubleEG_2015D_v3'  : "DoubleEG 2015D_v3 data",
    'DoubleEG_2015D_v4' : "DoubleEG 2015D_v4 data",
    
    'MuonEG_2015'      : "MuonEG 2015 data",
    'MuonEG_2015B'     : "MuonEG 2015B data",
    'MuonEG_2015C'     : "MuonEG 2015C data",
    'MuonEG_2015D'     : "MuonEG 2015D data",
    'MuonEG_2015D_v3'     : "MuonEG 2015D_v3 data",
    'MuonEG_2015D_v4'     : "MuonEG 2015D_v4 data",

    ###########################################################################
    #RECO stored on T3
    ###########################################################################
    #stopped particles stage2 signals
    'GluinoToUUbarChi0_1200_200' : "#tilde{g} #rightarrow u#topbar{u}#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 200 GeV)",
    'GluinoToUUbarChi0_1200_600' : "#tilde{g} #rightarrow u#topbar{u}#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 600 GeV)",
    'GluinoToUUbarChi0_1200_1000' : "#tilde{g} #rightarrow u#topbar{u}#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 1000 GeV)",
    'GluinoToGChi0_1200_200'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 200 GeV)",
    'GluinoToGChi0_1200_400'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 400 GeV)",
    'GluinoToGChi0_1200_600'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 600 GeV)",
    'GluinoToGChi0_1200_800'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 800 GeV)",
    'GluinoToGChi0_1200_1000'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 1000 GeV)",

    #stopped particles cosmic MC events
    'cosmic_preselection' : "Cosmic MC preselection",

    ############################################################################

    #NoBPTX Data 
    'NoBPTX_2015D_v3'     : "NoBPTX 2015D_v3 data",
    'NoBPTX_2015D_v4'     : "NoBPTX 2015D_v4 data",
    'NoBPTX_2015D'     : "Data",
    ############################################################################

    #control sample data
    'Commissioning2015_controlSample' : "Commissioning 2015 control sample",
    'NoBPTX_2015B_controlSample' : "NoBPTX 2015B control sample",
    'NoBPTX_2015C_controlSample' : "NoBPTX 2015C control sample",
    'StpPtls_controlSample_2015' : "Data control sample",

}

crossSections = {
    # Cross sections for samples stored on T3 should be registered in database.  
    # Cross sections below are for MiniAOD not stored on T3.  
    'DYJetsToLL_50'                 : 6025.2,       # https://twiki.cern.ch/twiki/bin/view/CMS/StandardModelCrossSectionsat13TeV (2008.4 * 3)  
    'WJetsToLNu'                    : 61526.7,      # https://twiki.cern.ch/twiki/bin/view/CMS/StandardModelCrossSectionsat13TeV (20508.9 * 3) 
'TTJets'                        : 831.76,       # https://twiki.cern.ch/twiki/bin/view/LHCPhysics/TtbarNNLO (for top mass = 172.5 GeV)  
    'WW'                       :                63.21, 
    'WZ'                       :                22.82, 
    'ZZ'                       :                10.32,  
#50ns
    'DYJetsToLL_50_50ns'                 : 6025.2,       # https://twiki.cern.ch/twiki/bin/view/CMS/StandardModelCrossSectionsat13TeV (2008.4 * 3)  
    'WJetsToLNu_50ns'                    : 61526.7,      # https://twiki.cern.ch/twiki/bin/view/CMS/StandardModelCrossSectionsat13TeV (20508.9 * 3) 
    'TTJets_50ns'                        : 831.76,       # https://twiki.cern.ch/twiki/bin/view/LHCPhysics/TtbarNNLO (for top mass = 172.5 GeV)  
'WW_50ns'                       :                63.21, 
    'WZ_50ns'                       :                22.82, 
    'ZZ_50ns'                       :                10.32,  
############################################################# 
    # Cross sections below obatined with crossSectionExtractor.py 
    'DYJetsToLL_10to50'        :              18610.0,      
    'DYJetsToLL_10to50_50ns'   :              18610.0,       
    'QCD_MuEnriched_20toInf'   :  720648000.0*0.00042, 
    'QCD_MuEnriched_15to20'    :   1273190000.0*0.003, 
    'QCD_MuEnriched_20to30'    :   558528000.0*0.0053, 
    'QCD_MuEnriched_30to50'    :  139803000.0*0.01182, 
    'QCD_MuEnriched_50to80'    :   19222500.0*0.02276, 
    'QCD_MuEnriched_80to120'   :    2758420.0*0.03844, 
    'QCD_MuEnriched_120to170'  :     469797.0*0.05362, 
    'QCD_MuEnriched_170to300'  :     117989.0*0.07335, 
    'QCD_MuEnriched_300to470'  :      7820.25*0.10196, 
    'QCD_MuEnriched_470to600'  :      645.528*0.12242, 
    'QCD_MuEnriched_600to800'  :      187.109*0.13412, 
    'QCD_MuEnriched_800to1000' :      32.3486*0.14552, 
    'QCD_MuEnriched_1000toInf' :      10.4305*0.15544, 
    # The MC contacts of the EMEnriched samples did not put the
    # filter efficiencies in the sample, these efficiencies are 
    # took from https://github.com/silviodonato/RateEstimate/blob/
    # master/datasetCrossSections/datasetCrossSectionsSpring15.py    
    'QCD_EMEnriched_15to20'    :1279000000.0*0.001309, 
    'QCD_EMEnriched_20to30'    : 557600000.0*0.009914, 
    'QCD_EMEnriched_30to50'    :  136000000.0*0.04988, 
    'QCD_EMEnriched_50to80'    :    19800000.0*0.1145, 
    'QCD_EMEnriched_80to120'   :     2800000.0*0.1507, 
    'QCD_EMEnriched_120to170'  :      477000.0*0.1576, 
    # Got from the McM Tool: https://cms-pdmv.cern.ch/mcm/requests?page=0&dataset_name=QCD_Pt-300toInf_EMEnriched_TuneCUETP8M1_13TeV_pythia8&
    #member_of_campaign=RunIIWinter15GS&shown=163213220927    
    'QCD_EMEnriched_170to300'  :      114000.0*0.1650, 
    'QCD_EMEnriched_300toInf'  :        9000.0*0.1500, 
    #############################################################    
    'QCD_bcToE_15to20'         :  1272980000.0*0.0002, 
    'QCD_bcToE_20to30'         :  557627000.0*0.00059, 
    'QCD_bcToE_30to80'         :  159068000.0*0.00255, 
    'QCD_bcToE_80to170'        :    3221000.0*0.01183, 
    'QCD_bcToE_170to250'       :     105771.0*0.02492, 
    'QCD_bcToE_250toInf'       :      21094.1*0.03375, 
    #############################################################  
    'stop200_1mm'    : 64.5085, 
    'stop200_10mm'   : 64.5085,
    'stop200_100mm'  : 64.5085,
    'stop200_1000mm' : 64.5085,
    'stop300_1mm'    : 8.51615, 
    'stop300_10mm'   : 8.51615,
    'stop300_100mm'  : 8.51615,
    'stop300_1000mm' : 8.51615,
    'stop400_1mm'    : 1.83537, 
    'stop400_10mm'   : 1.83537,
    'stop400_100mm'  : 1.83537,
    'stop400_1000mm' : 1.83537,
    'stop500_1mm'    : 0.51848, 
    'stop500_10mm'   : 0.51848,
    'stop500_100mm'  : 0.51848,
    'stop500_1000mm' : 0.51848,
    'stop600_1mm'    : 0.174599, 
    'stop600_10mm'   : 0.174599,
    'stop600_100mm'  : 0.174599,
    'stop600_1000mm' : 0.174599,
    'stop700_1mm'    : 0.0670476, 
    'stop700_10mm'   : 0.0670476,
    'stop700_100mm'  : 0.0670476,
    'stop700_1000mm' : 0.0670476,
    'stop800_1mm'    : 0.0283338, 
    'stop800_10mm'   : 0.0283338,
    'stop800_100mm'  : 0.0283338,
    'stop800_1000mm' : 0.0283338,
    'stop900_1mm'    : 0.0128895, 
    'stop900_10mm'   : 0.0128895,
    'stop900_100mm'  : 0.0128895,
    'stop900_1000mm' : 0.0128895,
    'stop1000_1mm'    : 0.00615134, 
    'stop1000_10mm'   : 0.00615134,
    'stop1000_100mm'  : 0.00615134,
    'stop1000_1000mm' : 0.00615134,
    'stop1100_1mm'    : 0.00307413, 
    'stop1100_10mm'   : 0.00307413,
    'stop1100_100mm'  : 0.00307413,
    'stop1100_1000mm' : 0.00307413,
    'stop1200_1mm'    : 0.00159844, 
    'stop1200_10mm'   : 0.00159844,
    'stop1200_100mm'  : 0.00159844,
    'stop1200_1000mm' : 0.00159844,
    'WZToLLQQ'       : 5.595,
    
    'WWToLNuQQ'   :  49.997,
    'WWToLNuLNu'  :  12.178,
    'WZToLNuQQ'   :  10.71,
    'WZToLNuNuNu' :  3.05,
    'WZToLLLNu'   :  4.42965,
    'ZZToNuNuQQ'  :  4.04,
    'ZZToLLQQ'    :  3.22,
    'ZZToLLNuNu'  :  0.5644,
    'ZZToLLLL'    :  1.256,
    'WG'  :  489.0,
    'ZG'  :  117.864,
    'SingleTop_s_channel'  :  3.3623,
    'SingleTop_t_channel'  :  70.695,
    'SingleTop_tW'         :  35.6,
    'SingleTop_tbarW'      :  35.6,
    'TTJets_DiLept'              :  88.31,
    'TTJets_SingleLeptFromT'     :  182.7,
    'TTJets_SingleLeptFromTbar'  :  182.7,

    ###########################################################################
    #RECO stored on T3
    ###########################################################################
    #stopped particles stage2 signals
    #from https://github.com/quertenmont/cmssw/blob/Run2HSCP_v9/SUSYBSMAnalysis/HSCP/test/AnalysisCode/Analysis_Samples.txt
    'GluinoToUUbarChi0_1200_200' : 8.3600000000E-02,
    'GluinoToUUbarChi0_1200_600' : 8.3600000000E-02,
    'GluinoToUUbarChi0_1200_1000' : 8.3600000000E-02,
    'GluinoToGChi0_1200_200'  : 8.3600000000E-02,
    'GluinoToGChi0_1200_400'  : 8.3600000000E-02,
    'GluinoToGChi0_1200_600'  : 8.3600000000E-02,
    'GluinoToGChi0_1200_800'  : 8.3600000000E-02,
    'GluinoToGChi0_1200_1000'  : 8.3600000000E-02,

    #stopped particles cosmic events
    'cosmic_preselection' : 8.3600000000E-02,


}

InputCondorArguments = {}
