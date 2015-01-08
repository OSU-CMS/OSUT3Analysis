import FWCore.ParameterSet.Config as cms
import copy

##########################################################################
##### Set up control regions for use with the doubleElectron dataset #####
##########################################################################

from OSUT3Analysis.ControlRegions.Preselections import *

singleMuTriggerPath = cms.vstring("HLT_IsoMu24_v")

##########################################################################

WtoMu_Selection = cms.PSet(
    name = cms.string("W to Mu"),
    triggers = singleMuTriggerPath,
    cuts = cms.VPSet ()
)
WtoMu_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
WtoMu_Selection.cuts.extend(copy.deepcopy(Muon_Preselection.cuts))
for cut in WtoMu_Selection.cuts:
    if "muons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 1')

met_cut =  cms.PSet (
    inputCollection = cms.vstring("mets"),
    cutString = cms.string("pt > 40"),
    numberRequired = cms.string("== 1")
)
WtoMu_Selection.cuts.append(met_cut)

metMT_cut =  cms.PSet (
    inputCollection = cms.vstring("muons"),
    cutString = cms.string("metMT > 40"),
    numberRequired = cms.string("== 1")
)
WtoMu_Selection.cuts.append(metMT_cut)

##########################################################################

TTbartoMuHad_Selection = cms.PSet(
    name = cms.string("TTbar to MuHad"),
    triggers = singleMuTriggerPath,
    cuts = cms.VPSet ()
)
TTbartoMuHad_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
TTbartoMuHad_Selection.cuts.extend(copy.deepcopy(Muon_Preselection.cuts))
for cut in TTbartoMuHad_Selection.cuts:
    if "muons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 1')

jet_eta_cut =  cms.PSet (
    inputCollection = cms.vstring("jets"),
    cutString = cms.string("abs(eta) < 2.4"),
    numberRequired = cms.string(">= 4")
)
TTbartoMuHad_Selection.cuts.append(jet_eta_cut)

jet_pt_cut =  cms.PSet (
    inputCollection = cms.vstring("jets"),
    cutString = cms.string("pt > 30"),
    numberRequired = cms.string(">= 4")
)
TTbartoMuHad_Selection.cuts.append(jet_pt_cut)

btag_cut =  cms.PSet (
    inputCollection = cms.vstring("jets"),
    cutString = cms.string("btagCombinedSecVertex > 0.679"),
    numberRequired = cms.string(">= 2")
)
TTbartoMuHad_Selection.cuts.append(btag_cut)

##########################################################################

QCDtoMu_Selection = cms.PSet(
    name = cms.string("QCD to Mu"),
    triggers = singleMuTriggerPath,
    cuts = cms.VPSet ()
)
QCDtoMu_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
QCDtoMu_Selection.cuts.extend(copy.deepcopy(Muon_Preselection.cuts))
for cut in QCDtoMu_Selection.cuts:
    if "muons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 1')
    if "relPFdBetaIso" in str(cut.cutString) and "muons" in str(cut.inputCollection):
        cut.cutString = cms.string('relPFdBetaIso > 0.3')

met_cut =  cms.PSet (
    inputCollection = cms.vstring("mets"),
    cutString = cms.string("pt < 40"),
    numberRequired = cms.string("== 1")
)
QCDtoMu_Selection.cuts.append(met_cut)


##########################################################################




