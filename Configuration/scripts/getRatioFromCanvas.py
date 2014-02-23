#!/usr/bin/env python

# Script to extract the ratio histogram from a given canvas, that was produced with makePlots.py or makeEfficiencyPlots.py 
# Ratio histogram is saved in specified output file.  

# Example usage:
# getRatioFromCanvas.py -i condor/condor_2014_01_31_TriggerEffAfterTrkSelMet/eff_histograms2012DataRebin5Ratio.root -o condor/condor_2014_01_31_TriggerEffAfterTrkSelMet/dataVsMC.root -n metPt

import sys
from optparse import OptionParser


parser = OptionParser()
parser.add_option("-i", "--infile", dest="infile",
                  help="input root file")
parser.add_option("-o", "--outfile", dest="outfile",
                  help="output root file")
parser.add_option("-n", "--canName", dest="canName",
                  help="canvas name")

(arguments, args) = parser.parse_args()

from ROOT import TFile, gDirectory, TH1F, TCanvas, TPad 

inputFile = TFile(arguments.infile, "READ")
can = inputFile.Get(arguments.canName).Clone()  
if not can:
    print "Could not find TCanvas " + can + " in " + inputFile
padName = arguments.canName + "_2" # by convention, the pad with the ratio has the same name as the canvas but with a suffix of "_2"
padName = padName[padName.rfind('/')+1:]  # strip off anything before the last '/'  
pad  = can.GetPrimitive(padName)  
hist = pad.GetPrimitive("ratio")                    # the ratio histogram from makePlots.py is named "ratio"
if not hist:
    padName = padName.replace("_2", "")  
    hist = pad.GetPrimitive(padName);               # the ratio hist made by makeEfficiencyPlots.py will have the same name as the canvas 
if not hist:
    print "Error:  could not extract ratio histogram from canvas " + arguments.canName + " in file " + arguments.infile
    sys.exit(0)
outputFile = TFile(arguments.outfile, "RECREATE")
hist.Write()
outputFile.Close()  
inputFile.Close()

print "Finished writing ratio histogram " + arguments.canName + " to " + arguments.outfile


