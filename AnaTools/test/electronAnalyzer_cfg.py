from OSUT3Analysis.AnaTools.osuAnalysis_cfi import *
from OSUT3Analysis.Configuration.processingUtilities import *

###############################################################
##### Set Options for Running your Analyzer Interactively #####
###############################################################

#uncomment this line to add a single file to be processed
#process.source.fileNames.append('file:'+os.environ['CMSSW_BASE']+'/src/OSUT3Analysis/AnaTools/test/stop600toBottom_100mm.root')

#uncomment these 3 lines to add all files in a given directory to be processed
dir = "/store/user/abrinke1/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/894bf83260076a22df0c97ce24c6bb58/"
for file in os.listdir(dir):
        process.source.fileNames.extend(cms.untracked.vstring('file:' + dir + file))

#output file name when running interactively
process.TFileService.fileName = 'hist.root'

#number of events to process when running interactively
process.maxEvents.input = -1

#True -> Fill histograms for all objects, False -> Fill histograms only for objects passing all cuts
#False is recommended under normal circumstances
process.OSUAnalysis.plotAllObjectsInPassingEvents = False

########################################################################
##### Import the information about all the histograms to be filled #####
########################################################################

#import the desired sets of histograms from the standard python file which defines them
from OSUT3Analysis.Configuration.histogramDefinitions import *
process.OSUAnalysis.histogramSets.append(ElectronHistograms)
process.OSUAnalysis.histogramSets.append(DiElectronHistograms)

#import user-defined histograms
from OSUT3Analysis.AnaTools.MyHistogramDefinitions import *
process.OSUAnalysis.histogramSets.append(ExtraElectronHistograms)


##########################################################
##### Add the Desired Channels to the List to be Run #####
##########################################################

from OSUT3Analysis.AnaTools.MyEventSelections import *

#add channels this way if you do not want BEAN skims
#process.OSUAnalysis.channels.append(ZtoEE)
#process.OSUAnalysis.channels.append(WtoENu)

#add channels this way if you do want BEAN skims
add_channels(process, [ZtoEE, WtoENu])
