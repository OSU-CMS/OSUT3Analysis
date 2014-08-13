#!/usr/bin/env python
import sys
import os
import re
from optparse import OptionParser
from array import *
from decimal import *

from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *


##set default plotting options
line_width = 2
plotting_options = ""

parser = OptionParser()
parser = set_commandline_arguments(parser)

parser.remove_option("-c")
parser.remove_option("-n")
parser.remove_option("-e")
parser.remove_option("-d")
parser.remove_option("--2D")


parser.add_option("--hist", action="store_true", dest="plot_hist", default=False,
                  help="plot as hollow histograms instead of error bar crosses")
parser.add_option("--line-width", dest="line_width",
                  help="set line width (default is 2)")
parser.add_option("--pdf", action="store_true", dest="plot_savePdf", default=False,
                  help="save plot as pdf in addition")
    

(arguments, args) = parser.parse_args()

if arguments.localConfig:
    sys.path.insert(0,os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.localConfig) + " import *")


outputFileName = "simple_plot.root"
if arguments.outputFileName:
        outputFileName = arguments.outputFileName
        

if arguments.plot_hist:
    plotting_options = plotting_options + "HIST"


from ROOT import TFile, gROOT, gStyle, gDirectory, TStyle, THStack, TH1F, TCanvas, TString, TLegend, TArrow, THStack, TIter, TKey, TPaveLabel, gPad
sys.argv = []
gROOT.SetBatch()
gStyle.SetOptStat(0)
gStyle.SetCanvasBorderMode(0)
gStyle.SetPadBorderMode(0)
gStyle.SetPadColor(0)
gStyle.SetCanvasColor(0)
gStyle.SetTextFont(42)
gROOT.ForceStyle()

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
        if Data > 0 and MC > 0 and Data != MC:
            return abs(math.sqrt( (dataErr2+mcErr2)/(Data-MC)**2 + mcErr2/MC**2 ) * (Data-MC)/MC)
        else:
            return 0
        
    def regroup(groups):
        err,iG = max( (groupErr(g),groups.index(g)) for g in groups )
        if err < relErrMax or len(groups)<3 : return groups
        iH = max( [iG-1,iG+1], key = lambda i: groupErr(groups[i]) if 0<=i<len(groups) else -1 )
        iLo,iHi = sorted([iG,iH])
        return regroup(groups[:iLo] + [groups[iLo]+groups[iHi]] + groups[iHi+1:])
    
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



outputFile = TFile(outputFileName, "RECREATE")

datasets_needed = []
for histogram in input_histograms:
    if histogram['dataset'] not in datasets_needed:
        datasets_needed.append(histogram['dataset'])


Legend = TLegend(0.70,0.65,0.9,0.9)
Legend.SetBorderSize(0)
Legend.SetFillColor(0)
Legend.SetFillStyle(0)



finalMax = 0

Histograms = []
for histogram in input_histograms:

    fileName = "condor/" + histogram['condor_dir'] + "/" + histogram['dataset'] + ".root"
    if not os.path.exists(fileName):
        continue
    inputFile = TFile(fileName)
    if inputFile.IsZombie() or not inputFile.GetNkeys():
        continue
    
    Histogram = inputFile.Get("OSUAnalysis/"+histogram['channel']+"/"+histogram['name']).Clone()
    Histogram.SetDirectory(0)
    inputFile.Close()


    fullTitle = Histogram.GetTitle()
    splitTitle = fullTitle.split(":")
#    print splitTitle
    Histogram.SetTitle(splitTitle[1].lstrip(" "))

    Histogram.SetMarkerColor(histogram['color'])
    Histogram.SetLineColor(histogram['color'])
    Histogram.SetLineWidth(line_width)
    Histogram.SetFillStyle(0)
    if(arguments.normalizeToUnitArea and Histogram.Integral() > 0):
        Histogram.Scale(1./Histogram.Integral())
    if arguments.rebinFactor:
        RebinFactor = int(arguments.rebinFactor)
        if Histogram.GetNbinsX() >= RebinFactor*10:
            Histogram.Rebin(RebinFactor)


    currentMax = Histogram.GetMaximum()
    if currentMax > finalMax:
        finalMax = currentMax

    Legend.AddEntry(Histogram,histogram['legend_entry'],"L")
    Histograms.append(Histogram)


makeRatioPlots = arguments.makeRatioPlots
if len(Histograms) is not 2:
    makeRatioPlots = False
    
Canvas = TCanvas(re.sub (r".root$", r"", outputFileName))


if makeRatioPlots:
    Canvas.SetFillStyle(0)
    Canvas.Divide(1,2)
    Canvas.cd(1)
    gPad.SetPad(0,0.25,1,1)
    gPad.SetMargin(0.15,0.05,0.01,0.07)
    gPad.SetFillStyle(0)
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

counter = 0
for Histogram in Histograms:

    if makeRatioPlots or makeDiffPlots:
        Histogram.GetXaxis().SetLabelSize(0)

    if counter is 0:
        Histogram.SetMaximum(1.1*finalMax)
        Histogram.SetMinimum(0.0001)
        Histogram.Draw(plotting_options)
        
    else:
        Histogram.Draw(plotting_options+" SAME")
    counter = counter+1


Legend.Draw()
if arguments.normalizeToUnitArea:
    NormLabel = TPaveLabel(0.1,0.75,0.35,0.85,"Scaled to unit area","NDC")
    NormLabel.SetBorderSize(0)
    NormLabel.SetFillColor(0)
    NormLabel.SetFillStyle(0)
    NormLabel.Draw()

if makeRatioPlots:
    Canvas.cd(2)
    Comparison = ratioHistogram(Histograms[0],Histograms[1])
    Comparison.GetYaxis().CenterTitle()
    Comparison.GetYaxis().SetTitleSize(0.1)
    Comparison.GetYaxis().SetTitleOffset(0.5)
    Comparison.GetXaxis().SetTitleSize(0.15)
    Comparison.GetYaxis().SetLabelSize(0.1)
    Comparison.GetXaxis().SetLabelSize(0.15)
    RatioYRange = 1.15
    Comparison.GetYaxis().SetRangeUser(-1*RatioYRange, RatioYRange)
    Comparison.GetYaxis().SetNdivisions(205)
    Comparison.Draw("E0")
    

outputFile.cd()
Canvas.Write()

if arguments.plot_savePdf:
    pdfFileName = outputFileName.replace(".root", ".pdf")  
    Canvas.SaveAs(pdfFileName)
    print "Saved file:  " + pdfFileName  


outputFile.Close()         
print "Saved plot in file: " + outputFileName  
