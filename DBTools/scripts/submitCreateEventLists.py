import os
import argparse
import json
import shutil
from OSUT3Analysis.DBTools.getSiblings import *
import sys

def writeCondorSub(exe, nJobs, outDir, jsonFile, requirements, wrapper, cmssw):
    exe = exe.split('/')[-1]
    jsonFile = jsonFile.split('/')[-1]
    wrapper = wrapper.split('/')[-1]
    f = open('run.sub', 'w')
    uid = os.getuid()
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
    transfer_input_files = {3}/{1}, {3}/{2}, {3}/{7}
    transfer_output_files = ""
    getenv = true
    x509userproxy = /tmp/x509up_u{9}
    queue {0}
    """.format(nJobs,exe,jsonFile,outDir,requirements[0],requirements[1],requirements[2],wrapper, cmssw, uid)

    f.write(submitLines)
    f.close()    

def checkEmpty(filename):
    data = np.load(filename)
    return len(data[data.files[0]]) == 0

def checkFailures(jsonFile, logDir):
    resubmitJobs = []
    outputDir = jsonFile.split('/')[-1].split('-')[0]

    print("Checking for failures")
    print(jsonFile, logDir, outputDir)

    #open json file to check file names against job numbers
    f_json = open('/'.join([logDir, jsonFile]), 'r')
    a_json = json.load(f_json)
    files = [x.split('/')[-1] for x in list(a_json.keys())]

    #loop over output files to check for issues
    for filename in os.listdir(logDir):
        needsResubmit = False
        if not filename.endswith('.out'): continue

        #get run number
        runNum = filename.split('_')[-1].split('.')[0]

        #check if the output exists
        thisFileName = files[int(runNum)]
        eventListDir = '/data/users/mcarrigan/condor/EventLists/{}/{}.npz'.format(outputDir, thisFileName)
        if not os.path.exists(eventListDir):
            needsResubmit = True
            print("Output file {} does not exist".format(eventListDir), runNum)
        
        if checkEmpty(eventListDir):
            needsResubmit = True
            print("Output file {} is empty".format(eventListDir), runNum)


        #check if there are any file open failures
        with open('/'.join([logDir, filename]), 'r') as fin:
            for line in fin:
                if 'Failed to open file after 5 attempts' in line:
                    needsResubmit = True
                    print("File {} had trouble opening root files".format(filename), runNum)
                    break

        if needsResubmit:
            resubmitJobs.append(runNum)


    resubmitScript = logDir + '/resubmit.sub'
    shutil.copy(logDir + '/run.sub', resubmitScript)

    with open(resubmitScript, 'r') as file:
        lines = file.readlines()

    # Modify the last line
    for i, line in enumerate(lines):
        if line.strip().startswith('queue'):
            # Change the line to the new queue value
            lines[i] = '    queue Process in {}\n'.format(' '.join(map(str, resubmitJobs)))
            break

    # Write the modified lines back to the file
    with open(resubmitScript, 'w') as file:
        file.writelines(lines)

    cmd = 'condor_submit {}'.format(resubmitScript)
    os.system(cmd)
    print(cmd)

    print("Jobs that need reprocessing", resubmitJobs)



if __name__ == "__main__":


    parser = argparse.ArgumentParser()
    parser.add_argument("-j", "--json", type=str, help="Input json of dataset to get events from")
    parser.add_argument('-d', '--dataset', type=str, help="Dataset to get json from")
    parser.add_argument('-r', '--resubmit', action='store_true', help='Option to check for failed jobs and resubmit')
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
        if not args.resubmit:
            getSiblings.getDASInfo(args.dataset, jsonName = '{}.json'.format(jsonFile))
        jsonFile = '{}.json'.format(jsonFile)

    outputDir = '/abyss/users/mcarrigan/log/DisappTrks/EventLists_{}'.format(jsonFile.split('/')[-1].split('.')[0])

    if args.resubmit:
        checkFailures(jsonFile, outputDir)
        sys.exit(0)

    nJobs = 0
    with open(jsonFile) as fin:
        f_json = json.load(fin)
        nJobs = len(f_json.keys())

    if not os.path.exists(outputDir):
        os.mkdir(outputDir)

    writeCondorSub(script, nJobs, outputDir, jsonFile, requirements, wrapper, cmssw)

    if not args.resubmit:
        shutil.copy(script, outputDir)
        shutil.copy('run.sub', outputDir)
        shutil.copy(jsonFile, outputDir)
        shutil.copy('eventListWrapper.sh', outputDir)

    os.system('condor_submit run.sub')



