from __future__ import print_function
from FWCore.PythonUtilities.LumiList   import LumiList
from DataFormats.FWLite import Lumis, Handle
import optparse
import json
import sys
import os

filenames = [
    '/data/users/borzari/condor/SignalMC/Run3/step3/1000cm/700GeV/AMSB_chargino_M_700GeV_CTau_1000cm_TuneCP5_PSweights_13p6TeV_madgraph5_pythia8/hist_641.root',
    '/data/users/borzari/condor/SignalMC/Run3/step3/1000cm/700GeV/AMSB_chargino_M_700GeV_CTau_1000cm_TuneCP5_PSweights_13p6TeV_madgraph5_pythia8/hist_642.root'
    ]

def getLumiBlocks(filelist, saveFile):

    json_dict = {}

    if os.path.exists(os.getcwd() + '/' + saveFile + '.json'):
        f_in = open(saveFile + '.json', 'r')
        json_dict = json.load(f_in)
        f_in.close()

    runLumisDict = {}
    lumis = Lumis (filelist)
    for i, (lumi, filename), in enumerate(zip(lumis, filelist)):

        '''if i%100 == 0:
            os.system('cat /proc/sys/fs/file-nr')'''

        print("Getting lumis for file {0}".format(filename))
        runLumisDict[filename] = [lumi.aux().run(), lumi.aux().id().luminosityBlock()]

    combined_dict = dict(runLumisDict, **json_dict)
    json_dict = json.dumps(combined_dict)
    f_out = open(saveFile + '.json', 'w')
    f_out.write(json_dict)
    f_out.close()

    del lumis


if __name__ == '__main__':

    if len(sys.argv) < 4:
        print("Need to give json file name and list of files to get lumi blocks from")
        sys.exit(1)

    getLumiBlocks(sys.argv[2:], sys.argv[1])
