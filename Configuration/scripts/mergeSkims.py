#!/usr/bin/env python3

# Script to merge skim output files into a single file.

import os
import re
from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *
from optparse import OptionParser
parser = OptionParser()


parser.add_option("-l", "--localConfig", dest="localConfig",
                  help="local configuration file")
parser.add_option("-w", "--workDirectory", dest="condorDir",
                  help="condor working directory")
parser.add_option("-d", "--dataset", dest="dataset", default = "", help="Specify which dataset to run.")
parser.add_option("-v", "--verbose", dest="verbose", action="store_true", default = False, help="Verbose output.")

(arguments, args) = parser.parse_args()


###############################################################################
#                           Get the working directory.                        #
###############################################################################
CondorDir = ''
if arguments.condorDir == "":
    print("No working directory is given, aborting.")
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
elif arguments.dataset != "":
    split_datasets.append(arguments.dataset)
else:
    print("There are no datasets to merge!")


###############################################################################
#                           Loop over datasets.                               #
###############################################################################
for dataset in split_datasets:
    directory = condorDir + '/' + dataset
    if not os.path.exists(directory):
        print(directory + " does not exist, will skip it and continue!")
        continue
    os.chdir(directory)

    # Get a list of all skim channels, each of which is in a different directory
    channels = os.popen('ls -d */ -1').read().split('\n')
    for channel in channels:
        if channel == 'merged/' or channel == '':
            continue
        skimFiles = os.popen('ls ' + channel + '*root -1').read().split('\n')
        command = 'edmCopyPickMerge inputFiles="'
        for skimFile in skimFiles:
            if ".root" in skimFile:
                command += "file:" + skimFile + ","
        command = command[:-1]  # Remove trailing comma
        command += '" outputFile="' + channel + 'merged/skimMerged.root"'
        if arguments.verbose:
            print("command = ", command)
        os.makedirs (directory + "/" + channel + "merged")
        os.system(command)
        print("Created condor/" + arguments.condorDir + "/" + dataset + "/" + channel + "merged/skimMerged.root")
