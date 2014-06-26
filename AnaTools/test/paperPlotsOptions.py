#!/usr/bin/env python

from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *

intLumi = 19680.  # MuEG 22Jan Re-Reco
systematics_file = "DisplacedSUSY.Configuration.systematicsDefinitions"
external_systematics_directory = 'DisplacedSUSY/Configuration/data/'




standard_datasets = [
    'Top',
    'EWK_WNjets_Other',
    'QCDFromData',
    'DYToTauTau_20',
    
    'MuEG_22Jan2013',
]

muon_datasets = [
    'Top',
    'EWK_WNjets_Other',
    'QCDFromDataMuon',
    'DYToTauTau_20',
    
    'MuEG_22Jan2013',
]

electron_datasets = [
    'Top',
    'EWK_WNjets_Other',
    'QCDFromDataElectron',
    'DYToTauTau_20',
    
    'MuEG_22Jan2013',
]

no_qcd_datasets = [
    'Top',
    'EWK_WNjets_Other',
    'DYToTauTau_20',
    
    'MuEG_22Jan2013',
]

signal_list = ['stopHadron200_1.0mm_br100']


###################################################
### REQUIRED arguments for each input histogram ###
###################################################

#   Variable name (type)
########################
# condor_dir (string)
# channel (string)
# name (string)
# output_dir (string)

####################################################
### OPTIONAL arguments for each input histogram  ###
####################################################

#   Variable name (type)
########################
# output_name (string)

# datasets (dictionary)
# setYMin,setYMax (double)
# setLogY (bool)
# rebinFactor (double)

# normalizeFactor (double)
# normalizeToUnitArea (bool)
# normalizeToData (bool)

# noStack (bool)
# makeRatioPlots (bool)
# makeDiffPlots (bool)
# ratioYRange (double)
# ratioRelErrMax (double)
# printYields (bool)
# includeSystematics (bool)
# addOverUnderFlow (bool)
# sortOrderByYields (bool)
# makeFancy (bool)


paper_histograms = [

## FIGURE 1: ONE PROMPT LEPTON CONTROL REGION
  {
    'condor_dir' : 'PromptNonPrompt_May_13',
    'channel' : 'MuonPromptElectronNonPrompt_Preselection',
    'name' : 'electronAbsD0BeamspotMVariableBinsForOverflow',
    'output_dir' : 'PromptNonPromptControlRegion',

    'datasets' : electron_datasets,
    'setYMin' : 1,
    'setLogY' : True,
    'includeSystematics' : True,
    'addOverUnderFlow' : True,
    'makeFancy' : True,
  },
  {
    'condor_dir' : 'PromptNonPrompt_May_13',
    'channel' : 'ElectronPromptMuonNonPrompt_Preselection',
    'name' : 'muonAbsD0BeamspotMVariableBinsForOverflow',
    'output_dir' : 'PromptNonPromptControlRegion',
    'datasets' : muon_datasets,
    'setYMin' : 0.01,
    'setLogY' : True,
    'includeSystematics' : True,
    'addOverUnderFlow' : True,
    'makeFancy' : True,
  },

## FIGURE 2: Z->TAUTAU CONTROL REGION
  {
    'condor_dir' : 'andrew_ZTauTautoEMu',
    'channel' : 'ZTauTautoEMu',
    'name' : 'electronAbsD0BeamspotMVariableBinsForOverflow',
    'output_dir' : 'otherBackgroundEst',
    'datasets' : no_qcd_datasets,
    'setYMin' : 1,
    'setLogY' : True,
    'includeSystematics' : True,
    'addOverUnderFlow' : True,
    'makeFancy' : True,
  },
  {
    'condor_dir' : 'andrew_ZTauTautoEMu',
    'channel' : 'ZTauTautoEMu',
    'name' : 'muonAbsD0BeamspotSVariableBinsForOverflow',
    'output_dir' : 'otherBackgroundEst',
    'datasets' : no_qcd_datasets,
    'setYMin' : 1,
    'setLogY' : True,
    'includeSystematics' : True,
    'addOverUnderFlow' : True,
    'makeFancy' : True,
  },

## FIGURE 3: PRESELECTION
  {
    'condor_dir' : 'MAY13__QCDEstimate',
    'channel' : 'Preselection_100um',
    'name' : 'electronAbsD0BeamspotMVariableBinsForOverflow',
    'output_dir' : 'DisplacedControlRegion',
    'datasets' : standard_datasets,
    'setYMin' : 0.01,
    'setLogY' : True,
    'includeSystematics' : True,
    'addOverUnderFlow' : True,
    'makeFancy' : True,
  },
  {
    'condor_dir' : 'MAY13__QCDEstimate',
    'channel' : 'Preselection_100um',
    'name' : 'muonAbsD0BeamspotMVariableBinsForOverflow',
    'output_dir' : 'DisplacedControlRegion',
    'datasets' : standard_datasets,
    'setYMin' : 0.1,
    'setLogY' : True,
    'includeSystematics' : True,
    'addOverUnderFlow' : True,
    'makeFancy' : True,
  },





]


