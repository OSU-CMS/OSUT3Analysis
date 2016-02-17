#!/usr/bin/env python
import sys
import getpass
import os
import re
import socket
import time
import copy
import glob
from math import *
from array import *
from optparse import OptionParser
from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *
from OSUT3Analysis.Configuration.formattingUtilities import *
from OSUT3Analysis.DBTools.condorSubArgumentsSet import *

parser = OptionParser()
parser = set_commandline_arguments(parser)

parser.remove_option("-o")
parser.remove_option("-n")
parser.remove_option("-u")
parser.remove_option("-e")
parser.remove_option("-r")
parser.remove_option("-R")
parser.remove_option("-d")
parser.remove_option("-b")
parser.remove_option("--2D")
parser.remove_option("-y")
parser.remove_option("-p")

parser.add_option("-r", "--randomSeed", action="store_true", dest="Random", default=False, help="Assign random seeds for each job.")
parser.add_option("-f", "--fileName", dest="FileName", default = 'process.TFileService.fileName', help="Set the parameter of output filename in config file.")
parser.add_option("-m", "--maxEvents", dest="MaxEvents", default = -1, help="Set the maximum number of events to run over (sum of all jobs).")
parser.add_option("-p", "--process", dest="Process", default = '', help="Set the suffix for the process name.")
parser.add_option("-t", "--typeOfSource", dest="FileType", default = 'OSUT3Ntuple', help="Specify the type of input files.  Options:  OSUT3Ntuple, UserDir, UserList, Dataset")
parser.add_option("-d", "--dataset", dest="Dataset", default = "", help="Specify which dataset to run.")  # Dataset is also the name of the output directory in the working directory if FileType == 'OSUT3Ntuple'.  
parser.add_option("-c", "--configuration", dest="Config", default = "", help="Specify the configuration file to run.")
parser.add_option("-n", "--numberOfJobs", dest="NumberOfJobs", default = -1, help="Specify how many jobs to submit.")
parser.add_option("-j", "--numberOfFilesPerJob", dest="NumberOfFilesPerJob", default = -1, help="Specify how many input files per job to submit. Overrides --numberOfJobs argument.")
parser.add_option("-u", "--userCondorSubFile", dest="CondorSubFilr", default = "", help="Specify the condor.sub file you want to use if you have to add arguments.")
parser.add_option("-U", "--uniqueEventId", action="store_true", dest="Unique", default=False, help="Assign unique and continuos event IDs")
parser.add_option("-N", "--noExec", action="store_true", dest="NotToExecute", default = False, help="Just generate necessary config files without executing them.")
parser.add_option("-L", "--Label", dest="Label", default = "", help="Give the dataset a short label.")
parser.add_option("-s", "--SkimDirectory", dest="SkimDirectory", default = "", help="Specicy the location of the skim.")
parser.add_option("-a", "--SkimChannel", dest="SkimChannel", default = "", help="Determine the skim channel to run over.")
parser.add_option("-R", "--Requirements", dest="Requirements", default = "", help="Requirements to be added to condor.sub submssion script, e.g. 'Memory > 1900'.")  
parser.add_option("-x", "--crossSection", dest="crossSection", default = "", help="Provide cross section to the given dataset.")  
parser.add_option("-A", "--UseAAA", dest="UseAAA", action="store_true", default = False, help="Use AAA.")  
parser.add_option("-J", "--JSONType", dest="JSONType", default = "", help="Determine which kind of JSON file to use. Generic, MuonPhysics, CaloOnly, Silver, etc")  
parser.add_option("-g", "--Generic", dest="Generic", action="store_true", default = False, help="Use generic python config. Choose this option for non-OSUT3Analysis CMSSW jobs.")  
parser.add_option("--resubmit", dest="Resubmit", action="store_true", default = False, help="Resubmit failed condor jobs.")  
parser.add_option("--redirector", dest="Redirector", default = "", help="Setup the redirector for xrootd service to use")  

(arguments, args) = parser.parse_args()

#Examples:
#1. Submit generic jobs(-g). "Generic" means jobs that are not using OSUT3Analysis.cc as the analyzer. But it can still use whatever in the configurationOptions.py. This ELOG explains why we need 'g': https://cmshead.mps.ohio-state.edu:8080/OSUT3Analysis/13
#   1.1 Generate GEN-SIM step MC samples.
#       It is the only case where the user does not need inputfiles. You should use a command like this:
#       osusub.py -c Config.py -n 100 -m 10000 -U -f process.FEVTDEBUGoutput.fileName -R "Memory > 1900" -r -w WorkingDirectory -g -L Label 
#       Notice that the argument following -f should be the one you have in your Config.py. -L will give your job a short label for the beauty of simplicity. 
#   1.2 Run generic cmssw jobs with some inputs.  
#       This includes running DIGI, HLT, RECO steps in MC generation or any standalone analyzers.
#       There are several categories:
#           1. Most general way: run over a list of files.
#               Any input files can be included in this list. It can be even a mixture of files from different sources. Example:
#               osusub.py -c Config.py -n 100 -m 10000 -f process.FEVTDEBUGHLToutput.fileName -R "Memory > 1900" -w WorkingDirectory -g -L Label -g -d ListOfFiles -t UserList -A(if you are using files via xrootd)
#           The rest of the special cases can be done automatically.
#           2. Run over files in a given directory.
#               osusub.py -c Config.py -n 100 -m 10000 -f process.FEVTDEBUGHLToutput.fileName -R "Memory > 1900" -w WorkingDirectory -g -L Label -g -d inputDirectory -t UserDir
#           3. Run over a dataset(s) on T3. 
#               For a single dataset:
#               osusub.py -d dataset(name shown in configurationOptions.py) -c Config.py -m -1 -n 100 -w WorkingDirectory -g
#               Notice this dataset does not have to be available on T3. As long as it is added into the configurationOptions.py, osusub.py will automatically search for it via das_client.py.      
#               For a list of datasets:
#               You will need the localConfig.py. Example:
#               osusub.py -l localConfig.py -w WorkingDirctory -c Config.py -R "Memory > 1900" -g
#           4. Run over a dataset which has not been registered in configurationOptions.py.
#               osusub.py -d /A/B/C(exact name shown on DAS) -c Config.py -A -m -1 -n 100 -w WorkingDirectory -g
#               In this case the lable will be A with all the '-' in 'A' changed to '_'. 
#           5. Run over a skim.
#               osusub.py -l localConfig.py -w WorkingDirctory -c Config.py -R "Memory > 1900" -g -s SkimDirectory -a SkimChannel 
#   Notice: Maybe in some generic jobs you want to also produce skimmed ntuples and histograms in the same time. For this case, osusub.py can not handle yet. But is is fairly straight forward. Just make --fileName a list of output modules(--fileNames).                
#2 Submit OSUT3Analysis jobs. Well, just remove the -g option.
#           2.1 Run over a dataset(s) on T3. 
#               For a single dataset:
#               osusub.py -d dataset(name shown in configurationOptions.py) -c Config.py -n 100 -m -1 -w WorkingDirectory 
#               Notice this dataset does not have to be available on T3. As long as it is added into the configurationOptions.py, osusub.py will automatically search for it via das_client.py.      
#               For a list of datasets:
#               You will need the localConfig.py. Example:
#               osusub.py -l localConfig.py -w WorkingDirctory -c Config.py -R "Memory > 1900" 
#           2.2 Run over files in a given directory.
#               osusub.py -c Config.py -n 100 -m 10000 -f process.FEVTDEBUGHLToutput.fileName -R "Memory > 1900" -w WorkingDirectory -g -L Label -d inputDirectory -t UserDir
#           2.3 Run over a dataset which has not been registered in configurationOptions.py.
#               osusub.py -d /A/B/C(exact name shown on DAS) -c Config.py -A -m -1 -n 100 -w WorkingDirectory 
#               In this case the lable will be A with all the '-' in 'A' changed to '_'. 
#           2.4 Run over a skim.
#               osusub.py -l localConfig.py -w WorkingDirctory -c Config.py -R "Memory > 1900" -s SkimDirectory -a SkimChannel
#3 Resubmit failed condor jobs. 
#    After merging the output files, mergeOut.py will generate a condor_resubmit.sub for each dataset if it detects non 0 exit code. Simple add --resubmit to the original osusub.py command and it will automatically resubmit the failed jobs. 
#
#General advice to users:
#    If you have problems in using the osusub.py. Try the most general case -t UserList. 
#    Actually I vote for removing UserDir.....

#Define the dictionary to look for the redirectors given the users input. 
RedirectorDic = {'Infn':'xrootd.ba.infn.it','FNAL':'cmsxrootd.fnal.gov','Global':'cms-xrd-global.cern.ch'}

#To get the JSON file the user specifies. Use -J 'TypeOfJSON' like -J Silver
def getLatestJsonFile():
    if arguments.JSONType[-4:] == ".txt":
        startIndex = 0
        endIndex = 0
        ultimateJson = ''
        for i in range(0,len(arguments.JSONType)):
            if arguments.JSONType[-(i+1)] == '/':
                startIndex = len(arguments.JSONType)-i
                break
        for i in range(0,len(arguments.JSONType)):
            if arguments.JSONType[i:i+3] == 'txt':
                endIndex = i+3
        if startIndex <= endIndex:
            ultimateJson = arguments.JSONType[startIndex: endIndex]
        if arguments.JSONType[:4] == 'http':
            os.system('wget ' + arguments.JSONType + ' -O ' + ultimateJson)
        else:
            os.system('cp ' + arguments.JSONType + ' ' + ultimateJson)            
        return ultimateJson

    else:
        os.system('wget https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/ -O jsonList.txt')
        os.system('grep "Cert" jsonList.txt > CertList.txt')
        Tmp = open('CertList.txt','r+w')
        jsonFileList = []
        for line in Tmp:
            startIndex = 0
            endIndex = 0
            for i in range(0,len(line)):
                if line[i:i + 5] == '"Cert':
                    startIndex = i + 1
                if line[i:i+4] == 'txt"':
                    endIndex = i + 3
            if startIndex <= endIndex:
                jsonFileList.append(line[startIndex: endIndex])
        jsonFileFiltered = []
        for fileName in jsonFileList:
            nameSplit = fileName.split('_')
            for i in range(0, len(nameSplit)):
                if arguments.JSONType == "" and nameSplit[i] == 'Collisions15' and nameSplit[i + 1] == '25ns' and nameSplit[i + 2] == 'JSON.txt':
                    jsonFileFiltered.append(fileName)
                elif nameSplit[i] == 'Collisions15' and nameSplit[i + 1] == '25ns' and nameSplit[i + 2] == 'JSON':
                    if nameSplit[i + 3].split('.')[0] == arguments.JSONType:
                        jsonFileFiltered.append(fileName)
        bestJsons = []
        bestJson = ''
        runRange = 0
        if len(jsonFileFiltered) == 0:
            print "#######################################################"
            print "Warning!!!!!!!!!!!Could not find wanted JSON file"
            print "#######################################################"
        for json in jsonFileFiltered:
            nameSplit = json.split('_')
            if len(nameSplit[1].split('-')) > 1:
                if float(nameSplit[1].split('-')[1]) - float(nameSplit[1].split('-')[0]) > runRange:
                    runRange = float(nameSplit[1].split('-')[1]) - float(nameSplit[1].split('-')[0])
                    bestJson = json
        for json in jsonFileFiltered:
            nameSplit = json.split('_')
            if len(nameSplit[1].split('-')) > 1:
                if float(nameSplit[1].split('-')[1]) - float(nameSplit[1].split('-')[0]) == runRange:
                    bestJsons.append(json)
        versionNumber = 0
        ultimateJson = ''
        #if len(bestJsons) == 1:
        ultimateJson = bestJsons[0]
        #else:
            #for bestJson in bestJsons:
                #nameSplit = bestJson.split('_')
                #if nameSplit[len(nameSplit) - 1][0] == 'v':
                    #versionString =  nameSplit[len(nameSplit) - 1].split('.')[0]
                    #currentVersionNumber = float(versionString[1 : ])
                    #if currentVersionNumber > versionNumber:
                        #versionNumber = currentVersionNumber
                       #ultimateJson = bestJson
        os.system('rm CertList.txt')
        os.system('rm jsonList.txt')
        os.system('wget https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/' + ultimateJson + ' -O ' + ultimateJson)
        return ultimateJson


#A function to deal with special characters. One can choose to split or replace the special strings. For example, if you have '/' like this: /A/B/C, it will return A if you add '/' into specialStringSplitList. If you have[['-','_'] like A-B, it will return A_B if you add ['-','_'] into specialStringReplaceList. This function is added to deal with special characters that may confuse this script.  
def SpecialStringModifier(inputString, specialStringSplitList, specialStringReplaceList):
    if len(specialStringSplitList):
        for member in specialStringSplitList:
            if member in inputString:
                if inputString.split(member)[0] == '':
                    inputString = inputString.split(member)[1]
                else:
                    inputString = inputString.split(member)[0]            
    if len(specialStringReplaceList):
        for member in specialStringReplaceList:
            if member[0] in inputString: 
                tmp = inputString.replace(member[0],member[1])   
                inputString = tmp 
    return inputString

def GetCommandLineString():
    # Return string of all arguments specified on the command line
    commandLine = ""  
    for arg in sys.argv:
        if len(arg.split(" ")) > 1:
            commandLine = commandLine + " \"" + arg + "\""  # add quotation marks around a multiple-word argument  
        else:
            commandLine = commandLine + " " + arg
    return commandLine  

def GetListOfRootFiles(Directory):
    fileList = os.popen('ls ' + Directory + '/*.root').read().split('\n')  # remove '\n' from each word  
    for f in fileList:
        if len(f) is 0: 
            fileList.remove(f)   # remove empty filename
    return fileList  
 

#It generates the condor.sub file for each dataset. 
def MakeCondorSubmitScript(Dataset,NumberOfJobs,Directory,Label, UseAAA, jsonFile):
    os.system('touch ' + Directory + '/condor.sub')
    SubmitFile = open(Directory + '/condor.sub','r+w')
    cmsRunExecutable = os.popen('which cmsRun').read()
    SubmitFile.write("# Command line arguments: \n# " + GetCommandLineString() + " \n\n\n")  
    for argument in sorted(currentCondorSubArgumentsSet):
        if currentCondorSubArgumentsSet[argument].has_key('Executable') and currentCondorSubArgumentsSet[argument]['Executable'] == "":
            SubmitFile.write('Executable = ' + cmsRunExecutable + '\n')
        elif currentCondorSubArgumentsSet[argument].has_key('Arguments') and currentCondorSubArgumentsSet[argument]['Arguments'] == "":
            SubmitFile.write('Arguments = config_cfg.py True ' + str(NumberOfJobs) + ' $(Process) ' + Dataset + ' ' + Label + '\n\n')
        elif currentCondorSubArgumentsSet[argument].has_key('Transfer_Input_files') and currentCondorSubArgumentsSet[argument]['Transfer_Input_files'] == "":   
            if Dataset == '':
                SubmitFile.write('Transfer_Input_files = config_cfg.py,userConfig_' + Label + '_cfg.py\n')
    	    elif UseAAA:
	        userName = getpass.getuser()
                userId = os.popen('id -u ' + userName).read()
                userProxy = '/tmp/x509up_u' + str(userId)
                SubmitFile.write('x509userproxy = ' + userProxy + '\n')
                SubmitFile.write('should_transfer_files   = YES\n')
                if jsonFile == '':
                    SubmitFile.write('Transfer_Input_files = config_cfg.py,userConfig_' + Label +'_cfg.py,datasetInfo_' + Label + '_cfg.py,' + userProxy + '\n')
                else:    
                    SubmitFile.write('Transfer_Input_files = config_cfg.py,userConfig_' + Label +'_cfg.py,datasetInfo_' + Label + '_cfg.py,' + userProxy.strip('\n') + ',' + str(jsonFile) + '\n')
            else:
                SubmitFile.write('Transfer_Input_files = config_cfg.py,userConfig_' + Label +'_cfg.py,datasetInfo_' + Label + '_cfg.py\n')
        elif currentCondorSubArgumentsSet[argument].has_key('Requirements') and arguments.Requirements:
            SubmitFile.write('Requirements = ' + arguments.Requirements + '\n')
        elif currentCondorSubArgumentsSet[argument].has_key('Queue'):
            SubmitFile.write('Queue ' + str(NumberOfJobs) +'\n')
        else:
            SubmitFile.write(currentCondorSubArgumentsSet[argument].keys()[0] + ' = ' + currentCondorSubArgumentsSet[argument].values()[0] + '\n')
    SubmitFile.close()

#It generates the config_cfg.py file for condor.sub to use. In this file it assign unique filenames to the outputs of all the jobs, both histogram outputs and skimmed ntuples.  
def MakeSpecificConfig(Dataset, Directory, Label, SkimChannelNames,jsonFile):
    os.system('touch ' + Directory + '/config_cfg.py')
    ConfigFile = open(Directory + '/config_cfg.py','r+w')
    sys.path.append(Directory)
    exec('import userConfig_' + Label + '_cfg' + ' as temPset')
    ConfigFile.write('import FWCore.ParameterSet.Config as cms\n')        
    ConfigFile.write('import OSUT3Analysis.DBTools.osusub_cfg as osusub\n')
    randomNumberSuffix = int (time.time ())  # Use the seconds since the Unix epoch to get a
                                             # different process name when running over a skim with the
                                             # same channel.
    ConfigFile.write('osusub.randomNumberSuffix = ' + str (randomNumberSuffix) + '\n')
    ConfigFile.write('import re\n')
    ConfigFile.write('import userConfig_' + Label + '_cfg as pset\n')
    if jsonFile != '':
        ConfigFile.write('import FWCore.PythonUtilities.LumiList as LumiList\n')
        ConfigFile.write('myLumis = LumiList.LumiList(filename = \'' + str(jsonFile) + '\').getCMSSWString().split(\',\')\n')
    ConfigFile.write('\n')
    if not Generic:
        if len(SkimChannelNames) == 0:
            SkimChannelNames = SkimChannelFinder('userConfig_' + Label + '_cfg', Directory) 
            for channelName in SkimChannelNames:
                if not channelName == '':
                    if not os.path.exists(Directory + '/' + channelName):
                        os.system('mkdir ' + Directory + '/' + channelName )
                    StringToAdd = 'pset.process.' + channelName + 'PoolOutputModule.fileName = cms.untracked.string(\'' + Directory + '/' + channelName +'/skim_\'' +'+ str (osusub.jobNumber)' + '+ \'.root\')\n'
                    ConfigFile.write(StringToAdd)
        else:
            for channelName in SkimChannelNames:
                if not channelName == '':
                    if not os.path.exists(Directory + '/' + channelName):
                        os.system('mkdir ' + Directory + '/' + channelName )
                    StringToAdd = 'pset.process.' + channelName + 'PoolOutputModule.fileName = cms.untracked.string(\'' + Directory + '/' + channelName +'/skim_\'' +'+ str (osusub.jobNumber)' + '+ \'.root\')\n'
                    ConfigFile.write(StringToAdd)
    ConfigFile.write('fileName = pset.' + arguments.FileName + '\n')
    ConfigFile.write('fileName = fileName.pythonValue ()\n')
    ConfigFile.write('fileName = fileName[1:(len (fileName) - 1)]\n')
    ConfigFile.write('fileName = re.sub (r\'^(.*)\.([^\.]*)$\', r\'\\1_\' + str (osusub.jobNumber) + r\'.\\2\', fileName)\n')
    ConfigFile.write('pset.' + arguments.FileName + ' = fileName\n')
    if not arguments.Generic:
      if types[Label] == "data":
        for module in vars(temPset.process).values():
            if hasattr(module, "weights"):
                ConfigFile.write('pset.process.' + str(module) + '.weights = cms.VPSet()\n')
      if hasattr(temPset.process, "DisplacedSUSYEventVariableProducer"):
        if types[Label] == "bgMC":
            ConfigFile.write('pset.process.DisplacedSUSYEventVariableProducer.type = cms.string("bgMC")\n')
        elif types[Label] == "signalMC":
            ConfigFile.write('pset.process.DisplacedSUSYEventVariableProducer.type = cms.string("signalMC")\n')
        else:
            ConfigFile.write('pset.process.DisplacedSUSYEventVariableProducer.type = cms.string("data")\n')
      if hasattr(temPset.process, "PUScalingFactorProducer"):
        if types[Label] == "bgMC":
            ConfigFile.write('pset.process.PUScalingFactorProducer.dataset = cms.string("' +  Label + '")\n')
            ConfigFile.write('pset.process.PUScalingFactorProducer.type = cms.string("bgMC")')
        elif types[Label] == "signalMC":
            ConfigFile.write('pset.process.PUScalingFactorProducer.dataset = cms.string("' +  Label + '")\n')
            ConfigFile.write('pset.process.PUScalingFactorProducer.type = cms.string("signalMC")')
        else:
            ConfigFile.write('pset.process.PUScalingFactorProducer.dataset = cms.string("MuonEG_2015D")\n')
            ConfigFile.write('pset.process.PUScalingFactorProducer.type = cms.string("data")')
    ConfigFile.write('\n')
    if Dataset != '':
	ConfigFile.write('pset.process.source.fileNames = cms.untracked.vstring (osusub.runList)\n')
        #If there are input datasets, on could set the MaxEvents to be -1.
        if EventsPerJob < 0:
            ConfigFile.write('pset.process.maxEvents.input = cms.untracked.int32 (' + str(EventsPerJob) + ')\n')  
    #If there are no input datasets, one needs a positive MaxEvents.
    if jsonFile != '':
        ConfigFile.write('pset.process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange()\n')
        ConfigFile.write('pset.process.source.lumisToProcess.extend(myLumis)\n')
    if EventsPerJob > 0:
        ConfigFile.write('pset.process.maxEvents.input = cms.untracked.int32 (' + str(EventsPerJob) + ')\n')
    ConfigFile.write('process = pset.process\n')
    if arguments.Process:
	ConfigFile.write('process.setName_ (process.name_ () + \'' + arguments.Process + '\')\n')
    #else:
#	ConfigFile.write('process.setName_ (process.name_ () + \'' + str(arguments.condorDir).replace('_','')  + '\')\n') 
    if arguments.Random:
        ConfigFile.write('pset.process.RandomNumberGeneratorService.generator.initialSeed = osusub.jobNumber\n')
    if arguments.Unique:
        # Specify a different lumi section for each job so that all events have unique run / lumi / event numbers.  
        # Instead of changing lumi section, could also change run number.  
        ConfigFile.write('pset.process.source.firstLuminosityBlock = cms.untracked.uint32((osusub.jobNumber)+1) \n') # osusub.jobNumber starts with 0  
    ConfigFile.close()
    return SkimChannelNames

#This is a generic function to get the dataset information via das_client.py. 
def AcquireAwesomeAAA(Dataset, datasetInfoName, AAAFileList, datasetRead, crossSection):
    os.system('das_client.py --query="file dataset=' + Dataset + ' instance=' + ('prod/global' if not Dataset.endswith ('/USER') else 'prod/phys03') + '" --limit 0 > ' + AAAFileList) 
    inputFileList = open(AAAFileList, "r") 
    inputFiles = inputFileList.read().split('\n')  
    for f in reversed(range(len(inputFiles))): 
        if not ".root" in inputFiles[f]: 
            del inputFiles[f]  
    datasetRead['numberOfFiles'] = len(inputFiles)  
    datasetRead['realDatasetName'] = Dataset 
    text = 'listOfFiles = [  \n' 
    for f in inputFiles:
        if arguments.Redirector != "":
            f = 'root://' + RedirectorDic[arguments.Redirector] + '/' + f
        elif lxbatch:
            f = "root://xrootd.ba.infn.it/" + f
        else: 
            f = "root://cms-xrd-global.cern.ch/" + f 
        text += '"' + f + '",\n'  
    text += ']  \n'  
    text += 'numberOfFiles = ' + str(datasetRead['numberOfFiles']) + '\n'          
    text += 'datasetName = \'' + str(Dataset) +'\'\n'
    text += 'crossSection = ' + str(crossSection) + '\n'          
        
    fnew = open(datasetInfoName, "w") 
    fnew.write(text)
    fnew.close()  
    
#It is the function which generates the list of input files for a given dataset type.
def MakeFileList(Dataset, FileType, Directory, Label, UseAAA, crossSection):
    numberOfFiles = -1
    datasetRead = {}
    datasetRead['useAAA'] = UseAAA
    runList = []
    datasetInfoName = Directory + '/datasetInfo_' + Label + '_cfg.py'
    AAAFileList = Directory + '/AAAFileList.txt'
    os.system('touch ' + datasetInfoName)  
    SkimExists = RunOverSkim and os.path.isdir (Condor + arguments.SkimDirectory + '/' + Label + '/' + arguments.SkimChannel)
    if UseAAA:
        os.system('touch ' + AAAFileList)  
        if FileType == 'OSUT3Ntuple' or FileType == 'Dataset':  
            AcquireAwesomeAAA(Dataset, datasetInfoName, AAAFileList, datasetRead, crossSection)
    if FileType == 'UserDir':
        isInCondorDir = False
        SubmissionDir = os.getcwd() 
        if not "/" in Dataset:
            # Then assume it is in condor/ directory.  
            Dataset = "condor/" + Dataset  
            isInCondorDir = True 
	if not os.path.exists(Dataset):
	    print "The directory you provided does not exist: ", Dataset  
            sys.exit()
        #Get the list of the root files in the directory and modify it to have the standard format. 
        inputFiles = GetListOfRootFiles(Dataset)  
        datasetRead['numberOfFiles'] = len(inputFiles)  
        datasetRead['realDatasetName'] = 'FilesInDirectory:' + Dataset 
        text = 'listOfFiles = [  \n' 
        if not UseAAA:
            for f in inputFiles:
                if remoteAccessT3:
                    f = "root://cms-0.mps.ohio-state.edu:1094/" + f
                else: 
                    if isInCondorDir:
                        f = SubmissionDir + "/" + f  
                    f = "file:" + f 
                text += '"' + f + '",\n'  
            text += ']  \n'  
        else:
            for f in inputFiles:
                if arguments.Redirector != "":
                    f = 'root://' + RedirectorDic[arguments.Redirector] + '/' + f
                elif lxbatch:
                    f = "root://xrootd.ba.infn.it/" + f
                else: 
                    f = "root://cmsxrootd.fnal.gov/" + f 
                text += '"' + f + '",\n'  
            text += ']  \n'  
        text += 'numberOfFiles = ' + str(datasetRead['numberOfFiles']) + '\n' 
        text += 'crossSection = ' + str(crossSection) + '\n'          
        fnew = open(datasetInfoName, "w") 
        fnew.write(text)
        fnew.close()  
    if FileType == 'UserList':
	if not os.path.exists(Dataset):
            print "The list you provided does not exist."
            sys.exit()
        #Get the list of the files to datasetInfo_cfg.py and modify it to have the standard format. 
        inputFileList = open(Dataset, "r") 
        inputFiles = inputFileList.read().split('\n')  
        for f in reversed(range(len(inputFiles))): 
            if not ".root" in inputFiles[f]: 
                del inputFiles[f]  
        datasetRead['numberOfFiles'] = len(inputFiles)  
        datasetRead['realDatasetName'] = 'FilesInList:' + Dataset 
        text = 'listOfFiles = [  \n' 
        #Please give the absolute paths of the files like /data/user/***/condor/dir or /store/....
        if not UseAAA:
            for f in inputFiles:
                if remoteAccessT3:
                    f = "root://cms-0.mps.ohio-state.edu:1094/" + f
                else:    
                    f = "file:" + f 
                text += '"' + f + '",\n'  
            text += ']  \n'  
        else:
            for f in inputFiles:
                if arguments.Redirector != "":
                    f = 'root://' + RedirectorDic[arguments.Redirector] + '/' + f
                elif lxbatch:
                    f = "root://xrootd.ba.infn.it/" + f
                else: 
                    f = "root://cmsxrootd.fnal.gov/" + f 
                text += '"' + f + '",\n'  
            text += ']  \n'  
        text += 'numberOfFiles = ' + str(datasetRead['numberOfFiles']) + '\n'     
        text += 'crossSection = ' + str(crossSection) + '\n'          
        fnew = open(datasetInfoName, "w") 
        fnew.write(text)
        fnew.close()  
    if FileType == 'OSUT3Ntuple':
        prefix = ''
        if not remoteAccessT3 :
	    prefix = 'file:' 
        else:
            prefix = 'root://cms-0.mps.ohio-state.edu:1094/'
        if RunOverSkim:
            print "You have specified a skim as input.  Will obtain cross sections from the database."  
        #Use MySQLModule, a perl script to get the information of the given dataset from T3 DB and save it in datasetInfo_cfg.py. 
        os.system('MySQLModule ' + Dataset + ' ' + datasetInfoName + ' ' + prefix)
        NTupleExistCheck = os.popen('cat ' + datasetInfoName).read()
        InitializeAAA = ""
        if (NTupleExistCheck == '#Dataset does not exist on the Tier 3!' or NTupleExistCheck == '') and not SkimExists: 
            #InitializeAAA = raw_input('The dataset ' + Dataset + ' is not available on T3, do you want to access it via xrootd?("y" to continue or "n" to skip)')    
            InitializeAAA = "y"
            os.system('touch ' + AAAFileList)  
            AcquireAwesomeAAA(Dataset, datasetInfoName, AAAFileList, datasetRead, crossSection)
            datasetRead['useAAA'] = True
            #else:
            #    return 
        if RunOverSkim:
            numInputFiles = len(glob.glob(Condor + arguments.SkimDirectory + '/' + Label + '/' + arguments.SkimChannel + "/*.root"))
            if not numInputFiles:
                print "No input skim files found for dataset " + Label + ".  Will skip it and continue"
                datasetRead['numberOfFiles'] = numInputFiles
                return datasetRead
            SkimDirectory = Condor + str(arguments.SkimDirectory) + '/' + str(Label) + '/' 
            #Copy the datasetInfo file from the skim directory.
            os.system('cp ' + SkimDirectory + 'datasetInfo_' + Label + '_cfg.py ' + datasetInfoName)
            #Modidy the datasetInfo file copied so that it can be used by the jobs running over skims. Also update the crossSection here.
            SkimModifier(Label, Directory, crossSection)
            
            InitializeAAA = ""
        sys.path.append(Directory)
        exec('import datasetInfo_' + Label +'_cfg as datasetInfo')
        if InitializeAAA == "" and not RunOverSkim:
            status = datasetInfo.status
            continueForNonPresentDataset = True
            if not status == 'present':
	        userDecision = raw_input('The dataset you selected is not marked as present on Tier3, do you still want to continue?(Type "y" for yes and "n" for no.)')
                if userDecision == "n":
                    continueForNonPresentDataset = False 
            if not continueForNonPresentDataset:
	        return    
        datasetRead['realDatasetName'] = datasetInfo.datasetName if hasattr (datasetInfo, "datasetName") else DatasetName
        datasetRead['numberOfFiles'] = datasetInfo.numberOfFiles
    return  datasetRead

def MakeBatchJobFile(WorkDir, Queue, NumberOfJobs):
    os.system('touch ' + currentDir + '/' + WorkDir + '/lxbatchSub.sh')
    os.system('touch ' + currentDir + '/' + WorkDir + '/lxbatchRun.sh')
    LxBatchSubFile = open(currentDir + WorkDir + '/lxbatchSub.sh','r+w')
    LxBatchSubFile.write('#!/bin/sh \n') 
    LxBatchSubFile.write('for i in {1..' + NumberOfJobs + '} \n') 
    LxBatchSubFile.write('do \n') 
    LxBatchSubFile.write(' bsub -q ' + Queue + ' -oo ' + currentDir + '/' + WorkDir + '/lxbatch_\$i.log ' + currentDir + '/' + WorkDir + '/lxbatchRun.sh \$i \n') 
    LxBatchSubFile.write('done \n') 
    LxBatchSubFile.write('echo \"Finished submitting ' + NumberOfJobs + ' jobs.\" \n') 
    LxBatchSubFile.close()
    LxBatchRunFile = open(currentDir + WorkDir + '/lxbatchRun.sh','r+w')
    LxBatchRunFile.write('#!/bin/sh \n')
    LxBatchRunFile.write('cd ' + currentDir + '/' + WorkDir + '\n')
    LxBatchRunFile.write('eval `scram runtime -sh` \n')
    LxBatchRunFile.write('cmsRun config_cfg.py True ' + NumberOfJobs + ' \$1 NULL NULL \n')
    LxBatchRunFile.close()
    os.system('chmod +x ' + currentDir + '/' + WorkDir + '/lxbatchRun.sh' )
    os.system('chmod +x ' + currentDir + '/' + WorkDir + '/lxbatchSub.sh' )

###############################################################################
#        Function to find all the skim channels from the userConfig.          # 
###############################################################################
def SkimChannelFinder(userConfig, Directory):
    sys.path.append(Directory)
    skimChannelNames = []
    os.chdir(Directory)
    exec('import ' + userConfig + ' as temPset')
    if not hasattr(temPset.process, "endPath"):
        return []
    outputModules = str(temPset.process.endPath).split('+') 
    for outputModule in outputModules:
        channelName = outputModule[0:len(outputModule)-16]
        skimChannelNames.append(channelName)
    return skimChannelNames
################################################################################
#            Function to modify the dataset_*_Info_cfy file for skim.          #  
################################################################################
def SkimModifier(Label, Directory, crossSection):
    SkimDirectory = Condor + str(arguments.SkimDirectory) + '/' + str(Label) + '/' + str(arguments.SkimChannel)
    OriginalNumberOfEvents = os.popen('cat ' + SkimDirectory + '/OriginalNumberOfEvents.txt').read().split()[0]  
    SkimNumberOfEvents     = os.popen('cat ' + SkimDirectory + '/SkimNumberOfEvents.txt').read().split()[0] 

    infoFile = Directory + '/datasetInfo_' + Label + '_cfg.py'
    fin = open(infoFile, "r")
    orig = fin.read()
    fin.close()
    orig = orig.replace("listOfFiles",   "originalListOfFiles")  
    orig = orig.replace("numberOfFiles", "originalNumberOfFiles")  
    #Use the up-to-date crossSection all the time and keep track of what value was used when making the skim. For a dataset not registered on T3, the corssSection entering this function will be -1, in this case we do not rewrite the crossSection. The value used when making the skim should be added by the -x option which is stored in the datasetInfo file. One rare case is that you want to change the crossSection to -1, but it can be done in the mergeing step where you merge the datasets with intLumi set to -1.  
    if crossSection != -1:
        orig = orig.replace("crossSection", "crossSection = " + str(crossSection) + "# Value used in making the skim is: ")  
    skimFiles = GetListOfRootFiles(SkimDirectory)
    add  = "\n"
    add += 'skimDirectory = "' + SkimDirectory + '"\n'  
    add += 'listOfFiles = [  \n' 
    for s in skimFiles:
        add += '"file:' + s + '",\n'  
    add += ']  \n'  
    add += 'numberOfFiles = ' + str(len(skimFiles)) + '\n'  
    add += 'originalNumberOfEvents = ' + str(OriginalNumberOfEvents) + '\n'
    add += 'skimNumberOfEvents = ' + str(SkimNumberOfEvents) + '\n'
    fnew = open(infoFile, "w")
    fnew.write(orig + add)
    fnew.close()


################################################################################
################################################################################
################################################################################
#                      END OF FUNCTION DEFINITIONS                             #
################################################################################
################################################################################
################################################################################


################################################################################
#             First of all to set up the working directory                     #
################################################################################
CondorDir = ''
Condor = os.getcwd() + '/condor/'
if not os.path.exists(Condor):
    print "The directory ", Condor, "does not exist.  Aborting."  
    sys.exit()
if arguments.condorDir == "":
    print "No working directory is given, aborting."
    sys.exit()
else:
    CondorDir = Condor + arguments.condorDir
#Check whether the directory specified already exists and warn the user if so.
if not os.path.exists(CondorDir):
    os.system('mkdir ' + CondorDir)
else:
    if arguments.FileType == "OSUT3Ntuple":
        # Ok to proceed, since the working directory will be "CondorDir/dataset"
        print 'Directory "' + str(CondorDir) + '" already exists in your condor directory. Will proceed with job submission.' 
    else:
        # Do not proceed because the working directory is CondorDir, and we do not want to overwrite existing files.  
        print "Directory", CondorDir, " already exists.  Please remove it before proceeding."  
        sys.exit()  
RunOverSkim = False
if arguments.SkimDirectory != "" and arguments.SkimChannel != "":
    RunOverSkim = True
elif arguments.SkimDirectory == "" and arguments.SkimChannel == "":
    RunOverSkim = False
else:
    print "Both skim directory and skim channel should be provided."

###############################################################################
# Find the list of dataset to run over, either from arguments or localConfig. #
###############################################################################
split_datasets = []

        
if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.localConfig) + " import *")
    split_datasets = split_composite_datasets(datasets, composite_dataset_definitions)

#Check whether the necessary options are correctly given if no local config is given.
if not arguments.localConfig:
    if not arguments.NumberOfJobs > 0:
    	print "Invalid number of jobs, aborting."
        sys.exit() 
    if arguments.Config == "":
    	print "No cmsRun executable is given, aborting."
        sys.exit() 
    if arguments.Dataset == "":
    	print "Warning, you are running batch jobs witout using input sources."
    else:
	split_datasets.append(arguments.Dataset)   

###############################################################################
#    Get the host name to determine whether you are using lxplus or OSU T3.   #
###############################################################################
UseAAA = False
Generic = False
if arguments.Generic:
    Generic = True
if arguments.UseAAA:
    UseAAA = True
remoteAccessT3 = True
lxbatch  = False
hostname = socket.gethostname()
if 'cern.ch' in hostname:
    lxbatch = True
if 'interactive' in hostname:
    remoteAccessT3 = False
if arguments.Redirector != "":
    if not RedirectorDic.has_key(arguments.Redirector):
        print "Warning! Invalid redirector provided!! Quit!!"
        sys.exit()

###############################################################################
#                End of Setup stage, will begin to submit jobs                #
###############################################################################
# Remove duplicates
split_datasets = list(set(split_datasets))

currentCondorSubArgumentsSet = {}
#Check whether the user wants to resubmit the failed condor jobs.
if not arguments.Resubmit:
    #Loop over the datasets in split_datasets.
    if split_datasets:
        SubmissionDir = os.getcwd()
        SkimChannelNames = []
        for dataset in split_datasets:
            currentCondorSubArgumentsSet = copy.deepcopy(CondorSubArgumentsSet)
            EventsPerJob = -1 
            DatasetName = dataset
            NumberOfJobs = int(arguments.NumberOfJobs)
            DatasetRead = {}
            MaxEvents = arguments.MaxEvents
            Config =  arguments.Config
            if arguments.localConfig:	
                if NumberOfJobs < 0:
                    NumberOfJobs = nJobs[dataset]  # If user has specified NumberOfJobs, use that value.  
                if not arguments.Generic:
                     DatasetName = dataset_names[dataset]
                else:
                     DatasetName = dataset 
                MaxEvents = maxEvents[dataset]
                Config = config_file

            GetCompleteOrderedArgumentsSet(InputCondorArguments, currentCondorSubArgumentsSet)
            
            if arguments.FileType == 'OSUT3Ntuple': 
                if dataset_names.has_key(dataset):
                    DatasetName = dataset_names[dataset]
                else:
                    print str(dataset) + ' has not been registered on T3. Will try to find it on DAS.'
                WorkDir = CondorDir + '/' + SpecialStringModifier(dataset,['/'],[['-','_']])
                if os.path.exists(WorkDir): 
                    print 'Directory "' + str(WorkDir) + '" already exists.  Please remove it and resubmit.'  
                    continue 
                else: 
                    os.system('mkdir ' + WorkDir )
            elif arguments.FileType == 'UserList':
                WorkDir = CondorDir + '/' + SpecialStringModifier(dataset,['/'],[['-','_']])
                if os.path.exists(WorkDir): 
                    print 'Directory "' + str(WorkDir) + '" already exists.  Please remove it and resubmit.'  
                    continue 
                else: 
                    os.system('mkdir ' + WorkDir )
            else:
                WorkDir = CondorDir 
            dataset = SpecialStringModifier(dataset, ['/','.'], [['-','_']])
            crossSection = -1
            if crossSections.has_key(dataset):
                crossSection = crossSections[dataset]       
            elif arguments.crossSection != "":
                crossSection = arguments.crossSection
            DatasetRead = MakeFileList(DatasetName,arguments.FileType,WorkDir,dataset, UseAAA, crossSection)
            NumberOfFiles = int(DatasetRead['numberOfFiles'])
            if not NumberOfFiles:
                continue
            UseAAA = DatasetRead['useAAA']

            if NumberOfJobs > NumberOfFiles:
                NumberOfJobs = NumberOfFiles
            if not arguments.localConfig:     
                NumberOfJobs = int(math.ceil(NumberOfFiles/math.ceil(NumberOfFiles/float(arguments.NumberOfJobs))))
            if float(arguments.NumberOfFilesPerJob) > 0:
                NumberOfJobs = int(math.ceil(NumberOfFiles/float(arguments.NumberOfFilesPerJob)))           
            if MaxEvents > 0:
    	        EventsPerJob = int(math.ceil(int(arguments.MaxEvents)/NumberOfJobs)) 	


            RealMaxEvents = EventsPerJob*NumberOfJobs
            userConfig = 'userConfig_' + dataset + '_cfg.py'
            os.system('cp ' + Config + ' ' + WorkDir + '/' + userConfig)
            jsonFile = ''
            if arguments.localConfig: 
                if(types[dataset] == 'data'):
                    jsonFile = getLatestJsonFile() 
                    os.system('mv ' + jsonFile + ' ' + WorkDir + '/')
            SkimChannelNames = MakeSpecificConfig(DatasetRead['realDatasetName'],WorkDir,dataset, SkimChannelNames, jsonFile)
    

    	    if lxbatch:
    	        MakeBatchJobFile(WorkDir, Queue, NumberOfJobs)
    	    else:
                MakeCondorSubmitScript(DatasetRead['realDatasetName'],NumberOfJobs,WorkDir,dataset, UseAAA, jsonFile)
            if not arguments.NotToExecute:
                os.chdir(WorkDir)
                if RealMaxEvents > 0 : 
                    print 'Submitting ' + str(NumberOfJobs) +  ' jobs to run on ' + str(RealMaxEvents)  + ' events in ' + str(DatasetRead['numberOfFiles']) + ' files for ' + str(dataset) + ' dataset.\n'
                else:
                    print 'Submitting ' + str(NumberOfJobs) +  ' jobs to run on all events in ' + str(DatasetRead['numberOfFiles'])  +' files for ' + str(dataset) + ' dataset.\n'
                if lxbatch: 
            	    os.syetem('./lxbatchSub.sh')
    	        else:
            	    os.system('condor_submit condor.sub')
    	        os.chdir(SubmissionDir)
            else:
                print 'Configuration files created for ' + str(dataset) + ' dataset but no jobs submitted.\n'
    	        os.chdir(SubmissionDir)
    #If there are no input datasets specified and the user still continues.
    else:
        SkimChannelNames = [] 
        NumberOfJobs = arguments.NumberOfJobs
        MaxEvents = -1
        Label = arguments.Label
        if arguments.MaxEvents < 0:
            print "Maximum number of events is negative and no input dataset is specified, Aborting!"
            sys.exit()
        else:
            MaxEvents = int(arguments.MaxEvents)
        EventsPerJob = int(math.ceil(float(MaxEvents)/float(NumberOfJobs)) )
        #Make sure at last the number of total events is larger than the MaxEvents.
        RealMaxEvents = EventsPerJob*int(NumberOfJobs)
        Config =  arguments.Config
        SubmissionDir = os.getcwd()
        WorkDir = CondorDir
        if arguments.localConfig:	
            GetCompleteOrderedArgumentsSet(InputCondorArguments, currentCondorSubArgumentsSet)
        userConfig = 'userConfig_' + Label + '_cfg.py'
        os.system('cp ' + Config + ' ' + WorkDir + '/' + userConfig)
         
        MakeSpecificConfig('',WorkDir,Label, SkimChannelNames,'')
    
        if lxbatch:
            MakeBatchJobFile(WorkDir, Queue, NumberOfJobs)
        else:
            MakeCondorSubmitScript('',NumberOfJobs,WorkDir,Label, UseAAA,'')
        if not arguments.NotToExecute:
            os.chdir(WorkDir)
            print 'Submitting ' + str(NumberOfJobs) +  ' jobs to run ' + str(RealMaxEvents)  + ' events for ' + str(Config) + '.\n'
            if lxbatch: 
                os.syetem('./lxbatchSub.sh')
    	    else:
                os.system('condor_submit condor.sub')
    	    os.chdir(SubmissionDir)
        else:
            print 'Configuration files created for ' + str(Config) + '  but no jobs submitted.\n'
else:
    if split_datasets:
        SubmissionDir = os.getcwd()
        for dataset in split_datasets: 
            WorkDir = CondorDir + '/' + str(dataset)
            if os.path.exists(WorkDir + '/condor_resubmit.sub'):
                os.chdir(WorkDir)
                #If a redirector is defined, switch to the new redirector.
                if arguments.Redirector != "" :
                    if RedirectorDic.has_key(arguments.Redirector):
                        originalRedirector = ""
                        datasetInfoFileName = os.popen("ls datasetInfo_*_cfg.py").read().split('\n')[0] 
                        datasetInfoFile = open(str(datasetInfoFileName),'r')
                        for line in datasetInfoFile:
                           if 'root:' in line:
                               originalRedirector = line.split('/')[2]
                               break
                        os.system('sed \'s/' + str(originalRedirector) + '/' + str(RedirectorDic[arguments.Redirector]) + '/g\' '  +  str(datasetInfoFileName))
                print '################ Resubmit failed jobs for ' + str(dataset) + ' dataset #############'  
                os.system('condor_submit condor_resubmit.sub')
                os.chdir(SubmissionDir)
        
