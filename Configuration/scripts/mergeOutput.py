#!/usr/bin/env python
import os
import sys
import fcntl
import datetime
from optparse import OptionParser
from multiprocessing import Process, Semaphore, cpu_count

from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *

parser = OptionParser()
parser = set_commandline_arguments(parser)
parser.add_option("-t", "--no-weights", action="store_true", dest="noWeights", default=False,
                  help="do not apply cross section weights")
parser.add_option("-q", "--quickMerge", action="store_true", dest="quickMerge", default=False,
                  help="do merge without making cutflow or plots")
parser.add_option("-C", "--compositeOnly", action="store_true", dest="compositeOnly", default=False,
                  help="do not merge individual datasets; only combine them into composite")
parser.add_option("-s", "--singleDataset", dest="singleDataset", 
                  help="Specify a single dataset to run over; overrides the list in the localOptions file")  
parser.add_option("-j", "--nJobs", dest="nJobs", default=cpu_count(),
                  help="Set the number of processes to run simultaneously (default: number of CPUs)")
parser.add_option("-i", "--nice", dest="increment", default=10,
                  help="Set the niceness increment for the processes that are forked (default: 10)")
(arguments, args) = parser.parse_args()

if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + arguments.localConfig.rstrip('.py') + " import *")

from ROOT import TFile, TH1D, TStopwatch

sw = TStopwatch ()
sw.Start ()

condor_dir = set_condor_output_dir(arguments)

if arguments.singleDataset:  # Only run over a single dataset 
    datasets = [ arguments.singleDataset ]  


#save a list of composite datasets
composite_datasets = get_composite_datasets(datasets, composite_dataset_definitions)

#save a list of datasets with composite datasets split up
split_datasets = split_composite_datasets(datasets, composite_dataset_definitions)

if arguments.compositeOnly:  # Do not merge any individual datasets; only merge composite datasets
    split_datasets = []

# Remove duplicates
split_datasets     = list(set(split_datasets))
composite_datasets = list(set(composite_datasets))


# mergeAll.out will contain the output of all the mergeHists commands. 
# Move an existing version of mergeAll.out to a backup file.  
now = datetime.datetime.now()
date_hash = now.strftime("%Y_%m_%d_%H:%M:%S")
if os.path.exists (condor_dir + "/mergeAll.out"):
    command = "mv " + condor_dir + "/mergeAll.out " + condor_dir + "/mergeAll-bkup" + date_hash + ".out"
    os.system(command)


#merge first layer
flags = TH1D ("flags", "", 1, 0, 1)
flags.GetXaxis ().SetBinLabel (1, "noWeights")
if arguments.noWeights:
    flags.SetBinContent (1, 1)

def mergeDataset (dataset, sema):
    sema.acquire ()

    os.nice (arguments.increment)
    dataset_dir = "%s/%s" % (condor_dir,dataset)  
    command = "mkdir -p %s/logMerge" % condor_dir # Create logMerge directory if it does not already exist  
    os.system(command)  
    logMerge = "%s/logMerge/%s.out" % (condor_dir,dataset)  # the logfile for a single dataset  
    if arguments.noWeights:
        command = "mergeHists -w 1 -p %s %s" % (dataset_dir, dataset_dir)
    else:
        command = "mergeHists -l %g -p %s %s" % (intLumi, dataset_dir, dataset_dir)
    command += " >> " + logMerge + " 2>&1"    # append stdout and stderr to log file  

    output = open (logMerge, "w")
    output.write("\n\n\n")
    output.write ("Merging output for " + dataset + " dataset\n")
    output.close ()

#    print command
    os.system(command)
    fcntl.lockf (sys.stdout, fcntl.LOCK_EX)
    output = open (logMerge, "r")
    sys.stdout.write (output.read ())
    output.close ()
    command = "cat " + logMerge + " >> " + condor_dir + "/mergeAll.out" 
    os.system(command)
    fcntl.lockf (sys.stdout, fcntl.LOCK_UN)

    fout = TFile (dataset_dir + ".root", "update")
    fout.cd ()
    flags.Write ()
    fout.Close ()

    sema.release ()

f = open ("/tmp/mergeOutput.lock", "w")
fcntl.lockf (f, fcntl.LOCK_EX)
processes = []
sema = Semaphore (int (arguments.nJobs))
for dataset in split_datasets:
    p = Process (target = mergeDataset, args = (dataset, sema))
    p.start ()
    processes.append (p)
for p in processes:
    p.join ()
fcntl.lockf (f, fcntl.LOCK_UN)
f.close ()

#merge together components of composite datasets
def mergeCompositeDataset (composite_dataset, sema):
    sema.acquire ()

    os.nice (arguments.increment)
    component_datasets_list = ""
    component_weights_list = ""
    component_dataset_file_path = ""
    composite_dataset_dir = "%s/%s" % (condor_dir,composite_dataset)
    command = "mkdir -p %s/logMerge" % condor_dir # Create logMerge directory if it does not already exist
    os.system(command)
    logMerge = "%s/logMerge/%s.out" % (condor_dir,composite_dataset)  # the logfile for the composite dataset
    for component_dataset in composite_dataset_definitions[composite_dataset]:
        component_dataset_dir = "%s/%s" % (condor_dir,component_dataset)
        component_dataset_file_path = component_dataset_dir + ".root"
        if os.path.isfile(component_dataset_file_path):
            component_datasets_list += " " + component_dataset_file_path
            if isinstance (composite_dataset_definitions[composite_dataset], dict):
                if len (component_weights_list):
                    component_weights_list += "," + str (composite_dataset_definitions[composite_dataset][component_dataset])
                else:
                    component_weights_list += str (composite_dataset_definitions[composite_dataset][component_dataset])
            else:
                if len (component_weights_list):
                    component_weights_list += ",1"
                else:
                    component_weights_list += "1"
    command = "mergeHists -w %s -p %s %s" % (component_weights_list, composite_dataset_dir, component_datasets_list)
    command += " >> " + logMerge + " 2>&1"

    output = open (logMerge, "w") 
    output.write ("\n\n\n")  
    output.write ("Merging component datasets for " + composite_dataset + " dataset\n")
    output.close ()

#    print command
    os.system(command)
    fcntl.lockf (sys.stdout, fcntl.LOCK_EX)
    output = open (logMerge, "r")
    sys.stdout.write (output.read ())
    output.close ()
    fcntl.lockf (sys.stdout, fcntl.LOCK_UN)

    command = "cat " + logMerge + " >> " + condor_dir + "/mergeAll.out" 
    os.system(command)

    fout = TFile (composite_dataset_dir + ".root", "update")
    fout.cd ()
    flags.Write ()
    fout.Close ()

    sema.release ()

f = open ("/tmp/mergeOutput.lock", "w")
fcntl.lockf (f, fcntl.LOCK_EX)
processes = []
for composite_dataset in composite_datasets:
    p = Process (target = mergeCompositeDataset, args = (composite_dataset, sema))
    p.start ()
    processes.append (p)
for p in processes:
    p.join ()
fcntl.lockf (f, fcntl.LOCK_UN)
f.close ()


sw.Stop ()
cpu = sw.CpuTime ()
real = sw.RealTime ()

days = int (cpu / (60.0 * 60.0 * 24.0))
cpu -= days * (60.0 * 60.0 * 24.0)
hours = int (cpu / (60.0 * 60.0))
cpu -= hours * (60.0 * 60.0)
minutes = int (cpu / 60.0)
cpu -= minutes * 60.0

timeInfo = "\n\n\n=============================================\n\n"
timeInfo += "CPU Time:  "
if days > 0:
    timeInfo += str (days) + " days, "
if days > 0 or hours > 0:
    timeInfo += str (hours) + " hours, "
if days > 0 or hours > 0 or minutes > 0:
    timeInfo += str (minutes) + " minutes, "
if days > 0 or hours > 0 or minutes > 0 or cpu > 0:
    timeInfo += str (cpu) + " seconds\n"

days = int (real / (60.0 * 60.0 * 24.0))
real -= days * (60.0 * 60.0 * 24.0)
hours = int (real / (60.0 * 60.0))
real -= hours * (60.0 * 60.0)
minutes = int (real / 60.0)
real -= minutes * 60.0

timeInfo += "Real Time: "
if days > 0:
    timeInfo += str (days) + " days, "
if days > 0 or hours > 0:
    timeInfo += str (hours) + " hours, "
if days > 0 or hours > 0 or minutes > 0:
    timeInfo += str (minutes) + " minutes, "
if days > 0 or hours > 0 or minutes > 0 or real > 0:
    timeInfo += str (real) + " seconds\n"

timeInfo += "\n=============================================\n"
print timeInfo
f = open (condor_dir + "/mergeAll.out", "a")
f.write (timeInfo)
f.close ()

print "\n\nWrote logfile to: %s/mergeAll.out " %  condor_dir

#recreate plots file with all datasets combined and in pretty colors
args = "-c %s" % condor_dir.partition('/')[2]

#pass all the options on to the plot and cutflow making scripts
if arguments.localConfig:
    args = args + " -l " + arguments.localConfig
if arguments.normalizeToData:
    args = args + " -n "
if arguments.normalizeToUnitArea:
    args = args + " -u "
if arguments.noStack:
    args = args + " -e "
if arguments.makeRatioPlots:
    args = args + " -r "
if arguments.makeDiffPlots:
    args = args + " -d "
if arguments.draw2DPlots:
    args = args + " --2D "
if arguments.savePDFs:
    args = args + " -p "
if arguments.outputFileName:
    args = args + " -o " + arguments.outputFileName
if arguments.rebinFactor:
    args = args + " -b " + arguments.rebinFactor
if arguments.ratioYRange:
    args = args + " -R " + arguments.ratioYRange
if arguments.printYields:
    args = args + " -y "
if arguments.savePDFs:
    args = args + " -p "

if not arguments.quickMerge:
    print "running makePlots.py"
    os.system("makePlots.py %s" % args)
    print "running makeCutFlows.py"
    os.system("makeCutFlows.py %s" % args)
