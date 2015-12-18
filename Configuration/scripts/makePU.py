#!/usr/bin/env python

import sys
import os
import re
from array import *
from optparse import OptionParser
from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *

parser = OptionParser()
parser = set_commandline_arguments(parser)
(arguments, args) = parser.parse_args()

if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.localConfig) + " import *")

condor_dir = set_condor_output_dir(arguments)

from ROOT import TFile, gROOT, gStyle, gDirectory, TStyle, THStack, TH1F, TCanvas, TString, TLegend, TArrow, THStack, TIter, TKey, TPaveLabel

gROOT.SetBatch()
outputFile = TFile(condor_dir + "/pu.root", "RECREATE")

processed_datasets = []

#### check which input datasets have valid output files
for sample in datasets:
    fileName = condor_dir + "/" + sample + ".root"
    if not os.path.exists(fileName):
        continue
    testFile = TFile(fileName)
    if not (testFile.IsZombie()):
        processed_datasets.append(sample)

if len(processed_datasets) is 0:
    sys.exit("No datasets have been processed")

#### open first input file and re-make its directory structure in the output file
testFile = TFile(condor_dir + "/" + processed_datasets[0] + ".root")
testFile.cd()
for key in testFile.GetListOfKeys():
    if (key.GetClassName() != "TDirectoryFile"):
        continue
    rootDirectory = key.GetName()

for sample in processed_datasets: # loop over different samples as listed in configurationOptions.py
    inputFile = TFile(condor_dir + "/" + sample + ".root")
    if(inputFile.IsZombie()):
        continue
    if(inputFile.Get(rootDirectory+"/pileup")):
        Histogram = inputFile.Get(rootDirectory+"/pileup").Clone()
        Histogram.SetDirectory(0)
        outputFile.cd()
        Histogram.Write (sample)
    else:
        print rootDirectory+"/pileup does not exist in " + condor_dir + "/" + sample + ".root"  
        continue
outputFile.Close()
