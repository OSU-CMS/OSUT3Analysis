#!/usr/bin/env python

from array import *


#################################################################################
#This is the config file used to specify your email and remote directory on T3  #
#################################################################################
#If you want to creat pat file after get the reco file, please use CMSSW_5_3_11 #
#package to set up your crab env.                                               #
#pickEvents.py will creat a pickEvents_crablog directory which stores all the   #
#log files. Each dataset will have a directory named as 'condor directory name  #
#+ dataset name + crab' under pickEvents_crablog.                               #
#This pickEvents.py scraipt would submit all the crab jobs for you but right now#
#you have to check the status by yourself.                                      # 
#################################################################################

UserString = [
	'return_data             = 0',
	'email                   = bingxuan.liu@cern.ch',  # your email
	'copy_data               = 1',
	'storage_element     = T3_US_OSU',
	'user_remote_dir     = displacedSUSY_pickEvts',    # the directory you want to store the output files.
	'check_user_remote_dir   = 0',
	'publish_data            = 0',
        'ui_working_dir          ='
]

