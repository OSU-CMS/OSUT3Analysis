from OSUT3Analysis.AnaTools.osuAnalysis_cfi import *

###############################################################
##### Set Options for Running your Analyzer Interactively #####
###############################################################



#dir = "/mnt/hadoop/mc/DYToEE_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_DISPLACED_LEPTON-v3"
#dir = "/mnt/hadoop/mc/WW_TuneZ2star_8TeV_pythia6_tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_DISPLACED_LEPTON-v3"
dir = "/data/users/hart/condor/eMuMinimal/W0jets/EMu_Minimal/"

for file in os.listdir(dir):
        process.source.fileNames.extend(cms.untracked.vstring('file:' + dir + '/' + file))
        
#output file name when running interactively
process.TFileService.fileName = 'hist.root'

#number of events to process when running interactively
process.maxEvents.input = -1

#True -> Fill histograms for all objects, False -> Fill histograms only for objects passing all cuts
#False is recommended under normal circumstances
process.OSUAnalysis.plotAllObjectsInPassingEvents = False
process.OSUAnalysis.electronSFFile  =  cms.string (os.environ['CMSSW_BASE'] + '/src/OSUT3Analysis/Configuration/data/MVANonTrig_HtoZZto4l_IdIsoSip.root')
process.OSUAnalysis.electronSFID    =  cms.string ('')
process.OSUAnalysis.electronSF      =  cms.string ('h_electronScaleFactor_IdIsoSip')
process.OSUAnalysis.muons = cms.InputTag ('BNproducer', 'selectedPatMuonsLoosePFlow')
process.OSUAnalysis.electrons = cms.InputTag ('BNproducer', 'selectedPatElectronsLoosePFlow')

########################################################################
##### Import the information about all the histograms to be filled #####
########################################################################

#import the desired sets of histograms from the standard python file which defines them
from OSUT3Analysis.Configuration.histogramDefinitions import *
process.OSUAnalysis.histogramSets.append(ElectronHistograms)
process.OSUAnalysis.histogramSets.append(DiElectronHistograms)
process.OSUAnalysis.histogramSets.append(JetHistograms)
process.OSUAnalysis.histogramSets.append(EventHistograms)
process.OSUAnalysis.histogramSets.append(MetHistograms)


##########################################################
##### Add the Desired Channels to the List to be Run #####
##########################################################

from OSUT3Analysis.ControlRegions.DoubleElectronSelections import *
process.OSUAnalysis.channels.append(ZtoEE_Selection)
#process.OSUAnalysis.channels.append(ZtoTauTautoEE_Selection)
#process.OSUAnalysis.channels.append(WWtoEE_Selection)
#process.OSUAnalysis.channels.append(WZtoEEE_Selection)
#process.OSUAnalysis.channels.append(ZZtoEEEE_Selection)
#process.OSUAnalysis.channels.append(TTbartoEE_Selection)
#process.OSUAnalysis.channels.append(QCDtoEE_Selection)





