#!/usr/bin/env python
import sys
import getpass
import os
import re
import socket
import time
import copy
from math import *
from array import *
from optparse import OptionParser
from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *
from OSUT3Analysis.Configuration.formattingUtilities import *
from OSUT3Analysis.DBTools.condorSubArgumentsSet import *

parser = OptionParser()
parser = set_commandline_arguments(parser)

parser.remove_option("-o")
parser.remove_option("-c")
parser.remove_option("-n")
parser.remove_option("-u")
parser.remove_option("-e")
parser.remove_option("-r")
parser.remove_option("-R")
parser.remove_option("-d")
parser.remove_option("-b")
parser.remove_option("--2D")
parser.remove_option("-y")
parser.remove_option("-p")


parser.add_option("-r", "--randomSeed", action="store_true", dest="Random", default=False, help="Assign random seeds for each job.")
parser.add_option("-f", "--fileName", dest="FileName", default = 'process.TFileService.fileName', help="Set the parameter of output filename in config file.")
parser.add_option("-m", "--maxEvents", dest="MaxEvents", default = -1, help="Set the maximum number of events to run per job.")
parser.add_option("-p", "--process", dest="Process", default = '', help="Set the suffix for the process name.")
parser.add_option("-t", "--typeOfSource", dest="FileType", default = 'OSUT3Ntuple', help="Specify the type of input files.  Options:  OSUT3Ntuple, UserDir, UserList.")
parser.add_option("-d", "--dataset", dest="Dataset", default = "", help="Specify which dataset to run.")  # Dataset is also the name of the output directory in the working directory if FileType == 'OSUT3Ntuple'.  
parser.add_option("-w", "--workDirectory", dest="Directory", default = "", help="Specify the working directroy.")
parser.add_option("-c", "--configuration", dest="Config", default = "", help="Specify the configuration file to run.")
parser.add_option("-n", "--numberOfJobs", dest="NumberOfJobs", default = -1, help="Specify how many jobs to submit.")
parser.add_option("-u", "--userCondorSubFile", dest="CondorSubFilr", default = "", help="Specify the condor.sub file you want to use if you have to add arguments.")
parser.add_option("-U", "--uniqueEventId", action="store_true", dest="Unique", default=False, help="Assign unique and continuos event IDs")
parser.add_option("-N", "--noExec", action="store_true", dest="NotToExecute", default = False, help="Just generate necessary config files without executing them.")
parser.add_option("-L", "--Label", dest="Label", default = "", help="Give the dataset a short label.")
parser.add_option("-s", "--SkimDirectory", dest="SkimDirectory", default = "", help="Specicy the location of the skim.")
parser.add_option("-a", "--SkimChannel", dest="SkimChannel", default = "", help="Determine the skim channel to run over.")
parser.add_option("-R", "--Requirements", dest="Requirements", default = "", help="Requirements to be added to condor.sub submssion script, e.g. 'Memory > 1900'.")  
parser.add_option("-x", "--crossSection", dest="crossSection", default = "", help="Provide cross section to the given dataset.")  
parser.add_option("-A", "--UseAAA", dest="UseAAA", action="store_true", default = False, help="Use AAA.")  

(arguments, args) = parser.parse_args()

def GetCommandLineString():
    # Return string of all arguments specified on the command line
    commandLine = ""  
    for arg in sys.argv:
        if len(arg.split(" ")) > 1:
            commandLine = commandLine + " \"" + arg + "\""  # add quotation marks around a multiple-word argument  
        else:
            commandLine = commandLine + " " + arg
    return commandLine  

def GetListOfRootFiles(Directory):
    fileList = os.popen('ls ' + Directory + '/*.root').read().split('\n')  # remove '\n' from each word  
    for f in fileList:
        if len(f) is 0: 
            fileList.remove(f)   # remove empty filename
    return fileList  
 

def MakeCondorSubmitScript(Dataset,NumberOfJobs,Directory,Label, UseAAA):
    os.system('touch ' + Directory + '/condor.sub')
    SubmitFile = open(Directory + '/condor.sub','r+w')
    cmsRunExecutable = os.popen('which cmsRun').read()
    SubmitFile.write("# Command line arguments: \n# " + GetCommandLineString() + " \n\n\n")  
    for argument in sorted(CondorSubArgumentsSet):
        if CondorSubArgumentsSet[argument].has_key('Executable') and CondorSubArgumentsSet[argument]['Executable'] == "":
            SubmitFile.write('Executable = ' + cmsRunExecutable + '\n')
        elif CondorSubArgumentsSet[argument].has_key('Arguments') and CondorSubArgumentsSet[argument]['Arguments'] == "":
            SubmitFile.write('Arguments = config_cfg.py True ' + str(NumberOfJobs) + ' $(Process) ' + Dataset + ' ' + Label + '\n\n')
        elif CondorSubArgumentsSet[argument].has_key('Transfer_Input_files') and CondorSubArgumentsSet[argument]['Transfer_Input_files'] == "":   
            if Dataset == '':
                SubmitFile.write('Transfer_Input_files = config_cfg.py,userConfig_cfg.py\n')
    	    elif UseAAA:
	        userName = getpass.getuser()
                userId = os.popen('id -u ' + userName).read()
                userProxy = '/tmp/x509up_u' + str(userId)
                SubmitFile.write('x509userproxy = ' + userProxy + '\n')
                SubmitFile.write('should_transfer_files   = YES\n')
                SubmitFile.write('Transfer_Input_files = config_cfg.py,userConfig_cfg.py,datasetInfo_' + Label + '_cfg.py,' + userProxy + '\n')
            else:
                SubmitFile.write('Transfer_Input_files = config_cfg.py,userConfig_cfg.py,datasetInfo_' + Label + '_cfg.py\n')
        elif CondorSubArgumentsSet[argument].has_key('Requirements') and arguments.Requirements:
            SubmitFile.write('Requirements = ' + arguments.Requirements + '\n')
        elif CondorSubArgumentsSet[argument].has_key('Queue'):
            SubmitFile.write('Queue ' + str(NumberOfJobs) +'\n')
        else:
            SubmitFile.write(CondorSubArgumentsSet[argument].keys()[0] + ' = ' + CondorSubArgumentsSet[argument].values()[0] + '\n')
    SubmitFile.close()

def MakeSpecificConfig(Dataset, Directory):
    os.system('touch ' + Directory + '/config_cfg.py')
    SkimChannelNames = SkimChannelFinder('userConfig_cfg', Directory) 
    ConfigFile = open(Directory + '/config_cfg.py','r+w')
    ConfigFile.write('import FWCore.ParameterSet.Config as cms\n')        
    ConfigFile.write('import OSUT3Analysis.DBTools.osusub_cfg as osusub\n')
    ConfigFile.write('import re\n')
    ConfigFile.write('import userConfig_cfg as pset\n')
    ConfigFile.write('\n')
    for channelName in SkimChannelNames:
        StringToAdd = 'pset.process.' + channelName + 'PoolOutputModule.fileName = cms.untracked.string(\'' + channelName +'/skim_\'' +'+ str (osusub.jobNumber)' + '+ \'.root\')\n'
        ConfigFile.write(StringToAdd)
    ConfigFile.write('fileName = pset.' + arguments.FileName + '\n')
    ConfigFile.write('fileName = fileName.pythonValue ()\n')
    ConfigFile.write('fileName = fileName[1:(len (fileName) - 1)]\n')
    ConfigFile.write('fileName = re.sub (r\'^(.*)\.([^\.]*)$\', r\'\\1_\' + str (osusub.jobNumber) + r\'.\\2\', fileName)\n')
    ConfigFile.write('pset.' + arguments.FileName + ' = fileName\n')
    ConfigFile.write('\n')
    if Dataset != '':
	ConfigFile.write('pset.process.source.fileNames = cms.untracked.vstring (osusub.runList)\n')
        #If there are input datasets, on could set the MaxEvents to be -1.
        if EventsPerJob < 0:
            ConfigFile.write('pset.process.maxEvents.input = cms.untracked.int32 (' + str(EventsPerJob) + ')\n')  
    #If there are no input datasets, one needs a positive MaxEvents.
    if EventsPerJob > 0:
        ConfigFile.write('pset.process.maxEvents.input = cms.untracked.int32 (' + str(EventsPerJob) + ')\n')
    ConfigFile.write('process = pset.process\n')
    if arguments.Process:
	ConfigFile.write('process.setName_ (process.name_ () + \'' + arguments.Process + '\')\n')
    #else:
#	ConfigFile.write('process.setName_ (process.name_ () + \'' + str(arguments.Directory).replace('_','')  + '\')\n') 
    if arguments.Random:
        ConfigFile.write('pset.process.RandomNumberGeneratorService.generator.initialSeed = osusub.jobNumber\n')
    if arguments.Unique:
        # Specify a different lumi section for each job so that all events have unique run / lumi / event numbers.  
        # Instead of changing lumi section, could also change run number.  
        ConfigFile.write('pset.process.source.firstLuminosityBlock = cms.untracked.uint32((osusub.jobNumber)+1) \n') # osusub.jobNumber starts with 0  
    ConfigFile.close()

def AquireAwesomeAAA(Dataset, datasetInfoName, AAAFileList, datasetRead, crossSection):
    os.system('das_client.py --query="file dataset=' + Dataset + '" --limit 0 > ' + AAAFileList) 
    if lxbatch:
        os.system('sed -i \'s/^/root:\/\/xrootd.ba.infn.it\//g\' ' + datasetInfoName) 
    else:
	os.system('sed -i \'s/^/root:\/\/cmsxrootd.fnal.gov\//g\' ' + datasetInfoName)
    inputFileList = open(AAAFileList, "r") 
    inputFiles = inputFileList.read().split('\n')  
    for f in reversed(range(len(inputFiles))): 
        if not ".root" in inputFiles[f]: 
            del inputFiles[f]  
    datasetRead['numberOfFiles'] = len(inputFiles)  
    datasetRead['realDatasetName'] = Dataset 
    text = 'listOfFiles = [  \n' 
    for f in inputFiles:
        if lxbatch:
            f = "root://xrootd.ba.infn.it/" + f
        else: 
            f = "root://cmsxrootd.fnal.gov/" + f 
        text += '"' + f + '",\n'  
    text += ']  \n'  
    text += 'numberOfFiles = ' + str(datasetRead['numberOfFiles']) + '\n'          
    text += 'datasetName = \'' + str(Dataset) +'\'\n'
    text += 'crossSection = ' + str(crossSection) + '\n'          
        
    fnew = open(datasetInfoName, "w") 
    fnew.write(text)
    fnew.close()  
    
def MakeFileList(Dataset, FileType, Directory, Label, UseAAA, crossSection):
    numberOfFiles = -1
    datasetRead = {}
    datasetRead['useAAA'] = UseAAA
    runList = []
    datasetInfoName = Directory + '/datasetInfo_' + Label + '_cfg.py'
    AAAFileList = Directory + '/AAAFileList.txt'
    os.system('touch ' + datasetInfoName)  
    os.system('touch ' + AAAFileList)  
    if FileType == 'OSUT3Ntuple' and UseAAA:
         AquireAwesomeAAA(Dataset, datasetInfoName, AAAFileList, datasetRead, crossSection)
    if FileType == 'UserDir':
        isInCondorDir = False
        SubmissionDir = os.getcwd() 
        if not "/" in Dataset:
            # Then assume it is in condor/ directory.  
            Dataset = "condor/" + Dataset  
            isInCondorDir = True 
	if not os.path.exists(Dataset):
	    print "The directory you provided does not exist: ", Dataset  
            sys.exit()
        #Get the list of the root files in the directory and modify it to have the standard format. 
        inputFiles = GetListOfRootFiles(Dataset)  
        datasetRead['numberOfFiles'] = len(inputFiles)  
        datasetRead['realDatasetName'] = 'FilesInDirectory:' + Dataset 
        text = 'listOfFiles = [  \n' 
        if not UseAAA:
            for f in inputFiles:
                if remoteAccessT3:
                    f = "root://cms-0.mps.ohio-state.edu:1094/" + f
                else: 
                    if isInCondorDir:
                        f = SubmissionDir + "/" + f  
                    f = "file:" + f 
                text += '"' + f + '",\n'  
            text += ']  \n'  
        else:
            for f in inputFiles:
                if lxbatch:
                    f = "root://xrootd.ba.infn.it/" + f   
                else:
                    f = "root://cmsxrootd.fnal.gov/" + f
                text += '"' + f + '",\n'  
            text += ']  \n'  
        text += 'numberOfFiles = ' + str(datasetRead['numberOfFiles']) + '\n' 
        text += 'crossSection = ' + str(crossSection) + '\n'          
        fnew = open(datasetInfoName, "w") 
        fnew.write(text)
        fnew.close()  
    if FileType == 'UserList':
	if not os.path.exists(Dataset):
            print "The list you provided does not exist."
            sys.exit()
        #Get the list of the files to datasetInfo_cfg.py and modify it to have the standard format. 
        inputFileList = open(Dataset, "r") 
        inputFiles = inputFileList.read().split('\n')  
        for f in reversed(range(len(inputFiles))): 
            if not ".root" in inputFiles[f]: 
                del inputFiles[f]  
        datasetRead['numberOfFiles'] = len(inputFiles)  
        datasetRead['realDatasetName'] = 'FilesInDirectory:' + Dataset 
        text = 'listOfFiles = [  \n' 
        if not UseAAA:
            for f in inputFiles:
                if remoteAccessT3:
                    f = "root://cms-0.mps.ohio-state.edu:1094/" + f
                else: 
                    if isInCondorDir:
                        f = SubmissionDir + "/" + f  
                    f = "file:" + f 
                text += '"' + f + '",\n'  
            text += ']  \n'  
        else:
            for f in inputFiles:
                if lxbatch:
                    f = "root://xrootd.ba.infn.it/" + f   
                else:
                    f = "root://cmsxrootd.fnal.gov/" + f
                text += '"' + f + '",\n'  
            text += ']  \n'  
        text += 'numberOfFiles = ' + str(datasetRead['numberOfFiles']) + '\n'     
        text += 'crossSection = ' + str(crossSection) + '\n'          
        fnew = open(datasetInfoName, "w") 
        fnew.write(text)
        fnew.close()  
    if FileType == 'OSUT3Ntuple':
        prefix = ''
        if not remoteAccessT3 :
	    prefix = 'file:' 
        else:
            prefix = 'root://cms-0.mps.ohio-state.edu:1094/'
        if RunOverSkim:
            print "You have specified a skim as input.  Will obtain cross sections from the database."  
        #Use MySQLModule, a perl script to get the information of the given dataset from T3 DB and save it in datasetInfo_cfg.py. 
        os.system('MySQLModule ' + Dataset + ' ' + datasetInfoName + ' ' + prefix)
        NTupleExistCheck = os.popen('cat ' + datasetInfoName).read()
        InitializeAAA = ""
        if NTupleExistCheck == 'Dataset does not exist on the Tier 3!': 
            InitializeAAA = raw_input('The dataset ' + Dataset + ' is not available on T3, do you want to access it via xrootd?("y" to continue or "n" to skip)')    
            if InitializeAAA == "y":
                AquireAwesomeAAA(Dataset, datasetInfoName, AAAFileList, datasetRead, crossSection)
                datasetRead['useAAA'] = True
            else:
                return 
        if RunOverSkim:
            SkimModifier(Label, Directory)
        sys.path.append(Directory)
        exec('import datasetInfo_' + Label +'_cfg as datasetInfo')
        if InitializeAAA == "":
            status = datasetInfo.status
            continueForNonPresentDataset = True
            if not status == 'present':
	        userDecision = raw_input('The dataset you selected is not marked as present on Tier3, do you still want to continue?(Type "y" for yes and "n" for no.)')
	        if userDecision == "n":
                    continueForNonPresentDataset = False 
            if not continueForNonPresentDataset:
	        return    
        datasetRead['realDatasetName'] = datasetInfo.datasetName
        datasetRead['numberOfFiles'] = datasetInfo.numberOfFiles
    return  datasetRead

def MakeBatchJobFile(WorkDir, Queue, NumberOfJobs):
    os.system('touch ' + currentDir + '/' + WorkDir + '/lxbatchSub.sh')
    os.system('touch ' + currentDir + '/' + WorkDir + '/lxbatchRun.sh')
    LxBatchSubFile = open(currentDir + WorkDir + '/lxbatchSub.sh','r+w')
    LxBatchSubFile.write('#!/bin/sh \n') 
    LxBatchSubFile.write('for i in {1..' + NumberOfJobs + '} \n') 
    LxBatchSubFile.write('do \n') 
    LxBatchSubFile.write(' bsub -q ' + Queue + ' -oo ' + currentDir + '/' + WorkDir + '/lxbatch_\$i.log ' + currentDir + '/' + WorkDir + '/lxbatchRun.sh \$i \n') 
    LxBatchSubFile.write('done \n') 
    LxBatchSubFile.write('echo \"Finished submitting ' + NumberOfJobs + ' jobs.\" \n') 
    LxBatchSubFile.close()
    LxBatchRunFile = open(currentDir + WorkDir + '/lxbatchRun.sh','r+w')
    LxBatchRunFile.write('#!/bin/sh \n')
    LxBatchRunFile.write('cd ' + currentDir + '/' + WorkDir + '\n')
    LxBatchRunFile.write('eval `scram runtime -sh` \n')
    LxBatchRunFile.write('cmsRun config_cfg.py True ' + NumberOfJobs + ' \$1 NULL NULL \n')
    LxBatchRunFile.close()
    os.system('chmod +x ' + currentDir + '/' + WorkDir + '/lxbatchRun.sh' )
    os.system('chmod +x ' + currentDir + '/' + WorkDir + '/lxbatchSub.sh' )

def GetCompleteOrderedArgumentsSet(InputArguments):
    NewArguments = copy.deepcopy(InputArguments)
    for argument in InputArguments:
	for index in CondorSubArgumentsSet:
	    if CondorSubArgumentsSet[index].has_key(argument):
                CondorSubArgumentsSet[index][argument] = InputArguments[argument]
                NewArguments.pop(argument)
                break
    for newArgument in NewArguments:
        CondorSubArgumentsSet.setdefault(len(CondorSubArgumentsSet.keys()) + 1,{newArgument : NewArguments[newArgument]})

###############################################################################
#        Function to find all the skim channels from the userConfig.          # 
###############################################################################
def SkimChannelFinder(userConfig, Directory):
    sys.path.append(Directory)
    configTmp = open(Directory + '/' +userConfig + '.py','r')
    skimChannelNames = []
    for line in configTmp:
        if "add_channels" not in line:
            continue
        elif "False" in line:
            continue
        elif line[0] == "#":
            continue
        else:
            PatternOne = r'.*\[(.*)\].*'
            DecodedOne = re.match(PatternOne,line)
            channel = DecodedOne.group(1)
            exec('from ' + userConfig + ' import ' + channel + ' as currentChannel')
            PatternTwo = r'.*\'(.*)\'.*'
            DecodedTwo = re.match(PatternTwo,str(currentChannel.name))
            skimChannelNames.append(DecodedTwo.group(1))
    return skimChannelNames

################################################################################
#            Function to modify the dataset_*_Info_cfy file for skim.          #  
################################################################################
def SkimModifier(Label, Directory):
    SkimDirectory = Condor + str(arguments.SkimDirectory) + '/' + str(Label) + '/' + str(arguments.SkimChannel)
    OriginalNumberOfEvents = os.popen('cat ' + SkimDirectory + '/OriginalNumberOfEvents.txt').read().split()[0]  
    SkimNumberOfEvents = os.popen('cat ' + SkimDirectory + '/SkimNumberOfEvents.txt').read().split()[0] 

    os.system('cp ' + Directory + '/datasetInfo_' + Label + '_cfg.py ' + Directory + '/datasetInfo_' + Label + '_cfg_beforeChanges.py ')
    infoFile = Directory + '/datasetInfo_' + Label + '_cfg.py'
    infoFileTest = infoFile.replace(".py", "-test.py")
    fin = open(infoFile, "r")
    orig = fin.read()
    fin.close()
    orig = orig.replace("listOfFiles",   "originalListOfFiles")  
    orig = orig.replace("numberOfFiles", "originalNumberOfFiles")  
    skimFiles = GetListOfRootFiles(SkimDirectory)
    add  = "\n"
    add += 'skimDirectory = "' + SkimDirectory + '"\n'  
    add += 'listOfFiles = [  \n' 
    for s in skimFiles:
        add += '"file:' + s + '",\n'  
    add += ']  \n'  
    add += 'numberOfFiles = ' + str(len(skimFiles)) + '\n'  
    add += 'originalNumberOfEvents = ' + str(OriginalNumberOfEvents) + '\n'
    add += 'skimNumberOfEvents = ' + str(SkimNumberOfEvents) + '\n'
    fnew = open(infoFileTest, "w")
    fnew.write(orig + add)
    fnew.close()  



################################################################################
#             First of all to set up the working directory                     #
################################################################################
CondorDir = ''
Condor = os.getcwd() + '/condor/'
if not os.path.exists(Condor):
    print "The directory ", Condor, "does not exist.  Aborting."  
    sys.exit()
if arguments.Directory == "":
    print "No working directory is given, aborting."
    sys.exit()
else:
    CondorDir = Condor + arguments.Directory
#Check whether the directory specified already exists and warn the user if so.
if not os.path.exists(CondorDir):
    os.system('mkdir ' + CondorDir)
else:
    if arguments.FileType == "OSUT3Ntuple":
        # Ok to proceed, since the working directory will be "CondorDir/dataset"
        print 'Directory "' + str(CondorDir) + '" already exists in your condor directory. Will proceed with job submission.' 
    else:
        # Do not proceed because the working directory is CondorDir, and we do not want to overwrite existing files.  
        print "Directory", CondorDir, " already exists.  Please remove it before proceeding."  
        sys.exit()  
RunOverSkim = False
if arguments.SkimDirectory != "" and arguments.SkimChannel != "":
    RunOverSkim = True
elif arguments.SkimDirectory == "" and arguments.SkimChannel == "":
    RunOverSkim = False
else:
    print "Both skim directory and skim channel should be provided."

###############################################################################
# Find the list of dataset to run over, either from arguments or localConfig. #
###############################################################################
split_datasets = []

        
if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.localConfig) + " import *")
    split_datasets = split_composite_datasets(datasets, composite_dataset_definitions)

#Check whether the necessary options are correctly given if no local config is given.
if not arguments.localConfig:
    if not arguments.NumberOfJobs > 0:
    	print "Invalid number of jobs, aborting."
        sys.exit() 
    if arguments.Config == "":
    	print "No cmsRun executable is given, aborting."
        sys.exit() 
    if arguments.Dataset == "":
    	print "Warning, you are running batch jobs witout using input sources."
    else:
	split_datasets.append(arguments.Dataset)   

###############################################################################
#    Get the host name to determine whether you are using lxplus or OSU T3.   #
###############################################################################
UseAAA = False
if arguments.UseAAA:
    UseAAA = True
remoteAccessT3 = True
lxbatch  = False
hostname = socket.gethostname()
if 'cern.ch' in hostname:
    lxbatch = True
if 'interactive' in hostname:
    remoteAccessT3 = False
###############################################################################
#                End of Setup stage, will begin to submit jobs                #
###############################################################################

#Loop over the datasets in split_datasets.
if split_datasets:
    for dataset in split_datasets:
        EventsPerJob = -1 
        DatasetName = dataset
        NumberOfJobs = -1
        DatasetRead = {}
        MaxEvents = arguments.MaxEvents
        Config =  arguments.Config
        if arguments.localConfig:	
            NumberOfJobs = nJobs[dataset]
            DatasetName = dataset_names[dataset]
            MaxEvents = maxEvents[dataset]
            Config = config_file
            GetCompleteOrderedArgumentsSet(InputCondorArguments)
        SubmissionDir = os.getcwd()
        if arguments.FileType == 'OSUT3Ntuple': 
            DatasetName = dataset_names[dataset]
            WorkDir = CondorDir + '/' + str(dataset)
            if os.path.exists(WorkDir): 
                print 'Directory "' + str(WorkDir) + '" already exists.  Please remove it and resubmit.'  
                continue 
            else: 
                os.system('mkdir ' + WorkDir )
        else:
            WorkDir = CondorDir 

        if '/' in dataset:
	    if dataset.split('/')[0] == "":
                dataset = dataset.split('/')[1]
            else:
                dataset = dataset.split('/')[0]
            if '_' in dataset:
	        if dataset.split('_')[0] == "":
                    dataset = dataset.split('_')[1]
                else:
                    dataset = dataset.split('_')[0]
	        if '-' in dataset:    
                    if dataset.split('-')[0] == "":
                        dataset = dataset.split('-')[1]
                    else:
                        dataset = dataset.split('-')[0]
        crossSection = -1
        if crossSections.has_key(dataset):
            crossSection = crossSections[dataset]       
        elif arguments.crossSection != "":
            crossSection = arguments.crossSection
        DatasetRead = MakeFileList(DatasetName,arguments.FileType,WorkDir,dataset, UseAAA, crossSection)
        NumberOfFiles = int(DatasetRead['numberOfFiles'])
        UseAAA = DatasetRead['useAAA']
        if NumberOfJobs > NumberOfFiles:
            NumberOfJobs = NumberOfFiles
        if not arguments.localConfig:    
            NumberOfJobs = int(math.ceil(NumberOfFiles/math.ceil(NumberOfFiles/float(arguments.NumberOfJobs))))
        if MaxEvents > 0:
	    EventsPerJob = int(math.ceil(int(arguments.MaxEvents)/NumberOfJobs)) 	

        RealMaxEvents = EventsPerJob*NumberOfJobs

        os.system('cp ' + Config + ' ' + WorkDir + '/userConfig_cfg.py')

        MakeSpecificConfig(DatasetRead['realDatasetName'],WorkDir)

	if lxbatch:
	    MakeBatchJobFile(WorkDir, Queue, NumberOfJobs)
	else:
            MakeCondorSubmitScript(DatasetRead['realDatasetName'],NumberOfJobs,WorkDir,dataset, UseAAA)
        if not arguments.NotToExecute:
            os.chdir(WorkDir)
            if RealMaxEvents > 0 : 
                print 'Submitting ' + str(NumberOfJobs) +  ' jobs to run on ' + str(RealMaxEvents)  + ' events in ' + str(DatasetRead['numberOfFiles']) + ' files for ' + str(dataset) + ' dataset.\n'
            else:
       		print 'Submitting ' + str(NumberOfJobs) +  ' jobs to run on all events in ' + str(DatasetRead['numberOfFiles'])  +' files for ' + str(dataset) + ' dataset.\n'
            if lxbatch: 
        	os.syetem('./lxbatchSub.sh')
	    else:
        	os.system('condor_submit condor.sub')
	    os.chdir(SubmissionDir)
        else:
            print 'Configuration files created for ' + str(dataset) + ' dataset but no jobs submitted.\n'

#If there are no input datasets specified and the user still continues.
else:
    NumberOfJobs = arguments.NumberOfJobs
    MaxEvents = -1
    Label = arguments.Label
    if arguments.MaxEvents < 0:
        print "Maximum number of events is negative and no input dataset is specified, Aborting!"
        sys.exit()
    else:
        MaxEvents = int(arguments.MaxEvents)
    EventsPerJob = int(math.ceil(float(MaxEvents)/float(NumberOfJobs)) )
    #Make sure at last the number of total events is larger than the MaxEvents.
    RealMaxEvents = EventsPerJob*int(NumberOfJobs)
    Config =  arguments.Config
    SubmissionDir = os.getcwd()
    WorkDir = CondorDir
    if arguments.localConfig:	
        GetCompleteOrderedArgumentsSet(InputCondorArguments)
    os.system('cp ' + Config + ' ' + WorkDir + '/userConfig_cfg.py')
    
    MakeSpecificConfig('',WorkDir)

    if lxbatch:
        MakeBatchJobFile(WorkDir, Queue, NumberOfJobs)
    else:
        MakeCondorSubmitScript('',NumberOfJobs,WorkDir,Label, UseAAA)
    if not arguments.NotToExecute:
        os.chdir(WorkDir)
        print 'Submitting ' + str(NumberOfJobs) +  ' jobs to run ' + str(RealMaxEvents)  + ' events for ' + str(Config) + '.\n'
        if lxbatch: 
            os.syetem('./lxbatchSub.sh')
	else:
            os.system('condor_submit condor.sub')
	os.chdir(SubmissionDir)
    else:
        print 'Configuration files created for ' + str(Config) + '  but no jobs submitted.\n'
