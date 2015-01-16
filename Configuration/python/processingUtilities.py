import os
import subprocess
import re
import sys
import math
import datetime
import copy
import ROOT
from optparse import OptionParser
import OSUT3Analysis.DBTools.osusub_cfg as osusub
import FWCore.ParameterSet.Config as cms
from OSUT3Analysis.Configuration.InfoPrinter_cff import *

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

def add_stops (options, masses, ctaus, bottomBranchingRatios = [], rHadron = True):
    prefix = 'stopHadron' if rHadron else 'stop'
    if not bottomBranchingRatios:
        bottomBranchingRatios.append (100.0)
    for mass in masses:
        for ctau in ctaus:
            for bottomBranchingRatio in bottomBranchingRatios:
                datasetName = prefix + str (mass) + "_" + str (ctau) + "mm_br" + str (int (bottomBranchingRatio))
                bottomDatasetName = prefix + str (mass) + "toBl_" + str (ctau) + "mm"
                sourceBottomDatasetName = prefix + str (mass) + "toBl_" + str (source_stop_ctau (ctau)) + "mm"
                topDatasetName = prefix + str (mass) + "toTnu_" + str (ctau) + "mm"
                sourceTopDatasetName = prefix + str (mass) + "toTnu_" + str (source_stop_ctau (ctau)) + "mm"
                mixedDatasetName = prefix + str (mass) + "toBT_" + str (ctau) + "mm"
                sourceMixedDatasetName = prefix + str (mass) + "toBT_" + str (source_stop_ctau (ctau)) + "mm"

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
                if not rHadron:
                    options['labels'][datasetName] += " (PYTHIA6)"
                    options['labels'][bottomDatasetName] += " (PYTHIA6)"
                    options['labels'][topDatasetName] += " (PYTHIA6)"
                    options['labels'][mixedDatasetName] += " (PYTHIA6)"


def chargino_ctau (dataset):
    if not re.match (r"AMSB_chargino_.*GeV_RewtCtau.*cm", dataset):
        return -99.0
    return float (re.sub (r"AMSB_chargino_[^_]*GeV_RewtCtau([^_]*)cm", r"\1", dataset))

def source_chargino_ctau (ctau):
    # Units of ctau are cm.
    # Choose as the source the next sample with ctau larger than the target.
    if ctau <= 10:
        src_ctau = 10
    elif ctau <= 100:
        src_ctau = 100
    else:
        src_ctau = 1000
    return float(src_ctau)

def get_collections (cuts):
    ############################################################################
    # Return a list of collections on which cuts are applied with duplicates
    # removed. In the case of a collection pair, we add the individual
    # collections to the list.
    ############################################################################
    collections = set ()
    for cut in cuts:
        for inputCollection in cut.inputCollection:
            collections.add (inputCollection)
    return sorted (list (collections))
    ############################################################################

def add_variables (process, modules, collections):

    ############################################################################
    # If only the default scheduler exists, create an empty one
    ############################################################################
    scheduleType =  type(process.schedule).__name__
    if scheduleType == 'NoneType':
        process.schedule = cms.Schedule ()
    ############################################################################


    ############################################################################
    # Add the variable production modules to a path
    ############################################################################
    if not hasattr (add_variables, "pathIndex"):
        add_variables.pathIndex = 0

    variableProducerPath = cms.Path ()
    for module in modules:
        producer = cms.EDProducer (module,
                                   collections = collections
                                   )
        setattr (process, module, producer)
        variableProducerPath += producer

    ############################################################################
    # Each module is added to the list of event variables in the collections
    # PSet.
    ############################################################################
    for module in modules:
        if not hasattr (collections, "eventVariables"):
            collections.eventVariables = cms.VInputTag ()
        collections.eventVariables.append (cms.InputTag (module, "extraEventVariables"))

    ############################################################################
    # Add the variable production path at the beginning of the schedule
    ############################################################################
    pathName = "variableProducerPath" + str(add_variables.pathIndex)
    setattr(process, pathName, variableProducerPath)
    process.schedule.insert(0,variableProducerPath)

    add_variables.pathIndex += 1


def add_channels (process, channels, histogramSets, collections, variableProducers, skim = True):

    ############################################################################
    # If only the default scheduler exists, create an empty one
    ############################################################################
    scheduleType =  type(process.schedule).__name__
    if scheduleType == 'NoneType':
        process.schedule = cms.Schedule ()
    ############################################################################

    ############################################################################
    # Suffix is appended to the name of the output file. In batch mode, an
    # underscore followed by the job number is appended.
    ############################################################################
    suffix = ""
    if osusub.batchMode:
        suffix = "_" + str (osusub.jobNumber)
    ############################################################################

    ############################################################################
    # We append filterIndexnames of the filter PSets since they need unique
    # names. We use function attributes so that add_channels can be called
    # multiple times to add additional channels.
    ############################################################################
    if not hasattr (add_channels, "filterIndex"):
        add_channels.filterIndex = 0
    if not hasattr (add_channels, "endPath"):
        add_channels.endPath = cms.EndPath ()
    ############################################################################

    ############################################################################
    # Change the process name to be unique, to avoid DuplicateProcess error
    # in the case of running over skims.
    # To ensure a unique process name, use a date/time stamp.
    ############################################################################
    if not hasattr (add_channels, "processNameUpdated"):
        add_channels.processNameUpdated = True
        now = datetime.datetime.now()
        date_hash = now.strftime("%YY%mM%dD%Hh%Mm%Ss")  # Non-alpha-numeric characters are not allowed in the process name.
        process.setName_ (process.name_ () + date_hash)
    ############################################################################


    for channel in channels:
        channelPath = cms.Path ()
        channelCollections = copy.deepcopy (collections)

        ########################################################################
        # Get the name of the channel and try to make a directory with that
        # name. If the directory already exists, an OSError exception will be
        # raised, which we ignore.
        ########################################################################
        channelName = channel.name.pythonValue ()
        channelName = channelName[1:-1]
        if skim:
            try:
                os.mkdir (channelName)
            except OSError:
                pass
        ########################################################################

        ########################################################################
        # Add a cut calculator module for this channel to the path.
        ########################################################################
        cutCalculator = cms.EDProducer ("CutCalculator",
            collections = collections,
            cuts = channel
        )
        channelPath += cutCalculator
        setattr (process, channelName + "CutCalculator", cutCalculator)
        ########################################################################

        ########################################################################
        # Add a cut flow plotting module for this channel to the path.
        ########################################################################
        cutFlowPlotter = cms.EDAnalyzer ("CutFlowPlotter",
            cutDecisions = cms.InputTag (channelName + "CutCalculator", "cutDecisions")
        )
        channelPath += cutFlowPlotter
        setattr (process, channelName + "CutFlowPlotter", cutFlowPlotter)
        ########################################################################

        ########################################################################
        # Add a module for printing info, both general and for specific events.
        ########################################################################
        channelInfoPrinter = copy.deepcopy (infoPrinter)
        channelInfoPrinter.collections = collections
        channelInfoPrinter.cutDecisions = cms.InputTag (channelName + "CutCalculator", "cutDecisions")
        channelPath += channelInfoPrinter
        setattr (process, channelName + "InfoPrinter", channelInfoPrinter)
        ########################################################################

        ########################################################################
        # Set up the output commands. For now, we drop everything except the
        # collections given in the collections PSet.
        ########################################################################
        outputCommands = ["drop *"]
        outputCommands.append("keep *_*_extraEventVariables_*")
        outputCommands.append("keep *_*_extraObjectVariables_*")
        for collection in [a for a in dir (collections) if not a.startswith('_') and not callable (getattr (collections, a)) and a is not "eventVariables" and a is not "objectVariables"]:
            collectionTag = getattr (collections, collection)
            outputCommand = "keep BN"
            outputCommand += collection
            outputCommand += "_"
            outputCommand += collectionTag.getModuleLabel ()
            outputCommand += "_"
            outputCommand += collectionTag.getProductInstanceLabel ()
            outputCommand += "_"
            outputCommand += "BEANs"
            outputCommands.append (outputCommand)
        ########################################################################

        ########################################################################
        # For each collection on which cuts are applied, we add the
        # corresponding object selector to the path. We also trade the original
        # collection for the slimmed collection in the output commands.
        ########################################################################
        cutCollections = get_collections (channel.cuts)
        for collection in cutCollections:
            filterName = collection[0].upper () + collection[1:-1] + "ObjectSelector"
            objectSelector = cms.EDFilter (filterName,
                collections = collections,
                collectionToFilter = cms.string (collection),
                cutDecisions = cms.InputTag (channelName + "CutCalculator", "cutDecisions")
            )
            channelPath += objectSelector
            setattr (process, "objectSelector" + str (add_channels.filterIndex), objectSelector)
            originalInputTag = getattr (collections, collection)
            setattr (channelCollections, collection, cms.InputTag ("objectSelector" + str (add_channels.filterIndex), originalInputTag.getProductInstanceLabel ()))
            outputCommands.append ("drop BN" + collection + "_*_*_*")
            outputCommands.append ("keep BN" + collection + "_objectSelector" + str (add_channels.filterIndex) + "_" + originalInputTag.getProductInstanceLabel () + "_" + process.name_ ())
            add_channels.filterIndex += 1
        ########################################################################

        ########################################################################
        # Add the variableProducers to the path. 
        ########################################################################
        for varProd in variableProducers:
            producer = cms.EDProducer (varProd,
                                       collections = collections
                                       )
            setattr (process, varProd, producer)
            channelPath += producer 

            if not hasattr (collections, "eventVariables"):
                collections.eventVariables = cms.VInputTag ()
            if not hasattr (channelCollections, "eventVariables"):
                channelCollections.eventVariables = cms.VInputTag ()
            collections.eventVariables.append        (cms.InputTag (varProd, "extraEventVariables"))
            channelCollections.eventVariables.append (cms.InputTag (varProd, "extraEventVariables"))


        ########################################################################
        # Add a plotting module for this channel to the path.
        ########################################################################
        if len (histogramSets):
            plotter = cms.EDAnalyzer ("Plotter",
                collections     =  channelCollections,
                histogramSets   =  histogramSets,
                verbose         =  cms.int32 (0)
            )
            channelPath += plotter
            setattr (process, channelName + "Plotter", plotter)
        ########################################################################

        ########################################################################
        # Add an output module for this channel to the path. We can use any of
        # the object selectors for this channel as the "SelectEvents" parameter
        # since they each return the global event decision. So we use the first
        # which was added.
        ########################################################################
        if skim:
            SelectEvents = cms.vstring ()
            if cutCollections:
                SelectEvents = cms.vstring (channelName)
            poolOutputModule = cms.OutputModule ("PoolOutputModule",
                splitLevel = cms.untracked.int32 (0),
                eventAutoFlushCompressedSize = cms.untracked.int32 (5242880),
                fileName = cms.untracked.string (channelName + "/skim" + suffix + ".root"),
                SelectEvents = cms.untracked.PSet (SelectEvents = SelectEvents),
                outputCommands = cms.untracked.vstring (outputCommands),
                dropMetaData = cms.untracked.string ("ALL")
            )
            add_channels.endPath += poolOutputModule
            setattr (process, channelName + "PoolOutputModule", poolOutputModule)
        ########################################################################

        setattr (process, channelName, channelPath)
        process.schedule.append(getattr(process,channelName))
    setattr (process, "endPath", add_channels.endPath)
    set_endPath(process, add_channels.endPath)

def set_endPath(process, endPath):

    ############################################################################
    # Update schedule with the current endPath at the end
    ############################################################################

    # find the old endpath (if there is one), and remove it
    endPathIndex = -1
    for index in range(len(process.schedule)):
        if type(process.schedule[index]).__name__ == 'EndPath':
            endPathIndex = index
    if endPathIndex > -1:
        del process.schedule[endPathIndex]

    # add the new endpath at the end of the schedule
    process.schedule.append(endPath)

def set_input(process, input_string):
    from OSUT3Analysis.Configuration.configurationOptions import dataset_names
    
    ############################################################################
    # This function configures the input dataset
    # It can take a dataset nickname, directory, or file as argument
    ############################################################################
    datasetInfo = cms.PSet ()
    datasetInfo.name = cms.string ("NONE")
    datasetInfo.type = cms.string ("NONE")
    process.source = cms.Source ('PoolSource',
                                 fileNames = cms.untracked.vstring ()
                                 )

    fileType = subprocess.check_output(['file',input_string]).split(":")[1]
    # try opening 'input_string' as a ROOT file
    if "ROOT" in fileType:
        process.source.fileNames.extend(cms.untracked.vstring('file:' + input_string))
        return
    
    # try opening 'input_string' as a directory
    elif "directory" in fileType:
        for file in os.listdir(input_string):
            filePath = input_string + "/" + file
            fileType = subprocess.check_output(['file',filePath]).split(":")[1]
            if "ROOT" in fileType:
                process.source.fileNames.extend(cms.untracked.vstring('file:' + input_string + "/" + file))
        return

    # try using 'input_string' as a registered dataset name        
    datasetDirectory = dataset_names[input_string]
    subprocess.call(['MySQLModule',datasetDirectory,'temp_datasetInfo.py','file:'])
    from temp_datasetInfo import listOfFiles
    process.source.fileNames.extend(cms.untracked.vstring(listOfFiles))
    subprocess.call(['rm','temp_datasetInfo.py'])
    subprocess.call(['rm','temp_datasetInfo.pyc'])
