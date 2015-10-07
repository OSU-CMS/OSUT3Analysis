#!/usr/bin/env python
import sys
import os
import re
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

parser.remove_option("-c")
parser.remove_option("-e")
parser.remove_option("-n")
parser.remove_option("--2D")
parser.remove_option("-y")

parser.add_option("-f", "--fancy", action="store_true", dest="makeFancy", default=False,
                  help="removes the title and replaces it with the official CMS plot heading")
parser.add_option("--dontRebinRatio", action="store_true", dest="dontRebinRatio", default=False,
                  help="don't do the rebinning of ratio plots")
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
parser.add_option("-v", "--verbose", action="store_true", dest="verbose", default=False,
                  help="verbose output")  


(arguments, args) = parser.parse_args()


if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.localConfig) + " import *")



#### deal with conflicting arguments

if arguments.makeRatioPlots or arguments.makeDiffPlots:
    if len(input_sources) is not 2:
        print "You need to have exactly two input sources to produce ratio or difference plots, turning them off"
        arguments.makeRatioPlots = False
        arguments.makeDiffPlots = False
        
if arguments.makeRatioPlots and arguments.makeDiffPlots:
    print "You have requested both ratio and difference plots.  Will make just ratio plots instead"
    arguments.makeRatioPlots = False


if arguments.makeSignificancePlots and arguments.makeRatioPlots:
    print "You have asked to make a ratio plot and significance plots. This is a very strange request.  Will skip making the ratio plot."
    arguments.makeRatioPlots = False
if arguments.makeSignificancePlots and arguments.makeDiffPlots:
    print "You have asked to make a difference plot and significance plots. This is a very strange request.  Will skip making the difference plot."
    arguments.makeDiffPlots = False

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
    LumiText = "L_{int} = " + str(intLumi) + " pb^{-1}"
    LumiText = "L_{int} = " + str.format('{0:.1f}', LumiInPb) + " pb^{-1}"
else:
    LumiInFb = intLumi/1000.
    LumiText = "L_{int} = " + str.format('{0:.1f}', LumiInFb) + " fb^{-1}"

#bestest place for lumi. label, in top left corner
topLeft_x_left    = 0.1375839
topLeft_x_right   = 0.4580537
topLeft_y_bottom  = 0.8479021
topLeft_y_top     = 0.9475524
topLeft_y_offset  = 0.035

#set the text for the fancy heading
HeaderText = "CMS Preliminary: " + LumiText + " at #sqrt{s} = 8 TeV"

#position for header
header_x_left    = 0.2181208
header_x_right   = 0.9562937
header_y_bottom  = 0.9479866
header_y_top     = 0.9947552


colors = {
    'black'  : 1,
    'red'    : 632,
    'green'  : 416,
    'purple' : 880,
    'blue'   : 600,
    'yellow' : 400,
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

# some fancy-ass code from Andrzej Zuranski to merge bins in the ratio plot until the error goes below some threshold
def ratioHistogram( dataHist, mcHist, relErrMax=0.10):

    # A group is a list of consecutive histogram bins
    def groupR(group):
        Data,MC = [float(sum(hist.GetBinContent(i) for i in group)) for hist in [dataHist,mcHist]]
        return (Data-MC)/MC if MC else 0
    
    def groupErr(group):
        Data,MC = [float(sum(hist.GetBinContent(i) for i in group)) for hist in [dataHist,mcHist]]
        dataErr2,mcErr2 = [sum(hist.GetBinError(i)**2 for i in group) for hist in [dataHist,mcHist]]
        if MC > 0 and Data > 0 and Data != MC:
            return abs(math.sqrt( (dataErr2+mcErr2)/(Data-MC)**2 + mcErr2/MC**2 ) * (Data-MC)/MC)
        else:
            return 0
        

    def regroup(groups):
        err,iG = max( (groupErr(g),groups.index(g)) for g in groups )
        if err < relErrMax or len(groups)<3 : return groups
        iH = max( [iG-1,iG+1], key = lambda i: groupErr(groups[i]) if 0<=i<len(groups) else -1 )
        iLo,iHi = sorted([iG,iH])
        return regroup(groups[:iLo] + [groups[iLo]+groups[iHi]] + groups[iHi+1:])

    #don't rebin the histograms of the number of a given object (except for the pileup ones)
    if ((dataHist.GetName().find("num") is not -1 and dataHist.GetName().find("Primaryvertexs") is -1) or
        dataHist.GetName().find("CutFlow")  is not -1 or
        dataHist.GetName().find("GenMatch") is not -1 or
        arguments.dontRebinRatio): 
        ratio = dataHist.Clone()
        ratio.Add(mcHist,-1)
        ratio.Divide(mcHist)
        ratio.SetTitle("")
    else:
        groups = regroup( [(i,) for i in range(1,1+dataHist.GetNbinsX())] )
        ratio = TH1F("ratio","",len(groups), array('d', [dataHist.GetBinLowEdge(min(g)) for g in groups ] + [dataHist.GetXaxis().GetBinUpEdge(dataHist.GetNbinsX())]) )
        for i,g in enumerate(groups) :
            ratio.SetBinContent(i+1,groupR(g))
            ratio.SetBinError(i+1,groupErr(g))

    ratio.GetYaxis().SetTitle("#frac{hist1-hist2}{hist2}")
    ratio.GetXaxis().SetLabelOffset(0.03)
    ratio.SetLineColor(1)
    ratio.SetMarkerColor(1)
    ratio.SetLineWidth(2)
    return ratio

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
    if integrateDir is "left":
        for i in range(0,nbins+1):  # start with underflow bin
            integral += hist.GetBinContent(i)
            error = math.sqrt(error*error + hist.GetBinError(i)*hist.GetBinError(i)) # sum errors in quadrature
            hist.SetBinContent(i, integral)
            hist.SetBinError  (i, error)
            # Then include overflow bin in the last bin
            hist.SetBinContent(nbins, hist.GetBinContent(nbins) + hist.GetBinContent(nbins+1))
            hist.SetBinError  (nbins, math.sqrt(hist.GetBinError(nbins)*hist.GetBinError(nbins) + hist.GetBinError(nbins+1)*hist.GetBinError(nbins+1)))
    elif integrateDir is "right":
        for i in xrange(nbins+1, 0, -1):  # start with overflow bin
            integral += hist.GetBinContent(i)
            error = math.sqrt(error*error + hist.GetBinError(i)*hist.GetBinError(i)) # sum errors in quadrature
            hist.SetBinContent(i, integral)
            hist.SetBinError  (i, error)
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
    HeaderLabel.SetBorderSize(0)
    HeaderLabel.SetFillColor(0)
    HeaderLabel.SetFillStyle(0)

    LumiLabel = TPaveLabel(topLeft_x_left,topLeft_y_bottom,topLeft_x_right,topLeft_y_top,LumiText,"NDC")
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

        if arguments.verbose:
            print "  Got histogram", Histogram.GetName(), "from file", dataset_file  
        if arguments.rebinFactor:
            RebinFactor = int(arguments.rebinFactor)
            #don't rebin histograms which will have less than 5 bins or any gen-matching histograms
            if Histogram.GetNbinsX() >= RebinFactor*5 and Histogram.GetTitle().find("GenMatch") is -1:
                Histogram.Rebin(RebinFactor)

        xAxisLabel = Histogram.GetXaxis().GetTitle()
        unitBeginIndex = xAxisLabel.find("[")
        unitEndIndex = xAxisLabel.find("]")
        xAxisLabelVar = xAxisLabel
        
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
        
        Histogram.SetLineWidth(line_width)
        Histogram.SetFillStyle(0)

        if arguments.normalizeToUnitArea and Histogram.Integral() > 0:
            Histogram.Scale(1./Histogram.Integral())

        Histogram = MakeIntegralHist(Histogram, integrateDir)
            
        LegendEntries.append(source['legend_entry']) 
        Histograms.append(Histogram)

            
    ### formatting histograms and adding to legend
    legendIndex = 0
    for histogram in Histograms:
        Legend.AddEntry(histogram,LegendEntries[legendIndex],"LEP")
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
    #makeSignifPlots = arguments.makeSignificancePlots
    
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
    drawHeaderLabel = False

    if arguments.makeFancy:
        drawHeaderLabel = True

    #now that flags are set, draw the appropriate labels

    if drawHeaderLabel:
        HeaderLabel.Draw()




    #drawing the ratio or difference plot if requested

    if makeRatioPlots or makeDiffPlots:
        Canvas.cd(2)
        if makeRatioPlots:
            if arguments.ratioRelErrMax:
                Comparison = ratioHistogram(Histograms[0],Histograms[1],float(arguments.ratioRelErrMax))
            else:
                Comparison = ratioHistogram(Histograms[0],Histograms[1])
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
            Comparison.GetYaxis().SetRangeUser(-1*RatioYRange, RatioYRange)
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
    os.system("rm -rf comparison_histograms_pdfs")
    os.system("mkdir comparison_histograms_pdfs")


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
