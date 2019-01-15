#!/usr/bin/env python

import os
import sys
import math
import glob
import tarfile

from OSUT3Analysis.Configuration.ProgressIndicator import ProgressIndicator

from optparse import OptionParser
from OSUT3Analysis.Configuration.processingUtilities import *

parser = OptionParser()
parser = set_commandline_arguments(parser)

parser.remove_option("-o")
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

parser.add_option("-d", "--dataset", dest="Dataset", default = "", help="Specify which dataset to run.")
parser.add_option("-L", "--targetLumi", dest="IntLumi", default = "", help="Specify the targeting luminosity.")
parser.add_option("-N", "--noExec", action="store_true", dest="NotToExecute", default = False, help="Just generate necessary config files without executing them.")
parser.add_option("-2", "--step2", action="store_true", dest="doStep2", default = False, help="Merge the mini-merged files to finish the process.")

(arguments, args) = parser.parse_args()

def MakeMiniMergingConfigForLPCCondor(Directory):

    words = """
#!/bin/bash
dataset=$1
cmsswVersion=$2
scramArch=$3
tar -xzf histograms_${1}.tar.gz
rm -f histograms_${1}.tar.gz
source /cvmfs/cms.cern.ch/cmsset_default.sh
tar -xzf ${4}.tar.gz
rm -f ${4}.tar.gz
SCRAM_ARCH=${5}
cd ${4}/src/
scramv1 b ProjectRename
eval `scramv1 runtime -sh`
cd -

python lpcMiniMerge.py $1 $2 $3 $4 $5 $6

rm -rf ${4}
rm -rf ${1}
"""

    BashScript = open(Directory + '/lpcMiniMerge.sh', 'w')
    BashScript.write(words)
    BashScript.close()
    os.chmod (Directory + '/lpcMiniMerge.sh', 0755)

    words = """
#!/usr/bin/env python
import os, sys, glob, math
from OSUT3Analysis.Configuration.mergeUtilities import *

dataset = sys.argv[1]
nJobs = int(sys.argv[2])
index = int(sys.argv[3])
cmsswVersion = sys.argv[4]
scramArch = sys.argv[5]
IntLumi = sys.argv[6]

exec('import datasetInfo_' + dataset + '_cfg as datasetInfo')
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

inputFiles = glob.glob(dataset + '/hist_*.root')
nFiles = len(inputFiles)
nFilesPerJob = int(math.ceil(float(nFiles)/nJobs))

indexesToMerge = range(index * nFilesPerJob, (index+1) * nFilesPerJob) if index < nJobs-1 else range(index * nFilesPerJob, nFiles)
filesToMerge = [dataset + '/hist_' + str(x) + '.root' for x in indexesToMerge]

InputWeightString = ','.join([str(Weight)] * len(filesToMerge))

cmd = 'mergeTFileServiceHistograms -i '
for f in filesToMerge:
    cmd += f + ' '
cmd += '-o miniMerge_' + dataset + '_' + str(index) + '.root -w ' + InputWeightString

print cmd

os.system(cmd)"""

    BashScript = open(Directory + '/lpcMiniMerge.py', 'w')
    BashScript.write(words)
    BashScript.close()
    os.chmod (Directory + '/lpcMiniMerge.py', 0755)

def makeLPCMiniMerge(Directory, datasets, lumi):
    cwd = os.getcwd()
    for dataset in datasets:
    	MakeMiniMergingConfigForLPCCondor(Directory)
    	progress = ProgressIndicator("Compressing " + dataset)
        os.chdir(Directory)
        nFiles = len(glob.glob(dataset + '/hist_*.root'))
        nJobs = int(math.ceil(math.sqrt(len(glob.glob(dataset + '/hist_*.root')))))
        print
        print 'Creating', nJobs, 'jobs merging', nFiles, 'files in', dataset
        if os.path.exists('histograms_' + dataset + '.tar.gz'):
        	print Directory + '/histograms_' + dataset + '.tar.gz already exists! Skipping...'
        else:
        	histogramsTar = tarfile.open ('histograms_' + dataset + '.tar.gz', 'w:gz')
        	for i, f in enumerate(glob.glob(dataset + '/hist_*.root')):
        		progress.setPercentDone(float(i+1) / nFiles * 100.0)
        		progress.printProgress(i+1 == nFiles)
        		histogramsTar.add(f)
        	histogramsTar.close ()
        os.chdir(cwd)

        words = """
Executable = lpcMiniMerge.sh
Universe = vanilla
Getenv = True
request_memory = 5000MB
Arguments = {0} {1} $(Process) {2} {3} {4}

Output = condor_{0}_$(Process).out
Error = condor_{0}_$(Process).err
Log = condor_{0}_$(Process).log

should_transfer_files = YES
Transfer_Input_files = {2}.tar.gz,{0}/datasetInfo_{0}_cfg.py,lpcMiniMerge.py,histograms_{0}.tar.gz
Transfer_Output_files = miniMerge_{0}_$(Process).root
+IsLocalJob = true
Rank = TARGET.IsLocalSlot

request_cpus = 1
Queue {1}
                """.format(dataset, nJobs, os.environ["CMSSW_VERSION"], os.environ["SCRAM_ARCH"], lumi)

        SubmitFile = open(Directory + '/lpcMiniMerge_{0}.sub'.format(dataset), 'w')
        SubmitFile.write(words)
        SubmitFile.close()

def finishLPCMerge(Directory, datasets, lumi):
    cwd = os.getcwd()
    os.chdir(Directory)
    for dataset in datasets:
        sys.path.append(dataset)
        exec('import datasetInfo_' + dataset + '_cfg as datasetInfo')
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

        filesToMerge = glob.glob('miniMerge_' + dataset + '_*.root')
        InputWeightString = ','.join([str(Weight)] * len(filesToMerge))

        cmd = 'mergeTFileServiceHistograms -i '
        for f in filesToMerge:
            cmd += f + ' '
        cmd += '-o finished_' + dataset + '.root -w ' + InputWeightString
        print cmd
        os.system(cmd)
        os.chdir(Directory)
    os.chdir(cwd)

##########################

pwd = os.getcwd()

CondorDir = ''
if arguments.condorDir == "":
    print "No working directory is given, aborting."
    sys.exit()
else:
    CondorDir = os.getcwd() + '/condor/' + arguments.condorDir

split_datasets = []
IntLumi = 0.0
if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.localConfig) + " import *")
    composite_datasets = get_composite_datasets(datasets, composite_dataset_definitions)
    split_datasets = split_composite_datasets(datasets, composite_dataset_definitions)
    IntLumi = intLumi
    if arguments.Dataset:
        print "ERROR:  The -d and -l options cannot be used simultaneously."
        exit(0)

if not arguments.localConfig:
    if arguments.Dataset == "":
        print "There are no datasets to merge!"
    else:
        split_datasets.append(arguments.Dataset)

if arguments.IntLumi is not "":
    IntLumi = float(arguments.IntLumi)

if arguments.doStep2:
    finishLPCMerge(CondorDir, split_datasets, IntLumi)
else:
    makeLPCMiniMerge(CondorDir, split_datasets, IntLumi)
    if not arguments.NotToExecute:
    	os.chdir(CondorDir)
    	for dataset in split_datasets:
    		print 'Submitting mini-merge of', dataset, '...'
    		os.system('condor_submit lpcMiniMerge_' + dataset + '.sub')
    	os.chdir(pwd)
    else:
    	print 'Skipping submission due to --noExec flag.'
