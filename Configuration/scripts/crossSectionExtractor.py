#!/usr/bin/env python3
# Script to extract the cross sections and filter efficiencies from MC samples.
# Produces output file, configurationOptionsXsecs.py, with content that can be
# appended to configurationOptions.py in the crossSections section.

# Usage:
# $ voms-proxy-init -voms cms  # Needed for xrootd access
# $ crossSectionExtractor.py


import sys
import os
import re
from math import *
from array import *
from ROOT import TFile
from DataFormats.FWLite import Events, Handle, Runs
from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *
from OSUT3Analysis.Configuration.formattingUtilities import *
from OSUT3Analysis.Configuration.miniAODV2_76X_Samples import *

datasets = [
    # The following are the miniAOD datasets for which we want to take the cross sections
    # from the AOD files.
    #'QCD_MuEnriched',
    #'QCD_EMEnriched',
    #'QCD_bcToE',
    #'SingleTop',
    # Other datasets that may also be checked:
    #'DYJetsToLL',
    #'TTJets',
    #'DYToMuMu_50',
    #'DYToEE_50',
    #'DYToMuMu',
    #'DYToEE',
    #'WJetsToLNu',
    'WZToLNu2QorQQ2L'
    #'WToENu',
    #'WToMuNu',
    #'Top',
    #'QCD',
    #'QCD_MuEnriched',
    #'QCD_bcToE',
    #'QCD_MuEnriched_20toInf',
]
split_datasets = split_composite_datasets(datasets, composite_dataset_definitions)

fout = open("configurationOptionsXSecs.py", "w")
content  = "############################################################# \n"
content += "# Cross sections below obatined with crossSectionExtractor.py \n"

for dataset in split_datasets:
    datasetName = dataset_names[dataset]
    fileList = os.popen('das_client.py --query="file dataset=' + datasetName + '" --limit 0').read().split('\n')
    xrootdPath = 'root://cmsxrootd.fnal.gov/' + str(fileList[0])
    f = TFile.Open(xrootdPath)  # Check that file is valid
    if not f:
        print "ERROR:  Could not open file: ", xrootdPath
        continue
    runs = Runs (xrootdPath)
    handle  = Handle ('GenRunInfoProduct')
    label = ("generator",'','SIM')
    for run in runs:
        run.getByLabel (label, handle)
        runGenInfo = handle.product()
        print "crossSection of      " + str(dataset) + " is: " + str(runGenInfo.crossSection())
        print "internal xsec of     " + str(dataset) + " is: " + str(runGenInfo.internalXSec().value())
        print "external LO xsec of  " + str(dataset) + " is: " + str(runGenInfo.externalXSecLO().value())
        print "external NLO xsec of " + str(dataset) + " is: " + str(runGenInfo.externalXSecNLO().value())
        print "filterEfficiency of  " + str(dataset) + " is: " + str(runGenInfo.filterEfficiency())
    content += "    " + ("'"+dataset+"'").ljust(35)
    content += ": "   + (str(runGenInfo.crossSection()) + "*" + str(runGenInfo.filterEfficiency())).rjust(20)
    content += ", \n"
    del datasetName
    del fileList
    del xrootdPath

content += "############################################################# \n"

fout.write(content)
fout.close()

print "\n\n\n"
print "The content of configurationOptionsXsecs.py below may be added to configurationOptions.py in the crossSections section."
print content



