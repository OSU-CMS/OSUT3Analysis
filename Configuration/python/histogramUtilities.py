#!/usr/bin/env python3

# Utilities for histogram manipulation.

from array import *
import math
from ROOT import TFile, TH1F, TMath, Double, TH1D, TGraphAsymmErrors

def getEfficiency(passes, passesError, total, totalError):
    passesHist = TH1D ("passes", "", 1, 0.0, 1.0)
    totalHist = TH1D ("total", "", 1, 0.0, 1.0)

    passesHist.SetBinContent (1, passes)
    passesHist.SetBinError (1, passesError)
    totalHist.SetBinContent (1, total)
    totalHist.SetBinError (1, totalError)

    g = TGraphAsymmErrors (passesHist, totalHist)
    x = Double (0.0)
    y = Double (0.0)
    g.GetPoint (0, x, y)

    return (y, g.GetErrorYlow (0), g.GetErrorYhigh (0))

def getYield(sample,condor_dir,channel):
    dataset_file = "condor/%s/%s.root" % (condor_dir,sample)
    inputFile = TFile(dataset_file)
    cutFlowHistogram = inputFile.Get(channel + "/cutFlow")
    if not cutFlowHistogram:
        print("ERROR: didn't find cutflow histogram ", channel+str("/cutFlow"), "in file ", dataset_file)
        return 0
    yield_     = float(cutFlowHistogram.GetBinContent(cutFlowHistogram.GetNbinsX()))
    statError_ = float(cutFlowHistogram.GetBinError  (cutFlowHistogram.GetNbinsX()))

    inputFile.Close()
    return (yield_, statError_)


def getYieldInBin(sample,condor_dir,channel,ibin):
    dataset_file = "condor/%s/%s.root" % (condor_dir,sample)
    inputFile = TFile(dataset_file)
    cutFlowHistogram = inputFile.Get(channel + "/cutFlow")
    if not cutFlowHistogram:
        print("WARNING: didn't find cutflow histogram ", channel, "CutFlow in file ", dataset_file)
        return 0
    yield_     = float(cutFlowHistogram.GetBinContent(ibin))
    statError_ = float(cutFlowHistogram.GetBinError  (ibin))
    inputFile.Close()
    return (yield_, statError_)


def getFirstBinWithLabel(sample,condor_dir,channel,label):
    dataset_file = "condor/%s/%s.root" % (condor_dir,sample)
    inputFile = TFile(dataset_file)
    cutFlowHistogram = inputFile.Get(channel + "/cutFlow")
    if not cutFlowHistogram:
        print("WARNING: didn't find cutflow histogram ", channel, "CutFlow in file ", dataset_file)
        return 0
    # Get the appropriate bin
    ibin = -1
    for i in range(1, cutFlowHistogram.GetNbinsX()+1):
        if label in cutFlowHistogram.GetXaxis().GetBinLabel(i):
            ibin = i
            break
    if ibin < 0:
        print("ERROR:  could not find bin with label containing", label, "for channel", channel)
    inputFile.Close()
    return ibin

def getNumEvents(sample,condor_dir,channel):  # Use in place of getYield if the cutflow histogram is not available
    dataset_file = "condor/%s/%s.root" % (condor_dir,sample)
    inputFile = TFile(dataset_file)
    numEvtHistogram = inputFile.Get("OSUAnalysis/"+channel+"/numEvents")
    if not numEvtHistogram:
        print("WARNING: didn't find cutflow histogram ", channel, "CutFlow in file ", dataset_file)
        return 0
    statError_ = Double(0.0)
    yield_ = numEvtHistogram.IntegralAndError(1, numEvtHistogram.GetNbinsX(), statError_)
    inputFile.Close()
    return (yield_, statError_)

def getHistIntegral(sample,condor_dir,channel,hist,xlo,xhi):
    # Modeled on getHistIntegrals.py
    dataset_file = "condor/%s/%s.root" % (condor_dir,sample)
    inputFile = TFile(dataset_file)
    histogram = inputFile.Get(channel+"/"+hist)
    if not histogram:
        print("WARNING: didn't find histogram ", channel + "/" + hist, " in file ", dataset_file)
        return 0
    Nxbins = histogram.GetNbinsX()
    xmax   = histogram.GetBinContent(Nxbins)
    xloBin = histogram.GetXaxis().FindBin(float(xlo))
    xhiBin = histogram.GetXaxis().FindBin(float(xhi))
    if xhi > xmax:
        # print "xhi is outside the range of the histogram, will include all the overflow instead"
        xhiBin = histogram.GetXaxis().FindBin(float(xhi))
    intError = Double (0.0)
    integral = histogram.IntegralAndError(xloBin, xhiBin, intError)

    inputFile.Close()
    return (integral, intError)


# some fancy-ass code from Andrzej Zuranski to merge bins in the ratio plot until the error goes below some threshold
def ratioHistogram( dataHist, mcHist, dontRebinRatio=False, addOne=False, relErrMax=0.10):

    def groupR(group):
        Data,MC = [float(sum(hist.GetBinContent(i) for i in group)) for hist in [dataHist,mcHist]]
        if MC!=0:
            return (Data-MC)/MC
        else:
            return 0

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
    if ((dataHist.GetName().find("num") != -1 and dataHist.GetName().find("Primaryvertexs") == -1) or
        dataHist.GetName().find("CutFlow")  != -1 or
        dataHist.GetName().find("GenMatch") != -1 or
        dontRebinRatio):
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

    ratio.GetYaxis().SetTitle(("#frac{obs-exp}{exp}") if not addOne else ("obs/exp"))
    ratio.GetXaxis().SetLabelOffset(0.03)
    ratio.SetLineColor(1)
    ratio.SetMarkerColor(1)
    ratio.SetLineWidth(2)
    if addOne:
        # Add 1.0 to all bins, so when histograms are equal, the bin content is 0.0, rather than 1.0.
        for i in range(1, ratio.GetNbinsX()+1):
            ratio.SetBinContent(i, ratio.GetBinContent(i) + 1.0)
    return ratio



def getUpperLimit(sample,condor_dir,channel):
    dataset_file = "condor/%s/%s.root" % (condor_dir,sample)
    inputFile = TFile(dataset_file)
    cutFlowHistogram = inputFile.Get("OSUAnalysis/"+channel+"CutFlowUpperLimit")
    if not cutFlowHistogram:
        print("WARNING: didn't find cutflow histogram ", channel, "CutFlow in file ", dataset_file)
        return 0
    limit = float(cutFlowHistogram.GetBinContent(cutFlowHistogram.GetNbinsX()))
    inputFile.Close()
    return (limit)

def getTruthYield(sample,condor_dir,channel,truthParticle):
    dataset_file = "condor/%s/%s.root" % (condor_dir,sample)
    inputFile = TFile(dataset_file)
    matchHistogram = inputFile.Get("OSUAnalysis/"+channel+"/trackGenMatchId")
    if not matchHistogram:
        print("WARNING: didn't find match histogram ", channel, "/trackGenMatchId in file ", dataset_file)
        return 0

    idx = -1
    for i in range(1,matchHistogram.GetNbinsX()+1):
        if truthParticle == matchHistogram.GetXaxis().GetBinLabel(i):
            idx = i
    if idx < 0:
        print("Error:  could not find bin with label " + truthParticle)
        yield_ = -1
        statError_ = -1
    else:
        yield_     = float(matchHistogram.GetBinContent(idx))
        statError_ = float(matchHistogram.GetBinError  (idx))
    inputFile.Close()
    return (yield_, statError_)


# Find number of raw events that correspond to the weighted number num and weighted error err
# num = w * N
# err = w * sqrt(N)
# N = num^2 / err^2
# In data, w = 1, so N = num.
def getRawEvts(num, err):
    N = round(math.pow(num,2) / math.pow(err,2)) if err else 0
    return N

def getHist(sample,condor_dir,channel,hist):
    dataset_file = "condor/%s/%s.root" % (condor_dir,sample)
    inputFile = TFile(dataset_file)
    h0 = inputFile.Get(channel + "/" + hist)
    if not h0:
        print("ERROR [getHist]: didn't find histogram ", channel+str("/")+hist, "in file", dataset_file)
        return 0
    h = h0.Clone()
    h.SetDirectory(0)
    inputFile.Close()
    return h


