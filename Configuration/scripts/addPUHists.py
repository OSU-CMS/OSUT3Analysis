#!/usr/bin/env python

# addPUHists.py
# Takes the histograms from pu.root for the dataset(s) and condor directory specified in the local options file,
# and merges them into OSUT3Analysis/Configuration/data/pu.root.  
#
# For instructions on performing the pile-up reweighting, see https://twiki.cern.ch/twiki/bin/view/CMS/OSUT3Analysis#Pile_Up_Reweighting
#
# Sample usage:
# > addPUHists.py -l localOptionsPU.py -c condor_PU

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

parser.add_option("-i", "--inputFile", dest="inputFile", default = "", help="Specify the inputFile")
(arguments, args) = parser.parse_args()  

from ROOT import TFile, gROOT, gStyle, gDirectory, TStyle, THStack, TH1F, TCanvas, TString, TLegend, TArrow, THStack, TIter, TKey, TGraphErrors, Double 

def copyOneFile(dataset):  
    # If the input and output files are defined outside the loop, histograms after the first instance are not found.
    # I did not track down what the cause of this behavior was.  --Wells
    
    fin  = TFile(condor_dir+"/pu.root", "READ");  
    fout = TFile(os.environ['CMSSW_BASE']+"/src/OSUT3Analysis/Configuration/data/pu.root", "UPDATE"); 
    print "Copying histogram " + dataset + " from " + fin.GetName() + " to " + fout.GetName()  

    fin.cd()
    h = fin.Get(dataset) 
    if not h: 
        print "  Could not find hist named " + dataset + " in " + fin.GetName()  
        return 
        
    fout.cd()
    if dataset != h.GetName():  
        # print "  Resetting name from " + h.GetName() + " to " + dataset 
        h.SetName(dataset) 

    # Check if the histogram already exists in the destination file 
    h2 = fout.Get(dataset) 
    if h2: 
        print "  The histogram " + h2.GetName() + " already exists in the destination file."
        overwrite = raw_input("  Do you want to overwrite it? (y/n): ")
        if not (overwrite is "y"):
            print "  Will not overwrite existing histogram."  
            return # Only overwrite if the user enters "y"  

        # delete all previously existing instances of the histogram
        print "  Will overwrite existing histogram."  
        fout.Delete(dataset+";*")       

    h.Write()  
    
    fin.Close()
    fout.Close()
    
    
###############################################################################################################################The following function is for a special case when you creat a new dataset(with new name) by selecting events from a current dataset(like a skim). For example, one may want to creat w0jets sample from wjets sample. This function would copy the existing pu distribution of wjets and rename it as w0jets.root
#If you want to use pu distribution of dataset A as the pu distribution for dataset B, just include A and B in the localconfig file, and do not INCLUDE -c in the command line.  
##############################################################################################################################

def copyAndRenameOneFile(fout, datasets):  
    Name = "Dummy"
    for dataset in datasets:
    	h1 = fout.Get(dataset)
        if h1:
		h2 = fout.Get(dataset)
        else:
                Name = str(dataset)
    print " Will use the existing " + str(h2.GetName()) + " pu distribution as the pu distribution for " + Name
    h2.SetName(Name)
    fout.cd()
    h2.Write()


def addDataDistribution(fout, inputFile):  
    fin = TFile(inputFile)
    h1 = fin.Get('pileup')
    if not (h1):
        print fin + " doe not have pileup histograms"              
        return 
    else:
        h1.SetName(str(inputFile.split('.')[0]))
        fout.cd()
        h1.Write()

if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.localConfig) + " import *")

    UseExisting = False
    if arguments.condorDir:
        condor_dir = set_condor_output_dir(arguments)
    else:
        overwrite = raw_input("Do you want to copy an existing pu distribution and rename it(Type y) or you just forget to type -c(Type n, BTW, Shame on you)? ")
        if overwrite is "y":
    	    UseExisting = True
        else:
	    sys.exit(0)
    


########################################################################################
########################################################################################
    if not UseExisting:
        for dataset in datasets:    
     	    copyOneFile(dataset)  
    else:
        fout = TFile(os.environ['CMSSW_BASE']+"/src/OSUT3Analysis/Configuration/data/pu.root", "UPDATE"); 
        copyAndRenameOneFile(fout, datasets)
        fout.Close()
elif arguments.inputFile:
    fout = TFile(os.environ['CMSSW_BASE']+"/src/OSUT3Analysis/Configuration/data/pu.root", "UPDATE");
    addDataDistribution(fout, str(arguments.inputFile))
     
       
print "Finished addPUHists.py successfully."  
    

   


