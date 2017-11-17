#!/usr/bin/env python
import sys
import os
import re
import time
import datetime
import shutil
import functools
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

parser.add_option("-f", "--fancy", action="store_true", dest="makeFancy", default=False,
                  help="removes the title and replaces it with the official CMS plot heading")
parser.add_option("--ylog", action="store_true", dest="setLogY", default=False,
                  help="Set logarithmic scale on vertical axis on all plots")
parser.add_option("--ymin", dest="setYMin",
                  help="Minimum of y axis")
parser.add_option("--ymax", dest="setYMax",
                  help="Maximum of y axis")
parser.add_option("-E", "--ratioRelErrMax", dest="ratioRelErrMax",
                  help="maximum error used in rebinning the ratio histogram")
parser.add_option("-A", "--addOneToRatio", action='store_true', dest="addOneToRatio",
                  help="add one to the ratio so that data/MC is plotted")
parser.add_option("--SO", "--sortOrderByYields", action="store_true", dest="sortOrderByYields", default=False,
                  help="will sort the order in the stacked histogram by yields")
parser.add_option("-N", "--normalizeFactor", dest="normalizeFactor",
                  help="scale bkgd MC by a specified scale factor")
parser.add_option("-q", "--quickRun", dest="quickHistName",
                  help="only make histograms that are named quickHistName")
parser.add_option("--qr", "--quickRename", dest="quickRename",
                  help="only make histograms that are named quickHistName with a different label on x axis")
parser.add_option("-S", "--systematics", action="store_true", dest="includeSystematics", default=False,
                  help="also lists the systematic uncertainties")
parser.add_option("-s", "--signif", action="store_true", dest="makeSignificancePlots", default=False,
                  help="Make significance plots")
parser.add_option("-c", "--cumulative", action="store_true", dest="makeCumulativePlots", default=False,
                  help="Make cumulative plots")
parser.add_option("--E0", action="store_true", dest="drawErrorsOnHists", default=False,
                  help="Draw error bars on unstacked histograms")
parser.add_option("--NO", "--noOverUnderFlow", action="store_true", dest="noOverUnderFlow", default=False,
                  help="Do not add the overflow and underflow entries to the last and first bins")
parser.add_option("-v", "--verbose", action="store_true", dest="verbose", default=False,
                  help="verbose output")
parser.add_option("-P", "--paperConfig", dest="paperConfig",
                      help="paper configuration file")
parser.add_option("--pe", "--poissonErrors", action="store_true", dest="poisErr",default=False,
                      help="draw data histograms with poisson errorbars")
parser.add_option("-O", "--output-dir", dest="outputDirectory",
                  help="specify an output directory for output file, default is to use the Condor directory")
parser.add_option("--unique", action="store_true", dest="unique2D",default=False,
                  help="draw 2D plots on unique canvases with the colz option")


(arguments, args) = parser.parse_args()


if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.localConfig) + " import *")

if arguments.paperConfig:
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.paperConfig) + " import *")

# if using the paperConfig, we need to check if any histgrams will have systematics included,
# in which case we should import from that config
includeSystematics = arguments.includeSystematics
if arguments.paperConfig:
    for histo in paper_histograms:
        if 'includeSystematics' in histo:
            if histo['includeSystematics']:
                includeSystematics = True
if includeSystematics:
        sys.path.append(os.getcwd())
        exec("from " + re.sub (r".py$", r"", systematics_file) + " import *")

#### deal with conflicting arguments
if arguments.normalizeToData and arguments.normalizeToUnitArea:
    print "Conflicting normalizations requsted, will normalize to unit area"
    arguments.normalizeToData = False
if arguments.normalizeToData and arguments.noStack:
    print "You have asked to scale non-stacked backgrounds to data.  This is a very strange request.  Will normalize to unit area instead"
    arguments.normalizeToData = False
    arguments.normalizeToUnitArea = True
if arguments.makeRatioPlots and arguments.makeDiffPlots:
    print "You have requested both ratio and difference plots.  Will make just ratio plots instead"
    arguments.makeRatioPlots = False
if arguments.makeRatioPlots and arguments.noStack:
    print "You have asked to make a ratio plot and to not stack the backgrounds.  This is a very strange request.  Will skip making the ratio plot."
    arguments.makeRatioPlots = False
if arguments.makeDiffPlots and arguments.noStack:
    print "You have asked to make a difference plot and to not stack the backgrounds.  This is a very strange request.  Will skip making the difference plot."
    arguments.makeDiffPlots = False
if arguments.makeSignificancePlots and arguments.makeRatioPlots:
    print "You have asked to make a ratio plot and significance plots. This is a very strange request.  Will skip making the ratio plot."
    arguments.makeRatioPlots = False
if arguments.makeSignificancePlots and arguments.makeDiffPlots:
    print "You have asked to make a difference plot and significance plots. This is a very strange request.  Will skip making the difference plot."
    arguments.makeDiffPlots = False

from OSUT3Analysis.Configuration.histogramUtilities import ratioHistogram
from ROOT import Math, TFile, gROOT, gStyle, gDirectory, TStyle, THStack, TH1, TH1F, TCanvas, TString, TLegend, TLegendEntry, THStack, TIter, TKey, TPaveLabel, gPad, TGraphAsymmErrors


### setting ROOT options so our plots will look awesome and everyone will love us

gROOT.SetBatch()
gStyle.SetOptStat(0)
gStyle.SetCanvasBorderMode(0)
gStyle.SetPadBorderMode(0)
gStyle.SetPadColor(0)
gStyle.SetCanvasColor(0)
gStyle.SetCanvasDefH(600)
gStyle.SetCanvasDefW(600)
gStyle.SetCanvasDefX(0)
gStyle.SetCanvasDefY(0)
gStyle.SetPadTopMargin(0.056)
gStyle.SetPadBottomMargin(0.13)
gStyle.SetPadLeftMargin(0.1476)
gStyle.SetPadRightMargin(0.05)
gStyle.SetHistTopMargin(0)
gStyle.SetTitleColor(1, "XYZ")
gStyle.SetTitleFont(42, "XYZ")
gStyle.SetTitleSize(0.05, "XYZ")
gStyle.SetTitleXSize(0.04)
gStyle.SetTitleXOffset(1.25)
gStyle.SetTitleYSize(0.04)
gStyle.SetTitleYOffset(1.5)
#gStyle.SetTextFont(42)
gStyle.SetTextAlign(12)
gStyle.SetLabelColor(1, "XYZ")
gStyle.SetLabelFont(42, "XYZ")
gStyle.SetLabelOffset(0.005, "XYZ")
gStyle.SetLabelSize(0.04, "XYZ")
gStyle.SetAxisColor(1, "XYZ")
gStyle.SetStripDecimals(True)
gStyle.SetTickLength(0.03, "XYZ")
gStyle.SetNdivisions(505, "XYZ")
gStyle.SetPadTickX(1)
gStyle.SetPadTickY(1)
gROOT.ForceStyle()

#set the text for the luminosity label
if(intLumi < 1000.):
    LumiInPb = intLumi
    LumiText = str(intLumi) + " pb^{-1}"
    LumiText = str.format('{0:.1f}', LumiInPb) + " pb^{-1}"
else:
    LumiInFb = intLumi/1000.
    LumiText = str.format('{0:.1f}', LumiInFb) + " fb^{-1}"
#book keeping, put time and condor directory on plot
ts = time.time()
TimeText = "Plot Created: " + datetime.datetime.fromtimestamp(ts).strftime('%Y-%m-%d %H:%M:%S')

DirText = "Condor Dir Used: " + arguments.condorDir


#bestest place for lumi. label, in top left corner
topLeft_x_left    = 0.16129
topLeft_y_bottom  = 0.832117
topLeft_x_right   = 0.512673
topLeft_y_top     = 0.892944
topLeft_y_offset  = 0.04

#set the text for the fancy heading
#HeaderText = "CMS Preliminary: " + LumiText + " at #sqrt{s} = 8 TeV"
HeaderText = LumiText + " (13 TeV)"

#position for header
header_x_left    = 0.602535
header_y_bottom  = 0.928224
header_x_right   = 0.963134
header_y_top     = 0.980535

#position for timestamp
ts_x_left   = 0.00
ts_y_bottom = 0.04
ts_x_right  = 0.35
ts_y_top    = 0.08

#position for condor dir, beneath the timestamp
dir_x_left   = ts_x_left
dir_y_bottom = 0.0
dir_x_right  = ts_x_right
dir_y_top    = ts_y_bottom


##########################################################################################################################################

##########################################################################################################################################
##########################################################################################################################################
##########################################################################################################################################

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

    # add sample-specific uncertainties
    for uncertainty in unique_systematic_uncertainties:
        if sample is unique_systematic_uncertainties[uncertainty]['dataset']:
            error = float(unique_systematic_uncertainties[uncertainty]['value']) -1
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

##########################################################################################################################################
##########################################################################################################################################
##########################################################################################################################################

def addSystematicError(histogram, fractionalSysError):

    for bin in range(histogram.GetNbinsX()):
        binContent = histogram.GetBinContent(bin)
        statError = histogram.GetBinError(bin)
        sysError = fractionalSysError * binContent
        totalError = math.sqrt(statError * statError + sysError * sysError)
        histogram.SetBinError(bin,totalError)

    return histogram

##########################################################################################################################################
##########################################################################################################################################
##########################################################################################################################################

def signifHistograms(BgSum,SignalMCHistograms):
    # Return a list of histograms that are the significance
    signifHists = []
    for sigHist in SignalMCHistograms:
        signifHist = sigHist.Clone()
        for i in range(0,signifHist.GetNbinsX()+1):
            x = sigHist.GetBinContent(i)
            y =   BgSum.GetBinContent(i)
            dx = sigHist.GetBinError(i)
            dy =   BgSum.GetBinError(i)
            if y > 0:
                signif = x / math.sqrt(x + y)
            else:
                signif = 0
            signifHist.SetBinContent(i, signif)
            # Error calculation is same as in AnaTools/bin/cutFlowTable.cpp
            binError = 4.0 * y * y * dx * dx + 4 * x * y * dx * dx + x * x * dy * dy + x * x * dx * dx
            if x + y > 0:   # protect against division by 0
                binError /= 4.0 * (x + y) * (x + y) * (x + y)
                binError = sqrt (binError)
            else:
                binError = 0
            signifHist.SetBinError(i, binError)
        signifHists.append(signifHist)
    return signifHists

##########################################################################################################################################
##########################################################################################################################################
##########################################################################################################################################

def MakeIntegralHist(hist, integrateDir):
    # return the integrated histogram, in the direction specified
    # integrateDir values: "left", "right", "none"
    if integrateDir is "none":
        return hist  # do nothing
    integral = 0
    error = 0
    nbins = hist.GetNbinsX()
    lowLimit = 0 # underflow bin
    uppLimit = nbins+1 # overflow bin
    # exclude under/overflow if it's turned off
    if arguments.noOverUnderFlow:
        lowLimit += 1
        uppLimit -= 1
    if integrateDir is "left":
        for i in range(lowLimit, nbins+1):
            integral += hist.GetBinContent(i)
            error = math.sqrt(error*error + hist.GetBinError(i)*hist.GetBinError(i)) # sum errors in quadrature
            hist.SetBinContent(i, integral)
            hist.SetBinError  (i, error)
        if not arguments.noOverUnderFlow:
            # Then include overflow bin in the last bin
            hist.SetBinContent(nbins, hist.GetBinContent(nbins) + hist.GetBinContent(nbins+1))
            hist.SetBinError  (nbins, math.sqrt(hist.GetBinError(nbins)*hist.GetBinError(nbins) + hist.GetBinError(nbins+1)*hist.GetBinError(nbins+1)))
    elif integrateDir is "right":
        for i in xrange(uppLimit, 0, -1):
            integral += hist.GetBinContent(i)
            error = math.sqrt(error*error + hist.GetBinError(i)*hist.GetBinError(i)) # sum errors in quadrature
            hist.SetBinContent(i, integral)
            hist.SetBinError  (i, error)
        if not arguments.noOverUnderFlow:
            # Then include underflow bin in the first bin
            hist.SetBinContent(1, hist.GetBinContent(1) + hist.GetBinContent(0))
            hist.SetBinError  (1, math.sqrt(hist.GetBinError(1)*hist.GetBinError(1) + hist.GetBinError(0)*hist.GetBinError(0)))
    return hist



##########################################################################################################################################
##########################################################################################################################################
##########################################################################################################################################
def YaxisTitleForVariableBinHist(histogram):
    variableBin = {}
    binWidth = []
    variableBin['isVariable'] = False
    for i in range(1,histogram.GetNbinsX() + 1):
        binWidth.append(histogram.GetXaxis().GetBinWidth(i))
    binWidth.sort()
    if binWidth[0] < binWidth[-1]:
        variableBin['isVariable'] = True
    variableBin['smallestBinWidth'] =  binWidth[0]
    return variableBin

def sortedDictValues(dic):
    keys = dic.keys()
    keys.sort()
    return [dic[key] for key in keys]

def setXAxisRangeUser(hist):
    if arguments.paperConfig and ('setXMin' in paperHistogram or 'setXMax' in paperHistogram):
        xAxisMin = hist.GetXaxis().GetBinLowEdge(1)
        xAxisMax = hist.GetXaxis().GetBinLowEdge(1+hist.GetXaxis().GetNbins())
        if 'setXMin' in paperHistogram:
            xAxisMin = float(paperHistogram['setXMin'])
        if 'setXMax' in paperHistogram:
            xAxisMax = float(paperHistogram['setXMax'])
        hist.GetXaxis().SetRangeUser(xAxisMin, xAxisMax)



def MakeOneDHist(pathToDir,histogramName,integrateDir):

    global processed_datasets
    global ts_x_left, ts_y_bottom, ts_x_right, ts_y_top
    global dir_x_left, dir_y_bottom, dir_x_right, dir_y_top

    # let's just assume the root directory is "OSUAnalysis", because, come on, of course it is
    #channel = pathToDir.lstrip(rootDirectory).lstrip('/')
    channel = pathToDir.lstrip("OSUAnalysis").lstrip('/')

    blindData = False
    # To blind histograms, define a list of histsToBlind in the localOptions.py file
    try:  # Use "try" in case histsToBlind does not exist
        if histsToBlind:
            for histToBlind in histsToBlind:
                if histToBlind in histogramName:
                    print "Blinding data for histogram " + histogramName
                    blindData = True
    except NameError:
        time.sleep(0.000001) # Do nothing if histsToBlind does not exist



    backgroundIntegral = 0
    dataIntegral = 0
    scaleFactor = 1

    numBgMCSamples = 0
    numDataSamples = 0
    numSignalSamples = 0


    # we'll define all the options for each plot here,
    # in case the user is using a "paperConfig" with specific options for each plot



    if arguments.paperConfig:
        # use the user-defined list of datasets if there is one
        if 'datasets' in paperHistogram:
            processed_datasets = paperHistogram['datasets']
        # otherwise, use the default set from the supplied localOptions.py file
        else:
            processed_datasets = datasets

    ###############################################
    ###############################################
    ###############################################
    ###############################################
    ###############################################

    if arguments.normalizeFactor:
        scaleFactor = float(arguments.normalizeFactor)
    if arguments.paperConfig:
        if 'normalizeFactor' in paperHistogram:
            scaleFactor = paperHistogram['normalizeFactor']
    ###############################################
    noStack = arguments.noStack
    if arguments.paperConfig:
        if 'noStack' in paperHistogram:
            noStack = paperHistogram['noStack']
    ###############################################
    normalizeToUnitArea = arguments.normalizeToUnitArea
    if arguments.paperConfig:
        if 'normalizeToUnitArea' in paperHistogram:
            normalizeToUnitArea = paperHistogram['normalizeToUnitArea']
    ###############################################
    normalizeToData = arguments.normalizeToData
    if arguments.paperConfig:
        if 'normalizeToData' in paperHistogram:
            normalizeToData = paperHistogram['normalizeToData']
    ###############################################
    makeRatioPlots = arguments.makeRatioPlots
    if arguments.paperConfig:
        if 'makeRatioPlots' in paperHistogram:
            makeRatioPlots = paperHistogram['makeRatioPlots']
    ###############################################
    makeDiffPlots = arguments.makeDiffPlots
    if arguments.paperConfig:
        if 'makeDiffPlots' in paperHistogram:
            makeDiffPlots = paperHistogram['makeDiffPlots']
    ###############################################
    printYields = arguments.printYields
    if arguments.paperConfig:
        if 'printYields' in paperHistogram:
            printYields = paperHistogram['printYields']
    ###############################################
    setLogY = arguments.setLogY
    if arguments.paperConfig:
        if 'setLogY' in paperHistogram:
            setLogY = paperHistogram['setLogY']
    ###############################################
    includeSystematics = arguments.includeSystematics
    if arguments.paperConfig:
        if 'includeSystematics' in paperHistogram:
            includeSystematics = paperHistogram['includeSystematics']
    ###############################################
    noOverUnderFlow = arguments.noOverUnderFlow
    if arguments.paperConfig:
        if 'noOverUnderFlow' in paperHistogram:
            noOverUnderFlow = paperHistogram['noOverUnderFlow']
    ###############################################
    sortOrderByYields = arguments.sortOrderByYields
    if arguments.paperConfig:
        if 'sortOrderByYields' in paperHistogram:
            sortOrderByYields = paperHistogram['sortOrderByYields']
    ###############################################
    makeFancy = arguments.makeFancy
    if arguments.paperConfig:
        if 'makeFancy' in paperHistogram:
            makeFancy = paperHistogram['makeFancy']
    ###############################################
    ratioRelErrMax = -1
    if arguments.ratioRelErrMax:
        ratioRelErrMax = arguments.ratioRelErrMax
    if arguments.paperConfig:
        if 'ratioRelErrMax' in paperHistogram:
            ratioRelErrMax = paperHistogram['ratioRelErrMax']
    ###############################################
    addOneToRatio = -1
    if arguments.addOneToRatio:
        addOneToRatio = arguments.addOneToRatio
    if arguments.paperConfig:
        if 'addOneToRatio' in paperHistogram:
            addOneToRatio = paperHistogram['addOneToRatio']
    ###############################################
    doRebin = False
    if arguments.rebinFactor:
        doRebin = True
        rebinFactor = arguments.rebinFactor
    if arguments.paperConfig:
        if 'rebinFactor' in paperHistogram:
            doRebin = True
            rebinFactor = paperHistogram['rebinFactor']
    ###############################################
    renamePaperHistX = False
    renamePaperHistY = False
    if arguments.quickRename:
        quickRenameString = arguments.quickRename
    if arguments.paperConfig:
        if 'quickRenameX' in paperHistogram:
            renamePaperHistX = True
            quickRenameStringX = paperHistogram['quickRenameX']
        if 'quickRenameY' in paperHistogram:
            renamePaperHistY = True
            quickRenameStringY = paperHistogram['quickRenameY']
    ####################################################
    makePoisErrHist = False
    if arguments.paperConfig:
        if 'poisErr' in paperHistogram:
            makePoisErrHist = True

    Stack = THStack("stack",histogramName)

    HeaderLabel = TPaveLabel(header_x_left,header_y_bottom,header_x_right,header_y_top,HeaderText,"NDC")
    HeaderLabel.SetTextAlign(32)
    HeaderLabel.SetTextFont(42)
    HeaderLabel.SetTextSize(0.697674)
    HeaderLabel.SetBorderSize(0)
    HeaderLabel.SetFillColor(0)
    HeaderLabel.SetFillStyle(0)

    CMSLabel = TPaveLabel(header_x_left,header_y_bottom,header_x_right,header_y_top,HeaderText,"NDC")
    CMSLabel.SetTextAlign(32)
    CMSLabel.SetTextFont(42)
    CMSLabel.SetTextSize(0.697674)
    CMSLabel.SetBorderSize(0)
    CMSLabel.SetFillColor(0)
    CMSLabel.SetFillStyle(0)

    if makeFancy:
        LumiLabel = TPaveLabel(topLeft_x_left,topLeft_y_bottom,topLeft_x_right,topLeft_y_top,"CMS Preliminary","NDC")
        LumiLabel.SetTextFont(62)
        LumiLabel.SetTextSize(0.8)
        LumiLabel.SetTextAlign(12)
    else:
        LumiLabel = TPaveLabel(topLeft_x_left,topLeft_y_bottom,topLeft_x_right,topLeft_y_top,LumiText,"NDC")
        LumiLabel.SetTextAlign(32)
        LumiLabel.SetTextFont(42)

        if makeRatioPlots:
            ts_y_bottom  = 0.96
            ts_y_top     = 1.0
            dir_y_top    = ts_y_bottom
            dir_y_bottom = 0.92

        TimeLabel = TPaveLabel(ts_x_left,ts_y_bottom,ts_x_right, ts_y_top,TimeText,"NDC")
        TimeLabel.SetTextAlign(12)  # 12 = 10 (left-justify horizontal) + 2 (center-justify vertical)
        TimeLabel.SetTextSize(0.55)
        TimeLabel.SetBorderSize(0)
        TimeLabel.SetFillColor(0)
        TimeLabel.SetFillStyle(0)

        DirLabel = TPaveLabel(dir_x_left,dir_y_bottom,dir_x_right, dir_y_top,DirText,"NDC")
        DirLabel.SetTextAlign(12)
        DirLabel.SetTextSize(0.55)
        DirLabel.SetBorderSize(0)
        DirLabel.SetFillColor(0)
        DirLabel.SetFillStyle(0)


    LumiLabel.SetBorderSize(0)
    LumiLabel.SetFillColor(0)
    LumiLabel.SetFillStyle(0)



    BgMCLegend = TLegend()
##     BgTitle = BgMCLegend.AddEntry(0, "Data & Bkgd. MC", "H")
##     BgTitle.SetTextAlign(22)
##     BgTitle.SetTextFont(62)
    BgMCLegend.SetBorderSize(0)
    BgMCLegend.SetFillColor(0)
    BgMCLegend.SetFillStyle(0)
    SignalMCLegend = TLegend()
#    SignalTitle = SignalMCLegend.AddEntry(0, "Signal MC", "H")
#    SignalTitle.SetTextAlign(22)
#    SignalTitle.SetTextFont(62)
    SignalMCLegend.SetBorderSize(0)
    SignalMCLegend.SetFillColor(0)
    SignalMCLegend.SetFillStyle(0)

    if not arguments.paperConfig:
        outputFile.cd(pathToDir)
    canvasName = histogramName
    if integrateDir is "left":
        canvasName += "_CumulativeLeft"
    if integrateDir is "right":
        canvasName += "_CumulativeRight"
    Canvas = TCanvas(canvasName,"",4,55,872,850)
    Canvas.SetHighLightColor(2)
    Canvas.Range(-72.16495,-10.50091,516.9367,82.84142)
    Canvas.SetFillColor(0)
    Canvas.SetBorderMode(0)
    Canvas.SetBorderSize(2)
    Canvas.SetTickx(1)
    Canvas.SetTicky(1)
    Canvas.SetLeftMargin(0.1225)
    Canvas.SetRightMargin(0.0357143)
    Canvas.SetTopMargin(0.0725)
    Canvas.SetBottomMargin(0.1125)
    Canvas.SetFrameBorderMode(0)
    Canvas.SetFrameBorderMode(0)
    BgMCHistograms = []
    BgMCUncertainties = []
    BgMCLegendEntries = []
    BgMCLegendLabelYieldsDic = {}
    SignalMCHistograms = []
    SignalMCLegendEntries = []
    DataHistograms = []
    DataLegendEntries = []
    BgMCHistYieldsDic = {}

    if arguments.verbose:
        print pathToDir+"/"+histogramName

    for sample in processed_datasets: # loop over different samples as listed in configurationOptions.py
        dataset_file = "%s/%s.root" % (condor_dir,sample)
        condorDir = condor_dir
        inputFile = TFile(dataset_file)
        HistogramObj = inputFile.Get(pathToDir+"/"+histogramName)
        if not HistogramObj:
            print "WARNING:  Could not find histogram " + pathToDir + "/" + histogramName + " in file " + dataset_file + ".  Will skip it and continue."
            continue
        Histogram = HistogramObj.Clone()
        Histogram.SetDirectory(0)

        inputFile.Close()

        # correct bin contents of object multiplcity plots
        if Histogram.GetName().startswith("num") and "PV" not in Histogram.GetName():
            # include overflow bin
            for bin in range(2,Histogram.GetNbinsX()+2):
                content = Histogram.GetBinContent(bin)
                Histogram.SetBinContent(bin, content/float(bin-1))

        isProfile = False
        if "_pfx" in Histogram.GetName() or "_pfy" in Histogram.GetName() or "_sigma" in Histogram.GetName():
            isProfile = True


        if doRebin and "CutFlowPlotter" not in pathToDir:
#            #don't rebin any gen-matching or cutflow histograms, or numObject type histograms
#            if (Histogram.GetName().find("num") is -1 and
#                Histogram.GetName().find("Primaryvertexs") is -1 and
#                Histogram.GetName().find("status3OutgoingID")  is -1 and
#                Histogram.GetName().find("Charge")  is -1 and
#                Histogram.GetName().find("GenMatch") is -1):
            Histogram.Rebin(int(rebinFactor))


        xAxisLabel = Histogram.GetXaxis().GetTitle()

        if arguments.quickRename:
            xAxisLabel = Histogram.GetXaxis().SetTitle(arguments.quickRename)
            xAxisLabel = Histogram.GetXaxis().GetTitle()
        if renamePaperHistX:
                 xAxisLabel = Histogram.GetXaxis().SetTitle(quickRenameStringX)
                 xAxisLabel = Histogram.GetXaxis().GetTitle()


        unitBeginIndex = xAxisLabel.find("[")
        unitEndIndex = xAxisLabel.find("]")
        xAxisLabelVar = xAxisLabel
        variableBinYaxisSet = YaxisTitleForVariableBinHist(Histogram)

        if unitBeginIndex is not -1 and unitEndIndex is not -1: #x axis has a unit
            if variableBinYaxisSet['isVariable']:
                    yAxisLabel = "Entries / (Width_{Bin}/" + str(variableBinYaxisSet['smallestBinWidth']) + " " + xAxisLabel[unitBeginIndex+1:unitEndIndex] + ")"
            else:
                    yAxisLabel = "Entries / " + str(Histogram.GetXaxis().GetBinWidth(1)) + " " + xAxisLabel[unitBeginIndex+1:unitEndIndex]
            xAxisLabelVar = xAxisLabel[0:unitBeginIndex]
        elif variableBinYaxisSet['isVariable']:
            yAxisLabel = "Entries per bin (Width_{Bin}/" + str(variableBinYaxisSet['smallestBinWidth']) + ")"
        else:
            yAxisLabel = "Entries per bin (" + str(Histogram.GetXaxis().GetBinWidth(1)) + " width)"

        if normalizeToUnitArea:
            yAxisLabel = yAxisLabel + " (Unit Area Norm.)"
        if normalizeToData:
            yAxisLabel = yAxisLabel + " (Bkgd. Scaled to Data)"
        if scaleFactor is not 1:
            yAxisLabel = yAxisLabel + " (Bkgd. Scaled by " + str(scaleFactor) + ")"

        if renamePaperHistY:
                 yAxisLabel = Histogram.GetYaxis().SetTitle(quickRenameStringY)
                 yAxisLabel = Histogram.GetYaxis().GetTitle()

        if normalizeToUnitArea and (arguments.makeSignificancePlots or arguments.makeCumulativePlots):
            unit = "Efficiency"
        else:
            unit = "Yield"
        if integrateDir is "left":
            yAxisLabel = unit + ", " + xAxisLabelVar + "< x (" + str(Histogram.GetXaxis().GetBinWidth(1)) + " bin width)"
        if integrateDir is "right":
            yAxisLabel = unit + ", " + xAxisLabelVar + "> x (" + str(Histogram.GetXaxis().GetBinWidth(1)) + " bin width)"

        if isProfile:
            yAxisLabel = Histogram.GetYaxis().GetTitle()

        if not makeFancy:
            histoTitle = Histogram.GetTitle()
        else:
            histoTitle = ""


        legLabel = labels[sample]
        if (printYields):
            yieldHist = Histogram.Integral()
            legLabel = legLabel + " (%.1f)" % yieldHist

        if not noOverUnderFlow:
            nbins = Histogram.GetNbinsX()
            Histogram.SetBinContent(1,     Histogram.GetBinContent(1)     + Histogram.GetBinContent(0))       # Add underflow
            Histogram.SetBinContent(nbins, Histogram.GetBinContent(nbins) + Histogram.GetBinContent(nbins+1)) # Add overflow
            # Set the errors to be the sum in quadrature
            Histogram.SetBinError(1,     math.sqrt(math.pow(Histogram.GetBinError(1),    2) + math.pow(Histogram.GetBinError(0),      2)))
            Histogram.SetBinError(nbins, math.sqrt(math.pow(Histogram.GetBinError(nbins),2) + math.pow(Histogram.GetBinError(nbins+1),2)))

        if arguments.verbose:
            print "Sample = " + sample + ", types[sample] = " + types[sample]
        if( types[sample] == "bgMC"):

            numBgMCSamples += 1
            backgroundIntegral += Histogram.Integral()

            Histogram.SetLineStyle(1)
            if noStack or isProfile:
                Histogram.SetFillStyle(0)
                Histogram.SetLineColor(colors[sample])
                Histogram.SetLineWidth(2)
            else:
                Histogram.SetFillStyle(1001)
                Histogram.SetFillColor(colors[sample])
                Histogram.SetLineColor(1)
                Histogram.SetLineWidth(1)
            if not sortOrderByYields or isProfile:
                BgMCHistograms.append(Histogram)
                BgMCLegendEntries.append(legLabel)

            # put histograms in a dictionary according to their yields
            if Histogram.Integral() > 0:
                BgMCLegendLabelYieldsDic[Histogram.Integral()] = legLabel
                BgMCHistYieldsDic[Histogram.Integral()] = Histogram
            # for empty histograms, put them as having negative yields (so multiple ones don't overwrite each other in the dictionary)
            else:
                BgMCLegendLabelYieldsDic[-1*numBgMCSamples] = legLabel
                BgMCHistYieldsDic[-1*numBgMCSamples] = Histogram

            if includeSystematics:
                BgMCUncertainties.append(getSystematicError(sample))

        elif( types[sample] == "signalMC"):

            numSignalSamples += 1

            Histogram.SetFillStyle(0)
            Histogram.SetLineColor(colors[sample])
            if (arguments.paperConfig and 'SigLineStyle' in paperHistogram):
                Histogram.SetLineStyle(paperHistogram['SigLineStyle'])
            else:
                Histogram.SetLineStyle(1)
            if (arguments.paperConfig and 'SigLineWidth' in paperHistogram):
                Histogram.SetLineWidth(paperHistogram['SigLineWidth'])
            else:
                Histogram.SetLineWidth(3)
            if(normalizeToUnitArea and Histogram.Integral() > 0):
                Histogram.Scale(1./Histogram.Integral())

            Histogram = MakeIntegralHist(Histogram, integrateDir)

            SignalMCLegendEntries.append(legLabel)
            SignalMCHistograms.append(Histogram)

        elif( types[sample] == "data" and not blindData):

            numDataSamples += 1
            dataIntegral += Histogram.Integral()

            Histogram.SetMarkerStyle(20)
            Histogram.SetMarkerSize(1.5)
            Histogram.SetFillStyle(0)
            Histogram.SetLineColor(colors[sample])
            Histogram.SetLineStyle(1)
            Histogram.SetLineWidth(2)
            if(normalizeToUnitArea and Histogram.Integral() > 0):
                Histogram.Scale(1./Histogram.Integral())

            Histogram = MakeIntegralHist(Histogram, integrateDir)
            if arguments.poisErr or makePoisErrHist == True:
                xBins = array ('d', [])
                for i in range(1,Histogram.GetNbinsX()+2):  # Want n+1 bin boundaries for n bins
                    xBins.append (Histogram.GetBinLowEdge (i))
                newDataHist  = TH1F(Histogram.GetName(), Histogram.GetTitle(), Histogram.GetNbinsX(), xBins)
                for i in range(1,Histogram.GetNbinsX()+1):
                    newDataHist.SetBinContent(i,Histogram.GetBinContent(i))
                newDataHist.SetBinErrorOption(TH1.kPoisson)
                newDataHist.SetMarkerColor(colors[sample])
                newDataHist.SetMarkerStyle(20)
                newDataHist.SetMarkerSize(1.5)
                newDataHist.SetLineColor(colors[sample])
                newDataHist.SetLineWidth(2)
                Histogram = newDataHist

            DataLegendEntries.append(legLabel)

            DataHistograms.append(Histogram)

    if sortOrderByYields and not isProfile:
        BgMCHistYieldsDic = sortedDictValues(BgMCHistYieldsDic)
        BgMCLegendLabelYieldsDic = sortedDictValues(BgMCLegendLabelYieldsDic)
        for hist in BgMCHistYieldsDic:
            BgMCHistograms.append(hist)
        for legend in BgMCLegendLabelYieldsDic:
            BgMCLegendEntries.append(legend)

    #scaling histograms as per user's specifications
    for bgMCHist in BgMCHistograms:

        if normalizeToData and dataIntegral > 0 and backgroundIntegral > 0:
            scaleFactor = dataIntegral/backgroundIntegral

        bgMCHist.Scale(scaleFactor)

        if normalizeToUnitArea and not noStack and backgroundIntegral > 0:
            bgMCHist.Scale(1./backgroundIntegral)
        elif normalizeToUnitArea and noStack and bgMCHist.Integral() > 0:
            bgMCHist.Scale(1./bgMCHist.Integral())

        bgMCHist = MakeIntegralHist(bgMCHist, integrateDir)

        if not noStack and not isProfile:
            Stack.Add(bgMCHist)


    ### formatting data histograms and adding to legend
    legendIndex = 0
    for Histogram in DataHistograms:
        BgMCLegend.AddEntry(Histogram,DataLegendEntries[legendIndex],"PE")
        legendIndex = legendIndex+1


    ### creating the histogram to represent the statistical errors on the stack
    if numBgMCSamples is not 0 and not noStack and not isProfile:
        if includeSystematics:
            addSystematicError(BgMCHistograms[0],BgMCUncertainties[0])
        ErrorHisto = BgMCHistograms[0].Clone("errors")
        ErrorHisto.SetFillStyle(3002)
        ErrorHisto.SetFillColor(13)
        ErrorHisto.SetLineWidth(0)
        for index in range(1,len(BgMCHistograms)):
            if includeSystematics:
                addSystematicError(BgMCHistograms[index],BgMCUncertainties[index])
            ErrorHisto.Add(BgMCHistograms[index])

        if includeSystematics:
            BgMCLegend.AddEntry(ErrorHisto,"stat. & syst. errors","F")
        else:
            BgMCLegend.AddEntry(ErrorHisto,"stat. errors","F")


    ### formatting bgMC histograms and adding to legend
    legendIndex = numBgMCSamples-1
    for Histogram in reversed(BgMCHistograms):
        if isProfile:
            BgMCLegend.AddEntry(Histogram,BgMCLegendEntries[legendIndex],"LEP")
        elif noStack:
            BgMCLegend.AddEntry(Histogram,BgMCLegendEntries[legendIndex],"L")
        else:
            BgMCLegend.AddEntry(Histogram,BgMCLegendEntries[legendIndex],"F")
        legendIndex = legendIndex-1


    ### formatting signalMC histograms and adding to legend
    legendIndex = 0
    for Histogram in SignalMCHistograms:
        #if legend entry is really long, split it into 2 lines (first line with the hist line/marker, second line just text)
        #this is better than using #splitline{}{} because with that method, the hist line/marker gets misaligned
        #Split location (here char 115) will need to be changed for each long entry. 
        if(len(SignalMCLegendEntries[legendIndex])>100):
            #print "first 115 char are: "+str(SignalMCLegendEntries[legendIndex][:115]) 
            firstLineEntry = str(SignalMCLegendEntries[legendIndex][:115])
            secondLineEntry = str(SignalMCLegendEntries[legendIndex][115:])
            SignalMCLegend.AddEntry(Histogram,firstLineEntry,"L")
            SignalMCLegend.AddEntry(0,secondLineEntry,"")
        else:
            SignalMCLegend.AddEntry(Histogram,SignalMCLegendEntries[legendIndex],"L")
        legendIndex = legendIndex+1

    ### finding the maximum value of anything going on the canvas, so we know how to set the y-axis
    finalMax = 0
    if numBgMCSamples is not 0 and not noStack and not isProfile:
        finalMax = ErrorHisto.GetMaximum() + ErrorHisto.GetBinError(ErrorHisto.GetMaximumBin())
    else:
        for bgMCHist in BgMCHistograms:
            if(bgMCHist.GetMaximum() > finalMax):
                finalMax = bgMCHist.GetMaximum()
    for signalMCHist in SignalMCHistograms:
        if(signalMCHist.GetMaximum() > finalMax):
            finalMax = signalMCHist.GetMaximum()
    for dataHist in DataHistograms:
        if(dataHist.GetMaximum() + dataHist.GetBinError(dataHist.GetMaximumBin()) > finalMax):
            finalMax = dataHist.GetMaximum() + dataHist.GetBinError(dataHist.GetMaximumBin())

    finalMax = 1.15*finalMax
    if finalMax <= 0: # if it's an empty canvas, set ymax > ymin to avoid an error
        finalMax = 1.0

    ### aaaaaand overwrite all that work we just did if the user wants us to
    yAxisMin = 1.0e-4
    if arguments.setYMin:
        yAxisMin = float(arguments.setYMin)
    if arguments.setYMax:
        finalMax = float(arguments.setYMax)

    if arguments.paperConfig:
        if 'setYMin' in paperHistogram:
            yAxisMin = float(paperHistogram['setYMin'])
        if 'setYMax' in paperHistogram:
            finalMax = float(paperHistogram['setYMax'])

    ### Drawing histograms to canvas

    if not arguments.paperConfig:
        outputFile.cd(pathToDir)

    makeSignifPlots = arguments.makeSignificancePlots

    if setLogY:
        gPad.SetLogy()

    if numBgMCSamples is 0 or numDataSamples is not 1 or isProfile:
        makeRatioPlots = False
        makeDiffPlots = False
    if makeSignifPlots and (numBgMCSamples is 0 or numSignalSamples is 0):
        print "Error:  you have requested to make significance plots, but you are missing either signal or background samples.  Will skip making the significance plots."
        print "numBgMCSamples = " + str(numBgMCSamples) + "; numSignalSamples = " + str(numSignalSamples)
        makeSignifPlots = False
    if makeRatioPlots or makeDiffPlots or makeSignifPlots:
        Canvas.SetFillStyle(0)
        Canvas.Divide(1,2)
        Canvas.cd(1)
        gPad.SetPad(0,0.25,1,1)
        gPad.SetMargin(0.15,0.05,0.01,0.07)
        gPad.SetFillStyle(0)
        # we need to reset this in the case of dividing the canvas into two parts
        if setLogY:
            gPad.SetLogy()
        gPad.Update()
        gPad.Draw()
        Canvas.cd(2)
        gPad.SetPad(0,0,1,0.25)
        #format: gPad.SetMargin(l,r,b,t)
        gPad.SetMargin(0.15,0.05,0.4,0.01)
        gPad.SetFillStyle(0)
        gPad.SetGridy(1)
        gPad.Update()
        gPad.Draw()

        Canvas.cd(1)

    if numBgMCSamples is not 0: # the first thing to draw to the canvas is a bgMC sample

        if not noStack and not isProfile: # draw stacked background samples
            Stack.SetTitle(histoTitle)
            Stack.Draw("HIST")
            Stack.SetMaximum(finalMax)
            Stack.SetMinimum(yAxisMin)
            setXAxisRangeUser(Stack)
            Stack.Draw("HIST")
            Stack.GetXaxis().SetMoreLogLabels()
            Stack.GetXaxis().SetTitle(xAxisLabel)
            Stack.GetYaxis().SetTitle(yAxisLabel)
            if makeRatioPlots or makeDiffPlots or makeSignifPlots:
                Stack.GetHistogram().GetXaxis().SetLabelSize(0)
            #redraw so the changes take effect
            Stack.Draw("HIST")
            gPad.Update()
            gPad.Modified()
            gPad.RedrawAxis()
            #draw shaded error bands
            ErrorHisto.Draw("A E2 SAME")

        else: #draw the unstacked backgrounds
            BgMCHistograms[0].SetTitle(histoTitle)
            if not isProfile and not arguments.drawErrorsOnHists:
                BgMCHistograms[0].Draw("HIST")
            else:
                BgMCHistograms[0].Draw("E0")
            BgMCHistograms[0].GetXaxis().SetTitle(xAxisLabel)
            BgMCHistograms[0].GetYaxis().SetTitle(yAxisLabel)
            BgMCHistograms[0].SetMaximum(finalMax)
            if not isProfile:
                BgMCHistograms[0].SetMinimum(yAxisMin)
            setXAxisRangeUser(BgMCHistograms[0])
            for bgMCHist in BgMCHistograms[1:]:
                if not isProfile and not arguments.drawErrorsOnHists:
                    bgMCHist.Draw("A HIST SAME")
                else:
                    bgMCHist.Draw("A E0 SAME")

        for signalMCHist in SignalMCHistograms:
            if not isProfile and not arguments.drawErrorsOnHists:
                signalMCHist.Draw("A HIST SAME")
            else:
                signalMCHist.Draw("A E0 SAME")
        for dataHist in DataHistograms:
            dataHist.Draw("A E X0 SAME")

    elif numSignalSamples is not 0: # the first thing to draw to the canvas is a signalMC sample
        SignalMCHistograms[0].SetTitle(histoTitle)
        if not isProfile and not arguments.drawErrorsOnHists:
            SignalMCHistograms[0].Draw("HIST")
        else:
            SignalMCHistograms[0].Draw("E0")
        SignalMCHistograms[0].GetXaxis().SetTitle(xAxisLabel)
        SignalMCHistograms[0].GetYaxis().SetTitle(yAxisLabel)
        SignalMCHistograms[0].SetMaximum(finalMax)
        SignalMCHistograms[0].SetMinimum(yAxisMin)
        setXAxisRangeUser(SignalMCHistograms[0])
        for signalMCHist in SignalMCHistograms:
            if(signalMCHist is not SignalMCHistograms[0]):
                if not isProfile and not arguments.drawErrorsOnHists:
                    signalMCHist.Draw("A HIST SAME")
                else:
                    signalMCHist.Draw("A E0 SAME")
        for dataHist in DataHistograms:
            dataHist.Draw("A E X0 SAME")



    elif(numDataSamples is not 0): # the first thing to draw to the canvas is a data sample
        DataHistograms[0].SetTitle(histoTitle)
        DataHistograms[0].Draw("E")
        DataHistograms[0].GetXaxis().SetTitle(xAxisLabel)
        DataHistograms[0].GetYaxis().SetTitle(yAxisLabel)
        if not isProfile:
            DataHistograms[0].SetMaximum(finalMax)
            DataHistograms[0].SetMinimum(yAxisMin)
        setXAxisRangeUser(DataHistograms[0])
        for dataHist in DataHistograms:
            if(dataHist is not DataHistograms[0]):
                dataHist.Draw("A E X0 SAME")



    #legend coordinates, empirically determined :-)
    x_left = 0.52765
    x_right = 0.834101
    x_width = x_right - x_left
    y_max = 0.879562
    entry_height = 0.037307

    if(numBgMCSamples is not 0 or numDataSamples is not 0): #then draw the data & bgMC legend


        numExtraEntries = 0 # count the legend title
        if (arguments.paperConfig and 'BkgdLegX1' in paperHistogram):
            BgMCLegend.SetX1NDC(float(paperHistogram['BkgdLegX1']))
        else:
            BgMCLegend.SetX1NDC(x_left)

        if numBgMCSamples > 0:
            numExtraEntries = numExtraEntries + 1 # count the stat. errors entry
        if (arguments.paperConfig and 'BkgdLegX1' in paperHistogram):
#            BgMCLegend.SetY1NDC(float(paperHistogram['BkgdLegY1'])-entry_height*(numExtraEntries+numBgMCSamples+numDataSamples))
            BgMCLegend.SetY1NDC(float(paperHistogram['BkgdLegY1']))
            BgMCLegend.SetX2NDC(float(paperHistogram['BkgdLegX2']))
            BgMCLegend.SetY2NDC(float(paperHistogram['BkgdLegY2']))
        else:
            BgMCLegend.SetY1NDC(y_max-entry_height*(numExtraEntries+numBgMCSamples+numDataSamples))
            BgMCLegend.SetX2NDC(x_right)
            BgMCLegend.SetY2NDC(y_max)
            BgMCLegend.SetTextSize(0.0364078)
        BgMCLegend.Draw()

    if (numSignalSamples is not 0 and arguments.paperConfig and 'SigLegX1' in paperHistogram): #then draw the signalMC legend to the left of the other one
        SignalMCLegend.SetX1NDC(float(paperHistogram['SigLegX1']))
        SignalMCLegend.SetY1NDC(float(paperHistogram['SigLegY1'])) # add one for the title
        SignalMCLegend.SetX2NDC(float(paperHistogram['SigLegX2']))
        SignalMCLegend.SetY2NDC(float(paperHistogram['SigLegY2']))
        if arguments.paperConfig and 'SigTextSize' in paperHistogram:
            SignalMCLegend.SetTextSize(paperHistogram['SigTextSize'])
        SignalMCLegend.Draw()
    elif (numSignalSamples is not 0):
        SignalMCLegend.SetX1NDC(0.157471)
        SignalMCLegend.SetY1NDC(0.821602-entry_height*numSignalSamples) # add one for the title
        SignalMCLegend.SetX2NDC(0.355172)
        SignalMCLegend.SetY2NDC(0.821602)
        SignalMCLegend.SetTextSize(0.0364078)
        SignalMCLegend.Draw()

    if integrateDir is "left":
        # Move the legend to the other side so it does not overlap with the cumulative histogram.
        BgMCLegend.SetX1NDC(topLeft_x_left + 0.05)
        BgMCLegend.SetX2NDC(topLeft_x_left + 0.05 + x_width)
        BgMCLegend.SetY1NDC(-0.05 + y_max-entry_height*(numExtraEntries+numBgMCSamples+numDataSamples))
        BgMCLegend.SetY2NDC(-0.05 + y_max)
        BgMCLegend.Draw()

    elif (numSignalSamples is not 0):
        SignalMCLegend.SetX1NDC(x_left)
        SignalMCLegend.SetY1NDC(y_max-entry_height*numSignalSamples)
        SignalMCLegend.SetX2NDC(x_right)
        SignalMCLegend.SetY2NDC(y_max)
        SignalMCLegend.SetTextSize(0.0364078)
        SignalMCLegend.Draw()

    # Deciding which text labels to draw and drawing them
    drawLumiLabel = False
    drawHeaderLabel = False
    drawTimeLabel = False
    drawDirLabel = False

    if not normalizeToUnitArea or numDataSamples > 0: #don't draw the lumi label if there's no data and it's scaled to unit area
        drawLumiLabel = True
        drawTimeLabel = True
        drawDirLabel = True
    if makeFancy:
        drawHeaderLabel = True
        drawLumiLabel = True
        drawTimeLabel = False
        drawDirLabel = False

    #now that flags are set, draw the appropriate labels

    if drawLumiLabel:
        LumiLabel.Draw()

    if drawHeaderLabel:
        HeaderLabel.Draw()
    if drawTimeLabel:
        TimeLabel.Draw()
    if drawDirLabel:
        DirLabel.Draw()



    #drawing the ratio or difference plot if requested

    if (makeRatioPlots or makeDiffPlots or makeSignifPlots):
        Canvas.cd(2)
        BgSum = Stack.GetStack().Last()
        if makeRatioPlots:
            makeRatio = functools.partial (ratioHistogram, DataHistograms[0], BgSum)
            if ratioRelErrMax is not -1: # it gets initialized to this dummy value of -1
                makeRatio = functools.partial (makeRatio, relErrMax = float (ratioRelErrMax))
            if addOneToRatio is not -1: # it gets initialized to this dummy value of -1
                makeRatio = functools.partial (makeRatio, addOne = bool (addOneToRatio))
            Comparison = makeRatio ()
        elif makeDiffPlots:
            Comparison = DataHistograms[0].Clone("diff")
            Comparison.Add(BgSum,-1)
            Comparison.SetTitle("")
            Comparison.GetYaxis().SetTitle("obs-exp")
        elif makeSignifPlots:
            Comparisons = signifHistograms(BgSum,SignalMCHistograms)
            Comparison = Comparisons[0]
            Comparison.SetTitle("")
            Comparison.GetYaxis().SetTitle("S/#sqrt{S+B}")
        Comparison.GetXaxis().SetTitle(xAxisLabel)
        Comparison.GetYaxis().CenterTitle()
        Comparison.GetYaxis().SetTitleSize(0.14)
        Comparison.GetYaxis().SetTitleOffset(0.43)
        Comparison.GetXaxis().SetTitleSize(0.15)
        Comparison.GetYaxis().SetLabelSize(0.13)
        Comparison.GetXaxis().SetLabelSize(0.15)

        RatioYRange = 1.15 # default of just over 100%
        if arguments.ratioYRange:
            RatioYRange = float(arguments.ratioYRange)
        if arguments.paperConfig:
            if 'ratioYRange' in paperHistogram:
                RatioYRange = paperHistogram['ratioYRange']

        if not addOneToRatio:
            Comparison.GetYaxis().SetRangeUser(-1*RatioYRange, RatioYRange)
        else:
            Comparison.GetYaxis().SetRangeUser(-1*RatioYRange + 1.0, RatioYRange + 1.0)
        Comparison.GetYaxis().SetNdivisions(205)
        Comparison.Draw("E0")
        if makeSignifPlots:  # Draw the other significance hists
            YMax = Comparison.GetMaximum()
            YMin = Comparison.GetMinimum()
            for i in range(1, len(Comparisons)):
                Comparisons[i].Draw("same")
                if Comparisons[i].GetMaximum() > YMax:
                    YMax = Comparisons[i].GetMaximum()
                if Comparisons[i].GetMinimum() < YMin:
                    YMax = Comparisons[i].GetMinimum()
            Comparison.GetYaxis().SetRangeUser(0, 1.2*YMax)


    if not arguments.paperConfig:
        Canvas.Write()

    if arguments.paperConfig:
        if 'output_name' in paperHistogram:
            Canvas.SaveAs("figures/"+paperHistogram['output_dir']+"/"+paperHistogram['output_name']+".pdf")
        else:
            Canvas.SaveAs("figures/"+paperHistogram['output_dir']+"/"+histogramName+".pdf")

    if arguments.savePDFs:
        pathToDirString = plainTextString(pathToDir)
        Canvas.SaveAs(condor_dir+"/stacked_histograms_pdfs/"+pathToDirString+"/"+histogramName+".pdf")


##########################################################################################################################################
##########################################################################################################################################
##########################################################################################################################################

def MakeTwoDHist(pathToDir,histogramName):
    blindData = False
    # To blind histograms, define a list of histsToBlind in the localOptions.py file
    try:  # Use "try" in case histsToBlind does not exist
        if histsToBlind:
            for histToBlind in histsToBlind:
                if histToBlind in histogramName:
                    print "Blinding data for histogram " + histogramName
                    blindData = True
    except NameError:
        time.sleep(0.000001) # Do nothing if histsToBlind does not exist

    numBgMCSamples = 0
    numDataSamples = 0
    numSignalSamples = 0


    ###############################################
    makeFancy = arguments.makeFancy
    if arguments.paperConfig:
        if 'makeFancy' in paperHistogram:
            makeFancy = paperHistogram['makeFancy']
    ###############################################

    HeaderLabel = TPaveLabel(header_x_left,header_y_bottom,header_x_right,header_y_top,HeaderText,"NDC")
    HeaderLabel.SetTextFont(42)
    HeaderLabel.SetTextSize(0.697674)
    HeaderLabel.SetTextAlign(32)
    HeaderLabel.SetBorderSize(0)
    HeaderLabel.SetFillColor(0)
    HeaderLabel.SetFillStyle(0)

    LumiLabel = TPaveLabel(topLeft_x_left,topLeft_y_bottom,topLeft_x_right,topLeft_y_top,LumiText,"NDC")
    LumiLabel.SetTextFont(42)
    LumiLabel.SetTextSize(0.731707)
    LumiLabel.SetBorderSize(0)
    LumiLabel.SetFillColor(0)
    LumiLabel.SetFillStyle(0)

    if makeFancy:
        LumiLabel = TPaveLabel(topLeft_x_left,topLeft_y_bottom,topLeft_x_right,topLeft_y_top,"CMS Preliminary","NDC")
        LumiLabel.SetTextFont(62)
        LumiLabel.SetTextSize(0.8)
        LumiLabel.SetTextAlign(12)
    else:
        LumiLabel = TPaveLabel(topLeft_x_left,topLeft_y_bottom,topLeft_x_right,topLeft_y_top,LumiText,"NDC")
        LumiLabel.SetTextAlign(32)
        LumiLabel.SetTextFont(42)

        if makeRatioPlots:
            ts_y_bottom  = 0.96
            ts_y_top     = 1.0
            dir_y_top    = ts_y_bottom
            dir_y_bottom = 0.92

        TimeLabel = TPaveLabel(ts_x_left,ts_y_bottom,ts_x_right, ts_y_top,TimeText,"NDC")
        TimeLabel.SetTextAlign(12)  # 12 = 10 (left-justify horizontal) + 2 (center-justify vertical)
        TimeLabel.SetTextSize(0.55)
        TimeLabel.SetBorderSize(0)
        TimeLabel.SetFillColor(0)
        TimeLabel.SetFillStyle(0)

        DirLabel = TPaveLabel(dir_x_left,dir_y_bottom,dir_x_right, dir_y_top,DirText,"NDC")
        DirLabel.SetTextAlign(12)
        DirLabel.SetTextSize(0.55)
        DirLabel.SetBorderSize(0)
        DirLabel.SetFillColor(0)
        DirLabel.SetFillStyle(0)


    LumiLabel.SetBorderSize(0)
    LumiLabel.SetFillColor(0)
    LumiLabel.SetFillStyle(0)

    NormLabel = TPaveLabel()
    NormLabel.SetDrawOption("NDC")
    NormLabel.SetX1NDC(topLeft_x_left)
    NormLabel.SetX2NDC(topLeft_x_right)

    NormLabel.SetBorderSize(0)
    NormLabel.SetFillColor(0)
    NormLabel.SetFillStyle(0)

    NormText = ""
    if arguments.normalizeToUnitArea:
        NormText = "Scaled to unit area"
    elif arguments.normalizeToData:
        NormText = "MC scaled to data"
        NormLabel.SetLabel(NormText)

    BgMCLegend = TLegend(0.52765,0.76764,0.834101,0.879562)
    BgMCLegend.AddEntry (0, "Data & Bkgd. MC", "H").SetTextFont (62)
    BgMCLegend.SetTextFont(42)
    BgMCLegend.SetTextSize(0.0364078)
    BgMCLegend.SetBorderSize(0)
    BgMCLegend.SetFillColor(0)
    BgMCLegend.SetFillStyle(0)
    SignalMCLegend = TLegend(0.157471,0.709648,0.355172,0.821602)
    SignalMCLegend.AddEntry (0, "Signal MC", "H").SetTextFont (62)
    SignalMCLegend.SetTextFont(42)
    SignalMCLegend.SetTextSize(0.0364078)
    SignalMCLegend.SetBorderSize(0)
    SignalMCLegend.SetFillColor(0)
    SignalMCLegend.SetFillStyle(0)

    if not arguments.paperConfig:
        outputFile.cd(pathToDir)
    Canvas = TCanvas(histogramName,"",4,55,872,850)
    Canvas.SetHighLightColor(2)
    Canvas.Range(-72.16495,-10.50091,516.9367,82.84142)
    Canvas.SetFillColor(0)
    Canvas.SetBorderMode(0)
    Canvas.SetBorderSize(2)
    Canvas.SetTickx(1)
    Canvas.SetTicky(1)
    Canvas.SetLeftMargin(0.1225)
    if arguments.unique2D:
        Canvas.SetRightMargin(0.14)
    else:
        Canvas.SetRightMargin(0.0357143)
    Canvas.SetTopMargin(0.0725)
    Canvas.SetBottomMargin(0.1125)
    Canvas.SetFrameBorderMode(0)
    Canvas.SetFrameBorderMode(0)
    BgMCHistograms = []
    SignalMCHistograms = []
    DataHistograms = []

    for sample in processed_datasets: # loop over different samples as listed in configurationOptions.py
        if arguments.verbose:
            print "Starting to process sample", sample
        dataset_file = "%s/%s.root" % (condor_dir,sample)
        inputFile = TFile(dataset_file)
        HistogramObj = inputFile.Get(pathToDir+"/"+histogramName)
        if not HistogramObj:
            print "WARNING:  Could not find histogram " + pathToDir + "/" + histogramName + " in file " + dataset_file + ".  Will skip it and continue."
            continue
        Histogram = HistogramObj.Clone()
        Histogram.SetDirectory(0)
        inputFile.Close()
        if arguments.rebinFactor:
            RebinFactor = int(arguments.rebinFactor)
            #don't rebin histograms which will have less than 5 bins or any gen-matching histograms
            if Histogram.GetNbinsX() >= RebinFactor*5 and Histogram.GetName().find("GenMatch") is -1:
                Histogram.Rebin(RebinFactor)
        xAxisLabel = Histogram.GetXaxis().GetTitle()
        yAxisLabel = Histogram.GetYaxis().GetTitle()
        if not arguments.makeFancy:
            histoTitle = Histogram.GetTitle()
        else:
            histoTitle = ""

        if( types[sample] == "bgMC"):

            numBgMCSamples += 1
            Histogram.SetMarkerColor(colors[sample])
            Histogram.SetMarkerStyle(24)
            Histogram.SetMarkerSize(1.5)
            Histogram.SetFillColor(colors[sample])
            BgMCLegend.AddEntry(Histogram,labels[sample],"P").SetTextFont (42)
            BgMCHistograms.append(Histogram)

        elif( types[sample] == "signalMC"):

            numSignalSamples += 1
            Histogram.SetMarkerColor(colors[sample])
            Histogram.SetMarkerStyle(20)
            Histogram.SetMarkerSize(1.5)
            Histogram.SetFillColor(colors[sample])
            SignalMCLegend.AddEntry(Histogram,labels[sample],"P").SetTextFont (42)
            SignalMCHistograms.append(Histogram)

        elif( types[sample] == "data" and not blindData):

            numDataSamples += 1
            Histogram.SetMarkerColor(colors[sample])
            Histogram.SetMarkerStyle(34)
            Histogram.SetMarkerSize(1.5)
            Histogram.SetFillColor(colors[sample])
            BgMCLegend.AddEntry(Histogram,labels[sample],"P").SetTextFont (42)
            DataHistograms.append(Histogram)

    if not arguments.paperConfig:
        outputFile.cd(pathToDir)

    #don't overlay the 2D plots, draw them separately with the colz option
    if arguments.unique2D:
        if(numDataSamples is not 0):
            DataHistograms[0].SetTitle(histoTitle)
            DataHistograms[0].GetXaxis().SetTitle(xAxisLabel)
            DataHistograms[0].GetYaxis().SetTitle(yAxisLabel)
            DataHistograms[0].Draw("colz")
        elif(numSignalSamples is not 0):
            SignalMCHistograms[0].SetTitle(histoTitle)
            SignalMCHistograms[0].GetXaxis().SetTitle(xAxisLabel)
            SignalMCHistograms[0].GetYaxis().SetTitle(yAxisLabel)
            SignalMCHistograms[0].Draw("colz")

    else:
        if(numBgMCSamples is not 0):
            BgMCHistograms[0].SetTitle(histoTitle)
            BgMCHistograms[0].GetXaxis().SetTitle(xAxisLabel)
            BgMCHistograms[0].GetYaxis().SetTitle(yAxisLabel)
            BgMCHistograms[0].Draw()
            for signalMCHist in SignalMCHistograms:
                signalMCHist.Draw("SAME")
            for dataHist in DataHistograms:
                dataHist.Draw("SAME")

        elif(numSignalSamples is not 0):
            SignalMCHistograms[0].SetTitle(histoTitle)
            SignalMCHistograms[0].Draw()
            SignalMCHistograms[0].GetXaxis().SetTitle(xAxisLabel)
            SignalMCHistograms[0].GetYaxis().SetTitle(yAxisLabel)
            for signalMCHist in SignalMCHistograms:
                if(signalMCHist is not SignalMCHistograms[0]):
                    signalMCHist.Draw("SAME")
            for dataHist in DataHistograms:
                dataHist.Draw("SAME")

        elif(numDataSamples is not 0):
            DataHistograms[0].SetTitle(histoTitle)
            DataHistograms[0].GetXaxis().SetTitle(xAxisLabel)
            DataHistograms[0].GetYaxis().SetTitle(yAxisLabel)
            DataHistograms[0].Draw()
            for dataHist in DataHistograms:
                if(dataHist is not DataHistograms[0]):
                    dataHist.Draw("SAME")



    # Deciding which text labels to draw and drawing them
    drawLumiLabel = False
    drawNormLabel = False
    offsetNormLabel = False
    drawHeaderLabel = False
    drawTimeLabel = False
    drawDirLabel = False
    if not arguments.normalizeToUnitArea or numDataSamples > 0: #don't draw the lumi label if there's no data and it's scaled to unit area
        drawLumiLabel = True
        drawTimeLabel = True
        drawDirLabel = True
        #move the normalization label down before drawing if we drew the lumi. label
        offsetNormLabel = True
    if arguments.normalizeToUnitArea or arguments.normalizeToData:
        drawNormLabel = True
        drawTimeLabel = True
        drawDirLabel = True
    if arguments.makeFancy:
        drawHeaderLabel = True
        drawLumiLabel = True
        drawTimeLabel = False
        drawDirLabel = False

    #now that flags are set, draw the appropriate labels

    if drawLumiLabel:
        LumiLabel.Draw()

    if drawTimeLabel:
        TimeLabel.Draw()
    if drawNormLabel:
        if offsetNormLabel:
            NormLabel.SetY1NDC(topLeft_y_bottom-topLeft_y_offset)
            NormLabel.SetY2NDC(topLeft_y_top-topLeft_y_offset)
        else:
            NormLabel.SetY1NDC(topLeft_y_bottom)
            NormLabel.SetY2NDC(topLeft_y_top)
        NormLabel.Draw()

    if drawHeaderLabel:
        HeaderLabel.Draw()



    if(numBgMCSamples is not 0 or numDataSamples is not 0):
        if not arguments.unique2D:
            BgMCLegend.Draw()
    if(numSignalSamples is not 0):
        if not arguments.unique2D:
            SignalMCLegend.Draw()
    if not arguments.paperConfig:
        Canvas.Write()

    if arguments.savePDFs:
        pathToDirString = plainTextString(pathToDir)
        Canvas.SaveAs(condor_dir+"/stacked_histograms_pdfs/"+pathToDirString+"/"+histogramName+".pdf")



##########################################################################################################################################
##########################################################################################################################################
##########################################################################################################################################


processed_datasets = []

#### if there's a list of specified histograms, we'll just make those ones and then quit
if arguments.paperConfig:

    try:
        shutil.rmtree ("figures")
    except:
        pass
    os.mkdir ("figures")

    for paperHistogram in paper_histograms:
        # set up output directories for paper-ready plots
        pathToDir = "figures/"+paperHistogram['output_dir']
        if not os.path.exists(pathToDir):
            os.mkdir ("%s" % (pathToDir))

        # make each canvas and save the pdf
        directory = "OSUAnalysis/" + paperHistogram['channel']
        condor_dir = "condor/" + paperHistogram['condor_dir']
        MakeOneDHist(directory,paperHistogram['name'],"none")

    sys.exit(0) # our work here is done




condor_dir = set_condor_output_dir(arguments)

#### check which input datasets have valid output files
if arguments.verbose:
    print "Number of datasets: " + str(len(datasets))
    print datasets
for sample in datasets:
    fileName = condor_dir + "/" + sample + ".root"
    if arguments.verbose:
        print "Adding fileName: " + fileName
    if not os.path.exists(fileName):
        print "WARNING: didn't find ",fileName
        continue
    testFile = TFile(fileName)
    if testFile.IsZombie() or not testFile.GetNkeys():
        continue
    processed_datasets.append(sample)

if len(processed_datasets) is 0:
    sys.exit("No datasets have been processed")

if arguments.savePDFs:
    try:
        shutil.rmtree ("%s/stacked_histograms_pdfs" % (condor_dir))
    except OSError:
        pass
    os.mkdir ("%s/stacked_histograms_pdfs" % (condor_dir))

#### make output file
outputFileString = ''
if arguments.rebinFactor:
    outputFileString += '_Rebin' + str(arguments.rebinFactor)
if arguments.makeRatioPlots:
    outputFileString += '_Ratio'
if arguments.normalizeToData:
    outputFileString += '_Norm'
if arguments.makeCumulativePlots:
    outputFileString += "_Cumulative"
outputFileName = "stacked_histograms" + outputFileString + ".root"
if arguments.makeSignificancePlots:
    outputFileName = "stacked_histogramsSignif.root"
if arguments.outputFileName:
    outputFileName = arguments.outputFileName
outputDir = "condor/" + arguments.outputDirectory if arguments.outputDirectory else condor_dir

outputFile = TFile(outputDir + "/" + outputFileName, "RECREATE")

#### use the first input file as a template and make stacked versions of all its histograms
inputFile = TFile(condor_dir + "/" + processed_datasets[0] + ".root")
inputFile.cd()
outputFile.cd()

#get root directory in the first layer, generally "OSUAnalysis"
for key in inputFile.GetListOfKeys():
    if (key.GetClassName() != "TDirectoryFile"):
        continue
    rootDirectory = key.GetName()
    outputFile.mkdir(rootDirectory)
    if arguments.savePDFs:
        os.mkdir ("%s/stacked_histograms_pdfs/%s" % (condor_dir,plainTextString(rootDirectory)))

    #cd to root directory and look for histograms
    inputFile.cd(rootDirectory)
    for key2 in gDirectory.GetListOfKeys():

        if re.match ('TH1', key2.GetClassName()): # found a 1-D histogram
            if arguments.makeSignificancePlots or arguments.makeCumulativePlots:
                MakeOneDHist(rootDirectory,key2.GetName(),"left")
                MakeOneDHist(rootDirectory,key2.GetName(),"right")
            else:
                MakeOneDHist(rootDirectory,key2.GetName(),"none")
        elif re.match ('TH2', key2.GetClassName()) and arguments.draw2DPlots: # found a 2-D histogram
            MakeTwoDHist(rootDirectory,key2.GetName())

        elif (key2.GetClassName() == "TDirectoryFile"): # found a directory, cd there and look for histograms
            level2Directory = rootDirectory+"/"+key2.GetName()

            #make a corresponding directory in the output file
            outputFile.cd(rootDirectory)
            gDirectory.mkdir(key2.GetName())
            if arguments.savePDFs:
                os.mkdir ("%s/stacked_histograms_pdfs/%s" % (condor_dir,plainTextString(level2Directory)))

            #####################################################
            ###  This layer is typically the "channels" layer ###
            #####################################################

            inputFile.cd(level2Directory)
            for key3 in gDirectory.GetListOfKeys():
#                if arguments.quickHistName and not arguments.quickHistName in key3.GetName():
                if arguments.quickHistName and not arguments.quickHistName == key3.GetName():
                    continue
                if re.match ('TH1', key3.GetClassName()): # found a 1-D histogram
                    if arguments.makeSignificancePlots or arguments.makeCumulativePlots:
                        MakeOneDHist(level2Directory,key3.GetName(),"left")
                        MakeOneDHist(level2Directory,key3.GetName(),"right")
                    else:
                        MakeOneDHist(level2Directory,key3.GetName(),"none")
                elif re.match ('TH2', key3.GetClassName()) and arguments.draw2DPlots: # found a 2-D histogram
                    MakeTwoDHist(level2Directory,key3.GetName())

                elif (key3.GetClassName() == "TDirectoryFile"): # found a directory, cd there and look for histograms
                    level3Directory = level2Directory+"/"+key3.GetName()

                    #make a corresponding directory in the output file
                    outputFile.cd(level2Directory)
                    gDirectory.mkdir(key3.GetName())
                    if arguments.savePDFs:
                        os.mkdir ("%s/stacked_histograms_pdfs/%s" % (condor_dir,plainTextString(level3Directory)))

                    #################################################
                    ###  This layer is typically the "cuts" layer ###
                    #################################################

                    inputFile.cd(level3Directory)
                    for key3 in gDirectory.GetListOfKeys():
                        if re.match ('TH1', key3.GetClassName()): # found a 1-D histogram
                            if arguments.makeSignificancePlots or arguments.makeCumulativePlots:
                                MakeOneDHist(level3Directory,key3.GetName(),"left")
                                MakeOneDHist(level3Directory,key3.GetName(),"right")
                            else:
                                MakeOneDHist(level3Directory,key3.GetName(),"none")
                        elif re.match ('TH2', key3.GetClassName()) and arguments.draw2DPlots: # found a 2-D histogram
                            MakeTwoDHist(level3Directory,key3.GetName())


outputFile.Close()
print "Finished writing plots to", str(outputDir + "/" + outputFileName)

