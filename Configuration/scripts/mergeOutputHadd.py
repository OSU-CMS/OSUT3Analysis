#!/usr/bin/env python

# Script to merge the output of root files that do not contain cutflow histograms.
# This is much simpler than mergeOutput.py:  it only runs hadd for the histograms in the specified datasets. 
#
# For example, may be used with PU files created using OSUT3Analysis/AnaTools/test/pu_cfg.py; see: 
# https://twiki.cern.ch/twiki/bin/view/CMS/OSUT3Analysis#Pile_Up_Reweighting
# Also may be used to merge TriggerEfficiencyAnalyzer output.  

import os
import sys
import fcntl
import datetime
import re
from optparse import OptionParser

from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *

parser = OptionParser()
parser = set_commandline_arguments(parser)

parser.remove_option("-o")
parser.remove_option("-n")
parser.remove_option("-u")
parser.remove_option("-e")
parser.remove_option("-r")
parser.remove_option("-R")
parser.remove_option("-d")
parser.remove_option("-b")
parser.remove_option("-y")
parser.remove_option("-p")
parser.remove_option("--2D")

(arguments, args) = parser.parse_args()

condor_dir = set_condor_output_dir(arguments)

if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.localConfig) + " import *")

for dataset in datasets:
    outputName = condor_dir + "/" + dataset + ".root"
    inputFiles = condor_dir + "/" + dataset + "/hist*.root"  
    command = "hadd -f %s %s" % (outputName, inputFiles)
    print "About to execute: " + command
    os.system(command)

