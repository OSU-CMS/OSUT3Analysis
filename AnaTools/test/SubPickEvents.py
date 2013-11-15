#!/usr/bin/env python

import sys
import os
import re
import time
from math import *
from array import *
from decimal import *
from optparse import OptionParser
from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *
from OSUT3Analysis.Configuration.formattingUtilities import *

parser = OptionParser()
parser = set_commandline_arguments(parser)
(arguments, args) = parser.parse_args()

if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + arguments.localConfig.rstrip('.py') + " import *")

datasets = split_composite_datasets(datasets, composite_dataset_definitions)

os.system('mkdir ' + txtDirectory + '_findEvents')
os.system('mkdir ' + ConfigDirectory)

def replaceName(Origin,String):
	if Origin.find('e=') > 0:
		Origin = Origin[0:Origin.find('e=') + 2] + String
        else:
		Origin = Origin[0:Origin.find('=') + 2] + String
        return  Origin     

for sample in datasets:
        datasetPath = dataset_names[sample]
        name = str(sample)
        filename = './' + txtDirectory + '_findEvents/' + name + '.txt'
        os.system('touch ' + filename)
        command = 'grep "Event passed" ' + outputDirectory +'/' + name + '/*.err | awk \'{print $9}\' > ./' + txtDirectory + '_findEvents/' + name + '.txt 2>&1'
        print command
        os.system(command)
        os.system('edmPickEvents.py "' + datasetPath + '" ' + filename + ' --crab')
        os.system('cp pickevents_runEvents.txt ' + filename)
        
        Config = open('pickevents_crab.config','r')
        ConfigTmp = open('pickevents_crab.config.new','w')
        for line in Config:
                Stop = 0
                for tmpline in UserString:
			TmpString = tmpline[0:5]
                	if TmpString in line:
				Stop = 1
                if Stop:
			continue
                if 'output' in line and 'Process_load'not in line:
			ConfigTmp.write(str(replaceName(line,name + '.root')))
                elif 'Process_load' and 'output' in line:
                        line = line[0:line.find('Process_load') + 13] + filename + ' ' + line[line.find('output'):]
			ConfigTmp.write(str(replaceName(line,name + '.root')))
               	elif '[USER]' in line:
                       	ConfigTmp.write('[USER]\n')
                       	for string in UserString:
                       		ConfigTmp.write(string + '\n')
                elif 'use_server' in line:
                       	line = line[0:line.find('=') +2] + '0'
                       	ConfigTmp.write(line)
               	else:
                  	ConfigTmp.write(line)
        os.system('mv pickevents_crab.config.new ' + './' + ConfigDirectory+'/' + name + '_pickevents_crab.config')
