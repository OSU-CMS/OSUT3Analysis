import sys
import math
import os
import json
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
    secondaryRunList = datasetInfo.listOfSecondaryFiles
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

# Function for getting siblings of a given file from a given dataset
def getSiblings (fileName, dataset):
  try:
    from dbs.apis.dbsClient import DbsApi
    from CRABClient.ClientUtilities import DBSURLS
  except ImportError:
    print("getSiblings() relies on CRAB. Please set up the environment for CRAB before using.")
    sys.exit (1)

  dbsurl_global = DBSURLS["reader"].get ("global", "global")
  dbsurl_phys03 = DBSURLS["reader"].get ("phys03", "phys03")
  dbs3api_phys03 = DbsApi (url = dbsurl_phys03)
  dbs3api_global = DbsApi (url = dbsurl_global)

  # if there is an xrootd prefix, strip it
  if "/store/" in fileName:
    i = fileName.find ("/store/")
    fileName = fileName[i:]

  miniaod = set ([])
  miniaodSubset = set ([])
  # if dataset is not a USER dataset, then assume the file comes from a USER dataset
  if "/USER" not in dataset:
    # first get the parents
    parents = dbs3api_phys03.listFileParents (logical_file_name = fileName)

    # for each of the parents, get the grandparents
    grandparents = []
    for parent in parents:
      for parent_file_name in parent["parent_logical_file_name"]:
        grandparents.extend (dbs3api_global.listFileParents (logical_file_name = parent_file_name))

    # then for each of the grandparents, get their children
    children = []
    for grandparent in grandparents:
      for grandparent_file_name in grandparent["parent_logical_file_name"]:
        children.extend (dbs3api_global.listFileChildren (logical_file_name = grandparent_file_name))

    # put the children in a set
    for child in children:
      for child_file_name in child["child_logical_file_name"]:
        miniaod.add (child_file_name)

    # put the files of the target dataset in another set
    dataset = dbs3api_global.listFiles (dataset = dataset)
    for f in dataset:
      miniaodSubset.add (f["logical_file_name"])

  # if dataset is a USER dataset, then assume the file comes from a MINIAOD dataset
  else:
    # first get the parents
    parents = dbs3api_global.listFileParents (logical_file_name = fileName)

    # then for each of the grandparents, get their children
    children = []
    for parent in parents:
      for parent_file_name in parent["parent_logical_file_name"]:
        children.extend (dbs3api_global.listFileChildren (logical_file_name = parent_file_name))

    # then for each of the grandparents, get their children
    grandchildren = []
    for child in children:
      for child_file_name in child["child_logical_file_name"]:
        if "/AOD/" not in child_file_name:
          continue
        grandchildren.extend (dbs3api_phys03.listFileChildren (logical_file_name = child_file_name))

    # put the children in a set
    for grandchild in grandchildren:
      for grandchild_file_name in grandchild["child_logical_file_name"]:
        miniaod.add (grandchild_file_name)

    # put the files of the target dataset in another set
    dataset = dbs3api_phys03.listFiles (dataset = dataset)
    for f in dataset:
      miniaodSubset.add (f["logical_file_name"])

  # return the intersection of the two sets
  return list (miniaodSubset.intersection (miniaod))

# Function for getting Run3 skim sibling of a given file from a given dataset
def getRun3SkimSiblings (fileName, dataset, inputUser='global'):
  try:
    from dbs.apis.dbsClient import DbsApi
    #from CRABClient.ClientUtilities import DBSURLS
  except ImportError:
    print("getSiblings() relies on CRAB. Please set up the environment for CRAB before using.")
    sys.exit (1)

  #dbsurl_global = DBSURLS["reader"].get ("global", "global")
  #dbsurl_phys03 = DBSURLS["reader"].get ("phys03", "phys03")
  dbs3api_phys03 = DbsApi (url = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSReader')
  dbs3api_global = DbsApi (url = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader')

  # if there is an xrootd prefix, strip it
  if "/store/" in fileName:
    i = fileName.find ("/store/")
    fileName = fileName[i:]

  miniaod = set ([])
  miniaodSubset = set ([])
  # if dataset is not a USER dataset, then assume the file comes from a USER dataset
  if "/USER" not in dataset:
    # first get the parents
    if inputUser == 'allUser':
      #if dataset was created by user then the parents will be AOD files -> Need to get grandparents (RAW) to find MINIAOD siblings
      grandparents = []
      parents = dbs3api_phys03.listFileParents (logical_file_name = fileName)
      for parent in parents:
        for parent_file_name in parent['parent_logical_file_name']:
          grandparents.extend (dbs3api_phys03.listFileParents(logical_file_name = parent_file_name))
      parents = grandparents
    elif inputUser == 'user':
      print("Getting user dataset, not grandparents")
      parents = dbs3api_phys03.listFileParents (logical_file_name = fileName)
    else:
      print("Getting user dataset, not grandparents")
      parents = dbs3api_global.listFileParents (logical_file_name = fileName)

    children = []
    for parent in parents:
      for parent_file_name in parent["parent_logical_file_name"]:
        children.extend (dbs3api_global.listFileChildren (logical_file_name = parent_file_name))


     # put the children in a set
    for child in children:
      #for child_file_name in child["child_logical_file_name"]:
      miniaod.add (child["child_logical_file_name"])

    # put the files of the target dataset in another set
    dataset = dbs3api_global.listFiles (dataset = dataset)
    for f in dataset:
      miniaodSubset.add (f["logical_file_name"])

  # if dataset is a USER dataset, then assume the file comes from a MINIAOD dataset
  else:
    print("No USER dataset is expected in Run3 analysis. Please double check the input dataset !!!")
 
  # return the intersection of the two sets
  return list (miniaodSubset.intersection (miniaod))

def skimListExists(dataset):
  if os.path.exists(os.environ.get("CMSSW_BASE") + '/src/DisappTrks/Skims/data/' + dataset + '.json'):
    print("Skim list for {0} exists".format(dataset))
    return True
  else:
    print("Skim list does not exist for {0}".format(dataset))
    return False

def getSiblingList(sibList, runList, siblingDataset):
  
  print("Trying to get sibling list")
  siblings = []
  
  fin = open(sibList, 'r')
  data = json.load(fin)

  for filename in runList:
    if filename in data.keys():
      print("{0} in dictionary".format(filename))
      siblings.extend(data[filename])
    else:
      print("{0} not in dictionary, trying to get the siblings with function".format(filename))
      siblings.extend(getRun3SkimSiblings(filename, siblingDataset))

  return siblings

