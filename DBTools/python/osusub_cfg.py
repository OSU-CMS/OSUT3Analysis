import sys
import math
# For jobs with input datasets, normal cases: cmsRun config_cfg.py True 671 $(Process) /DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3/MINIAODSIM DYJetsToLL_50_MiniAOD
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

# For jobs without inputs such as MC generation: cmsRun config_cfg.py True 20 $(Process)  Stop200ToEMu_1000mm_GEN_745
elif len (sys.argv) == 5 and sys.argv[2] == "True":
  nJobs = float (sys.argv[3])
  jobNumber = int (sys.argv[4])
  batchMode = True
else:
  batchMode = False

