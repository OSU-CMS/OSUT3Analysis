#!/usr/bin/env python

# rewtHist.py
# Reweights a specified histogram for a list of datasets by the weights specified in a weight histogram.
#
# Sample usage:
# > rewtHist.py -l localOptions.py -c condorDir -n OSUAnalysis/myChannel/histogramName -i OSUAnalysis/ctrlChannel/wtHistogramName -f fileWithWtHistogram.root -s ReweightedTest

import sys
import os
import re
from math import *
from array import *
from decimal import *
from optparse import OptionParser
import copy
from operator import itemgetter

from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *
from OSUT3Analysis.Configuration.formattingUtilities import *


parser = OptionParser()
parser = set_commandline_arguments(parser)
parser.remove_option("-n")
parser.remove_option("-d")

parser.add_option("-n", "--histToBeReWeighted", dest="histToBeReWeighted",
                  help="histogram that will be reweighted")
parser.add_option("-i", "--weightsHist", dest="weightsHist",
                  help="histogram used to get weights")
parser.add_option("-f", "--fileWithWtHist", dest="fileWithWtHist",
                  help="file containing weights histogram")
parser.add_option("-s", "--suffixRename", dest="suffixRename", default="_Reweighted",
                  help="suffix to add to histogram name after weighting")
parser.add_option("-d", "--singleDataset", dest="singleDataset",
                  help="Specify a single dataset to run over; overrides the list in the localOptions file")

(arguments, args) = parser.parse_args()

from ROOT import TFile, gROOT, gStyle, gDirectory, TStyle, THStack, TH1F, TCanvas, TString, TLegend, TArrow, THStack, TIter, TKey, TGraphErrors, Double

if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.localConfig) + " import *")

if arguments.singleDataset: # Only run over a single dataset; override definition in localConfig
    datasets = [ arguments.singleDataset ]

condor_dir = set_condor_output_dir(arguments)

def rewtOneHist(dataset, hwts):
    fileName = condor_dir + "/" + dataset + ".root"
    if not os.path.exists(fileName):
        print "WARNING: didn't find ",fileName
        return
    print "About to reweight histogram in " + fileName
    inFile = TFile(fileName, "UPDATE")
    if inFile.IsZombie() or not inFile.GetNkeys():
        return
    inFile.cd()
    h = inFile.Get(str(arguments.histToBeReWeighted)).Clone()
    if not h:
        print "  Could not find hist named " + arguments.histToBeReWeighted + " in " + inFile.GetName()
        return
    h.SetDirectory(0)
    newName = h.GetName() + str(arguments.suffixRename)
    h.SetName(newName)
    dir = arguments.histToBeReWeighted
    dir = dir[:dir.rfind("/")]
    print "Will write hist to directory " + dir
    inFile.cd(dir)
    tdir = inFile.GetDirectory(dir)
    tdir.Delete(newName + ";*")
    for i in range(1,h.GetNbinsX()+1):
        val = h.GetBinContent(i)
        err = h.GetBinError(i)
        binCtr = h.GetBinCenter(i)
        wt = hwts.GetBinContent(hwts.FindBin(binCtr))
        h.SetBinContent(i, val * wt)
        h.SetBinError  (i, err * wt)
    h.Write()

    inFile.Close()

########################################################################################
########################################################################################

print "Will take weights from hist " + str(arguments.weightsHist) + " in " + str(arguments.fileWithWtHist)

fileWithWtHist = TFile(str(arguments.fileWithWtHist))
hwts = fileWithWtHist.Get(arguments.weightsHist)

if arguments.rebinFactor:
    RebinFactor = int(arguments.rebinFactor)
    hwts.Rebin(RebinFactor)

for dataset in datasets:
    rewtOneHist(dataset, hwts)


print "Finished rewtHist.py successfully."





