#!/usr/bin/env python

# getHistIntegrals.py
# Prints the integral of specified histograms over the specified range.  
#
# Example usage:
# Usage:  getHistIntegrals.py -l sampleHistIntegrals.py 

import sys
import os
import re
from math import *
from array import *
from decimal import *
from optparse import OptionParser
import copy
from operator import itemgetter

from ROOT import TFile, gROOT, gStyle, gDirectory, TStyle, THStack, TH1F, TCanvas, TString, TLegend, TArrow, THStack, TIter, TKey, TGraphErrors, Double 

from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *
from OSUT3Analysis.Configuration.formattingUtilities import *


parser = OptionParser()
parser = set_commandline_arguments(parser)
(arguments, args) = parser.parse_args()  

if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + arguments.localConfig.rstrip('.py') + " import *")

  
for hist in input_hists:     # loop over different input hists in config file
   
    inputFile = TFile("condor/"+hist['condor_dir']+"/"+hist['dataset']+".root")
    #print "Reading file:  " + inputFile.GetName()

    HistogramObj = inputFile.Get("OSUAnalysis/"+hist['channel']+"/"+hist['histName'])
    if not HistogramObj:
        print "WARNING: Could not find histogram " + "OSUAnalysis/"+hist['channel']+"/"+hist['histName'] + " in file " + hist['dataset']+".root" + ". Will skip it and continue."
        continue 
    histogram = HistogramObj.Clone()
    histogram.SetDirectory(0)
    inputFile.Close()

    xlo = hist['xlo']
    xhi = hist['xhi']
    xloBin = histogram.GetXaxis().FindBin(float(xlo))
    xhiBin = histogram.GetXaxis().FindBin(float(xhi))
    xlo = histogram.GetXaxis().GetBinLowEdge(xloBin)   # lo edge is the left edge of the first bin 
    xhi = histogram.GetXaxis().GetBinLowEdge(xhiBin+1) # hi edge is the left edge of the bin to the right of the last bin
    intError = Double (0.0)
    integral = histogram.IntegralAndError(xloBin, xhiBin, intError)

#    print "xloBin = " + str(xloBin) 
#    print "xhiBin = " + str(xhiBin) 

    print "Integral of " + hist['histName'] + " in " + inputFile.GetName() + " from " + str(xlo) + " to " + str(xhi) + ": " + str (integral) + " +- " + str (intError) 




