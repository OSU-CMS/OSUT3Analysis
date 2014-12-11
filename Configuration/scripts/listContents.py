#!/usr/bin/env python

# Prints the keys of all objects in a .root file.
#
# Usage:
# listContents.py myfile.root

import sys
from ROOT import TFile, TDirectory, gDirectory

def getAllKeys(self):
    keylist = []
    for key in gDirectory.GetListOfKeys():
        mypath = ""
        self.filterKey(key, mypath, keylist)
    return keylist

def filterKey(self, key, currentpath, keylist):
    if key.IsFolder():
        if not currentpath.endswith('/') and not currentpath == "":
            currentpath += '/'
        subfolder =  currentpath + key.GetName()
        self.cd(subfolder)
        for k in gDirectory.GetListOfKeys():
            self.filterKey(k, subfolder, keylist)
    else:
        keylist.append(currentpath + '/' + key.GetName())
        return

TFile.filterKey = filterKey
TFile.getAllKeys = getAllKeys



if len(sys.argv) <= 1:
    print "Missing input file name."
    print "Sample usage: "
    print "> listContents.py myfile.root"
    sys.exit()

fname = sys.argv[1]

f = TFile(fname)
keys = f.getAllKeys()

for key in keys:
    obj = f.Get(key)
    print obj.ClassName(), ": ", key
print "Found", len(keys), "objects in ", fname
