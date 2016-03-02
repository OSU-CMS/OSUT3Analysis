#!/usr/bin/env python
import sys
import os
import math 
import fileinput
import re

from array import *
from optparse import OptionParser
from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *
from OSUT3Analysis.Configuration.formattingUtilities import *

###############################################################################
###############################################################################
# OPTIONS PARSING
###############################################################################
###############################################################################
parser = OptionParser()
parser = set_commandline_arguments(parser)

parser.remove_option("-d")
parser.remove_option("-n")
parser.remove_option("-u")
parser.remove_option("-e")
parser.remove_option("-R")
parser.remove_option("-b")
parser.remove_option("-y")
parser.remove_option("-p")
parser.remove_option("-r")
parser.remove_option("-o")
parser.remove_option("--2D")

parser.add_option("-p", "--precision", dest="precision", 
                  help="precision of values in table, default in cutFlowTable.cpp is 3")  
parser.add_option("-d", "--diff", action="store_true", dest="makeDiffPlots", default=False,
                  help="add a column for data-MC to the tables")
parser.add_option("-r", "--ratio", action="store_true", dest="makeRatioPlots", default=False,
                  help="add a colunn for (data-MC)/MC to the tables")
parser.add_option("-R", "--raw", action="store_true", dest="rawYields", default=False,
                  help="Remove the luminosity weighting factor")
parser.add_option("-s", "--sToB", action="store_true", dest="signalToBackground", default=False,
                  help="add a column for S/sqrt(S+B) to the tables")
parser.add_option("-t", "--totBkgd", action="store_true", dest="totalBkgd", default=False,
                  help="add a column for total background to the tables")
parser.add_option("-o", "--output-file", dest="outputFileName",
                  help="specify an output file base name for the cutflow table (suffix will be appended), default is 'cutFlow'")
parser.add_option("-O", "--output-dir", dest="outputDirectory",
                  help="specify an output directory for the cutflow table, default is to use the Condor directory")
parser.add_option("-v", "--verbose", action="store_true", dest="verbose", default=False,
                  help="verbose output")
parser.add_option("-e", "--noErrors", action="store_true", dest="noErrors", default=False,
                  help="do not print errors in table")
parser.add_option("-x", "--xsecTheory", dest="xsecTheory", 
                  help="specify file with theory cross sections for each dataset")
parser.add_option("-i", "--inputFile", dest="inputFile", 
                  help="specify inputFile; overrides condor directory")

(arguments, args) = parser.parse_args()


from OSUT3Analysis.Configuration.fileUtilities import *  # Import after parsing arguments, to avoid ROOT override of optionparser.  
from ROOT import TFile  


###############################################################################
###############################################################################
# FUNCTIONS, GLOBAL VARIABLES
###############################################################################
###############################################################################


class Table(object):
# A Tables stores the text of all the cells in a rectangular table,
# and the information needed to print the table in Latex format.

    def __init__(self, name=""):
        self.name = name
        self.contents = []  # contents of the table: a 2D array (list of lists) 
        # self.contents[c][r] is the content of the cell in column c and row r,
        # counting from the upper-left corner of the table  
        self.hlines = [] # list of rows that have horizontal lines above them (multiples allowed)
        self.vlines = [] # list of columns that have vertical lines before them (multiples allowed)
        self.formatLatex = True
        self.columnJustify = [] # list of characters specifying the justification ("l", "c", "r") of each column
    def numCols(self):
        return len(self.contents)
    def numRows(self):
        if len(self.contents) == 0:
            return 0
        else:
            return len(self.contents[0])
    def maxColWidth(self, col):
        if col >= len(self.contents):
            print "ERROR:  Cannot find maximum column width for column index", col, "because the table only contains", len(self.contents), "columns."
            return 0
        maxWidth = -1
        for r in range(self.numRows()):
            if len(self.contents[col][r]) > maxWidth:
                maxWidth = len(self.contents[col][r]) 
        return maxWidth

    def addColumn(self, col):
        if self.numRows() > 0 and len(col) != self.numRows():
            print "ERROR: Cannot add column with length", len(col), "to table with", self.numRows(), " rows."
            print "Printing column contents: ", col 
            print "Printing table contents: "
            self.printToFile() 
            return
        newcol = copy.deepcopy(col)
        self.contents.append(newcol)
    
    def addRow(self, row):  
        if self.numCols() == 0: # Must create new columns
            for c in range(len(row)):
                newcol = [ row[c] ]
                self.contents.append(newcol)
            return
        if len(col) != self.numCols(): 
            print "ERROR: Cannot add row with length", len(row), "to table with", self.numCols(), " columns."
            return
        else: 
            for c in range(self.numCols()): 
                self.contents[c].append(row[c]) 

    def initializeJustification(self, option = 'l'):
        self.columnJustify = []
        for c in range(len(self.contents)):
            self.columnJustify.append(option)  

    def getMulticolumnRange(self, row):
        # return the range of columns that will be combined into a multicolumn,
        # i.e., all columns with empty-string entries on either side of a cell
        # that specifies a multicolumn
        minCell = -1
        maxCell = -1
        foundMulticolumn = False 
        for c in range(self.numCols()):
            if "multicolumn" in self.contents[c][row]:
                foundMulticolumn = True
                maxCell = c
            if foundMulticolumn and self.contents[c][row] == "":
                maxCell = c
        foundMulticolumn = False
        for c in reversed(range(self.numCols())): 
            if "multicolumn" in self.contents[c][row]:
                foundMulticolumn = True
                minCell = c
            if foundMulticolumn and self.contents[c][row] == "":
                minCell = c
        return[minCell, maxCell]  
        
    def printToFile(self, texfile=""):
        if texfile == "":
            texfile = "temp.txt" # write to temporary file, then print to screen and delete
            os.system("/bin/rm -f temp.txt")  
        fout = open (texfile, "a")
        hlinesToPrint = copy.deepcopy(self.hlines)
        vlinesToPrint = copy.deepcopy(self.vlines)  


        # Write header: 
        tableFormat = ""        
        for c in range(len(self.columnJustify)):
            while c in vlinesToPrint:
                tableFormat += "|" 
                vlinesToPrint.remove(c)             
            tableFormat += self.columnJustify[c] # Add a character for each column  
        # Check for vlines after last column:  
        while self.numCols()+1 in vlinesToPrint:
            tableFormat += "|" 
            vlinesToPrint.remove(self.numCols()+1) 
        if len(vlinesToPrint) != 0:
            print "WARNING: Found additional vline's that were not printed:"
            print vlinesToPrint
        fout.write("\\begin{table}[htbp] \n") 
        fout.write("\\begin{center} \n") 
        fout.write("\\begin{tabular}{" + tableFormat + "}\n")        
        
        # Write table contents:  
        for r in range(self.numRows()):
            multicolRange = self.getMulticolumnRange(r) 
            while r in hlinesToPrint:
                fout.write("\hline \n")
                hlinesToPrint.remove(r) 
            for c in range(self.numCols()):
                width = self.maxColWidth(c) 
                text = ('{0: <' + str(width) + '}').format(self.contents[c][r]) # left-justify, with specified width 
                fout.write(text + " ")
                printAmpersand = True
                if c >= self.numCols()-1:  # Do not print after last column
                    printAmpersand = False                    
                if multicolRange[0] >= 0 and multicolRange[0] <= c and c <= multicolRange[1] and self.contents[c][r] == "":
                    # Do not print if column will be combined by multicolumn.
                    printAmpersand = False                                        
                if printAmpersand: 
                    fout.write("& ")  
            fout.write("  \\\\ \n")  
        
        # Check for hlines after last row 
        while self.numRows() in hlinesToPrint:
            fout.write("\hline \n")
            hlinesToPrint.remove(self.numRows()) 
        if len(hlinesToPrint) != 0:
            print "WARNING: Found additional hline's that were not printed:"
            print hlinesToPrint
            print "   self.numRows() = ", self.numRows()  
        
        # Close table:
        fout.write("\\end{tabular} \n")
        fout.write("\\end{center} \n") 
        fout.write("\\end{table} \n\n\n\n") 
        fout.close()  

        # If no output file specified originally, print to screen.  
        if texfile == "temp.txt":  
            os.system("cat temp.txt")  
            os.system("/bin/rm -f temp.txt")  
    def makeAllReplacements(self, replacements):
        for replacement in replacements.keys():
            for c in range(self.numCols()):
                for r in range(self.numRows()):
                    self.contents[c][r] = self.contents[c][r].replace(replacement,replacements[replacement]) 
            
        
class CFCell(object): 
    def __init__(self, val=0, err=0):
        # for TH1, xValue filled with bin center
        self.val = val
        self.err = err
        self.upperLimit = 0
        self.useUL = False
        self.precision = -1  # Specify the last digit to be printed, corresponding to 10^precision
        self.roundValToInt = False
        self.printErr = True
        self.printPercent = False 
    def __repr__(self):
        return 'val: %s, val: %s\n' % (self.val,
                                       self.err)
    def __add__(self, other):
        sum = CFCell()
        sum.val = self.val + other.val
        sum.err = math.sqrt(pow(self.err, 2) + pow(other.err, 2))  
        return sum  
    def __sub__(self, other):
        diff = CFCell()
        diff.val = self.val - other.val
        diff.err = math.sqrt(pow(self.err, 2) + pow(other.err, 2))  
        return diff  
    def __div__(self, other):
        quot = CFCell()
        quot.val = self.val / other.val if other.val  else float('nan')
        # Find error on quotient with standard error propagation
        relErrNum =  self.val /  self.err if  self.err else float('nan')
        relErrDen = other.val / other.err if other.err else float('nan')  
        quot.err = quot.val * math.sqrt(pow(relErrNum, 2) + pow(relErrDen, 2))  
        return quot
    def getStr(self):
        precision = 3 
        if arguments.precision:
            precision = int(arguments.precision)
        val = round_sigfigs(self.val, precision)
        err = round_sigfigs(self.err, precision)
        if self.roundValToInt:
            val = int(val)
        if self.printPercent: 
            val *= 100 
            err *= 100  
        newstr = str(val)
        if self.printErr:
            newstr += " \\pm " + str(err)
            if self.printPercent:
                newstr = "(" + newstr + ")"
        if self.printPercent:
            newstr += "\\%" 
        return newstr
    
class CFColumn(object):
    def __init__(self, dataset=""):
        # for TH1, xValue filled with bin center
        self.dataset = dataset
        self.label = getLabel(dataset) 
        self.type = "bgMC" # other options: "signalMC", "data", "extra"  
        self.yields = []  # CFCells 
    def __repr__(self):
        return 'label: ', label, ', number of yields: ', len(self.yields), '\n'
    def getStrColumn(self): # Return a list of strings  
        strCol = ["", self.label]
        for y in self.yields:            
            strCol.append(y.getStr()) 
        return strCol
    def setRoundValToInt(self, toround=True):
        for y in self.yields:
            y.roundValToInt = toround  


class CFTable(object): 
    def __init__(self, channel=""):  
        # for TH1, xValue filled with bin center  
        self.channel = channel 
        self.type = "Event yield" # other options: "Efficiency", "Marginal efficiency" 
        self.cutNames = []  # strings  
        self.datasets = []  # CFColumns  
    def __repr__(self):
        return 'channel: ', channel, ', number of datasets: ', len(self.datasets), '\n'  
    def writeToFile(self, texfile):  
        if arguments.noErrors:
            self.printErrors(False) 
        table = self.getTable()  
        table.printToFile(texfile) 
    def getTable(self): 
        table = Table(self.channel)
        firstCol = [getLumiLabel(), ""] # lumi label at top, then blank line, corresponding to the heading column
        firstCol.extend(copy.deepcopy(self.cutNames))  
        table.addColumn(firstCol)        
        for d in self.datasets:
            if d.type == "data" and self.type == "Event yield":  
                d.setRoundValToInt()  
            table.addColumn(d.getStrColumn())            
        table.contents[-1][0] = "\\multicolumn{" + str(len(self.datasets)) + "}{r}{" + self.type + "}"  
        table.initializeJustification()  
        table.hlines = [1, 2, table.numRows()]
        table.makeAllReplacements(replacements)
        table.makeAllReplacements(secondary_replacements) 
        return table 
    def printErrors(self, toprint): 
        for d in self.datasets:
            for y in d.yields:
                y.printErr = toprint 
    def printPercent(self, toprint): 
        for d in self.datasets:
            for y in d.yields:
                y.printPercent = toprint 


def fillTableCuts(table, dataset_file):
    if len(table.cutNames) != 0:
        print "WARNING: Cuts already defined for table; will not add cuts for channel", table.channel
        return
    inputFile = TFile(dataset_file)
    cutFlow = inputFile.Get(table.channel + "/cutFlow") 
    for i in range(1, cutFlow.GetNbinsX()+1):  # Loop over cuts
        table.cutNames.append(cutFlow.GetXaxis().GetBinLabel(i))  


def getLumiWt(dataset_file):
    mergeLogName = dataset_file.replace(".root", "/mergeOut.log")
    mergeLog = open(mergeLogName, "r") 
    lumiWt = -99
    for line in mergeLog.readlines():
        # Look for line of type:
        # The weighting factor is 0.171865466412.
        if "weighting factor" in line: 
            words = line.split(" ")
            words = filter(None, words) # Remove empty entries.  
            lumiWt = float(words[-1].rstrip(".\n"))  # Strip off the end-line and period from the last word in the line.  
    if lumiWt <= 0:
        print "ERROR:  Found invalid lumiWt from file:", mergeLogName 
        exit(0)
    return lumiWt   

def fillTableColumn(table, dataset_file, dataset):  
    inputFile = TFile(dataset_file)
    cutFlow = inputFile.Get(table.channel + "/cutFlow") 
    if cutFlow.GetNbinsX() != len(table.cutNames):
        print "ERROR:  cutFlow.GetNbinsX() = ", cutFlow.GetNbinsX(), " does not equal len(table.cutNames) = ", len(table.cutNames) 
        print "Will skip channel", table.channel, " from file ", dataset_file
        return    
    newcol = CFColumn(dataset)
    newcol.label = getLabel(dataset)
    newcol.type = types[dataset]  
    if arguments.rawYields: 
        lumiWt = getLumiWt(dataset_file)
    for i in range(1, cutFlow.GetNbinsX()+1):  # Loop over cuts
        newcell = CFCell()
        newcell.val = cutFlow.GetBinContent(i)
        newcell.err = cutFlow.GetBinError(i)
        if arguments.rawYields:
            newcell.val /= lumiWt
            newcell.err /= lumiWt
        newcol.yields.append(newcell)
    table.datasets.append(newcol)  


def makeTableEff(table):
    tableEff = copy.deepcopy(table)
    for c in reversed(range(len(tableEff.datasets))):  # Go backwards to allow removal of columns.  
        col = tableEff.datasets[c]
        if col.dataset is "diff" or col.dataset is "ratio":
            # The difference and ratio do not make sense in the efficiency table.  
            del tableEff.datasets[c]
            continue
        total = copy.deepcopy(col.yields[0])  
        for i in range(0, len(tableEff.cutNames)):
            col.yields[i] = col.yields[i] / total if total else float('nan')
    tableEff.printErrors(False)  
    tableEff.printPercent(True)  
    tableEff.type = "Efficiency"
    return tableEff  

def makeTableMargEff(table):
    tableMargEff = copy.deepcopy(table)
    for c in reversed(range(len(tableMargEff.datasets))):  # Go backwards to allow removal of columns.  
        col = tableMargEff.datasets[c]
        if col.dataset is "diff" or col.dataset is "ratio":
            # The difference and ratio do not make sense in the efficiency table.  
            del tableMargEff.datasets[c]
            continue
        total = copy.deepcopy(col.yields[0])  
        for i in reversed(range(1, len(tableMargEff.cutNames))): # Go backwards, and skip the first element
            col.yields[i] = col.yields[i] / col.yields[i-1] if col.yields[i-1] else float('nan')
        col.yields[0] = CFCell(1.0, 0)  # Marginal efficiency of first cut is 100% by definition.  
    tableMargEff.printErrors(False) 
    tableMargEff.printPercent(True)  
    tableMargEff.type = "Marginal efficiency"
    return tableMargEff  

def addExtraColumn(table, option):
    # First get the values that will be needed
    
    newcol = CFColumn(option)
    newcol.type = "extra" 
    for i in range(0, len(table.cutNames)):  # Loop over cuts
        bkgd = CFCell()
        data = CFCell()
        sig  = CFCell()  
        for d in table.datasets:   # Loop over datasets
            if d.type == "signalMC":
                if sig.val != 0 and option is "signif":
                    print "Warning:  table contains more than one signal sample; will calculate signif with only the first."
                else:
                    sig = d.yields[i]
            elif d.type == "bgMC":
                bkgd += d.yields[i]
            elif d.type == "data":
                data += d.yields[i]
        # Perform calculation, and add cell to the column.
        newcell = CFCell()  
        if option is "total":
            newcell = bkgd
        elif option is "diff":
            newcell = data - bkgd
        elif option is "ratio":
            newcell = (data - bkgd) / bkgd 
        elif option is "signif":
            x  =  sig.val
            y  = bkgd.val
            dx =  sig.err
            dy = bkgd.err
            newcell.val = x / math.sqrt (x + y) if (x+y) else float('nan') 
            # Calculate error with standard error propagation:
            # F = x / sqrt (x + y)
            # errF = sqrt ( (dF/dx)^2 * dx^2 + (dF/dy)^2 * dy^2 )
            # A lot of algebra gives the expression below.
            binError = 4.0 * y * y * dx * dx + 4 * x * y * dx * dx + x * x * dy * dy + x * x * dx * dx;
            binError /= 4 * (x + y) * (x + y) * (x + y);
            binError = math.sqrt (binError);
            newcell.err = binError 
        newcol.yields.append(newcell)  
    table.datasets.append(newcol)  

def getLumiLabel():
    lumiStr = str(intLumi / 1000.) # convert /pb to /fb  
    label = "$\\mathrm{L} = " + lumiStr + "\\,\\mathrm{fb}^{-1}$"
    if arguments.rawYields:
        label = "Raw event counts" 
    return label  

def makePdf(condor_dir,texfile):
    command = "pdflatex -interaction=batchmode -output-directory=./%s %s > /dev/null" % (condor_dir,texfile)
    # process tex files to make pdf files
    os.system(command)
    os.system(command)
    if (arguments.verbose):
        print "Finished running: " + command  
    os.system("rm %saux" % (texfile.rstrip("tex")))
    os.system("rm %slog" % (texfile.rstrip("tex")))


def writeTexFileHeader(texFile):
    fout = open (texfile, "w")
    fout.write ("\\documentclass[a2paper,8pt]{article}\n\n")
    fout.write ("\\usepackage[landscape,margin=0.15cm]{geometry}\n\n")
    fout.write ("\\usepackage{multirow}\n\n")
    fout.write ("\\begin{document}\n\n")
    fout.write ("\\pagestyle{empty}\n\n")
    fout.close ()


def closeTexFile(texFile): 
    fout = open (texfile, "a")
    fout.write ("\n\n")
    fout.write ("\\end{document}\n")
    fout.close ()


def getProcessedDatasets(condor_dir, datasets):
    processed_datasets = []  
    for dataset in datasets:
        fileName = condor_dir + "/" + dataset + ".root"
        if not os.path.exists(fileName):
            if (arguments.verbose):
                print "Couldn't find output file for",dataset,"dataset",fileName,"fileName"  
            continue
        testFile = TFile(fileName)
        if not (testFile.IsZombie()):
            processed_datasets.append(dataset)
        if arguments.verbose:
            print "Opening testFile: ", fileName  
    if len(processed_datasets) is 0:
        sys.exit("Can't find any output root files for the given list of datasets")
    return processed_datasets


def getChannels(condor_dir, dataset):
    # open first input file and re-make its directory structure in the output file
    channels = []
    testFile = TFile(condor_dir + "/" + dataset + ".root")
    testFile.cd()
    for key in testFile.GetListOfKeys():
        if (key.GetClassName() != "TDirectoryFile"):
            continue
        if not "CutFlow" in key.GetName():
            continue     
        channels.append(key.GetName())        
    return channels 


def getLabel(dataset): 
    label = "$" + labels[dataset] + "$"
    label = label.replace("#","\\")
    return label 

#FIXME:  Figure out how best to handle noWeights option.  
#def runNoWeights(condor_dir, processed_datasets):
#weight = intLumi / 10000.0
#for dataset in processed_datasets:
#    dataset_file = "%s/%s.root" % (condor_dir,dataset)
#    fin = TFile (dataset_file)
#    flags = fin.Get ("flags")
#    noWeights = flags and flags.GetBinContent (1)
#    fin.Close ()
#    if types[dataset] != "data" and not noWeights:
#        os.system("mergeTFileServiceHistograms -i %s -o %s -w %g" % (dataset_file, dataset_file + "_tmp", weight))
#    else:
#        os.system("mergeTFileServiceHistograms -i %s -o %s -w %g" % (dataset_file, dataset_file + "_tmp", 1.0))


replacements = {
    ">":"$>$",
    "<":"$<$",
    "\\pm":"$\\pm$",
    " eta ":" $\\eta$ ",
    " abs(eta) ":" $|\\eta|$ ",
    "#":"Num",

    "\\rightarrow":"{\\rightarrow}",
    "\\mu QCD":"\\mu$ $QCD",
    "EM QCD":"EM$ $QCD",
    "BCtoE QCD":"BCtoE$ $QCD",


    " pt ":" $p_{T}$ ",
    " ht ":"$H_{T}$ ",

    "d0":"$d_{0}$",

    " MET ":" Met ",
    " MEt ":" Met ",
    " met ":" Met ",
    " Met ":"$\\not\\!\\!{E}_{T}$ ",

    " MHT ":" Mht ",
    " MHt ":" Mht ",
    " mht ":" Mht ",
    " Mht ":"$\\not\\!\\!{H}_{T}$ ",

    "M_Z" : "$M_{Z}$",
    "M_mumu" : "$M_{\\mu\\mu}$",
    "M_ee" : "$M_{ee}$",
    "M_ll" : "$M_{ll}$",

}

secondary_replacements = {
    "$$<$":"$<"

}

replacements_extra = {}


###############################################################################
###############################################################################
# MAIN CODE BLOCK 
###############################################################################
###############################################################################

if arguments.inputFile:
    condor_dir = "./"
    if "/" in arguments.inputFile: 
        condor_dir = arguments.inputFile
        condor_dir = condor_dir[:condor_dir.rfind('/')]
        condor_dir += "/"  
else: 
    condor_dir = set_condor_output_dir(arguments)

outputDir = "condor/" + arguments.outputDirectory if arguments.outputDirectory else condor_dir

from ROOT import TFile, gROOT, gStyle, gDirectory, TKey

processed_datasets = []

outputFileName = "cutFlow"
if arguments.outputFileName:
    outputFileName = arguments.outputFileName
outputFileName = outputFileName.partition(".")[0]  # If the input filename contains a period, take only the part before the period.  

texfile = outputDir + "/" + outputFileName + ".tex"

if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.localConfig) + " import *")    

###read in any extra replacements from local config file
secondary_replacements.update(replacements_extra)

#### check which input datasets have valid output files
processed_datasets = getProcessedDatasets(condor_dir, datasets)
channels = getChannels(condor_dir, processed_datasets[0])
if arguments.inputFile:
    dataset = arguments.inputFile[arguments.inputFile.rfind('/')+1:]
    dataset = dataset.replace(".root", "")
    datasets = [dataset]
    processed_datasets = []  
    labels = { dataset : dataset }  
if arguments.verbose:
    print "Will process the following channels: ", channels, " in the datasets ", processed_datasets

writeTexFileHeader(texfile) 
labels["total"]  = "$total bkgd MC$"
labels["diff"]   = "$data - MC$"
labels["ratio"]  = "$(data - MC) / MC$"
labels["signif"] = "S / \\sqrt(S+B)" 

for channel in channels: # loop over final states, which each have their own directory
    table = CFTable(channel)
    for dataset in processed_datasets:
        dataset_file = "%s/%s.root" % (condor_dir,dataset)
        if len(table.datasets) == 0:  # Fill the cuts only the first time
            fillTableCuts(table, dataset_file)  
        fillTableColumn(table, dataset_file, dataset)
        
    # Add optional columns to end of table:
    if arguments.totalBkgd:
        addExtraColumn(table, "total")
    if arguments.makeDiffPlots:
        addExtraColumn(table, "diff")
    if arguments.makeRatioPlots:
        addExtraColumn(table, "ratio")
    if arguments.signalToBackground: 
        addExtraColumn(table, "signif")

    tableEff     = makeTableEff    (table)
    tableMargEff = makeTableMargEff(table)
    
    # Write to file  
    fout = open (texfile, "a") 
    if channel != channels[0]: # Do for all except first
        fout.write ("\\pagebreak\n\n")
    formatted_channel = channel.replace("_"," ")
    fout.write ("\\section*{" + formatted_channel + " channel}\n\n")
    fout.write ("\\subsection*{Cut flow}\n\n")
    fout.close ()
    table       .writeToFile(texfile)
    tableEff    .writeToFile(texfile)
    tableMargEff.writeToFile(texfile)
    # END LOOP OVER CHANNELS

closeTexFile(texfile)  
makePdf(outputDir,texfile) 

print "Finished writing cutFlow to " + outputDir + "/" + outputFileName + "{.tex,.pdf}"

