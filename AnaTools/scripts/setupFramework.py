#!/usr/bin/env python

import sys
import os
from optparse import OptionParser

supported_formats = ["AOD", "MINI_AOD", "MINI_AOD_2017"]

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
    print "Data type is currently '" + data_line.split()[2] + "'"
    sys.exit(0)

if arguments.reset:
    os.system('git checkout $CMSSW_BASE/src/OSUT3Analysis/AnaTools/interface/DataFormat.h')
    print "Data format has been reset."
    print "Do not forget to recompile."
    sys.exit(0)

if not arguments.data_format:
    print "Please specify data type with '-f' option"
    sys.exit(0)

if arguments.data_format not in supported_formats:
    print "'"+arguments.data_format+"' is not a supported format."
    print "Supported formats are the following: ",supported_formats
    sys.exit(0)

if arguments.custom_config:
    arguments.data_format += "_CUSTOM"

if arguments.custom_define:
    for definition in arguments.custom_define.split(','):
        print "Defining custom variable: #define " + definition
        os.system('sed -i "16i #define %s" $CMSSW_BASE/src/OSUT3Analysis/AnaTools/interface/DataFormat.h' % definition)

os.system('sed -i "s/#define DATA_FORMAT .*/#define DATA_FORMAT %s/g" $CMSSW_BASE/src/OSUT3Analysis/AnaTools/interface/DataFormat.h' % (arguments.data_format))

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
print "Data format changed to " + arguments.data_format + "."
print "Do not forget to recompile."

#enable git hooks
os.system("cd " + os.environ["CMSSW_BASE"] + "/src/OSUT3Analysis/.git/hooks/; ln -s ../../githooks/pre-commit .; ln -s ../../githooks/pre-commit.d .; ln -s ../../githooks/post-checkout .; ln -s ../../githooks/post-checkout.d .")
