#!/usr/bin/env python
import py_compile
import imp
import sys
import os
import re
import glob
import subprocess
import pickle
import shutil
import math
from threading import Thread, Lock, Semaphore
from multiprocessing import cpu_count
from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *
from OSUT3Analysis.Configuration.formattingUtilities import *
from OSUT3Analysis.DBTools.condorSubArgumentsSet import *
import FWCore.ParameterSet.Config as cms
from ROOT import TFile

NTHREADS_FOR_BATCH_MERGING = 1

###############################################################################
#                     Make the submission script for condor.                  #
###############################################################################
def MakeSubmissionScriptForMerging(Directory, currentCondorSubArgumentsSet, split_datasets):
    SubmitFile = open(Directory + '/condorMerging.sub','w')
    wroteCPUs = False
    for argument in sorted(currentCondorSubArgumentsSet):
        if currentCondorSubArgumentsSet[argument].has_key('Executable') and currentCondorSubArgumentsSet[argument]['Executable'] == "":
            SubmitFile.write('Executable = merge.py\n')
        elif currentCondorSubArgumentsSet[argument].has_key('Arguments') and currentCondorSubArgumentsSet[argument]['Arguments'] == "":
            SubmitFile.write('Arguments = $(Process) ' + '\n\n')
        elif currentCondorSubArgumentsSet[argument].has_key('Transfer_Input_files') and currentCondorSubArgumentsSet[argument]['Transfer_Input_files'] == "":
             datasetInfoString = ''
             for dataset in split_datasets:
                 oneDataset = './' + dataset + '/datasetInfo_' + dataset +'_cfg.py'
                 if os.path.exists(os.path.join(Directory, oneDataset)):
                     datasetInfoString = datasetInfoString + oneDataset + ","
                 else:
                     print "ERROR: ", os.path.join(Directory, oneDataset), "does not exist.  Will proceed to merge other datasets."
             SubmitFile.write('Transfer_Input_files = merge.py,' + datasetInfoString + '\n')
        elif len (currentCondorSubArgumentsSet[argument].keys ()) > 0 and currentCondorSubArgumentsSet[argument].keys ()[0].lower () == "request_cpus":
            SubmitFile.write ('request_cpus = ' + str (NTHREADS_FOR_BATCH_MERGING) + '\n') # explicitly request 8 CPU cores
            wroteCPUs = True
        elif currentCondorSubArgumentsSet[argument].has_key('Queue'):
            if not wroteCPUs:
                SubmitFile.write ('request_cpus = ' + str (NTHREADS_FOR_BATCH_MERGING) + '\n') # explicitly request 8 CPU cores
            SubmitFile.write('Queue ' + str(len(split_datasets)) +'\n')
        else:
            SubmitFile.write(currentCondorSubArgumentsSet[argument].keys()[0] + ' = ' + currentCondorSubArgumentsSet[argument].values()[0] + '\n')
    SubmitFile.close()

###############################################################################
#                 Make the configuration for condor to run over.              #
###############################################################################
def MakeMergingConfigForCondor(Directory, OutputDirectory, split_datasets, IntLumi):
    MergeScript = open(Directory + '/merge.py','w')
    MergeScript.write('#!/usr/bin/env python\n')
    MergeScript.write('from OSUT3Analysis.Configuration.mergeUtilities import *\n')
    MergeScript.write('datasets = [\n')
    for dataset in split_datasets:
        MergeScript.write("'" + dataset + "',\n")
    MergeScript.write(']\n')
    MergeScript.write('Index = int(sys.argv[1])\n\n')
    MergeScript.write('dataset = datasets[Index]\n\n')
    MergeScript.write('IntLumi = ' + str(IntLumi) + '\n')
    MergeScript.write('mergeOneDataset(dataset, IntLumi, os.getcwd(), "", ' + str (NTHREADS_FOR_BATCH_MERGING) + ')\n') # use 8 CPU cores by default
    MergeScript.write("print 'Finished merging dataset ' + dataset\n")
    MergeScript.close()
    os.chmod (Directory + '/merge.py', 0755)

###############################################################################
#                       Get the exit code of condor jobs.                     #
###############################################################################
def MessageDecoder(Message, Good):
    Pattern = r'condor_(.*).log.*value (.*)\)'
    Decoded = re.match(Pattern,Message)
    report = ""
    if not Good:
        report = "Warning!!! Job " + Decoded.group(1) +" has non ZERO exit code: " + Decoded.group(2) + "\n"
    return (report, Decoded.group(1))
###############################################################################
#  Get the string of good root files and the corresponding string of weights  #
###############################################################################
def GetGoodRootFiles(Index):
    return " ".join (glob.glob ("*_" + str (Index) + ".root"))
def MakeWeightsString(Weight,FilesSet):
    Str = ''
    for i in range(0,len(FilesSet)):
        if i == 0:
            Str = Str + str(Weight)
        else:
            Str = Str + ',' + str(Weight)
    return Str
###############################################################################
#   Get the total number of events from cutFlows to calculate the weights     #
###############################################################################
def GetNumberOfEvents(FilesSet):
    NumberOfEvents = {'SkimNumber' : {}, 'TotalNumber' : 0}
    for File in FilesSet:
        ScoutFile = TFile(File)
        if ScoutFile.IsZombie():
            print File + " is a bad root file."
            FilesSet.remove(File)
            continue
        randomChannelDirectory = ""
        TotalNumberTmp = 0
        for key in ScoutFile.GetListOfKeys():
            if key.GetClassName() != "TDirectoryFile" or "CutFlow" not in key.GetName():
                continue
            randomChannelDirectory = key.GetName()
            channelName = randomChannelDirectory[0:len(randomChannelDirectory)-14]
            if not NumberOfEvents['SkimNumber'].has_key(channelName):
                NumberOfEvents['SkimNumber'][channelName] = 0
            OriginalCounterObj = ScoutFile.Get(randomChannelDirectory + "/eventCounter")
            SkimCounterObj = ScoutFile.Get(randomChannelDirectory + "/cutFlow")
            TotalNumberTmp = 0
            if not OriginalCounterObj:
                print "Could not find eventCounter histogram in " + str(File) + " !"
                continue
            elif not SkimCounterObj:
                print "Could not find cutFlow histogram in " + str(File) + " !"
            else:
                OriginalCounter = OriginalCounterObj.Clone()
                OriginalCounter.SetDirectory(0)
                TotalNumberTmp = TotalNumberTmp + OriginalCounter.GetBinContent(1)
                SkimCounter = SkimCounterObj.Clone()
                SkimCounter.SetDirectory(0)
                NumberOfEvents['SkimNumber'][channelName] = NumberOfEvents['SkimNumber'][channelName] + SkimCounter.GetBinContent(SkimCounter.GetXaxis().GetNbins())
        NumberOfEvents['TotalNumber'] = NumberOfEvents['TotalNumber'] + TotalNumberTmp
    return NumberOfEvents
###############################################################################
#                 Produce important files for the skim directory.             #
###############################################################################
def MakeFilesForSkimDirectory(Directory, DirectoryOut, TotalNumber, SkimNumber, BadIndices, FilesToRemove):
    print "in MakeFilesForSkimDirectory"
    for Member in os.listdir(Directory):
        if os.path.isfile(os.path.join(Directory, Member)):
            continue;
        try:
            os.makedirs (DirectoryOut + '/' + Member)
        except OSError:
            pass
        outfile = os.path.join(DirectoryOut, Member, 'OriginalNumberOfEvents.txt')
        try:
            os.unlink (outfile)
        except OSError:
            pass
        fout = open (outfile, "w")
        fout.write (str (TotalNumber) + "\n")
        fout.close ()
        outfile = os.path.join(DirectoryOut, Member, 'SkimNumberOfEvents.txt')
        try:
            os.unlink (outfile)
        except OSError:
            pass
        fout = open (outfile, "w")
        fout.write (str (SkimNumber[Member]) + "\n")
        fout.close ()
        os.chdir(Directory + '/' + Member)
        listOfSkimFiles = glob.glob('*.root')
        sys.path.append(Directory + '/' + Member)
        createdSkimInputTags = False
        for file in listOfSkimFiles:
            index = file.split('.')[0].split('_')[1]
            if index in BadIndices:
                continue
            if not createdSkimInputTags:
              GetSkimInputTags(file.rstrip('\n'))
              createdSkimInputTags = True
        os.chdir(Directory)
    for file in FilesToRemove:
        os.unlink (file)
###############################################################################
#           Produce a pickle file containing the skim input tags.             #
###############################################################################
def GetSkimInputTags(File):
    print "Getting skim input tags..."
    eventContent = subprocess.check_output (["edmDumpEventContent", "--all", os.getcwd () + "/" + File])
    parsing = False
    cppTypes = []
    inputTags = {}

    # First get all of the collections in the output skim file.
    for line in eventContent.splitlines ():
        if line.find ("----------") == 0:  # all of the collections will be after a line containing "---------"
            parsing = True
            continue
        if not parsing:
            continue
        splitLine = line.split ()
        cppTypes.append (splitLine[0])
        if splitLine[0] in inputTags:
            inputTags[splitLine[0]].append ( cms.InputTag (splitLine[1][1:-1], splitLine[2][1:-1], splitLine[3][1:-1]) )
        else:
            inputTags[splitLine[0]] = [ cms.InputTag (splitLine[1][1:-1], splitLine[2][1:-1], splitLine[3][1:-1]) ]

    # Figure out which collection map we're using to determine priority in input tag determination
    from OSUT3Analysis.AnaTools.osuAnalysis_cfi import dataFormat, collectionMap, collectionMapMiniAOD2017, collectionMapAOD
    if dataFormat.startswith ("MINI_AOD") and "2017" not in dataFormat:
        collectionMap = collectionMap
    elif dataFormat.startswith ("MINI_AOD_2017"):
        collectionMap = collectionMapMiniAOD2017
    elif dataFormat.startswith ("AOD"):
        collectionmap = collectionMapAOD
    else:
        print "Unknown data type, use either AOD or MINI_AOD! Cannot create SkimInputTags.pkl!"
	return

    collectionTypes = subprocess.check_output (["getCollectionType"] + cppTypes)
    # Save only the collections for which there is a valid type.
    for i in range (0, len (cppTypes)):
        if cppTypes[i] not in inputTags:
            continue
        collectionType = collectionTypes.splitlines ()[i]
        if collectionType == "INVALID_TYPE":
            inputTags.pop (cppTypes[i])
        else:
            # There may be multiple instances of the same cpp type, for example we now store both the skimmed/selected framework objects
	        # and the original (mini)AOD collections for a full skim. Another example is that slimmedJetsAK8PFPuppiSoftDropPacked and 
	        # slimmedJets exist in miniAOD and are of type pat::Jet.
	        # There is a clear hierarchy of what to use as in input: OSUAnalysis types > (mini)AOD types used in CollectionProducer_cff > (mini)AOD types > anything
	        availableTags = inputTags.pop (cppTypes[i])

	        # If there's nothing of this type available, we're in trouble
	        if len(availableTags) == 0:
                print "The collection \"" + collectionType + "\" has C++ class \"" + cppTypes[i]  + "\" and is expected in this skim file, yet no object of this C++ class is found. Something has broken, and no SkimInputTags.pkl can be created!"

            # If there's only one of this type available, take it
            if len(availableTags) == 1:
                inputTags[collectionType] = availableTags[0]
                continue
    
            # If there's something with process name OSUAnalysis, take that
            foundPreferredTag = False
            for tag in availableTags:
            if "OSUAnalysis" in tag.getProcessName ():
                inputTags[collectionType] = tag
                foundPreferredTag = True
                break
	       if foundPreferredTag:
               continue
    
	       # If there's no OSUAnalysis type available, take what's in osuAnalysis_cfi's collection map
	       for tag in availableTags:
               if hasattr (collectionMap, collectionType):
                   if getattr (collectionMap, collectionType).getModuleLabel () == tag.getModuleLabel ():
                       productInstanceLabel = getattr (collectionMap, collectionType).getProductInstanceLabel ()
                       processName = getattr (collectionMap, collectionType).getProcessName ()
                       if productInstanceLabel != '' and productInstanceLabel != tag.getProductInstanceLabel ():
                           continue
                       if processName != '' and processName != tag.getProcessName ():
                           continue
                       inputTags[collectionType] = tag
                       foundPreferredTag = True
                       break
	       if foundPreferredTag:
               continue
    
	       # If we still don't have anything, take whatever is available for this class
	       inputTags[collectionType] = availableTags[0]

    if os.path.exists("SkimInputTags.pkl"):
        os.remove("SkimInputTags.pkl")
    fout = open ("SkimInputTags.pkl", "w")
    pickle.dump (inputTags, fout)
    fout.close ()

###############################################################################
#                 Make submission script for the failed jobs.                 #
###############################################################################
def MakeResubmissionScript(badIndices, originalSubmissionScript):
    badIndices = list (set (badIndices)) # remove duplicates
    resubScript = open('condor_resubmit.sub','w')
    originalScript = open(originalSubmissionScript,'r')
    indexDependence = []

    for line in originalScript:
        if '$(Process)' not in line and 'Queue' not in line:
            resubScript.write(line)
        elif '$(Process)' in line:
            indexDependence.append(line)
            resubScript.write(line.replace('$(Process)',str(badIndices[0])))
        else:
            resubScript.write('Queue 1\n\n')

    for index in badIndices:
        for item in indexDependence:
            resubScript.write(item.replace('$(Process)',str(index)))
        resubScript.write('Queue 1\n\n')

    resubScript.close()
    originalScript.close()

    resubScript = open('condor_resubmit.sh','w')
    resubScript.write ("#!/usr/bin/env bash\n")
    resubScript.write ("\n")
    for index in badIndices:
        resubScript.write ("rm -f condor_" + str(index) + ".*\n")
    resubScript.close ()
    os.chmod ("condor_resubmit.sh", 0755)
###############################################################################
#                       Determine whether a skim file is valid.               #
###############################################################################
def SkimFileValidator(File):
    print "testing ", File
    FileToTest = TFile(File)
    Valid = True
    for TreeToTest in ['MetaData', 'ParameterSets', 'Parentage', 'Events', 'LuminosityBlocks', 'Runs']:
        Valid = Valid and (FileToTest.Get(TreeToTest) != None)
    InvalidOrEmpty = not Valid or not FileToTest.Get ("Events").GetEntries ()
    return Valid, InvalidOrEmpty

###############################################################################
#                   Merge an intermediate file with a thread.                 #
###############################################################################
threadLog = ""
outputFiles = []
lock = Lock () # to control appending to threadLog and outputFiles
semaphore = None # to control how many merging threads are active at a given time
def MergeIntermediateFile (files, outputDir, dataset, weight, threadIndex, verbose):
    global threadLog
    global outputFiles
    global lock
    global semaphore
    localThreadLog = ""

    weightString = MakeWeightsString(weight, files)
    outputFile = dataset + '_' + str (threadIndex) + '.root'
    cmd = 'mergeTFileServiceHistograms -i ' + " ".join (files) + ' -o ' + outputDir + "/" + outputFile + ' -w ' + weightString

    semaphore.acquire ()
    if verbose:
        print "Executing: ", cmd
    try:
        localThreadLog += subprocess.check_output (cmd.split (), stderr = subprocess.STDOUT)
    except subprocess.CalledProcessError as e:
        localThreadLog += e.output
    semaphore.release ()

    lock.acquire ()
    threadLog += localThreadLog
    outputFiles.append (outputDir + '/' + outputFile)
    lock.release ()

###############################################################################
#                       Main function to do merging work.                     #
###############################################################################
def mergeOneDataset(dataSet, IntLumi, CondorDir, OutputDir="", nThreadsActive = cpu_count () + 1, verbose = False):
    global threadLog
    global outputFiles
    global semaphore

    os.chdir(CondorDir)
    directory = CondorDir + '/' + dataSet
    if not os.path.exists(directory):
        print directory + " does not exist, will skip it and continue!"
        return
    if not OutputDir:
        OutputDir = CondorDir
    directoryOut = OutputDir + "/" + dataSet  # Allow writing output to a different directory
    try:
        os.makedirs (directoryOut)
    except OSError:
        pass
    log = "....................Merging dataset " + dataSet + " ....................\n"
    os.chdir(directory)
    if verbose:
        print "Moved to directory: ", directory
    ReturnValues = []
    StdErrErrors = []
    if os.path.islink(directory + '/hist.root'):
        os.unlink (directory + '/hist.root')
    # check to see if any jobs ran
    if not len(glob.glob('condor_*.log')):
        print "no jobs were run for dataset '" + dataSet + "', will skip it and continue!"
        return
    LogFiles = os.popen('ls condor_*.log').readlines()
    if verbose:
        print "parsing log files to find good jobs"

    for i in range(0,len(LogFiles)):
        ReturnValues.append('condor_' + str(i) + '.log' + str(os.popen('grep -E "return value|condor_rm|Abnormal termination" condor_' + str(i)  + '.log | tail -1').readline().rstrip('\n')))

    GoodIndices = []
    GoodRootFiles = []
    BadIndices = []

    # check for files that weren't found and were skipped
    StdErrFiles = sorted(glob.glob('condor_*.err'))

    for file in StdErrFiles:
        index = file.split("_")[-1].split(".")[0]
        if 'was not found or could not be opened, and will be skipped.' in open(file).read():
            BadIndices.append(index)
            if verbose:
                print "  job" + ' ' * (4-len(str(index))) + index + " had bad/skipped input file"


    # check for any corrupted skim output files
    skimDirs = [member for member in  os.listdir(os.getcwd()) if os.path.isdir(member)]
    FilesToRemove = []
    for channel in skimDirs:
        for skimFile in glob.glob(channel+'/*.root'):
            # don't check for good skims of jobs we already know are bad
            index = skimFile.split('.')[0].split('_')[1]
            if index in BadIndices:
                continue
            Valid, InvalidOrEmpty = SkimFileValidator(skimFile.rstrip('\n'))
            if not Valid:
                BadIndices.append(index)
                if verbose:
                    print "  job" + ' ' * (4-len(str(index))) + index + " had bad skim output file"
            if InvalidOrEmpty:
                FilesToRemove.append (skimFile)


    # check for abnormal condor return values
    sys.path.append(directory)
    for returnValue in ReturnValues:
        if "return value 0" in returnValue:
            (report, GoodIndex) = MessageDecoder(returnValue, True)
            if GoodIndex not in BadIndices:
                GoodIndices.append(GoodIndex)
        elif "return value" in returnValue:
            log += returnValue + "\n"
            (report, BadIndex) = MessageDecoder(returnValue, False)
            log += report
            BadIndices.append(BadIndex)
        else:
            log += returnValue + "\n"
            Pattern = r'condor_(.*).log'
            Decoded = re.match(Pattern,returnValue)
            BadIndex = Decoded.group(1)
            log += "Warning!!! Job " + str(BadIndex) + " exited inproperly!\n"
            BadIndices.append(BadIndex)
    if os.path.exists('condor_resubmit.sub'):
        os.remove('condor_resubmit.sub')
    if os.path.exists('condor_resubmit.sh'):
        os.remove('condor_resubmit.sh')
    if BadIndices:
        MakeResubmissionScript(BadIndices, 'condor.sub')
    for i in GoodIndices:
        GoodRootFiles.append(GetGoodRootFiles(i))
    if not len(GoodRootFiles):
        print "For dataset", dataSet, ": Unfortunately there are no good root files to merge!\n"
        return
    exec('import datasetInfo_' + dataSet + '_cfg as datasetInfo')

    TotalNumber = GetNumberOfEvents(GoodRootFiles)['TotalNumber']
    SkimNumber = GetNumberOfEvents(GoodRootFiles)['SkimNumber']
    if verbose:
        print "TotalNumber =", TotalNumber, ", SkimNumber =", SkimNumber
    if not TotalNumber:
        MakeFilesForSkimDirectory(directory, directoryOut, TotalNumber, SkimNumber, BadIndices, FilesToRemove)
        return
    Weight = 1.0
    crossSection = float(datasetInfo.crossSection)
    runOverSkim = True
    try:
        datasetInfo.originalNumberOfEvents
    except AttributeError:
        runOverSkim = False
    if crossSection > 0 and IntLumi > 0:
        if runOverSkim and float(datasetInfo.originalNumberOfEvents)*float(TotalNumber):
            Weight = IntLumi*crossSection*float(datasetInfo.skimNumberOfEvents)/(float(datasetInfo.originalNumberOfEvents)*float(TotalNumber))
            # The factor TotalNumber / skimNumberOfEvents corresponds to the fraction of skim events that were actually processed,
            # i.e., it accounts for the fact that perhaps not all of the jobs finished successfully.
        elif float(TotalNumber):
            Weight = IntLumi*crossSection/float(TotalNumber)
    InputWeightString = MakeWeightsString(Weight, GoodRootFiles)
    if runOverSkim:
        MakeFilesForSkimDirectory(directory, directoryOut, datasetInfo.originalNumberOfEvents, SkimNumber, BadIndices, FilesToRemove)
    else:
        MakeFilesForSkimDirectory(directory, directoryOut, TotalNumber, SkimNumber, BadIndices, FilesToRemove)

    threadLog = ""
    outputFiles = []
    semaphore = Semaphore (nThreadsActive)

    # start threads, each of which merges some fraction of the input files
    filesPerThread = 100 # to be adjusted if need be
    nThreadTarget = cpu_count () + 1
    nFiles = len (GoodRootFiles)
    nThreads = int (math.ceil (float (nFiles) / float (filesPerThread)))
    if nThreads < nThreadTarget:
        nThreads = min (nThreadTarget, nFiles)
        filesPerThread = int (math.ceil (float (nFiles) / float (nThreads)))
    threads = []
    for i in range (0, nThreads):
        fileSubset = GoodRootFiles[(i * filesPerThread):((i + 1) * filesPerThread)]
        if len (fileSubset):
            threads.append (Thread (target = MergeIntermediateFile, args = (fileSubset, OutputDir, dataSet, Weight, i, verbose)))
            threads[-1].start ()
    for thread in threads:
        thread.join ()
    log += threadLog

    # merge the intermediate files produced by the threads above
    cmd = 'mergeTFileServiceHistograms -i ' + " ".join (outputFiles) + ' -o ' + OutputDir + "/" + dataSet + '.root'
    if verbose:
        print "Executing: ", cmd
    try:
        log += subprocess.check_output (cmd.split (), stderr = subprocess.STDOUT)
    except subprocess.CalledProcessError as e:
        log += e.output

    # remove the intermediate files produced by the threads above
    for outputFile in outputFiles:
        os.unlink (outputFile)

    log += "\nFinished merging dataset " + dataSet + ":\n"
    log += "    "+ str(len(GoodRootFiles)) + " good files are used for merging out of " + str(len(LogFiles)) + " submitted jobs.\n"
    log += "    "+ str(TotalNumber) + " events were successfully run over.\n"
    log += "    The target luminosity is " + str(IntLumi) + " inverse pb.\n"
    if crossSection != -1:
        log += "    The crossSection of dataset " + dataSet + " is " + str(crossSection) + " pb.\n"
    log += "    The weighting factor is " + str(Weight) + ".\n"
    if crossSection != -1:
        log += "    " + str(Weight*TotalNumber) + " weighted events and the effective luminosity is " + str(IntLumi/Weight) + " inverse pb.\n"
    log += "...............................................................\n"
    os.chdir(CondorDir)
    flogName = directoryOut + '/mergeOut.log'
    flog = open (flogName, "w")
    flog.write(log)
    flog.close()
    fin = open (flogName)
    shutil.copyfileobj (fin, sys.stdout)
    fin.close ()
