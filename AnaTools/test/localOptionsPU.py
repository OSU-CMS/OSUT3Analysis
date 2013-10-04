#!/usr/bin/env python

# For usage instructions, see:
# https://twiki.cern.ch/twiki/bin/view/CMS/OSUT3Analysis#Pile_Up_Reweighting

import os

config_file = os.environ['CMSSW_BASE']+'/src/OSUT3Analysis/AnaTools/test/pu_cfg.py'  

datasets = [

### Add here the samples to run ####
#    'DY',
#    'Wjets', 
    

]


