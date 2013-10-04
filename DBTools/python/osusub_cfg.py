import sys
import math

if len (sys.argv) == 7 and sys.argv[2] == "True":
  nJobs = float (sys.argv[3])
  jobNumber = int (sys.argv[4])
  if int (sys.argv[3]) != 0 and sys.argv[5] != "NULL":
    import runList as runList

    filesPerJob = int (math.ceil (len (runList.runList) / nJobs))
    runList = runList.runList[(jobNumber * filesPerJob):(jobNumber * filesPerJob + filesPerJob)]
  dataset = sys.argv[5]
  datasetLabel = sys.argv[6]
  batchMode = True
else:
  batchMode = False
