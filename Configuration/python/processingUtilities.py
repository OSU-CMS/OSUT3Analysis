import os
import subprocess
import re
import sys
import math
import datetime
import time
import copy
import FWCore.ParameterSet.Modules
from optparse import OptionParser
import OSUT3Analysis.DBTools.osusub_cfg as osusub
import FWCore.ParameterSet.Config as cms
from OSUT3Analysis.Configuration.InfoPrinter_cff import *
from OSUT3Analysis.Configuration.CollectionProducer_cff import *

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

def add_channels (process, channels, histogramSets, weights, scalingfactorproducers, collections, variableProducers, skim = True):

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
        originalName = outputName = str(process.TFileService.fileName.pythonValue()).replace("'", "")  # Remove quotes.
        suffix = "_" + str(channels[0].name.pythonValue()).replace("'", "") + "_" + get_date_time_stamp()
        outputName = outputName.replace(".root", suffix + ".root")
        process.TFileService.fileName = cms.string(outputName)
        if os.path.islink (originalName):
            os.unlink (originalName)
        os.symlink (outputName, originalName)
    ############################################################################

    plotCollections = get_collections (histogramSets)

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
        if skim:
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
            if not hasattr (process, module):
                producer = cms.EDProducer (module,
                                           collections = collections
                                           )
                setattr (process, module, producer)
                variableProducerPath += producer
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
        # collections given in the collections PSet.
        ########################################################################
        outputCommands = ["drop *"]
        outputCommands.append("keep *_*_uservariables_*")
        outputCommands.append("keep *_*_eventvariables_*")
        ########################################################################
        #Check all the modules in collectionProducer, if a module is an EDProducer 
        #and have extra InputTags specified, keep the correspoding collections 
        #in the outputCommands
        ########################################################################
        for collection in dir(collectionProducer):
            if isinstance(getattr(collectionProducer,collection) ,FWCore.ParameterSet.Modules.EDProducer):    
                dic = vars(getattr(collectionProducer,collection))
                for p in dic: 
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

        collectionsToProduce = [
            "mcparticles",  # needed for gen-matching, must be produced first
        ]

        ########################################################################
        # Add an OSU object producer for each collection used in a cut or
        # histogram.
        ########################################################################
        producedCollections = copy.deepcopy (collections)
        cutCollections = get_collections (channel.cuts)
        usedCollections = sorted (list (set (cutCollections + plotCollections)))
        for collection in collectionsToProduce:
            if collection in usedCollections:
                usedCollections.remove (collection)
            if hasattr (collections, collection):
                usedCollections.insert (0, collection)
        for collection in usedCollections:
            if collection is "uservariables" or collection is "eventvariables":
                newInputTags = cms.VInputTag()
                inputTags = getattr (collections, collection)
                for inputTag in inputTags:
                    eventvariableCollections = copy.deepcopy (collections)
                    setattr (eventvariableCollections, collection,inputTag)
                    if collection != "mcparticles":
                        setattr (eventvariableCollections, "mcparticles", cms.InputTag ("objectProducer0", ""))
                    objectProducer = getattr (collectionProducer, collection).clone()
                    objectProducer.collections = eventvariableCollections
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
                if collection != "mcparticles":
                    setattr (objectProducer.collections, "mcparticles", cms.InputTag ("objectProducer0", ""))
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
        for module in scalingfactorproducers:
            # Here we try to add the original producer as specified in the config files. 
            objectProducer = cms.EDProducer (str(module['name']),
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
            # Add the eventvariables produced in this module to the filteredCollections for the plotter after to use. 
            filteredCollections.eventvariables.append(cms.InputTag ("objectProducer" + str (add_channels.producerIndex), "eventvariables"))
            add_channels.producerIndex += 1
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
#        fileType = subprocess.check_output(['/usr/bin/file', input_string]).split(":")[1]
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
