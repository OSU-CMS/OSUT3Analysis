#!/usr/bin/env python
import sys
import os
import fileinput
import re
from array import *
from optparse import OptionParser
from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *

parser = OptionParser()
parser = set_commandline_arguments(parser)

parser.remove_option("-d")
parser.remove_option("-n")
parser.remove_option("-u")
parser.remove_option("-e")
parser.remove_option("-R")
parser.remove_option("-b")
parser.remove_option("-y")
parser.remove_option("-p")
parser.remove_option("-r")
parser.remove_option("-o")
parser.remove_option("--2D")

parser.add_option("-d", "--diff", action="store_true", dest="makeDiffPlots", default=False,
                  help="add a column for data-MC to the tables")
parser.add_option("-r", "--ratio", action="store_true", dest="makeRatioPlots", default=False,
                  help="add a colunn for (data-MC)/MC to the tables")
parser.add_option("-s", "--sToB", action="store_true", dest="signalToBackground", default=False,
                  help="add a column for S/sqrt(S+B) to the tables")
parser.add_option("-t", "--totBkgd", action="store_true", dest="totalBkgd", default=False,
                  help="add a column for total background to the tables")
parser.add_option("-o", "--output-file", dest="outputFileName",
                  help="specify an output file base name for the cutflow table (suffix will be appended), default is 'cutFlow'")


(arguments, args) = parser.parse_args()

condor_dir = set_condor_output_dir(arguments)

from ROOT import TFile, gROOT, gStyle, gDirectory, TKey

channels = []
processed_datasets = []

outputFileName = "cutFlow"
if arguments.outputFileName:
    outputFileName = arguments.outputFileName
outputFileName = outputFileName.partition(".")[0]  # If the input filename contains a period, take only the part before the period.  

texfile = condor_dir + "/" + outputFileName + ".tex"

replacements = {
    ">":"$>$",
    "<":"$<$",
    " eta ":" $\\eta$ ",
    " abs(eta) ":" $|\\eta|$ ",
    "#":"Num",

    "\\rightarrow":"{\\rightarrow}",
    "\\mu QCD":"\\mu$ $QCD",
    "EM QCD":"EM$ $QCD",
    "BCtoE QCD":"BCtoE$ $QCD",


    " pt ":" $p_{T}$ ",
    " ht ":"$H_{T}$ ",

    "d0":"$d_{0}$",

    " MET ":" Met ",
    " MEt ":" Met ",
    " met ":" Met ",
    " Met ":"$\\not\\!\\!{E}_{T}$ ",

    " MHT ":" Mht ",
    " MHt ":" Mht ",
    " mht ":" Mht ",
    " Mht ":"$\\not\\!\\!{H}_{T}$ ",

    "M_Z" : "$M_{Z}$",
    "M_mumu" : "$M_{\\mu\\mu}$",
    "M_ee" : "$M_{ee}$",
    "M_ll" : "$M_{ll}$",

}

secondary_replacements = {
    "$$<$":"$<"

}

replacements_extra = {}

if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.localConfig) + " import *")    

###read in any extra replacements from local config file
secondary_replacements.update(replacements_extra)

#### check which input datasets have valid output files
for dataset in datasets:
    fileName = condor_dir + "/" + dataset + ".root"
    if not os.path.exists(fileName):
        #print "Couldn't find output file for",dataset,"dataset"
        continue
    testFile = TFile(fileName)
    if not (testFile.IsZombie()):
        processed_datasets.append(dataset)

if len(processed_datasets) is 0:
    sys.exit("Can't find any output root files for the given list of datasets")

#### open first input file and re-make its directory structure in the output file
testFile = TFile(condor_dir + "/" + processed_datasets[0] + ".root")
testFile.cd()
for key in testFile.GetListOfKeys():
    if (key.GetClassName() != "TDirectoryFile"):
        continue
    rootDirectory = key.GetName()
    testFile.cd(key.GetName())
    for key2 in gDirectory.GetListOfKeys():
        if (key2.GetClassName() != "TDirectoryFile"):
            continue
        channels.append(key2.GetName())

fout = open (texfile, "w")
fout.write ("\\documentclass[a2paper,8pt]{article}\n\n")
fout.write ("\\usepackage[landscape,margin=0.15cm]{geometry}\n\n")
fout.write ("\\usepackage{multirow}\n\n")
fout.write ("\\begin{document}\n\n")
fout.write ("\\pagestyle{empty}\n\n")
fout.close ()

firstChannel = True

#weight = intLumi / 10000.0
#for dataset in processed_datasets:
#    dataset_file = "%s/%s.root" % (condor_dir,dataset)
#    fin = TFile (dataset_file)
#    flags = fin.Get ("flags")
#    noWeights = flags and flags.GetBinContent (1)
#    fin.Close ()
#    if types[dataset] != "data" and not noWeights:
#        os.system("mergeTFileServiceHistograms -i %s -o %s -w %g" % (dataset_file, dataset_file + "_tmp", weight))
#    else:
#        os.system("mergeTFileServiceHistograms -i %s -o %s -w %g" % (dataset_file, dataset_file + "_tmp", 1.0))


for channel in channels: # loop over final states, which each have their own directory
    formatted_channel = channel.replace("_"," ")
    cutFlowArgs = ""
    selectionArgs = ""
    minusOneArgs = ""
    #print hist

    for dataset in processed_datasets:
            dataset_file = "%s/%s.root" % (condor_dir,dataset)
            #print dataset_file
            if arguments.makeDiffPlots or arguments.makeRatioPlots or arguments.totalBkgd:
              if types[dataset] == "data":
                dataset_file = "\"<" + dataset_file + "\""
              elif types[dataset] == "bgMC":
                dataset_file = "\">" + dataset_file + "\""
            if arguments.signalToBackground:
              if types[dataset] == "signalMC":
                dataset_file = "\"<" + dataset_file + "\""
              elif types[dataset] == "bgMC":
                dataset_file = "\">" + dataset_file + "\""
            cutFlowArgs = cutFlowArgs + " " + dataset_file 
            selectionArgs = selectionArgs + " " + dataset_file 
            minusOneArgs = minusOneArgs + " " + dataset_file 
            cutFlowArgs = cutFlowArgs + " '" + channel + "CutFlow'"
            selectionArgs = selectionArgs + " '" + channel + "Selection'"
            minusOneArgs = minusOneArgs + " '" + channel + "MinusOne'"

            rawlabel = "$" + labels[dataset] + "$"
            label = rawlabel.replace("#","\\")
            label = "'" + label + "'"
            #print label
            cutFlowArgs = cutFlowArgs + " " + label
            selectionArgs = selectionArgs + " " + label
            minusOneArgs = minusOneArgs + " " + label

    #make cutFlowTable objects
    fout = open (texfile, "a")
    if not firstChannel:
        fout.write ("\\pagebreak\n\n")
    cutFlowTable = "cutFlowTable"
    if arguments.makeDiffPlots:
      cutFlowTable += " -d \"Data - MC\""
    if arguments.makeRatioPlots:
      cutFlowTable += " -r \"(Data - MC) / MC\""
    if arguments.totalBkgd:
      cutFlowTable += " -t \"Total Bkgd\""
    if arguments.signalToBackground:
      cutFlowTable += " -s \"$\\mathrm{S} / \\sqrt{\\mathrm{S} + \\mathrm{B}}$\""
    firstChannel = False
    fout.write ("\\section*{" + formatted_channel + " channel}\n\n")
    fout.write ("\\subsection*{Cut flow}\n\n")
    fout.close ()
#    print "Debug:  running command:  %s -l %g -m %s >> %s" % (cutFlowTable, intLumi,cutFlowArgs,texfile)
    os.system("%s -l %g -m %s >> %s" % (cutFlowTable, intLumi,cutFlowArgs,texfile))
    fout = open (texfile, "a")
    fout.write ("\\pagebreak\n\n")
    fout.write ("\\section*{" + formatted_channel + " channel}\n\n")
    fout.write ("\\subsection*{Individual selection}\n\n")
    fout.close ()
    os.system("%s -l %g %s >> %s" % (cutFlowTable, intLumi,selectionArgs,texfile))
    fout = open (texfile, "a")
    fout.write ("\\pagebreak\n\n")
    fout.write ("\\section*{" + formatted_channel + " channel}\n\n")
    fout.write ("\\subsection*{Minus one}\n\n")
    fout.close ()
    os.system("%s -l %g %s >> %s" % (cutFlowTable, intLumi,minusOneArgs,texfile))

#for dataset in processed_datasets:
#    dataset_file = "%s/%s.root_tmp" % (condor_dir,dataset)
#    os.remove(dataset_file)

#reformat tex files
for line in fileinput.FileInput(texfile,inplace=1):
    for replacement in replacements.keys():
        line = line.replace(replacement,replacements[replacement])
    print line,

for line in fileinput.FileInput(texfile,inplace=1):
    for replacement in secondary_replacements.keys():
        line = line.replace(replacement,secondary_replacements[replacement])
    print line,

fout = open (texfile, "a")
fout.write ("\\end{document}\n")
fout.close ()

#process tex files to make pdf files
command = "pdflatex -interaction=batchmode -output-directory=./%s %s > /dev/null" % (condor_dir,texfile)
os.system(command)
os.system(command)
#os.system("rm %s" % texfile)
os.system("rm %saux" % (texfile.rstrip("tex")))
os.system("rm %slog" % (texfile.rstrip("tex")))

print "Finished writing cutFlow to " + condor_dir + "/" + outputFileName + "{.tex,.pdf}"

