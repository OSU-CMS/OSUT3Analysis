from OSUT3Analysis.AnaTools.osuAnalysis_cfi import *

###############################################################
##### Set Options for Running your Analyzer Interactively #####
###############################################################



#dir = "/mnt/hadoop/mc/DYToEE_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_DISPLACED_LEPTON-v3"
#dir = "/mnt/hadoop/mc/WW_TuneZ2star_8TeV_pythia6_tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_DISPLACED_LEPTON-v3"
dir = "/store/user/lantonel/BN_DoubleMu_Run2012A-recover-06Aug2012-v1_AOD"

for file in os.listdir(dir):
        process.source.fileNames.extend(cms.untracked.vstring('file:' + dir + '/' + file))
        
#output file name when running interactively
process.TFileService.fileName = 'hist.root'

#number of events to process when running interactively
process.maxEvents.input = 10000

#True -> Fill histograms for all objects, False -> Fill histograms only for objects passing all cuts
#False is recommended under normal circumstances
process.OSUAnalysis.plotAllObjectsInPassingEvents = False

process.OSUAnalysis.muons = cms.InputTag ('BNproducer', 'selectedPatMuonsLoosePFlow')
process.OSUAnalysis.electrons = cms.InputTag ('BNproducer', 'selectedPatElectronsLoosePFlow')

########################################################################
##### Import the information about all the histograms to be filled #####
########################################################################

#import the desired sets of histograms from the standard python file which defines them
from OSUT3Analysis.Configuration.histogramDefinitions import *
process.OSUAnalysis.histogramSets.append(MuonHistograms)
process.OSUAnalysis.histogramSets.append(DiMuonHistograms)
process.OSUAnalysis.histogramSets.append(MetHistograms)
process.OSUAnalysis.histogramSets.append(EventHistograms)

##########################################################
##### Add the Desired Channels to the List to be Run #####
##########################################################

from OSUT3Analysis.ControlRegions.DoubleMuSelections import *
process.OSUAnalysis.channels.append(ZtoMuMu_Selection)






