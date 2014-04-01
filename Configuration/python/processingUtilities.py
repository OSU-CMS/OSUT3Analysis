#!/usr/bin/env python
import os
import re
import sys
import math
import datetime
import copy
from optparse import OptionParser
import OSUT3Analysis.DBTools.osusub_cfg as osusub
import FWCore.ParameterSet.Config as cms

def split_composite_datasets(datasets, composite_dataset_definitions):
    for dataset in datasets:
        if dataset in composite_dataset_definitions:
            for component_dataset in composite_dataset_definitions[dataset]:
                datasets.insert(datasets.index(dataset),component_dataset)
            datasets.remove(dataset)
    return datasets

def get_composite_datasets(datasets, composite_dataset_definitions):
    composite_datasets = []
    for dataset in datasets:
        if dataset in composite_dataset_definitions:
            composite_datasets.append(dataset)
    return composite_datasets

def set_condor_submit_dir(arguments):
    if arguments.condorDir:
        condor_dir = "condor/%s" % arguments.condorDir
    else:
        now = datetime.datetime.now()
        date_hash = now.strftime("%Y_%m_%d_%H:%M:%S")
        condor_dir = "condor/condor_%s" % date_hash
    #print "Condor submit directory set to ",condor_dir
    return condor_dir

def set_condor_output_dir(arguments):
    if arguments.condorDir:
        condor_dir = "condor/%s" % arguments.condorDir
    else: #get most recent condor submission directory
        dir_list = []
        for directory in os.listdir("./condor/"):
            if directory.find("condor_") is not -1:
                dir_list.append(directory)
        if len(dir_list) is 0:
            sys.exit("Cannot find last condor working directory")
        dir_list.sort(reverse=True)
        condor_dir = "condor/%s" % dir_list[0]
    #print "Condor output directory set to ",condor_dir
    return condor_dir

def set_commandline_arguments(parser):
    #### Configuration-related Options
    parser.add_option("-l", "--localConfig", dest="localConfig",
                  help="local configuration file")
    parser.add_option("-c", "--condorDir", dest="condorDir",
                      help="condor output directory")
    parser.add_option("-o", "--output-file", dest="outputFileName",
                      help="specify an output file name for the histogram file, default is 'stacked_histograms.root'")


    #### Histogram Formatting Options
    parser.add_option("-n", "--normalize", action="store_true", dest="normalizeToData", default=False,
                      help="normalize total background MC yield to the data")
    parser.add_option("-u", "--unit-area", action="store_true", dest="normalizeToUnitArea", default=False,
                      help="normalize all samples to unit area (useful to compare shapes)")
    parser.add_option("-e", "--empty", action="store_true", dest="noStack", default=False,
                      help="don't stack the background samples, draw them as empty histograms instead")

    parser.add_option("-r", "--ratio", action="store_true", dest="makeRatioPlots", default=False,
                      help="draw (data-MC)/MC plots below all 1D histograms")
    parser.add_option("-R", "--ratioYRange", dest="ratioYRange",
                      help="maximum of range of vertical scale for ratio plots")  
    parser.add_option("-d", "--diff", action="store_true", dest="makeDiffPlots", default=False,
                      help="draw data-MC plots below all 1D histograms")
    parser.add_option("-b", "--rebin", dest="rebinFactor",
                      help="Rebin all the histograms which will have at least 10 bins after rebinning")
    parser.add_option("--2D", action="store_true", dest="draw2DPlots", default=False,
                      help="draw stacked 2D histograms")
    parser.add_option("-y", "--yields", action="store_true", dest="printYields", default=False,
                      help="Include the yield of each source in the legend")
    parser.add_option("-p", "--pdfs",  action="store_true", dest="savePDFs", default=False,
                      help="Save pdfs files for all plots made")

    return parser

def get_short_name(dataset, dataset_names):
    for key in dataset_names:
        if dataset_names[key] == dataset:
            return key
    return "Unknown"

def stop_ctau (dataset):
    if not re.match (r"stop[^_]*to[^_]*_[^_]*mm.*", dataset):
      return 0.0
    return float (re.sub (r"stop[^_]*to[^_]*_([^_]*)mm.*", r"\1", dataset))

def source_stop_ctau (ctau):
    return max (int (math.pow (10.0, math.ceil (math.log10 (ctau)))), 1)

def add_stops (options, masses, ctaus, bottomBranchingRatios = []):
    if not bottomBranchingRatios:
        bottomBranchingRatios.append (50.0)
    for mass in masses:
        for ctau in ctaus:
            for bottomBranchingRatio in bottomBranchingRatios:
                datasetName = 'stop' + str (mass) + "_" + str (ctau) + "mm_br" + str (int (bottomBranchingRatio))
                bottomDatasetName = 'stop' + str (mass) + "toBl_" + str (ctau) + "mm"
                sourceBottomDatasetName = 'stop' + str (mass) + "toBl_" + str (source_stop_ctau (ctau)) + "mm"
                topDatasetName = 'stop' + str (mass) + "toTnu_" + str (ctau) + "mm"
                sourceTopDatasetName = 'stop' + str (mass) + "toTnu_" + str (source_stop_ctau (ctau)) + "mm"
                mixedDatasetName = 'stop' + str (mass) + "toBT_" + str (ctau) + "mm"
                sourceMixedDatasetName = 'stop' + str (mass) + "toBT_" + str (source_stop_ctau (ctau)) + "mm"

                options['datasets'].append (datasetName)
                bottomBranchingRatio /= 100.0
                options['composite_dataset_definitions'][datasetName] = {}
                if bottomBranchingRatio > 1.0e-6:
                    options['composite_dataset_definitions'][datasetName][bottomDatasetName] = bottomBranchingRatio * bottomBranchingRatio
                    options['dataset_names'][bottomDatasetName] = options['dataset_names'][sourceBottomDatasetName]
                if 1.0 - bottomBranchingRatio > 1.0e-6:
                    options['composite_dataset_definitions'][datasetName][topDatasetName] = (1.0 - bottomBranchingRatio) * (1.0 - bottomBranchingRatio)
                    options['dataset_names'][topDatasetName] = options['dataset_names'][sourceTopDatasetName]
                if bottomBranchingRatio > 1.0e-6 and 1.0 - bottomBranchingRatio > 1.0e-6:
                    options['composite_dataset_definitions'][datasetName][mixedDatasetName] = (1.0 - bottomBranchingRatio * bottomBranchingRatio - (1.0 - bottomBranchingRatio) * (1.0 - bottomBranchingRatio))
                    options['dataset_names'][mixedDatasetName] = options['dataset_names'][sourceMixedDatasetName]
                options['nJobs'][bottomDatasetName] = 100
                options['nJobs'][topDatasetName] = 100
                options['nJobs'][mixedDatasetName] = 100
                options['maxEvents'][bottomDatasetName] = -1
                options['maxEvents'][topDatasetName] = -1
                options['maxEvents'][mixedDatasetName] = -1
                options['types'][datasetName] = "signalMC"
                options['types'][bottomDatasetName] = "signalMC"
                options['types'][topDatasetName] = "signalMC"
                options['types'][mixedDatasetName] = "signalMC"
                options['labels'][datasetName] = str (mass) + " GeV stop (#LTc#tau#GT = " + str (ctau) + " mm)"
                options['labels'][bottomDatasetName] = "#tilde{t}#tilde{t}#rightarrowbbll (#LTc#tau#GT = " + str (ctau) + " mm)"
                options['labels'][topDatasetName] = "#tilde{t}#tilde{t}#rightarrowtt#nu#nu (#LTc#tau#GT = " + str (ctau) + " mm)"
                options['labels'][mixedDatasetName] = "#tilde{t}#tilde{t}#rightarrowbtl#nu (#LTc#tau#GT = " + str (ctau) + " mm)"


def chargino_ctau (dataset):
    if not re.match (r"AMSB_mGrav.*_.*cm", dataset):
        return 0.0
    return float (re.sub (r"AMSB_mGrav[^_]*_([^_]*)cm.*", r"\1", dataset))

def source_chargino_tauFromCtau_Num(ctau):
    # ctau should be in units of cm
    # return tau in ns
    c = 2.998e1       # cm / ns
    tau = ctau / c    # ns
    if tau < 0.5:
        src_tau = 0.5
    elif tau < 1:
        src_tau = 1.0
    else:
        src_tau = 5.0
    return src_tau
    
def source_chargino_tauFromCtau_Str (ctau):
    src_tau = source_chargino_tauFromCtau_Num(ctau)
    if src_tau <= 0.5:
        return "0p5"
    elif src_tau <= 1:
        return "1"
    else:
        return "5"
    
def source_chargino_ctau (ctau):
    src_tau = source_chargino_tauFromCtau_Num(ctau)
    c = 2.998e1      # cm / ns
    ctau = c * src_tau
    return ctau



def add_channels (process, channels, outputCommands = ["keep *"]):
    suffix = ""
    if osusub.batchMode:
        suffix = "_" + str (osusub.jobNumber)
    i = 0
    for channel in channels:
        channelName = channel.name.pythonValue ()
        channelName = channelName[1:-1]
        try:
            os.mkdir (channelName)
        except OSError:
            pass
        out = cms.OutputModule ("PoolOutputModule",
          fileName = cms.untracked.string (channelName + "/bean" + suffix + ".root"),
          SelectEvents = cms.untracked.PSet (
            SelectEvents = cms.vstring ("myFilterPath" + str (i))
          ),
          outputCommands = cms.untracked.vstring (
            outputCommands
          ),
          dropMetaData = cms.untracked.string ("ALL")
        )
        myFilter = cms.EDFilter ("FilterOnChannelDecision",
            src = cms.InputTag ("OSUAnalysis", "channelDecisions"),
            channelName = cms.string (channelName)
        )
        myFilterPath = cms.Path (myFilter)
        myEndPath = cms.EndPath (out)
        process.__setattr__ ("out" + str (i), out)
        process.OSUAnalysis.channels.append (channel)
        process.OSUAnalysis.useEDMFormat = cms.bool (True)
        process.__setattr__ ("myFilter" + str (i), myFilter)
        process.__setattr__ ("myFilterPath" + str (i), myFilterPath)
        process.__setattr__ ("myEndPath" + str (i), myEndPath)
        i += 1
