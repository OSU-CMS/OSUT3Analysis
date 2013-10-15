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
parser.add_option("-P", "--parametricErrors", action="store_true", dest="parametricErrors", default=False,
                  help="calculate parametric errors and display on histograms")
parser.add_option("-Y", "--showFittedYields", action="store_true", dest="showFittedYields", default=False,
                  help="show yields of fitted samples instead of ratios")



(arguments, args) = parser.parse_args()


if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + arguments.localConfig.rstrip('.py') + " import *")

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


from ROOT import TFile, gROOT, gStyle, gDirectory, TStyle, THStack, TH1F, TCanvas, TString, TLegend, TLegendEntry, THStack, TIter, TKey, TPaveLabel, TPaveText, TF1, gPad


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

# some fancy-ass code from Andrzej Zuranski to merge bins in the ratio plot until the error goes below some threshold
def ratioHistogram( dataHist, mcHist, relErrMax=0.10):

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

    ratio.GetYaxis().SetTitle("#frac{Data-MC}{MC}")
    ratio.SetLineColor(1)
    ratio.SetLineWidth(2)
    return ratio

##########################################################################################################################################
##########################################################################################################################################
##########################################################################################################################################


def MakeOneDHist(pathToDir,distribution):

    numFittingSamples = 0

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

    YieldsLabel = TPaveText(0.39, 0.7, 0.59, 0.9,"NDC")
    YieldsLabel.SetBorderSize(0)
    YieldsLabel.SetFillColor(0)
    YieldsLabel.SetFillStyle(0)
    YieldsLabel.SetTextAlign(12)

    RatiosLabel = TPaveText()
    RatiosLabel.SetDrawOption("NDC")
    RatiosLabel.SetBorderSize(0)
    RatiosLabel.SetFillColor(0)
    RatiosLabel.SetFillStyle(0)
    RatiosLabel.SetTextAlign(32)


    Legend = TLegend()
    Legend.SetBorderSize(0)
    Legend.SetFillColor(0)
    Legend.SetFillStyle(0)


    fittingIntegral = 0
    scaleFactor = 1

    HistogramsToFit = []
    TargetDataset = distribution['target_dataset']

    FittingLegendEntries = []
    DataLegendEntries = []

    FittingHistogramDatasets = []


    Stack_list = []
    Stack_list.append (THStack("stack_before",distribution['name']))
    Stack_list.append (THStack("stack_after",distribution['name']))

    fileName = condor_dir + "/" + distribution['target_dataset'] + ".root"
    if not os.path.exists(fileName):
        return
    inputFile = TFile(fileName)
    if inputFile.IsZombie() or not inputFile.GetNkeys():
        return



    Target = inputFile.Get("OSUAnalysis/"+distribution['channel']+"/"+distribution['name']).Clone()
    Target.SetDirectory(0)
    inputFile.Close()

    Target.SetMarkerStyle(20)
    Target.SetMarkerSize(0.8)
    Target.SetFillStyle(0)
    Target.SetLineColor(colors[TargetDataset])
    Target.SetLineStyle(1)
    Target.SetLineWidth(2)
    targetIntegral = Target.Integral()
    if(arguments.normalizeToUnitArea and Target.Integral() > 0):
        Target.Scale(1./Target.Integral())
    if arguments.rebinFactor:
        RebinFactor = int(arguments.rebinFactor)
        #don't rebin histograms which will have less than 5 bins or any gen-matching histograms
        if Target.GetNbinsX() >= RebinFactor*5 and Target.GetName().find("GenMatch") is -1:
            Target.Rebin(RebinFactor)


    ### formatting target histogram and adding to legend
    legendIndex = 0
    Legend.AddEntry(Target,labels[TargetDataset],"LEP")
    legendIndex = legendIndex+1

    if not outputFile.Get ("OSUAnalysis"):
        outputFile.mkdir ("OSUAnalysis")
    if not outputFile.Get ("OSUAnalysis/" + distribution['channel']):
        outputFile.Get ("OSUAnalysis").mkdir (distribution['channel'])

    for sample in distribution['datasets']: # loop over different samples requested to be fit

        dataset_file = "%s/%s.root" % (condor_dir,sample)
        inputFile = TFile(dataset_file)
        HistogramObj = inputFile.Get(pathToDir+"/"+distribution['channel']+"/"+distribution['name'])
        if not HistogramObj:
            print "WARNING:  Could not find histogram " + pathToDir + "/" + distribution['name'] + " in file " + dataset_file + ".  Will skip it and continue."
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
        unitBeginIndex = xAxisLabel.find("[")
        unitEndIndex = xAxisLabel.find("]")

        if unitBeginIndex is not -1 and unitEndIndex is not -1: #x axis has a unit
            yAxisLabel = "Entries / " + str(Histogram.GetXaxis().GetBinWidth(1)) + " " + xAxisLabel[unitBeginIndex+1:unitEndIndex]
        else:
            yAxisLabel = "Entries per bin (" + str(Histogram.GetXaxis().GetBinWidth(1)) + " width)"

        if not arguments.makeFancy:
            histoTitle = Histogram.GetTitle()
        else:
            histoTitle = ""


        legLabel = labels[sample]
        if (arguments.printYields):
            yieldHist = Histogram.Integral()
            legLabel = legLabel + " (%.1f)" % yieldHist
        FittingLegendEntries.append(legLabel)

        if( types[sample] == "bgMC"):

            numFittingSamples += 1
            fittingIntegral += Histogram.Integral()

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

        elif( types[sample] == "signalMC"):

            numFittingSamples += 1

            Histogram.SetFillStyle(0)
            Histogram.SetLineColor(colors[sample])
            Histogram.SetLineStyle(1)
            Histogram.SetLineWidth(2)
        if(arguments.normalizeToUnitArea and Histogram.Integral() > 0):
            Histogram.Scale(1./Histogram.Integral())

        HistogramsToFit.append(Histogram)
        FittingHistogramDatasets.append(sample)

    #scaling histograms as per user's specifications
    if targetIntegral > 0 and fittingIntegral > 0:
        scaleFactor = targetIntegral/fittingIntegral
    for fittingHist in HistogramsToFit:
        if arguments.normalizeToData:
            fittingHist.Scale(scaleFactor)

        if arguments.normalizeToUnitArea and not arguments.noStack and fittingIntegral > 0:
            fittingHist.Scale(1./fittingIntegral)
        elif arguments.normalizeToUnitArea and arguments.noStack and fittingHist.Integral() > 0:
            fittingHist.Scale(1./fittingHist.Integral())


    def fitf (x, par):
        xBin = HistogramsToFit[0].FindBin (x[0])
        value = 0.0

        for i in range (0, len (HistogramsToFit)):
            value += par[i] * HistogramsToFit[i].GetBinContent (xBin) + par[i + len (HistogramsToFit)] * HistogramsToFit[i].GetBinError (xBin)

        return value


    lowerLimit = Target.GetBinLowEdge (1)
    upperLimit = Target.GetBinLowEdge (Target.GetNbinsX ()) + Target.GetBinWidth (Target.GetNbinsX ())
    if 'lowerLimit' in distribution:
        lowerLimit = distribution['lowerLimit']
    if 'upperLimit' in distribution:
        upperLimit = distribution['upperLimit']
    func = TF1 ("fit", fitf, lowerLimit, upperLimit, 2 * len (HistogramsToFit))

    for i in range (0, len (HistogramsToFit)):
        if 'fixed_datasets' in distribution and distribution['datasets'][i] in distribution['fixed_datasets']:
            func.FixParameter (i, 1.0)
        else:
            func.SetParameter (i, 1.0)
            func.SetParLimits (i, 0.0, 1.0e2)
        func.SetParName (i, labels[FittingHistogramDatasets[i]])

    parErrorRanges = {}
    if arguments.parametricErrors:
        for i in range (0, len (HistogramsToFit)):
            for j in [-1, 1]:
                for k in range (len (HistogramsToFit), 2 * len (HistogramsToFit)):
                    func.FixParameter (k, 0)
                func.FixParameter (i + len (HistogramsToFit), j)
                for k in range (0, distribution['iterations'] - 1):
                    if j == -1:
                        print "Scale down " + labels[FittingHistogramDatasets[i]] + " iteration " + str (k + 1) + "..."
                    if j == 1:
                        print "Scale up " + labels[FittingHistogramDatasets[i]] + " iteration " + str (k + 1) + "..."
                    Target.Fit ("fit", "QEMR0")
                Target.Fit ("fit", "VEMR0")
                if j == -1:
                    parErrorRanges[labels[FittingHistogramDatasets[i]]] = [func.GetParameter (i)]
                if j == 1:
                    parErrorRanges[labels[FittingHistogramDatasets[i]]].append (func.GetParameter (i))

    for i in range (len (HistogramsToFit), 2 * len (HistogramsToFit)):
        func.FixParameter (i, 0)
    for i in range (0, distribution['iterations'] - 1):
        print "Iteration " + str (i + 1) + "..."
        Target.Fit ("fit", "QEMR0")
    Target.Fit ("fit", "VEMR0")

    finalMax = 0
    if not arguments.noStack:
        for fittingHist in HistogramsToFit:
            finalMax += fittingHist.GetMaximum()
        else:
            for fittingHist in HistogramsToFit:
                if(fittingHist.GetMaximum() > finalMax):
                    finalMax = fittingHist.GetMaximum()
    if(Target.GetMaximum() > finalMax):
        finalMax = Target.GetMaximum()

    Target.SetMaximum(1.1*finalMax)
    Target.SetMinimum(0.0001)

    Canvas = TCanvas(distribution['name'] + "_FitFunction")
    Canvas.cd (1)
    Target.Draw ()
    func.Draw ("same")

    outputFile.cd ("OSUAnalysis/" + distribution['channel'])
    Canvas.Write ()
    if arguments.savePDFs:
        if histogram == input_histograms[0]:
            Canvas.Print (pdfFileName + "(", "pdf")
        else:
            Canvas.Print (pdfFileName, "pdf")
    Target.SetStats (0)




    ### formatting bgMC histograms and adding to legend
    legendIndex = numFittingSamples-1
    for Histogram in reversed(HistogramsToFit):
        if(arguments.noStack):
            Legend.AddEntry(Histogram,FittingLegendEntries[legendIndex],"L")
        else:
            Legend.AddEntry(Histogram,FittingLegendEntries[legendIndex],"F")
        legendIndex = legendIndex-1


    ### Drawing histograms to canvas

    makeRatioPlots = arguments.makeRatioPlots
    makeDiffPlots = arguments.makeDiffPlots

    yAxisMin = 0.0001
    if arguments.setYMin:
        yAxisMin = float(arguments.setYMin)


    ### Draw everything to the canvases !!!!

    for i in range (0, 2): # 0 => before, 1 => after

        integrals = []
        ratios = []
        errors = []
        parErrors = []

        if i == 1:
            for j in range (0, len (HistogramsToFit)):

                integrals.append(HistogramsToFit[j].Integral())
                HistogramsToFit[j].Scale (func.GetParameter (j))
                ratios.append(func.GetParameter (j))
                errors.append(func.GetParError(j))
                if arguments.parametricErrors:
                    scaleDown = parErrorRanges[labels[FittingHistogramDatasets[j]]][0]
                    scaleUp = parErrorRanges[labels[FittingHistogramDatasets[j]]][1]
                    parErrors.append (abs (scaleUp - scaleDown))

        for fittingHist in HistogramsToFit:
            if not arguments.noStack:
                Stack_list[i].Add(fittingHist)


        #creating the histogram to represent the statistical errors on the stack
        if not arguments.noStack:
            ErrorHisto = HistogramsToFit[0].Clone("errors")
            ErrorHisto.SetFillStyle(3001)
            ErrorHisto.SetFillColor(13)
            ErrorHisto.SetLineWidth(0)
            if i == 1:
                Legend.AddEntry(ErrorHisto,"Stat. Errors","F")
            for Histogram in HistogramsToFit:
                if Histogram is not HistogramsToFit[0]:
                    ErrorHisto.Add(Histogram)

        if i == 0:
            Canvas = TCanvas(distribution['name'] + "_Before")
        if i == 1:
            Canvas = TCanvas(distribution['name'] + "_After")

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
            # format: gPad.SetMargin(l,r,b,t)
            gPad.SetMargin(0.15,0.05,0.4,0.01)
            gPad.SetFillStyle(0)
            gPad.SetGridy(1)
            gPad.Update()
            gPad.Draw()

            Canvas.cd(1)

        ### finding the maximum value of anything going on the canvas, so we know how to set the y-axis
        finalMax = 0
        if numFittingSamples is not 0 and not arguments.noStack:
            finalMax = ErrorHisto.GetMaximum() + ErrorHisto.GetBinError(ErrorHisto.GetMaximumBin())
        else:
            for bgMCHist in HistogramsToFit:
                if(bgMCHist.GetMaximum() > finalMax):
                    finalMax = bgMCHist.GetMaximum()
        if(Target.GetMaximum() > finalMax):
            finalMax = Target.GetMaximum() + Target.GetBinError(Target.GetMaximumBin())
        finalMax = 1.15*finalMax
        if arguments.setYMax:
            finalMax = float(arguments.setYMax)


        if not arguments.noStack: # draw stacked background samples
            Stack_list[i].SetTitle(histoTitle)
            Stack_list[i].Draw("HIST")
            Stack_list[i].GetXaxis().SetTitle(xAxisLabel)
            Stack_list[i].GetYaxis().SetTitle(yAxisLabel)
            Stack_list[i].SetMaximum(finalMax)
            Stack_list[i].SetMinimum(yAxisMin)
            if makeRatioPlots or makeDiffPlots:
                Stack_list[i].GetHistogram().GetXaxis().SetLabelSize(0)
            #draw shaded error bands
            ErrorHisto.Draw("A E2 SAME")

        else: #draw the unstacked backgrounds
            HistogramsToFit[0].SetTitle(histoTitle)
            HistogramsToFit[0].Draw("HIST")
            HistogramsToFit[0].GetXaxis().SetTitle(xAxisLabel)
            HistogramsToFit[0].GetYaxis().SetTitle(yAxisLabel)
            HistogramsToFit[0].SetMaximum(finalMax)
            HistogramsToFit[0].SetMinimum(yAxisMin)
            for bgMCHist in HistogramsToFit:
                bgMCHist.Draw("A HIST SAME")

        Target.Draw("A E X0 SAME")



        #legend coordinates, empirically determined :-)
        x_left = 0.6761745
        x_right = 0.9328859
        x_width = x_right - x_left
        y_max = 0.9
        entry_height = 0.05

        if(numFittingSamples is not 0): #then draw the data & bgMC legend

            numExtraEntries = 2 # count the target and (lack of) title
            Legend.SetX1NDC(x_left)
            numExtraEntries = numExtraEntries + 1 # count the stat. errors entry

            Legend.SetY1NDC(y_max-entry_height*(numExtraEntries+numFittingSamples))
            Legend.SetX2NDC(x_right)
            Legend.SetY2NDC(y_max)
            Legend.Draw()

            RatiosLabel.SetX1NDC(x_left - 0.1)
            RatiosLabel.SetX2NDC(x_right)
            RatiosLabel.SetY2NDC(Legend.GetY1NDC() - 0.1)
            RatiosLabel.SetY1NDC(RatiosLabel.GetY2NDC() - entry_height*(numFittingSamples))

            # Deciding which text labels to draw and drawing them
            drawLumiLabel = False
            drawNormLabel = False
            offsetNormLabel = False
            drawHeaderLabel = False

            if not arguments.normalizeToUnitArea: #don't draw the lumi label if there's no data and it's scaled to unit area
                drawLumiLabel = True
                # move the normalization label down before drawing if we drew the lumi. label
                offsetNormLabel = True
            if arguments.normalizeToUnitArea or arguments.normalizeToData:
                drawNormLabel = True
            if arguments.makeFancy:
                drawHeaderLabel = True
                drawLumiLabel = False

            # now that flags are set, draw the appropriate labels

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

            YieldsLabel.Clear()
            mcYield = Stack_list[i].GetStack().Last().Integral()
            dataYield = Target.Integral()
            if i == 0:
                YieldsLabel.AddText ("Before Fit to Data")
            if i == 1:
                YieldsLabel.AddText ("After Fit to Data")
            YieldsLabel.AddText ("data yield: " + '%.1f' % dataYield)
            YieldsLabel.AddText ("MC yield: " + '%.1f' % mcYield)
            YieldsLabel.AddText ("data/MC: " + '%.2f' % (dataYield/mcYield))
            if i == 1:
                for j in range(0,len(FittingLegendEntries)):
                    if abs(ratios[j]-1) < 0.001 and abs(errors[j]) < 0.001: #then it probably was held fixed
                        continue
                    if arguments.showFittedYields:
                        yield_ = ratios[j]*integrals[j]
                        yielderror_ = errors[j]*yield_
                        text = FittingLegendEntries[j]+" yield: " + '%.0f' % yield_ + ' #pm %.0f' % yielderror_
                    else:
                        text = FittingLegendEntries[j]+" ratio: " + '%.2f' % ratios[j] + ' #pm %.2f' % errors[j]
                    text = text + " (stat)"
                    if arguments.parametricErrors:
                        yield_ = ratios[j]*integrals[j]
                        yieldParError_ = parErrors[j]*yield_
                        if arguments.showFittedYields:
                            text += ' #pm %.2f' % yieldParError_
                        else:
                            text += ' #pm %.2f' % parErrors[j]
                        text = text + " (sys)"
                    RatiosLabel.AddText (text)
            YieldsLabel.Draw()
            RatiosLabel.Draw()

        # drawing the ratio or difference plot if requested
        if (makeRatioPlots or makeDiffPlots):
            Canvas.cd(2)
            BgSum = Stack_list[i].GetStack().Last()
            if makeRatioPlots:
                if arguments.ratioRelErrMax:
                    Comparison = ratioHistogram(Target,BgSum,arguments.ratioRelErrMax)
                else:
                    Comparison = ratioHistogram(Target,BgSum)
            elif makeDiffPlots:
                Comparison = Target.Clone("diff")
                Comparison.Add(BgSum,-1)
                Comparison.SetTitle("")
                Comparison.GetYaxis().SetTitle("Data-MC")
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
            Comparison.Draw()



        if i == 0:
            Canvas.Write (distribution['name'] + "_Before")
            if arguments.savePDFs:
                pathToDirString = plainTextString(pathToDir)
                Canvas.SaveAs(condor_dir+"/fitting_histogram_pdfs/"+pathToDirString+"/"+distribution['name']+"_Before.pdf")

        if i == 1:
            Canvas.Write (distribution['name'] + "_After")
            if arguments.savePDFs:
                pathToDirString = plainTextString(pathToDir)
                Canvas.SaveAs(condor_dir+"/fitting_histogram_pdfs/"+pathToDirString+"/"+distribution['name']+"_After.pdf")





##########################################################################################################################################
##########################################################################################################################################
##########################################################################################################################################


##########################################################################################################################################
##########################################################################################################################################
##########################################################################################################################################


condor_dir = set_condor_output_dir(arguments)


# make output file
outputFileName = "mc_fit_to_data.root"
if arguments.outputFileName:
    outputFileName = arguments.outputFileName

outputFile = TFile(condor_dir + "/" + outputFileName, "RECREATE")


if arguments.savePDFs:
    os.system("rm -rf %s/fitting_histograms_pdfs" % (condor_dir))
    os.system("mkdir %s/fitting_histograms_pdfs" % (condor_dir))


#get root directory in the first layer, generally "OSUAnalysis"
for distribution in input_distributions:
    MakeOneDHist("OSUAnalysis",distribution)

outputFile.Close()
