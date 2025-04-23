#!/usr/bin/env python3

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
parser.add_option("-D", "--dir", default=None, type=str, help="Directory to perform merging on")
parser.add_option("-f", "--fileStr", default=None, type=str, help="Option to pass a file string to be used when looking for root files to merge (files will be required to have file string)")

(arguments, args) = parser.parse_args()

from OSUT3Analysis.Configuration.mergeUtilities import *

###############################################################################
#                           Getting the working directory.                    #
###############################################################################
CondorDir = ''
if arguments.condorDir == "" and arguments.dir is None:
    print("No working directory is given, aborting.")
    sys.exit()

else:
    if arguments.dir is not None:
        CondorDir = arguments.dir
        OutputDir = arguments.dir
    else:
        CondorDir = os.getcwd() + '/condor/' + arguments.condorDir
        OutputDir = os.getcwd() + "/condor/" + arguments.outputDirectory if arguments.outputDirectory else CondorDir
try:
    os.makedirs (OutputDir)
except OSError:
    pass

###############################################################################
#If running over directory from CRAB merge here
###############################################################################

if arguments.dir is not None:
    from DisappTrks.StandardAnalysis.IntegratedLuminosity_cff import *
    from OSUT3Analysis.Configuration.configurationOptions import crossSections
    if "CMSSW_12" in os.environ.get("CMSSW_BASE") or "CMSSW_13" in os.environ.get("CMSSW_BASE"):
        from DisappTrks.StandardAnalysis.miniAOD_124X_Ntuples import *
    else:
        print("There is no ntuples file for {}, either use the hist files to get event counts or add a new list of event counts".format(os.environ.get("CMSSW_BASE")))
    
    filesToMerge, totalEvents = getFilesFromCrab(arguments.dir, arguments.fileStr)

    print(arguments.IntLumi)
    if arguments.IntLumi not in lumi.keys():
        print(f"Error: did not find dataset {arguments.IntLumi} in luminosity file")
        sys.exit(1)
    targetLumi = lumi[arguments.IntLumi]
    
    if arguments.Dataset not in crossSections.keys():
        print(f"Error: did not find dataset {arguments.Dataset} in cross sections file")
        sys.exit(1)
    crossSection = crossSections[arguments.Dataset]

    if arguments.Dataset not in eventCounts.keys():
        print(f"Error: did not find dataset {arguments.Dataset} in event counts file")
        sys.exit(1)
    totalEvents = eventCounts[arguments.Dataset]

    print(f"Target Lumi ({arguments.IntLumi}): {targetLumi}")
    print(f"Cross section ({arguments.Dataset}): {crossSection}")
    print(f"Total Events ({arguments.Dataset}): {totalEvents}")

    Weight = float(targetLumi)*float(crossSection)/float(totalEvents)
    weightString = MakeWeightsString(Weight, filesToMerge)

    print(weightString)

    if arguments.outputDirectory:
        outputFile = arguments.outputDirectory
    else:
        outputFile = '{}/{}.root'.format(arguments.dir, arguments.Dataset)

    cmd = 'mergeTFileServiceHistograms -i ' + " ".join (filesToMerge) + ' -o ' + outputFile + ' -w ' + weightString
    print(cmd)

    os.system(cmd)

    sys.exit(0)

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
        print( "ERROR:  The -d and -l options cannot be used simultaneously." )
        exit(0)

if not arguments.localConfig:
    if arguments.Dataset == "":
        print( "There are no datasets to merge!" )
    else:
        split_datasets.append(arguments.Dataset)

if arguments.IntLumi != "":
    IntLumi = float(arguments.IntLumi)

# Remove duplicates
split_datasets     = list(set(split_datasets))
composite_datasets = list(set(composite_datasets))

if "optional_dict_ntupleEff" not in locals () and "optional_dict_ntupleEff" not in globals ():
    optional_dict_ntupleEff = {}

currentCondorSubArgumentsSet = {}
if arguments.verbose:
    print( "List of datasets: ", split_datasets )
if not arguments.compositeOnly and not arguments.UseCondor:
    for dataSet in split_datasets:
        mergeOneDataset(dataSet, IntLumi, CondorDir, OutputDir, optional_dict_ntupleEff, verbose = arguments.verbose, skipMerging = arguments.skipMerging)

if arguments.UseCondor:
    # Make necessary files for condor and submit condor jobs.
    print( '......................Using Condor!...........................' )
    currentCondorSubArgumentsSet = copy.deepcopy(CondorSubArgumentsSet)
    GetCompleteOrderedArgumentsSet(InputCondorArguments, currentCondorSubArgumentsSet)
    MakeSubmissionScriptForMerging(CondorDir, currentCondorSubArgumentsSet, split_datasets)
    MakeMergingConfigForCondor(CondorDir, OutputDir, split_datasets, IntLumi, optional_dict_ntupleEff)
    os.chdir(CondorDir)
    if arguments.NotToExecute:
        print( 'Configuration files created in ' + str(CondorDir) + ' directory but no jobs submitted.\n' )
    else:
        os.system('condor_submit condorMerging.sub')
else:
    os.chdir(CondorDir)
    for dataSet_component in composite_datasets:
        print( "................Merging composite dataset " + dataSet_component + " ................" )
        memberList = []
        for dataset in composite_dataset_definitions[dataSet_component]:
            if not os.path.exists(dataset + '.root'):
                print( dataset + '.root does not exist, component dataset ' + dataSet_component + ' wont be complete!' )
                continue
            memberList.append(dataset + '.root')
        InputFileString = " ".join (memberList)
        os.system('mergeTFileServiceHistograms -i ' + InputFileString + ' -o ' + OutputDir + "/" + dataSet_component + '.root')
        print( 'Finish merging composite dataset ' + dataSet_component )
        print( "...............................................................\n" )

if arguments.UseCondor:
    print( "After all condor merging jobs have finished, rerun mergeOut.py with the -C option to merge composite datasets." )
elif not arguments.compositeOnly:
    print( "Next time, consider using the -c option to send the merging jobs to the condor queue.")
print("Finished mergeOut.py.")
