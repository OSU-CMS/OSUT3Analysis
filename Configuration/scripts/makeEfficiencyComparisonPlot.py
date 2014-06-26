#!/usr/bin/env python

### this script has been done to overlaid on the same canva multiple efficency's curves. It uses the asymmetryc errors. 



import sys
import os
import re
from optparse import OptionParser
from array import *
from decimal import *

from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *
import ROOT







##set default plotting options
line_width = 2
plotting_options = ""

parser = OptionParser()
parser = set_commandline_arguments(parser)

parser.remove_option("-c")
parser.remove_option("-n")
parser.remove_option("-e")
parser.remove_option("-r")
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


outputFileName = "efficiency_plot.root"
if arguments.outputFileName:
        outputFileName = arguments.outputFileName
        

if arguments.plot_hist:
    plotting_options = plotting_options + "HIST"


from ROOT import TFile, gROOT, gStyle, gDirectory, TStyle, THStack, TH1F, TCanvas, TString, TLegend, TArrow, THStack, TIter, TKey, TPaveLabel
sys.argv = []
gROOT.SetBatch()
gStyle.SetOptStat(0)
gStyle.SetCanvasBorderMode(0)
gStyle.SetPadBorderMode(0)
gStyle.SetPadColor(0)
gStyle.SetCanvasColor(0)
gStyle.SetTextFont(42)
gROOT.ForceStyle()


outputFile = TFile(outputFileName, "RECREATE")

datasets_needed = []
for histogram in input_histograms:
    if histogram['dataset'] not in datasets_needed:
        datasets_needed.append(histogram['dataset'])


Legend = TLegend(0.70,0.65,0.9,0.9)# two times 0.9 ??
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
    
    #    Histogram = inputFile.Get("OSUAnalysis/"+histogram['channel']+"/"+histogram['name']).Clone()
    
    Numerator = inputFile.Get("OSUAnalysis/"+histogram['channel_numerator']+"/"+histogram['name']).Clone()
    Denominator = inputFile.Get("OSUAnalysis/"+histogram['channel_denominator']+"/"+histogram['name']).Clone()
    
#    Numerator.Divide(Denominator)
    Histogram = ROOT.TGraphAsymmErrors(Numerator,Denominator)

#    Histogram.SetDirectory(0)
    #    Histogram.SetDirectory(0)
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


Canvas = TCanvas(re.sub (r".root$", r"", outputFileName))

counter = 0
for Histogram in Histograms:
    if Histogram.InheritsFrom("TGraph") and counter==0:
        plotting_options = "AP"
    Histogram.Draw(plotting_options)
#    Histogram.GetXaxis().SetTitle(xAxisLabel)
    Histogram.GetYaxis().SetTitle("cut efficiency")
    if counter is 0:
        if Histogram.InheritsFrom("TGraph"):
            plotting_options = "P"
    counter = counter+1 

Legend.Draw()
if arguments.normalizeToUnitArea:
    NormLabel = TPaveLabel(0.1,0.75,0.35,0.85,"Scaled to unit area","NDC")
    NormLabel.SetBorderSize(0)
    NormLabel.SetFillColor(0)
    NormLabel.SetFillStyle(0)
    NormLabel.Draw()

outputFile.cd()
Canvas.Write()

if arguments.plot_savePdf:
    pdfFileName = outputFileName.replace(".root", ".pdf")  
    Canvas.SaveAs(pdfFileName)
    print "Saved file:  " + pdfFileName  


outputFile.Close()         
print "Saved plot in file: " + outputFileName  
