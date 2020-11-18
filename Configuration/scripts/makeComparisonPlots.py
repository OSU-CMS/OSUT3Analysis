#!/usr/bin/env python
import sys
import os
import re
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

parser.remove_option("-e")
parser.remove_option("-n")
parser.remove_option("--2D")
parser.remove_option("-y")

parser.add_option("-f", "--fancy", action="store_true", dest="makeFancy", default=False,
                  help="removes the title and replaces it with the official CMS plot heading")
parser.add_option("--dontRebinRatio", action="store_true", dest="dontRebinRatio", default=False,
                  help="don't do the rebinning of ratio plots")
parser.add_option("-A", "--addOneToRatio", action='store_true', dest="addOneToRatio",
                  help="add one to the ratio so that data/MC is plotted")
parser.add_option("-E", "--ratioRelErrMax", dest="ratioRelErrMax",
                  help="maximum error used in rebinning the ratio histogram")
parser.add_option("--ylog", action="store_true", dest="setLogY", default=False,
                  help="Set logarithmic scale on vertical axis on all plots")
parser.add_option("--ymin", dest="setYMin",
                  help="Minimum of y axis")
parser.add_option("--ymax", dest="setYMax",
                  help="Maximum of y axis")
parser.add_option("--hist", action="store_true", dest="plot_hist", default=False,
                                    help="plot as hollow histograms instead of error bar crosses")
parser.add_option("--line-width", dest="line_width",
                                    help="set line width (default is 2)")
parser.add_option("-g", "--generic", action="store_true", dest="generic", default=False,
                  help="generic root file directory structure; does not assume that channel dir is in OSUAnalysis dir")
parser.add_option("-s", "--signif", action="store_true", dest="makeSignificancePlots", default=False,
                                    help="Make significance plots")
parser.add_option("--NO", "--noOverUnderFlow", action="store_true", dest="noOverUnderFlow", default=False,
                  help="Do not add the overflow and underflow entries to the last and first bins")
parser.add_option("-v", "--verbose", action="store_true", dest="verbose", default=False,
                  help="verbose output")


(arguments, args) = parser.parse_args()


if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.localConfig) + " import *")

noOverUnderFlow = arguments.noOverUnderFlow


#### deal with conflicting arguments

if arguments.makeRatioPlots and arguments.makeDiffPlots:
    print "You have requested both ratio and difference plots.  Will make just ratio plots instead"
    arguments.makeRatioPlots = False

if arguments.makeSignificancePlots and arguments.makeRatioPlots:
    print "You have asked to make a ratio plot and significance plots. This is a very strange request.  Will skip making the ratio plot."
    arguments.makeRatioPlots = False
if arguments.makeSignificancePlots and arguments.makeDiffPlots:
    print "You have asked to make a difference plot and significance plots. This is a very strange request.  Will skip making the difference plot."
    arguments.makeDiffPlots = False

from OSUT3Analysis.Configuration.histogramUtilities import ratioHistogram
from ROOT import TFile, gROOT, gStyle, gDirectory, TStyle, THStack, TH1F, TCanvas, TString, TLegend, TLegendEntry, THStack, TIter, TKey, TPaveLabel, gPad


### setting ROOT options so our plots will look awesome and everyone will love us

gROOT.SetBatch()
gStyle.SetOptStat(0)
gStyle.SetCanvasBorderMode(0)
gStyle.SetPadBorderMode(0)
gStyle.SetPadColor(0)
gStyle.SetCanvasColor(0)
gStyle.SetTextFont(42)
gStyle.SetCanvasDefH(600)
gStyle.SetCanvasDefW(600)
gStyle.SetCanvasDefX(0)
gStyle.SetCanvasDefY(0)
gStyle.SetPadTopMargin(0.07)
gStyle.SetPadBottomMargin(0.13)
gStyle.SetPadLeftMargin(0.15)
gStyle.SetPadRightMargin(0.05)
gStyle.SetTitleColor(1, "XYZ")
gStyle.SetTitleFont(42, "XYZ")
gStyle.SetTitleSize(0.04, "XYZ")
gStyle.SetTitleXOffset(1.1)
gStyle.SetTitleYOffset(2)
gStyle.SetTextAlign(12)
gStyle.SetLabelColor(1, "XYZ")
gStyle.SetLabelFont(42, "XYZ")
gStyle.SetLabelOffset(0.007, "XYZ")
gStyle.SetLabelSize(0.04, "XYZ")
gStyle.SetAxisColor(1, "XYZ")
gStyle.SetStripDecimals(True)
gStyle.SetTickLength(0.03, "XYZ")
gStyle.SetNdivisions(510, "XYZ")
gStyle.SetPadTickX(1)
gStyle.SetPadTickY(1)
gROOT.ForceStyle()

line_width = 2
if arguments.line_width:
    line_width = arguments.line_width

#set the text for the luminosity label
if(intLumi < 1000.):
    LumiInPb = intLumi
    LumiText = str(intLumi) + " pb^{-1}"
    LumiText = str.format('{0:.1f}', LumiInPb) + " pb^{-1}"
else:
    LumiInFb = intLumi/1000.
    LumiText = str.format('{0:.1f}', LumiInFb) + " fb^{-1}"

#bestest place for lumi. label, in top left corner
topLeft_x_left    = 0.16129
topLeft_y_bottom  = 0.8
topLeft_x_right   = 0.512673
topLeft_y_top     = 0.892944
topLeft_y_offset  = 0.04

#set the text for the fancy heading
HeaderText = LumiText + " (13 TeV)"

#position for header
header_x_left    = 0.602535
header_y_bottom  = 0.928224
header_x_right   = 0.963134
header_y_top     = 0.980535

makeFancy = arguments.makeFancy

colors = {
    'black'  : 1,
    'red'    : 633,
    'green'  : 417,
    'purple' : 881,
    'blue'   : 601,
    'orange' : 801,
    'yellow' : 401,
}

colorList = [
    'black',
    'red',
    'green',
    'purple',
    'blue',
    'orange',
    'yellow',
]


markers = {
    'circle'   : 20,
    'square'   : 21,
    'triangle' : 22,
}

markerStyleList = [
    'circle',
    'square',
    'triangle',
]

fills = {
    'solid'  : 0,
    'hollow' : 4,
}

fillList = [
    'solid',
    'hollow',
]



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
        for i in range(lowLimit,nbins+1):  # start with underflow bin
            integral += hist.GetBinContent(i)
            error = math.sqrt(error*error + hist.GetBinError(i)*hist.GetBinError(i)) # sum errors in quadrature
            hist.SetBinContent(i, integral)
            hist.SetBinError  (i, error)
        if not arguments.noOverUnderFlow:
            # Then include overflow bin in the last bin
            hist.SetBinContent(nbins, hist.GetBinContent(nbins) + hist.GetBinContent(nbins+1))
            hist.SetBinError  (nbins, math.sqrt(hist.GetBinError(nbins)*hist.GetBinError(nbins) + hist.GetBinError(nbins+1)*hist.GetBinError(nbins+1)))
    elif integrateDir is "right":
        for i in xrange(uppLimit, 0, -1):  # start with overflow bin
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


def MakeOneDHist(histogramDirectory, histogramName,integrateDir):

    if arguments.verbose:
        print "Creating histogram", histogramName, "in directory", histogramDirectory

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
        LumiLabel.SetTextSize(0.7)
        LumiLabel.SetTextAlign(12)
    else:
        LumiLabel = TPaveLabel(topLeft_x_left,topLeft_y_bottom,topLeft_x_right,topLeft_y_top,LumiText,"NDC")
        LumiLabel.SetTextAlign(32)
        LumiLabel.SetTextFont(42)
    LumiLabel.SetBorderSize(0)
    LumiLabel.SetFillColor(0)
    LumiLabel.SetFillStyle(0)

    Legend = TLegend()
    Legend.SetBorderSize(0)
    Legend.SetFillColor(0)
    Legend.SetFillStyle(0)

    canvasName = histogramName
    if integrateDir is "left":
        canvasName += "_CumulativeLeft"
    elif integrateDir is "right":
        canvasName += "_CumulativeRight"
    Canvas = TCanvas(canvasName)
    Histograms = []
    RefIndex = -99
    LegendEntries = []

    colorIndex = 0
    markerStyleIndex = 0
    fillIndex = 0

    for source in input_sources: # loop over different input sources in config file
        dataset_file = "condor/%s/%s.root" % (source['condor_dir'],source['dataset'])
        inputFile = TFile(dataset_file)


        if arguments.generic:
            if histogramDirectory == "":
                histPath = histogramName
            else:
                histPath = histogramDirectory + "/" + histogramName
            HistogramObj = inputFile.Get(histPath)
        else:
            HistogramObj = inputFile.Get(source['channel'] + "Plotter/" + histogramDirectory + "/" + histogramName)
        if not HistogramObj:
            print "WARNING:  Could not find histogram " + source['channel'] + "/" + histogramName + " in file " + dataset_file + ".  Will skip it and continue."
            return
        Histogram = HistogramObj.Clone()
        Histogram.SetDirectory(0)
        inputFile.Close()
        Histogram.Sumw2()
        if arguments.verbose:
            print "  Got histogram", Histogram.GetName(), "from file", dataset_file
        if arguments.rebinFactor:
            RebinFactor = int(arguments.rebinFactor)
            #don't rebin histograms which will have less than 5 bins or any gen-matching histograms
            if Histogram.GetNbinsX() >= RebinFactor*5 and Histogram.GetTitle().find("GenMatch") is -1:
                Histogram.Rebin(RebinFactor)

        # correct bin contents of object multiplcity plots
        if Histogram.GetName().startswith("num") and "PV" not in Histogram.GetName():
            # include overflow bin
            for bin in range(2,Histogram.GetNbinsX()+2):
                content = Histogram.GetBinContent(bin)
                Histogram.SetBinContent(bin, content/float(bin-1))


        xAxisLabel = Histogram.GetXaxis().GetTitle()
        unitBeginIndex = xAxisLabel.find("[")
        unitEndIndex = xAxisLabel.find("]")
        xAxisLabelVar = xAxisLabel

        if "_pfx" in Histogram.GetName() or "_pfy" in Histogram.GetName() or "_sigma" in Histogram.GetName():
            yAxisLabel = Histogram.GetYaxis().GetTitle()
        else:

            if unitBeginIndex is not -1 and unitEndIndex is not -1: #x axis has a unit
                yAxisLabel = "Entries / " + str(Histogram.GetXaxis().GetBinWidth(1)) + " " + xAxisLabel[unitBeginIndex+1:unitEndIndex]
                xAxisLabelVar = xAxisLabel[0:unitBeginIndex]
            else:
                yAxisLabel = "Entries per bin (" + str(Histogram.GetXaxis().GetBinWidth(1)) + " width)"
            if arguments.normalizeToUnitArea:
                yAxisLabel = yAxisLabel + " (Unit Area Norm.)"

            if arguments.normalizeToUnitArea and arguments.makeSignificancePlots:
                unit = "Efficiency"
            else:
                unit = "Yield"
            if integrateDir is "left":
                yAxisLabel = unit + ", " + xAxisLabelVar + "< x (" + str(Histogram.GetXaxis().GetBinWidth(1)) + " bin width)"
            if integrateDir is "right":
                yAxisLabel = unit + ", " + xAxisLabelVar + "> x (" + str(Histogram.GetXaxis().GetBinWidth(1)) + " bin width)"


        if not noOverUnderFlow:
            nbins = Histogram.GetNbinsX()
            Histogram.SetBinContent(1,     Histogram.GetBinContent(1)     + Histogram.GetBinContent(0))       # Add underflow
            Histogram.SetBinContent(nbins, Histogram.GetBinContent(nbins) + Histogram.GetBinContent(nbins+1)) # Add overflow
            # Set the errors to be the sum in quadrature
            Histogram.SetBinError(1,     math.sqrt(math.pow(Histogram.GetBinError(1),    2) + math.pow(Histogram.GetBinError(0),      2)))
            Histogram.SetBinError(nbins, math.sqrt(math.pow(Histogram.GetBinError(nbins),2) + math.pow(Histogram.GetBinError(nbins+1),2)))

        if not arguments.makeFancy and not arguments.generic:
            fullTitle = Histogram.GetTitle()
            splitTitle = fullTitle.split(":")
            if len(splitTitle) > 1:
                histoTitle = splitTitle[1].lstrip(" ")
            else:
                histoTitle = splitTitle[0]
        else:
            histoTitle = ""

        if 'color' in source:
            Histogram.SetMarkerColor(colors[source['color']])
            Histogram.SetLineColor(colors[source['color']])
        else:
            Histogram.SetMarkerColor(colors[colorList[colorIndex]])
            Histogram.SetLineColor(colors[colorList[colorIndex]])
            colorIndex = colorIndex + 1
            if colorIndex is len(colorList):
                colorIndex = 0
                markerStyleIndex = markerStyleIndex + 1
                if markerStyleIndex is len(markerStyleList):
                    markerStyleIndex = 0
                    fillIndex = fillIndex + 1


        if 'scale' in source:
            Histogram.Scale(source['scale'])

        markerStyle = 20
        if 'marker' in source:
            markerStyle = markers[source['marker']]
        else:
            markerStyle = markers[markerStyleList[markerStyleIndex]]

        fillStyle = 0
        if 'fill' in source:
            markerStyle = markerStyle + fills[source['fill']]
        else:
            markerStyle = markerStyle + fills[fillList[fillIndex]]

        Histogram.SetMarkerStyle(markerStyle)
        Histogram.SetMarkerSize(0.5)

        Histogram.SetLineWidth(line_width)
        Histogram.SetFillStyle(0)

        if arguments.normalizeToUnitArea and Histogram.Integral() > 0:
            Histogram.Scale(1./Histogram.Integral())

        Histogram = MakeIntegralHist(Histogram, integrateDir)

        LegendEntries.append(source['legend_entry'])
        Histograms.append(Histogram)
        if 'reference' in source:
            if source['reference']:
                RefIndex = len(Histograms)-1

    ### formatting histograms and adding to legend
    legendIndex = 0
    for histogram in Histograms:
        Legend.AddEntry(histogram,LegendEntries[legendIndex],"LEP")
#        Legend.AddEntry(histogram,LegendEntries[legendIndex],"P")
        legendIndex = legendIndex+1

    ### finding the maximum value of anything going on the canvas, so we know how to set the y-axis
    finalMax = 0
    for histogram in Histograms:
        currentMax = histogram.GetMaximum() + histogram.GetBinError(histogram.GetMaximumBin())
        if(currentMax > finalMax):
            finalMax = currentMax
    finalMax = 1.5*finalMax
    if arguments.setYMax:
        finalMax = float(arguments.setYMax)

    ### Drawing histograms to canvas

    makeRatioPlots = arguments.makeRatioPlots
    makeDiffPlots = arguments.makeDiffPlots
    addOneToRatio = -1
    if arguments.addOneToRatio:
        addOneToRatio = arguments.addOneToRatio

    yAxisMin = 0.0001
    if arguments.setYMin:
        yAxisMin = float(arguments.setYMin)

    if makeRatioPlots or makeDiffPlots:
        Canvas.SetFillStyle(0)
        Canvas.Divide(1,2)
        Canvas.cd(1)
        gPad.SetPad(0,0.25,1,1)
        gPad.SetMargin(0.15,0.05,0.01,0.07)
        gPad.SetFillStyle(0)
        gPad.Update()
        gPad.Draw()
        if arguments.setLogY:
            gPad.SetLogy()
        Canvas.cd(2)
        gPad.SetPad(0,0,1,0.25)
        #format: gPad.SetMargin(l,r,b,t)
        gPad.SetMargin(0.15,0.05,0.4,0.01)
        gPad.SetFillStyle(0)
        gPad.SetGridy(1)
        gPad.Update()
        gPad.Draw()

        Canvas.cd(1)

    histCounter = 0
    plotting_options = ""
    if arguments.generic:
        plotting_options = "p,e"
    if arguments.plot_hist:
        plotting_options = "HIST"

    for histogram in Histograms:
        histogram.SetTitle(histoTitle)
        if arguments.verbose:
            print "  Drawing hist " + histogram.GetName() + ", with plotting_options = " + plotting_options + ", with mean = " + str(histogram.GetMean()) + ", with color = " + str(histogram.GetLineColor())
        histogram.Draw(plotting_options)
        histogram.GetXaxis().SetTitle(xAxisLabel)
        histogram.GetYaxis().SetTitle(yAxisLabel)
        histogram.SetMaximum(finalMax)

        if "_pfx" not in Histogram.GetName() and "_pfy" not in Histogram.GetName() and "_sigma" not in Histogram.GetName():
            histogram.SetMinimum(yAxisMin)
        if makeRatioPlots or makeDiffPlots:
            histogram.GetXaxis().SetLabelSize(0)
        if histCounter is 0:
            plotting_options = plotting_options + " SAME"
        histCounter = histCounter + 1

    #legend coordinates, empirically determined :-)
    x_left = 0.1677852
    x_right = 0.9647651
    y_min = 0.6765734
    y_max = 0.9

    Legend.SetX1NDC(x_left)
    Legend.SetY1NDC(y_min)
    Legend.SetX2NDC(x_right)
    Legend.SetY2NDC(y_max)
    Legend.Draw()


    # Deciding which text labels to draw and drawing them
    if arguments.makeFancy:
        HeaderLabel.Draw()
        LumiLabel.Draw()




    #drawing the ratio or difference plot if requested

    if makeRatioPlots or makeDiffPlots:
        Comparisons = []
        Canvas.cd(2)
        if RefIndex == -99:
            Reference = Histograms[0]
        else:
            Reference = Histograms[RefIndex]

        for Histogram in Histograms:
            if Histogram is Reference:
                continue

            if makeRatioPlots:
                makeRatio = functools.partial (ratioHistogram,Histogram, Reference)
                if arguments.ratioRelErrMax is not -1: # it gets initialized to this dummy value of -1
                    makeRatio =  functools.partial (makeRatio, relErrMax = float(arguments.ratioRelErrMax))
                if addOneToRatio != -1: # it gets initialized to this dummy value of -1
                    makeRatio = functools.partial (makeRatio, addOne = bool (addOneToRatio))
                Comparison = makeRatio()
            elif makeDiffPlots:
                Comparison = Reference.Clone("diff")
                Comparison.Add(Histograms[1],-1)
                Comparison.SetTitle("")
                Comparison.GetYaxis().SetTitle("X-ref")

            Comparison.SetLineColor(Histogram.GetLineColor())
            Comparison.SetFillColor(Histogram.GetFillColor())
            Comparison.SetFillStyle(Histogram.GetFillStyle())
            Comparison.SetMarkerColor(Histogram.GetMarkerColor())
            Comparison.SetMarkerStyle(Histogram.GetMarkerStyle())

            Comparison.GetXaxis().SetTitle(xAxisLabel)
            Comparison.GetYaxis().CenterTitle()
            Comparison.GetYaxis().SetTitleSize(0.1)
            Comparison.GetYaxis().SetTitleOffset(0.5)
            Comparison.GetXaxis().SetTitleSize(0.15)
            Comparison.GetYaxis().SetLabelSize(0.1)
            Comparison.GetXaxis().SetLabelSize(0.15)

            if makeRatioPlots:
                RatioYRange = 1.15
                if arguments.ratioYRange:
                    RatioYRange = float(arguments.ratioYRange)
                if addOneToRatio == -1: # it gets initialized to this dummy value of -1
                    Comparison.GetYaxis().SetRangeUser(-1*RatioYRange, RatioYRange)
                else:
                    Comparison.GetYaxis().SetRangeUser(-1*RatioYRange + 1.0, RatioYRange + 1.0)

            elif makeDiffPlots:
                YMax = Comparison.GetMaximum()
                YMin = Comparison.GetMinimum()
                if YMax <= 0 and YMin <= 0:
                    Comparison.GetYaxis().SetRangeUser(-1.2*YMin,0)
                elif YMax >= 0 and YMin >= 0:
                    Comparison.GetYaxis().SetRangeUser(0,1.2*YMax)
                else: #axis crosses y=0
                    if abs(YMax) > abs(YMin):
                        Comparison.GetYaxis().SetRangeUser(-1.2*YMax,1.2*YMax)
                    else:
                        Comparison.GetYaxis().SetRangeUser(-1.2*YMin,1.2*YMin)

            Comparison.GetYaxis().SetNdivisions(205)
            Comparisons.append(Comparison)

        option = "E0"
        for index,Comparison in enumerate(Comparisons):
            if index == 0:
                option += " SAME"
            Comparison.Draw(option)

    outputFile.cd(histogramDirectory)
    Canvas.Write()
    if arguments.verbose:
        print "  Finished writing canvas: ", Canvas.GetName()

    if arguments.savePDFs:
        Canvas.SaveAs("comparison_histograms_pdfs/"+histogramName+".pdf")



def MakeOnePlot(histDir, histName):
    if arguments.makeSignificancePlots:
        MakeOneDHist(histDir, histName,"left")
        MakeOneDHist(histDir, histName,"right")
    else:
        MakeOneDHist(histDir, histName,"none")


def LoopOverKeys(currentDir, testFile, outputFile):
    testFile.cd(currentDir)
    for key in gDirectory.GetListOfKeys():  # Loop over directories
        if re.match ('TH1', key.GetClassName()):  #found a 1D histogram
            MakeOnePlot(currentDir, key.GetName())

        if (key.GetClassName() == "TDirectoryFile"):
            if arguments.verbose:
                print "Looping over directory: ", key.GetName()
            if currentDir == "":
                histDir = key.GetName()
            else:
                histDir = currentDir + "/" + key.GetName()
            outputFile.cd()
            gDirectory.mkdir(histDir)
            outputFile.cd(histDir)
            LoopOverKeys(histDir, testFile, outputFile)


##########################################################################################################################################
##########################################################################################################################################
##########################################################################################################################################


#### make output file
outputFileName = "comparison_histograms.root"
if arguments.outputFileName:
    outputFileName = arguments.outputFileName

outputFile = TFile(outputFileName, "RECREATE")

first_input = input_sources[0]

#### use the first input file as a template and make comparison versions of all its histograms
testFile = TFile("condor/" + first_input['condor_dir'] + "/" + first_input['dataset'] + ".root")
channelDirectory = ""
if arguments.generic:
    channelDirectory = first_input['channel']
else:
    channelDirectory = first_input['channel'] + "Plotter"

testFile.cd(channelDirectory)

if arguments.savePDFs:
    try:
        shutil.rmtree ("comparison_histograms_pdfs")
    except OSError:
        pass
    os.mkdir ("comparison_histograms_pdfs")


if arguments.generic:
    outputFile.cd()
    gDirectory.mkdir(channelDirectory)
    LoopOverKeys(channelDirectory, testFile, outputFile)
else:
    for key in gDirectory.GetListOfKeys():  # Loop over directories in same way as in makePlots.py
        if re.match ('TH1', key.GetClassName()):  #found a 1D histogram
            MakeOnePlot("", key.GetName())

        if (key.GetClassName() != "TDirectoryFile"):
            continue
        if arguments.verbose:
            print "Checking key: ", key.GetName()

        histogramDirectory = key.GetName()
        outputFile.cd()
        gDirectory.mkdir(histogramDirectory)
        outputFile.cd(key.GetName())
        testFile.cd(channelDirectory + "/" + histogramDirectory)
        for key2 in gDirectory.GetListOfKeys():
            if re.match ('TH1', key2.GetClassName()):  #found a 1D histogram
                MakeOnePlot(histogramDirectory, key2.GetName())

testFile.Close()
outputFile.Close()
print "Finished writing " + outputFile.GetName()
