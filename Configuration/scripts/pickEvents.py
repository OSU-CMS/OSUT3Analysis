#!/usr/bin/env python
import os 
import re 
from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *
from optparse import OptionParser
from multiprocessing import Process

parser = OptionParser ("Usage: %prog [options]", description="This script retrieves original events corresponding to the events selected in a skim.  Before running, you must have a valid proxy, i.e., run voms-proxy-init -voms cms.")  


parser.add_option("-l", "--localConfig", dest="localConfig",
                  help="local configuration file")
parser.add_option("-w", "--workDirectory", dest="condorDir",
                  help="condor working directory")
parser.add_option("-c", "--channel", dest="channel", default = "", help="Channel directory containing skims")  
parser.add_option("-r", "--replace", dest="replace", default = "", help="In the dataset name, replace orig with new.  Argument must be in form orig,new.  Example argument:  MINIAODSIM,AODSIM.")  
parser.add_option("-e", "--eventsOnly", dest="eventsOnly", action="store_true", default = False, 
                  help="Create event list only.  Do not run edmPickEvents.py")  
parser.add_option("-v", "--verbose", dest="verbose", action="store_true", default = False, help="Verbose output.")
parser.add_option("--redirector", dest="Redirector", default = "FNAL", help="Setup the redirector for xrootd service to use.  Options:  Infn, FNAL (default), Global.")  

# FIXME:  Add option to submit jobs to condor.  See example:  mergeOut.py.  


(arguments, args) = parser.parse_args()

#Define the dictionary to look for the redirectors given the users input. 
RedirectorDic = {'Infn':'xrootd.ba.infn.it','FNAL':'cmsxrootd.fnal.gov','Global':'cms-xrd-global.cern.ch'}

def createEventList():  
    files = os.popen('ls skim*root -1').read().split('\n')  
    print "Files = ", files
    eventList = ""
    nEventsTot = 0
    for f in files:
        if f == '':  
            continue  # Skip blank lines.  
        nEvents = -1
        logFileUtil = os.popen('edmFileUtil    -f ' + f).read().split(' ')  # First get number of events
        print "Debug: logFileUtil = ", logFileUtil  
        for i in range(len(logFileUtil)-1, -1, -1):  # Iterate backwards over the words in the output.  
            if "events" in logFileUtil[i]:
                nEvents = int(logFileUtil[i-1])  # The number of events comes immediately before the word "events"  
        print "Debug:  nEvents = ", nEvents, "file = ", f  
        if nEvents < 0:
            print "ERROR: Failed to find valid number of events for file: ", f
            print "Will exit prematurely."
            exit(0)  
        nEventsTot += nEvents
        logFileUtil = os.popen('edmFileUtil -e -f ' + f).read().split('\n')  # Now get list of events
        nFoundEvts = 0
        for line in logFileUtil:
            words = line.split(' ')  
            words = filter(None, words) # Remove empty elements  
            # Content of lines corresponding to a valid event is in the format: 
            # Run           Lumi          Event    TTree Entry
            # print "Debug:  checking line=", line, "words=", words  
            if len(words) == 4:
                run  = words[0]
                lumi = words[1]
                evt  = words[2]
                eventList += run + ":" + lumi + ":" + evt + "\n"
                nFoundEvts += 1  
                print "Debug:  filling words for line: ", line, ", run = ", run, "lumi = ", lumi 
        if nFoundEvts != nEvents:
            print "ERROR:  Number of found events = ", nFoundEvts, " does not match number of expected events = ", nEvents, ".  Please investigate."  
            exit(0) 
        fout = open("pickevents.txt", "w")
        fout.write(eventList)
        fout.close() 


def runEdmPickEvents(dataset): 
    datasetName = dataset_names[dataset] 
    if arguments.replace:
        orig = arguments.replace.split(",")[0]
        new  = arguments.replace.split(",")[1]
        datasetName = datasetName.replace(orig, new)  
    cmd = 'edmPickEvents.py "' + datasetName + '" pickevents.txt --printInteractive > pickevents.src'      
    if arguments.verbose:  
        print "Running command: ", cmd 
    os.system(cmd)  
    redirector = RedirectorDic[arguments.Redirector]  
    # Replace "/store/" with, e.g., root://cmsxrootd.fnal.gov///store/
    os.system('sed -i "s_/store/_root://' + redirector + '///store/_g" pickevents.src')  
    os.system("source pickevents.src")  
    

def doOneDirectory(directory):
    if not os.path.exists(directory):
        print directory + " does not exist, will skip it and continue!"
        return 
    os.chdir(directory)
    createEventList()
    if not arguments.eventsOnly:
        runEdmPickEvents(dataset)  
    print "Created and executed command file: " + os.getcwd() + "/pickevents.src"  


###############################################################################
#                           Get the working directory.                        #
###############################################################################
CondorDir = ''
if arguments.condorDir == "":
    print "No working directory is given, aborting."
    sys.exit()
else:
    condorDir = os.getcwd() + '/condor/' + arguments.condorDir

###############################################################################
#                           Get the list of datasets.                         #
###############################################################################
split_datasets = []
if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.localConfig) + " import *")
    composite_datasets = get_composite_datasets(datasets, composite_dataset_definitions)
    split_datasets   = split_composite_datasets(datasets, composite_dataset_definitions)
else:
    print "There are no datasets specified!"
    exit(0)  
    


###############################################################################
#                           Loop over datasets.                               #
###############################################################################
for dataset in split_datasets:
    directory = condorDir + '/' + dataset + '/' + arguments.channel  
    p = Process(target=doOneDirectory, args=(directory,))  
    p.start()
    p.join()
    
