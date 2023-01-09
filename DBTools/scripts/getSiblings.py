import os
import sys
import json
import argparse
from OSUT3Analysis.DBTools.osusub_cfg import getRun3SkimSiblings


try:
    from dbs.apis.dbsClient import DbsApi
    #from CRABClient.ClientUtilities import DBSURLS
except ImportError:
    print("getSiblings() relies on CRAB. Please set up the environment for CRAB before using.")
    sys.exit (1)

parser = argparse.ArgumentParser()
parser.add_argument("-i", "--inputDataset", type=str, help="Input dataset to get siblings of")
parser.add_argument("-s", "--siblingDataset", type=str, help="Sibling dataset to get sibling files from")
parser.add_argument("-u", "--user", action="store_true", help="Use this option to use a user created dataset")
parser.add_argument("-n", "--nameList", type=str, help="Name of the json file to output")

file_dict = {}
dataset_in = ''
dataset_sib = ''

args = parser.parse_args()

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

input_files = dbs3api_in.listFiles (dataset = dataset_in)

for ifile, filename in enumerate(input_files):
    print('Input File: ' + filename['logical_file_name'])
    this_file = filename['logical_file_name']
    siblings = getRun3SkimSiblings(this_file, dataset_sib, args.user)
    file_dict[this_file] = siblings
    print('Siblings: ')
    for sib in siblings:
        print('\t', sib)

json_dict = json.dumps(file_dict)
f_out = open(dictName + '.json', 'w')
f_out.write(json_dict)
f_out.close()
