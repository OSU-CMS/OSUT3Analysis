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
parser.add_option("-m", "--getMean", action="store_true", dest="getMean", default=False,
                  help="report the mean for each histogram (upper and lower bounds are ignored)")  
(arguments, args) = parser.parse_args()  

if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + arguments.localConfig.split(".")[0] + " import *")
 
DoRatio = False
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

    Nxbins = histogram.GetNbinsX() 
    xmax = histogram.GetXaxis().GetBinLowEdge(Nxbins+1)
    xlo = hist['xlo']
    xhi = hist['xhi']
    
    xloBin = histogram.GetXaxis().FindBin(float(xlo))
    if xhi > xmax:
    	print "xhi is outside the range of the histogram, will include all the overflow instead"
    xhiBin = histogram.GetXaxis().FindBin(float(xhi))
    xlo = histogram.GetXaxis().GetBinLowEdge(xloBin)   # lo edge is the left edge of the first bin 
    if xhi > xmax:
	xhi = "All to infinity"
    else:
	xhi = histogram.GetXaxis().GetBinLowEdge(xhiBin+1)
    intError = Double (0.0)
    integral = histogram.IntegralAndError(xloBin, xhiBin, intError)

    line = "Integral of " + hist['histName'] + " in " + inputFile.GetName() + " from " + str(xlo) + " to " + str(xhi) + ": " + str (integral) + " +- " + str (intError) 
    if arguments.getMean:
         line += "; Mean of entire histogram= " + str(histogram.GetMean()) + " +- " + str(histogram.GetMeanError())
    print line 
    if hist.has_key('role in ratioDic'):
    	DoRatio = True
        for key in RatioDic.keys():
            if key == hist['role in ratioDic'][0]:
                if hist['role in ratioDic'][1] == "Denominator":
                    RatioDic[key]['Denominator']['value'] = integral
                    RatioDic[key]['Denominator']['error'] = intError
                if hist['role in ratioDic'][1] == "Numerator":
                    RatioDic[key]['Numerator']['value'] = integral
                    RatioDic[key]['Numerator']['error'] = intError
if DoRatio:
	for key in RatioDic.keys():
		NumValue = RatioDic[key]['Numerator']['value']
        	DenValue = RatioDic[key]['Denominator']['value']
		NumError = RatioDic[key]['Numerator']['error']
		DenError = RatioDic[key]['Denominator']['error']
        	if DenValue == 0:
			print "Denominator of " + str(key) + " is 0 ! Please Check! "
			break
        	Ratio = NumValue/DenValue
        	RatioError = math.pow(math.pow(NumError,2)/math.pow(DenValue,2) + math.pow(DenError,2)*math.pow(NumValue,2)/math.pow(DenValue,4),0.5)
		print str(key) + " is " + str(Ratio) + " +- " + str(RatioError) 



