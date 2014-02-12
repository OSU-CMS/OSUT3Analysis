#!/usr/bin/env python
import sys
import os
import re
import time  
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
parser.add_option("-S", "--systematics", action="store_true", dest="includeSystematics", default=False,
                  help="also lists the systematic uncertainties")
parser.add_option("-s", "--signif", action="store_true", dest="makeSignificancePlots", default=False,		 
                  help="Make significance plots")	 
parser.add_option("-O", "--addOverUnderFlow", action="store_true", dest="addOverUnderFlow", default=False,		 
                  help="Add the overflow and underflow entries to the last and first bins")	 
(arguments, args) = parser.parse_args()


if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.localConfig) + " import *")

if arguments.includeSystematics:
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



##########################################################################################################################################
##########################################################################################################################################
##########################################################################################################################################

def getSystematicError(sample,channel):
    errorSquared = 0.0
    if types[sample] is "data":
        return 0.0
    if len(channel) is 0:
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
        input_file_path = os.environ['CMSSW_BASE'] + "/src/" + external_systematics_directory + "systematic_values__" + uncertainty + "__" + channel + ".txt"
        if not os.path.exists(input_file_path):
            print "WARNING: didn't find ",input_file_path
            input_file_path = os.environ['CMSSW_BASE'] + "/src/" + external_systematics_directory + "systematic_values__" + uncertainty + ".txt"
            if not os.path.exists(input_file_path):
                print "   skipping",uncertainty,"systematic for the",channel,"channel"
                return 0
            else:
                print "   using default",uncertainty,"systematic for the",channel,"channel"

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

# some fancy-ass code from Andrzej Zuranski to merge bins in the ratio plot until the error goes below some threshold
def ratioHistogram( dataHist, mcHist, relErrMax=0.1):

    if not dataHist:
        print "Error:  trying to run ratioHistogram but dataHist is invalid" 
        return

    if not mcHist:
        print "Error:  trying to run ratioHistogram but mcHist is invalid" 
        return

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

    #don't rebin the histograms of the number of a given object (except for the pileup ones)
    if ((dataHist.GetName().find("num") is not -1 and dataHist.GetName().find("Primaryvertexs") is -1) or
        dataHist.GetName().find("CutFlow")  is not -1 or
        dataHist.GetName().find("GenMatch") is not -1): 
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

    ratio.GetYaxis().SetTitle("#frac{Data-Bkgd}{Bkgd}")
    ratio.SetLineColor(1)
    ratio.SetLineWidth(2)
    return ratio

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
            binError = 4.0 * y * y * dx * dx + 4 * x * y * dx * dx + x * x * dy * dy + x * x * dx * dx;
            if x + y > 0:   # protect against division by 0
                binError /= 4.0 * (x + y) * (x + y) * (x + y);
                binError = sqrt (binError);
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


def MakeOneDHist(pathToDir,histogramName,integrateDir): 

    channel = pathToDir.lstrip(rootDirectory).lstrip('/')

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
    
    Stack = THStack("stack",histogramName)

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
    elif arguments.normalizeToData:
        NormText = "MC scaled to data"
        NormLabel.SetLabel(NormText)
        
        
    BgMCLegend = TLegend()
    BgTitle = BgMCLegend.AddEntry(0, "Data & Bkgd. MC", "H")
    BgTitle.SetTextAlign(22)
    BgTitle.SetTextFont(62)
    BgMCLegend.SetBorderSize(0)
    BgMCLegend.SetFillColor(0)
    BgMCLegend.SetFillStyle(0)
    SignalMCLegend = TLegend()
    SignalTitle = SignalMCLegend.AddEntry(0, "Signal MC", "H")
    SignalTitle.SetTextAlign(22)
    SignalTitle.SetTextFont(62)
    SignalMCLegend.SetBorderSize(0)
    SignalMCLegend.SetFillColor(0)
    SignalMCLegend.SetFillStyle(0)
    
    outputFile.cd(pathToDir)
    canvasName = histogramName
    if integrateDir is "left":
        canvasName += "_CumulativeLeft" 
    if integrateDir is "right":
        canvasName += "_CumulativeRight" 
    Canvas = TCanvas(canvasName)
    BgMCHistograms = []
    BgMCUncertainties = []
    BgMCLegendEntries = []
    SignalMCHistograms = []
    SignalMCLegendEntries = []
    DataHistograms = []
    DataLegendEntries = []


    
    
    backgroundIntegral = 0
    dataIntegral = 0
    scaleFactor = 1

    for sample in processed_datasets: # loop over different samples as listed in configurationOptions.py
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
            #don't rebin any gen-matching or cutflow histograms, or numObject type histograms
            if (Histogram.GetName().find("num") is -1 and
                Histogram.GetName().find("Primaryvertexs") is -1 and
                Histogram.GetName().find("CutFlow")  is -1 and
                Histogram.GetName().find("cutFlow")  is -1 and                
                Histogram.GetName().find("Selection")  is -1 and
                Histogram.GetName().find("selection")  is -1 and                
                Histogram.GetName().find("MinusOne")  is -1 and
                Histogram.GetName().find("minusOne")  is -1 and                                
                Histogram.GetName().find("GenMatch") is -1): 
                
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
        if integrateDir is "left": 
            yAxisLabel = "Yield, " + xAxisLabelVar + " < x"  
        if integrateDir is "right": 
            yAxisLabel = "Yield, " + xAxisLabelVar + " > x"  
        
        if not arguments.makeFancy:
            histoTitle = Histogram.GetTitle()
        else:
            histoTitle = ""


        legLabel = labels[sample]
        if (arguments.printYields):
            yieldHist = Histogram.Integral()
            legLabel = legLabel + " (%.1f)" % yieldHist

        Histogram = MakeIntegralHist(Histogram, integrateDir)  

        if (arguments.addOverUnderFlow):
            nbins = Histogram.GetNbinsX()
            Histogram.SetBinContent(1,     Histogram.GetBinContent(1)     + Histogram.GetBinContent(0))       # Add underflow 
            Histogram.SetBinContent(nbins, Histogram.GetBinContent(nbins) + Histogram.GetBinContent(nbins+1)) # Add overflow 
            # Set the errors to be the sum in quadrature
            Histogram.SetBinError(1,     math.sqrt(math.pow(Histogram.GetBinError(1),    2) + math.pow(Histogram.GetBinError(0),      2))) 
            Histogram.SetBinError(nbins, math.sqrt(math.pow(Histogram.GetBinError(nbins),2) + math.pow(Histogram.GetBinError(nbins+1),2)))

        if( types[sample] == "bgMC"):
            
            numBgMCSamples += 1
            backgroundIntegral += Histogram.Integral()
            
            Histogram.SetLineStyle(1)
            if(arguments.noStack):
                Histogram.SetFillStyle(0)
                Histogram.SetLineColor(colors[sample])
                Histogram.SetLineWidth(2)
            else:
                Histogram.SetFillStyle(1001)
                Histogram.SetFillColor(colors[sample])
                Histogram.SetLineColor(1)
                Histogram.SetLineWidth(1)

            BgMCLegendEntries.append(legLabel) 
            BgMCHistograms.append(Histogram)

            if arguments.includeSystematics:
                BgMCUncertainties.append(getSystematicError(sample,channel))
                    
        elif( types[sample] == "signalMC"):
            
            numSignalSamples += 1
            
            Histogram.SetFillStyle(0)
            Histogram.SetLineColor(colors[sample])
            Histogram.SetLineStyle(1)
            Histogram.SetLineWidth(2)
            if(arguments.normalizeToUnitArea and Histogram.Integral() > 0):
                Histogram.Scale(1./Histogram.Integral())
                
            SignalMCLegendEntries.append(legLabel)
            SignalMCHistograms.append(Histogram)
                    
        elif( types[sample] == "data" and not blindData):

            numDataSamples += 1
            dataIntegral += Histogram.Integral()
            
            Histogram.SetMarkerStyle(20)
            Histogram.SetMarkerSize(0.8)
            Histogram.SetFillStyle(0)
            Histogram.SetLineColor(colors[sample])
            Histogram.SetLineStyle(1)
            Histogram.SetLineWidth(2)
            if(arguments.normalizeToUnitArea and Histogram.Integral() > 0):
                Histogram.Scale(1./Histogram.Integral())

            DataLegendEntries.append(legLabel)
            DataHistograms.append(Histogram)
                    
    #scaling histograms as per user's specifications
    if dataIntegral > 0 and backgroundIntegral > 0:
        scaleFactor = dataIntegral/backgroundIntegral
    for bgMCHist in BgMCHistograms:
        if arguments.normalizeToData:
            bgMCHist.Scale(scaleFactor)

        if arguments.normalizeToUnitArea and not arguments.noStack and backgroundIntegral > 0:
            bgMCHist.Scale(1./backgroundIntegral)
        elif arguments.normalizeToUnitArea and arguments.noStack and bgMCHist.Integral() > 0:
            bgMCHist.Scale(1./bgMCHist.Integral())

        if not arguments.noStack:
            Stack.Add(bgMCHist)

                

    ### formatting data histograms and adding to legend
    legendIndex = 0
    for Histogram in DataHistograms:
        BgMCLegend.AddEntry(Histogram,DataLegendEntries[legendIndex],"LEP")
        legendIndex = legendIndex+1


    ### creating the histogram to represent the statistical errors on the stack
    if numBgMCSamples is not 0 and not arguments.noStack: 
        if arguments.includeSystematics:
            addSystematicError(BgMCHistograms[0],BgMCUncertainties[0])
        ErrorHisto = BgMCHistograms[0].Clone("errors")
        ErrorHisto.SetFillStyle(3001)
        ErrorHisto.SetFillColor(13)
        ErrorHisto.SetLineWidth(0)
        for index in range(1,len(BgMCHistograms)):
            if arguments.includeSystematics:
                addSystematicError(BgMCHistograms[index],BgMCUncertainties[index])
            ErrorHisto.Add(BgMCHistograms[index])

        if arguments.includeSystematics:
            BgMCLegend.AddEntry(ErrorHisto,"Stat. & Sys. Errors","F")
        else:
            BgMCLegend.AddEntry(ErrorHisto,"Stat. Errors","F")            


    ### formatting bgMC histograms and adding to legend
    legendIndex = numBgMCSamples-1
    for Histogram in reversed(BgMCHistograms):
        if(arguments.noStack):
            BgMCLegend.AddEntry(Histogram,BgMCLegendEntries[legendIndex],"L")
        else:
            BgMCLegend.AddEntry(Histogram,BgMCLegendEntries[legendIndex],"F")
        legendIndex = legendIndex-1


    ### formatting signalMC histograms and adding to legend
    legendIndex = 0
    for Histogram in SignalMCHistograms:
        SignalMCLegend.AddEntry(Histogram,SignalMCLegendEntries[legendIndex],"L")
        legendIndex = legendIndex+1


    ### finding the maximum value of anything going on the canvas, so we know how to set the y-axis
    finalMax = 0
    if numBgMCSamples is not 0 and not arguments.noStack:
        finalMax = ErrorHisto.GetMaximum() + ErrorHisto.GetBinError(ErrorHisto.GetMaximumBin())
    else:
        for bgMCHist in BgMCHistograms:
            if(bgMCHist.GetMaximum() > finalMax):
                finalMax = bgMCHist.GetMaximum()
    for signalMCHist in SignalMCHistograms:
        if(signalMCHist.GetMaximum() > finalMax):
            finalMax = signalMCHist.GetMaximum()
    for dataHist in DataHistograms:
        if(dataHist.GetMaximum() > finalMax):
            finalMax = dataHist.GetMaximum() + dataHist.GetBinError(dataHist.GetMaximumBin())
    finalMax = 1.15*finalMax
    if arguments.setYMax:  
        finalMax = float(arguments.setYMax)


    ### Drawing histograms to canvas

    outputFile.cd(pathToDir)        
        
    makeRatioPlots = arguments.makeRatioPlots
    makeDiffPlots = arguments.makeDiffPlots
    makeSignifPlots = arguments.makeSignificancePlots 

    yAxisMin = 0.0001
    if arguments.setYMin:
        yAxisMin = float(arguments.setYMin)

    if numBgMCSamples is 0 or numDataSamples is not 1:
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

    if numBgMCSamples is not 0: # the first thing to draw to the canvas is a bgMC sample

        if not arguments.noStack: # draw stacked background samples
            Stack.SetTitle(histoTitle)
            Stack.Draw("HIST")
            Stack.GetXaxis().SetTitle(xAxisLabel)
            Stack.GetYaxis().SetTitle(yAxisLabel)
            Stack.SetMaximum(finalMax)
            Stack.SetMinimum(yAxisMin)
            if makeRatioPlots or makeDiffPlots or makeSignifPlots:
                Stack.GetHistogram().GetXaxis().SetLabelSize(0)
            #draw shaded error bands
            ErrorHisto.Draw("A E2 SAME")
                
        else: #draw the unstacked backgrounds
            BgMCHistograms[0].SetTitle(histoTitle)
            BgMCHistograms[0].Draw("HIST")
            BgMCHistograms[0].GetXaxis().SetTitle(xAxisLabel)
            BgMCHistograms[0].GetYaxis().SetTitle(yAxisLabel)
            BgMCHistograms[0].SetMaximum(finalMax)
            BgMCHistograms[0].SetMinimum(yAxisMin)            
            for bgMCHist in BgMCHistograms:
                bgMCHist.Draw("A HIST SAME")

        for signalMCHist in SignalMCHistograms:
            signalMCHist.Draw("A HIST SAME")
        for dataHist in DataHistograms:
            dataHist.Draw("A E X0 SAME")

                                
    elif numSignalSamples is not 0: # the first thing to draw to the canvas is a signalMC sample 
        SignalMCHistograms[0].SetTitle(histoTitle)
        SignalMCHistograms[0].Draw("HIST")
        SignalMCHistograms[0].GetXaxis().SetTitle(xAxisLabel)
        SignalMCHistograms[0].GetYaxis().SetTitle(yAxisLabel)
        SignalMCHistograms[0].SetMaximum(finalMax)
        SignalMCHistograms[0].SetMinimum(yAxisMin)

        for signalMCHist in SignalMCHistograms:
            if(signalMCHist is not SignalMCHistograms[0]):
                signalMCHist.Draw("A HIST SAME")
        for dataHist in DataHistograms:
            dataHist.Draw("A E X0 SAME")


    elif(numDataSamples is not 0): # the first thing to draw to the canvas is a data sample
        DataHistograms[0].SetTitle(histoTitle)
        DataHistograms[0].Draw("E")
        DataHistograms[0].GetXaxis().SetTitle(xAxisLabel)
        DataHistograms[0].GetYaxis().SetTitle(yAxisLabel)
        DataHistograms[0].SetMaximum(finalMax)
        DataHistograms[0].SetMinimum(yAxisMin)
        for dataHist in DataHistograms:
            if(dataHist is not DataHistograms[0]):
                dataHist.Draw("A E X0 SAME")



    #legend coordinates, empirically determined :-)
    x_left = 0.6761745
    x_right = 0.9328859
    x_width = x_right - x_left
    y_max = 0.9
    entry_height = 0.05

    if(numBgMCSamples is not 0 or numDataSamples is not 0): #then draw the data & bgMC legend

        numExtraEntries = 1 # count the legend title
        BgMCLegend.SetX1NDC(x_left)
        if numBgMCSamples > 0:
            numExtraEntries = numExtraEntries + 1 # count the stat. errors entry
            
        BgMCLegend.SetY1NDC(y_max-entry_height*(numExtraEntries+numBgMCSamples+numDataSamples))
        BgMCLegend.SetX2NDC(x_right)
        BgMCLegend.SetY2NDC(y_max)
        BgMCLegend.Draw()

        if(numSignalSamples is not 0): #then draw the signalMC legend to the left of the other one
            SignalMCLegend.SetX1NDC(x_left-x_width)
            SignalMCLegend.SetY1NDC(y_max-entry_height*(1+numSignalSamples)) # add one for the title
            SignalMCLegend.SetX2NDC(x_left)
            SignalMCLegend.SetY2NDC(y_max)
            SignalMCLegend.Draw()

        if integrateDir is "left":
            # Move the legend to the other side so it does not overlap with the cumulative histogram.  
            BgMCLegend.SetX1NDC(topLeft_x_left + 0.05)
            BgMCLegend.SetX2NDC(topLeft_x_left + 0.05 + x_width)
            BgMCLegend.SetY1NDC(-0.05 + y_max-entry_height*(numExtraEntries+numBgMCSamples+numDataSamples))
            BgMCLegend.SetY2NDC(-0.05 + y_max)
            BgMCLegend.Draw()
            

    elif numSignalSamples is not 0: #draw the signalMC legend in the upper right corner
        SignalMCLegend.SetX1NDC(x_left)
        SignalMCLegend.SetY1NDC(y_max-entry_height*(1+numSignalSamples)) # add one for the title
        SignalMCLegend.SetX2NDC(x_right)
        SignalMCLegend.SetY2NDC(y_max)
        SignalMCLegend.Draw()


    # Deciding which text labels to draw and drawing them
    drawLumiLabel = False
    drawNormLabel = False
    offsetNormLabel = False
    drawHeaderLabel = False

    if not arguments.normalizeToUnitArea or numDataSamples > 0: #don't draw the lumi label if there's no data and it's scaled to unit area
        drawLumiLabel = True
        #move the normalization label down before drawing if we drew the lumi. label
        offsetNormLabel = True
    if arguments.normalizeToUnitArea or arguments.normalizeToData:
        drawNormLabel = True
    if arguments.makeFancy:
        drawHeaderLabel = True
        drawLumiLabel = False

    #now that flags are set, draw the appropriate labels

    if drawLumiLabel:
        LumiLabel.Draw()

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




    #drawing the ratio or difference plot if requested

    if (makeRatioPlots or makeDiffPlots or makeSignifPlots): 
        Canvas.cd(2)
        BgSum = Stack.GetStack().Last()
        if makeRatioPlots:
            if arguments.ratioRelErrMax:
                Comparison = ratioHistogram(DataHistograms[0],BgSum,arguments.ratioRelErrMax)
            else:
                Comparison = ratioHistogram(DataHistograms[0],BgSum)
        elif makeDiffPlots:
            Comparison = DataHistograms[0].Clone("diff")
            Comparison.Add(BgSum,-1)
            Comparison.SetTitle("")
            Comparison.GetYaxis().SetTitle("Data-Bkgd")
        elif makeSignifPlots:
            Comparisons = signifHistograms(BgSum,SignalMCHistograms)
            Comparison = Comparisons[0]  
            Comparison.SetTitle("")
            Comparison.GetYaxis().SetTitle("S/#sqrt{S+B}")
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
        elif makeDiffPlots or makeSignifPlots:  
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
        Comparison.Draw()
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
        

    Canvas.Write()
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
    elif arguments.normalizeToData:
        NormText = "MC scaled to data"
        NormLabel.SetLabel(NormText)

    BgMCLegend = TLegend(0.76,0.65,0.99,0.9)
    BgMCLegend.AddEntry (0, "Data & Bkgd. MC", "H").SetTextFont (62)
    BgMCLegend.SetBorderSize(0)
    BgMCLegend.SetFillColor(0)
    BgMCLegend.SetFillStyle(0)
    SignalMCLegend = TLegend(0.76,0.135,0.99,0.377)
    SignalMCLegend.AddEntry (0, "Signal MC", "H").SetTextFont (62)
    SignalMCLegend.SetBorderSize(0)
    SignalMCLegend.SetFillColor(0)
    SignalMCLegend.SetFillStyle(0)

    outputFile.cd(pathToDir)
    Canvas = TCanvas(histogramName)
    Canvas.SetRightMargin(0.2413793);
    BgMCHistograms = []
    SignalMCHistograms = []
    DataHistograms = []

    for sample in processed_datasets: # loop over different samples as listed in configurationOptions.py
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
            Histogram.SetMarkerSize(1.2)
            Histogram.SetFillColor(colors[sample])                    
            BgMCLegend.AddEntry(Histogram,labels[sample],"P").SetTextFont (42)
            BgMCHistograms.append(Histogram)
        
        elif( types[sample] == "signalMC"):

            numSignalSamples += 1
            Histogram.SetMarkerColor(colors[sample])
            Histogram.SetMarkerStyle(20)
            Histogram.SetMarkerSize(1.2)
            Histogram.SetFillColor(colors[sample])
            BgMCLegend.AddEntry(Histogram,labels[sample],"P").SetTextFont (42)
#            SignalMCLegend.AddEntry(Histogram,labels[sample],"P").SetTextFont (42)
            SignalMCHistograms.append(Histogram)

        elif( types[sample] == "data" and not blindData):

            numDataSamples += 1
            Histogram.SetMarkerColor(colors[sample])
            Histogram.SetMarkerStyle(34)
            Histogram.SetMarkerSize(1.2)
            Histogram.SetFillColor(colors[sample])
            BgMCLegend.AddEntry(Histogram,labels[sample],"P").SetTextFont (42)
            DataHistograms.append(Histogram)

    outputFile.cd(pathToDir)        

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

    if not arguments.normalizeToUnitArea or numDataSamples > 0: #don't draw the lumi label if there's no data and it's scaled to unit area
        drawLumiLabel = True
        #move the normalization label down before drawing if we drew the lumi. label
        offsetNormLabel = True
    if arguments.normalizeToUnitArea or arguments.normalizeToData:
        drawNormLabel = True
    if arguments.makeFancy:
        drawHeaderLabel = True
        drawLumiLabel = False

    #now that flags are set, draw the appropriate labels

    if drawLumiLabel:
        LumiLabel.Draw()

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
        BgMCLegend.Draw()
    if(numSignalSamples is not 0):
        SignalMCLegend.Draw()

    Canvas.Write()




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


#### make output file
outputFileName = "stacked_histograms.root"
if arguments.makeSignificancePlots:
    outputFileName = "stacked_histogramsSignif.root"  
if arguments.outputFileName:
    outputFileName = arguments.outputFileName

outputFile = TFile(condor_dir + "/" + outputFileName, "RECREATE")



#### use the first input file as a template and make stacked versions of all its histograms
inputFile = TFile(condor_dir + "/" + processed_datasets[0] + ".root")
inputFile.cd()
outputFile.cd()

if arguments.savePDFs:
    os.system("rm -rf %s/stacked_histograms_pdfs" % (condor_dir))
    os.system("mkdir %s/stacked_histograms_pdfs" % (condor_dir))
    

#get root directory in the first layer, generally "OSUAnalysis"
for key in inputFile.GetListOfKeys():
    if (key.GetClassName() != "TDirectoryFile"):
        continue
    rootDirectory = key.GetName()
    outputFile.mkdir(rootDirectory)
    if arguments.savePDFs:
        os.system("mkdir %s/stacked_histograms_pdfs/%s" % (condor_dir,plainTextString(rootDirectory)))

    #cd to root directory and look for histograms
    inputFile.cd(rootDirectory)
    for key2 in gDirectory.GetListOfKeys():

        if re.match ('TH1', key2.GetClassName()): # found a 1-D histogram
            if arguments.makeSignificancePlots:
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
                os.system("mkdir %s/stacked_histograms_pdfs/%s" % (condor_dir,plainTextString(level2Directory)))

            #####################################################
            ###  This layer is typically the "channels" layer ###
            #####################################################

            inputFile.cd(level2Directory)
            for key3 in gDirectory.GetListOfKeys():
                if re.match ('TH1', key3.GetClassName()): # found a 1-D histogram
                    if arguments.makeSignificancePlots:
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
                        os.system("mkdir %s/stacked_histograms_pdfs/%s" % (condor_dir,plainTextString(level3Directory)))

                    #################################################
                    ###  This layer is typically the "cuts" layer ###
                    #################################################

                    inputFile.cd(level3Directory)
                    for key3 in gDirectory.GetListOfKeys():
                        if re.match ('TH1', key3.GetClassName()): # found a 1-D histogram
                            if arguments.makeSignificancePlots:
                                MakeOneDHist(level3Directory,key3.GetName(),"left")
                                MakeOneDHist(level3Directory,key3.GetName(),"right")
                            else:
                                MakeOneDHist(level3Directory,key3.GetName(),"none")
                        elif re.match ('TH2', key3.GetClassName()) and arguments.draw2DPlots: # found a 2-D histogram
                            MakeTwoDHist(level3Directory,key3.GetName())


outputFile.Close()         
