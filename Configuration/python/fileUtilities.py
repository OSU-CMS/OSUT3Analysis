#!/usr/bin/env python

# Utilities for management of ROOT files.

from ROOT import TFile, TDirectory, gDirectory

def GetAllKeys(self):
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
TFile.GetAllKeys = GetAllKeys


