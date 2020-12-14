import os
import subprocess
import re
import sys
import math
import datetime
import time
import copy
import pickle
import tempfile
import shutil
import FWCore.ParameterSet.Modules
from optparse import OptionParser
import OSUT3Analysis.DBTools.osusub_cfg as osusub
import FWCore.ParameterSet.Config as cms
from OSUT3Analysis.Configuration.InfoPrinter_cff import *
from OSUT3Analysis.Configuration.CollectionProducer_cff import *
from OSUT3Analysis.Configuration.LifetimeWeightProducer_cff import *
from OSUT3Analysis.Configuration.ISRWeightProducer_cff import *
from OSUT3Analysis.Configuration.PUScalingFactorProducer_cff import *
from OSUT3Analysis.Configuration.L1PrefiringWeightProducer_cff import *

def GetCompleteOrderedArgumentsSet(InputArguments, currentCondorSubArgumentsSet):
    NewArguments = copy.deepcopy(InputArguments)
    for argument in InputArguments:
        for index in currentCondorSubArgumentsSet:
            if currentCondorSubArgumentsSet[index].has_key(argument):
                currentCondorSubArgumentsSet[index][argument] = InputArguments[argument]
                NewArguments.pop(argument)
                break
    for newArgument in NewArguments:
        currentCondorSubArgumentsSet.setdefault(len(currentCondorSubArgumentsSet.keys()) + 1, {newArgument : NewArguments[newArgument]})
    currentCondorSubArgumentsSet.setdefault(len(currentCondorSubArgumentsSet.keys()) + 1, {'Queue': ''})

def get_date_time_stamp():
    # Return a string that encodes the date and time
    # Do not include ":" in string, because it is not allowed in a filename
    now = datetime.datetime.now()
    date_hash = now.strftime("%Y_%m_%d_%Hh%Mm%Ss") # Format:  1970_01_01_12h00m30s
    return date_hash

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
    parser.add_option("-w", "--workDirectory", dest="condorDir",
                      help="condor working directory")
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
    parser.add_option("--pngs",  action="store_true", dest="savePNGs", default=False,
                      help="Save pngs files for all plots made")

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

def get_collections (cuts):
    ############################################################################
    # Return a list of collections on which cuts are applied with duplicates
    # removed. In the case of a collection pair, we add the individual
    # collections to the list.
    ############################################################################
    collections = set ()
    for cut in cuts:
        for inputCollection in (cut.inputCollection if hasattr (cut, "inputCollection") else cut.inputCollections):
            collections.add (inputCollection)
    return sorted (list (collections))
    ############################################################################

def set_skim_tags (inputFileName, collections):
    # If we are running over a skim via XrootD, get SkimInputTags.pkl via XrootD
    if inputFileName.startswith ('root:'):
        tmpDir = tempfile.mkdtemp ()
        subprocess.call('xrdcp ' + os.path.dirname(inputFileName) + '/SkimInputTags.pkl ' + tmpDir + '/SkimInputTags.pkl', shell = True)
        inputTagPickleName = tmpDir + '/SkimInputTags.pkl'
    # Otherwise get SkimInputTags.pkl via the regular file system
    else:
        if inputFileName.startswith ('file:'):
            inputFileName = inputFileName[5:]
        inputTagPickleName = os.path.dirname (os.path.realpath (inputFileName)) + '/SkimInputTags.pkl'
    if not os.path.isfile (inputTagPickleName):
        print "ERROR:  The input file appears to be a skim file but no SkimInputTags.pkl file found in the skim directory."
        print "Input file is", inputFileName
        print "Be sure that you have run mergeOut.py."
        if inputFileName.startswith ('root:'):
            shutil.rmtree (tmpDir)
        sys.exit(1)
    fin = open (inputTagPickleName)
    inputTags = pickle.load (fin)
    fin.close ()
    if inputFileName.startswith ('root:'):
        shutil.rmtree (tmpDir)
    for tag in inputTags:
        setattr (collections, tag, inputTags[tag])

#def add_channels (process, channels, histogramSets, weights, scalingfactorproducers, collections, variableProducers, skim = True, branchSets):
def add_channels (process,
                  channels,
                  histogramSets = None,
                  weights = None,
                  scalingfactorproducers = None,
                  collections = None,
                  variableProducers = None,
                  skim = None,
                  branchSets = None,
                  ignoreSkimmedCollections = False,
                  forceNonEmptySkim = False):
    if skim is not None:
        print "# The \"skim\" parameter of add_channels is obsolete and will soon be deprecated."
        print "# Please remove from your config files."

    ############################################################################
    # If there are only two arguments, then channels is actually an
    # AddChannelArguments object that needs to be unpacked.
    ############################################################################
    standAloneAnalyzers = []
    if histogramSets is None:
        variableProducers       =  channels.variableProducers
        weights                 =  channels.weights
        scalingfactorproducers  =  channels.scalingfactorproducers
        standAloneAnalyzers     =  channels.standAloneAnalyzers
        histogramSets           =  channels.histogramSets
        collections             =  channels.collections
        channels                =  channels.channels

    ############################################################################
    # Check that cut and histogram definitions contain no invalid attributes
    ############################################################################
    validCutAttributes = [
        'inputCollection',
        'cutString',
        'alias',
        'numberRequired',
        'isVeto',
    ]
    validHistAttributes = [
        'name',
        'title',
        'binsX', 'binsY', 'binsZ',
        'indexX', 'indexY', 'indexZ',
        'inputVariables',
        'weight',
    ]

    # find all cuts with invalid attributes
    exceptionString = ""
    for selection in channels:
        for cut in selection.cuts:
            attributes = [a for a in cut.__dict__.keys() if not a.startswith('_')]
            invalidAttributes = list(set(attributes) - set(validCutAttributes))
            if invalidAttributes:
                name = cut.alias if hasattr(cut, 'alias') else cut.cutString
                name = str(name)[12:-2]
                exceptionString += "\ncut '{}' has invalid attributes: {}".format(name, invalidAttributes)

    # find all hists with invalid attributes
    for histogramSet in histogramSets:
        for hist in histogramSet.histograms:
            attributes = [a for a in hist.__dict__.keys() if not a.startswith('_')]
            invalidAttributes = list(set(attributes) - set(validHistAttributes))
            if invalidAttributes:
                name = str(hist.name)[12:-2]
                exceptionString += "\nhist '{}' has invalid attributes: {}".format(name, invalidAttributes)

    # if necessary, raise exception listing all cuts or hists with invalid attributes
    if exceptionString:
        raise AttributeError(exceptionString)

    ############################################################################
    # Check the directory of the first input file for SkimInputTags.pkl. If it
    # exists, update the input tags with those stored in the pickle file.
    ############################################################################
    makeEmptySkim = False
    fileName = None
    if hasattr (process.source, "fileNames") and len (process.source.fileNames) > 0:
        fileName = process.source.fileNames[0]
    if osusub.batchMode:
        fileName = osusub.runList[0]
    rootFile = fileName.split("/")[-1]  # e.g., skim_0.root

    # If we are running over an empty skim, get the file name from the
    # secondary files. The secondary files should be a full skim with
    # SkimInputTags.pkl in the same directory.
    if rootFile.startswith ("emptySkim_"):
        makeEmptySkim = True
        primaryFileName = fileName
        fileName = None
        if hasattr (process.source, "secondaryFileNames") and len (process.source.secondaryFileNames) > 0:
            fileName = process.source.secondaryFileNames[0]
        if osusub.batchMode:
            fileName = osusub.secondaryRunList[0]
        if fileName is None:
            print "ERROR:  The input file appears to be an empty skim file but no secondary files were found."
            print "Input file is", primaryFileName
            print "This should not be."
            sys.exit(1)
        rootFile = fileName.split("/")[-1]  # e.g., skim_0.root
    elif rootFile.startswith ("skim_"):
        makeEmptySkim = True

    # If the input file is either skim_*.root or emptySkim_*.root,
    # we need now to read the input tags and replace as necessary
    if makeEmptySkim:
        # If we deliberately ignore this by user flag, just print a message
        if ignoreSkimmedCollections:
            print "INFO: user has set ignoreSkimmedCollections, meaning that the original data collections will be used instead of skimmed framework collections."
        else:
            set_skim_tags (fileName, collections)

    ############################################################################

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
    if not hasattr (add_channels, "producerIndex"):
        add_channels.producerIndex = 0
    if not hasattr (add_channels, "standAloneAnalyzerIndex"):
        add_channels.standAloneAnalyzerIndex = 0
    if not hasattr (add_channels, "filterIndex"):
        add_channels.filterIndex = 0
    if not hasattr (add_channels, "endPath"):
        add_channels.endPath = cms.EndPath ()
    ############################################################################

    ############################################################################
    # Change the process name, by adding the name of the first channel,
    # to avoid DuplicateProcess error in the case of running over skims.
    ############################################################################
    if not hasattr (add_channels, "processNameUpdated"):
        add_channels.processNameUpdated = True
        if osusub.batchMode:
            randomNumberSuffix = osusub.randomNumberSuffix
        else:
            randomNumberSuffix = int (time.time ())  # Use the seconds since the Unix epoch to get a
                                                     # different process name when running over a skim with the
                                                     # same channel.
        channelName = str(channels[0].name.pythonValue())
        channelName = channelName.replace("'", "").replace("_", "") # Non-alpha-numeric characters are not allowed in the process name.
        process.setName_ (process.name_ () + channelName + str(randomNumberSuffix))
    ############################################################################

    ############################################################################
    # For interactive jobs only,
    # change the output filename to include the name of the first channel,
    # as well as a date/time stamp.
    # For batch jobs, do not change filename; we want it identical for all jobs.
    ############################################################################
    if not osusub.batchMode:
        if not hasattr (add_channels, "originalName"):
            add_channels.originalName = str(process.TFileService.fileName.pythonValue()).replace("'", "")  # Remove quotes.
            outputName = add_channels.originalName
            suffix = "_" + str(channels[0].name.pythonValue()).replace("'", "") + "_" + get_date_time_stamp()
            outputName = outputName.replace(".root", suffix + ".root")
            process.TFileService.fileName = cms.string(outputName)
            if os.path.islink (add_channels.originalName):
                os.unlink (add_channels.originalName)
            if os.path.exists (add_channels.originalName):
                os.remove (add_channels.originalName)
            os.symlink (outputName, add_channels.originalName)
    ############################################################################

    plotCollections = get_collections (histogramSets)
    weightCollections = get_collections (weights)

    for channel in channels:
        channelPath = cms.Path ()
        channelName = channel.name.pythonValue ()
        channelName = channelName[1:-1]  # Remove quotation marks

        ########################################################################
        # Check to see if this channel has already been added.
        # Since all channels must have unique names, this will break everything.
        # So we'll print a warning and skip this channel.
        ########################################################################
        if hasattr (process, channelName):
            print ("WARNING [add_channels]: The '" +
                   channelName +
                   "' channel has been added more than once")
            print "  Skipping this channel!"
            continue

        ########################################################################
        # If a skim is requested, get the name of the channel
        # and try to make a directory with that name.
        # If the directory already exists, an OSError exception will be
        # raised, which we ignore.
        ########################################################################
        try:
            os.mkdir (channelName)
        except OSError:
            pass
        ########################################################################

        ########################################################################
        # Add the variable production modules to a path
        ########################################################################
        variableProducerPath = cms.Path ()
        for module in variableProducers:
            if module == 'prefiringweight':
                if module in locals ():
                    setattr (process, module, locals ()[module])
                elif module in globals ():
                    setattr (process, module, globals ()[module])
                else:
                    sys.exit("ERROR [add_channels]: cannot find '" + module + "'") # should not be possible since this module itself imports this object
                variableProducerPath += getattr (process, module)
                continue
            else:
                if module not in locals () and module not in globals ():
                    producer = cms.EDFilter (module,
                        collections = collections
                    )
                    setattr (process, module, producer)
                elif module in locals ():
                    setattr (process, module, locals ()[module])
                    setattr (getattr (process, module), "collections", collections)
                elif module in globals ():
                    setattr (process, module, globals ()[module])
                    setattr (getattr (process, module), "collections", collections)
                if module == 'L1PrefiringWeightProducer' and hasattr (process, 'prefiringweight'):
                    # copy the DataEra from the CMSSW producer to the framework producer, to check that it is valid
                    setattr (process.L1PrefiringWeightProducer, 'DataEra', process.prefiringweight.DataEra)
                variableProducerPath += getattr (process, module)
            ########################################################################

            ########################################################################
            # Each variable producer module is added to the list of user variable
            # collections in the collections PSet.
            ########################################################################
            if not hasattr (collections, "uservariables"):
                collections.uservariables = cms.VInputTag ()
            # verify this collection hasn't already been added
            isDuplicate = False
            for inputTag in collections.uservariables:
                if inputTag.getModuleLabel() is module:
                    isDuplicate = True
                    break
            if not isDuplicate:
                collections.uservariables.append (cms.InputTag (module, "uservariables"))
            ########################################################################

            ########################################################################
            # Each event variable producer module is added to the list of user variable
            # collections in the collections PSet.
            ########################################################################
            if not hasattr (collections, "eventvariables"):
                collections.eventvariables = cms.VInputTag ()
            # verify this collection hasn't already been added
            isDuplicate = False
            for inputTag in collections.eventvariables:
                if inputTag.getModuleLabel() is module:
                    isDuplicate = True
                    break
            if not isDuplicate:
                collections.eventvariables.append (cms.InputTag (module, "eventvariables"))
        ########################################################################

        ########################################################################
        # Add the variable production path at the beginning of the schedule
        ########################################################################
        if not hasattr (process, "variableProducerPath"):
            setattr(process, "variableProducerPath", variableProducerPath)
            process.schedule.insert(0,variableProducerPath)
        ########################################################################

        ########################################################################
        # Set up the output commands. For now, we drop everything except the
        # collections given in the collections PSet, and a few other collections
        # that are generally useful.
        ########################################################################
        outputCommands = ["drop *"]
        outputCommands.append("keep *_*_uservariables_*")
        outputCommands.append("keep *_*_eventvariables_*")
        outputCommands.append("keep *_*_reducedConversions_*")
        ########################################################################
        #Check all the modules in collectionProducer, if a module is an EDProducer
        #and have extra InputTags specified, keep the correspoding collections
        #in the outputCommands
        ########################################################################
        for collection in dir(collectionProducer):
            if isinstance(getattr(collectionProducer,collection) ,FWCore.ParameterSet.Modules.EDProducer) or isinstance(getattr(collectionProducer,collection) ,FWCore.ParameterSet.Modules.EDFilter):
                dic = vars(getattr(collectionProducer,collection))
                for p in dic:
                    if isinstance(dic[p], cms.VPSet):
                        continue
                    if 'InputTag' in str(dic[p]):
                        outputCommand = "keep *_"
                        outputCommand += dic[p].getModuleLabel ()
                        outputCommand += "_"
                        outputCommand += dic[p].getProductInstanceLabel ()
                        outputCommand += "_"
                        if dic[p].getProcessName ():
                            outputCommand += dic[p].getProcessName ()
                        else:
                            outputCommand += "*"
                        if dic[p].getModuleLabel () == 'egmGsfElectronIDs':
                            outputCommand = "drop *_egmGsfElectronIDs_*_*"
                        if dic[p].getModuleLabel () == 'egmGsfElectronIDsOriginalElectrons':
                            outputCommand = "drop *_egmGsfElectronIDsOriginalElectrons_*_*"
                        if dic[p].getModuleLabel () == 'egmPhotonIDs':
                            outputCommand = "drop *_egmPhotonIDs_*_*"
                        if dic[p].getModuleLabel () == 'egmPhotonIDsOriginalPhotons':
                            outputCommand = "drop *_egmPhotonIDsOriginalPhotons_*_*"
                        outputCommands.append (outputCommand)

        ########################################################################
        # Add keep statements for all collections except uservariables and
        # eventvariables.
        ########################################################################
        for collection in [a for a in dir (collections) if not a.startswith('_') and not callable (getattr (collections, a)) and a is not "uservariables" and a is not "eventvariables"]:
            collectionTag = getattr (collections, collection)
            outputCommand = "keep *_"
            outputCommand += collectionTag.getModuleLabel ()
            outputCommand += "_"
            outputCommand += collectionTag.getProductInstanceLabel ()
            outputCommand += "_"
            if collectionTag.getProcessName ():
                outputCommand += collectionTag.getProcessName ()
            else:
                outputCommand += "*"
            outputCommands.append (outputCommand)
        ########################################################################

        # The following collections are meant to be produced before any others.
        # This is usually necessary because the production of other collections
        # depends on these.
        collectionsToProduce = [
            "mets",         # needed in order to use metNoMu in the lepton collections
            "mcparticles",  # needed for gen-matching
        ]

        ########################################################################
        # Add an OSU object producer for each collection used in a cut or
        # histogram.
        ########################################################################
        producedCollections = copy.deepcopy (collections)
        cutCollections = get_collections (channel.cuts)
        usedCollections = sorted (list (set (cutCollections + plotCollections + weightCollections)))
        for collection in collectionsToProduce:
            if collection in usedCollections:
                usedCollections.remove (collection)
            if hasattr (collections, collection):
                usedCollections.insert (0, collection)
        for collection in usedCollections:
            if collection is "uservariables" or collection is "eventvariables":
                newInputTags = cms.VInputTag()
                if hasattr (collections, collection):
                    inputTags = getattr (collections, collection)
                else:
                    inputTags = cms.VInputTag()
                for inputTag in inputTags:
                    eventvariableCollections = copy.deepcopy (collections)
                    setattr (eventvariableCollections, collection, cms.InputTag ("",""))
                    setattr (eventvariableCollections, collection,inputTag)
                    objectProducer = getattr (collectionProducer, collection).clone()
                    objectProducer.collections = eventvariableCollections
                    # Set the input tag for mcparticles to that produced by the
                    # first object producer. Needed for gen-matching to work. DO
                    # NOT ERASE!!!
                    if collection != "mcparticles":
                        label = getattr (collections, "mcparticles").getProductInstanceLabel () if hasattr (collections, "mcparticles") else ""
                        setattr (objectProducer.collections, "mcparticles", cms.InputTag ("objectProducer0", label))
                    # Set the input tag for mets to that produced by the second
                    # object producer. Needed for metNoMu. DO NOT ERASE!!!
                    if collection != "mcparticles" and collection != "mets":
                        label = getattr (collections, "mets").getProductInstanceLabel () if hasattr (collections, "mets") else ""
                        setattr (objectProducer.collections, "mets", cms.InputTag ("objectProducer1", label))
                    channelPath += objectProducer
                    setattr (process, "objectProducer" + str (add_channels.producerIndex), objectProducer)
                    newInputTags.append(cms.InputTag ("objectProducer" + str (add_channels.producerIndex), inputTag.getProductInstanceLabel ()))
                    if collection in cutCollections:
                        dropCommand = "drop *_" + inputTag.getModuleLabel () + "_" + inputTag.getProductInstanceLabel () + "_"
                        if inputTag.getProcessName ():
                            dropCommand += inputTag.getProcessName ()
                        else:
                            dropCommand += "*"
                        outputCommands.append (dropCommand)
                    # if collection not in cutCollections:
                    #     outputCommands.append ("keep *_objectProducer" + str (add_channels.producerIndex) + "_" + inputTag.getProductInstanceLabel () + "_" + process.name_ ())
                    add_channels.producerIndex += 1
                setattr (producedCollections, collection, newInputTags)
            else:
                objectProducer = getattr (collectionProducer, collection).clone()
                objectProducer.collections = copy.deepcopy (collections)
                # Set the input tag for mcparticles to that produced by the
                # first object producer. Needed for gen-matching to work. DO
                # NOT ERASE!!!
                if collection != "mcparticles":
                    label = getattr (collections, "mcparticles").getProductInstanceLabel () if hasattr (collections, "mcparticles") else ""
                    setattr (objectProducer.collections, "mcparticles", cms.InputTag ("objectProducer0", label))
                # Set the input tag for mets to that produced by the second
                # object producer. Needed for metNoMu. DO NOT ERASE!!!
                if collection != "mcparticles" and collection != "mets":
                    label = getattr (collections, "mets").getProductInstanceLabel () if hasattr (collections, "mets") else ""
                    setattr (objectProducer.collections, "mets", cms.InputTag ("objectProducer1", label))
                channelPath += objectProducer
                setattr (process, "objectProducer" + str (add_channels.producerIndex), objectProducer)
                originalInputTag = getattr (collections, collection)
                setattr (producedCollections, collection, cms.InputTag ("objectProducer" + str (add_channels.producerIndex), originalInputTag.getProductInstanceLabel ()))
                if collection in cutCollections:
                    dropCommand = "drop *_" + originalInputTag.getModuleLabel () + "_" + originalInputTag.getProductInstanceLabel () + "_"
                    if originalInputTag.getProcessName ():
                        dropCommand += originalInputTag.getProcessName ()
                    else:
                        dropCommand += "*"
                    outputCommands.append (dropCommand)
                # if collection not in cutCollections:
                #     outputCommands.append ("keep *_objectProducer" + str (add_channels.producerIndex) + "_" + originalInputTag.getProductInstanceLabel () + "_" + process.name_ ())
                add_channels.producerIndex += 1
        ########################################################################

        ########################################################################
        # Add a cut calculator module for this channel to the path.
        ########################################################################
        cutCalculator = cms.EDProducer ("CutCalculator",
            collections = producedCollections,
            cuts = channel
        )
        channelPath += cutCalculator
        setattr (process, channelName + "CutCalculator", cutCalculator)
        ########################################################################

        ########################################################################
        # Add a cut flow plotting module for this channel to the path.
        ########################################################################
        cutFlowPlotter = cms.EDAnalyzer ("CutFlowPlotter",
            collections = producedCollections,
            cutDecisions = cms.InputTag (channelName + "CutCalculator", "cutDecisions")
        )
        channelPath += cutFlowPlotter
        setattr (process, channelName + "CutFlowPlotter", cutFlowPlotter)
        ########################################################################

        ########################################################################
        # Add a module for printing info, both general and for specific events.
        ########################################################################
        channelInfoPrinter = copy.deepcopy (infoPrinter)
        channelInfoPrinter.collections = producedCollections
        channelInfoPrinter.cutDecisions = cms.InputTag (channelName + "CutCalculator", "cutDecisions")
        channelPath += channelInfoPrinter
        setattr (process, channelName + "InfoPrinter", channelInfoPrinter)
        ########################################################################

        ########################################################################
        # For each collection on which cuts are applied, we add the
        # corresponding object selector to the path. We also trade the original
        # collection for the slimmed collection in the output commands.
        ########################################################################
        filteredCollections = copy.deepcopy (producedCollections)
        for collection in cutCollections:
            # Temporary fix for user-defined variables
            # For the moment, they won't be filtered
            if collection is "uservariables" or collection is "eventvariables":
                continue
            filterName = collection[0].upper () + collection[1:-1] + "ObjectSelector"
            objectSelector = cms.EDFilter (filterName,
                collections = producedCollections,
                collectionToFilter = cms.string (collection),
                originalCollection = getattr (collections, collection),
                cutDecisions = cms.InputTag (channelName + "CutCalculator", "cutDecisions")
            )
            channelPath += objectSelector
            setattr (process, "objectSelector" + str (add_channels.filterIndex), objectSelector)
            originalInputTag = getattr (collections, collection)
            setattr (filteredCollections, collection, cms.InputTag ("objectSelector" + str (add_channels.filterIndex), originalInputTag.getProductInstanceLabel ()))
            outputCommands.append ("keep *_objectSelector" + str (add_channels.filterIndex) + "_originalFormat_" + process.name_ ())
            add_channels.filterIndex += 1

        ########################################################################
        # Add producers for the scaling factor producers which need the selected
        # objects. For example the lepton scaling factors.
        ########################################################################
        if len(scalingfactorproducers):
            for module in scalingfactorproducers:
                # Here we try to add the original producer as specified in the config files.
                objectProducer = cms.EDFilter (str(module['name']),
                                        # Use filteredCollections, the ones selected by the objectSelectors
                                        collections = copy.deepcopy(filteredCollections)
                                           )
                setattr (process, "objectProducer" + str (add_channels.producerIndex), objectProducer)
                # Add the user defined configable variables.
                for key in module:
                    if str(key) != 'name':
                        setattr (objectProducer, key, module[key])

                # Add this producer in to the path of this channel.
                channelPath += objectProducer
                add_channels.producerIndex += 1
                # Now add osu eventvariable producer to produce <osu::eventvariable> for the plotter to use.
                objectProducer = getattr (collectionProducer, "eventvariables").clone()
                objectProducer.collections = copy.deepcopy(filteredCollections)
                channelPath += objectProducer
                setattr (process, "objectProducer" + str (add_channels.producerIndex), objectProducer)
                # Use the eventvariables producered in the above specific producers.
                setattr(objectProducer.collections, "eventvariables" ,cms.InputTag ("objectProducer" + str (add_channels.producerIndex - 1), "eventvariables"))
                if not hasattr (filteredCollections, "eventvariables"):
                    filteredCollections.eventvariables = cms.VInputTag ()
                # Add the eventvariables produced in this module to the filteredCollections for the plotter after to use.
                filteredCollections.eventvariables.append(cms.InputTag ("objectProducer" + str (add_channels.producerIndex), "eventvariables"))
                add_channels.producerIndex += 1

        if len(standAloneAnalyzers):
            for module in standAloneAnalyzers:
                # Here we try to add the original producer as specified in the config files.
                standAloneAnalyzer = cms.EDAnalyzer (str(module['name']),
                                        # Use filteredCollections, the ones selected by the objectSelectors
                                        collections = copy.deepcopy(filteredCollections)
                                           )
                setattr (process, str(module['name']) + str (add_channels.standAloneAnalyzerIndex), standAloneAnalyzer)
                # Add the user defined configable variables.
                for key in module:
                    if str(key) != 'name':
                        setattr (standAloneAnalyzer, key, module[key])
                # Add this stand alone analyzer  in to the path of this channel.
                channelPath += standAloneAnalyzer
                add_channels.standAloneAnalyzerIndex += 1
        ########################################################################
        # Add a plotting module for this channel to the path.
        ########################################################################
        if len (histogramSets):
            plotter = cms.EDAnalyzer ("Plotter",
                collections     =  filteredCollections,
                histogramSets   =  histogramSets,
                weights         =  weights,
                verbose         =  cms.int32 (0)
            )
            channelPath += plotter
            setattr (process, channelName + "Plotter", plotter)

            # Add copies of the plotting module for any weights being fluctuated
            for weight in weights:
                # if "fluctuations" is defined in the PSet
                for fluctuation in (weight.fluctuations if hasattr (weight, "fluctuations") else []):
                    # copy all the weights into a new VPSet
                    fluctuatedWeights = copy.deepcopy(weights)
                    # now find the weight being fluctuated in the newly copied VPSet
                    for fluctuatedWeight in fluctuatedWeights:
                        if fluctuatedWeight.inputVariable == weight.inputVariable and fluctuatedWeight.inputCollections == weight.inputCollections:
                            # now change the name of the inputVariable to the fluctuation and add a plotting module for this weights VPSet
                            fluctuatedWeight.inputVariable = fluctuation
                            fluctuatedPlotter = copy.deepcopy(plotter)
                            fluctuatedPlotter.weights = fluctuatedWeights
                            channelPath += fluctuatedPlotter
                            setattr (process, channelName + "Plotter_" + fluctuation, fluctuatedPlotter)
                            break

        ########################################################################
        # Add a tree-making module for this channel to the path.
        ########################################################################
        if branchSets is not None and len(branchSets):
            treeMaker = cms.EDAnalyzer ("TreeMaker",
                collections = filteredCollections,
                branchSets  = branchSets,
                weights     = weights,
                verbose     = cms.int32 (0)
            )
            channelPath += treeMaker
            setattr (process, channelName + "TreeMaker", treeMaker)

        ########################################################################

        ########################################################################
        # Add an output module for this channel to the path. We can use any of
        # the object selectors for this channel as the "SelectEvents" parameter
        # since they each return the global event decision. So we use the first
        # which was added.
        ########################################################################
        SelectEvents = cms.vstring ()
        if cutCollections:
            SelectEvents = cms.vstring (channelName)
        skimFilePrefix = "skim"

        from OSUT3Analysis.AnaTools.osuAnalysis_cfi import dataFormat
        if dataFormat.startswith ("MINI_AOD"):
            from Configuration.EventContent.EventContent_cff import MINIAODSIMEventContent
            for outputCommand in MINIAODSIMEventContent.outputCommands:
                # Don't add extra drop commands, we already have one at the beginning
                if outputCommand != 'drop *':
                    outputCommands.append(outputCommand)
        elif dataFormat.startswith ("AOD"):
            from Configuration.EventContent.EventContent_cff import AODSIMEventContent
            for outputCommand in AODSIMEventContent.outputCommands:
                # Don't add extra drop commands, we already have one at the beginning
                if outputCommand != 'drop *':
                    outputCommands.append(outputCommand)

        if not hasattr (add_channels, "customOutputCommands"):
            add_channels.customOutputCommands = []
        outputCommands.extend (add_channels.customOutputCommands)

        ########################################################################
        # When creating a non-empty skim from skim input files, ambigious extra
        # copies of the "originalFormat" collections are created. The osu::*
        # versions are used only as transients to create plots, and should never
        # be kept.
        ########################################################################
        outputCommands.append("drop osu*_*_originalFormat_*")

        # if running over a full skim, do not recreate a full skim for passing
        # events, but rather create an empty skim (no event content, just
        # metadata); deliberate user flag can ignore this behavior
        if makeEmptySkim and not forceNonEmptySkim:
            skimFilePrefix = "emptySkim"
            outputCommands.append ("drop *")

        poolOutputModule = cms.OutputModule ("PoolOutputModule",
            overrideInputFileSplitLevels = cms.untracked.bool (True),
            splitLevel = cms.untracked.int32 (0),
            eventAutoFlushCompressedSize = cms.untracked.int32 (5242880),
            fileName = cms.untracked.string (channelName + "/" + skimFilePrefix + suffix + ".root"),
            SelectEvents = cms.untracked.PSet (SelectEvents = SelectEvents),
            outputCommands = cms.untracked.vstring (outputCommands),
            dropMetaData = cms.untracked.string ("ALL"),
        )
        add_channels.endPath += poolOutputModule
        setattr (process, channelName + "PoolOutputModule", poolOutputModule)
        ########################################################################

        setattr (process, channelName, channelPath)
        process.schedule.append(getattr(process,channelName))
    setattr (process, "endPath", add_channels.endPath)
    set_endPath(process, add_channels.endPath)

    ########################################################################
    # If MINIAOD is being used, and the egmGsfElectronIDSequence step hasn't
    # yet been added, add it here.
    ########################################################################
    if dataFormat.startswith ("MINI_AOD") and not hasattr (process, "egmGsfElectronIDSequence_step"):
        process = customizeMINIAODElectronVID(process, collections, usedCollections)

    ########################################################################
    # If MINIAOD is being used, and the egmPhotonIDSequence step hasn't
    # yet been added, add it here.
    ########################################################################
    if dataFormat.startswith ("MINI_AOD") and not hasattr (process, "egmPhotonIDSequence_step"):
        process = customizeMINIAODPhotonVID(process, collections, usedCollections)

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
    from OSUT3Analysis.Configuration.configurationOptions import composite_dataset_definitions
    # N.B. using miniAOD v2 samples by default
    from OSUT3Analysis.Configuration.miniAODV2Samples import dataset_names

    ############################################################################
    # This function configures the input dataset.
    # It can take a dataset nickname, directory, or file as argument.
    # Subsequent calls to this function will overwrite previous results.
    ############################################################################

    # print a warning if the input source has already been set
    sourceType =  type(process.source).__name__
    if sourceType != 'NoneType':
        print "WARNING [set_input]: There are multiple calls to set_input!"
        print "  The previous input source will be overwritten!"

    # initialize the process source
    datasetInfo = cms.PSet ()
    datasetInfo.name = cms.string ("NONE")
    datasetInfo.type = cms.string ("NONE")
    process.source = cms.Source ('PoolSource',
                                 fileNames = cms.untracked.vstring ()
                                 )

    # remove leading "file:" in case the user included it out of habit
    if input_string.startswith("file:"):
        input_string = input_string.lstrip("file:")


    # check for validity
    fileType = "No such file or directory"
    try:
        #fileType = subprocess.check_output(['/usr/bin/file', input_string]).split(":")[1]
        fileType = subprocess.check_output(['file', input_string]).split(":")[1]
    except:
        pass
    isValidFileOrDir = "No such file or directory" not in fileType

    isValidDataset = input_string in dataset_names.keys()
    # if the file starts with "root:" it's a AAA file
    isAAAFile = input_string.startswith("root:")


    # print error and exit if the input is invalid
    if not isValidFileOrDir and not isValidDataset and not isAAAFile:
        if input_string in composite_dataset_definitions.keys():
            print "ERROR [set_input]: '" + input_string + "' is a composite dataset"
            print "  Composite datasets should not processed interactively",
            print "because their components won't have the proper relative weights."
            print "  No files have been added to process.source.fileNames"
        else:
            print "ERROR [set_input]: '" + input_string + "' is not a valid root file, directory, or dataset name."
            print "  No files have been added to process.source.fileNames"
        return


    # use 'input_string' as a registered dataset name
    if isValidDataset:
        datasetDirectory = dataset_names[input_string]
        subprocess.call(['MySQLModule', datasetDirectory, 'temp_datasetInfo.py', 'file:'])
        import temp_datasetInfo
        process.source.fileNames.extend(cms.untracked.vstring(temp_datasetInfo.listOfFiles))
        subprocess.call(['rm', '-f', 'temp_datasetInfo.py'])
        subprocess.call(['rm', '-f', 'temp_datasetInfo.pyc'])
        return

    # open 'input_string' as a ROOT file
    if isValidFileOrDir and "ROOT" in fileType:
        process.source.fileNames.extend(cms.untracked.vstring('file:' + input_string))
        return

    # open 'input_string' as a ROOT file via xrootd
    if isAAAFile:
        process.source.fileNames.extend(cms.untracked.vstring(input_string))
        return

    # open 'input_string' as a directory
    if isValidFileOrDir and "directory" in fileType:
        for fileName in os.listdir(input_string):
            # ignore hidden files
            if fileName[0] == '.':
                continue
            if fileName.endswith(".root"):
                process.source.fileNames.extend(cms.untracked.vstring('file:' + input_string + "/" + fileName))
        return

def customizeMINIAODElectronVID(process, collections, usedCollections):
    if not hasattr (collections, "electrons") or ("electrons" not in usedCollections and "tracks" not in usedCollections):
        return process

    from PhysicsTools.SelectorUtils.tools.vid_id_tools import DataFormat,switchOnVIDElectronIdProducer,setupAllVIDIdsInModule,setupVIDElectronSelection
    switchOnVIDElectronIdProducer(process, DataFormat.MiniAOD)

    my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Spring15_25ns_V1_cff']

    if os.environ["CMSSW_VERSION"].startswith ("CMSSW_8_0_"):
        my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Summer16_80X_V1_cff']

    if os.environ["CMSSW_VERSION"].startswith ("CMSSW_9_4_"):
        my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Fall17_94X_V1_cff']
        if int(os.environ["CMSSW_VERSION"].split("_")[3]) >= 9:
            my_id_modules.extend(['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Fall17_94X_V2_cff'])

    if os.environ["CMSSW_VERSION"].startswith ("CMSSW_10_2_"):
        my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Fall17_94X_V2_cff']

    # Setup all the desired modules to be run
    for idmod in my_id_modules:
        setupAllVIDIdsInModule(process, idmod, setupVIDElectronSelection)
    process.egmGsfElectronIDSequence_step = cms.Path(process.egmGsfElectronIDSequence)
    process.schedule.insert(0, process.egmGsfElectronIDSequence_step)

    # In the case where tracks are being used in a skim where electron cuts are applied,
    # there's now two different electron collections of interest. We need to duplicate
    # the VID producer for the original collection
    if 'tracks' in usedCollections and collections.electrons.getProcessName().startswith('OSUAnalysis'):
        setattr(process, 'egmGsfElectronIDsOriginalElectrons', copy.deepcopy(getattr(process, 'egmGsfElectronIDs')))
        process.egmGsfElectronIDTaskOriginalElectrons = cms.Task(process.egmGsfElectronIDsOriginalElectrons)
        process.egmGsfElectronIDSequenceOriginalElectrons = cms.Sequence(process.egmGsfElectronIDTaskOriginalElectrons)
        process.egmGsfElectronIDSequenceOriginalElectrons_step = cms.Path(process.egmGsfElectronIDSequenceOriginalElectrons)
        process.schedule.insert(0, process.egmGsfElectronIDSequenceOriginalElectrons_step)

        # Change the InputTags for the track producers to use these duplicated VID results
        for a in dir(process):
            x = getattr(process, a)
            if not hasattr(x, "type_"):
                continue
            if x.type_() == "OSUTrackProducer" or x.type_() == "OSUSecondaryTrackProducer":
                x.eleVIDVetoIdMap.setModuleLabel(x.eleVIDVetoIdMap.getModuleLabel() + 'OriginalElectrons')
                x.eleVIDLooseIdMap.setModuleLabel(x.eleVIDLooseIdMap.getModuleLabel() + 'OriginalElectrons')
                x.eleVIDMediumIdMap.setModuleLabel(x.eleVIDMediumIdMap.getModuleLabel() + 'OriginalElectrons')
                x.eleVIDTightIdMap.setModuleLabel(x.eleVIDTightIdMap.getModuleLabel() + 'OriginalElectrons')

    # In the case of a skim, the input tag needs to be changed to that stored in the skim
    process.egmGsfElectronIDs.physicsObjectSrc = collections.electrons

    return process

def customizeMINIAODPhotonVID(process, collections, usedCollections):
    if not hasattr (collections, "photons") or "photons" not in usedCollections:
        return process

    from PhysicsTools.SelectorUtils.tools.vid_id_tools import DataFormat,switchOnVIDPhotonIdProducer,setupAllVIDIdsInModule,setupVIDPhotonSelection
    switchOnVIDPhotonIdProducer(process, DataFormat.MiniAOD)

    my_id_modules = ['RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Spring16_V2p2_cff']

    if os.environ["CMSSW_VERSION"].startswith ("CMSSW_9_4_"):
        my_id_modules = ['RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Fall17_94X_V1_cff']
        if int(os.environ["CMSSW_VERSION"].split("_")[3]) >= 13:
            my_id_modules = ['RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Fall17_94X_V2_cff']

    if os.environ["CMSSW_VERSION"].startswith ("CMSSW_10_2_"):
        my_id_modules = ['RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Fall17_94X_V2_cff']

    # Setup all the desired modules to be run
    for idmod in my_id_modules:
        setupAllVIDIdsInModule(process, idmod, setupVIDPhotonSelection)
    process.egmPhotonIDSequence_step = cms.Path(process.egmPhotonIDSequence)
    process.schedule.insert(0, process.egmPhotonIDSequence_step)

    # In the case of a skim, the input tag needs to be changed to that stored in the skim
    process.egmPhotonIDs.physicsObjectSrc = collections.photons

    return process

# e.g. removeVIDCut(process, 'cutBasedElectronID-Fall17-94X-V1-tight', 'GsfEleEffAreaPFIsoCut')
def removeVIDCut(process, idName, cutToRemove):
    if not hasattr (process, "egmGsfElectronIDs"):
        return
    for poid in process.egmGsfElectronIDs.physicsObjectIDs:
        if not poid.idDefinition.idName == idName:
            continue
        for cut in poid.idDefinition.cutFlow:
            if cut.cutName == cutToRemove:
                cut.isIgnored = cms.bool(True)

    if not hasattr (process, "egmGsfElectronIDsOriginalElectrons"):
        return
    for poid in process.egmGsfElectronIDsOriginalElectrons.physicsObjectIDs:
        if not poid.idDefinition.idName == idName:
            continue
        for cut in poid.idDefinition.cutFlow:
            if cut.cutName == cutToRemove:
                cut.isIgnored = cms.bool(True)
