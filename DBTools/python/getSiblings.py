import os
import sys
import json
import argparse
import psutil
import ROOT as r
from OSUT3Analysis.DBTools.osusub_cfg import getRun3SkimSiblings
from FWCore.PythonUtilities.LumiList   import LumiList
from DataFormats.FWLite import Lumis, Handle
from multiprocessing import Process, Queue
import subprocess
import numpy as np
import math
import importlib
import ast

r.gInterpreter.Declare(
    '''
    #include "OSUT3Analysis/DBTools/interface/getEvents.h"

    void dummyWrapper(){
        getEvents();
    }
    '''
)
r.gSystem.Load('libOSUT3AnalysisDBTools.so')

# class used to get the secondary files (siblings) of the primary input files
class getSiblings():

    def __init__(self, inputJSON, secondaryJSON, label):

        self.numEvents = 0
        self.numSharedEvents = 0
        self.numPrimaryEvents = 0
        self.numSiblingEvents = 0
        self.sharedEvents = np.array([])


        self.dataset_in = None
        self.dataset_sib = None

        self.inputJSON = inputJSON
        self.secondaryJSON = secondaryJSON
        self.label = label

        self.nJobs = -1
        self.jobNumber = -1
        self.eventsPerJob = -1

        self.local=False

    #main function for getting siblings using either lumi matching or DBS
    def getSiblings(self):
        if args.prod == 'local' or args.prod == 'allLocal' or args.lumiMatching:
            #self.getLumiMatching(self.dictName, args.prod, args.inputJSON, args.siblingJSON)
            print("Running local option...")
            self.local = True
            self.getFilesFromList(args.jobNumber, args.totalJobs)
            self.findMatches()
        else:
            self.getFilesFromList(args.jobNumber, args.totalJobs)
            self.findMatches()

    @staticmethod
    def getDASInfo(dataset, jsonName=None):
        cmd = 'dasgoclient -query="file,run,lumi,events dataset={0}" -json'.format(dataset)
        process = subprocess.Popen(cmd, stdout=subprocess.PIPE, shell=True)
        output, error = process.communicate()

        miniaod = output.decode()

        test = ast.literal_eval(miniaod)
        files = [x['file'][0]['name'] for x in test]
        lumis = [[x['lumi'][0]['number']] if isinstance(x['lumi'][0]['number'], int) else x['lumi'][0]['number'] for x in test]
        runs = [[x['run'][0]['run_number']] if isinstance(x['run'][0]['run_number'], int) else x['run'][0]['run_number'] for x in test]

        miniDict = {}

        for f, l, r in zip(files, lumis, runs):
            miniDict[f] = {'lumis': l, 'runs': r}

        if jsonName:
            with open(jsonName, 'w') as fout:
                json.dump(miniDict, fout)

        return miniDict

    @staticmethod
    def getLocalInfo(dataset, jsonName=None):

        miniDict = {}

        for filename in os.listdir(dataset):
            if not filename.endswith('.root'): continue
            print("Working on file", dataset+filename)
            lumi_list = []
            run_list = []

            ''' fin = r.TFile.Open(dataset + filename, 'read')
            lumis = fin.Get("LuminosityBlocks")
            for i, ilumi in enumerate(lumis):
                lumis.GetEntry(i)
                lumi_list.append(lumis.LuminosityBlockAuxiliary.luminosityBlock())
                run_list.append(lumis.LuminosityBlockAuxiliary.run())'''
            
            events = r.getEventsInFile(dataset+filename)

            for event in events:
                lumi_list.append(event.lumiBlock)
                run_list.append(event.runNum)

            #fin.Close()
            miniDict[dataset+filename] = {'lumis': lumi_list, 'runs': run_list}

        if jsonName:
            with open(jsonName, 'w') as fout:
                json.dump(miniDict, fout)

        return miniDict

    def findMatches(self, jsonName='default.json'):

        #jsonName = self.label + '.json'

        eventCount = 0
        
        siblings = {}
        #open primary json file and get runs, lumis
        with open(self.inputJSON) as primary_fin:
            primary_dict = json.load(primary_fin)

            #open secondary json and find matches
            with open(self.secondaryJSON) as secondary_fin:
                secondary_dict = json.load(secondary_fin)

                for inputFile in self.inputFiles:
                    if inputFile not in primary_dict.keys():
                        continue
                    p_file = inputFile
                    primary_info = primary_dict[p_file]
                    if p_file not in self.inputFiles: continue
                    sibs = []
                    for s_file, secondary_info in secondary_dict.items():
                        if len(np.intersect1d(primary_info['runs'], secondary_info['runs'])) == 0: continue
                        if len(np.intersect1d(primary_info['lumis'], secondary_info['lumis'])) != 0:
                            sibs.append(s_file)
                    
                    siblings[p_file] = sibs
                    self.getEventList(p_file, sibs)

                    if self.eventsPerJob != -1 and len(self.sharedEvents) > self.eventsPerJob:
                        break                        

        np.savetxt('eventList_{0}.txt'.format(self.jobNumber), self.sharedEvents, fmt='%s', delimiter=',')
        print("There are {0} miniAOD events, and {1} AOD events, {2} shared events".format(self.numPrimaryEvents, self.numSiblingEvents, self.numSharedEvents))


        with open(jsonName, 'w') as fout:
            json.dump(siblings, fout)

        self.siblingDict = siblings

    def getEventList(self, primaryFile, siblings):

        primaryEvents = np.array([])
        if not primaryFile in self.inputFiles: 
            print("File is missing from input file list")
            return
        if not primaryFile.startswith("root://") and not self.local: 
            primaryFile = 'root://cms-xrd-global.cern.ch:/' + primaryFile
        events = r.getEventsInFile(primaryFile)
        tmpEvents = np.array([str(x.runNum)+':'+str(x.lumiBlock)+':'+str(x.event) for x in events])
        primaryEvents = np.concatenate((primaryEvents, tmpEvents))

        secondaryEvents = np.array([])
        for filename in siblings:
            if not filename.startswith("root://") and not self.local: 
                filename = 'root://cms-xrd-global.cern.ch:/' + filename
            events = r.getEventsInFile(filename)
            tmpEvents = np.array([str(x.runNum)+':'+str(x.lumiBlock)+':'+str(x.event) for x in events])
            secondaryEvents = np.concatenate((secondaryEvents, tmpEvents))
        
        this_sharedEvents = np.intersect1d(primaryEvents, secondaryEvents)
        self.sharedEvents = np.concatenate((self.sharedEvents, this_sharedEvents))
        self.numSharedEvents = len(self.sharedEvents)
        self.numPrimaryEvents += len(primaryEvents)
        self.numSiblingEvents += len(secondaryEvents)


    def getFilesFromList(self, jobNumber, nJobs):
        sys.path.append(os.getcwd())

        self.jobNumber = jobNumber
        self.nJobs = nJobs

        Label = self.label
        datasetInfo = importlib.import_module('datasetInfo_' + Label +'_cfg', package=None)

        #If no job number or number of jobs is passed use the full file list
        if jobNumber == -1 or nJobs == -1:
            self.inputFiles = datasetInfo.listOfFiles
            return

        filesPerJob = int (math.floor (len (datasetInfo.listOfFiles) / nJobs))
        residualLength = int(len(datasetInfo.listOfFiles)%nJobs)
        if jobNumber < residualLength:
            runList = datasetInfo.listOfFiles[(jobNumber * filesPerJob + jobNumber):(jobNumber * filesPerJob + filesPerJob + jobNumber + 1)]
        else:
            runList = datasetInfo.listOfFiles[(jobNumber * filesPerJob + residualLength):(jobNumber * filesPerJob + residualLength + filesPerJob)]
    
        #local files need to have the file prefix removed
        if runList[0].startswith('file:'):
            runList = [x.split('file:')[1] for x in runList]

        #print("This is the run list:\n",runList)
        
        self.inputFiles = runList


if __name__ == "__main__":

    #both files are real datasets -> global/global
    #input file is user (skim) sib is miniAOD -> phys03/global
    #input file is local user sib is miniAOD -> noAPI/global
    #both files are only local -> noAPI/noAPI

    parser = argparse.ArgumentParser()
    parser.add_argument("-L", "--inputDataset", type=str, help="Input dataset to get siblings of")
    #parser.add_argument("-f", "--inputFiles", type=str, help="Input file list to get siblings from")
    parser.add_argument("-j", "--jobNumber", type=int, help="Job number to run from the input file list", default=-1)
    parser.add_argument("-t", "--totalJobs", type=int, help="Total number of jobs condor is running over", default=-1)
    parser.add_argument("-m", "--eventsPerJob", type=int, help="Total number of events for jobs to run over (note this is a maximum but may return fewer if there are fewer events in filelist)", default=-1)
    #parser.add_argument("--siblingDataset", type=str, help="Sibling dataset to get sibling files from")
    parser.add_argument("-n", "--nameList", type=str, help="Name of the json file to output")
    parser.add_argument("-l", "--lumiMatching", action="store_true", help="Force lumi matched siblings instead of siblings listed in DAS")
    parser.add_argument("-s", "--siblingJSON", type=str, help="Name of sibling JSON file to use for matching", required=True)
    parser.add_argument("-i", "--inputJSON", type=str, help="Name of primary dataset JSON file to use for matching", required=True)
    #parser.add_argument("--eventList", type=str, default=None, help="Option to get event list, argument is input json file")
    parser.add_argument("-p", "--prod", type=str, default='global', help="Select DAS prod type or local, Options: \n"+
                                                                        "\tglobal: both datasets are global \n" + 
                                                                        "\tuser: input dataset is produced by user (phys03)\n" +
                                                                        "\tallUser: input and sibling datasets are produced by user (phys03)\n" +
                                                                        "\tlocal: input dataset is produced/stored locally on T3 (will do lumi matching)\n" +
                                                                        "\tallLocal: input and sibling dataset are produced/stored locally on T3")

    args = parser.parse_args()

    #default output json name
    output_json = 'output.json'
    if args.nameList: output_json = args.nameList + '.json'

    #if input and sibling jsons exist and will not be used, remove them
    if os.path.exists('input.json') and args.inputJSON != 'input.json': os.system('rm input.json')
    if os.path.exists('sibling.json') and args.siblingJSON != 'sibling.json': os.system('rm sibling.json')
    
    mysiblings = getSiblings(args.inputJSON, args.siblingJSON, args.inputDataset)

    if args.eventsPerJob:
        mysiblings.eventsPerJob = args.eventsPerJob

    mysiblings.getSiblings()
