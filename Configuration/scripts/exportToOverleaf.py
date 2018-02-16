#!/usr/bin/env python

from enum import Enum
from distutils.spawn import find_executable
import sys, os, re, subprocess, tempfile, shutil, tarfile

class DocumentType (Enum):
    UNKNOWN = 0
    AN = 1
    PAS = 2
    PAPER = 3

def createTempFile (suffix = "", prefix = os.path.basename (sys.argv[0]) + "_"):
    tempFileTuple = tempfile.mkstemp (suffix = suffix, prefix = prefix)
    os.close (tempFileTuple[0])
    return tempFileTuple[1]

def executeCommand (command, stdout, stderr):
    if not hasattr (executeCommand, "exitCode"):
        executeCommand.exitCode = 5

    stdout.write ("================================================================================\n")
    stdout.write ("Executing \"" + command + "\"...\n")
    stdout.write ("--------------------------------------------------------------------------------\n")
    stdout.flush ()
    stderr.write ("================================================================================\n")
    stderr.write ("Executing \"" + command + "\"...\n")
    stderr.write ("--------------------------------------------------------------------------------\n")
    stderr.flush ()
    status = subprocess.call (command, stdout = stdout, stderr = stderr, shell = True)
    stdout.write ("================================================================================\n\n\n\n")
    stdout.flush ()
    stderr.write ("================================================================================\n\n\n\n")
    stderr.flush ()

    if status:
        stdout.close ()
        stderr.close ()
        print "\n\nERROR: A Git command failed. Please check the output in:"
        print "  " + os.path.realpath (stdout.name)
        print "  " + os.path.realpath (stderr.name)
        sys.exit (executeCommand.exitCode)

    executeCommand.exitCode += 1

    return status

################################################################################
# Figure out if the TDR environment has been setup and extract command line
# argument.
#-------------------------------------------------------------------------------
tdrIsSetup = (find_executable ("tdr") is not None)
if not tdrIsSetup:
    print "Please setup TDR environment before executing."
    sys.exit (1)

if os.path.exists (".git"):
    print "This is already a Git repository."
    print "Has this project already been exported? Quitting."
    sys.exit (2)

if len (sys.argv) < 2:
    print "Usage: " + os.path.basename (sys.argv[0]) + " OVERLEAF_GIT_LINK"
    sys.exit (0)
overleafRepo = sys.argv[1]
################################################################################


################################################################################
# Figure out the document number and whether the document is an AN, PAS, or
# paper.
#-------------------------------------------------------------------------------
cwd = os.getcwd ()
cadiNumber = None
documentType = DocumentType.UNKNOWN
if re.match (r".*\/(notes|paper)\/[^/]*\/trunk", cwd):
    cadiNumber = re.sub (r".*\/(notes|paper)\/([^/]*)\/trunk", r"\2", cwd)
else:
    print "Please change to the \"trunk\" directory of some note/paper before executing."
    sys.exit (3)

if re.match (r".*\/notes\/[^/]*\/trunk", cwd):
    if cadiNumber.startswith ("AN"):
        documentType = DocumentType.AN
    else:
        documentType = DocumentType.PAS
else:
    documentType = DocumentType.PAPER

if os.path.exists (cadiNumber + "_temp.tex"):
    print "There already exists a \"" + cadiNumber + "_temp.tex\"."
    print "Has this project already been exported? Quitting."
    sys.exit (4)

print "Document is of type " + documentType.name + " with number " + cadiNumber + ".\n"
################################################################################

################################################################################
# Run the tdr command with the --export option and copy the resulting tarball
# to a temporary directory and name it export.tgz.
#-------------------------------------------------------------------------------
print "Exporting document with requisite files...",
sys.stdout.flush ()
tdrOutput = subprocess.check_output (["tdr", "--style=" + documentType.name.lower (), "--export", "b", cadiNumber])
exportDir = None
for line in tdrOutput.splitlines ():
    if not line.startswith ("Export tarball"):
        continue
    exportName = re.sub (r"Export tarball (.*\.tgz).*", r"\1", line)
    exportDir = tempfile.mkdtemp (prefix = os.path.basename (sys.argv[0]) + "_")
    shutil.copy (exportName, exportDir + "/export.tgz")
    break
os.chdir (exportDir)
print " Done."
################################################################################

################################################################################
# Extract files from tarball and copy missing files back to original directory.
#-------------------------------------------------------------------------------
print "Extracting export tarball and copying back requisite files...",
sys.stdout.flush ()
export = tarfile.open ("export.tgz")
export.extractall ()
export.close ()
os.remove ("export.tgz")
os.chdir ("export")
dstFiles = os.listdir (cwd)
for f in os.listdir ("."):
    if not os.path.exists (cwd + "/" + f):
        shutil.copy (f, cwd)
shutil.rmtree (exportDir)
os.chdir (cwd)
print " Done."
################################################################################

################################################################################
# The tdr command creates a temp file that contains a copy of the main document
# file with the requisite preamble and footer added. Replace the copy of the
# main file within the temp file with an \input{} command of the original main
# document file.
#-------------------------------------------------------------------------------
print "Reformatting temp document file...",
sys.stdout.flush ()
mainFile = open (cadiNumber + ".tex", "r")
tempFile = open (cadiNumber + "_temp.tex", "r")
main = mainFile.read ().splitlines ()
temp = tempFile.read ().splitlines ()
mainFile.close ()
tempFile.close ()

newTempFileName = createTempFile ()
newTempFile = open (newTempFileName, "w")
wroteInputLine = False
i = 0
for lineInTemp in temp:
    lineInMain = main[i] if i < len (main) else None
    if lineInMain is None or (lineInTemp != lineInMain and lineInTemp != "% " + lineInMain):
        newTempFile.write (lineInTemp + "\n")
    elif not wroteInputLine:
        newTempFile.write ("\n\input{" + cadiNumber + "}\n\n")
        wroteInputLine = True
        i += 1
    else:
        i += 1
newTempFile.close ()
os.remove (cadiNumber + "_temp.tex")
shutil.move (newTempFileName, cadiNumber + "_temp.tex")
print " Done."
################################################################################

################################################################################
# Push the files to our blank 
#-------------------------------------------------------------------------------
print "Pushing to Overleaf git repository...",
sys.stdout.flush ()
gitOutName = createTempFile (suffix = ".out", prefix = "git_")
gitErrName = createTempFile (suffix = ".err", prefix = "git_")
gitOut = open (gitOutName, "w")
gitErr = open (gitErrName, "w")
executeCommand ("git init", stdout = gitOut, stderr = gitErr)
executeCommand ("git remote add origin " + overleafRepo, stdout = gitOut, stderr = gitErr)
executeCommand ("git fetch origin", stdout = gitOut, stderr = gitErr)
executeCommand ("git merge origin/master", stdout = gitOut, stderr = gitErr)
executeCommand ("git add *", stdout = gitOut, stderr = gitErr)
executeCommand ("git rm main.tex", stdout = gitOut, stderr = gitErr)
executeCommand ("git commit -m \"Initial sync with SVN repository.\"", stdout = gitOut, stderr = gitErr)
executeCommand ("git push origin master", stdout = gitOut, stderr = gitErr)
gitOut.close ()
gitErr.close ()
os.remove (gitOutName)
os.remove (gitErrName)
print " Done."
################################################################################

print "\nProject successfully exported to Overleaf."
