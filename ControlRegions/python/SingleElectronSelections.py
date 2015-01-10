import FWCore.ParameterSet.Config as cms
import copy

##########################################################################
##### Set up control regions for use with the doubleElectron dataset #####
##########################################################################

from OSUT3Analysis.ControlRegions.Preselections import *

singleElectronTriggerPath = cms.vstring("HLT_Ele27_WP80_v")

##########################################################################

WtoE_Selection = cms.PSet(
    name = cms.string("W to E"),
    triggers = singleElectronTriggerPath,
    cuts = cms.VPSet ()
)
WtoE_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
WtoE_Selection.cuts.extend(copy.deepcopy(Electron_Preselection.cuts))
for cut in WtoE_Selection.cuts:
    if "electrons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 1')

met_cut =  cms.PSet (
    inputCollection = cms.vstring("mets"),
    cutString = cms.string("pt > 40"),
    numberRequired = cms.string("== 1")
)
WtoE_Selection.cuts.append(met_cut)

metMT_cut =  cms.PSet (
    inputCollection = cms.vstring("electrons"),
    cutString = cms.string("metMT > 40"),
    numberRequired = cms.string("== 1")
)
WtoE_Selection.cuts.append(metMT_cut)

##########################################################################

TTbartoEHad_Selection = cms.PSet(
    name = cms.string("TTbar to EHad"),
    triggers = singleElectronTriggerPath,
    cuts = cms.VPSet ()
)
TTbartoEHad_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
TTbartoEHad_Selection.cuts.extend(copy.deepcopy(Electron_Preselection.cuts))
for cut in TTbartoEHad_Selection.cuts:
    if "electrons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 1')

jet_eta_cut =  cms.PSet (
    inputCollection = cms.vstring("jets"),
    cutString = cms.string("abs(eta) < 2.4"),
    numberRequired = cms.string(">= 4")
)
TTbartoEHad_Selection.cuts.append(jet_eta_cut)

jet_pt_cut =  cms.PSet (
    inputCollection = cms.vstring("jets"),
    cutString = cms.string("pt > 30"),
    numberRequired = cms.string(">= 4")
)
TTbartoEHad_Selection.cuts.append(jet_pt_cut)

btag_cut =  cms.PSet (
    inputCollection = cms.vstring("jets"),
    cutString = cms.string("btagCombinedSecVertex > 0.679"),
    numberRequired = cms.string(">= 2")
)
TTbartoEHad_Selection.cuts.append(btag_cut)

##########################################################################

QCDtoE_Selection = cms.PSet(
    name = cms.string("QCD to E"),
    triggers = singleElectronTriggerPath,
    cuts = cms.VPSet ()
)
QCDtoE_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
QCDtoE_Selection.cuts.extend(copy.deepcopy(Electron_Preselection.cuts))
for cut in QCDtoE_Selection.cuts:
    if "electrons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 1')
    if "relPFrhoIso" in str(cut.cutString) and "electrons" in str(cut.inputCollection):
        cut.cutString = cms.string('relPFrhoIso > 0.3')
met_cut =  cms.PSet (
    inputCollection = cms.vstring("mets"),
    cutString = cms.string("pt < 40"),
    numberRequired = cms.string("== 1")
)
QCDtoE_Selection.cuts.append(met_cut)


##########################################################################




