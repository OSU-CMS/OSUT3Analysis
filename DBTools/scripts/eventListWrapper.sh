#!/usr/bin/bash

(>&2 echo "Starting job on " `date`) # Date/time of start of job
(>&2 echo "Running on: `uname -a`") # Condor job is running on this node
(>&2 echo "System software: `cat /etc/redhat-release`") # Operating System on that node
(>&2 echo "Arguments passed to this script are: $@") #print all arguments

export CMSSW_VERSION_LOCAL=$1
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc7_amd64_gcc820
#eval `scramv1 runtime -sh`

python3 createEventLists.py $2 $3
