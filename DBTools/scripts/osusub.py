#!/usr/bin/env python
import sys
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
parser.add_option("-t", "--typeOfSource", dest="FileType", default = 'OSUT3Ntuple', help="Specify the type of input files, ntuples from T3 or AAA via xrootd.")
parser.add_option("-d", "--dataset", dest="Dataset", default = "", help="Specify which dataset to run.")
parser.add_option("-w", "--workDirectory", dest="Directory", default = "", help="Specify the working directroy.")
parser.add_option("-c", "--configuration", dest="Config", default = "", help="Specify the configuration file to run.")
parser.add_option("-n", "--numberOfJobs", dest="NumberOfJobs", default = -1, help="Specify how many jobs to submit.")
parser.add_option("-u", "--userCondorSubFile", dest="CondorSubFilr", default = "", help="Specify the condor.sub file you want to use if you have to add arguments.")
parser.add_option("-U", "--uniqueEventId", action="store_true", dest="Unique", default=False, help="Assign unique and continuos event IDs")
parser.add_option("-N", "--noExec", action="store_true", dest="NotToExecute", default = False, help="Just generate necessary config files without executing them.")
parser.add_option("-L", "--Label", dest="Label", default = "", help="Give the dataset a short label.")

(arguments, args) = parser.parse_args()

def MakeCondorSubmitScript(Dataset,NumberOfJobs,Directory,Label):
    os.system('touch ' + Directory + '/condor.sub')
    SubmitFile = open(Directory + '/condor.sub','r+w')
    cmsRunExecutable = os.popen('which cmsRun').read()
    for argument in sorted(CondorSubArgumentsSet):
        if CondorSubArgumentsSet[argument].has_key('Executable') and CondorSubArgumentsSet[argument]['Executable'] == "":
            SubmitFile.write('Executable = ' + cmsRunExecutable + '\n')
        elif CondorSubArgumentsSet[argument].has_key('Arguments') and CondorSubArgumentsSet[argument]['Arguments'] == "":
            SubmitFile.write('Arguments = config_cfg.py True ' + str(NumberOfJobs) + ' $(Process) ' + Dataset + ' ' + Label + '\n\n')
        elif CondorSubArgumentsSet[argument].has_key('Transfer_Input_files') and CondorSubArgumentsSet[argument]['Transfer_Input_files'] == "":
            if Dataset == '':
                SubmitFile.write('Transfer_Input_files = config_cfg.py,userConfig_cfg.py\n')
            else:
                SubmitFile.write('Transfer_Input_files = config_cfg.py,userConfig_cfg.py,datasetInfo_cfg.py\n')
        elif CondorSubArgumentsSet[argument].has_key('Queue'):
            SubmitFile.write('Queue ' + str(NumberOfJobs) +'\n')
        else:
            SubmitFile.write(CondorSubArgumentsSet[argument].keys()[0] + ' = ' + CondorSubArgumentsSet[argument].values()[0] + '\n')
    SubmitFile.close()

def MakeSpecificConfig(Dataset, Directory):
    os.system('touch ' + Directory + '/config_cfg.py')
    ConfigFile = open(Directory + '/config_cfg.py','r+w')
    ConfigFile.write('import FWCore.ParameterSet.Config as cms\n')        
    ConfigFile.write('import OSUT3Analysis.DBTools.osusub_cfg as osusub\n')
    ConfigFile.write('import re\n')
    ConfigFile.write('import userConfig_cfg as pset\n')
    ConfigFile.write('\n')
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
    else:
	ConfigFile.write('process.setName_ (process.name_ () + \'' + str(arguments.Directory).replace('_','')  + '\')\n') 
    if arguments.Random:
        ConfigFile.write('pset.process.RandomNumberGeneratorService.generator.initialSeed = osusub.jobNumber\n')
    if arguments.Unique:
        #Make all the events IDs unique and continuos.
        ConfigFile.write('pset.process.source.firstEvent = cms.untracked.uint32((osusub.jobNumber-1)*' + str(EventsPerJob) + '+1)\n')
    ConfigFile.close()

def MakeFileList(Dataset, FileType, Directory):
    numberOfFiles = -1
    datasetRead = {}
    runList = []
    os.system('touch ' + Directory + '/datasetInfo_cfg.py')
    if FileType == 'AAA':	
        os.system('das_client.py --query="file Dataset=' + Dataset + '" --limit 0 > ' + Directory + '/runList.py')
        if lxbatch:
	    os.system('sed -i \'s/^/root:\/\/xrootd.ba.infn.it\//g\' ' + Directory + '/runList.py')
        else:
	    os.system('sed -i \'s/^/root:\/\/cmsxrootd.fnal.gov\//g\' ' + Directory + '/runList.py')
    if FileType == 'UserDir':
	if not os.path.exists(Dataset):
	    print "The directory you provided does not exist."
            sys.exit()
        datasetRead['numberOfFiles'] = len(os.listdir(Dataset))
        datasetRead['realDatasetName'] = 'FilesInDirectory:' + Dataset 
        #Get the list of the root files in the directory and modify it to have the standard format. 
        if not remoteAccessT3:
            os.system('sed -i \'s/^/"file:/g\' ' + Directory + '/datasetInfo_cfg.py')
        else:
            os.system('sed -i \'s/^/"root:\/\/cms-0.mps.ohio-state.edu:1094\//g\' ' + Directory + '/datasetInfo_cfg.py')
        os.system('sed -i \'1,$s/$/",/g\' ' + Directory + '/datasetInfo_cfg.py')
        os.system('sed -i "1i runList = [" ' + Directory + '/datasetInfo_cfg.py')                  
        os.system('sed -i \'$s/,//g\' ' + Directory + '/datasetInfo_cfg.py')                  
        os.system('sed -i \'$a ]\' ' + Directory + '/datasetInfo_cfg.py')                  
    if FileType == 'UserList':
	if not os.path.exists(Dataset):
            print "The list you provided does not exist."
            sys.exit()
        #Get the list of the files to datasetInfo_cfg.py and modify it to have the standard format. 
        os.system('cp ' + Dataset + ' '  + Directory + '/datasetInfo_cfg.py')	
        datasetRead['numberOfFiles'] = os.popen('wc -l ' + Directory + '/datasetInfo_cfg.py').read().split(' ')[0] 
        datasetRead['realDatasetName'] = 'FilesInList:' + Dataset 
        if not remoteAccessT3:
            os.system('sed -i \'s/^/"file:/g\' ' + Directory + '/datasetInfo_cfg.py')
        else:
            os.system('sed -i \'s/^/"root:\/\/cms-0.mps.ohio-state.edu:1094\//g\' ' + Directory + '/datasetInfo_cfg.py')
        os.system('sed -i \'1,$s/$/",/g\' ' + Directory + '/datasetInfo_cfg.py')
        os.system('sed -i "1i runLinst = [" ' + Directory + '/datasetInfo_cfg.py')                  
        os.system('sed -i \'$s/,//g\' ' + Directory + '/datasetInfo_cfg.py')                  
        os.system('sed -i \'$a ]\' ' + Directory + '/datasetInfo_cfg.py')                  
    if FileType == 'OSUT3Ntuple':
        prefix = ''
        if not remoteAccessT3 :
	    prefix = 'file:' 
        else:
            prefix = 'root://cms-0.mps.ohio-state.edu:1094/'
        #Use MySQLModule, a perl script to get the information of the given dataset from T3 DB and save it in datasetInfo_cfg.py. 
        os.system('MySQLModule ' + Dataset + ' ' + Directory + '/datasetInfo_cfg.py ' + prefix)
        sys.path.append(Directory)
        import datasetInfo_cfg as datasetInfo
        location = datasetInfo.location 
        if location == 'Dataset does not exist on the Tier 3!':
 	    print 'Dataset does not exist on the Tier 3!'
            sys.exit() 
        status = datasetInfo.status
        continueForNonPresentDataset = True
        if not status == 'present':
	    userDecision = raw_input('The dataset you selected is not marked as present on Tier3, do you still want to continue?(Type "y" for yes and "n" for no.)')
	    if userDecision == "n":
                continueForNonPresentDataset = False 
        if not continueForNonPresentDataset:
	    sys.exit()    
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
#             First of all to set up the working directory                    #
###############################################################################
CondorDir = ''
if arguments.Directory == "":
   print "No working directory is given, aborting."
   sys.exit()
else:
   CondorDir = os.getcwd() + '/condor/' + arguments.Directory
#Check whether the directory specified already exists and cancel the submission if so.
if not os.path.exists(CondorDir):
   os.system('mkdir ' + CondorDir)
else:
   print 'Directory "' + str(CondorDir) + '" already exists in your condor directory. Please check and corresponding submission aborted.' 
   sys.exit()

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
        WorkDir = CondorDir + '/' + str(dataset)
        os.system('mkdir ' + WorkDir )

        DatasetRead = MakeFileList(DatasetName,arguments.FileType,WorkDir)
        NumberOfFiles = int(DatasetRead['numberOfFiles'])
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
            MakeCondorSubmitScript(DatasetRead['realDatasetName'],NumberOfJobs,WorkDir,dataset)
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
            MakeCondorSubmitScript('',NumberOfJobs,WorkDir,Label)
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
     
        

