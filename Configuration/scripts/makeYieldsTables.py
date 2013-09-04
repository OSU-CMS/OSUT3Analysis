#!/usr/bin/env python
import sys
import os
import re
import math
from array import *
from decimal import *
from optparse import OptionParser
from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *
from OSUT3Analysis.Configuration.formattingUtilities import *



### parse the command-line options

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
parser.remove_option("--2D")
parser.remove_option("-y")
parser.remove_option("-p")

parser.add_option("-s", "--standAlone", action="store_true", dest="standAlone", default=False,
                  help="adds the necessary header to be able to compile it")


(arguments, args) = parser.parse_args()


if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + arguments.localConfig.rstrip('.py') + " import *")

#set condor directory
condor_dir = set_condor_output_dir(arguments)


from ROOT import TFile, TH1F, gDirectory


hLine = "\\hline\n"
endLine = " \\\\ "
newLine = " \n"


#### check which input datasets have valid output files
processed_datasets = []
for dataset in datasets:
    if types[dataset] is "signalMC": #only include bgMC and data yields
        continue
    fileName = condor_dir + "/" + dataset + ".root"
    if not os.path.exists(fileName):
        continue
    testFile = TFile(fileName)
    if not (testFile.IsZombie()):
        processed_datasets.append(dataset)

#### exit if no datasets found        
if len(processed_datasets) is 0:
    sys.exit("Can't find any output root files for the given list of datasets")


#open the first ROOT file and get the list of channels
channels = []
dataset_file = "%s/%s.root" % (condor_dir,processed_datasets[0])
inputFile = TFile(dataset_file)
inputFile.cd("OSUAnalysis")

for key in gDirectory.GetListOfKeys():
    if (key.GetClassName() != "TDirectoryFile"):
        continue
    channels.append(key.GetName())

#get and store the yields and errors for each dataset                                                
yields = {}
errors = {}
bgMCSum = {}
bgMCErrSquared = {}
processed_datasets_channels = {}

for channel in channels:
    bgMCSum[channel] = 0
    bgMCErrSquared[channel] = 0
    processed_datasets_channels[channel] = []

for sample in processed_datasets:
    yields[sample] = {}
    errors[sample] = {}
    dataset_file = "%s/%s.root" % (condor_dir,sample)
    inputFile = TFile(dataset_file)
    for channel in channels:
        cutFlowHistogram = inputFile.Get("OSUAnalysis/"+channel+"CutFlow")
        if not cutFlowHistogram:
            print "WARNING: didn't find cutflow for ", sample, "dataset in", channel, "channel"
            continue
        processed_datasets_channels[channel].append(sample)

        yield_ = cutFlowHistogram.GetBinContent(cutFlowHistogram.GetNbinsX())
        error_ = cutFlowHistogram.GetBinError(cutFlowHistogram.GetNbinsX())
        yields[sample][channel] = formatNumber(str(round_sigfigs(yield_,3)).rstrip("0").rstrip("."))
        errors[sample][channel] = formatNumber(str(round_sigfigs(error_,3)).rstrip("0").rstrip("."))

        if types[sample] is "bgMC":
            bgMCSum[channel] = bgMCSum[channel] + yield_
            bgMCErrSquared[channel] = bgMCErrSquared[channel] + error_*error_

    inputFile.Close()


#write a table for each channel to a separate tex file

for channel in channels:
    outputFile = condor_dir + "/yields_" + plainTextString(channel) + ".tex"
    fout = open (outputFile, "w")
    if(arguments.standAlone):
        fout.write("\\documentclass{article}"+newLine+"\\begin{document}"+newLine)
    fout.write ("\\makebox[0pt]{\\renewcommand{\\arraystretch}{1.2}\\begin{tabular}{lr}"+newLine+hLine)

    fout.write("Event Source & Event Yield $\pm$ 1$\sigma$ (stat.)"+endLine+newLine+hLine)

    #write a line for each background sample
    bgMCcounter = 0
    for sample in processed_datasets_channels[channel]:
        if types[sample] is not "bgMC":
            continue
        bgMCcounter = bgMCcounter + 1
        rawlabel = "$" + labels[sample] + "$"
        label = rawlabel.replace("#","\\").replace("\\rightarrow","{\\rightarrow}").replace(" ","\\ ")
        fout.write(label + " & " + yields[sample][channel] + " $\pm$ " + errors[sample][channel] + endLine + newLine)

    #write a line with the sum of the backgrounds
    if bgMCcounter is not 0:

        bgMCSum_ = formatNumber(str(round_sigfigs(bgMCSum[channel],3)).rstrip("0").rstrip("."))
        bgMCErr_ = formatNumber(str(round_sigfigs(math.sqrt(bgMCErrSquared[channel]),3)).rstrip("0").rstrip("."))

        fout.write(hLine+"background sum & " + bgMCSum_ + " $\pm$ " + bgMCErr_ + endLine + newLine + hLine)

    #write a line for each data sample
    for sample in processed_datasets_channels[channel]:
        if types[sample] is not "data":
            continue
        rawlabel = "$" + labels[sample] + "$"
        label = rawlabel.replace("#","\\").replace("\\rightarrow","{\\rightarrow}").replace(" ","\\ ")
        fout.write(label + " & " + yields[sample][channel] + " $\pm$ " + errors[sample][channel] + endLine + newLine)
                                            
    fout.write("\\end{tabular}}"+newLine)
    if(arguments.standAlone):
        fout.write("\\end{document}"+newLine)

    fout.close()

    if arguments.standAlone:
        #process tex files to make pdf files
        command = "pdflatex -interaction=batchmode -output-directory=./%s %s > /dev/null" % (condor_dir,outputFile)
        os.system(command)
        os.system(command)
        #os.system("rm %s" % outputFile)
        os.system("rm %saux" % (outputFile.rstrip("tex")))
        os.system("rm %slog" % (outputFile.rstrip("tex")))
        print "Finished writing cutFlow to " + outputFile + " and compiling pdf"



