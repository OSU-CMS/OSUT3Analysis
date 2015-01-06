#!/usr/bin/env python

# Must specify options with -l argument, and condor directory with -c argument, e.g.:  
# > makeBNTreePlot.py -l sampleBNTreePlotConfig.py -c myCondorDir

# Additional arugments specify the running mode:
# -D <dataset>:  Run on a single dataset (typically for testing)
# -C:  Submit jobs to run on condor over all datasets
# BNTreeUseScript=True (set in sampleBNTreePlotConfig.py):  run BNTreeScript root macro (also set in sampleBNTreePlotConfig.py),
#    which must take as arguments the condor directory, dataset, and channel 


import sys
import os
import re
from optparse import OptionParser
from array import *
from decimal import *
import subprocess

from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *


parser = OptionParser()
parser = set_commandline_arguments(parser)

###  Only used by makeBNTreePlot.py (maybe move to another file?)	 
parser.remove_option("-p")
parser.add_option("-D", "--dataset", dest="datasetName",	 
                  help="Name of dataset (overrides value from local configuration file)")	 
parser.add_option("-C", "--runOnCondor",  action="store_true", dest="runOnCondor", default=False,	 
                  help="Run on condor instead of interactively")	 
parser.add_option("-q", "--quickMerge",  action="store_true", dest="quickMerge", default=False,	 
                  help="Run merge only")	 
parser.add_option("-S", "--splitCondorJobs", action="store_true", dest="splitCondorJobs", default=False,	 	 
                  help="Split condor jobs to have one for each file, rather than one for each dataset")	 
parser.add_option("-p", "--condorProcessNum", dest="condorProcessNum", default=-1,	 
                  help="Specify which condor process to run (default is to run over all).")	 

(arguments, args) = parser.parse_args()
 

from ROOT import TChain, TCut, TDirectory, TFile, TH1D, TH2D, TStopwatch, TTree, gROOT  

gROOT.SetBatch(True)  # This is to prevent pop-up graphical windows


def MakeCondorSubmitFile(arguments, dataset):
    outdir = "condor/"+arguments.condorDir+"/"+dataset+"/"  
    p = subprocess.Popen(["which", "makeBNTreePlot.py"], stdout=subprocess.PIPE)
    executable = p.communicate()[0]  # read the stdout of the command
    workdir = os.getcwd() + "/" 

    # Count the number of hist*root files in the specified directory 
    p = subprocess.Popen("ls " + workdir + outdir + "hist*root | wc", shell=True, stdout=subprocess.PIPE)  
    out = p.communicate()[0]
    outSplit = out.split()
    totalJobs = int(outSplit[0])  # The first element of the list is the number of files

    ## print "Debug: outSplit = "
    ## print outSplit
    ## out, err = p.communicate()
    ## print "Debug: Size of out:"
    ## print len(out)
    ## print "out = "
    ## print out
    ## print "err = "
    ## print err 
    ## print "Debug:  p.communicate()[0] = " + p.communicate()[0]
    ## totalJobs = len(out)
    out = open(outdir+"/condorBNTree.sub", "w")
    out.write("Executable              = " + executable + " \n")
    out.write("Universe                = vanilla \n")
    out.write("Getenv                  = True \n")
    argCondorProcess = ""  
    if arguments.splitCondorJobs: 
        argCondorProcess = " -p $(Process) " 
    out.write("Arguments               = -D " + dataset + " -l " + arguments.localConfig + " -c " + arguments.condorDir + argCondorProcess + " \n")
    out.write("Output                  = " + workdir + outdir + "condorBNTree_$(Process).out \n")
    out.write("Error                   = " + workdir + outdir + "condorBNTree_$(Process).err \n")
    out.write("Log                     = " + workdir + outdir + "condorBNTree_$(Process).log \n")
    out.write("+IsLocalJob             = true \n")
    out.write("Rank                    = TARGET.IsLocalSlot \n")
    if arguments.splitCondorJobs: 
        out.write("Queue " + str(totalJobs) + " \n")
    else: 
        out.write("Queue 1 \n")
    out.close()  


def RunOnCondor(arguments, split_datasets):  
    print "Running jobs on condor, instead of interactively."
    ## print "Found condorDir = %s" % (condorDir)
    ## print "Found split_datasets = "
    ## print split_datasets

    for dataset in split_datasets:  
        MakeCondorSubmitFile(arguments, dataset)  
        cmd = "condor_submit condor/"+arguments.condorDir+"/"+dataset+"/condorBNTree.sub"
        os.system(cmd)
        print "Submitting job: %s " % cmd

    print "Once condor jobs have finished, merge the composite datasets and then make plots with:"  
    if arguments.splitCondorJobs: 
        print "    mergeOutput.py -q    -l " + arguments.localConfig + " -c " + arguments.condorDir
    else: 
        print "    makeBNTreePlot.py -q -l " + arguments.localConfig + " -c " + arguments.condorDir
    print     "    makePlots.py         -l " + arguments.localConfig + " -c " + arguments.condorDir  
        
    return


watch  = TStopwatch()
watch1 = TStopwatch()


if not arguments.localConfig:
    sys.exit(" You must specify a localOptions.py file with -l")
if arguments.localConfig:  
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.localConfig) + " import *")
if not arguments.condorDir:
    sys.exit(" You must specify a condor directory  with -c")
if arguments.condorDir:
    condor_dir = "condor/%s" % arguments.condorDir

if arguments.datasetName:  # If datasetName is specified on command line, then override the value from localConfig  
    datasets = [
        arguments.datasetName,
    ]

#save a list of composite datasets 
composite_datasets = get_composite_datasets(datasets, composite_dataset_definitions)
#save a list of datasets with composite datasets split up 
split_datasets = split_composite_datasets(datasets, composite_dataset_definitions)

if arguments.quickMerge and arguments.runOnCondor:
    print "Cannot use -q (--quickMerge) and -C (--runOnCondor) options simultaneously.  Please choose one or the other."
    exit()  

if arguments.runOnCondor:
    RunOnCondor(arguments, split_datasets)
    exit()    

#write new histogram to dataset
if not arguments.quickMerge: 
    for dataset in split_datasets:
        if BNTreeUseScript:
            chainName = "OSUAnalysis/" + BNTreeChannel + "/BNTree_" + BNTreeChannel
            command = "root -l -b -q '" + BNTreeScript + "+(\"" + condor_dir + "\",\"" + dataset + "\",\"" + chainName + "\"," + str(arguments.condorProcessNum) + ")'"  
            print "About to execute command:  " + command  
            os.system(command)
        else: 
            for hist in input_histograms:
                #chain trees together
                ch = TChain("OSUAnalysis/"+hist['channel']+"/BNTree_"+hist['channel'])
                ch.Add(condor_dir + "/" + dataset + "/hist_*.root")
                print ("Looping over chain with # entries = %f; split time = " % ch.GetEntries()), 
                watch1.Stop(); watch1.Print(); watch1.Start()

                outputFile = TFile(condor_dir + "/" + dataset + ".root", "UPDATE")
                if not outputFile or outputFile.IsZombie():  print "Could not open file: %s/%s.root" % (condor_dir, dataset)  
                outputFile.cd("OSUAnalysis/"+hist['channel'])

                deleteString = hist['histName'] + ";*"  # delete all existing instances of the object
                currentDir = outputFile.GetDirectory("OSUAnalysis/"+hist['channel']);
                if not currentDir: print "Could not find directory OSUAnalysis/%s in file %s" % (hist['channel'], outputFile.GetName())  
                currentDir.Delete(deleteString);
                if 'nbinsY' in hist:  # only make a 2D histogram if the key "nbinsY" is defined  
                    h = TH2D(hist['histName'], hist['histName'],
                             hist['nbins'],  hist['xMin'], hist['xMax'],
                             hist['nbinsY'], hist['yMin'], hist['yMax'])
                else:             
                    h = TH1D(hist['histName'], hist['histName'], hist['nbins'], hist['xMin'], hist['xMax'])
                h.Sumw2()  # Needed to get weights correct.  
                cut = TCut(hist['cutString'])
                ch.Draw(hist['varToPlot']+">>"+hist['histName'], cut)  
                h.Write()
                outputFile.Close()
                print "Histogram " + hist['histName'] + " has been added to " + condor_dir + "/"+ dataset + ".root"

#merge output if composite dataset
for composite_dataset in composite_datasets:
    component_datasets_list = ""
    component_dataset_file_path = ""
    for component_dataset in composite_dataset_definitions[composite_dataset]:
        component_dataset_dir = "%s/%s" % (condor_dir,component_dataset)
        component_dataset_file_path = component_dataset_dir + ".root"
        if os.path.isfile(component_dataset_file_path):
            component_datasets_list += " " + component_dataset_file_path
    composite_dataset_dir = "%s/%s" % (condor_dir,composite_dataset)
    command = "mergeHists -p %s %s" % (composite_dataset_dir, component_datasets_list)
    print "Merging output for composite dataset: " + composite_dataset
    os.system(command)
      
print ("Total time to run makeBNTreePlot.py: "), 
watch.Stop()
watch.Print()
            


