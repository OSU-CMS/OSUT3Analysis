#!/usr/bin/env python

import sys
import os
from optparse import OptionParser


supported_formats = ["AOD","MINI_AOD"]

parser = OptionParser()
parser.add_option("-f", "--format", dest="data_format",
                  help="data format to switch to - current options: " + ",".join(supported_formats))
parser.add_option("-r", "--reset", action="store_true", dest="reset", default=False,
                  help="reset to original version and exit")
parser.add_option("-v", "--version", action="store_true", dest="version", default=False,
                  help="print data format version and exit")
parser.add_option("-c","--config", dest="custom_config",
                  help="path to config file specifying the custom format (starting with package name)")

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

os.system('sed -i "s/#define DATA_FORMAT .*/#define DATA_FORMAT %s/g" $CMSSW_BASE/src/OSUT3Analysis/AnaTools/interface/DataFormat.h' % (arguments.data_format))

if arguments.custom_config:
    os.system('sed -i "s:.*CustomDataFormat.h.*:%s:" $CMSSW_BASE/src/OSUT3Analysis/AnaTools/interface/DataFormat.h' % ('  #include \\\"' + arguments.custom_config + '\\\"'))

os.system('touch $CMSSW_BASE/src/OSUT3Analysis/AnaTools/interface/DataFormat.h')
print "Data format changed to " + arguments.data_format + "."
print "Do not forget to recompile."
