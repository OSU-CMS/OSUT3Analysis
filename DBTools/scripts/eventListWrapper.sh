#!/usr/bin/bash

export CMSSW_VERSION_LOCAL=$1
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc7_amd64_gcc820
eval `scramv1 runtime -sh`

python3 createEventLists.py $2 $3