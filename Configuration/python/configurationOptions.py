#!/usr/bin/env python

config_file = "standardConfig_cfg.py"

###### 2015A = 961 pb-1
###### 2012B = 4.9 fb-1
###### 2012B = 4.9 fb-1
###### 2012B = 4.9 fb-1

intLumi = 2613 #2015D Silver Json

datasets = [

   #data
   'DoubleMu_2015',
   'DoubleMu_2015D',
   'SingleMu_2015',
   'SingleMu_2015D',
   'SingleMu_2016',
   'SingleMu_2016B',
   'SingleEle_2015',
   'SingleEle_2015D',
   'SingleEle_2016',
   'SingleEle_2016B',
   'SinglePhoton_2015',
   'SinglePhoton_2015D',
   'DoubleEG_2015',
   'DoubleEG_2015D',
   'DoubleEG_2016',
   'DoubleEG_2016B',
   'MuonEG_2015',
   'MuonEG_2015D',
   'MET_2015',
   'MET_2015D',
   'MET_2016',
   'MET_2016B',
   'NoBPTX_2015D',
   'NoBPTX_2015D_v3',
   'NoBPTX_2015D_v4',
   'StpPtls_controlSample_2015',
   'Commissioning2015_controlSample',
   'NoBPTX_2015B_controlSample',
   'NoBPTX_2015C_controlSample',
   'NoBPTX_2015Dv3_controlSample',
   'NoBPTX_2015Dv4_controlSample',
   'NoBPTX_2015C_16Dec2015',
   'NoBPTX_2015D_16Dec2015',
   'NoBPTX_2015B_PromptReco',
   'NoBPTX_2015C_PromptReco',
   'NoBPTX_2015Dv3_PromptReco',
   'NoBPTX_2015Dv4_PromptReco',
   'NoBPTX_2015E_PromptReco',

   'NoBPTX_2015C_16Dec2015_Ntuple',
   'NoBPTX_2015D_16Dec2015_Ntuple',
   
   'NoBPTX_2015B_PromptReco_Cosmic_Ntuple',
   'NoBPTX_2015C_PromptReco_Cosmic_Ntuple',
   'NoBPTX_2015Dv3_PromptReco_Cosmic_Ntuple',
   'NoBPTX_2015Dv4_PromptReco_Cosmic_Ntuple',
   'NoBPTX_2015E_PromptReco_Cosmic_Ntuple',
   
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
]

composite_dataset_definitions = {
    'DYJetsToLL' : [
        'DYJetsToLL_50',
        'DYJetsToLL_10to50',
    ],
    'ZJetsToNuNu' : [
        'ZJetsToNuNu_HT100to200',
        'ZJetsToNuNu_HT200to400',
        'ZJetsToNuNu_HT400to600',
        'ZJetsToNuNu_HT600toInf',
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
        'WZToLNu2QorQQ2L',
        'WWToLNuLNu',
        'WZToLNuNuNu',
        'WZToLLLNu',
        # Switch to inclusive WZ sample when it is ready.
        'ZZToNuNuQQ',
        'ZZToLLQQ',
        'ZZToLLNuNu',
        'ZZToLLLL',
        'WG',
        'ZG'
    ],
    'Diboson_Incl' : [  # Inclusive samples, where available
        'WWToLNuQQ',
        'WWToLNuLNu',
        'WZToLNuQQ',
        'WZToLLQQ',
        'WZToLNuNuNu',
        'WZToLLLNu',
        # Switch to inclusive WZ sample when it is ready.
        'ZZ',
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
        'SingleTop_t_channel_top',
        'SingleTop_t_channel_antitop',
        'SingleTop_tW',
        'SingleTop_tbarW',
    ],
    'TTJets_Lept' : [
        'TTJets_DiLept',
        'TTJets_SingleLeptFromT',
        'TTJets_SingleLeptFromTbar',
    ],
    'QCD' : [
        'QCD_5to10',
        'QCD_10to15',
        'QCD_15to30',
        'QCD_30to50',
        'QCD_50to80',
        'QCD_80to120',
        'QCD_120to170',
        'QCD_170to300',
        'QCD_300to470',
        'QCD_470to600',
        'QCD_600to800',
        'QCD_800to1000',
        'QCD_1000to1400',
        'QCD_1400to1800',
        'QCD_1800to2400',
        'QCD_2400to3200',
        'QCD_3200toInf',
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
        'DoubleMu_2015D',
    ],
    'SingleMu_2015' : [
        'SingleMu_2015B',
        'SingleMu_2015C',
        'SingleMu_2015D',
    ],
    'SingleMu_2016' : [
        'SingleMu_2016B',
    ],
    'SingleEle_2015' : [
        'SingleEle_2015B',
        'SingleEle_2015C',
        'SingleEle_2015D',
    ],
    'SingleEle_2016' : [
        'SingleEle_2016B',
    ],
    'SinglePhoton_2015' : [
        'SinglePhoton_2015B',
        'SinglePhoton_2015C',
        'SinglePhoton_2015D',
    ],
    'DoubleEG_2015' : [
        'DoubleEG_2015B',
        'DoubleEG_2015C',
        'DoubleEG_2015D',
    ],
    'DoubleEG_2016' : [
        'DoubleEG_2016B',
    ],
    'MuonEG_2015' : [
        'MuonEG_2015B',
        'MuonEG_2015C',
        'MuonEG_2015D',
    ],
    'NoBPTX_2015D' : [
        'NoBPTX_2015D_v3',
        'NoBPTX_2015D_v4',
    ],
    'NoBPTX_2015BCDE_PromptReco' : [
        'NoBPTX_2015B_PromptReco',
        'NoBPTX_2015C_PromptReco',
        'NoBPTX_2015Dv3_PromptReco',
        'NoBPTX_2015Dv4_PromptReco',
        'NoBPTX_2015E_PromptReco',
        ],
   'StpPtls_controlSample_2015' : [
        #'Commissioning2015_controlSample',
        'NoBPTX_2015B_controlSample',
        'NoBPTX_2015C_controlSample',
        'NoBPTX_2015Dv3_controlSample',
        'NoBPTX_2015Dv4_controlSample',
        #'NoBPTX_2015E_controlSample'
        ],
    'NoBPTX_16Dec2015' : [
        'NoBPTX_2015C_16Dec2015',
        'NoBPTX_2015D_16Dec2015',
        ],
    'NoBPTX_16Dec2015_Ntuple' : [
        'NoBPTX_2015C_16Dec2015_Ntuple',
        'NoBPTX_2015D_16Dec2015_Ntuple',
        ],  
    'NoBPTX_2015BCDE_PromptReco_Cosmic_Ntuple' : [
        'NoBPTX_2015B_PromptReco_Cosmic_Ntuple',
        'NoBPTX_2015C_PromptReco_Cosmic_Ntuple',
        'NoBPTX_2015Dv3_PromptReco_Cosmic_Ntuple',
        'NoBPTX_2015Dv4_PromptReco_Cosmic_Ntuple',
        'NoBPTX_2015E_PromptReco_Cosmic_Ntuple',
        ],
    'MET_2015' : [
        'MET_2015B',
        'MET_2015C',
        'MET_2015D',
    ],
    'MET_2016' : [
	'MET_2016B',
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

    #ZJetsToNuNu
    'DYJetsToNuNu'   :  999,
    'ZJetsToNuNu'   :  999,
    'ZJetsToNuNu_HT100to200'   :  999,
    'ZJetsToNuNu_HT200to400'   :  999,
    'ZJetsToNuNu_HT400to600'   :  999,
    'ZJetsToNuNu_HT600toInf'   :  999,

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
    'WZToLNuNuNu'  :  999,
    'WZToLLLNu'    :  999,
    'WZToLNu2QorQQ2L' : 999,
    #ZZ
    'ZZToNuNuQQ'  :  999,
    'ZZToLLQQ'    :  999,
    'ZZToLLNuNu'  :  999,
    'ZZToLLLL'    :  999,

    #VG
    'WG'  :  999,
    'ZG'  :  999,

    #SingleTop
    'SingleTop_s_channel'          :  999,
    'SingleTop_t_channel'          :  999,
    'SingleTop_t_channel_top'      :  999,
    'SingleTop_t_channel_antitop'  :  999,
    'SingleTop_tW'                 :  999,
    'SingleTop_tbarW'              :  999,

    #TTJets
    'TTJets_DiLept'              :  300,
    'TTJets_SingleLeptFromT'     :  300,
    'TTJets_SingleLeptFromTbar'  :  300,

    #MET data
    'MET_2015D' : 999,
    'MET_2016B' : 999,

    #AMSB chargino
    'AMSB_chargino_100GeV_10cm'    :  99,
    'AMSB_chargino_100GeV_100cm'   :  99,
    'AMSB_chargino_100GeV_1000cm'  :  99,

    'AMSB_chargino_200GeV_10cm'    :  99,
    'AMSB_chargino_200GeV_100cm'   :  99,
    'AMSB_chargino_200GeV_1000cm'  :  99,

    'AMSB_chargino_300GeV_10cm'    :  99,
    'AMSB_chargino_300GeV_100cm'   :  99,
    'AMSB_chargino_300GeV_1000cm'  :  99,

    'AMSB_chargino_400GeV_10cm'    :  99,
    'AMSB_chargino_400GeV_100cm'   :  99,
    'AMSB_chargino_400GeV_1000cm'  :  99,

    'AMSB_chargino_500GeV_10cm'    :  99,
    'AMSB_chargino_500GeV_100cm'   :  99,
    'AMSB_chargino_500GeV_1000cm'  :  99,

    'AMSB_chargino_600GeV_10cm'    :  99,
    'AMSB_chargino_600GeV_100cm'   :  99,
    'AMSB_chargino_600GeV_1000cm'  :  99,

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

    'QCD_5to10'       :  999,
    'QCD_10to15'      :  999,
    'QCD_15to30'      :  999,
    'QCD_30to50'      :  999,
    'QCD_50to80'      :  999,
    'QCD_80to120'     :  999,
    'QCD_120to170'    :  999,
    'QCD_170to300'    :  999,
    'QCD_300to470'    :  999,
    'QCD_470to600'    :  999,
    'QCD_600to800'    :  999,
    'QCD_800to1000'   :  999,
    'QCD_1000to1400'  :  999,
    'QCD_1400to1800'  :  999,
    'QCD_1800to2400'  :  999,
    'QCD_2400to3200'  :  999,
    'QCD_3200toInf'   :  999,

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

    'DoubleMu_2015B'   :  40,
    'DoubleMu_2015C'   : 187,
    'DoubleMu_2015D': 1000,

    'SingleMu_2015B'      :  57,
    'SingleMu_2015C'      : 220,
    'SingleMu_2015D'   : 1000,
    'SingleMu_2016B'   : 1000,

    'SingleEle_2015B'    :  77,
    'SingleEle_2015C'    : 137,
    'SingleEle_2015D' : 1000,
    'SingleEle_2016B' : 1000,

    'SinglePhoton_2015B'   :  55,
    'SinglePhoton_2015C'   : 130,
    'SinglePhoton_2015D'   : 1000,

    'DoubleEG_2015B'      :  100,
    'DoubleEG_2015C'      :  298,
    'DoubleEG_2015D'   :  1000,
    'DoubleEG_2016B'      : 1000,

    'MuonEG_2015B'   :  30,
    'MuonEG_2015C'   :  82,

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
    #signals for energy scan
    'GluinoToGChi0_600_309'  : 5,
    'GluinoToGChi0_600_354'  : 5,
    'GluinoToGChi0_600_424'  : 5,
    'GluinoToGChi0_600_483'  : 5,
    'GluinoToGChi0_600_495'  : 5,
    'GluinoToGChi0_600_507'  : 5,
    'GluinoToGChi0_600_519'  : 5,
    'GluinoToGChi0_600_547'  : 5,
    'GluinoToGChi0_1200_848'  : 5,
    'GluinoToGChi0_1200_903'  : 5,
    'GluinoToGChi0_1200_954'  : 5,
    'GluinoToGChi0_1200_997'  : 5,
    'GluinoToGChi0_1200_1044'  : 5,
    'GluinoToGChi0_1200_1089'  : 5,
    'GluinoToGChi0_1200_1100'  : 5,
    'GluinoToGChi0_1200_1111'  : 5,
    'GluinoToGChi0_1200_1122'  : 5,
    'GluinoToGChi0_1200_1148'  : 5,
    'GluinoToGChi0_1800_1047'  : 2,
    'GluinoToGChi0_1800_1122'  : 1,
    'GluinoToGChi0_1800_1200'  : 1,
    'GluinoToGChi0_1800_1272'  : 2,
    'GluinoToGChi0_1800_1341'  : 3,
    'GluinoToGChi0_1800_1407'  : 3,
    'GluinoToGChi0_1800_1469'  : 4,
    'GluinoToGChi0_1800_1529'  : 4,
    'StopToTChi0_400_75' : 4,
    'StopToTChi0_400_150' : 4,
    'StopToTChi0_400_199' : 4,
    'StopToTChi0_400_250' : 4,
    'StopToTChi0_400_285' : 4,
    'StopToTChi0_400_300' : 4,
    'StopToTChi0_600_100' : 5,
    'StopToTChi0_600_300' : 5,
    'StopToTChi0_600_400' : 5,
    'StopToTChi0_600_500' : 5,
    'StopToTChi0_1000_100' : 2,
    'StopToTChi0_1000_173' : 4,
    'StopToTChi0_1000_360' : 3,
    'StopToTChi0_1000_480' : 3,
    'StopToTChi0_1000_574' : 4,
    'StopToTChi0_1000_656' : 4,
    'StopToTChi0_1000_728' : 4,

    #stopped particles cosmic events
    #'cosmic_preselection' : 300,
    'cosmic_preselection' : 999,

    #stopped particles stage1 GEN-SIM
    'mchamp100' : 999,
    'mchamp200' : 999,
    'mchamp400' : 999,
    'mchamp600' : 999,
    'mchamp800' : 999,
    'mchamp1000' : 999,
    'mchamp1400' : 999,
    'mchamp1800' : 999,
    'mchamp2200' : 999,
    'mchamp2600' : 999,

    'gluino100'   : 999,
    'gluino200'   : 999,
    'gluino400'   : 999,
    'gluino600'   : 999,
    'gluino800'   : 999,
    'gluino1000'  : 999,
    'gluino1200'  : 999,
    'gluino1400'  : 999,
    'gluino1600'  : 999,
    'gluino1800'  : 999,
    'gluino2000'  : 999,
    'gluino2200'  : 999,
    'gluino2400'  : 999,
    'gluino2600'  : 999,

    #stopped particles stage2 GEN-SIM
    #'mchamp100_Stage2SeparateEventsParticle0'   : 999,
    #'mchamp200_Stage2SeparateEventsParticle0'   : 999,
    #'mchamp400_Stage2SeparateEventsParticle0'   : 999,
    'mchamp600_Stage2SeparateEventsParticle0'   : 999,
    #'mchamp800_Stage2SeparateEventsParticle0'   : 999,
    #'mchamp1000_Stage2SeparateEventsParticle0'  : 999,
    #'mchamp1400_Stage2SeparateEventsParticle0'  : 999,
    #'mchamp1800_Stage2SeparateEventsParticle0'  : 999,
    #'mchamp2200_Stage2SeparateEventsParticle0'  : 999,
    #'mchamp2600_Stage2SeparateEventsParticle0'  : 999,

    #stopped particles stage2 DIGI-HLT
    #'mchamp100_DigiHltSeparateEventsParticle0'   :    999,
    #'mchamp200_DigiHltSeparateEventsParticle0'   :    999,
    #'mchamp400_DigiHltSeparateEventsParticle0'   :    999,
    'mchamp600_DigiHltSeparateEventsParticle0'   :    999,
    #'mchamp800_DigiHltSeparateEventsParticle0'   :    999,
    #'mchamp1000_DigiHltSeparateEventsParticle0'  :    999,
    #'mchamp1400_DigiHltSeparateEventsParticle0'  :    999,
    #'mchamp1800_DigiHltSeparateEventsParticle0'  :    999,
    #'mchamp2200_DigiHltSeparateEventsParticle0'  :    999,
    #'mchamp2600_DigiHltSeparateEventsParticle0'  :    999,

    #stopped particles stage2 RECO
    #'mchamp100_RecoSeparateEventsParticle0'   :    999,
    #'mchamp200_RecoSeparateEventsParticle0'   :    999,
    #'mchamp400_RecoSeparateEventsParticle0'   :    999,
    'mchamp600_RecoSeparateEventsParticle0'   :    999,
    #'mchamp800_RecoSeparateEventsParticle0'   :    999,
    #'mchamp1000_RecoSeparateEventsParticle0'  :    999,
    #'mchamp1400_RecoSeparateEventsParticle0'  :    999,
    #'mchamp1800_RecoSeparateEventsParticle0'  :    999,
    #'mchamp2200_RecoSeparateEventsParticle0'  :    999,
    #'mchamp2600_RecoSeparateEventsParticle0'  :    999,

    #stopped particles stage2 Ntuples
    #'mchamp100_NtupleSeparateEventsParticle0'   :    999,
    #'mchamp200_NtupleSeparateEventsParticle0'   :    999,
    #'mchamp400_NtupleSeparateEventsParticle0'   :    999,
    'mchamp600_NtupleSeparateEventsParticle0'   :    999,
    #'mchamp800_NtupleSeparateEventsParticle0'   :    999,
    #'mchamp1000_NtupleSeparateEventsParticle0'  :    999,
    #'mchamp1400_NtupleSeparateEventsParticle0'  :    999,
    #'mchamp1800_NtupleSeparateEventsParticle0'  :    999,
    #'mchamp2200_NtupleSeparateEventsParticle0'  :    999,
    #'mchamp2600_NtupleSeparateEventsParticle0'  :    999,


    ###########################################################################
    #RECO not stored on T3
    ###########################################################################
    #NoBPTX data
    'NoBPTX_2015D_v3' : 999,
    'NoBPTX_2015D_v4' : 999,
    #'NoBPTX_2015D_v3' : 30,
    #'NoBPTX_2015D_v4' : 30,

    'NoBPTX_2015C_16Dec2015' : 999,
    'NoBPTX_2015D_16Dec2015' : 999,

    'NoBPTX_2015B_PromptReco' : 999,
    'NoBPTX_2015C_PromptReco' : 999,
    'NoBPTX_2015Dv3_PromptReco' : 999,
    'NoBPTX_2015Dv4_PromptReco' : 999,
    'NoBPTX_2015E_PromptReco' : 999,

    'NoBPTX_2015C_16Dec2015_Ntuple' : 999,
    'NoBPTX_2015D_16Dec2015_Ntuple' : 999,

    'NoBPTX_2015B_PromptReco_Cosmic_Ntuple' : 999,
    'NoBPTX_2015C_PromptReco_Cosmic_Ntuple' : 999,
    'NoBPTX_2015Dv3_PromptReco_Cosmic_Ntuple' : 999,
    'NoBPTX_2015Dv4_PromptReco_Cosmic_Ntuple' : 999,
    'NoBPTX_2015E_PromptReco_Cosmic_Ntuple' : 999,

    #control sample data
    'Commissioning2015_controlSample' : 999,
    'NoBPTX_2015B_controlSample' : 999,
    'NoBPTX_2015C_controlSample' : 999,
    'NoBPTX_2015Dv3_controlSample' : 256,
    'NoBPTX_2015Dv4_controlSample' : 662,
    'NoBPTX_2015E_controlSample' : 31,
    #rpc study
    'GluinoToGChi0_1200_1000_noiseFrate0p4' : 5,
    'GluinoToGChi0_1200_1000_noiseFrate0p6' : 5,
    'GluinoToGChi0_1200_1000_noiseFrate0p7' : 5,
    'GluinoToGChi0_1200_1000_noiseFrate0p8' : 5,
    'GluinoToGChi0_1200_1000_noiseFrate1p0' : 5,
    'GluinoToGChi0_1200_1000_nonoise' : 5,

    'MuonEG_2015D'   :  1000,

    ############################################################################
}

maxEvents = {
    ############################################################################
    #MiniAOD stored on T3.
    ############################################################################
    #DY
    'DYJetsToLL_50'  :  -1,

    #ZJetsToNuNu
    'DYJetsToNuNu'   :  -1,
    'ZJetsToNuNu'   :  -1,
    'ZJetsToNuNu_HT100to200'   :  -1,
    'ZJetsToNuNu_HT200to400'   :  -1,
    'ZJetsToNuNu_HT400to600'   :  -1,
    'ZJetsToNuNu_HT600toInf'   :  -1,

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
    'WZToLNuNuNu'  :  -1,
    'WZToLLLNu'    :  -1,
    'WZToLNu2QorQQ2L' : -1,
    #ZZ
    'ZZToNuNuQQ'  :  -1,
    'ZZToLLQQ'    :  -1,
    'ZZToLLNuNu'  :  -1,
    'ZZToLLLL'    :  -1,

    #VG
    'WG'  :  -1,
    'ZG'  :  -1,

    #SingleTop
    'SingleTop_s_channel'          :  -1,
    'SingleTop_t_channel'          :  -1,
    'SingleTop_t_channel_top'      :  -1,
    'SingleTop_t_channel_antitop'  :  -1,
    'SingleTop_tW'                 :  -1,
    'SingleTop_tbarW'              :  -1,

    #TTJets
    'TTJets_DiLept'              :  -1,
    'TTJets_SingleLeptFromT'     :  -1,
    'TTJets_SingleLeptFromTbar'  :  -1,

    #MET data
    'MET_2015D' : -1,
    'MET_2016B' : -1,

    #AMSB chargino
    'AMSB_chargino_100GeV_10cm'    :  -1,
    'AMSB_chargino_100GeV_100cm'   :  -1,
    'AMSB_chargino_100GeV_1000cm'  :  -1,

    'AMSB_chargino_200GeV_10cm'    :  -1,
    'AMSB_chargino_200GeV_100cm'   :  -1,
    'AMSB_chargino_200GeV_1000cm'  :  -1,

    'AMSB_chargino_300GeV_10cm'    :  -1,
    'AMSB_chargino_300GeV_100cm'   :  -1,
    'AMSB_chargino_300GeV_1000cm'  :  -1,

    'AMSB_chargino_400GeV_10cm'    :  -1,
    'AMSB_chargino_400GeV_100cm'   :  -1,
    'AMSB_chargino_400GeV_1000cm'  :  -1,

    'AMSB_chargino_500GeV_10cm'    :  -1,
    'AMSB_chargino_500GeV_100cm'   :  -1,
    'AMSB_chargino_500GeV_1000cm'  :  -1,

    'AMSB_chargino_600GeV_10cm'    :  -1,
    'AMSB_chargino_600GeV_100cm'   :  -1,
    'AMSB_chargino_600GeV_1000cm'  :  -1,

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

    'QCD_5to10'       :  -1,
    'QCD_10to15'      :  -1,
    'QCD_15to30'      :  -1,
    'QCD_30to50'      :  -1,
    'QCD_50to80'      :  -1,
    'QCD_80to120'     :  -1,
    'QCD_120to170'    :  -1,
    'QCD_170to300'    :  -1,
    'QCD_300to470'    :  -1,
    'QCD_470to600'    :  -1,
    'QCD_600to800'    :  -1,
    'QCD_800to1000'   :  -1,
    'QCD_1000to1400'  :  -1,
    'QCD_1400to1800'  :  -1,
    'QCD_1800to2400'  :  -1,
    'QCD_2400to3200'  :  -1,
    'QCD_3200toInf'   :  -1,

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

    'DoubleMu_2015B'       : -1,
    'DoubleMu_2015C'       : -1,
    'DoubleMu_2015D'    : -1,

    'SingleMu_2015B'       : -1,
    'SingleMu_2015C'       : -1,
    'SingleMu_2015D'       : -1,
    'SingleMu_2016B'       : -1,

    'SingleEle_2015B'       : -1,
    'SingleEle_2015C'       : -1,
    'SingleEle_2015D'       : -1,
    'SingleEle_2016B'       : -1,

    'SinglePhoton_2015B'       : -1,
    'SinglePhoton_2015C'       : -1,
    'SinglePhoton_2015D'       : -1,

    'DoubleEG_2015B'       : -1,
    'DoubleEG_2015C'       : -1,
    'DoubleEG_2015D'       : -1,
    'DoubleEG_2016B'       : -1,

    'MuonEG_2015B'       : -1,
    'MuonEG_2015C'       : -1,

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
    #signals for energy scan
    'GluinoToGChi0_600_309'  : -1,
    'GluinoToGChi0_600_354'  : -1,
    'GluinoToGChi0_600_424'  : -1,
    'GluinoToGChi0_600_483'  : -1,
    'GluinoToGChi0_600_495'  : -1,
    'GluinoToGChi0_600_507'  : -1,
    'GluinoToGChi0_600_519'  : -1,
    'GluinoToGChi0_600_547'  : -1,
    'GluinoToGChi0_1200_848'  : -1,
    'GluinoToGChi0_1200_903'  : -1,
    'GluinoToGChi0_1200_954'  : -1,
    'GluinoToGChi0_1200_997'  : -1,
    'GluinoToGChi0_1200_1044'  : -1,
    'GluinoToGChi0_1200_1089'  : -1,
    'GluinoToGChi0_1200_1100'  : -1,
    'GluinoToGChi0_1200_1111'  : -1,
    'GluinoToGChi0_1200_1122'  : -1,
    'GluinoToGChi0_1200_1148'  : -1,
    'GluinoToGChi0_1800_1047'  : -1,
    'GluinoToGChi0_1800_1122'  : -1,
    'GluinoToGChi0_1800_1200'  : -1,
    'GluinoToGChi0_1800_1272'  : -1,
    'GluinoToGChi0_1800_1341'  : -1,
    'GluinoToGChi0_1800_1407'  : -1,
    'GluinoToGChi0_1800_1469'  : -1,
    'GluinoToGChi0_1800_1529'  : -1,
    'StopToTChi0_400_75' : -1,
    'StopToTChi0_400_150' : -1,
    'StopToTChi0_400_199' : -1,
    'StopToTChi0_400_250' : -1,
    'StopToTChi0_400_285' : -1,
    'StopToTChi0_400_300' : -1,
    'StopToTChi0_600_100' : -1,
    'StopToTChi0_600_300' : -1,
    'StopToTChi0_600_400' : -1,
    'StopToTChi0_600_500' : -1,
    'StopToTChi0_1000_100' : -1,
    'StopToTChi0_1000_173' : -1,
    'StopToTChi0_1000_360' : -1,
    'StopToTChi0_1000_480' : -1,
    'StopToTChi0_1000_574' : -1,
    'StopToTChi0_1000_656' : -1,
    'StopToTChi0_1000_728' : -1,

    #stopped particles cosmic events
    'cosmic_preselection' : -1,

    #stopped particles stage1 GEN-SIM
    'mchamp100' : -1,
    'mchamp200' : -1,
    'mchamp400' : -1,
    'mchamp600' : -1,
    'mchamp800' : -1,
    'mchamp1000' : -1,
    'mchamp1400' : -1,
    'mchamp1800' : -1,
    'mchamp2200' : -1,
    'mchamp2600' : -1,

    'gluino100'   : -1,
    'gluino200'   : -1,
    'gluino400'   : -1,
    'gluino600'   : -1,
    'gluino800'   : -1,
    'gluino1000'  : -1,
    'gluino1200'  : -1,
    'gluino1400'  : -1,
    'gluino1600'  : -1,
    'gluino1800'  : -1,
    'gluino2000'  : -1,
    'gluino2200'  : -1,
    'gluino2400'  : -1,
    'gluino2600'  : -1,

    #stopped particles stage2 GEN-SIM
    #'mchamp100_Stage2SeparateEventsParticle0'   : -1,
    #'mchamp200_Stage2SeparateEventsParticle0'   : -1,
    #'mchamp400_Stage2SeparateEventsParticle0'   : -1,
    'mchamp600_Stage2SeparateEventsParticle0'   : -1,
    #'mchamp800_Stage2SeparateEventsParticle0'   : -1,
    #'mchamp1000_Stage2SeparateEventsParticle0'  : -1,
    #'mchamp1400_Stage2SeparateEventsParticle0'  : -1,
    #'mchamp1800_Stage2SeparateEventsParticle0'  : -1,
    #'mchamp2200_Stage2SeparateEventsParticle0'  : -1,
    #'mchamp2600_Stage2SeparateEventsParticle0'  : -1,

    #stopped particles stage2 DIGI-HLT
    #'mchamp100_DigiHltSeparateEventsParticle0'   :    -1,
    #'mchamp200_DigiHltSeparateEventsParticle0'   :    -1,
    #'mchamp400_DigiHltSeparateEventsParticle0'   :    -1,
    'mchamp600_DigiHltSeparateEventsParticle0'   :    -1,
    #'mchamp800_DigiHltSeparateEventsParticle0'   :    -1,
    #'mchamp1000_DigiHltSeparateEventsParticle0'  :    -1,
    #'mchamp1400_DigiHltSeparateEventsParticle0'  :    -1,
    #'mchamp1800_DigiHltSeparateEventsParticle0'  :    -1,
    #'mchamp2200_DigiHltSeparateEventsParticle0'  :    -1,
    #'mchamp2600_DigiHltSeparateEventsParticle0'  :    -1,

    #stopped particles stage2 RECO
    #'mchamp100_RecoSeparateEventsParticle0'   :    -1,
    #'mchamp200_RecoSeparateEventsParticle0'   :    -1,
    #'mchamp400_RecoSeparateEventsParticle0'   :    -1,
    'mchamp600_RecoSeparateEventsParticle0'   :    -1,
    #'mchamp800_RecoSeparateEventsParticle0'   :    -1,
    #'mchamp1000_RecoSeparateEventsParticle0'  :    -1,
    #'mchamp1400_RecoSeparateEventsParticle0'  :    -1,
    #'mchamp1800_RecoSeparateEventsParticle0'  :    -1,
    #'mchamp2200_RecoSeparateEventsParticle0'  :    -1,
    #'mchamp2600_RecoSeparateEventsParticle0'  :    -1,

    #stopped particles stage2 Ntuple
    #'mchamp100_NtupleSeparateEventsParticle0'   :    -1,
    #'mchamp200_NtupleSeparateEventsParticle0'   :    -1,
    #'mchamp400_NtupleSeparateEventsParticle0'   :    -1,
    'mchamp600_NtupleSeparateEventsParticle0'   :    -1,
    #'mchamp800_NtupleSeparateEventsParticle0'   :    -1,
    #'mchamp1000_NtupleSeparateEventsParticle0'  :    -1,
    #'mchamp1400_NtupleSeparateEventsParticle0'  :    -1,
    #'mchamp1800_NtupleSeparateEventsParticle0'  :    -1,
    #'mchamp2200_NtupleSeparateEventsParticle0'  :    -1,
    #'mchamp2600_NtupleSeparateEventsParticle0'  :    -1,


    ###########################################################################
    #RECO not stored on T3
    ###########################################################################
    #NoBPTX data
    'NoBPTX_2015D_v3' : -1,
    'NoBPTX_2015D_v4' : -1,

    'NoBPTX_2015C_16Dec2015' : -1,
    'NoBPTX_2015D_16Dec2015' : -1,

    'NoBPTX_2015B_PromptReco' : -1,
    'NoBPTX_2015C_PromptReco' : -1,
    'NoBPTX_2015Dv3_PromptReco' : -1,
    'NoBPTX_2015Dv4_PromptReco' : -1,
    'NoBPTX_2015E_PromptReco' : -1,

    'NoBPTX_2015C_16Dec2015_Ntuple' : -1,
    'NoBPTX_2015D_16Dec2015_Ntuple' : -1,

    'NoBPTX_2015B_PromptReco_Cosmic_Ntuple' : -1,
    'NoBPTX_2015C_PromptReco_Cosmic_Ntuple' : -1,
    'NoBPTX_2015Dv3_PromptReco_Cosmic_Ntuple' : -1,
    'NoBPTX_2015Dv4_PromptReco_Cosmic_Ntuple' : -1,
    'NoBPTX_2015E_PromptReco_Cosmic_Ntuple' : -1,

    #control sample data
    'Commissioning2015_controlSample' : -1,
    'NoBPTX_2015B_controlSample' : -1,
    'NoBPTX_2015C_controlSample' : -1,
    'NoBPTX_2015Dv3_controlSample' : -1,
    'NoBPTX_2015Dv4_controlSample' : -1,
    'NoBPTX_2015E_controlSample' : -1,
    #rpc study
    'GluinoToGChi0_1200_1000_noiseFrate0p4' : -1,
    'GluinoToGChi0_1200_1000_noiseFrate0p6' : -1,
    'GluinoToGChi0_1200_1000_noiseFrate0p7' : -1,
    'GluinoToGChi0_1200_1000_noiseFrate0p8' : -1,
    'GluinoToGChi0_1200_1000_noiseFrate1p0' : -1,
    'GluinoToGChi0_1200_1000_nonoise' : -1,

    'MuonEG_2015D'       : -1,

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

    #ZJetsToNuNu
    'DYJetsToNuNu'   :  "bgMC",
    'ZJetsToNuNu'   :  "bgMC",
    'ZJetsToNuNu_HT100to200'   :  "bgMC",
    'ZJetsToNuNu_HT200to400'   :  "bgMC",
    'ZJetsToNuNu_HT400to600'   :  "bgMC",
    'ZJetsToNuNu_HT600toInf'   :  "bgMC",

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
    'WZToLNuNuNu'  :  "bgMC",
    'WZToLLLNu'    :  "bgMC",
    'WZToLNu2QorQQ2L'    :  "bgMC",

    #ZZ
    'ZZToNuNuQQ'  :  "bgMC",
    'ZZToLLQQ'    :  "bgMC",
    'ZZToLLNuNu'  :  "bgMC",
    'ZZToLLLL'    :  "bgMC",

    #VG
    'WG'  :  "bgMC",
    'ZG'  :  "bgMC",

    #SingleTop
    'SingleTop'                    :  "bgMC",
    'SingleTop_s_channel'          :  "bgMC",
    'SingleTop_t_channel'          :  "bgMC",
    'SingleTop_t_channel_top'      :  "bgMC",
    'SingleTop_t_channel_antitop'  :  "bgMC",
    'SingleTop_tW'                 :  "bgMC",
    'SingleTop_tbarW'              :  "bgMC",

    #TTJets
    'TTJets_Lept'                :  "bgMC",
    'TTJets_DiLept'              :  "bgMC",
    'TTJets_SingleLeptFromT'     :  "bgMC",
    'TTJets_SingleLeptFromTbar'  :  "bgMC",

    #MET data
    'MET_2015D' : "data",
    'MET_2015'  : "data",
    'MET_2016B' : "data",
    'MET_2016' : "data",

    #AMSB chargino
    'AMSB_chargino_100GeV_10cm'    :  "signalMC",
    'AMSB_chargino_100GeV_100cm'   :  "signalMC",
    'AMSB_chargino_100GeV_1000cm'  :  "signalMC",

    'AMSB_chargino_200GeV_10cm'    :  "signalMC",
    'AMSB_chargino_200GeV_100cm'   :  "signalMC",
    'AMSB_chargino_200GeV_1000cm'  :  "signalMC",

    'AMSB_chargino_300GeV_10cm'    :  "signalMC",
    'AMSB_chargino_300GeV_100cm'   :  "signalMC",
    'AMSB_chargino_300GeV_1000cm'  :  "signalMC",

    'AMSB_chargino_400GeV_10cm'    :  "signalMC",
    'AMSB_chargino_400GeV_100cm'   :  "signalMC",
    'AMSB_chargino_400GeV_1000cm'  :  "signalMC",

    'AMSB_chargino_500GeV_10cm'    :  "signalMC",
    'AMSB_chargino_500GeV_100cm'   :  "signalMC",
    'AMSB_chargino_500GeV_1000cm'  :  "signalMC",

    'AMSB_chargino_600GeV_10cm'    :  "signalMC",
    'AMSB_chargino_600GeV_100cm'   :  "signalMC",
    'AMSB_chargino_600GeV_1000cm'  :  "signalMC",

    'AMSB_chargino_700GeV_10cm'    :  "signalMC",
    'AMSB_chargino_700GeV_100cm'   :  "signalMC",
    'AMSB_chargino_700GeV_1000cm'  :  "signalMC",
    ############################################################################

    ############################################################################
    #MiniAOD not stored on T3.
    ############################################################################
    'DYJetsToLL'             : "bgMC",
    'DYJetsToLL_10to50'      : "bgMC",
    'TTJets'                 : "bgMC",
    'Diboson'                : "bgMC",
    'Diboson_Incl'           : "bgMC",
    'WW'                     : "bgMC",
    'ZZ'                     : "bgMC",
    'WZ'                     : "bgMC",

    'DYJetsToLL_50ns'        : "bgMC",
    'DYJetsToLL_50_50ns'     : "bgMC",
    'DYJetsToLL_10to50_50ns' : "bgMC",
    'WJetsToLNu_50ns'        : "bgMC",
    'TTJets_50ns'            : "bgMC",
    'Diboson_50ns'           : "bgMC",
    'WW_50ns'                : "bgMC",
    'ZZ_50ns'                : "bgMC",
    'WZ_50ns'                : "bgMC",

    'QCD'             :  "bgMC",
    'QCD_5to10'       :  "bgMC",
    'QCD_10to15'      :  "bgMC",
    'QCD_15to30'      :  "bgMC",
    'QCD_30to50'      :  "bgMC",
    'QCD_50to80'      :  "bgMC",
    'QCD_80to120'     :  "bgMC",
    'QCD_120to170'    :  "bgMC",
    'QCD_170to300'    :  "bgMC",
    'QCD_300to470'    :  "bgMC",
    'QCD_470to600'    :  "bgMC",
    'QCD_600to800'    :  "bgMC",
    'QCD_800to1000'   :  "bgMC",
    'QCD_1000to1400'  :  "bgMC",
    'QCD_1400to1800'  :  "bgMC",
    'QCD_1800to2400'  :  "bgMC",
    'QCD_2400to3200'  :  "bgMC",
    'QCD_3200toInf'   :  "bgMC",

    #QCD MuEnriched
    'QCD_MuEnriched_20toInf'        : "bgMC",
    'QCD_MuEnriched'                : "bgMC",
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

    'DoubleMu_2015B'   : "data",
    'DoubleMu_2015C'   : "data",
    'DoubleMu_2015D'   : "data",
    'DoubleMu_2015'    : "data",

    'SingleMu_2015B'    : "data",
    'SingleMu_2015C'    : "data",
    'SingleMu_2015D'    : "data",
    'SingleMu_2016B'    : "data",

    'SingleMu_2015'     : "data",
    'SingleMu_2016'     : "data",

    'SingleEle_2015B'   : "data",
    'SingleEle_2015C'   : "data",
    'SingleEle_2015D'   : "data",
    'SingleEle_2016B'   : "data",

    'SingleEle_2015'    : "data",
    'SingleEle_2016'    : "data",

    'SinglePhoton_2015B'   : "data",
    'SinglePhoton_2015C'   : "data",
    'SinglePhoton_2015D'   : "data",
    'SinglePhoton_2015'    : "data",

    'DoubleEG_2015B'   : "data",
    'DoubleEG_2015C'   : "data",
    'DoubleEG_2015'    : "data",
    'DoubleEG_2015D'    : "data",
    'DoubleEG_2016'    : "data",
    'DoubleEG_2016B'   : "data",

    'MuonEG_2015B'   : "data",
    'MuonEG_2015C'   : "data",
    'MuonEG_2015D'   : "data",
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
    #signal for energy scan
    'GluinoToGChi0_600_309'  : "signalMC",
    'GluinoToGChi0_600_354'  : "signalMC",
    'GluinoToGChi0_600_424'  : "signalMC",
    'GluinoToGChi0_600_483'  : "signalMC",
    'GluinoToGChi0_600_495'  : "signalMC",
    'GluinoToGChi0_600_507'  : "signalMC",
    'GluinoToGChi0_600_519'  : "signalMC",
    'GluinoToGChi0_600_547'  : "signalMC",
    'GluinoToGChi0_1200_848'  : "signalMC",
    'GluinoToGChi0_1200_903'  : "signalMC",
    'GluinoToGChi0_1200_954'  : "signalMC",
    'GluinoToGChi0_1200_997'  : "signalMC",
    'GluinoToGChi0_1200_1044'  : "signalMC",
    'GluinoToGChi0_1200_1089'  : "signalMC",
    'GluinoToGChi0_1200_1100'  : "signalMC",
    'GluinoToGChi0_1200_1111'  : "signalMC",
    'GluinoToGChi0_1200_1122'  : "signalMC",
    'GluinoToGChi0_1200_1148'  : "signalMC",
    'GluinoToGChi0_1800_1047'  : "signalMC",
    'GluinoToGChi0_1800_1122'  : "signalMC",
    'GluinoToGChi0_1800_1200'  : "signalMC",
    'GluinoToGChi0_1800_1272'  : "signalMC",
    'GluinoToGChi0_1800_1341'  : "signalMC",
    'GluinoToGChi0_1800_1407'  : "signalMC",
    'GluinoToGChi0_1800_1469'  : "signalMC",
    'GluinoToGChi0_1800_1529'  : "signalMC",
    'StopToTChi0_400_75' : "signalMC",
    'StopToTChi0_400_150' : "signalMC",
    'StopToTChi0_400_199' : "signalMC",
    'StopToTChi0_400_250' : "signalMC",
    'StopToTChi0_400_285' : "signalMC",
    'StopToTChi0_400_300' : "signalMC",
    'StopToTChi0_600_100' : "signalMC",
    'StopToTChi0_600_300' : "signalMC",
    'StopToTChi0_600_400' : "signalMC",
    'StopToTChi0_600_500' : "signalMC",
    'StopToTChi0_1000_100' : "signalMC",
    'StopToTChi0_1000_173' : "signalMC",
    'StopToTChi0_1000_360' : "signalMC",
    'StopToTChi0_1000_480' : "signalMC",
    'StopToTChi0_1000_574' : "signalMC",
    'StopToTChi0_1000_656' : "signalMC",
    'StopToTChi0_1000_728' : "signalMC",

    #stopped particles cosmic events
    'cosmic_preselection' : "bgMC",

    #stopped particles stage1 GEN-SIM
    'mchamp100' : "signalMC",
    'mchamp200' : "signalMC",
    'mchamp400' : "signalMC",
    'mchamp600' : "signalMC",
    'mchamp800' : "signalMC",
    'mchamp1000' : "signalMC",
    'mchamp1400' : "signalMC",
    'mchamp1800' : "signalMC",
    'mchamp2200' : "signalMC",
    'mchamp2600' : "signalMC",

    'gluino100'   : "signalMC",
    'gluino200'   : "signalMC",
    'gluino400'   : "signalMC",
    'gluino600'   : "signalMC",
    'gluino800'   : "signalMC",
    'gluino1000'  : "signalMC",
    'gluino1200'  : "signalMC",
    'gluino1400'  : "signalMC",
    'gluino1600'  : "signalMC",
    'gluino1800'  : "signalMC",
    'gluino2000'  : "signalMC",
    'gluino2200'  : "signalMC",
    'gluino2400'  : "signalMC",
    'gluino2600'  : "signalMC",

    #stopped particles stage2 GEN-SIM
    #'mchamp100_Stage2SeparateEventsParticle0'   : "signalMC",
    #'mchamp200_Stage2SeparateEventsParticle0'   : "signalMC",
    #'mchamp400_Stage2SeparateEventsParticle0'   : "signalMC",
    'mchamp600_Stage2SeparateEventsParticle0'   : "signalMC",
    #'mchamp800_Stage2SeparateEventsParticle0'   : "signalMC",
    #'mchamp1000_Stage2SeparateEventsParticle0'  : "signalMC",
    #'mchamp1400_Stage2SeparateEventsParticle0'  : "signalMC",
    #'mchamp1800_Stage2SeparateEventsParticle0'  : "signalMC",
    #'mchamp2200_Stage2SeparateEventsParticle0'  : "signalMC",
    #'mchamp2600_Stage2SeparateEventsParticle0'  : "signalMC",

    #stopped particles stage2 DIGI-HLT
    #'mchamp100_DigiHltSeparateEventsParticle0'   :    "signalMC",
    #'mchamp200_DigiHltSeparateEventsParticle0'   :    "signalMC",
    #'mchamp400_DigiHltSeparateEventsParticle0'   :    "signalMC",
    'mchamp600_DigiHltSeparateEventsParticle0'   :    "signalMC",
    #'mchamp800_DigiHltSeparateEventsParticle0'   :    "signalMC",
    #'mchamp1000_DigiHltSeparateEventsParticle0'  :    "signalMC",
    #'mchamp1400_DigiHltSeparateEventsParticle0'  :    "signalMC",
    #'mchamp1800_DigiHltSeparateEventsParticle0'  :    "signalMC",
    #'mchamp2200_DigiHltSeparateEventsParticle0'  :    "signalMC",
    #'mchamp2600_DigiHltSeparateEventsParticle0'  :    "signalMC",

    #stopped particles stage2 RECO
    #'mchamp100_RecoSeparateEventsParticle0'   :    "signalMC",
    #'mchamp200_RecoSeparateEventsParticle0'   :    "signalMC",
    #'mchamp400_RecoSeparateEventsParticle0'   :    "signalMC",
    'mchamp600_RecoSeparateEventsParticle0'   :    "signalMC",
    #'mchamp800_RecoSeparateEventsParticle0'   :    "signalMC",
    #'mchamp1000_RecoSeparateEventsParticle0'  :    "signalMC",
    #'mchamp1400_RecoSeparateEventsParticle0'  :    "signalMC",
    #'mchamp1800_RecoSeparateEventsParticle0'  :    "signalMC",
    #'mchamp2200_RecoSeparateEventsParticle0'  :    "signalMC",
    #'mchamp2600_RecoSeparateEventsParticle0'  :    "signalMC",

    #stopped particles stage2 Ntuples
    #'mchamp100_NtupleSeparateEventsParticle0'   :    "signalMC",
    #'mchamp200_NtupleSeparateEventsParticle0'   :    "signalMC",
    #'mchamp400_NtupleSeparateEventsParticle0'   :    "signalMC",
    'mchamp600_NtupleSeparateEventsParticle0'   :    "signalMC",
    #'mchamp800_NtupleSeparateEventsParticle0'   :    "signalMC",
    #'mchamp1000_NtupleSeparateEventsParticle0'  :    "signalMC",
    #'mchamp1400_NtupleSeparateEventsParticle0'  :    "signalMC",
    #'mchamp1800_NtupleSeparateEventsParticle0'  :    "signalMC",
    #'mchamp2200_NtupleSeparateEventsParticle0'  :    "signalMC",
    #'mchamp2600_NtupleSeparateEventsParticle0'  :    "signalMC",

    ###########################################################################
    #RECO not stored on T3
    ###########################################################################
    #NoBPTX data
    'NoBPTX_2015D_v3' : 'data',
    'NoBPTX_2015D_v4' : 'data',

    'NoBPTX_2015C_16Dec2015' : 'data',
    'NoBPTX_2015D_16Dec2015' : 'data',

    #NoBPTX data
    'NoBPTX_2015D' : 'data',
    'NoBPTX_16Dec2015' : 'data',

    'NoBPTX_2015BCDE_PromptReco' : 'data',
    'NoBPTX_2015B_PromptReco' : 'data',
    'NoBPTX_2015C_PromptReco' : 'data',
    'NoBPTX_2015Dv3_PromptReco' : 'data',
    'NoBPTX_2015Dv4_PromptReco' : 'data',
    'NoBPTX_2015E_PromptReco' : 'data',

    'NoBPTX_2015C_16Dec2015_Ntuple' : 'data',
    'NoBPTX_2015D_16Dec2015_Ntuple' : 'data',
    'NoBPTX_16Dec2015_Ntuple' : 'data',

    'NoBPTX_2015B_PromptReco_Cosmic_Ntuple' : 'data',
    'NoBPTX_2015C_PromptReco_Cosmic_Ntuple' : 'data',
    'NoBPTX_2015Dv3_PromptReco_Cosmic_Ntuple' : 'data',
    'NoBPTX_2015Dv4_PromptReco_Cosmic_Ntuple' : 'data',
    'NoBPTX_2015E_PromptReco_Cosmic_Ntuple' : 'data',
    'NoBPTX_2015BCDE_PromptReco_Cosmic_Ntuple' : 'data',

    ############################################################################

    #control sample data
    'Commissioning2015_controlSample' : 'data',
    'NoBPTX_2015B_controlSample' : 'data',
    'NoBPTX_2015C_controlSample' : 'data',
    'NoBPTX_2015Dv3_controlSample' : 'data',
    'NoBPTX_2015Dv4_controlSample' : 'data',
    'NoBPTX_2015E_controlSample' : 'data',
    'StpPtls_controlSample_2015' : 'data',


    #rpc study
    'GluinoToGChi0_1200_1000_noiseFrate0p4' : "signalMC",
    'GluinoToGChi0_1200_1000_noiseFrate0p6' : "signalMC",
    'GluinoToGChi0_1200_1000_noiseFrate0p7' : "signalMC",
    'GluinoToGChi0_1200_1000_noiseFrate0p8' : "signalMC",
    'GluinoToGChi0_1200_1000_noiseFrate1p0' : "signalMC",
    'GluinoToGChi0_1200_1000_nonoise' : "signalMC",
}

colors = {
    ############################################################################
    #MiniAOD stored on T3.
    ############################################################################
    #DY
    'DYJetsToLL_50'  :  410,

    #ZJetsToNuNu
    'DYJetsToNuNu'   :  831,
    'ZJetsToNuNu'   :  831,
    'ZJetsToNuNu_HT100to200'   :  831,
    'ZJetsToNuNu_HT200to400'   :  831,
    'ZJetsToNuNu_HT400to600'   :  831,
    'ZJetsToNuNu_HT600toInf'   :  831,

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
    'WZToLNuNuNu'  :  393,
    'WZToLLLNu'    :  393,
    'WZToLNu2QorQQ2L'    :  393,

    #ZZ
    'ZZToNuNuQQ'  :  397,
    'ZZToLLQQ'    :  397,
    'ZZToLLNuNu'  :  397,
    'ZZToLLLL'    :  397,

    #VG
    'WG'  :  399,
    'ZG'  :  398,

    #SingleTop
    'SingleTop'                    :  607,
    'SingleTop_s_channel'          :  905,
    'SingleTop_t_channel'          :  907,
    'SingleTop_t_channel_top'      :  907,
    'SingleTop_t_channel_antitop'  :  908,
    'SingleTop_tW'                 :  909,
    'SingleTop_tbarW'              :  910,

    #TTJets
    'TTJets_Lept'                :  872,
    'TTJets_DiLept'              :  873,
    'TTJets_SingleLeptFromT'     :  872,
    'TTJets_SingleLeptFromTbar'  :  872,

    #MET data
    'MET_2015D' : 1,
    'MET_2015' : 1,
    'MET_2016B' : 1,
    'MET_2016' : 1,

    #AMSB chargino
    'AMSB_chargino_100GeV_10cm'    :  628,
    'AMSB_chargino_100GeV_100cm'   :  596,
    'AMSB_chargino_100GeV_1000cm'  :  923,

    'AMSB_chargino_200GeV_10cm'    :  628,
    'AMSB_chargino_200GeV_100cm'   :  596,
    'AMSB_chargino_200GeV_1000cm'  :  923,

    'AMSB_chargino_300GeV_10cm'    :  628,
    'AMSB_chargino_300GeV_100cm'   :  596,
    'AMSB_chargino_300GeV_1000cm'  :  923,

    'AMSB_chargino_400GeV_10cm'    :  628,
    'AMSB_chargino_400GeV_100cm'   :  596,
    'AMSB_chargino_400GeV_1000cm'  :  923,

    'AMSB_chargino_500GeV_10cm'    :  628,
    'AMSB_chargino_500GeV_100cm'   :  596,
    'AMSB_chargino_500GeV_1000cm'  :  923,

    'AMSB_chargino_600GeV_10cm'    :  628,
    'AMSB_chargino_600GeV_100cm'   :  596,
    'AMSB_chargino_600GeV_1000cm'  :  923,

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
    'Diboson_Incl'        : 393,
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

    'QCD'             :  623,
    'QCD_5to10'       :  623,
    'QCD_10to15'      :  623,
    'QCD_15to30'      :  623,
    'QCD_30to50'      :  623,
    'QCD_50to80'      :  623,
    'QCD_80to120'     :  623,
    'QCD_120to170'    :  623,
    'QCD_170to300'    :  623,
    'QCD_300to470'    :  623,
    'QCD_470to600'    :  623,
    'QCD_600to800'    :  623,
    'QCD_800to1000'   :  623,
    'QCD_1000to1400'  :  623,
    'QCD_1400to1800'  :  623,
    'QCD_1800to2400'  :  623,
    'QCD_2400to3200'  :  623,
    'QCD_3200toInf'   :  623,

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

    'DoubleMu_2015B'   : 1,
    'DoubleMu_2015C'   : 1,
    'DoubleMu_2015D'   : 1,
    'DoubleMu_2015'   : 1,

    'SingleMu_2015B'   : 1,
    'SingleMu_2015C'   : 1,
    'SingleMu_2015D'   : 1,
    'SingleMu_2015'   : 1,
    'SingleMu_2016B'   : 1,
    'SingleMu_2016'  : 1,

    'SingleEle_2015B'   : 1,
    'SingleEle_2015C'   : 1,
    'SingleEle_2015D'   : 1,
    'SingleEle_2015'   : 1,
    'SingleEle_2016B'   : 1,
    'SingleEle_2016'    : 1,

    'SinglePhoton_2015B'   : 1,
    'SinglePhoton_2015C'   : 1,
    'SinglePhoton_2015D'   : 1,
    'SinglePhoton_2015'   : 1,

    'DoubleEG_2015B'   : 1,
    'DoubleEG_2015C'   : 1,
    'DoubleEG_2015'   : 1,
    'DoubleEG_2015D'   : 1,
    'DoubleEG_2016'    : 1,
    'DoubleEG_2016B'   : 1,

    'MuonEG_2015B'   : 1,
    'MuonEG_2015C'   : 1,
    'MuonEG_2015D'   : 1,
    'MuonEG_2015'    : 1,

    ###########################################################################
    #RECO stored on T3
    ###########################################################################
    #stopped particles stage2 signals
    'GluinoToUUbarChi0_1200_200' : 4,
    'GluinoToUUbarChi0_1200_600' : 22,
    'GluinoToUUbarChi0_1200_1000' : 2,
    'GluinoToGChi0_1200_200'  : 1,
    'GluinoToGChi0_1200_400'  : 3,
    'GluinoToGChi0_1200_600'  : 6,
    'GluinoToGChi0_1200_800'  : 7,
    'GluinoToGChi0_1200_1000'  : 4,
    #signals for energy scan
    'GluinoToGChi0_600_309'  : 1,
    'GluinoToGChi0_600_354'  : 1,
    'GluinoToGChi0_600_424'  : 1,
    'GluinoToGChi0_600_483'  : 1,
    'GluinoToGChi0_600_495'  : 1,
    'GluinoToGChi0_600_507'  : 1,
    'GluinoToGChi0_600_519'  : 1,
    'GluinoToGChi0_600_547'  : 1,
    'GluinoToGChi0_1200_848'  : 1,
    'GluinoToGChi0_1200_903'  : 1,
    'GluinoToGChi0_1200_954'  : 1,
    'GluinoToGChi0_1200_997'  : 1,
    'GluinoToGChi0_1200_1044'  : 2,
    'GluinoToGChi0_1200_1089'  : 1,
    'GluinoToGChi0_1200_1100'  : 1,
    'GluinoToGChi0_1200_1111'  : 1,
    'GluinoToGChi0_1200_1122'  : 1,
    'GluinoToGChi0_1200_1148'  : 1,
    'GluinoToGChi0_1800_1047'  : 1,
    'GluinoToGChi0_1800_1122'  : 1,
    'GluinoToGChi0_1800_1200'  : 1,
    'GluinoToGChi0_1800_1272'  : 1,
    'GluinoToGChi0_1800_1341'  : 1,
    'GluinoToGChi0_1800_1407'  : 1,
    'GluinoToGChi0_1800_1469'  : 1,
    'GluinoToGChi0_1800_1529'  : 1,
    'StopToTChi0_400_75' : 1,
    'StopToTChi0_400_150' : 1,
    'StopToTChi0_400_199' : 1,
    'StopToTChi0_400_250' : 1,
    'StopToTChi0_400_285' : 1,
    'StopToTChi0_400_300' : 1,
    'StopToTChi0_600_100' : 1,
    'StopToTChi0_600_300' : 1,
    'StopToTChi0_600_400' : 4,
    'StopToTChi0_600_500' : 1,
    'StopToTChi0_1000_100' : 1,
    'StopToTChi0_1000_173' : 1,
    'StopToTChi0_1000_360' : 1,
    'StopToTChi0_1000_480' : 1,
    'StopToTChi0_1000_574' : 1,
    'StopToTChi0_1000_656' : 1,
    'StopToTChi0_1000_728' : 1,


    #stopped particles cosmic events
    'cosmic_preselection' : 2,

    #stopped particles stage1 GEN-SIM
    'mchamp100' : 1,
    'mchamp200' : 1,
    'mchamp400' : 1,
    'mchamp600' : 1,
    'mchamp800' : 1,
    'mchamp1000' : 1,
    'mchamp1400' : 1,
    'mchamp1800' : 1,
    'mchamp2200' : 1,
    'mchamp2600' : 1,

    'gluino100'   : 1,
    'gluino200'   : 1,
    'gluino400'   : 1,
    'gluino600'   : 1,
    'gluino800'   : 1,
    'gluino1000'  : 1,
    'gluino1200'  : 1,
    'gluino1400'  : 1,
    'gluino1600'  : 1,
    'gluino1800'  : 1,
    'gluino2000'  : 1,
    'gluino2200'  : 1,
    'gluino2400'  : 1,
    'gluino2600'  : 1,

    #stopped particles stage2 GEN-SIM
    #'mchamp100_Stage2SeparateEventsParticle0'   : 1,
    #'mchamp200_Stage2SeparateEventsParticle0'   : 1,
    #'mchamp400_Stage2SeparateEventsParticle0'   : 1,
    'mchamp600_Stage2SeparateEventsParticle0'   : 1,
    #'mchamp800_Stage2SeparateEventsParticle0'   : 1,
    #'mchamp1000_Stage2SeparateEventsParticle0'  : 1,
    #'mchamp1400_Stage2SeparateEventsParticle0'  : 1,
    #'mchamp1800_Stage2SeparateEventsParticle0'  : 1,
    #'mchamp2200_Stage2SeparateEventsParticle0'  : 1,
    #'mchamp2600_Stage2SeparateEventsParticle0'  : 1,

    #stopped particles stage2 DIGI-HLT
    #'mchamp100_DigiHltSeparateEventsParticle0'   :    1,
    #'mchamp200_DigiHltSeparateEventsParticle0'   :    1,
    #'mchamp400_DigiHltSeparateEventsParticle0'   :    1,
    'mchamp600_DigiHltSeparateEventsParticle0'   :    1,
    #'mchamp800_DigiHltSeparateEventsParticle0'   :    1,
    #'mchamp1000_DigiHltSeparateEventsParticle0'  :    1,
    #'mchamp1400_DigiHltSeparateEventsParticle0'  :    1,
    #'mchamp1800_DigiHltSeparateEventsParticle0'  :    1,
    #'mchamp2200_DigiHltSeparateEventsParticle0'  :    1,
    #'mchamp2600_DigiHltSeparateEventsParticle0'  :    1,

    #stopped particles stage2 RECO
    #'mchamp100_RecoSeparateEventsParticle0'   :    1,
    #'mchamp200_RecoSeparateEventsParticle0'   :    1,
    #'mchamp400_RecoSeparateEventsParticle0'   :    1,
    'mchamp600_RecoSeparateEventsParticle0'   :    1,
    #'mchamp800_RecoSeparateEventsParticle0'   :    1,
    #'mchamp1000_RecoSeparateEventsParticle0'  :    1,
    #'mchamp1400_RecoSeparateEventsParticle0'  :    1,
    #'mchamp1800_RecoSeparateEventsParticle0'  :    1,
    #'mchamp2200_RecoSeparateEventsParticle0'  :    1,
    #'mchamp2600_RecoSeparateEventsParticle0'  :    1,

    #stopped particles stage2 Ntuples
    #'mchamp100_NtupleSeparateEventsParticle0'   :    1,
    #'mchamp200_NtupleSeparateEventsParticle0'   :    1,
    #'mchamp400_NtupleSeparateEventsParticle0'   :    1,
    'mchamp600_NtupleSeparateEventsParticle0'   :    1,
    #'mchamp800_NtupleSeparateEventsParticle0'   :    1,
    #'mchamp1000_NtupleSeparateEventsParticle0'  :    1,
    #'mchamp1400_NtupleSeparateEventsParticle0'  :    1,
    #'mchamp1800_NtupleSeparateEventsParticle0'  :    1,
    #'mchamp2200_NtupleSeparateEventsParticle0'  :    1,
    #'mchamp2600_NtupleSeparateEventsParticle0'  :    1,

    ############################################################################

    #NoBPTX data
    'NoBPTX_2015D_v3' : 1,
    'NoBPTX_2015D_v4' : 1,
    'NoBPTX_2015D' : 1,

    'NoBPTX_2015C_16Dec2015' : 1,
    'NoBPTX_2015D_16Dec2015' : 1,
    'NoBPTX_16Dec2015' : 1,

    'NoBPTX_2015BCDE_PromptReco' : 1,
    'NoBPTX_2015B_PromptReco' : 1,
    'NoBPTX_2015C_PromptReco' : 1,
    'NoBPTX_2015Dv3_PromptReco' : 1,
    'NoBPTX_2015Dv4_PromptReco' : 1,
    'NoBPTX_2015E_PromptReco' : 1,

    'NoBPTX_2015C_16Dec2015_Ntuple' : 1,
    'NoBPTX_2015D_16Dec2015_Ntuple' : 1,
    'NoBPTX_16Dec2015_Ntuple' : 1,

    'NoBPTX_2015B_PromptReco_Cosmic_Ntuple' : 1,
    'NoBPTX_2015C_PromptReco_Cosmic_Ntuple' : 1,
    'NoBPTX_2015Dv3_PromptReco_Cosmic_Ntuple' : 1,
    'NoBPTX_2015Dv4_PromptReco_Cosmic_Ntuple' : 1,
    'NoBPTX_2015E_PromptReco_Cosmic_Ntuple' : 1,
    'NoBPTX_2015BCDE_PromptReco_Cosmic_Ntuple' : 1,
    ############################################################################

    #control sample data
    'Commissioning2015_controlSample' : 1,
    'NoBPTX_2015B_controlSample' : 1,
    'NoBPTX_2015C_controlSample' : 1,
    'NoBPTX_2015Dv3_controlSample' : 1,
    'NoBPTX_2015Dv4_controlSample' : 1,
    'NoBPTX_2015E_controlSample' : 1,
    'StpPtls_controlSample_2015' : 1,

    #rpc study
    'GluinoToGChi0_1200_1000_noiseFrate0p4' : 7,
    'GluinoToGChi0_1200_1000_noiseFrate0p6' : 6,
    'GluinoToGChi0_1200_1000_noiseFrate0p7' : 4,
    'GluinoToGChi0_1200_1000_noiseFrate0p8' : 5,
    'GluinoToGChi0_1200_1000_noiseFrate1p0' : 8,
    'GluinoToGChi0_1200_1000_nonoise' : 9,
}

labels = {
    ############################################################################
    #MiniAOD stored on T3.
    ############################################################################
    #DY
    'DYJetsToLL_50'  :  "Z#rightarrowl^{+}l^{-} M(50)",
    'DYJetsToLL'     :  "Z#rightarrowl^{+}l^{-} M(10)",
    'DYJetsToLL_10to50'     :  "Z#rightarrowl^{+}l^{-} M(10-50)",

    #ZJetsToNuNu
    'DYJetsToNuNu'   :  "Z#rightarrow#nu#bar{#nu}",
    'ZJetsToNuNu'   :  "Z#rightarrow#nu#bar{#nu}",
    'ZJetsToNuNu_HT100to200'   :  "Z#rightarrow#nu#bar{#nu} (100<H_{T}<200 GeV)",
    'ZJetsToNuNu_HT200to400'   :  "Z#rightarrow#nu#bar{#nu} (200<H_{T}<400 GeV)",
    'ZJetsToNuNu_HT400to600'   :  "Z#rightarrow#nu#bar{#nu} (400<H_{T}<600 GeV)",
    'ZJetsToNuNu_HT600toInf'   :  "Z#rightarrow#nu#bar{#nu} (H_{T}>600 GeV)",

    #WJets
    'WJetsToLNu'            :  "W#rightarrowl#nu",
    'WJetsToLNu_HT'         :  "W#rightarrowl#nu (H_{T}>100 GeV)",
    'WJetsToLNu_HT100to200' :  "W#rightarrowl#nu (100<H_{T}<200 GeV)",
    'WJetsToLNu_HT200to400' :  "W#rightarrowl#nu (200<H_{T}<400 GeV)",
    'WJetsToLNu_HT400to600' :  "W#rightarrowl#nu (400<H_{T}<600 GeV)",
    'WJetsToLNu_HT600toInf' :  "W#rightarrowl#nu (H_{T}>600 GeV)",

    #WW
    'WWToLNuQQ'   :  "WW#rightarrowl#nuqq",
    'WWToLNuLNu'  :  "WW#rightarrowl#nul#nu",

    #WZ
    'WZToLNuNuNu'  :  "WZ#rightarrowl#nu#nu#nu",
    'WZToLLLNu'    :  "WZ#rightarrowl#null",
    'WZToLNu2QorQQ2L'    :  "WZ#rightarrowqqll/l#nuqq",

    #ZZ
    'ZZToNuNuQQ'  :  "ZZ#rightarrow#nu#nuqq",
    'ZZToLLQQ'    :  "ZZ#rightarrowllqq",
    'ZZToLLNuNu'  :  "ZZ#rightarrowll#nu#nu",
    'ZZToLLLL'    :  "ZZ#rightarrowllll",

    #VG
    'WG'  :  "W#gamma#rightarrowl#nu#gamma",
    'ZG'  :  "Z#gamma#rightarrowll#gamma",

    #SingleTop
    'SingleTop'                    :  "Single top",
    'SingleTop_s_channel'          :  "Single top (s-channel)",
    'SingleTop_t_channel'          :  "Single top (t-channel)",
    'SingleTop_t_channel_top'      :  "Single top(top) (t-channel)",
    'SingleTop_t_channel_antitop'  :  "Single top(antitop) (t-channel)",
    'SingleTop_tW'                 :  "Single top (tW)",
    'SingleTop_tbarW'              :  "Single top (#bar{t}W)",

    #TTJets
    'TTJets_Lept'                :  "t#bar{t}",
    'TTJets_DiLept'              :  "t#bar{t} (fully leptonic)",
    'TTJets_SingleLeptFromT'     :  "t#bar{t} (single lepton from t)",
    'TTJets_SingleLeptFromTbar'  :  "t#bar{t} (single lepton from #bar{t})",

    #MET data
    'MET_2015D' : "MET 2015D data",
    'MET_2015'  : "MET 2015 data",
    'MET_2016B' : "MET 2016B data",
    'MET_2016'  : "MET 2016 data",

    #AMSB chargino
    'AMSB_chargino_100GeV_10cm'    :  "AMSB #tilde{#chi}^{#pm} (100 GeV, 10 cm)",
    'AMSB_chargino_100GeV_100cm'   :  "AMSB #tilde{#chi}^{#pm} (100 GeV, 100 cm)",
    'AMSB_chargino_100GeV_1000cm'  :  "AMSB #tilde{#chi}^{#pm} (100 GeV, 1000 cm)",

    'AMSB_chargino_200GeV_10cm'    :  "AMSB #tilde{#chi}^{#pm} (200 GeV, 10 cm)",
    'AMSB_chargino_200GeV_100cm'   :  "AMSB #tilde{#chi}^{#pm} (200 GeV, 100 cm)",
    'AMSB_chargino_200GeV_1000cm'  :  "AMSB #tilde{#chi}^{#pm} (200 GeV, 1000 cm)",

    'AMSB_chargino_300GeV_10cm'    :  "AMSB #tilde{#chi}^{#pm} (300 GeV, 10 cm)",
    'AMSB_chargino_300GeV_100cm'   :  "AMSB #tilde{#chi}^{#pm} (300 GeV, 100 cm)",
    'AMSB_chargino_300GeV_1000cm'  :  "AMSB #tilde{#chi}^{#pm} (300 GeV, 1000 cm)",

    'AMSB_chargino_400GeV_10cm'    :  "AMSB #tilde{#chi}^{#pm} (400 GeV, 10 cm)",
    'AMSB_chargino_400GeV_100cm'   :  "AMSB #tilde{#chi}^{#pm} (400 GeV, 100 cm)",
    'AMSB_chargino_400GeV_1000cm'  :  "AMSB #tilde{#chi}^{#pm} (400 GeV, 1000 cm)",

    'AMSB_chargino_500GeV_10cm'    :  "AMSB #tilde{#chi}^{#pm} (500 GeV, 10 cm)",
    'AMSB_chargino_500GeV_100cm'   :  "AMSB #tilde{#chi}^{#pm} (500 GeV, 100 cm)",
    'AMSB_chargino_500GeV_1000cm'  :  "AMSB #tilde{#chi}^{#pm} (500 GeV, 1000 cm)",

    'AMSB_chargino_600GeV_10cm'    :  "AMSB #tilde{#chi}^{#pm} (600 GeV, 10 cm)",
    'AMSB_chargino_600GeV_100cm'   :  "AMSB #tilde{#chi}^{#pm} (600 GeV, 100 cm)",
    'AMSB_chargino_600GeV_1000cm'  :  "AMSB #tilde{#chi}^{#pm} (600 GeV, 1000 cm)",

    'AMSB_chargino_700GeV_10cm'    :  "AMSB #tilde{#chi}^{#pm} (700 GeV, 10 cm)",
    'AMSB_chargino_700GeV_100cm'   :  "AMSB #tilde{#chi}^{#pm} (700 GeV, 100 cm)",
    'AMSB_chargino_700GeV_1000cm'  :  "AMSB #tilde{#chi}^{#pm} (700 GeV, 1000 cm)",
    ############################################################################

    ############################################################################
    #MiniAOD not stored on T3.
    ############################################################################
    'DYJetsToLL_10to50'             : "Z#rightarrowll M(10to50)",
    'TTJets'                        : "t#bar{t}",
    'Diboson'                   : "diboson",
    'Diboson_Incl'              : "diboson",
    'WW'                        : "WW",
    'WZ'                        : "WZ",
    'ZZ'                        : "ZZ",

    'DYJetsToLL_50_50ns'                 : "Z#rightarrowll M(50) 50ns",
    'DYJetsToLL_10to50_50ns'             : "Z#rightarrowll M(10to50) 50ns",
    'WJetsToLNu_50ns'                    : "W#rightarrowl#nu 50ns",
    'TTJets_50ns'                        : "t#bar{t} 50ns",
    'Diboson_50ns'                   : "diboson 50ns",
    'WW_50ns'                        : "WW 50ns",
    'WZ_50ns'                        : "WZ 50ns",
    'ZZ_50ns'                        : "ZZ 50ns",

    'QCD'             :  "QCD",
    'QCD_5to10'       :  "QCD 5<#hat{p}_{T}<10 GeV",
    'QCD_10to15'      :  "QCD 10<#hat{p}_{T}<15 GeV",
    'QCD_15to30'      :  "QCD 15<#hat{p}_{T}<30 GeV",
    'QCD_30to50'      :  "QCD 30<#hat{p}_{T}<50 GeV",
    'QCD_50to80'      :  "QCD 50<#hat{p}_{T}<80 GeV",
    'QCD_80to120'     :  "QCD 80<#hat{p}_{T}<120 GeV",
    'QCD_120to170'    :  "QCD 120<#hat{p}_{T}<170 GeV",
    'QCD_170to300'    :  "QCD 170<#hat{p}_{T}<300 GeV",
    'QCD_300to470'    :  "QCD 300<#hat{p}_{T}<470 GeV",
    'QCD_470to600'    :  "QCD 470<#hat{p}_{T}<600 GeV",
    'QCD_600to800'    :  "QCD 600<#hat{p}_{T}<800 GeV",
    'QCD_800to1000'   :  "QCD 800<#hat{p}_{T}<1000 GeV",
    'QCD_1000to1400'  :  "QCD 1000<#hat{p}_{T}<1400 GeV",
    'QCD_1400to1800'  :  "QCD 1400<#hat{p}_{T}<1800 GeV",
    'QCD_1800to2400'  :  "QCD 1800<#hat{p}_{T}<2400 GeV",
    'QCD_2400to3200'  :  "QCD 2400<#hat{p}_{T}<3200 GeV",
    'QCD_3200toInf'   :  "QCD #hat{p}_{T}>3200 GeV",

    #QCD MuEnriched
    'QCD_MuEnriched_20toInf'        : "QCD MuEnriched Pt 20-Inf",
    'QCD_MuEnriched'                : "QCD MuEnriched",
    'QCD_MuEnriched_15to20'         : "QCD MuEnriched Pt 15-20",
    'QCD_MuEnriched_20to30'         : "QCD MuEnriched Pt 20-30",
    'QCD_MuEnriched_30to50'         : "QCD MuEnriched Pt 30-50",
    'QCD_MuEnriched_50to80'         : "QCD MuEnriched Pt 50-80",
    'QCD_MuEnriched_80to120'        : "QCD MuEnriched Pt 80-120",
    'QCD_MuEnriched_120to170'       : "QCD MuEnriched Pt 120-170",
    'QCD_MuEnriched_170to300'       : "QCD MuEnriched Pt 170-300",
    'QCD_MuEnriched_300to470'       : "QCD MuEnriched Pt 300-470",
    'QCD_MuEnriched_470to600'       : "QCD MuEnriched Pt 470-600",
    'QCD_MuEnriched_600to800'       : "QCD MuEnriched Pt 600-800",
    'QCD_MuEnriched_800to1000'      : "QCD MuEnriched Pt 800-1000",
    'QCD_MuEnriched_1000toInf'      : "QCD MuEnriched Pt 1000-Inf",

    #QCD EMEnriched
    'QCD_EMEnriched'                : "QCD EMEnriched",
    'QCD_EMEnriched_15to20'         : "QCD EMEnriched Pt 15-20",
    'QCD_EMEnriched_20to30'         : "QCD EMEnriched Pt 20-30",
    'QCD_EMEnriched_30to50'         : "QCD EMEnriched Pt 30-50",
    'QCD_EMEnriched_50to80'         : "QCD EMEnriched Pt 50-80",
    'QCD_EMEnriched_80to120'        : "QCD EMEnriched Pt 80-120",
    'QCD_EMEnriched_120to170'       : "QCD EMEnriched Pt 120-170",
    'QCD_EMEnriched_170to300'       : "QCD EMEnriched Pt 170-300",
    'QCD_EMEnriched_300toInf'       : "QCD EMEnriched Pt 300-Inf",

    #QCD bcToE
    'QCD_bcToE'                     : "QCD bcToE",
    'QCD_bcToE_15to20'              : "QCD bcToE Pt 15-20",
    'QCD_bcToE_20to30'              : "QCD bcToE Pt 20-30",
    'QCD_bcToE_30to80'              : "QCD bcToE Pt 30-80",
    'QCD_bcToE_80to170'             : "QCD bcToE Pt 80-170",
    'QCD_bcToE_170to250'            : "QCD bcToE Pt 170-250",
    'QCD_bcToE_250toInf'            : "QCD bcToE Pt 250-Inf",

    'QCDFromData'     : "data-driven QCD",

    'DoubleMu_2015'      : "Double Muon 2015 data",
    'DoubleMu_2015B'     : "DoubleMu 2015B data",
    'DoubleMu_2015C'     : "DoubleMu 2015C data",
    'DoubleMu_2015D'     : "DoubleMu 2015D data",

    'SingleMu_2015'      : "Single Muon 2015 data",
    'SingleMu_2015B'     : "SingleMu 2015B data",
    'SingleMu_2015C'     : "SingleMu 2015C data",
    'SingleMu_2015D'     : "SingleMu 2015D data",
    'SingleMu_2016'      : "Single Muon 2016 data",
    'SingleMu_2016B'     : "SingleMu 2016B data",

    'SingleEle_2015'      : "Single Electron 2015 data",
    'SingleEle_2015B'     : "SingleEle 2015B data",
    'SingleEle_2015C'     : "SingleEle 2015C data",
    'SingleEle_2015D'     : "SingleEle 2015D data",
    'SingleEle_2016'      : "Single Electron 2016 data",
    'SingleEle_2016B'     : "SingleEle 2016B data",

    'SinglePhoton_2015'      : "SinglePhoton 2015 data",
    'SinglePhoton_2015B'     : "SinglePhoton 2015B data",
    'SinglePhoton_2015C'     : "SinglePhoton 2015C data",
    'SinglePhoton_2015D'     : "SinglePhoton 2015D data",

    'DoubleEG_2015'      : "DoubleEG 2015 data",
    'DoubleEG_2015D'     : "DoubleEG 2015D data",
    'DoubleEG_2015B'     : "DoubleEG 2015B data",
    'DoubleEG_2015C'     : "DoubleEG 2015C data",
    'DoubleEG_2016'      : "DoubleEG 2016 data",
    'DoubleEG_2016B'     : "DoubleEG 2016B data",

    'MuonEG_2015'      : "MuonEG 2015 data",
    'MuonEG_2015B'     : "MuonEG 2015B data",
    'MuonEG_2015C'     : "MuonEG 2015C data",
    'MuonEG_2015D'     : "MuonEG 2015D data",

    ###########################################################################
    #RECO stored on T3
    ###########################################################################
    #stopped particles stage2 signals
    'GluinoToUUbarChi0_1200_200' : "#tilde{g} #rightarrow u#topbar{u}#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 200 GeV, Frate1p0)",
    'GluinoToUUbarChi0_1200_600' : "#tilde{g} #rightarrow u#topbar{u}#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 600 GeV)",
    'GluinoToUUbarChi0_1200_1000' : "#tilde{g} #rightarrow u#topbar{u}#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 1000 GeV)",
    'GluinoToGChi0_1200_200'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 200 GeV)Frate=1.0",
    'GluinoToGChi0_1200_400'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 400 GeV)",
    'GluinoToGChi0_1200_600'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 600 GeV)",
    'GluinoToGChi0_1200_800'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 800 GeV)",
    'GluinoToGChi0_1200_1000'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 1000 GeV)",
    #signal for energy scan
    'GluinoToGChi0_600_309'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 600 GeV, M_{#tilde{#chi}^{0}} = 309 GeV)",
    'GluinoToGChi0_600_354'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 600 GeV, M_{#tilde{#chi}^{0}} = 354 GeV)",
    'GluinoToGChi0_600_424'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 600 GeV, M_{#tilde{#chi}^{0}} = 424 GeV)",
    'GluinoToGChi0_600_483'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 600 GeV, M_{#tilde{#chi}^{0}} = 483 GeV)",
    'GluinoToGChi0_600_495'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 600 GeV, M_{#tilde{#chi}^{0}} = 495 GeV)",
    'GluinoToGChi0_600_507'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 600 GeV, M_{#tilde{#chi}^{0}} = 507 GeV)",
    'GluinoToGChi0_600_519'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 600 GeV, M_{#tilde{#chi}^{0}} = 519 GeV)",
    'GluinoToGChi0_600_547'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 600 GeV, M_{#tilde{#chi}^{0}} = 547 GeV)",
    'GluinoToGChi0_1200_848'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 848 GeV)",
    'GluinoToGChi0_1200_903'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 903 GeV)",
    'GluinoToGChi0_1200_954'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 954 GeV)",
    'GluinoToGChi0_1200_997'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 997 GeV)",
    'GluinoToGChi0_1200_1044'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 1044 GeV)",
    'GluinoToGChi0_1200_1089'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 1089 GeV)",
    'GluinoToGChi0_1200_1100'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 1100 GeV)",
    'GluinoToGChi0_1200_1111'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 1111 GeV)",
    'GluinoToGChi0_1200_1122'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 1122 GeV)",
    'GluinoToGChi0_1200_1148'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 1148 GeV)",
    'GluinoToGChi0_1800_1047'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1800 GeV, M_{#tilde{#chi}^{0}} = 1047 GeV)",
    'GluinoToGChi0_1800_1122'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1800 GeV, M_{#tilde{#chi}^{0}} = 1122 GeV)",
    'GluinoToGChi0_1800_1200'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1800 GeV, M_{#tilde{#chi}^{0}} = 1200 GeV)",
    'GluinoToGChi0_1800_1272'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1800 GeV, M_{#tilde{#chi}^{0}} = 1272 GeV)",
    'GluinoToGChi0_1800_1341'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1800 GeV, M_{#tilde{#chi}^{0}} = 1341 GeV)",
    'GluinoToGChi0_1800_1407'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1800 GeV, M_{#tilde{#chi}^{0}} = 1407 GeV)",
    'GluinoToGChi0_1800_1469'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1800 GeV, M_{#tilde{#chi}^{0}} = 1469 GeV)",
    'GluinoToGChi0_1800_1529'  : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1800 GeV, M_{#tilde{#chi}^{0}} = 1529 GeV)",
    'StopToTChi0_400_75' : "#tilde{t} #rightarrow t#tilde{#chi}^{0} (M_{#tilde{t}} = 400 GeV, M_{#tilde{#chi}^{0}} = 75GeV)",
    'StopToTChi0_400_150' : "#tilde{t} #rightarrow t#tilde{#chi}^{0} (M_{#tilde{t}} = 400 GeV, M_{#tilde{#chi}^{0}} = 150GeV)",
    'StopToTChi0_400_199' : "#tilde{t} #rightarrow t#tilde{#chi}^{0} (M_{#tilde{t}} = 400 GeV, M_{#tilde{#chi}^{0}} = 199GeV)",
    'StopToTChi0_400_250' : "#tilde{t} #rightarrow t#tilde{#chi}^{0} (M_{#tilde{t}} = 400 GeV, M_{#tilde{#chi}^{0}} = 250GeV)",
    'StopToTChi0_400_285' : "#tilde{t} #rightarrow t#tilde{#chi}^{0} (M_{#tilde{t}} = 400 GeV, M_{#tilde{#chi}^{0}} = 285GeV)",
    'StopToTChi0_400_300' : "#tilde{t} #rightarrow t#tilde{#chi}^{0} (M_{#tilde{t}} = 400 GeV, M_{#tilde{#chi}^{0}} = 300GeV)",
    'StopToTChi0_600_100' : "#tilde{t} #rightarrow t#tilde{#chi}^{0} (M_{#tilde{t}} = 600 GeV, M_{#tilde{#chi}^{0}} = 100GeV)",
    'StopToTChi0_600_300' : "#tilde{t} #rightarrow t#tilde{#chi}^{0} (M_{#tilde{t}} = 600 GeV, M_{#tilde{#chi}^{0}} = 300GeV)",
    'StopToTChi0_600_400' : "#tilde{t} #rightarrow t#tilde{#chi}^{0} (M_{#tilde{t}} = 600 GeV, M_{#tilde{#chi}^{0}} = 400GeV)",
    'StopToTChi0_600_500' : "#tilde{t} #rightarrow t#tilde{#chi}^{0} (M_{#tilde{t}} = 600 GeV, M_{#tilde{#chi}^{0}} = 500GeV)",
    'StopToTChi0_1000_100' : "#tilde{t} #rightarrow t#tilde{#chi}^{0} (M_{#tilde{t}} = 1000 GeV, M_{#tilde{#chi}^{0}} = 100GeV)",
    'StopToTChi0_1000_173' : "#tilde{t} #rightarrow t#tilde{#chi}^{0} (M_{#tilde{t}} = 1000 GeV, M_{#tilde{#chi}^{0}} = 173GeV)",
    'StopToTChi0_1000_360' : "#tilde{t} #rightarrow t#tilde{#chi}^{0} (M_{#tilde{t}} = 1000 GeV, M_{#tilde{#chi}^{0}} = 360GeV)",
    'StopToTChi0_1000_480' : "#tilde{t} #rightarrow t#tilde{#chi}^{0} (M_{#tilde{t}} = 1000 GeV, M_{#tilde{#chi}^{0}} = 480GeV)",
    'StopToTChi0_1000_574' : "#tilde{t} #rightarrow t#tilde{#chi}^{0} (M_{#tilde{t}} = 1000 GeV, M_{#tilde{#chi}^{0}} = 574GeV)",
    'StopToTChi0_1000_656' : "#tilde{t} #rightarrow t#tilde{#chi}^{0} (M_{#tilde{t}} = 1000 GeV, M_{#tilde{#chi}^{0}} = 656GeV)",
    'StopToTChi0_1000_728' : "#tilde{t} #rightarrow t#tilde{#chi}^{0} (M_{#tilde{t}} = 1000 GeV, M_{#tilde{#chi}^{0}} = 728GeV)",

    #stopped particles cosmic MC events
    'cosmic_preselection' : "Cosmic MC preselection",

    #stopped particles stage1 GEN-SIM
    'mchamp100' : "Mchamp (Q = 2e, M = 100 GeV)",
    'mchamp200' : "Mchamp (Q = 2e, M = 200 GeV)",
    'mchamp400' : "Mchamp (Q = 2e, M = 400 GeV)",
    'mchamp600' : "Mchamp (Q = 2e, M = 600 GeV)",
    'mchamp800' : "Mchamp (Q = 2e, M = 800 GeV)",
    'mchamp1000' : "Mchamp (Q = 2e, M = 1000 GeV)",
    'mchamp1400' : "Mchamp (Q = 2e, M = 1400 GeV)",
    'mchamp1800' : "Mchamp (Q = 2e, M = 1800 GeV)",
    'mchamp2200' : "Mchamp (Q = 2e, M = 2200 GeV)",
    'mchamp2600' : "Mchamp (Q = 2e, M = 2600 GeV)",

    'gluino100'   : "Gluino (M=100 GeV)",
    'gluino200'   : "Gluino (M=200 GeV)",
    'gluino400'   : "Gluino (M=400 GeV)",
    'gluino600'   : "Gluino (M=600 GeV)",
    'gluino800'   : "Gluino (M=800 GeV)",
    'gluino1000'  : "Gluino (M=1000 GeV)",
    'gluino1200'  : "Gluino (M=1200 GeV)",
    'gluino1400'  : "Gluino (M=1400 GeV)",
    'gluino1600'  : "Gluino (M=1600 GeV)",
    'gluino1800'  : "Gluino (M=1800 GeV)",
    'gluino2000'  : "Gluino (M=2000 GeV)",
    'gluino2200'  : "Gluino (M=2200 GeV)",
    'gluino2400'  : "Gluino (M=2400 GeV)",
    'gluino2600'  : "Gluino (M=2600 GeV)",

    #stopped particles stage2 GEN-SIM
    #'mchamp100_Stage2SeparateEventsParticle0'   : "Mchamp (Q = 2e, M = 100 GeV, Separate Events, Particle 0)",
    #'mchamp200_Stage2SeparateEventsParticle0'   : "Mchamp (Q = 2e, M = 200 GeV, Separate Events, Particle 0)",
    #'mchamp400_Stage2SeparateEventsParticle0'   : "Mchamp (Q = 2e, M = 400 GeV, Separate Events, Particle 0)",
    'mchamp600_Stage2SeparateEventsParticle0'   :  "Mchamp (Q = 2e, M = 600 GeV, Separate Events, Particle 0)",
    #'mchamp800_Stage2SeparateEventsParticle0'   : "Mchamp (Q = 2e, M = 800 GeV, Separate Events, Particle 0)",
    #'mchamp1000_Stage2SeparateEventsParticle0'  : "Mchamp (Q = 2e, M = 1000 GeV, Separate Events, Particle 0)",
    #'mchamp1400_Stage2SeparateEventsParticle0'  : "Mchamp (Q = 2e, M = 1400 GeV, Separate Events, Particle 0)",
    #'mchamp1800_Stage2SeparateEventsParticle0'  : "Mchamp (Q = 2e, M = 1800 GeV, Separate Events, Particle 0)",
    #'mchamp2200_Stage2SeparateEventsParticle0'  : "Mchamp (Q = 2e, M = 2200 GeV, Separate Events, Particle 0)",
    #'mchamp2600_Stage2SeparateEventsParticle0'  : "Mchamp (Q = 2e, M = 2600 GeV, Separate Events, Particle 0)",

    #stopped particles stage2 DIGI-HLT
    #'mchamp100_DigiHltSeparateEventsParticle0'   :  "Mchamp (Q = 2e, M = 100 GeV, Separate Events, Particle 0)",
    #'mchamp200_DigiHltSeparateEventsParticle0'   :  "Mchamp (Q = 2e, M = 200 GeV, Separate Events, Particle 0)",
    #'mchamp400_DigiHltSeparateEventsParticle0'   :  "Mchamp (Q = 2e, M = 400 GeV, Separate Events, Particle 0)",
    'mchamp600_DigiHltSeparateEventsParticle0'   :   "Mchamp (Q = 2e, M = 600 GeV, Separate Events, Particle 0)",
    #'mchamp800_DigiHltSeparateEventsParticle0'   :  "Mchamp (Q = 2e, M = 800 GeV, Separate Events, Particle 0)",
    #'mchamp1000_DigiHltSeparateEventsParticle0'  :  "Mchamp (Q = 2e, M = 1000 GeV, Separate Events, Particle 0)",
    #'mchamp1400_DigiHltSeparateEventsParticle0'  :  "Mchamp (Q = 2e, M = 1400 GeV, Separate Events, Particle 0)",
    #'mchamp1800_DigiHltSeparateEventsParticle0'  :  "Mchamp (Q = 2e, M = 1800 GeV, Separate Events, Particle 0)",
    #'mchamp2200_DigiHltSeparateEventsParticle0'  :  "Mchamp (Q = 2e, M = 2200 GeV, Separate Events, Particle 0)",
    #'mchamp2600_DigiHltSeparateEventsParticle0'  :  "Mchamp (Q = 2e, M = 2600 GeV, Separate Events, Particle 0)",

    #stopped particles stage2 RECO
    #'mchamp100_RecoSeparateEventsParticle0'   :   "Mchamp (Q = 2e, M = 100 GeV, Separate Events, Particle 0)",
    #'mchamp200_RecoSeparateEventsParticle0'   :   "Mchamp (Q = 2e, M = 200 GeV, Separate Events, Particle 0)",
    #'mchamp400_RecoSeparateEventsParticle0'   :   "Mchamp (Q = 2e, M = 400 GeV, Separate Events, Particle 0)",
    'mchamp600_RecoSeparateEventsParticle0'   :    "Mchamp (Q = 2e, M = 600 GeV, Separate Events, Particle 0)",
    #'mchamp800_RecoSeparateEventsParticle0'   :   "Mchamp (Q = 2e, M = 800 GeV, Separate Events, Particle 0)",
    #'mchamp1000_RecoSeparateEventsParticle0'  :   "Mchamp (Q = 2e, M = 1000 GeV, Separate Events, Particle 0)",
    #'mchamp1400_RecoSeparateEventsParticle0'  :   "Mchamp (Q = 2e, M = 1400 GeV, Separate Events, Particle 0)",
    #'mchamp1800_RecoSeparateEventsParticle0'  :   "Mchamp (Q = 2e, M = 1800 GeV, Separate Events, Particle 0)",
    #'mchamp2200_RecoSeparateEventsParticle0'  :   "Mchamp (Q = 2e, M = 2200 GeV, Separate Events, Particle 0)",
    #'mchamp2600_RecoSeparateEventsParticle0'  :   "Mchamp (Q = 2e, M = 2600 GeV, Separate Events, Particle 0)",

    #stopped particles stage2 Ntuples
    #'mchamp100_NtupleSeparateEventsParticle0'   :   "Mchamp (Q = 2e, M = 100 GeV, Separate Events, Particle 0)",
    #'mchamp200_NtupleSeparateEventsParticle0'   :   "Mchamp (Q = 2e, M = 200 GeV, Separate Events, Particle 0)",
    #'mchamp400_NtupleSeparateEventsParticle0'   :   "Mchamp (Q = 2e, M = 400 GeV, Separate Events, Particle 0)",
    'mchamp600_NtupleSeparateEventsParticle0'   :    "Mchamp (Q = 2e, M = 600 GeV, Separate Events, Particle 0)",
    #'mchamp800_NtupleSeparateEventsParticle0'   :   "Mchamp (Q = 2e, M = 800 GeV, Separate Events, Particle 0)",
    #'mchamp1000_NtupleSeparateEventsParticle0'  :   "Mchamp (Q = 2e, M = 1000 GeV, Separate Events, Particle 0)",
    #'mchamp1400_NtupleSeparateEventsParticle0'  :   "Mchamp (Q = 2e, M = 1400 GeV, Separate Events, Particle 0)",
    #'mchamp1800_NtupleSeparateEventsParticle0'  :   "Mchamp (Q = 2e, M = 1800 GeV, Separate Events, Particle 0)",
    #'mchamp2200_NtupleSeparateEventsParticle0'  :   "Mchamp (Q = 2e, M = 2200 GeV, Separate Events, Particle 0)",
    #'mchamp2600_NtupleSeparateEventsParticle0'  :   "Mchamp (Q = 2e, M = 2600 GeV, Separate Events, Particle 0)",

    ############################################################################

    #NoBPTX Data
    'NoBPTX_2015D_v3'     : "NoBPTX 2015D_v3 data",
    'NoBPTX_2015D_v4'     : "NoBPTX 2015D_v4 data",
    'NoBPTX_2015D'     : "NoBPTX 2015D data",

    'NoBPTX_2015C_16Dec2015'     : "NoBPTX 2015C Rerecoed data",
    'NoBPTX_2015D_16Dec2015'     : "NoBPTX 2015D Rerecoed data",
    'NoBPTX_16Dec2015'     : "NoBPTX 2015C,D Rerecoed data",

    'NoBPTX_2015BCDE_PromptReco' : "NoBPTX 2015B-E Prompt reco data",
    'NoBPTX_2015B_PromptReco' : "NoBPTX 2015B Prompt reco data",
    'NoBPTX_2015C_PromptReco' : "NoBPTX 2015C Prompt reco data",
    'NoBPTX_2015Dv3_PromptReco' : "NoBPTX 2015D_v3 Prompt reco data",
    'NoBPTX_2015Dv4_PromptReco' : "NoBPTX 2015D_v4 Prompt reco data",
    'NoBPTX_2015E_PromptReco' : "NoBPTX 2015E Prompt reco data",

    'NoBPTX_2015C_16Dec2015_Ntuple' : "NoBPTX 2015C Rerecoed data",
    'NoBPTX_2015D_16Dec2015_Ntuple' : "NoBPTX 2015D Rerecoed data",
    'NoBPTX_16Dec2015_Ntuple' : "NoBPTX 2015C,D Rerecoed data",

    'NoBPTX_2015B_PromptReco_Cosmic_Ntuple' : "NoBPTX 2015B cosmic data",
    'NoBPTX_2015C_PromptReco_Cosmic_Ntuple' : "NoBPTX 2015C cosmic data",
    'NoBPTX_2015Dv3_PromptReco_Cosmic_Ntuple' : "NoBPTX 2015D_v3 cosmic data",
    'NoBPTX_2015Dv4_PromptReco_Cosmic_Ntuple' : "NoBPTX 2015D_v4 cosmic data",
    'NoBPTX_2015E_PromptReco_Cosmic_Ntuple' : "NoBPTX 2015E cosmic data",
    'NoBPTX_2015BCDE_PromptReco_Cosmic_Ntuple' : "NoBPTX 2015B-E cosmic data",

    ############################################################################

    #control sample data
    'Commissioning2015_controlSample' : "Commissioning 2015 control sample",
    'NoBPTX_2015B_controlSample' : "NoBPTX 2015B control sample",
    'NoBPTX_2015C_controlSample' : "NoBPTX 2015C control sample",
    'NoBPTX_2015Dv3_controlSample': "NoBPTX 2015Dv3 control sample",
    'NoBPTX_2015Dv4_controlSample': "NoBPTX 2015Dv4 control sample",
    'NoBPTX_2015E_controlSample' : "NoBPTX 2015E control sample",
    'StpPtls_controlSample_2015' : "Data control sample",

    #rpc study
    'GluinoToGChi0_1200_1000_noiseFrate0p4' : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 1000 GeV, Frate0p4)",
    'GluinoToGChi0_1200_1000_noiseFrate0p6' : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 1000 GeV, Frate0p6)",
    'GluinoToGChi0_1200_1000_noiseFrate0p7' : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 1000 GeV, Frate0p7)",
    'GluinoToGChi0_1200_1000_noiseFrate0p8' : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 1000 GeV, Frate0p8)",
    'GluinoToGChi0_1200_1000_noiseFrate1p0' : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 1000 GeV, Frate1p0)",
    'GluinoToGChi0_1200_1000_nonoise' : "#tilde{g} #rightarrow g#tilde{#chi}^{0} (M_{#tilde{g}} = 1200 GeV, M_{#tilde{#chi}^{0}} = 1000 GeV, noNoise)",


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
    'QCD_EMEnriched_50to80'    :    19800000.0*0.146,
    'QCD_EMEnriched_80to120'   :     2800000.0*0.125,
    'QCD_EMEnriched_120to170'  :      477000.0*0.132,
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

    'WWToLNuQQ'   :  49.997,
    'WWToLNuLNu'  :  12.178,
    'WZToLNuNuNu' :  3.05,
    'WZToLLLNu'   :  4.42965,
    'WZToLNu2QorQQ2L'  :  9.82423165827,
    'ZZToNuNuQQ'  :  4.04,
    'ZZToLLQQ'    :  3.22,
    'ZZToLLNuNu'  :  0.5644,
    'ZZToLLLL'    :  1.256,
    'WG'  :  489.0,
    'ZG'  :  117.864,
    'SingleTop_s_channel'          :  3.3623,
    'SingleTop_t_channel'          :  70.695,
    'SingleTop_t_channel_top'      :  44.3151,
    'SingleTop_t_channel_antitop'  :  26.3734,
    'SingleTop_tW'                 :  19.418,
    'SingleTop_tbarW'              :  19.418,
    'TTJets_DiLept'                :  87.31,
    'TTJets_SingleLeptFromT'       :  182.7,
    'TTJets_SingleLeptFromTbar'    :  182.7,

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
    #signal for energy scan
    'GluinoToGChi0_600_309'  : 8.3600000000E-02,
    'GluinoToGChi0_600_354'  : 8.3600000000E-02,
    'GluinoToGChi0_600_424'  : 8.3600000000E-02,
    'GluinoToGChi0_600_483'  : 8.3600000000E-02,
    'GluinoToGChi0_600_495'  : 8.3600000000E-02,
    'GluinoToGChi0_600_507'  : 8.3600000000E-02,
    'GluinoToGChi0_600_519'  : 8.3600000000E-02,
    'GluinoToGChi0_600_547'  : 8.3600000000E-02,
    'GluinoToGChi0_1200_848'  : 8.3600000000E-02,
    'GluinoToGChi0_1200_903'  : 8.3600000000E-02,
    'GluinoToGChi0_1200_954'  : 8.3600000000E-02,
    'GluinoToGChi0_1200_997'  : 8.3600000000E-02,
    'GluinoToGChi0_1200_1044'  : 8.3600000000E-02,
    'GluinoToGChi0_1200_1089'  : 8.3600000000E-02,
    'GluinoToGChi0_1200_1100'  : 8.3600000000E-02,
    'GluinoToGChi0_1200_1111'  : 8.3600000000E-02,
    'GluinoToGChi0_1200_1122'  : 8.3600000000E-02,
    'GluinoToGChi0_1200_1148'  : 8.3600000000E-02,
    'GluinoToGChi0_1800_1047'  : 8.3600000000E-02,
    'GluinoToGChi0_1800_1122'  : 8.3600000000E-02,
    'GluinoToGChi0_1800_1200'  : 8.3600000000E-02,
    'GluinoToGChi0_1800_1272'  : 8.3600000000E-02,
    'GluinoToGChi0_1800_1341'  : 8.3600000000E-02,
    'GluinoToGChi0_1800_1407'  : 8.3600000000E-02,
    'GluinoToGChi0_1800_1469'  : 8.3600000000E-02,
    'GluinoToGChi0_1800_1529'  : 8.3600000000E-02,
    'StopToTChi0_400_75' : 8.3600000000E-02,
    'StopToTChi0_400_150' : 8.3600000000E-02,
    'StopToTChi0_400_199' : 8.3600000000E-02,
    'StopToTChi0_400_250' : 8.3600000000E-02,
    'StopToTChi0_400_285' : 8.3600000000E-02,
    'StopToTChi0_400_300' : 8.3600000000E-02,
    'StopToTChi0_600_100' : 8.3600000000E-02,
    'StopToTChi0_600_300' : 8.3600000000E-02,
    'StopToTChi0_600_400' : 8.3600000000E-02,
    'StopToTChi0_600_500' : 8.3600000000E-02,
    'StopToTChi0_1000_100' : 8.3600000000E-02,
    'StopToTChi0_1000_173' : 8.3600000000E-02,
    'StopToTChi0_1000_360' : 8.3600000000E-02,
    'StopToTChi0_1000_480' : 8.3600000000E-02,
    'StopToTChi0_1000_574' : 8.3600000000E-02,
    'StopToTChi0_1000_656' : 8.3600000000E-02,
    'StopToTChi0_1000_728' : 8.3600000000E-02,


    #stopped particles cosmic events
    'cosmic_preselection' : 8.3600000000E-02,

    #stopped particles stage1 GEN-SIM
    #from https://github.com/quertenmont/cmssw/blob/HSCP2015_PAS-EXO-15-010/SUSYBSMAnalysis/HSCP/test/AnalysisCode/Analysis_Samples.txt
    'mchamp100'  :  3.7779440000E+00,
    'mchamp200'  :  2.9865270000E-01,
    'mchamp400'  :  2.3099720000E-02,
    'mchamp600'  :  3.4651160000E-03,
    'mchamp800'  :  7.9696270000E-04,
    'mchamp1000' :  2.4031900000E-04,
    'mchamp1400' :  2.6633380000E-05,
    'mchamp1800' :  3.9346950000E-06,
    'mchamp2200' :  5.5863300000E-07,
    'mchamp2600' :  7.3973180000E-08,

    'gluino100'   : 8.1700000000E+04,
    'gluino200'   : 3.4000000000E+03,
    'gluino400'   : 9.4800000000E+01,
    'gluino600'   : 9.0700000000E+00,
    'gluino800'   : 1.4700000000E+00,
    'gluino1000'  : 3.2000000000E-01,
    'gluino1200'  : 8.3600000000E-02,
    'gluino1400'  : 2.4700000000E-02,
    'gluino1600'  : 7.9600000000E-03,
    'gluino1800'  : 2.7300000000E-03,
    'gluino2000'  : 9.7400000000E-04,
    'gluino2200'  : 3.5800000000E-04,
    'gluino2400'  : 1.3400000000E-04,
    'gluino2600'  : 5.0300000000E-05,

    #stopped particles stage2 GEN-SIM
    #'mchamp100_Stage2SeparateEventsParticle0'   : 3.7779440000E+00,
    #'mchamp200_Stage2SeparateEventsParticle0'   : 2.9865270000E-01,
    #'mchamp400_Stage2SeparateEventsParticle0'   : 2.3099720000E-02,
    'mchamp600_Stage2SeparateEventsParticle0'   :  3.4651160000E-03,
    #'mchamp800_Stage2SeparateEventsParticle0'   : 7.9696270000E-04,
    #'mchamp1000_Stage2SeparateEventsParticle0'  : 2.4031900000E-04,
    #'mchamp1400_Stage2SeparateEventsParticle0'  : 2.6633380000E-05,
    #'mchamp1800_Stage2SeparateEventsParticle0'  : 3.9346950000E-06,
    #'mchamp2200_Stage2SeparateEventsParticle0'  : 5.5863300000E-07,
    #'mchamp2600_Stage2SeparateEventsParticle0'  : 7.3973180000E-08,

    #stopped particles stage2 DIGI-HLT
    #'mchamp100_DigiHltSeparateEventsParticle0'   :  3.7779440000E+00,
    #'mchamp200_DigiHltSeparateEventsParticle0'   :  2.9865270000E-01,
    #'mchamp400_DigiHltSeparateEventsParticle0'   :  2.3099720000E-02,
    'mchamp600_DigiHltSeparateEventsParticle0'   :   3.4651160000E-03,
    #'mchamp800_DigiHltSeparateEventsParticle0'   :  7.9696270000E-04,
    #'mchamp1000_DigiHltSeparateEventsParticle0'  :  2.4031900000E-04,
    #'mchamp1400_DigiHltSeparateEventsParticle0'  :  2.6633380000E-05,
    #'mchamp1800_DigiHltSeparateEventsParticle0'  :  3.9346950000E-06,
    #'mchamp2200_DigiHltSeparateEventsParticle0'  :  5.5863300000E-07,
    #'mchamp2600_DigiHltSeparateEventsParticle0'  :  7.3973180000E-08,

    #stopped particles stage2 RECO
    #'mchamp100_RecoSeparateEventsParticle0'   :  3.7779440000E+00,
    #'mchamp200_RecoSeparateEventsParticle0'   :  2.9865270000E-01,
    #'mchamp400_RecoSeparateEventsParticle0'   :  2.3099720000E-02,
    'mchamp600_RecoSeparateEventsParticle0'   :   3.4651160000E-03,
    #'mchamp800_RecoSeparateEventsParticle0'   :  7.9696270000E-04,
    #'mchamp1000_RecoSeparateEventsParticle0'  :  2.4031900000E-04,
    #'mchamp1400_RecoSeparateEventsParticle0'  :  2.6633380000E-05,
    #'mchamp1800_RecoSeparateEventsParticle0'  :  3.9346950000E-06,
    #'mchamp2200_RecoSeparateEventsParticle0'  :  5.5863300000E-07,
    #'mchamp2600_RecoSeparateEventsParticle0'  :  7.3973180000E-08,

    #stopped particles stage2 Ntuples
    #'mchamp100_NtupleSeparateEventsParticle0'   :  3.7779440000E+00,
    #'mchamp200_NtupleSeparateEventsParticle0'   :  2.9865270000E-01,
    #'mchamp400_NtupleSeparateEventsParticle0'   :  2.3099720000E-02,
    'mchamp600_NtupleSeparateEventsParticle0'   :   3.4651160000E-03,
    #'mchamp800_NtupleSeparateEventsParticle0'   :  7.9696270000E-04,
    #'mchamp1000_NtupleSeparateEventsParticle0'  :  2.4031900000E-04,
    #'mchamp1400_NtupleSeparateEventsParticle0'  :  2.6633380000E-05,
    #'mchamp1800_NtupleSeparateEventsParticle0'  :  3.9346950000E-06,
    #'mchamp2200_NtupleSeparateEventsParticle0'  :  5.5863300000E-07,
    #'mchamp2600_NtupleSeparateEventsParticle0'  :  7.3973180000E-08,


    #rpc study
    'GluinoToGChi0_1200_1000_noiseFrate0p4' : 8.3600000000E-02,
    'GluinoToGChi0_1200_1000_noiseFrate0p6' : 8.3600000000E-02,
    'GluinoToGChi0_1200_1000_noiseFrate0p7' : 8.3600000000E-02,
    'GluinoToGChi0_1200_1000_noiseFrate0p8' : 8.3600000000E-02,
    'GluinoToGChi0_1200_1000_noiseFrate1p0' : 8.3600000000E-02,
    'GluinoToGChi0_1200_1000_nonoise' : 8.3600000000E-02,


}

InputCondorArguments = {}

pdgIdsForLifetimeReweighting = {
    # Defines the PDG IDs of the particles to be used for lifetime reweighting.
    # The keys are dataset labels and the values are either single PDG IDs or
    # lists of PDG IDs, e.g.:
    # 'stop1200_50mm'               : 1000006,
    # 'stopAndGluino1200_50mm_30mm' : [1000006, 1000021],
}

srcCTauForLifetimeReweighting = {
    # Defines the proper lifetimes (in units of cm/c) of the particles defined
    # by pdgIdsForLifetimeReweighting in the original samples before lifetime
    # reweighting. The keys are dataset labels and the values are either single
    # lifetime values or lists of lifetimes, e.g.:
    # 'stop1200_50mm'               : 10.0,
    # 'stopAndGluino1200_50mm_30mm' : [10.0, 10.0],
}

dstCTauForLifetimeReweighting = {
    # Defines the target proper lifetimes (in units of cm/c) of the particles
    # defined by pdgIdsForLifetimeReweighting to which the sample should be
    # reweighted.  The keys are dataset labels and the values are either single
    # lifetime values or lists of lifetimes, e.g.:
    # 'stop1200_50mm'               : 10.0,
    # 'stopAndGluino1200_50mm_30mm' : [5.0, 3.0],
}

##########################################################################
### code to set relevant parameters for displaced SUSY signal samples, ###
### which are a scan in the plane of stop mass and lifetime            ###
##########################################################################

import math

def mass(sample):
    start = sample.find("stop")+4
    end = sample.find("_")
    return sample[start:end]

def lifetime(sample):
    start = sample.find("_")+1
    end = sample.find("mm")
    lt = sample[start:end]
    return lt.replace("p",".")

##########################################################################

# generate list of masses
masses = [str(i*100) for i in range(2,13)]
# generate list of lifetimes
lifetimes = ["%g" % (0.1*i*(pow(10,j))) for j in range(4) for i in range(1,10)]
lifetimes.append("1000")
lifetimes = [lt.replace(".","p") for lt in lifetimes]

# generate list of sample names from masses, lifetimes
signal_datasets = ["stop%s_%smm" % (m,ctau) for m in masses for ctau in lifetimes]

datasets.extend(signal_datasets)
composite_dataset_definitions['DisplacedSUSYSignal'] = signal_datasets

signal_crossSections = {
    '200'  : 64.5085,
    '300'  : 8.51615,
    '400'  : 1.83537,
    '500'  : 0.51848,
    '600'  : 0.174599,
    '700'  : 0.0670476,
    '800'  : 0.0283338,
    '900'  : 0.0128895,
    '1000' : 0.00615134,
    '1100' : 0.00307413,
    '1200' : 0.00159844,
}

##########################################################################

for index, sample in enumerate(signal_datasets):
    nJobs[sample] = 99
    maxEvents[sample] = -1
    types[sample] = 'signalMC'
    labels[sample] = "#tilde{t}#tilde{t} M(%s) c#tau(%smm)" % (mass(sample), lifetime(sample))
    colors[sample] = 20 + index
    crossSections[sample] = signal_crossSections[mass(sample)]
    pdgIdsForLifetimeReweighting[sample] = 1000006
    dstCTauForLifetimeReweighting[sample] = 0.1*float(lifetime(sample))
    srcCTauForLifetimeReweighting[sample] = 0.1*10**(math.ceil(math.log10(float(lifetime(sample)))))
    # special case
    if lifetime(sample) == "0.1":
        srcCTauForLifetimeReweighting[sample] = 0.1*1.0

################################################################################
### code to set relevant parameters for disappearing tracks signal samples,  ###
### which are a scan in the plane of chargino mass and lifetime              ###
################################################################################

import re

new_nJobs = {}
for dataset0 in nJobs:
    if not re.match (r'AMSB_chargino_[^_]*GeV_[^_]*cm', dataset0):
        continue
    mass = re.sub (r'AMSB_chargino_([^_]*)GeV_[^_]*cm', r'\1', dataset0)
    ctau0 = float (re.sub (r'AMSB_chargino_[^_]*GeV_([^_]*)cm', r'\1', dataset0))
    for i in range (2, 9):
        ctau = ctauP = 0.1 * i * ctau0
        if int (ctau) * 10 == int (ctau * 10):
            ctau = ctauP = str (int (ctau))
        else:
            ctau = ctauP = str (ctau)
            ctauP = re.sub (r'\.', r'p', ctau)
        dataset = 'AMSB_chargino_' + mass + 'GeV_' + ctauP + 'cm'

        new_nJobs[dataset] = nJobs[dataset0]
        maxEvents[dataset] = maxEvents[dataset0]
        types[dataset] = types[dataset0]
        colors[dataset] = colors[dataset0]
        labels[dataset] = "AMSB #tilde{#chi}^{#pm} (" + mass + " GeV, " + ctau + " cm)"
        pdgIdsForLifetimeReweighting[dataset] = 1000024
        srcCTauForLifetimeReweighting[dataset] = ctau0
        dstCTauForLifetimeReweighting[dataset] = float (ctau)

nJobs.update (new_nJobs)
