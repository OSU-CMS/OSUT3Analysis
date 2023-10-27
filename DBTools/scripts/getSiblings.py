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

r.gInterpreter.Declare(
    '''
    #include "getEvents.h"

    void dummyWrapper(){
        getEvents();
    }
    '''
)
r.gSystem.Load('./getEvents_cpp.so')

# class used to get the secondary files (siblings) of the primary input files
class getSiblings():

    def __init__(self):

        #require input dataset
        if args.inputDataset:
            self.dataset_in = args.inputDataset
        else:
            print("Need to specify the input dataset")
            sys.exit(2)

        #require sibling dataset
        if args.siblingDataset:
            self.dataset_sib = args.siblingDataset
        else:
            print("Need to specify the sibling dataset")
            sys.exit(2)

        #name of output json file
        if args.nameList:
            self.dictName = args.nameList
        else:
            self.dictName = self.dataset_in.split('/')[1] + '_' + self.dataset_in.split('/')[2].split('-')[0].replace('Run', '')

        if not self.dictName.endswith('.json'):
            self.dictName += '.json'

        #if all datasets are not local set APIs
        if args.prod != 'allLocal':
            self.setAPIs()
        
        #set the file lists
        if args.siblingJSON and not args.inputJSON:
            self.setFileLists(listType = 'input')
        if args.inputJSON and not args.siblingJSON:
            self.setFileLists(listType = 'sibling')
        if not args.siblingJSON and not args.inputJSON:
            self.setFileLists()

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

        print("Output JSON file will be called {0}".format(self.dictName))

        print('Input dataset: {}'.format(self.dataset_in))

        for ifile, filename in enumerate(self.inputFileList):
            if not filename == '/store/data/Run2022D/Muon/MINIAOD/27Jun2023-v2/80000/db2c623b-346f-4c2e-b061-c8b8728df070.root': continue #debugging
            #if not filename =='/store/data/Run2022D/Muon/AOD/EXODisappTrk-27Jun2023-v2/80000/97236e0a-68e6-4b34-8a24-53f2a101f34d.root': continue
            primaryEvents = r.getEventsInFile(filename)
            primaryEvents = np.array([str(x.runNum)+':'+str(x.lumiBlock)+':'+str(x.event) for x in primaryEvents])
            print(primaryEvents[:10])
            if args.prod == 'allUser':
                siblings = getRun3SkimSiblings(filename, self.dataset_sib, args.prod)
            else:
                siblings = getRun3SkimSiblings(filename, self.dataset_sib, args.prod)
            file_dict[filename] = siblings
            print('Siblings: ')
            siblingEvents = []
            for sib in siblings:
                print('\t', sib)
                if len(siblingEvents) == 0: 
                    siblingEvents = np.array([str(x.runNum)+':'+str(x.lumiBlock)+':'+str(x.event) for x in r.getEventsInFile(sib)])
                else: 
                    siblingEvents = np.concatenate((siblingEvents, np.array([str(x.runNum)+':'+str(x.lumiBlock)+':'+str(x.event) for x in r.getEventsInFile(sib)])))

        json_dict = json.dumps(file_dict)
        f_out = open(output_json, 'w')
        f_out.write(json_dict)
        f_out.close()
        print(siblingEvents[:10])
        sharedEvents = np.intersect1d(primaryEvents, siblingEvents)
        print("There are {0} miniAOD events, and {1} AOD events, {2} shared events".format(len(primaryEvents), len(siblingEvents), len(sharedEvents)))
        
        print("type: ", type(sharedEvents), type(sharedEvents[0]))
        np.savetxt('testEvents.txt', sharedEvents, fmt='%s', delimiter=',')
        #fout = open('testEvents.txt', 'w')
        #fout.write(str(sharedEvents))
        #fout.close()

    
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
    def getLumiBlocks(self, filelist, saveFile, local=True):
        list_ = []
        for ifile, filename in enumerate(filelist):
            if not local: filename = 'root://cmsxrootd.fnal.gov:/' + filename
            list_.append(filename)

        handle, lable = None, None

        json_dict = {}

        if os.path.exists(os.getcwd() + '/' + saveFile + '.json'):
            f_in = open(saveFile + '.json', 'r')
            json_dict = json.load(f_in)
            f_in.close()

        runsLumisDict = {}
        for ifile, filename in enumerate(list_):
            if ifile % 10==0:
                self.saveToJson(runsLumisDict, saveFile)
                runsLumisDict.clear()
            lumis = Lumis (filename)
            delivered = recorded = 0
            for i, lum in enumerate(lumis):
                runList = runsLumisDict.setdefault (filename, [])
                runList.append( lum.aux().id().luminosityBlock() )

        self.saveToJson(runsLumisDict, saveFile)

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
            fileList.append(dataDir + filename)
        return fileList

    #match files using lumi blocks
    def getLumiMatching(self, output_json, prod, inputJSON=None, sibJSON=None):
        if not inputJSON:
            if prod != 'local' and prod != 'allLocal': 
                self.getLumiBlocksDBS(self.inputFileList, self.dbsapi_in, 'input') #special case when forcing lumi matching on non local
            else: 
                print("getting lumi blocks for input files")
                self.getLumiBlocks(self.inputFileList, 'input')
            input_fin = open('input.json', 'r')
        else:
            input_fin = open(inputJSON, 'r')

        if not sibJSON:
            if prod != 'allLocal': 
                self.getLumiBlocksDBS(self.siblingFileList, self.dbsapi_out, 'sibling') #special case when dataset is not local
            else: 
                self.getLumiBlocks(self.siblingFileList, 'sibling')
            sib_fin = open('sibling.json', 'r')
        else:
            sib_fin = open(sibJSON, 'r')

        inputLumiBlocks = json.load(input_fin)
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


if __name__ == "__main__":

    #both files are real datasets -> global/global
    #input file is user (skim) sib is miniAOD -> phys03/global
    #input file is local user sib is miniAOD -> noAPI/global
    #both files are only local -> noAPI/noAPI

    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--inputDataset", type=str, help="Input dataset to get siblings of")
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
