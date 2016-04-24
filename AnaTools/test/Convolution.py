#!/usr/bin/env python

import time
import os
import sys
import math
import copy
import re
from array import *
import numpy as np 
from optparse import OptionParser
from OSUT3Analysis.Configuration.processingUtilities import *
from ROOT import *

outPutFile = TFile("Convolution.root","RECREATE")

parser = OptionParser()
parser.add_option("-l", "--localConfig", dest="localConfig",
                  help="local configuration file")

(arguments, args) = parser.parse_args()

if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.localConfig) + " import *")
else:
    print "No local config specified, shame on you"
    sys.exit(0)

GausHist = TH1D("Gaus","Gaus",GausBins[0],GausBins[1],GausBins[2])
GausHist.SetDirectory(0)
PoisHist = TH1D("Pois","Pois",PoisBins[0],PoisBins[1],PoisBins[2])
PoisHist.SetDirectory(0)
ConvHist = TH1D("Conv","Conv",ConvBins[0],ConvBins[1],ConvBins[2])
ConvHist.SetDirectory(0)

for i in range(0,numTrial):
    GausHist.Fill(np.random.normal(mu,sigma))
    PoisHist.Fill(np.random.poisson(Lam))
    ConvHist.Fill(np.random.poisson(Lam) + np.random.normal(mu,sigma))

print "The mean value is " + str(ConvHist.GetMean())
meanBin = ConvHist.FindBin(ConvHist.GetMean())
rightInt = ConvHist.Integral(meanBin, ConvHist.GetNbinsX() + 1)
for n in range(meanBin, ConvHist.GetNbinsX() + 1):
    if ConvHist.Integral(meanBin, n)/rightInt >= CLs:
        print str(CLs) + "% Upper Limit is " + str(ConvHist.GetBinCenter(n-1))
        break
leftInt = ConvHist.Integral(0, meanBin-1)
for n in range(0, meanBin):
    if ConvHist.Integral(0, n)/leftInt >= 1 - CLs:
        print str(CLs) + "% Lower Limit is " + str(ConvHist.GetBinCenter(n-1))
        break

outPutFile.cd()
GausHist.Write()
PoisHist.Write()
ConvHist.Write()
