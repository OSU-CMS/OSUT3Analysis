#!/usr/bin/env bash

if [ $# -ne 2 ]
then
  echo "Usage: $(basename $0) CONDOR_DIR CHANNEL_NAME"
  exit 1
fi
condorDir=$1
channel=$2

if [ ! -e $condorDir ]
then
  echo "\"$condorDir\" does not exist!"
  exit 1
fi
if [ ! -e "${condorDir}/${channel}" ]
then
  echo "\"$channel\" does not exist as a skim directory!"
  exit 1
fi

skimFilePrefix="skim"
ls ${condorDir}/${channel}/emptySkim_*.root > /dev/null 2>&1
if [ $? -eq 0 ]
then
  skimFilePrefix="emptySkim"
fi

logFiles=`ls ${condorDir}/condor_*.log | sort -h`
for logFile in ${logFiles}
do
  jobNumber=`echo $logFile | sed "s/^.*\/condor_\(.*\)\.log$/\1/g"`
  grep "return value 0" $logFile > /dev/null 2>&1
  isFailed=$?
  ls ${condorDir}/${channel}/${skimFilePrefix}_${jobNumber}.root > /dev/null 2>&1
  isMissing=$?
  if [ $isFailed -eq 0 ] && [ $isMissing -ne 0 ]
  then
    echo "MISSING: ${condorDir}/${channel}/${skimFilePrefix}_${jobNumber}.root"
  fi
done
