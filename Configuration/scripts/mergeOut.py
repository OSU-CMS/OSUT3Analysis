#!/usr/bin/env python

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
parser.add_option("-C", "--compositeOnly", dest="compositeOnly", default = False, action = "store_true", help="Only merge composite datasets; do not merge individual datasets.")
parser.add_option("-L", "--targetLumi", dest="IntLumi", default = "", help="Specify the targeting luminosity.")
parser.add_option("-c", "--condor", dest="UseCondor", default = False,action = "store_true", help="Run merging jobs on condor.")
parser.add_option("-N", "--noExec", action="store_true", dest="NotToExecute", default = False, help="Just generate necessary config files without executing them.")
parser.add_option("-O", "--output-dir", dest="outputDirectory", help="specify an output directory for output file, default is to use the Condor directory")
parser.add_option("-v", "--verbose", action="store_true", dest="verbose", default=False, help="verbose output")
parser.add_option("-s", "--skipMerging", action="store_true", dest="skipMerging", default=False, help="skip merging of histogram files (only check and create resub scripts).")

(arguments, args) = parser.parse_args()

from OSUT3Analysis.Configuration.mergeUtilities import *

###############################################################################
#                           Getting the working directory.                    #
###############################################################################
CondorDir = ''
if arguments.condorDir == "":
    print "No working directory is given, aborting."
    sys.exit()
else:
    CondorDir = os.getcwd() + '/condor/' + arguments.condorDir
OutputDir = os.getcwd() + "/condor/" + arguments.outputDirectory if arguments.outputDirectory else CondorDir
try:
    os.makedirs (OutputDir)
except OSError:
    pass

###############################################################################
#Check whether the necessary arguments or the local config are given correctly#
###############################################################################
split_datasets = []
composite_datasets = []
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

# Remove duplicates
split_datasets     = list(set(split_datasets))
composite_datasets = list(set(composite_datasets))

if "optional_dict_ntupleEff" not in locals () and "optional_dict_ntupleEff" not in globals ():
    optional_dict_ntupleEff = {}

currentCondorSubArgumentsSet = {}
if arguments.verbose:
    print "List of datasets: ", split_datasets
if not arguments.compositeOnly and not arguments.UseCondor:
    for dataSet in split_datasets:
        mergeOneDataset(dataSet, IntLumi, CondorDir, OutputDir, optional_dict_ntupleEff, verbose = arguments.verbose, skipMerging = arguments.skipMerging)

if arguments.UseCondor:
    # Make necessary files for condor and submit condor jobs.
    print '......................Using Condor!...........................'
    currentCondorSubArgumentsSet = copy.deepcopy(CondorSubArgumentsSet)
    GetCompleteOrderedArgumentsSet(InputCondorArguments, currentCondorSubArgumentsSet)
    MakeSubmissionScriptForMerging(CondorDir, currentCondorSubArgumentsSet, split_datasets)
    MakeMergingConfigForCondor(CondorDir, OutputDir, split_datasets, IntLumi, optional_dict_ntupleEff)
    os.chdir(CondorDir)
    if arguments.NotToExecute:
        print 'Configuration files created in ' + str(CondorDir) + ' directory but no jobs submitted.\n'
    else:
        os.system('condor_submit condorMerging.sub')
else:
    os.chdir(CondorDir)
    for dataSet_component in composite_datasets:
        print "................Merging composite dataset " + dataSet_component + " ................"
        memberList = []
        for dataset in composite_dataset_definitions[dataSet_component]:
            if not os.path.exists(dataset + '.root'):
                print dataset + '.root does not exist, component dataset ' + dataSet_component + ' wont be complete!'
                continue
            memberList.append(dataset + '.root')
        InputFileString = " ".join (memberList)
        os.system('mergeTFileServiceHistograms -i ' + InputFileString + ' -o ' + OutputDir + "/" + dataSet_component + '.root')
        print 'Finish merging composite dataset ' + dataSet_component
        print "...............................................................\n"

if arguments.UseCondor:
    print "After all condor merging jobs have finished, rerun mergeOut.py with the -C option to merge composite datasets."
elif not arguments.compositeOnly:
    print "Next time, consider using the -c option to send the merging jobs to the condor queue."
print "Finished mergeOut.py."
