import os
import sys
import json
import argparse
import psutil
from OSUT3Analysis.DBTools.osusub_cfg import getRun3SkimSiblings
from FWCore.PythonUtilities.LumiList   import LumiList
from DataFormats.FWLite import Lumis, Handle
from multiprocessing import Process, Queue
import subprocess


def getDBSSiblings(output_json):

    file_dict = {}
    dataset_in = ''
    dataset_sib = ''

    dbs3api_in = DbsApi (url = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader')
    dbs3api_out = DbsApi (url = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader')

    if args.user:
        dbs3api_in = DbsApi (url = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSReader')
    if args.inputDataset:
        dataset_in = args.inputDataset
    else:
        print("Need to specify the input dataset")
        sys.exit(2)
    if args.siblingDataset:
        dataset_sib = args.siblingDataset
    else:
        print("Need to specify the sibling dataset")
        sys.exit(2)
    if args.nameList:
        dictName = args.nameList
    else:
        dictName = dataset_in.split('/')[1] + '_' + dataset_in.split('/')[2].split('-')[0].replace('Run', '')

    print("Output JSON file will be called {0}".format(dictName))

    input_files = dbs3api_in.listFiles (dataset = dataset_in, detail=True)
    print(input_files)

    for ifile, filename in enumerate(input_files):
        if(filename['is_file_valid']): print('Input File: ' + filename['logical_file_name'])
        this_file = filename['logical_file_name']
        siblings = getRun3SkimSiblings(this_file, dataset_sib, False)
        file_dict[this_file] = siblings
        print('Siblings: ')
        for sib in siblings:
            print('\t', sib)

    json_dict = json.dumps(file_dict)
    f_out = open(output_json, 'w')
    f_out.write(json_dict)
    f_out.close()

def getLocalSiblings(output_json):

    inputFileList = []
    siblingFileList = []

    for filename in os.listdir(args.inputDataset):
        if not filename.endswith('root'): continue
        inputFileList.append(args.inputDataset + filename)

    for filename in os.listdir(args.siblingDataset):
        if not filename.endswith('root'): continue
        siblingFileList.append(args.siblingDataset + filename)

    subList = []

    #this is run as a subprocess because the CMSSW Lumi class used does not close root files
    #this results in too many files open error / going over memory usage
    for ifile, filename in enumerate(inputFileList):
        subList.append(filename)
        if ifile % 100 == 0 and ifile > 0:
            os.system('cat /proc/sys/fs/file-nr')
            inputLumiBlocks = subprocess.run(['python3 getLumiBlock.py input '+ ' '.join(subList)], shell=True)
            subList = []
    inputLumiBlocks = subprocess.run(['python3 getLumiBlock.py input '+ ' '.join(subList)], shell=True)


    subList = []
    for ifile, filename in enumerate(siblingFileList):
        subList.append(filename)
        if ifile % 100 == 0 and ifile > 0:
            os.system('cat /proc/sys/fs/file-nr')
            inputLumiBlocks = subprocess.run(['python3 getLumiBlock.py sibling '+ ' '.join(subList)], shell=True)
            subList = []
    inputLumiBlocks = subprocess.run(['python3 getLumiBlock.py sibling '+ ' '.join(subList)], shell=True)


    input_fin = open('input.json', 'r')
    sib_fin = open('sibling.json', 'r')
    inputLumiBlocks = json.load(input_fin)

    siblingLumiBlocks = json.load(sib_fin)

    dict_out = {}

    for filename, [run, lumi] in inputLumiBlocks.items():
        for sibname, [sibRun, sibLumi] in siblingLumiBlocks.items():
            if run == sibRun and lumi == sibLumi:
                if filename in dict_out:
                    dict_out[filename] = dict_out[filename].append(sibname)
                else:
                    dict_out[filename] = [sibname]
        if not filename in dict_out:
            print("File {0} has no matching run/lumi in the sibling dataset".format(filename))

    f_out = open(output_json, 'w')
    json_dict = json.dumps(dict_out)
    f_out.write(json_dict)
    f_out.close()


if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--inputDataset", type=str, help="Input dataset to get siblings of")
    parser.add_argument("-s", "--siblingDataset", type=str, help="Sibling dataset to get sibling files from")
    parser.add_argument("-u", "--user", action="store_true", help="Use this option to use a user created dataset")
    parser.add_argument("-n", "--nameList", type=str, help="Name of the json file to output")
    parser.add_argument("-l", "--local", action="store_true", help="If input data is user created on the T3")

    args = parser.parse_args()

    output_json = 'output.json'
    if args.nameList: output_json = args.nameList + '.json'

    if not args.local:
        try:
            from dbs.apis.dbsClient import DbsApi
        except ImportError:
            print("getSiblings() relies on CRAB. Please set up the environment for CRAB before using.")
            sys.exit (1)
        getDBSSiblings(output_json)
    else:
        getLocalSiblings(output_json)
