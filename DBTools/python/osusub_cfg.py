import sys
import math

if len (sys.argv) == 7 and sys.argv[2] == "True":
  nJobs = float (sys.argv[3])
  Label = str(sys.argv[6])
  jobNumber = int (sys.argv[4])
  if int (sys.argv[3]) != 0 and sys.argv[5] != "NULL":
    exec("import datasetInfo_" + Label +"_cfg as datasetInfo")
    filesPerJob = int (math.floor (len (datasetInfo.listOfFiles) / nJobs))
    residualLength = int(len(datasetInfo.listOfFiles)%nJobs)
    if jobNumber < residualLength:  
        runList = datasetInfo.listOfFiles[(jobNumber * filesPerJob + jobNumber):(jobNumber * filesPerJob + filesPerJob + jobNumber + 1)]
    else:
	runList = datasetInfo.listOfFiles[(jobNumber * filesPerJob + residualLength):(jobNumber * filesPerJob + residualLength + filesPerJob)]
  dataset = sys.argv[5]
  datasetLabel = sys.argv[6]
  batchMode = True

if len (sys.argv) == 5 and sys.argv[2] == "True":
  nJobs = float (sys.argv[3])
  jobNumber = int (sys.argv[4])
  batchMode = True
else:
  batchMode = False
