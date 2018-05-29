#!/usr/bin/env python
import sys
import os
import re
import time
import datetime
from math import *
from array import *
from decimal import *
from optparse import OptionParser
from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *
from OSUT3Analysis.Configuration.formattingUtilities import *



### parse the command-line options

parser = OptionParser()
parser = set_commandline_arguments(parser)

(arguments, args) = parser.parse_args()


if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.localConfig) + " import *")



from ROOT import TFile, gDirectory, TH1D

##########################################################################################################################################
##########################################################################################################################################
##########################################################################################################################################

processed_datasets = []

condor_dir = set_condor_output_dir(arguments)

#### check which input datasets have valid output files

for sample in datasets:
    fileName = condor_dir + "/" + sample + ".root"

    if not os.path.exists(fileName):
        print "WARNING: didn't find ",fileName
        continue
    testFile = TFile(fileName)
    if testFile.IsZombie() or not testFile.GetNkeys():
        continue
    processed_datasets.append(sample)

if len(processed_datasets) is 0:
    sys.exit("No datasets have been processed")


for dataset in processed_datasets:

    inputFile = TFile(condor_dir + "/" + dataset + ".root", "UPDATE")
    inputFile.cd()

    #get root directory in the first layer, generally "OSUAnalysis"
    for key in inputFile.GetListOfKeys():
        if (key.GetClassName() != "TDirectoryFile"):
            continue
        rootDirectory = key.GetName()

        #cd to root directory and look for subdirectories
        inputFile.cd(rootDirectory)
        for key2 in gDirectory.GetListOfKeys():

            if (key2.GetClassName() == "TDirectoryFile"): # found a directory, cd there and look for histograms
                level2Directory = rootDirectory+"/"+key2.GetName()

                inputFile.cd(level2Directory)

                oldProfiles = []
                for key3 in gDirectory.GetListOfKeys():
                    if "_pfx" in key3.GetName() or "_pfy" in key3.GetName() or "_sigma" in key3.GetName():
                        oldProfiles.append(key3.GetName())
                for oldProfile in oldProfiles:
                    gDirectory.Delete(oldProfile+";*")

                for key3 in gDirectory.GetListOfKeys():
                    if re.match ('TH2', key3.GetClassName()): # found a 2-D histogram
                        plot = gDirectory.Get(key3.GetName())
                        title = plot.GetTitle()

                        ############################
                        ###  set up X,Y profiles ###
                        ############################

                        rawProfileX = plot.ProfileX()
                        rawProfileY = plot.ProfileY()

                        profileX = TH1D(rawProfileX.GetName(),
                                        rawProfileX.GetTitle(),
                                        plot.GetNbinsX(),
                                        plot.GetXaxis().GetBinLowEdge(1),
                                        plot.GetXaxis().GetBinUpEdge(plot.GetNbinsX())
                                        )
                        profileX.Sumw2()
                        profileX.SetDirectory(0)

                        for bin in range(1,profileX.GetNbinsX()):
                            profileX.SetBinContent(bin, rawProfileX.GetBinContent(bin))
                            profileX.SetBinError(bin,rawProfileX.GetBinError(bin))

                        profileY = TH1D(rawProfileY.GetName(),
                                        rawProfileY.GetTitle(),
                                        plot.GetNbinsY(),
                                        plot.GetYaxis().GetBinLowEdge(1),
                                        plot.GetYaxis().GetBinUpEdge(plot.GetNbinsY())
                                        )
                        profileY.Sumw2()
                        profileY.SetDirectory(0)

                        for bin in range(1,profileY.GetNbinsX()):
                            profileY.SetBinContent(bin, rawProfileY.GetBinContent(bin))
                            profileY.SetBinError(bin,rawProfileY.GetBinError(bin))

                        ###  try to set up profile titles, with some assumptions about original title

                        profileX.SetTitle("Average " + title)

                        if "vs. " in title.lower():
                            vsIndex = title.lower().find("vs. ")+4
                            vsText = title[vsIndex-5:vsIndex]
                            profileY.SetTitle("Average " + title[vsIndex:] + vsText + title[:vsIndex-4])
                        else:
                            profileY.SetTitle(title)

                        profileX.GetXaxis().SetTitle(plot.GetXaxis().GetTitle())

                        yAxisLabel = plot.GetYaxis().GetTitle()
                        if "[" in yAxisLabel:
                            yUnitIndex = yAxisLabel.find("[") - 1
                            profileX.GetYaxis().SetTitle("<" + yAxisLabel[:yUnitIndex] + ">" + yAxisLabel[yUnitIndex:])
                        else:
                            profileX.GetYaxis().SetTitle("<" + yAxisLabel + ">")
                        profileX.Write()

                        xAxisLabel = plot.GetXaxis().GetTitle()

                        profileX.GetXaxis().SetTitle(xAxisLabel)
                        profileY.GetXaxis().SetTitle(yAxisLabel)

                        if "[" in xAxisLabel:
                            xUnitIndex = xAxisLabel.find("[") - 1
                            profileY.GetYaxis().SetTitle("<" + xAxisLabel[:xUnitIndex] + ">" + xAxisLabel[xUnitIndex:])
                        else:
                            profileY.GetYaxis().SetTitle("<" + xAxisLabel + ">")
                        profileY.Write()


                        ##############################################
                        ###  also create TH1Fs of the error on X,Y ###
                        ##############################################

                        ##############################################

                        rawProfileXRMS = rawProfileX.Clone()
                        rawProfileXRMS.BuildOptions(0,0,'s')

                        rawProfileYRMS = rawProfileY.Clone()
                        rawProfileYRMS.BuildOptions(0,0,'s')

                        profileXErr = TH1D(profileX.GetName() + "_sigma",
                                          profileX.GetTitle().replace("Average ","Error in "),
                                          plot.GetNbinsX(),
                                          plot.GetXaxis().GetBinLowEdge(1),
                                          plot.GetXaxis().GetBinUpEdge(plot.GetNbinsX())
                                           )
                        profileXErr.Sumw2()
                        profileXErr.SetDirectory(0)

                        for bin in range(1,profileX.GetNbinsX()):
                            profileXErr.SetBinContent(bin, rawProfileXRMS.GetBinError(bin))
                            profileXErr.SetBinError(bin,0.0000001)

                        profileXErr.GetXaxis().SetTitle(profileX.GetXaxis().GetTitle())
                        profileXErr.GetYaxis().SetTitle(profileX.GetYaxis().GetTitle().replace("<","#sigma(").replace(">",")"))

                        profileXErr.Write()

                        ##############################################

                        profileYErr = TH1D(profileY.GetName() + "_sigma",
                                          profileY.GetTitle().replace("Average ","Error in "),
                                          plot.GetNbinsY(),
                                          plot.GetYaxis().GetBinLowEdge(1),
                                          plot.GetYaxis().GetBinUpEdge(plot.GetNbinsY())
                                           )
                        profileYErr.Sumw2()
                        profileYErr.SetDirectory(0)

                        for bin in range(1,profileY.GetNbinsX()):
                            profileYErr.SetBinContent(bin, rawProfileYRMS.GetBinError(bin))
                            profileYErr.SetBinError(bin,0.0000001)

                        profileYErr.GetXaxis().SetTitle(profileY.GetXaxis().GetTitle())
                        profileYErr.GetYaxis().SetTitle(profileY.GetYaxis().GetTitle().replace("<","#sigma(").replace(">",")"))

                        profileYErr.Write()

                        ##############################################

    inputFile.Close()


