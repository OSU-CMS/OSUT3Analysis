#!/usr/bin/env python
import py_compile
import imp 
import sys
import os
import re
from optparse import OptionParser
from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *
from OSUT3Analysis.Configuration.formattingUtilities import *
from ROOT import TFile
parser = OptionParser()
parser = set_commandline_arguments(parser)

parser.remove_option("-o")
parser.remove_option("-c")
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

parser.add_option("-d", "--dataset", dest="Dataset", default = "", help="Specify which dataset to run.")
parser.add_option("-w", "--workDirectory", dest="Directory", default = "", help="Specify the working directroy.")
parser.add_option("-L", "--targetLumi", dest="IntLumi", default = "", help="Specify the targeting luminosity.")
#parser.add_option("-c", "--configuration", dest="Config", default = "", help="Specify the configuration file to run.")

(arguments, args) = parser.parse_args()
###############################################################################
#                       Get the exit code of condor jobs.                     #
###############################################################################
def MessageDecoder(Message, Good):
    Pattern = r'condor_(.*).log.*value (.*)\)'
    Decoded = re.match(Pattern,Message)    
    if not Good:
        print "Warning!!! Job " + Decoded.group(1) +" has non ZERO exit code: " + Decoded.group(2)
    return Decoded.group(1)   
###############################################################################
#  Get the string of good root files and the corresponding string of weights  #
###############################################################################
def GetGoodRootFiles(Index):
    return os.popen('ls *_' + str(Index) + '.root').read().rstrip('\n')
def MakeInputFileString(FilesSet):
    Str = ''
    for i in range(0,len(FilesSet)):
	Str = Str + ' ' + str(FilesSet[i])
    return Str
def MakeWeightsString(Weight,FilesSet):
    Str = ''
    for i in range(0,len(FilesSet)):
        Str = Str + ',' + str(Weight)
    return Str
###############################################################################
#   Get the total number of events from cutFlows to calculate the weights     #
###############################################################################
def GetTotalNumberOfEvents(FilesSet):
    TotalNumber = 0
    for File in FilesSet:
        ScoutFile = TFile(File)
        if ScoutFile.IsZombie(): 
            print File + " is a bad root file."
            FilesSet.remove(File)
            continue
        CutFlowObj = ScoutFile.Get("OSUAnalysis/cutFlow")
	if not CutFlowObj:
	    print "Could not find cutFlow histogram in " + str(File) + " !"
	    continue
        else:
	    CutFlow = CutFlowObj.Clone()
            CutFlow.SetDirectory(0)
            TotalNumber = TotalNumber + CutFlow.GetBinContent(1) 
    return TotalNumber
###############################################################################
#                           Getting the working directory.                    #
###############################################################################
CondorDir = ''
if arguments.Directory == "":
    print "No working directory is given, aborting."
    sys.exit()
else:
    CondorDir = os.getcwd() + '/condor/' + arguments.Directory
###############################################################################
#Check whether the necessary arguments or the local config are given correctly#
###############################################################################
split_datasets = []
IntLumi = 0.0
if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.localConfig) + " import *")
    composite_datasets = get_composite_datasets(datasets, composite_dataset_definitions)
    split_datasets = split_composite_datasets(datasets, composite_dataset_definitions)
    IntLumi = intLumi    
if not arguments.localConfig:
    if arguments.Dataset == "":
        print "There are no datasets to merge!"
    else:
        split_datasets.append(arguments.Dataset) 
    IntLumi = float(arguments.IntLumi)

for dataSet in split_datasets:
    directory = CondorDir + '/' + dataSet
    if not os.path.exists(directory):
        print directory + " does not exist, will skip it and continue!"
        continue
    print "....................Merging dataset " + dataSet + " ...................."
    os.chdir(directory)
    ReturnValues = os.popen('grep "return value" *.log').readlines()
    LogFiles = os.popen('grep "submitted" *.log').readlines()
    GoodIndices = []
    GoodRootFiles = []

    for i in range(0,len(ReturnValues)):
        if "return value 0" not in ReturnValues[i]:
            BadIndex = MessageDecoder(ReturnValues[i],False)
        else:    
            GoodIndex = MessageDecoder(ReturnValues[i], True)
            GoodIndices.append(GoodIndex)

    for i in range(0,len(GoodIndices)):
        GoodRootFiles.append(GetGoodRootFiles(GoodIndices[i]))

    if not len(GoodRootFiles):
        print "Unfortunately there are no good root files to merge!"
        continue
    InputFileString = MakeInputFileString(GoodRootFiles)
    sys.path.append(directory)
    exec('import datasetInfo_' + dataSet + '_cfg as datasetInfo')
    TotalNumber = GetTotalNumberOfEvents(GoodRootFiles)
    if not TotalNumber:
        continue
    Weight = 1.0
    crossSection = float(datasetInfo.crossSection)
    if crossSection != -1:
        Weight = IntLumi*crossSection/float(TotalNumber)
    InputWeightString = MakeWeightsString(Weight, GoodRootFiles)
    os.system('mergeTFileServiceHistograms -i ' + InputFileString + ' -o ' + dataSet + '.root' + ' -w ' + InputWeightString)
    os.system('mv ' + dataSet + '.root ' + '../')
    print "\nFinish merging dataset " + dataSet + ":"
    print "    "+ str(len(GoodRootFiles)) + " good files are used for merging out of " + str(len(LogFiles)) + " submitted jobs."
    print "    The targeting luminosity is " + str(IntLumi) + " inverse pb."
    if crossSection != -1:
        print "    The crossSection of dataset " + dataSet + " is " + str(crossSection) + " pb."
    print "    The weighting factor is " + str(Weight) + ", which is a nunitless number." 
    print "...............................................................\n"
    os.chdir(CondorDir) 

for dataSet_component in composite_datasets:
    print "................Merging composite dataset " + dataSet + " ................"
    memberList = []
    for dataset in composite_dataset_definitions[dataSet_component]:
        if not os.path.exists(dataset + '.root'):
            print dataset + '.root does not exist, component dataset ' + dataSet_component + ' wont be complete!'
            continue  	
        memberList.append(dataset + '.root')
    InputFileString = MakeInputFileString(memberList)
    os.system('mergeTFileServiceHistograms -i ' + InputFileString + ' -o ' + dataSet_component + '.root')
    print 'Finish merging composite dataset ' + dataSet_component
    print "...............................................................\n"
