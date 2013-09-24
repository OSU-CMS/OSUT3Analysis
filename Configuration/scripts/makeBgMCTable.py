#!/usr/bin/env python
import sys
import os
import fileinput
from array import *
from optparse import OptionParser
from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *
from OSUT3Analysis.Configuration.formattingUtilities import *

parser = OptionParser()

#input/output options
parser.add_option("-l", "--localConfig", dest="localConfig",
                  help="local configuration file")
parser.add_option("-c", "--condorDir", dest="condorDir",
                  help="condor output directory")
parser.add_option("-o", "--output-file", dest="outputFileName",
                  help="specify an output file name for the histogram file, default is 'bgMCTable.tex'")

#columns to include
parser.add_option("-b", "--box", action="store_true", dest="showDatasetColors", default=False,
                  help="include a column of dataset colors NOTE: the colors must be predefined in your latex file!")
parser.add_option("-s", "--shortName", action="store_true", dest="showShortNames", default=False,
                  help="include a column of component dataset nicknames (using the legend entry text)")
parser.add_option("-a", "--summedName", action="store_true", dest="showSummedNames", default=False,
                  help="include a column of composite dataset nicknames (using the legend entry text)")
parser.add_option("-f", "--fullNames", action="store_true", dest="showFullNames", default=False,
                  help="include a column of full dataset names")

parser.add_option("-x", "--xsection", action="store_true", dest="showXsections", default=False,
                  help="include a column of process cross sections")
parser.add_option("-e", "--efflumi", action="store_true", dest="showEffLumi", default=False,
                  help="include a column of effective luminosities")
parser.add_option("-n", "--numEvents", action="store_true", dest="showNumEvents", default=False,
                  help="include a column of number of generated events")
parser.add_option("-w", "--weight", action="store_true", dest="showWeight", default=False,
                  help="include a column of luminosity weighting factors")

#other options
parser.add_option("-r", "--replace", action="append", dest="replacements",
                  help="specify strings to be replaced by askterisks and moved into the caption.  the syntax more multiple replacements is '-r STRING1 -r STRING2' etc.")

#initialize parser
(arguments, args) = parser.parse_args()

#import from local config
if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + arguments.localConfig.rstrip('.py') + " import *")

#set condor directory
condor_dir = set_condor_output_dir(arguments)

#set output file name
outputFileName = "bgMCTable.tex"
if arguments.outputFileName:
    outputFileName = arguments.outputFileName

#outputFile = condor_dir + "/" + outputFileName
outputFile = outputFileName

from ROOT import TFile

hLine = "\\hline\n"
endLine = " \\\\ "
newLine = " \n"


def makeReplacements(line):
    if arguments.replacements:
        filler = "*"
        for replacement in arguments.replacements:
            line = line.replace(replacement,filler)
            filler = filler + "*"
    return line


#### check which bgMC input datasets have valid output files
## processed_datasets = []
## for dataset in datasets:
##     if types[dataset] is not "bgMC":
##         continue
##     fileName = condor_dir + "/" + dataset + ".root"
##     if not os.path.exists(fileName):
##         continue
##     testFile = TFile(fileName)
##     if not (testFile.IsZombie()):
##         processed_datasets.append(dataset)

## if len(processed_datasets) is 0:
##     sys.exit("Can't find any output root files for the given list of datasets")


#set the text for the luminosity label
if(intLumi < 1000.):
    LumiText = str.format('{0:.1f}', LumiInPb) + " \\pbinv"
else:
    LumiInFb = intLumi/1000.
    LumiText = str.format('{0:.1f}', LumiInFb) + " \\fbinv"

#setting up the column alignments for the table
numColumns = 0 #always show the full datatset name column
columnStructure = "{"
if arguments.showDatasetColors:
    numColumns = numColumns + 1
    columnStructure = columnStructure + "c"
if arguments.showShortNames:
    numColumns = numColumns + 1
    columnStructure = columnStructure + "c"
if arguments.showSummedNames:
    numColumns = numColumns + 1
    columnStructure = columnStructure + "c"
if arguments.showFullNames:
    numColumns = numColumns + 1
    columnStructure = columnStructure + "l"
if arguments.showNumEvents:
    numColumns = numColumns + 1
    columnStructure = columnStructure + "c"
if arguments.showXsections:
    numColumns = numColumns + 1
    columnStructure = columnStructure + "c"
if arguments.showEffLumi:
    numColumns = numColumns + 1
    columnStructure = columnStructure + "c"
if arguments.showWeight:
    numColumns = numColumns + 1
    columnStructure = columnStructure + "c"
columnStructure = columnStructure + "}"
    


fout = open (outputFile, "w")
fout.write ("\\makebox[0pt]{\\renewcommand{\\arraystretch}{1.2}\\begin{tabular}"+columnStructure+newLine+hLine)

#write the column headers to the output file
headerLine = ""
if arguments.showDatasetColors:
    headerLine = headerLine + "\\shortstack{Composite \\\\ Dataset \\\\ Color} & "
if arguments.showSummedNames:
    headerLine = headerLine + "\\shortstack{Composite \\\\ Dataset \\\\ Nickname} & "
if arguments.showShortNames:
    headerLine = headerLine + "\\shortstack{Individual \\\\ Dataset \\\\ Nickname} & "
if arguments.showFullNames:
    headerLine = headerLine + "\\multicolumn{1}{c}{Individual Dataset Source Name} & "
if arguments.showNumEvents:
    headerLine = headerLine + "\\shortstack{Generated \\\\ Events} & "
if arguments.showXsections:
    headerLine = headerLine + "\\shortstack{Cross \\\\ Section \\\\ (pb)} & "
if arguments.showEffLumi:
    headerLine = headerLine + "\\shortstack{Effective \\\\ Luminosity \\\\ (\\fbinv)} & "
if arguments.showWeight:
    headerLine = headerLine + "\\shortstack{Weighting \\\\ Factor \\\\ for " + LumiText + "} & "


fout.write (headerLine.rstrip(" & ")+endLine+newLine+hLine)


# for dataset in processed_datasets:
for dataset in datasets:
    if types[dataset] is not "bgMC":
        continue

    datasetLines = ""
    numComponents = 1
    component_datasets = []

    #if there are component datasets, we'll need all of them
    if dataset in composite_dataset_definitions:
        numComponents = len(composite_dataset_definitions[dataset])
        for component in composite_dataset_definitions[dataset]:
            component_datasets.append(component)
    else:
        component_datasets.append(dataset)


    #include the fancy multirow business for dataset nicknames
    if arguments.showDatasetColors:
        datasetLines = datasetLines + "\\multirow{"+str(numComponents)+"}{*}{\\color{"+str(colors[dataset])+"}{\\LARGE $\\blacksquare$}} & "
    if arguments.showSummedNames:
        rawlabel = "$" + labels[dataset] + "$"
        label = rawlabel.replace("#","\\").replace("\\rightarrow","{\\rightarrow}").replace(" ","\\ ")
        datasetLines = datasetLines + "\\multirow{"+str(numComponents)+"}{*}{"+label+"} & "


    #loop over each component (even if there's just one) and add the appropriate content to the table
    counter = 0
    for component in component_datasets:
        counter = counter + 1
        if arguments.showDatasetColors and counter > 1:
            datasetLines = datasetLines + " & "
        if arguments.showSummedNames and counter > 1:
            datasetLines = datasetLines + " & "
        if arguments.showShortNames:
            datasetLines = datasetLines + "$" + labels[component].replace("#","\\").replace("\\rightarrow","{\\rightarrow}").replace(" ","\\ ").replace("Pt","\\pt") + "$" + " & "
        if arguments.showFullNames:
            datasetLines = datasetLines + "\\texttt{" + makeReplacements(dataset_names[component]).lstrip("/").replace("/","_").replace("_","\_") + "}" + " & "
        if arguments.showNumEvents:
            fileName = condor_dir + "/" + component + "/numberOfEvents.txt"
            with open(fileName) as numEventsFile:
                    content = numEventsFile.readlines()
                    numEvents = content[0].strip("\n")
            datasetLines = datasetLines + formatNumber(str(round_sigfigs(float(numEvents),4))) + " & "
        if arguments.showXsections:
            fileName = condor_dir + "/" + component + "/crossSectionInPicobarn.txt"
            with open(fileName) as crossSectionFile:
                    content = crossSectionFile.readlines()
                    crossSection = content[0].strip("\n")
            datasetLines = datasetLines + formatNumber(str(round_sigfigs(float(crossSection),4))) + " & "
        if arguments.showEffLumi:
            fileName = condor_dir + "/" + component + "/numberOfEvents.txt"
            with open(fileName) as numEventsFile:
                    content = numEventsFile.readlines()
                    numEvents = content[0].strip("\n")
            fileName = condor_dir + "/" + component + "/crossSectionInPicobarn.txt"
            with open(fileName) as crossSectionFile:
                    content = crossSectionFile.readlines()
                    crossSection = content[0].strip("\n")
            datasetLines = datasetLines + formatNumber(str(round_sigfigs(float(numEvents)/float(crossSection)/1000.,3)).rstrip("0").rstrip(".")) + " & "
        if arguments.showWeight:
            fileName = condor_dir + "/" + component + "/numberOfEvents.txt"
            with open(fileName) as numEventsFile:
                    content = numEventsFile.readlines()
                    numEvents = content[0].strip("\n")
            fileName = condor_dir + "/" + component + "/crossSectionInPicobarn.txt"
            with open(fileName) as crossSectionFile:
                    content = crossSectionFile.readlines()
                    crossSection = content[0].strip("\n")
            datasetLines = datasetLines + formatNumber(str(round_sigfigs(intLumi*float(crossSection)/float(numEvents),3)).rstrip("0").rstrip(".")) + " & "


        datasetLines = datasetLines.rstrip(" & ")
        datasetLines = datasetLines + endLine + newLine
        
    fout.write(datasetLines)
    fout.write(hLine)



if arguments.replacements:
    replacementsText = ""
    filler = "*"
    for replacement in arguments.replacements:
        line = "\\multicolumn{"+str(numColumns)+"}{l}{" + " \\texttt{" + filler + " " + replacement.replace("/","_").replace("_","\_") + "}}" + endLine + newLine
        filler = filler + "*"
        replacementsText = replacementsText + line
    fout.write(replacementsText)




fout.write("\\end{tabular}}")
fout.close()



