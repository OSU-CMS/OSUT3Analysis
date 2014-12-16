#!/usr/bin/env python

# Prints the keys of all objects in a .root file.
#
# Usage:
# listContents.py myfile.root

import sys

from OSUT3Analysis.Configuration.fileUtilities import *

if len(sys.argv) <= 1:
    print "Missing input file name."
    print "Sample usage: "
    print "> listContents.py myfile.root"
    sys.exit()

fname = sys.argv[1]

f = TFile(fname)
keys = f.GetAllKeys()

for key in keys:
    obj = f.Get(key)
    print obj.ClassName(), ": ", key
print "Found", len(keys), "objects in ", fname
