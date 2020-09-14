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
#parser.remove_option("--2D")
parser.remove_option("-y")

parser.add_option("-f", "--fancy", action="store_true", dest="makeFancy", default=False,
                  help="removes the title and replaces it with the official CMS plot heading")
parser.add_option("--dontRebinRatio", action="store_true", dest="dontRebinRatio", default=False,
                  help="don't do the rebinning of ratio plots")
parser.add_option("-E", "--ratioRelErrMax", dest="ratioRelErrMax",
                  help="maximum error used in rebinning the ratio histogram")
parser.add_option("-A", "--addOneToRatio", action='store_true', dest="addOneToRatio",
                  help="add one to the ratio so that data/MC is plotted")
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
parser.add_option("-v", "--verbose", action="store_true", dest="verbose", default=False,
                  help="verbose output")

(arguments, args) = parser.parse_args()


if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.localConfig) + " import *")  # Remove .py from end of config file first.



#### deal with conflicting arguments

if arguments.makeRatioPlots or arguments.makeDiffPlots:
    if len(input_sources) is not 2:
        print "You need to have exactly two input sources to produce ratio or difference plots, turning them off"
        arguments.makeRatioPlots = False
        arguments.makeDiffPlots = False

if arguments.makeRatioPlots and arguments.makeDiffPlots:
    print "You have requested both ratio and difference plots.  Will make just ratio plots instead"
    arguments.makeRatioPlots = False


from OSUT3Analysis.Configuration.histogramUtilities import ratioHistogram
from ROOT import TFile, gROOT, gStyle, gDirectory, TStyle, TH1F, TH2F, TCanvas, TString, TLegend, TLegendEntry, TIter, TKey, TPaveLabel, gPad, TGraphAsymmErrors, TEfficiency


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
gStyle.SetTitleYOffset(1.5)
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
topLeft_x_left    = 0.19
topLeft_y_bottom  = 0.83
topLeft_x_right   = 0.512673
topLeft_y_top     = 0.91
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
    'green'  : 419,
    'purple' : 875,
    'blue'   : 601,
    'yellow' : 401,
}

colorList = [
    'black',
    'red',
    'green',
    'purple',
    'blue',
    'yellow',
]


markers = {
    'circle'   : 20,
    'square'   : 21,
    'triangle' : 22,
}

fills = {
    'solid'  : 0,
    'hollow' : 4,
}

##########################################################################################################################################
##########################################################################################################################################
##########################################################################################################################################

def MakeOneHist(dirName, histogramName):

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

    #legend coordinates, empirically determined :-)
    x_left = 0.4
    x_right = 0.7
    y_min = 0.15
    y_max = 0.3

    Legend = TLegend(x_left,y_min,x_right,y_max)
    Legend.SetBorderSize(0)
    Legend.SetFillColor(0)
    Legend.SetFillStyle(0)

    Canvas = TCanvas(histogramName)
    Histograms = []
    HistogramClones = []
    NBins = []
    MaxXValues = []
    MinXValues = []
    LegendEntries = []

    colorIndex = 0

    for source in input_sources: # loop over different input sources in config file
        dataset_file = "condor/%s/%s.root" % (source['condor_dir'],source['dataset'])
        inputFile = TFile(dataset_file)
        NumHistogramObj = inputFile.Get(source['num_channel'] + "Plotter/" + dirName + "/" + histogramName)
        if 'condor_dir_den' in source:   # If specified, take the denominator histogram from a different condor directory.
            dataset_fileDen = "condor/%s/%s.root" % (source['condor_dir_den'],source['dataset'])
            inputFileDen = TFile(dataset_fileDen)
            DenHistogramObj = inputFileDen.Get(source['den_channel'] + "Plotter/" + dirName + "/" + histogramName)
        else:   # Default is to use the same condor directory
            DenHistogramObj = inputFile.Get(source['den_channel'] + "Plotter/" + dirName + "/" + histogramName)
        if not NumHistogramObj:
            print "WARNING:  Could not find histogram " + source['num_channel'] + "Plotter/" + dirName + "/" + histogramName + " in file " + dataset_file + ".  Will skip it and continue."
            return
        if not DenHistogramObj:
            print "WARNING:  Could not find histogram " + source['den_channel'] + "Plotter/" + dirName + "/" + histogramName + " in file " + dataset_file + ".  Will skip it and continue."
            return

        Histogram = NumHistogramObj.Clone()
        if Histogram.Class().InheritsFrom("TH2"):
            Histogram.SetName(Histogram.GetName() + "__" + source['dataset'])
        Histogram.SetDirectory(0)
        DenHistogram = DenHistogramObj.Clone()
        DenHistogram.SetDirectory(0)
        inputFile.Close()

        if arguments.rebinFactor:
            RebinFactor = int(arguments.rebinFactor)
            #don't rebin histograms which will have less than 5 bins or any gen-matching histograms
            if Histogram.GetNbinsX() >= RebinFactor*5 and Histogram.GetTitle().find("GenMatch") is -1 and not Histogram.Class().InheritsFrom("TH2"):
                Histogram.Rebin(RebinFactor)
                DenHistogram.Rebin(RebinFactor)

        xAxisLabel = Histogram.GetXaxis().GetTitle()
        unitBeginIndex = xAxisLabel.find("[")
        unitEndIndex = xAxisLabel.find("]")

        if unitBeginIndex is not -1 and unitEndIndex is not -1: #x axis has a unit
            yAxisLabel = "#epsilon_{ " + cutName + "} (" + str(Histogram.GetXaxis().GetBinWidth(1)) + " " + xAxisLabel[unitBeginIndex+1:unitEndIndex] + " width)"
        else:
            yAxisLabel = "#epsilon_{ " + cutName + "} (" + str(Histogram.GetXaxis().GetBinWidth(1)) + " width)"
        if arguments.normalizeToUnitArea:
            yAxisLabel = yAxisLabel + " (Unit Area Norm.)"

        #check if bin content is consistent
        TEfficiency.CheckConsistency(Histogram,DenHistogram)
        for i in range(1,Histogram.GetNbinsX()+1):
            if Histogram.GetBinContent(i) > DenHistogram.GetBinContent(i):
                DenHistogram.SetBinContent(i,Histogram.GetBinContent(i))

        #HistogramClone and HistogramClones only used for ratio plot
        HistogramClone = Histogram.Clone()
        HistogramClone.SetDirectory(0)
        HistogramClone.Divide(DenHistogram)
        Nbins = HistogramClone.GetNbinsX()
        MaxXValue = HistogramClone.GetXaxis().GetBinLowEdge(Nbins+1)
        MinXValue = HistogramClone.GetXaxis().GetBinLowEdge(1)

        #this Histogram becomes the main TEfficiency
        if Histogram.Class().InheritsFrom("TH2"):
            Histogram.Divide(DenHistogram)
        else:
            #using default methods (which give correct uncertainties)
            #see https://root.cern.ch/doc/master/classTEfficiency.html (c.f. section IV)
            Histogram = TEfficiency(Histogram,DenHistogram)

        if not arguments.makeFancy:
            fullTitle = Histogram.GetTitle()
            splitTitle = fullTitle.split(":")
            #    print splitTitle
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

        markerStyle = 20
        if 'marker' in source:
            markerStyle = markers[source['marker']]
        if 'fill' in source:
            markerStyle = markerStyle + fills[source['fill']]

        Histogram.SetMarkerStyle(markerStyle)

        Histogram.SetLineWidth(line_width)
        Histogram.SetFillStyle(0)

        LegendEntries.append(source['legend_entry'])
        Histograms.append(Histogram)
        HistogramClones.append(HistogramClone)
        NBins.append(Nbins)
        MaxXValues.append(MaxXValue)
        MinXValues.append(MinXValue)

    ### scaling histograms as per user's specifications
    for histogram in Histograms:
        if arguments.normalizeToUnitArea and histogram.Integral() > 0:
            histogram.Scale(1./histogram.Integral())


    ### formatting histograms and adding to legend
    legendIndex = 0
    for histogram in Histograms:
        Legend.AddEntry(histogram,LegendEntries[legendIndex],"LEP")
        legendIndex = legendIndex+1

    ### finding the maximum value of anything going on the canvas, so we know how to set the y-axis
    finalMax = 1.1
    if arguments.setYMax:
        finalMax = float(arguments.setYMax)

    ### Drawing histograms to canvas

    makeRatioPlots = arguments.makeRatioPlots
    makeDiffPlots = arguments.makeDiffPlots
    addOneToRatio = -1
    if arguments.addOneToRatio:
        addOneToRatio = arguments.addOneToRatio

    dontRebinRatio = -1
    if arguments.dontRebinRatio:
        dontRebinRatio = arguments.dontRebinRatio

    ratioRelErrMax = -1
    if arguments.ratioRelErrMax:
        ratioRelErrMax = arguments.ratioRelErrMax

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
    if arguments.plot_hist:
        plotting_options = "HIST"

    h = TH2F("h1","",NBins[0],MinXValues[0],MaxXValues[0],110,0.,finalMax)
    h.SetTitle(";"+xAxisLabel+";"+yAxisLabel)
    h.Draw()

    for histogram in Histograms:
        if histogram.Class().InheritsFrom("TH2"):
            histogram.SetTitle(histoTitle)
            histogram.Draw("colz")
            DatasetName = histogram.GetName()
            DatasetName = DatasetName[DatasetName.rfind('__')+2:]  # substring starting with the last underscore
            DatasetLabel = TPaveLabel(topLeft_x_left,topLeft_y_bottom,topLeft_x_right,topLeft_y_top,DatasetName,"NDC")
            DatasetLabel.SetBorderSize(0)
            DatasetLabel.SetFillColor(0)
            DatasetLabel.SetFillStyle(0)
            DatasetLabel.Draw()
            outputFile.cd()
            Canvas.SetName(histogram.GetName())
            Canvas.Write()

        else:
            if histogram.InheritsFrom("TEfficiency") and histCounter==0:
                plotting_options = "P SAME"
            histogram.SetTitle(histoTitle)
            histogram.Draw(plotting_options)

            if histogram.InheritsFrom("TH1"):
                histogram.SetMaximum(finalMax)
                histogram.SetMinimum(yAxisMin)

        if histCounter is 0:
            if histogram.InheritsFrom("TH1"):
                plotting_options = plotting_options + " SAME"
            elif histogram.InheritsFrom("TEfficiency"):
                plotting_options = "P" + " SAME"

        histCounter = histCounter + 1

    if histogram.Class().InheritsFrom("TH2"):
        return

    Legend.Draw()


    if arguments.makeFancy:
        HeaderLabel.Draw()
        LumiLabel.Draw()



    #drawing the ratio or difference plot if requested

    if makeRatioPlots or makeDiffPlots:
        Canvas.cd(2)
        if makeRatioPlots:
            makeRatio = functools.partial (ratioHistogram,HistogramClones[0],HistogramClones[1])
            if addOneToRatio != -1: # it gets initialized to this dummy value of -1
                makeRatio = functools.partial (makeRatio, addOne = bool (addOneToRatio))
            if ratioRelErrMax is not -1: # it gets initialized to this dummy value of -1
                makeRatio = functools.partial (makeRatio, relErrMax = float (ratioRelErrMax))
            if dontRebinRatio is True:
                makeRatio = functools.partial (makeRatio, dontRebinRatio)
            Comparison = makeRatio ()
        elif makeDiffPlots:
            Comparison = Histograms[0].Clone("diff")
            Comparison.Add(Histograms[1],-1)
            Comparison.SetTitle("")
            Comparison.GetYaxis().SetTitle("hist1-hist2")
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
        Comparison.Draw("E0")

    outputFile.cd(dirName)
    Canvas.Write()

    if arguments.savePDFs:
        Canvas.SaveAs("efficiency_histograms_pdfs/"+histogramName+".pdf")


##########################################################################################################################################
##########################################################################################################################################
##########################################################################################################################################


#### make output file
outputFileName = "efficiency_histograms.root"
if arguments.outputFileName:
    outputFileName = arguments.outputFileName

outputFile = TFile(outputFileName, "RECREATE")

outputConfigFile = outputFileName.replace(".root", ".py")
if os.path.exists(outputConfigFile):
    shutil.move (outputConfigFile, outputConfigFile.replace(".py", "-bkup.py"))  # make a backup copy of a previous config file
shutil.copy (arguments.localConfig, outputConfigFile)  # make a copy of the config file

first_input = input_sources[0]


#### use the first input file as a template and make comparison versions of all its histograms
testFile = TFile("condor/" + first_input['condor_dir'] + "/" + first_input['dataset'] + ".root")
if arguments.verbose:
    print "Opened testFile: ", testFile.GetName()
rootDirectory = first_input['num_channel'] + "Plotter"
testFile.cd(rootDirectory)
if arguments.verbose:
    print "Going to rootDirectory: ", rootDirectory

if arguments.savePDFs:
    try:
        shutil.rmtree ("efficiency_histograms_pdfs")
    except OSError:
        pass
    os.mkdir ("efficiency_histograms_pdfs")


for key in gDirectory.GetListOfKeys(): # Loop over directories in same way as in makePlots.py
    if arguments.verbose:
        print "  Checking key: ", key.GetName()

    if (key.GetClassName() != "TDirectoryFile"):
        continue

    level2Directory = rootDirectory + "/" + key.GetName()
    if arguments.verbose:
        print "  Checking directory: ", level2Directory
    outputFile.cd()
    outputFile.mkdir(key.GetName())
    outputFile.cd(key.GetName())
    testFile.cd(level2Directory)
    for key2 in gDirectory.GetListOfKeys():
        if re.match ('TH1', key2.GetClassName()): #found a 1D histogram
            if arguments.verbose:
                print "    Will make 1D histogram: ", key2.GetName()
            MakeOneHist(key.GetName(), key2.GetName())
        if arguments.draw2DPlots and re.match ('TH2', key2.GetClassName()):  # make 2D histograms
            if arguments.verbose:
                print "    Will make 2D histogram: ", key2.GetName()
            MakeOneHist(key.GetName(), key2.GetName())

testFile.Close()
outputFile.Close()
print "Finished writing " + outputFile.GetName()
