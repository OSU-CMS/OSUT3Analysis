#!/usr/bin/env python

import sys
import os
import glob
import re
import time
from math import *
from array import *
import fcntl
from decimal import *
from optparse import OptionParser
from multiprocessing import Process, Semaphore, cpu_count, Lock
from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *
from OSUT3Analysis.Configuration.formattingUtilities import *

parser = OptionParser()
parser = set_commandline_arguments(parser)
#UserConfig file is what you use to specify your email and directory to use on T3,etc. 
parser.add_option("-U", "--UserConfig", dest="UserConfig",
                  help="user configuration file")
parser.add_option("--SU", "--SubmitCrabJobs", action = "store_true", dest="SubmitCrabJobs", default=False,
                  help="Will submit the Crab jobs")
parser.add_option("-M", "--MergeFiles", action = "store_true", dest="MergeFiles", default=False,
                  help="Will merge the output root file into a composite file")
parser.add_option("--RS", "--ResubmitCrabJobs", dest="ResubmitBlocks",default=[],
                  help="Will submit the Crab jobs")
parser.add_option("--CC", "--CleanCrabJobs", action = "store_true", dest="CleanCrabJobs",default=False,
                  help="Will clean up the Crab jobs")
parser.add_option("--SR", "--StatusReport", action="store_true", dest="StatusReport",default=False,
                  help="Will print out the status report of the Crab jobs")
parser.add_option("--GO", "--GetOutput", action="store_true", dest="GetOutput",default=False,
                  help="will ge the output of Crab jobs")
parser.add_option("-j", "--nJobs", dest="nJobs", default=cpu_count(),
                  help="Set the number of processes to run simultaneously (default: number of CPUs)")
(arguments, args) = parser.parse_args()
condorDir = set_condor_output_dir(arguments)


CMSSWDir = os.getcwd()

if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + arguments.localConfig.rstrip('.py') + " import *")
if arguments.UserConfig:
    sys.path.append(os.getcwd())
    exec("from " + arguments.UserConfig.rstrip('.py') + " import *")

compositeDatasets = get_composite_datasets(datasets, composite_dataset_definitions)
datasets = split_composite_datasets(datasets, composite_dataset_definitions)

def replaceName(Origin,String):
	if Origin.find('e=') > 0:
		Origin = Origin[0:Origin.find('e=') + 2] + String
        else:
		Origin = Origin[0:Origin.find('=') + 2] + String
        return  Origin     

#Create a new directory to store the crablog files. 
if not os.path.exists('./pickEvents_crabLog'):
	os.system('mkdir ' + 'pickEvents_crabLog')

if arguments.SubmitCrabJobs:
	if os.path.exists('./pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:]):
                print 'Warning: ' + condorDir[condorDir.find('/')+1:] + ' already exists.'
	        whatever = raw_input(" Do you want to submit these crab jobs anyway (Suggestion: Do --CC to clean up the old ones and then submit)? (y/n)")
                if not whatever is "y":
			sys.exit(0)
        else:
		os.system('mkdir pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:])
        for sample in datasets:
                datasetPath = dataset_names[sample]
        	name = str(sample)
        	#Create a x_pickevents.txt file in the corresponding dataset directory.
        	filename = CMSSWDir + '/' + condorDir + '/' + name + '/' + name + '_pickevents.txt'
        	os.system('touch ' + filename)
        	command = 'grep "Event passed" ' +'./' + condorDir + '/' + name + '/*.err | awk \'{print $9}\' >' + filename +' 2>&1'
        	os.system(command)
        	os.system('edmPickEvents.py "' + datasetPath +'" ' + filename + ' --crab')
        	os.system('mv pickevents_runEvents.txt ' + filename)
        	#Create the json file and cfg file used in the crab job in the corresponding dataset directory.
        	Cfgname = CMSSWDir + '/' + condorDir + '/' + name + '/' + name + '_pickevents_crab.config'
        	Maskname = CMSSWDir + '/' + condorDir + '/' + name + '/' + name + '_pickevents.json'
        	os.system('touch ' + Cfgname) 
        	os.system('touch ' + Maskname) 
        	os.system('mv pickevents.json ' + Maskname)
        	Config = open('pickevents_crab.config','r')
        	os.system('touch pickevents_crab.config.new') 
        	ConfigTmp = open('pickevents_crab.config.new','r+')
        	#Change the cfg file generated automatically inprocessing edmPickEvents.py script.
        	for line in Config:
               		Stop = 0
                	if '#' in line:
				continue
                	for tmpline in UserString:
				TmpString = tmpline[0:5]
                		if TmpString in line:
					Stop = 1
                	if Stop:
				continue
                	if 'output' in line and 'Process_load'not in line:
				ConfigTmp.write(str(replaceName(line,condorDir[condorDir.find('/')+1:] + '_' + name + '.root')))
                	elif 'Process_load' and 'output' in line:
                        	line = line[0:line.find('Process_load') + 13] +  filename + ' ' + line[line.find('output'):]
				ConfigTmp.write(str(replaceName(line,condorDir[condorDir.find('/')+1:] + '_' + name + '.root')))
                	elif 'lumi_mask' in line:
                        	line = line[0:line.find('=') + 2] +  Maskname + '\n'
                       		ConfigTmp.write(line)
               		elif '[USER]' in line:
                       		ConfigTmp.write('[USER]\n')
                       		for string in UserString:
                       			if 'ui_working_dir' in string:
						string = string[0:string.find('=') + 3] + name
						ConfigTmp.write(string + '\n')
                        		else:	
						ConfigTmp.write(string + '\n')
                	elif 'scheduler' in line:
				line = line[0:line.find('=') + 2] + 'remoteglidein' 
                       		ConfigTmp.write(line + '\n')
                	elif 'use_server' in line:
                       		line = line[0:line.find('=') +2] + '0'
                       		ConfigTmp.write(line)
                	else:
                  		ConfigTmp.write(line)
		ConfigTmp.close()
        	os.system('mv pickevents_crab.config.new ' + Cfgname)
        	os.system('rm pickevents_crab.config')
	for sample in datasets:
		print str(sample) + ' has ' + str(len(open(CMSSWDir + '/' + condorDir + '/' + str(sample) + '/' + str(sample) + '_pickevents.txt','rU').readlines())) + ' events passed all cuts.'
def submitCrabJobs(dataset,sema):
	sema.acquire()
	name = str(dataset)
        ConfigPath = '/' + condorDir + '/' + name + '/' + name + '_pickevents_crab.config'       
        if not os.path.exists('./pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:] + '/' + name + '_crab'):
        	os.system('mkdir ./pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:] + '/' + name + '_crab')
        fcntl.lockf (sys.stdout, fcntl.LOCK_EX)
        os.chdir( CMSSWDir + '/pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:] + '/' + name + '_crab')
        command = 'crab -create -cfg ' + CMSSWDir + ConfigPath
        os.system(command)
        #Change the name of the crab log directory to be the name of the dataset.
        os.system('crab -submit -c ' + name)
        fcntl.lockf (sys.stdout, fcntl.LOCK_EX)
        sema.release()

def reSubmitCrabJobs(dataset,sema):
	sema.acquire()
	name = str(dataset)
        fcntl.lockf (sys.stdout, fcntl.LOCK_EX)
        os.chdir( CMSSWDir + '/pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:] + '/' + name + '_crab')
        #Change the name of the crab log directory to be the name of the dataset.
        os.system('crab -forceResubmit all -c ' + name)
        fcntl.lockf (sys.stdout, fcntl.LOCK_EX)
        sema.release()

def statusReport(dataset,sema):
	sema.acquire()
	name = str(dataset)
        if not os.path.exists('./pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:] + '/' + name + '_crab'):
        	print "You have not submitted the crab job for " + name
        else:
        	fcntl.lockf (sys.stdout, fcntl.LOCK_EX)
        	os.chdir( CMSSWDir + '/pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:] + '/' + name + '_crab')
        	os.system('crab -status -c ' + name)
        	fcntl.lockf (sys.stdout, fcntl.LOCK_EX)
        	sema.release()
def getOutput(dataset,sema):
	sema.acquire()
	name = str(dataset)
        if not os.path.exists('./pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:] + '/' + name + '_crab'):
        	print "You have not submitted the crab job for " + name
        else:
        	fcntl.lockf (sys.stdout, fcntl.LOCK_EX)
        	os.chdir( CMSSWDir + '/pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:] + '/' + name + '_crab')
        	os.system('crab -getoutput -c ' + name)
        	fcntl.lockf (sys.stdout, fcntl.LOCK_EX)
        	sema.release()

def MergeFilesStep1(datasets):
	for dataset in datasets:
        	name = str(dataset)
                print name
         	LocalFileList = glob.glob(r'./pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:] + '/' + name + '_crab/' + name + '/res/*.root')
        	RemoteFileList = glob.glob(RemoteDirT3 + condorDir[condorDir.find('/')+1:] + '_' + name + '*.root')
        	os.chdir(CMSSWDir)
                if not os.path.exists('./pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:] + '/' + name + '_crab'):
        		print "You have not submitted the crab job for " + name
        	elif len(LocalFileList):

        		os.chdir( CMSSWDir + '/pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:] + '/' + name + '_crab/' + name + '/res/')
        		os.system('ls *.root | perl -ne \'print "file:$_"\' > myFiles.txt')
        	elif len(RemoteFileList):
                	os.chdir(RemoteDirT3)
                	os.system('ls ' + condorDir[condorDir.find('/')+1:] + '*' + name + '*root | perl -ne \'print "file:$_"\' > myFiles_' + condorDir[condorDir.find('/')+1:] + '_' + name + '.txt')
        	else:
			print 'No outputs of ' + name + ' are found.'

def CleanCrabJobs(dataset,sema):
		sema.acquire()
        	name = str(dataset)
                fcntl.lockf (sys.stdout, fcntl.LOCK_EX)
        	if os.path.exists(CMSSWDir + '/pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:] + '/' + name + '_crab'):
                	os.chdir( CMSSWDir + '/pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:] + '/' + name + '_crab')
                	os.system('crab -kill all -c ' + name)
                fcntl.lockf (sys.stdout, fcntl.LOCK_EX)
                sema.release()	 


def MergeFilesStep2(compositeDataset,sema):
	sema.acquire()
        fcntl.lockf (sys.stdout, fcntl.LOCK_EX)
	sema.acquire()
	compositeName = str(compositeDataset)
        print compositeName
        compositeFilePath = CMSSWDir + '/pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:] + '/' + compositeName + '_Files.txt'
        if os.path.exists(compositeFilePath):
		os.system('rm ' + compositeFilePath)
        copyData = False
        for dataset in composite_dataset_definitions[compositeDataset]:
        	name = str(dataset)
                LocalFileList = glob.glob(r'./pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:] + '/' +  name + '_crab/' + name + '/res/*.root')
        	RemoteFileList = glob.glob(RemoteDirT3 + condorDir[condorDir.find('/')+1:] + '_' + name + '*.root')
        	if len(LocalFileList):
        		os.chdir( CMSSWDir + '/pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:] + '/' + name + '_crab/' + name + '/res/')
        		os.system('ls *.root | perl -ne \'print "file:$_"\' > myFiles.txt')
		        splitFilePath = CMSSWDir + '/pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:] + '/'+ name + '_crab/' + name + '/res/myFiles.txt'
                        os.system('touch ' + compositeFilePath)
                        os.system('cat ' + splitFilePath + '>>' + compositeFilePath)
        	elif len(RemoteFileList):
                	os.chdir(RemoteDirT3)
                	os.system('ls ' + condorDir[condorDir.find('/')+1:] + '*' + name + '*root | perl -ne \'print "file:$_"\' >' +  CMSSWDir + '/pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:] + '/' + name + '_crab/' + name + '_Files.txt')
                	splitFilePath = CMSSWDir + '/pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:] + '/' + name + '_crab/' + name + '_Files.txt'
                        os.system('touch ' + compositeFilePath)
                        os.system('cat ' + splitFilePath + '>>' + compositeFilePath)
                        copyData = True
         	else:
			print 'No outputs of ' + name + ' are found.'
	if copyData:
        	os.chdir(RemoteDirT3)
                os.system('edmCopyPickMerge inputFiles_load=' + compositeFilePath +' outputFile=' + CMSSWDir + '/pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:] + '/' + compositeName +'_merged.root maxSize=1000000')
	else:
        	os.chdir(CMSSWDir + '/pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:] + '/' + name + '_crab/' + name + '/res/')
                os.system('edmCopyPickMerge inputFiles_load=' + compositeFilePath +' outputFile=' + CMSSWDir + '/pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:] + '/' + compositeName +'_merged.root maxSize=1000000')
        fcntl.lockf (sys.stdout, fcntl.LOCK_EX)
        sema.release()


f = open ("/tmp/pickEvents.lock", "w")
stat = os.stat ("/tmp/pickEvents.lock")
if stat.st_uid == os.getuid () and stat.st_gid == os.getgid ():
    os.chmod ("/tmp/pickEvents.lock", 0666)
fcntl.lockf (f, fcntl.LOCK_EX)

processes = []
sema = Semaphore(arguments.nJobs)
if arguments.SubmitCrabJobs:
	for dataset in datasets:
                p = Process (target = submitCrabJobs, args = (dataset, sema))
        	p.start ()
    		processes.append (p) 
        for p in processes:
    		p.join ()
if arguments.StatusReport:
	for dataset in datasets:
    		p = Process (target = statusReport, args = (dataset, sema))
         	p.start ()
    		processes.append (p) 
	for p in processes:
    		p.join ()
if arguments.GetOutput:
	for dataset in datasets:
    		p = Process (target = getOutput, args = (dataset, sema))
         	p.start ()
    		processes.append (p) 
	for p in processes:
    		p.join ()
reSubmitDatasetArray = arguments.ResubmitBlocks 
reSubmitDatasets = []
if len(reSubmitDatasetArray):
	for n in reSubmitDatasetArray:
		reSubmitDatasets.append(datasets[int(n)-1])
	for dataset in reSubmitDatasets:
    		p = Process (target = reSubmitCrabJobs, args = (dataset, sema))
         	p.start ()
    		processes.append (p) 
	for p in processes:
    		p.join ()
if arguments.MergeFiles:
	MergeFilesStep1(datasets)
        for composite_dataset in compositeDatasets:
                p = Process (target = MergeFilesStep2, args = (composite_dataset, sema))
         	p.start ()
    		processes.append (p) 
	for p in processes:
    		p.join ()
if arguments.CleanCrabJobs:
        delete = raw_input("  Do you really want to clean up all the crab jobs related to " + condorDir[condorDir.find('/')+1:] + " ?(y/n) ")
  	if delete is "y":
		os.system('rm -f ' + RemoteDirT3 + condorDir[condorDir.find('/')+1:] + '*')	
        	for dataset in datasets:
                	p = Process (target = CleanCrabJobs, args = (dataset, sema))
                	p.start ()
                	processes.append (p)
        	for p in processes:  
	                p.join ()
		os.system('rm -rf ' + CMSSWDir + '/pickEvents_crabLog/' + condorDir[condorDir.find('/')+1:])


fcntl.lockf (f, fcntl.LOCK_UN)
f.close ()


