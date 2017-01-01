#!/usr/bin/env python

# Usage: getDatasetJSON.py /DATA/SET/NAME
# The output is a JSON file called DATA_SET_NAME.txt containing the lumi
# sections in the dataset.

from CRABClient.UserUtilities import getLumiListInValidFiles
from WMCore.DataStructs.LumiList import LumiList
import sys
import re

dataset = sys.argv[1]
instance = "global"
if re.sub (r"^\/([^/]*)\/([^/]*)\/([^/]*)$", r"\3", dataset) == "USER":
  instance = "phys03"
if len (sys.argv) > 2:
  instance = sys.argv[2]

dataset0 = re.sub (r"^\/([^/]*)\/([^/]*)\/([^/]*)$", r"\1", dataset)
dataset1 = re.sub (r"^\/([^/]*)\/([^/]*)\/([^/]*)$", r"\2", dataset)
dataset2 = re.sub (r"^\/([^/]*)\/([^/]*)\/([^/]*)$", r"\3", dataset)
print "Getting JSON from DBS instance " + instance + "..."
taskALumis = getLumiListInValidFiles(dataset=dataset, dbsurl=instance)
taskALumis.writeJSON(dataset0 + "_" + dataset1 + "_" + dataset2 + ".txt")
print "Wrote JSON to \"" + dataset0 + "_" + dataset1 + "_" + dataset2 + ".txt\"."
