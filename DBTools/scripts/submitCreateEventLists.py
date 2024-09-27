import os
import argparse
import json
import shutil
from OSUT3Analysis.DBTools.getSiblings import *
import sys

def writeCondorSub(exe, nJobs, outDir, jsonFile, requirements, wrapper, cmssw):
    f = open('run.sub', 'w')
    submitLines = """
    Universe = vanilla
    Rank = TARGET.IsLocalSlot
    request_disk = {6}
    request_memory = {5}
    request_cpus = {4}
    executable              = {7}
    arguments               = {8} {2} $(PROCESS)
    log                     = {3}/log_$(PROCESS).log
    output                  = {3}/out_$(PROCESS).out
    error                   = {3}/error_$(PROCESS).err
    should_transfer_files   = Yes
    when_to_transfer_output = ON_EXIT
    transfer_input_files = {1}, {2}, {7}
    transfer_output_files = ""
    getenv = true
    x509userproxy = /tmp/x509up_u1009
    queue {0}
    """.format(nJobs,exe,jsonFile,outDir,requirements[0],requirements[1],requirements[2],wrapper, cmssw)

    f.write(submitLines)
    f.close()    


if __name__ == "__main__":


    parser = argparse.ArgumentParser()
    parser.add_argument("-j", "--json", type=str, help="Input json of dataset to get events from")
    parser.add_argument('-d', '--dataset', type=str, help="Dataset to get json from")
    args = parser.parse_args()

    jsonFile = '/home/mcarrigan/scratch0/disTracksML/CMSSW_13_0_13/src/DisappTrks/BackgroundEstimation/test/debugMuonSkim/Muon_Run2022E-EXODisappTrk-27Jun2023-v1_AOD.json'
    wrapper = os.environ['CMSSW_BASE'] + '/src/OSUT3Analysis/DBTools/scripts/eventListWrapper.sh'
    script = os.environ['CMSSW_BASE'] + '/src/OSUT3Analysis/DBTools/python/createEventLists.py'
    requirements = ['2', '2000MB', '3000MB'] #CPU, Memory, Disk
    cmssw = os.environ['CMSSW_VERSION']

    if args.json:
        jsonFile = args.json

    if args.dataset:
        jsonFile = args.dataset.replace('/', '_')[1:]
        getSiblings.getDASInfo(args.dataset, jsonName = '{}.json'.format(jsonFile))
        jsonFile = '{}.json'.format(args.dataset)

    outputDir = '/abyss/users/mcarrigan/log/DisappTrks/EventLists_{}'.format(jsonFile.split('/')[-1].split('.')[0])

    nJobs = 0
    with open(jsonFile) as fin:
        f_json = json.load(fin)
        nJobs = len(f_json.keys())

    if not os.path.exists(outputDir):
        os.mkdir(outputDir)

    writeCondorSub(script, nJobs, outputDir, jsonFile, requirements, wrapper, cmssw)

    shutil.copy(script, outputDir)
    shutil.copy('run.sub', outputDir)
    shutil.copy(jsonFile, outputDir)

    os.system('condor_submit run.sub')
