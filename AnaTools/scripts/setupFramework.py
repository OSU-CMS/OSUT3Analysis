#!/usr/bin/env python3

import sys
import os
import subprocess
from optparse import OptionParser

# define some constants with meaningful names for the ANSI color codes
# https://en.wikipedia.org/wiki/ANSI_escape_code#Colors

A_BLACK           =  "\033[30m"
A_RED             =  "\033[31m"
A_GREEN           =  "\033[32m"
A_YELLOW          =  "\033[33m"
A_BLUE            =  "\033[34m"
A_MAGENTA         =  "\033[35m"
A_CYAN            =  "\033[36m"
A_WHITE           =  "\033[37m"

A_BRIGHT_BLACK    =  "\033[1;30m"
A_BRIGHT_RED      =  "\033[1;31m"
A_BRIGHT_GREEN    =  "\033[1;32m"
A_BRIGHT_YELLOW   =  "\033[1;33m"
A_BRIGHT_BLUE     =  "\033[1;34m"
A_BRIGHT_MAGENTA  =  "\033[1;35m"
A_BRIGHT_CYAN     =  "\033[1;36m"
A_BRIGHT_WHITE    =  "\033[1;37m"

A_RESET           =  "\033[0m"

supported_formats = ["AOD", "MINI_AOD", "MINI_AOD_2017", "MINI_AOD_2022", "MINI_AOD_ONLY_2022"]

parser = OptionParser()
parser.add_option("-f", "--format", dest="data_format",
                  help="data format to switch to - current options: " + ",".join(supported_formats))
parser.add_option("-r", "--reset", action="store_true", dest="reset", default=False,
                  help="reset to original version and exit")
parser.add_option("-v", "--version", action="store_true", dest="version", default=False,
                  help="print data format version and exit")
parser.add_option("-c","--config", dest="custom_config",
                  help="path to config file specifying the custom format (starting with package name)")
parser.add_option("-d", "--define", dest="custom_define",
                  help="comma-separated list of custom preprocessor definitions to provide with #define. Examples: \"-d DEF1\", \"-d DEF1,DEF2\"")

(arguments, args) = parser.parse_args()

if arguments.version:
    data_line = ""
    with open(os.environ.get("CMSSW_BASE")+"/src/OSUT3Analysis/AnaTools/interface/DataFormat.h") as f:
        for line in f:
            if "#define DATA_FORMAT" in line:
                data_line = line
                break
    print( "Data type is currently '" + data_line.split()[2] + "'" )
    sys.exit(0)

if arguments.reset:
    os.system('git checkout $CMSSW_BASE/src/OSUT3Analysis/AnaTools/interface/DataFormat.h')
    print( "Data format has been reset.")
    print( "Do not forget to recompile.")
    sys.exit(0)

if not arguments.data_format:
    print( "Please specify data type with '-f' option")
    sys.exit(0)

if arguments.data_format not in supported_formats:
    print( "'"+arguments.data_format+"' is not a supported format.")
    print( "Supported formats are the following: ",supported_formats)
    sys.exit(0)

if arguments.custom_config:
    arguments.data_format += "_CUSTOM"

if arguments.custom_define:
    for definition in arguments.custom_define.split(','):
        print( "Defining custom variable: #define " + definition)
        os.system('sed -i "16i #define %s" $CMSSW_BASE/src/OSUT3Analysis/AnaTools/interface/DataFormat.h' % definition)

# redefine the data format in C++
os.system('sed -i "s/#define DATA_FORMAT .*/#define DATA_FORMAT %s/g" $CMSSW_BASE/src/OSUT3Analysis/AnaTools/interface/DataFormat.h' % (arguments.data_format))

# redefine the data format in python
os.system('sed -i "s/dataFormat = .*/dataFormat = \\"%s\\"/g" $CMSSW_BASE/src/OSUT3Analysis/AnaTools/python/osuAnalysis_cfi.py' % (arguments.data_format))

# CMSSW_X_Y_Z
# Ignore patchN and preN
# Interpret things like X_Y_ROOT6 or _CLANG or _THREADED as X_Y_0
CMSSWVersionCode = 0
versionWords = os.environ["CMSSW_VERSION"].split("_")[1:]
if len(versionWords) >= 3:
    CMSSWVersionCode = int(versionWords[0]) << 16 # X
    CMSSWVersionCode += int(versionWords[1]) << 8 # Y
    if versionWords[2].isdigit():
        CMSSWVersionCode += int(versionWords[2])  # Z
os.system('sed -i "s/#define CMSSW_VERSION_CODE .*/#define CMSSW_VERSION_CODE %d/g" $CMSSW_BASE/src/OSUT3Analysis/AnaTools/interface/CMSSWVersion.h' % (CMSSWVersionCode))

if arguments.custom_config:
    os.system('sed -i "s:.*CustomDataFormat.h.*:%s:" $CMSSW_BASE/src/OSUT3Analysis/AnaTools/interface/DataFormat.h' % ('  #include \\\"' + arguments.custom_config + '\\\"'))

os.utime (os.environ["CMSSW_BASE"] + "/src/OSUT3Analysis/AnaTools/interface/DataFormat.h", None)
print( "Data format changed to " + arguments.data_format + ".")
print( "Do not forget to recompile.")

#enable git hooks
os.system("ln -s $CMSSW_BASE/src/OSUT3Analysis/githooks/* $CMSSW_BASE/src/OSUT3Analysis/.git/hooks/")

# warn user about v2 photon ID availability in 9_4_X
if os.environ["CMSSW_VERSION"].startswith("CMSSW_9_4_") and int(os.environ["CMSSW_VERSION"].split("_")[3]) < 13:
    print( "")
    print( "If using photons, note that v2 IDs are only available in >= 9_4_13")
    print( "Your current release uses v1 photon IDs; consider changing to a newer release")

# check that necessary merge-topics have been done
if os.environ["CMSSW_VERSION"].startswith("CMSSW_9_4_") and int(os.environ["CMSSW_VERSION"].split("_")[3]) >= 9:
    if not os.path.isfile(os.environ["CMSSW_BASE"] + "/src/RecoEgamma/ElectronIdentification/python/Identification/cutBasedElectronID_Fall17_94X_V2_cff.py"):
        print( "")
        print( "If using electrons, please run the following before recompiling:" )
        print( A_BRIGHT_RED + "  git cms-merge-topic UAEDF-tomc:eleCutBasedId_94X_V2" + A_RESET )
    if not os.path.isfile(os.environ["CMSSW_BASE"] + "/src/L1Prefiring/EventWeightProducer/python/L1ECALPrefiringWeightProducer_cfi.py"):
        print( "" )
        print( "If your analysis needs to correct for L1 ECAL prefiring issue, please run the following before recompling:")
        print( A_BRIGHT_RED + "  git cms-merge-topic lathomas:L1Prefiring_9_4_9" + A_RESET )

if os.environ["CMSSW_VERSION"].startswith("CMSSW_8_0_"):
    if not os.path.isfile(os.environ["CMSSW_BASE"] + "/src/RecoEgamma/PhotonIdentification/python/Identification/cutBasedPhotonID_PHYS14_PU20bx25_V1_cff.py"):
        print( "" )
        print( "If using photons, please run the following before recompiling:" )
        print( A_BRIGHT_RED + "  git cms-merge-topic ikrav:egm_id_80X_v3_photons" + A_RESET)
    if int(os.environ["CMSSW_VERSION"].split("_")[3]) >= 32 and not os.path.isfile(os.environ["CMSSW_BASE"] + "/src/L1Prefiring/EventWeightProducer/python/L1ECALPrefiringWeightProducer_cfi.py"):
        print( "" )
        print( "If your analysis needs to correct for L1 ECAL prefiring issue, please run the following before recompling:" )

        print( A_BRIGHT_RED + "  git cms-merge-topic lathomas:L1Prefiring_8_0_32" + A_RESET)

# Set up the BuildFile
if os.environ["CMSSW_VERSION"].startswith(("CMSSW_12_4_", "CMSSW_13_0_", "CMSSW_15_0_")):
    for r,directory,fileNames in os.walk("./"):
       for fin in fileNames:
            if fin != "BuildFile.xml":
                continue
            buildFile = os.path.join(r,fin)
            subprocess.call(["sed -i -e 's/RecoEgamma\/EgammaTools/CommonTools\/Egamma/g' {0}".format(buildFile)],shell=True)
            print("Resetting the BuildFile:{0}".format(buildFile))
