#!/usr/bin/env python

from array import *
from OSUT3Analysis.Configuration.configurationOptions import *


###################################
#Give the dataset you want to get.#
###################################
datasets = [
         'QCD_MuEnriched',
]



txtDirectory = 'Nov_15' 
ConfigDirectory = 'Nov_15_Cof' 
outputDirectory = "/home/bing/CMSSW_6_1_2/src/DisplacedSUSY/BackgroundStudies/test/condor/BbBarIsoStudyMuon_Nov_14_2"

UserString = [
	'return_data             = 0',
	'email                   = bingxuan.liu@cern.ch',  # your email
	'copy_data               = 1',
	'storage_element     = T3_US_OSU',
	'user_remote_dir     = displacedSUSY_pickEvts',    # the directory you want to store the output files.
	'check_user_remote_dir   = 0',
	'publish_data            = 0'
]

