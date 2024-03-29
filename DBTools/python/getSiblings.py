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

    def __init__(self):

        self.numEvents = 0
        self.numSharedEvents = 0
        self.numPrimaryEvents = 0
        self.numSiblingEvents = 0

        #if all datasets are not local set APIs
        if args.prod != 'allLocal':
            self.setAPIs()

        #require input dataset
        if args.inputDataset:
            self.dataset_in = args.inputDataset
            #set the file lists
            if args.siblingJSON and not args.inputJSON:
                self.setFileLists(listType = 'input')
            if args.inputJSON and not args.siblingJSON:
                self.setFileLists(listType = 'sibling')
            if not args.siblingJSON and not args.inputJSON:
                self.setFileLists()
        elif args.inputFiles:
            if args.jobNumber==None or args.totalJobs==None:
                print("Need to specify the job number and total jobs")
                sys.exit(2)
            self.inputFileList = self.getFilesFromList(args.jobNumber, args.totalJobs)
        else:
            print("Need to specify the input dataset (-i) or input files (-f)")
            sys.exit(2)

        #require sibling dataset
        print(args.siblingDataset)
        if args.siblingDataset:
            self.dataset_sib = args.siblingDataset
            if not args.siblingJSON:self.setFileLists(listType = 'sibling')
        else:
            print("Need to specify the sibling dataset")
            sys.exit(2)

        #name of output json file
        if args.nameList:
            self.dictName = args.nameList
        elif args.inputDataset:
            self.dictName = self.dataset_in.split('/')[1] + '_' + self.dataset_in.split('/')[2].split('-')[0].replace('Run', '')
        else:
            self.dictName = 'default.json'

        if not self.dictName.endswith('.json'):
            self.dictName += '.json'

    #get list of files for datasets
    def setFileLists(self, listType=''):
        if args.prod == 'allLocal':
            if listType == 'input' or listType == '': 
                self.inputFileList = self.getLocalFileList(self.dataset_in)
            if listType == 'sibling' or listType == '': 
                self.siblingFileList = self.getLocalFileList(self.dataset_sib)
        elif args.prod == 'local':
            if listType == 'input' or listType == '': 
                self.inputFileList = self.getLocalFileList(self.dataset_in)
            if listType == 'sibling' or listType == '': 
                self.siblingFileList = self.getFileList(self.dataset_sib, self.dbsapi_out)
        else:
            if listType == 'input' or listType == '': 
                self.inputFileList = self.getFileList(self.dataset_in, self.dbsapi_in)
            if listType == 'sibling' or listType == '': 
                self.siblingFileList = self.getFileList(self.dataset_sib, self.dbsapi_out)

    #get the dbs api for a given dataset
    def setAPIs(self):
        try:
            from dbs.apis.dbsClient import DbsApi
        except ImportError:
            print("getSiblings() relies on CRAB. Please set up the environment for CRAB before using.")
            sys.exit (1)
        if args.prod == 'local':
            self.dbsapi_out = DbsApi (url = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader')
        elif args.prod == 'global':
            self.dbsapi_in = DbsApi (url = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader')
            self.dbsapi_out = DbsApi (url = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader')
        elif args.prod == 'user':
            self.dbsapi_in = DbsApi (url = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSReader')
            self.dbsapi_out = DbsApi (url = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader')
        elif args.prod == 'allUser':
            self.dbsapi_in = DbsApi (url = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSReader')
            self.dbsapi_out = DbsApi (url = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSReader')

    #main function for getting siblings using either lumi matching or DBS
    def getSiblings(self):
        if args.prod == 'local' or args.prod == 'allLocal' or args.lumiMatching:
            self.getLumiMatching(self.dictName, args.prod, args.inputJSON, args.siblingJSON)
        else:
            self.getDBSSiblings(self.dictName)

    #function to get siblings from DBS
    def getDBSSiblings(self, output_json):
        print("getting dbs siblings")

        file_dict = {}

        inputFiles = self.inputFileList

        #primaryEventsTotal = np.array([])
        #siblingEventsTotal = np.array([])
        sharedEvents = np.array([])

        for ifile, filename in enumerate(inputFiles):

            if args.eventsPerJob != -1 and self.numSharedEvents >= args.eventsPerJob: break

            if not filename.startswith('root://'): filename = 'root://cms-xrd-global.cern.ch:/' + filename
            primaryEvents = r.getEventsInFile(filename)
            primaryEvents = np.array([str(x.runNum)+':'+str(x.lumiBlock)+':'+str(x.event) for x in primaryEvents])

            print("Input File: {} with {} events".format(filename, len(primaryEvents)))

            siblings = getRun3SkimSiblings(filename, self.dataset_sib, args.prod)
            file_dict[filename] = siblings
            print('Siblings: ')
            siblingEvents = []
            for sib in siblings:
                if not sib.startswith("root://"):
                    sib  = 'root://cms-xrd-global.cern.ch:/' + sib
                print('\t', sib)
                if len(siblingEvents) == 0: 
                    siblingEvents = np.array([str(x.runNum)+':'+str(x.lumiBlock)+':'+str(x.event) for x in r.getEventsInFile(sib)])
                else: 
                    siblingEvents = np.concatenate((siblingEvents, np.array([str(x.runNum)+':'+str(x.lumiBlock)+':'+str(x.event) for x in r.getEventsInFile(sib)])))
            
            thisSharedEvents = np.intersect1d(primaryEvents, siblingEvents)
            sharedEvents = np.concatenate((sharedEvents, thisSharedEvents))
            self.numSharedEvents += len(thisSharedEvents)
            self.numPrimaryEvents += len(primaryEvents)
            self.numSiblingEvents += len(siblingEvents)

        if args.eventsPerJob != -1: sharedEvents = sharedEvents[:args.eventsPerJob]

        print("There are {0} miniAOD events, and {1} AOD events, {2} shared events".format(self.numPrimaryEvents, self.numSiblingEvents, self.numSharedEvents))
        
        json_dict = json.dumps(file_dict)
        f_out = open(output_json, 'w')
        f_out.write(json_dict)
        f_out.close()     
        np.savetxt('eventList_{0}.txt'.format(args.jobNumber), sharedEvents, fmt='%s', delimiter=',')

    
    #save a json dictionary to a given file
    def saveToJson(self, dict_out, saveFile):
        if os.path.exists(os.getcwd() + '/' + saveFile + '.json'):
            f_in = open(saveFile + '.json', 'r')
            json_dict = json.load(f_in)
            f_in.close()
        else:
            json_dict = {}
        combined_dict = dict(dict_out, **json_dict)
        json_dict = json.dumps(combined_dict)
        f_out = open(saveFile + '.json', 'w')
        f_out.write(json_dict)
        f_out.close()

    #script taken from CMSSW and made into function
    def getLumiBlocks(self, filelist, local=True, isInput=True, inputDict=None):
        list_ = []
        for ifile, filename in enumerate(filelist):
            if not local: filename = 'root://cmsxrootd.fnal.gov:/' + filename
            list_.append(filename)

        self.numEvents = 0
        events = 0

        runsLumisDict = {}
        for ifile, filename in enumerate(list_):
            # Don't need to look into all sibling files, so break whenever it has all lumis
            if not isInput and len(runsLumisDict) == len(inputDict): break
            # If number of events is high enough, break
            if isInput and args.eventsPerJob != -1 and self.numEvents >= args.eventsPerJob: break
            events = r.getEventsInFile(filename)
            lumis = list(set([x.lumiBlock for x in events]))
            # If lumis is not in the input dict, skip and don't add it to sibling dict
            if not isInput and lumis not in list(inputDict.values()): continue
            runsLumisDict[filename] = lumis
            if isInput: self.numEvents += len(np.array([str(x.runNum)+':'+str(x.lumiBlock)+':'+str(x.event) for x in events]))

        return runsLumisDict

    #function to get lumi blocks from a file list using DBS
    def getLumiBlocksDBS(self, filelist, api, saveFile):
        json_dict = {}
        if os.path.exists(os.getcwd() + '/' + saveFile + '.json'):
            f_in = open(saveFile + '.json', 'r')
            json_dict = json.load(f_in)
            f_in.close()
        
        runsLumisDict = {}
        for ifile, filename in enumerate(filelist):
            if ifile % 100 ==0: print("Working on {}".format(ifile))
            mylumis = api.listFileLumiArray(logical_file_name=filename)
            runsLumisDict[filename] = mylumis[0]['lumi_section_num']
        
        self.saveToJson(runsLumisDict, saveFile)

    #get list of files from DBS
    def getFileList(self, dataset, api, debug=False):
        files = api.listFiles (dataset = dataset, detail=True)
        names = [x['logical_file_name'] for x in files if x['is_file_valid']]
        if debug: return names[:10] #for debugging
        else: return names

    #get list of local files
    def getLocalFileList(self, dataDir):
        fileList = []
        for filename in os.listdir(dataDir):
            if not filename.endswith('root'): continue
            if not (dataDir + filename).startswith('file:'): fileList.append('file:' + dataDir + filename)
            else: fileList.append(dataDir + filename)
        return fileList

    #match files using lumi blocks
    def getLumiMatching(self, output_json, prod, inputJSON=None, sibJSON=None):
        if not inputJSON:
            if prod != 'local' and prod != 'allLocal': 
                self.getLumiBlocksDBS(self.inputFileList, self.dbsapi_in, 'input') #special case when forcing lumi matching on non local
            else: 
                print("getting lumi blocks for input files")
                inputLumiBlocks = self.getLumiBlocks(self.inputFileList)
        else:
            input_fin = open(inputJSON, 'r')
            inputLumiBlocks = json.load(input_fin)

        if not sibJSON:
            if prod != 'allLocal': 
                self.getLumiBlocksDBS(self.siblingFileList, self.dbsapi_out, 'sibling') #special case when dataset is not local
            else: 
                print("getting lumi blocks for sibling files")
                siblingLumiBlocks = self.getLumiBlocks(self.siblingFileList, isInput=False, inputDict=inputLumiBlocks)
        else:
            sib_fin = open(sibJSON, 'r')
            siblingLumiBlocks = json.load(sib_fin)

        dict_out = {}

        for inputFile in inputLumiBlocks.keys():
            print("Looking for lumis in {}".format(inputFile))
            lumis_in = np.array(inputLumiBlocks[inputFile])
            for sibFile in siblingLumiBlocks.keys():
                lumis_sib = np.array(siblingLumiBlocks[sibFile])
                if np.intersect1d(lumis_in, lumis_sib).size > 0: 
                    if inputFile in dict_out.keys():
                        dict_out[inputFile].append(sibFile)
                    else:
                        dict_out[inputFile] = [sibFile]
            if not inputFile in dict_out:
                print("File {0} has no matching run/lumi in the sibling dataset".format(inputFile))

        f_out = open(output_json, 'w')
        json_dict = json.dumps(dict_out)
        f_out.write(json_dict)
        f_out.close()

    def getFilesFromList(self, jobNumber, nJobs):
        sys.path.append(os.getcwd())

        Label = args.inputFiles
        datasetInfo = importlib.import_module('datasetInfo_' + Label +'_cfg', package=None)

        filesPerJob = int (math.floor (len (datasetInfo.listOfFiles) / nJobs))
        residualLength = int(len(datasetInfo.listOfFiles)%nJobs)
        if jobNumber < residualLength:
            runList = datasetInfo.listOfFiles[(jobNumber * filesPerJob + jobNumber):(jobNumber * filesPerJob + filesPerJob + jobNumber + 1)]
        else:
            runList = datasetInfo.listOfFiles[(jobNumber * filesPerJob + residualLength):(jobNumber * filesPerJob + residualLength + filesPerJob)]
    
        print("This is the run list:\n",runList)
        return runList



if __name__ == "__main__":

    #both files are real datasets -> global/global
    #input file is user (skim) sib is miniAOD -> phys03/global
    #input file is local user sib is miniAOD -> noAPI/global
    #both files are only local -> noAPI/noAPI

    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--inputDataset", type=str, help="Input dataset to get siblings of")
    parser.add_argument("-f", "--inputFiles", type=str, help="Input file list to get siblings from")
    parser.add_argument("-j", "--jobNumber", type=int, help="Job number to run from the input file list")
    parser.add_argument("-t", "--totalJobs", type=int, help="Total number of jobs condor is running over")
    parser.add_argument("-m", "--eventsPerJob", type=int, help="Total number of events for jobs to run over (note this is a maximum but may return fewer if there are fewer events in filelist)", default=-1)
    parser.add_argument("-s", "--siblingDataset", type=str, help="Sibling dataset to get sibling files from")
    parser.add_argument("-n", "--nameList", type=str, help="Name of the json file to output")
    parser.add_argument("-l", "--lumiMatching", action="store_true", help="Force lumi matched siblings instead of siblings listed in DAS")
    parser.add_argument("--siblingJSON", type=str, help="Name of sibling JSON file to use for matching")
    parser.add_argument("--inputJSON", type=str, help="Name of input JSON file to use for matching")
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
    
    mysiblings = getSiblings()
    mysiblings.getSiblings()
