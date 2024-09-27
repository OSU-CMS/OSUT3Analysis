#!/usr/bin/python3

import json
import ROOT as r
import numpy as np
import sys
import os

r.gInterpreter.Declare(
    '''
    #include "OSUT3Analysis/DBTools/interface/getEvents.h"

    void dummyWrapper(){
        getEvents();
    }
    '''
)
r.gSystem.Load('libOSUT3AnalysisDBTools.so')

if __name__ == "__main__":


    if not len(sys.argv) >= 3:
        print("Error: Need to provide the input json file")
        sys.exit(1)

    outputDir = '/data/users/mcarrigan/condor/EventLists/'
    if len(sys.argv) >= 4:
        outputDir = sys.argv[3]

    f_json = sys.argv[1]

    job = int(sys.argv[2])

    dataset = f_json.split('/')[-1].split('-')[0]

    outputPath = outputDir + dataset + '/'

    if not os.path.exists(outputPath):
        os.mkdir(outputPath)

    with open(f_json) as secondary_fin:
        secondary_dict = json.load(secondary_fin)
        filename = list(secondary_dict.keys())[job]

    #for filename in secondary_dict.keys():
    if not filename.startswith('root://'): filename = 'root://cmsxrootd.fnal.gov:/' + filename
    events = r.getEventsInFile(filename)
    tmpEvents = np.array([str(x.runNum)+':'+str(x.lumiBlock)+':'+str(x.event) for x in events])
    print(tmpEvents, len(tmpEvents))
    fileStr = filename.split('/')[-1]
    outputFile = outputPath + fileStr
    np.savez(outputFile, eventList=tmpEvents)
    #break