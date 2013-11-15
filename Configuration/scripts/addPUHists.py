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

if arguments.condorDir:
    condor_dir = set_condor_output_dir(arguments)
else:
    print "No condor output directory specified, shame on you"
    sys.exit(0)
    

def copyOneFile(fin, fout, dataset):  
    print "Copying histogram " + dataset 
    
    h = fin.Get(dataset) 
    if not h: 
        print "  Could not find hist named " + dataset 
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
    


########################################################################################
########################################################################################

fin  = TFile(condor_dir+"/pu.root", "READ");  
fout = TFile(os.environ['CMSSW_BASE']+"/src/OSUT3Analysis/Configuration/data/pu.root", "UPDATE"); 

print "Will merge histograms from " + fin.GetName() + " to " + fout.GetName() 
for dataset in datasets:    
     copyOneFile(fin, fout, dataset)

fin .Close()
fout.Close()    
print "Finished addPUHists.py successfully."  
    

   


