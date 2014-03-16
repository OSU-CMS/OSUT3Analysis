#!/usr/bin/env python
import os
import sys
import datetime
import shutil
import re
import subprocess
import signal
import fcntl
from optparse import OptionParser

from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *

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

parser.add_option("-s", "--skimDir", dest="skimDir",
                  help="condor directory containing skim")
parser.add_option("-a", "--skimChannel", dest="skimChannel",
                  help="channel from skim to use")
parser.add_option("-d", "--mergeDaemon", action="store_true", dest="mergeDaemon", default=False,
                  help="launch a daemon to merge output when jobs are done")

(arguments, args) = parser.parse_args()

if (arguments.skimDir and not arguments.skimChannel) or (not arguments.skimDir and arguments.skimChannel):
    print "Both the skim directory (--skimDir) and channel (--skimChannel) must be given."
    exit ()

if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.localConfig) + " import *")
exec("import " + re.sub (r".py$", r"", config_file) + " as configFileContents")
hasTTree = len (configFileContents.process.OSUAnalysis.treeBranchSets)

condor_dir = set_condor_submit_dir(arguments)
short_condor_dir = condor_dir
short_condor_dir = re.sub (r".*/([^/]*)", r"\1", short_condor_dir)
short_condor_dir = list (short_condor_dir)
short_condor_dir[0] = short_condor_dir[0].upper ()
short_condor_dir = "".join (short_condor_dir)

if not os.path.exists (condor_dir):
    os.system("mkdir %s" % (condor_dir))

split_datasets = split_composite_datasets(datasets, composite_dataset_definitions)

clusters = ""
submissionErrors = False
for dataset in split_datasets:
    output_dir = "%s/%s" % (condor_dir, dataset)
    skim_dir = ""
    skim_channel_dir = ""
    command = "osusub -l %s -m %d -p %s %s %s %s %s" % (dataset, maxEvents[dataset], short_condor_dir, dataset_names[dataset], config_file, output_dir, nJobs[dataset])
    if arguments.skimDir:
        skim_dir = "condor/" + arguments.skimDir + "/" + dataset
        skim_channel_dir = "condor/" + arguments.skimDir + "/" + dataset + "/" + arguments.skimChannel
        if os.path.exists (skim_channel_dir) and (types[dataset] == "data" or (os.path.exists (skim_channel_dir + "/skimNumberOfEvents.txt") and os.path.exists (skim_dir + "/numberOfEvents.txt") and os.path.exists (skim_dir + "/crossSectionInPicobarn.txt"))):
            command = "osusub -d %s -l %s -m %d -p %s %s %s %s %s" % (dataset_names[dataset], dataset, maxEvents[dataset], short_condor_dir, skim_channel_dir, config_file, output_dir, nJobs[dataset])
        elif not os.path.exists (skim_channel_dir):
            print dataset + "/" + arguments.skimChannel + " not in skim directory. Skipping."
            continue
        elif os.path.exists (skim_channel_dir):
            print dataset + " skim is missing required files. Skipping."
            print "  Run mergeOutput.py to generate these files."
            continue
    print command
    pid = os.getpid ()
    p0 = subprocess.Popen (command.split (), bufsize=1, stdout=subprocess.PIPE)
    flags = fcntl.fcntl (p0.stdout.fileno (), fcntl.F_GETFL, 0)
    fcntl.fcntl (p0.stdout.fileno (), fcntl.F_SETFL, flags | os.O_NONBLOCK)
    output = ""
    while p0.poll () is None:
        try:
            tmpOutput = p0.stdout.read (1024)
            if re.search (r"[^ \f\n\r\t]", tmpOutput):
                print tmpOutput,
            output += tmpOutput
        except IOError:
            pass
    tmpOutput = p0.stdout.read (1024)
    if re.search (r"[^ \f\n\r\t]", tmpOutput):
        print tmpOutput,
    output += tmpOutput
    output = re.sub (r"[\f\n\r]", r"", output)
    if re.search (r"submitted to cluster", output):
        output = re.sub (r".*submitted to cluster (.*)\..*$", r"\1", output)
        clusters += " " + output
    else:
        submissionErrors = True
    if arguments.skimDir and os.path.exists (skim_channel_dir + "/skimNumberOfEvents.txt") and os.path.exists (skim_dir + "/numberOfEvents.txt") and os.path.exists (skim_dir + "/crossSectionInPicobarn.txt"):
        shutil.copy (skim_channel_dir + "/skimNumberOfEvents.txt", output_dir + "/skimNumberOfEvents.txt")
        shutil.copy (skim_dir + "/numberOfEvents.txt", output_dir + "/originalNumberOfEvents.txt")
        f = open (skim_channel_dir + "/skimNumberOfEvents.txt", "r")
        skimNumberOfEvents = float (f.read ().rstrip ())
        f.close ()
        f = open (skim_dir + "/numberOfEvents.txt", "r")
        numberOfEvents = float (f.read ().rstrip ())
        f.close ()
        f = open (skim_dir + "/crossSectionInPicobarn.txt", "r")
        crossSectionInPicobarn = float (f.read ().rstrip ())
        f.close ()
        if numberOfEvents:
            crossSectionInPicobarn *= skimNumberOfEvents / numberOfEvents
        else:
            crossSectionInPicobarn *= skimNumberOfEvents * numberOfEvents
        f = open (output_dir + "/crossSectionInPicobarn.txt", "w")
        f.write (str (crossSectionInPicobarn) + "\n")
        f.close ()

if arguments.mergeDaemon and len (clusters) > 0:
    response = "y"
    if submissionErrors:
        print "\nIt looks like there were errors during submission."
        response = raw_input ("Launch merging daemon anyway? (y/N): ").lower ()
        response = re.sub (r"[ \f\n\r\t]", r"", response)
    if len (response) > 0 and response[0] == "y":
        command = "mergeOutput.py"
        if arguments.condorDir:
            command += " -c " + arguments.condorDir
        pid = os.fork ()
        if not pid:
            signal.signal (signal.SIGHUP, signal.SIG_IGN)
            pid = os.getpid ()
            if arguments.localConfig:
                shutil.copy (arguments.localConfig, "mergeDaemonOptions_" + str (pid) + ".py")
                command += " -l mergeDaemonOptions_" + str (pid) + ".py"
            if hasTTree:
                command += " -t"
            os.execvp ("mergeDaemon.pl", ["mergeDaemon.pl", command] + clusters.split ())
        else:
            print "\nMerging daemon PID: " + str (pid)
