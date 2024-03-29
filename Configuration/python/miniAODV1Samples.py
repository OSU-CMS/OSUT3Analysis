#!/usr/bin/env python3

############################################################################################################
#########  LIST OF MINIAOD V1 DATASETS  ####################################################################
############################################################################################################

dataset_names = {
    ############################################################################
    #MiniAOD stored on T3.
    ############################################################################
    #DY
    'DYJetsToLL_50'  :  "/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3/MINIAODSIM",
    #We have two DY samples, add another one commented out here for you convenience.
    #'DYJetsToLL_50'  :  "/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'DYJetsToNuNu'   :  "/DYJetsToNuNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",

    #WJets
    'WJetsToLNu'  :  "/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",

    #WJets_HT
    'WJetsToLNu_HT100to200' : "/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'WJetsToLNu_HT200to400' : "/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'WJetsToLNu_HT400to600' : "/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3/MINIAODSIM",
    'WJetsToLNu_HT600toInf' : "/WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",

    #WW
    'WWToLNuQQ'   :  "/WWTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'WWToLNuLNu'  :  "/WWTo2L2Nu_13TeV-powheg/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",

    #WZ
    'WZToLNuQQ'    :  "/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'WZToLLQQ'     :  "/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM",
    'WZToLNuNuNu'  :  "/WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'WZToLLLNu'    :  "/WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",

    #ZZ
    'ZZToNuNuQQ'  :  "/ZZTo2Q2Nu_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'ZZToLLQQ'    :  "/ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'ZZToLLNuNu'  :  "/ZZTo2L2Nu_13TeV_powheg_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'ZZToLLLL'    :  "/ZZTo4L_13TeV_powheg_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",

    #VG
    'WG'  :  "/WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'ZG'  :  "/ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",

    #SingleTop
    'SingleTop_s_channel'  :  "/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'SingleTop_t_channel'  :  "/ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9_ext1-v1/MINIAODSIM",
    'SingleTop_tW'         :  "/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'SingleTop_tbarW'      :  "/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",

    #TTJets
    'TTJets_DiLept'              :  "/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9_ext1-v1/MINIAODSIM",
    'TTJets_SingleLeptFromT'     :  "/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9_ext1-v1/MINIAODSIM",
    'TTJets_SingleLeptFromTbar'  :  "/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9_ext1-v1/MINIAODSIM",

    #MET data
    'MET_2015D_05Oct2015' : "/MET/Run2015D-05Oct2015-v1/MINIAOD",
    'MET_2015D_v3'        : "/MET/ahart-Run2015D-PromptReco-v3-DisappTrks-v1-5e1b2f90f66dd8f324805a21b41c0bb6/USER",
    'MET_2015D_v4'        : "/MET/ahart-Run2015D-PromptReco-v4-DisappTrks-v1-5e1b2f90f66dd8f324805a21b41c0bb6/USER",

    #AMSB chargino
    'AMSB_chargino_100GeV_10cm'    :  "/AMSB_chargino_M-100_CTau-10_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'AMSB_chargino_100GeV_100cm'   :  "/AMSB_chargino_M-100_CTau-100_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'AMSB_chargino_100GeV_1000cm'  :  "/AMSB_chargino_M-100_CTau-1000_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",

    'AMSB_chargino_300GeV_10cm'    :  "/AMSB_chargino_M-300_CTau-10_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'AMSB_chargino_300GeV_100cm'   :  "/AMSB_chargino_M-300_CTau-100_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'AMSB_chargino_300GeV_1000cm'  :  "/AMSB_chargino_M-300_CTau-1000_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",

    'AMSB_chargino_500GeV_10cm'    :  "/AMSB_chargino_M-500_CTau-10_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'AMSB_chargino_500GeV_100cm'   :  "/AMSB_chargino_M-500_CTau-100_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'AMSB_chargino_500GeV_1000cm'  :  "/AMSB_chargino_M-500_CTau-1000_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",

    'AMSB_chargino_700GeV_10cm'    :  "/AMSB_chargino_M-700_CTau-10_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'AMSB_chargino_700GeV_100cm'   :  "/AMSB_chargino_M-700_CTau-100_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'AMSB_chargino_700GeV_1000cm'  :  "/AMSB_chargino_M-700_CTau-1000_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    ############################################################################

    ############################################################################
    #MiniAOD not stored on T3.
    ############################################################################
    'DYJetsToLL_10to50' : "/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'TTJets'            : "/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM",
    'ZZ'                : "/ZZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3/MINIAODSIM",
    'WW'                : "/WW_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'WZ'                : "/WZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",

    'DYJetsToLL_50_50ns'     : "/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/MINIAODSIM",
    'DYJetsToLL_10to50_50ns' : "/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM",
    'WJetsToLNu_50ns'        : "/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM",
    'TTJets_50ns'            : "/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM",
    'ZZ_50ns'                : "/ZZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/MINIAODSIM",
    'WW_50ns'                : "/WW_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM",
    'WZ_50ns'                : "/WZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/MINIAODSIM",

    #QCD MuEnriched
    'QCD_MuEnriched_20toInf'        : "/QCD_Pt-20toInf_MuEnrichedPt15_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'QCD_MuEnriched_15to20'         : "/QCD_Pt-15to20_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'QCD_MuEnriched_20to30'         : "/QCD_Pt-20to30_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM",
    'QCD_MuEnriched_30to50'         : "/QCD_Pt-30to50_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'QCD_MuEnriched_50to80'         : "/QCD_Pt-50to80_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'QCD_MuEnriched_80to120'        : "/QCD_Pt-80to120_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'QCD_MuEnriched_120to170'       : "/QCD_Pt-120to170_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM",
    'QCD_MuEnriched_170to300'       : "/QCD_Pt-170to300_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM",
    'QCD_MuEnriched_300to470'       : "/QCD_Pt-300to470_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM",
    'QCD_MuEnriched_470to600'       : "/QCD_Pt-470to600_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM",
    'QCD_MuEnriched_600to800'       : "/QCD_Pt-600to800_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM",
    'QCD_MuEnriched_800to1000'      : "/QCD_Pt-800to1000_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM",
    'QCD_MuEnriched_1000toInf'      : "/QCD_Pt-1000toInf_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM",
    #QCD EMEnriched
    'QCD_EMEnriched_15to20'         : "/QCD_Pt-15to20_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'QCD_EMEnriched_20to30'         : "/QCD_Pt-20to30_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'QCD_EMEnriched_30to50'         : "/QCD_Pt-30to50_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'QCD_EMEnriched_50to80'         : "/QCD_Pt-50to80_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'QCD_EMEnriched_80to120'        : "/QCD_Pt-80to120_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3/MINIAODSIM",
    'QCD_EMEnriched_120to170'       : "/QCD_Pt-120to170_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'QCD_EMEnriched_170to300'       : "/QCD_Pt-170to300_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'QCD_EMEnriched_300toInf'       : "/QCD_Pt-300toInf_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM",
    #QCD bcToE
    'QCD_bcToE_15to20'              : "/QCD_Pt_15to20_bcToE_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'QCD_bcToE_20to30'              : "/QCD_Pt_20to30_bcToE_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'QCD_bcToE_30to80'              : "/QCD_Pt_30to80_bcToE_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM",
    'QCD_bcToE_80to170'             : "/QCD_Pt_80to170_bcToE_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM",
    'QCD_bcToE_170to250'            : "/QCD_Pt_170to250_bcToE_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
    'QCD_bcToE_250toInf'            : "/QCD_Pt_250toInf_bcToE_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM",
    #DisplacedSUSY Signal MC MiniAOD
    'stop200_1mm'    : "/DisplacedSUSY_StopToBL_M-200_CTau-1_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop200_10mm'   : "/DisplacedSUSY_StopToBL_M-200_CTau-10_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop200_100mm'  : "/DisplacedSUSY_StopToBL_M-200_CTau-100_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop200_1000mm' : "/DisplacedSUSY_StopToBL_M-200_CTau-1000_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop300_1mm'    : "/DisplacedSUSY_StopToBL_M-300_CTau-1_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop300_10mm'   : "/DisplacedSUSY_StopToBL_M-300_CTau-10_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop300_100mm'  : "/DisplacedSUSY_StopToBL_M-300_CTau-100_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop300_1000mm' : "/DisplacedSUSY_StopToBL_M-300_CTau-1000_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop400_1mm'    : "/DisplacedSUSY_StopToBL_M-400_CTau-1_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop400_10mm'   : "/DisplacedSUSY_StopToBL_M-400_CTau-10_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop400_100mm'  : "/DisplacedSUSY_StopToBL_M-400_CTau-100_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop400_1000mm' : "/DisplacedSUSY_StopToBL_M-400_CTau-1000_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop500_1mm'    : "/DisplacedSUSY_StopToBL_M-500_CTau-1_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop500_10mm'   : "/DisplacedSUSY_StopToBL_M-500_CTau-10_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop500_100mm'  : "/DisplacedSUSY_StopToBL_M-500_CTau-100_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop500_1000mm' : "/DisplacedSUSY_StopToBL_M-500_CTau-1000_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop600_1mm'    : "/DisplacedSUSY_StopToBL_M-600_CTau-1_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop600_10mm'   : "/DisplacedSUSY_StopToBL_M-600_CTau-10_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop600_100mm'  : "/DisplacedSUSY_StopToBL_M-600_CTau-100_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop600_1000mm' : "/DisplacedSUSY_StopToBL_M-600_CTau-1000_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop700_1mm'    : "/DisplacedSUSY_StopToBL_M-700_CTau-1_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop700_10mm'   : "/DisplacedSUSY_StopToBL_M-700_CTau-10_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop700_100mm'  : "/DisplacedSUSY_StopToBL_M-700_CTau-100_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop700_1000mm' : "/DisplacedSUSY_StopToBL_M-700_CTau-1000_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop800_1mm'    : "/DisplacedSUSY_StopToBL_M-800_CTau-1_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop800_10mm'   : "/DisplacedSUSY_StopToBL_M-800_CTau-10_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop800_100mm'  : "/DisplacedSUSY_StopToBL_M-800_CTau-100_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop800_1000mm' : "/DisplacedSUSY_StopToBL_M-800_CTau-1000_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop900_1mm'    : "/DisplacedSUSY_StopToBL_M-900_CTau-1_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop900_10mm'   : "/DisplacedSUSY_StopToBL_M-900_CTau-10_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop900_100mm'  : "/DisplacedSUSY_StopToBL_M-900_CTau-100_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop900_1000mm' : "/DisplacedSUSY_StopToBL_M-900_CTau-1000_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop1000_1mm'   : "/DisplacedSUSY_StopToBL_M-1000_CTau-1_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop1000_10mm'  : "/DisplacedSUSY_StopToBL_M-1000_CTau-10_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop1000_100mm' : "/DisplacedSUSY_StopToBL_M-1000_CTau-100_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop1000_1000mm': "/DisplacedSUSY_StopToBL_M-1000_CTau-1000_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop1100_1mm'   : "/DisplacedSUSY_StopToBL_M-1100_CTau-1_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop1100_10mm'  : "/DisplacedSUSY_StopToBL_M-1100_CTau-10_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop1100_100mm' : "/DisplacedSUSY_StopToBL_M-1100_CTau-100_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop1100_1000mm': "/DisplacedSUSY_StopToBL_M-1100_CTau-1000_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop1200_1mm'   : "/DisplacedSUSY_StopToBL_M-1200_CTau-1_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop1200_10mm'  : "/DisplacedSUSY_StopToBL_M-1200_CTau-10_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop1200_100mm' : "/DisplacedSUSY_StopToBL_M-1200_CTau-100_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
    'stop1200_1000mm': "/DisplacedSUSY_StopToBL_M-1200_CTau-1000_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",

    'DoubleMu_2015B'       : "/DoubleMuon/Run2015B-PromptReco-v1/MINIAOD",
    'DoubleMu_2015C'       : "/DoubleMuon/Run2015C-PromptReco-v1/MINIAOD",
    'DoubleMu_2015D_v3'    : "/DoubleMuon/Run2015D-05Oct2015-v1/MINIAOD",
    'DoubleMu_2015D_v4'    : "/DoubleMuon/Run2015D-PromptReco-v4/MINIAOD",

    'SingleMu_2015B'       : "/SingleMuon/Run2015B-PromptReco-v1/MINIAOD",
    'SingleMu_2015C'       : "/SingleMuon/Run2015C-PromptReco-v1/MINIAOD",
    'SingleMu_2015D_v3'    : "/SingleMuon/ahart-Run2015D-PromptReco-v3-DisappTrks-v1-5e1b2f90f66dd8f324805a21b41c0bb6/USER",
    'SingleMu_2015D_v4'    : "/SingleMuon/ahart-Run2015D-PromptReco-v4-DisappTrks-v1-5e1b2f90f66dd8f324805a21b41c0bb6/USER",

    'SingleEle_2015B'      : "/SingleElectron/Run2015B-PromptReco-v1/MINIAOD",
    'SingleEle_2015C'      : "/SingleElectron/Run2015C-PromptReco-v1/MINIAOD",
    'SingleEle_2015D_v3'   : "/SingleElectron/ahart-Run2015D-PromptReco-v3-DisappTrks-v1-5e1b2f90f66dd8f324805a21b41c0bb6/USER",
    'SingleEle_2015D_v4'   : "/SingleElectron/ahart-Run2015D-PromptReco-v4-DisappTrks-v1-5e1b2f90f66dd8f324805a21b41c0bb6/USER",

    'SinglePhoton_2015B'   : "/SinglePhoton/Run2015B-PromptReco-v1/MINIAOD",
    'SinglePhoton_2015C'   : "/SinglePhoton/Run2015C-PromptReco-v1/MINIAOD",
    'SinglePhoton_2015D'   : "/SinglePhoton/Run2015D-PromptReco-v3/MINIAOD",

    'DoubleEG_2015B'       : "/DoubleEG/Run2015B-PromptReco-v1/MINIAOD",
    'DoubleEG_2015C'       : "/DoubleEG/Run2015C-PromptReco-v1/MINIAOD",
    'DoubleEG_2015D_v3'    : "/DoubleEG/Run2015D-05Oct2015-v1/MINIAOD",
    'DoubleEG_2015D_v4'    : "/DoubleEG/Run2015D-PromptReco-v4/MINIAOD",

    'MuonEG_2015B'         : "/MuonEG/Run2015B-PromptReco-v1/MINIAOD",
    'MuonEG_2015C'         : "/MuonEG/Run2015C-PromptReco-v1/MINIAOD",
    'MuonEG_2015D_v3'      : "/MuonEG/Run2015D-PromptReco-v3/MINIAOD",
    'MuonEG_2015D_v4'      : "/MuonEG/Run2015D-PromptReco-v4/MINIAOD",
    ############################################################################
}
