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

parser.add_option("-S", "--systematics", action="store_true", dest="includeSystematics", default=False,
                                    help="also lists the systematic uncertainties")
parser.add_option("-i", "--inputHistogram", dest="inputHistogram",
                  help="choose an input histogram and calculate the yield from its integral (histogram should be filled once per event)")

(arguments, args) = parser.parse_args()


if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.localConfig) + " import *")

if arguments.includeSystematics:
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", systematics_file) + " import *")



#set condor directory
condor_dir = set_condor_output_dir(arguments)


from ROOT import TFile, TH1F, gDirectory, Double


hLine = "\\hline\n"
endLine = " \\\\ "
newLine = " \n"


def getSystematicError(sample):
    errorSquared = 0.0
    if types[sample] is "data":
        return 0.0

    # add uncertainty on normalization method
    if sample in background_normalization_uncertainties:
        input_error = background_normalization_uncertainties[sample]['value']
        if '/' in input_error:
            line = input_error.split('/')
            minus_error = float(line[0]) - 1
            plus_error = float(line[1]) - 1
            if abs(minus_error) > abs(plus_error):
                error = minus_error
            else:
                error = plus_error
        else:
            error = float(input_error) - 1
        errorSquared = errorSquared + error * error

    # add global uncertainties
    for uncertainty in global_systematic_uncertainties:
        if sample in global_systematic_uncertainties[uncertainty]['applyList']:
            error = float(global_systematic_uncertainties[uncertainty]['value']) -1
            errorSquared = errorSquared + error * error

    # add sample-specific uncertainties from text files
    for uncertainty in external_systematic_uncertainties:
        input_file_path = os.environ['CMSSW_BASE'] + "/src/" + external_systematics_directory + "systematic_values__" + uncertainty + ".txt"
        if not os.path.exists(input_file_path):
            print "   skipping",uncertainty,"systematic for the",channel,"channel"
            return 0

        input_file = open(input_file_path)
        for line in input_file:
            line = line.rstrip("n").split(" ")
            dataset = line[0]
            if dataset != sample:
                continue
            if len(line) is 2: #just one error
                error = float(line[1]) - 1
            elif len(line) is 3: #asymmetric +- errors (we'll take the bigger one)
                minus_error = float(line[1]) - 1
                plus_error = float(line[2]) - 1
                if abs(minus_error) > abs(plus_error):
                    error = minus_error
                else:
                    error = plus_error
            errorSquared = errorSquared + error * error

    return math.sqrt(errorSquared)




#### check which input datasets have valid output files
processed_datasets = []
for dataset in datasets:
    fileName = condor_dir + "/" + dataset + ".root"
    if not os.path.exists(fileName):
        "input file not found for ",dataset,"dataset"
        continue
    testFile = TFile(fileName)
    if not (testFile.IsZombie()):
        processed_datasets.append(dataset)

#### exit if no datasets found
if len(processed_datasets) is 0:
    print datasets
    sys.exit("Can't find any output root files for the given list of datasets")


#open the first ROOT file and get the list of channels
channels = []
dataset_file = "%s/%s.root" % (condor_dir,processed_datasets[0])
inputFile = TFile(dataset_file)

for key in gDirectory.GetListOfKeys():
    if (key.GetClassName() != "TDirectoryFile"):
        continue
    if "CutFlowPlotter" not in key.GetName():
        continue
    channels.append(key.GetName())

#get and store the yields and errors for each dataset
yields = {}
stat_errors = {}
sys_errors = {}
bgMCSum = {}
bgMCStatErrSquared = {}
bgMCSysErrSquared = {}
processed_datasets_channels = {}

for channel in channels:
    bgMCSum[channel] = 0
    bgMCStatErrSquared[channel] = 0
    bgMCSysErrSquared[channel] = 0
    processed_datasets_channels[channel] = []

for sample in processed_datasets:
    yields[sample] = {}
    stat_errors[sample] = {}
    sys_errors[sample] = {}
    dataset_file = "%s/%s.root" % (condor_dir,sample)
    inputFile = TFile(dataset_file)
    for channel in channels:
        if not arguments.inputHistogram:
            cutFlowHistogram = inputFile.Get(channel+"/cutFlow")
            if not cutFlowHistogram:
                print "WARNING: didn't find cutflow for ", sample, "dataset in", channel, "channel"
                continue
            yield_ = cutFlowHistogram.GetBinContent(cutFlowHistogram.GetNbinsX())
            statError_ = cutFlowHistogram.GetBinError(cutFlowHistogram.GetNbinsX())

        else:
            newChannel = channel.replace("CutFlow","")
            inputHistogram = inputFile.Get(newChannel + "/" + arguments.inputHistogram)
            if not inputHistogram:
                print "WARNING: didn't find input histogram for ", sample, "dataset in", newChannel, "channel"
                continue
            statError_ = Double(0.0)
            yield_ = inputHistogram.IntegralAndError(0, inputHistogram.GetNbinsX()+1, statError_)

        processed_datasets_channels[channel].append(sample)

        if arguments.includeSystematics:
            fractionalSysError_ = getSystematicError(sample)
            sysError_ = fractionalSysError_ * yield_
        else:
            sysError_ = 0.0
        roundedNumbersDictionary = roundingNumbers(yield_,statError_,sysError_)
        if types[sample] is "data":
            yields[sample][channel] = str(int(yield_))
        else:
            yields[sample][channel] = str(roundedNumbersDictionary['central_value'])
        stat_errors[sample][channel] = str(roundedNumbersDictionary['stat_error'])
        sys_errors[sample][channel] = str(roundedNumbersDictionary['syst_error'])

        if types[sample] is "bgMC":
            bgMCSum[channel] = bgMCSum[channel] + yield_
            bgMCStatErrSquared[channel] = bgMCStatErrSquared[channel] + statError_*statError_
            bgMCSysErrSquared[channel] = bgMCSysErrSquared[channel] + sysError_*sysError_

    inputFile.Close()


#write a table for each channel to a separate tex file

for channel in channels:
    outputFile = condor_dir + "/yields_" + plainTextString(channel.replace("CutFlowPlotter","")) + ".tex"
    fout = open (outputFile, "w")
    if(arguments.standAlone):
        fout.write("\\documentclass{article}"+newLine+"\\begin{document}"+newLine)
    fout.write ("\\renewcommand{\\arraystretch}{1.2}\\begin{tabular}{lr}"+newLine+hLine)

    line = "Event Source & Event Yield $\pm$ 1$\sigma$ (stat.)"
    if arguments.includeSystematics:
        line = line + " $\pm$ 1$\sigma$ (syst.)"
    line = line +endLine+newLine+hLine
    fout.write(line)

    #write a line for each background sample
    bgMCcounter = 0
    for sample in processed_datasets_channels[channel]:
        if types[sample] is not "bgMC":
            continue

        bgMCcounter = bgMCcounter + 1
        rawlabel = labels[sample]
        label = rawlabel.replace("#tilde{t}","\\tilde{\\mathrm{t}}").replace("#bar{t}","$\\bar{\\mathrm{t}}$").replace("#tau","$\\tau$").replace("#nu","$\\nu$").replace("#rightarrow","${\\rightarrow}$").replace(" ","\\ ")
        line = label + " & " + yields[sample][channel] + " $\pm$ " + stat_errors[sample][channel]
        if arguments.includeSystematics:
            line = line + " $\pm$ " + sys_errors[sample][channel]
        line = line + endLine + newLine
        fout.write(line)

    #write a line with the sum of the backgrounds
    if bgMCcounter is not 0:
        roundedNumbersDictionary = roundingNumbers(bgMCSum[channel], math.sqrt(bgMCStatErrSquared[channel]),math.sqrt(bgMCSysErrSquared[channel]))
        bgMCSum_ = str(roundedNumbersDictionary['central_value'])
        bgMCStatErr_ = str(roundedNumbersDictionary['stat_error'])
        bgMCSysErr_ = str(roundedNumbersDictionary['syst_error'])
        line = hLine+"Total expected background & " + bgMCSum_ + " $\pm$ " + bgMCStatErr_
        if arguments.includeSystematics:
            line = line + " $\pm$ " + bgMCSysErr_
        line = line + endLine + newLine + hLine
        fout.write(line)

    #write a line for each data sample
    for sample in processed_datasets_channels[channel]:
        if types[sample] is not "data":
            continue

        label =  "Observation"
        fout.write(label + " & " + yields[sample][channel] + endLine + newLine + hLine)


    #write a line for each signal sample
    signalCounter = 0
    for sample in processed_datasets_channels[channel]:
        if types[sample] is not "signalMC":
            continue
        signalCounter += 1
        rawlabel = labels[sample]
        label = rawlabel.replace("#tilde{t}","\\tilde{\\mathrm{t}}").replace("#bar{t}","$\\bar{\\mathrm{t}}$").replace("#tau","$\\tau$").replace("#nu","$\\nu$").replace("#rightarrow","${\\rightarrow}$").replace(" ","\\ ")
        line = label + " & " + yields[sample][channel] + " $\pm$ " + stat_errors[sample][channel]
        if arguments.includeSystematics:
            line = line + " $\pm$ " + sys_errors[sample][channel]
        line = line + endLine + newLine
        fout.write(line)
    if signalCounter > 0:
        fout.write(hLine)

    fout.write("\\end{tabular}"+newLine)
    if(arguments.standAlone):
        fout.write("\\end{document}"+newLine)




    fout.close()

    if arguments.standAlone:
        #process tex files to make pdf files
        command = "pdflatex -interaction=batchmode -output-directory=./%s %s > /dev/null" % (condor_dir,outputFile)
        os.system(command)
        os.system(command)
        #os.system("rm %s" % outputFile)
        os.unlink ("%saux" % (outputFile.rstrip("tex")))
        os.unlink ("%slog" % (outputFile.rstrip("tex")))
        print "Finished writing cutFlow to " + outputFile + " and compiling pdf"



