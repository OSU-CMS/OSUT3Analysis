#!/usr/bin/env python3

# Usage: getDatasetJSON.py /DATA/SET/NAME
# The output is a JSON file called DATA_SET_NAME.txt containing the lumi
# sections in the dataset.

import sys, re, os
try:
    from CRABClient.UserUtilities import getLumiListInValidFiles
    from CRABClient.ClientExceptions import ClientException
    from WMCore.DataStructs.LumiList import LumiList
except ImportError:
    print("This tool relies on CRAB. Please set up the environment for CRAB before using.")
    sys.exit (2)

if len (sys.argv) < 2:
    print("Usage: " + os.path.basename (sys.argv[0]) + " DATASET [OUTPUT_FILE]")
    sys.exit (1)

dataset = sys.argv[1]
instance = "global"
if re.sub (r"^\/([^/]*)\/([^/]*)\/([^/]*)$", r"\3", dataset) == "USER":
  instance = "phys03"
if len (sys.argv) > 2:
  instance = sys.argv[2]

dataset0 = re.sub (r"^\/([^/]*)\/([^/]*)\/([^/]*)$", r"\1", dataset)
dataset1 = re.sub (r"^\/([^/]*)\/([^/]*)\/([^/]*)$", r"\2", dataset)
dataset2 = re.sub (r"^\/([^/]*)\/([^/]*)\/([^/]*)$", r"\3", dataset)
print("Getting JSON from DBS instance " + instance + "...")
try:
    taskALumis = getLumiListInValidFiles(dataset=dataset, dbsurl=instance)
except ClientException:
    print("Error accessing DAS. Please initialize your grid proxy before using.")
    sys.exit (3)
taskALumis.writeJSON(dataset0 + "_" + dataset1 + "_" + dataset2 + ".txt")
print("Wrote JSON to \"" + dataset0 + "_" + dataset1 + "_" + dataset2 + ".txt\".")
