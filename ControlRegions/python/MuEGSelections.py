import FWCore.ParameterSet.Config as cms
import copy

##########################################################################
##### Set up control regions for use with the doubleElectron dataset #####
##########################################################################

from OSUT3Analysis.ControlRegions.Preselections import *

#Z->tautau->emu
#WW->emu
#WZ->emumu
#WZ->eemu
#ZZ->eemumu
#TTbar->emu
#QCD->emu

muEGTriggerPath = cms.vstring("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v","HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v")

#defining these cuts first since they will be used repeatedly 
e_os_cut = cms.PSet (
    inputCollection = cms.string("electron-electron pairs"),
    cutString = cms.string("chargeProduct < 0"),
    numberRequired = cms.string(">= 1")
)

mu_os_cut = cms.PSet (
    inputCollection = cms.string("muon-muon pairs"),
    cutString = cms.string("chargeProduct < 0"),
    numberRequired = cms.string(">= 1")
)

emu_os_cut = cms.PSet (
    inputCollection = cms.string("electron-muon pairs"),
    cutString = cms.string("chargeProduct < 0"),
    numberRequired = cms.string(">= 1")
)

e_zpeak_cut = cms.PSet (
    inputCollection = cms.string("electron-electron pairs"),
    cutString = cms.string("invMass > 60 & invMass < 120"),
    numberRequired = cms.string(">= 1")
)

mu_zpeak_cut = cms.PSet (
    inputCollection = cms.string("muon-muon pairs"),
    cutString = cms.string("invMass > 60 & invMass < 120"),
    numberRequired = cms.string(">= 1")
)

##########################################################################

ZtoTauTautoEMu_Selection = cms.PSet(
    name = cms.string("Z to TauTau to EMu"),
    triggers = muEGTriggerPath,
    cuts = cms.VPSet ()
)
ZtoTauTautoEMu_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
ZtoTauTautoEMu_Selection.cuts.extend(copy.deepcopy(Electron_Preselection.cuts))
ZtoTauTautoEMu_Selection.cuts.extend(copy.deepcopy(Muon_Preselection.cuts))
for cut in ZtoTauTautoEMu_Selection.cuts:
    if "muons" in str(cut.inputCollection) or "electrons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 1')

e_metMT_cut =  cms.PSet (
    inputCollection = cms.string("electrons"),
    cutString = cms.string("metMT < 50"),
    numberRequired = cms.string("== 1")
)
ZtoTauTautoEMu_Selection.cuts.append(e_metMT_cut)

mu_metMT_cut =  cms.PSet (
    inputCollection = cms.string("muons"),
    cutString = cms.string("metMT < 50"),
    numberRequired = cms.string("== 1")
)
ZtoTauTautoEMu_Selection.cuts.append(mu_metMT_cut)

ZtoTauTautoEMu_Selection.cuts.append(emu_os_cut)

##########################################################################

WWtoEMu_Selection = cms.PSet(
    name = cms.string("WW to EMu"),
    triggers = muEGTriggerPath,
    cuts = cms.VPSet ()
)
WWtoEMu_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
WWtoEMu_Selection.cuts.extend(copy.deepcopy(Electron_Preselection.cuts))
WWtoEMu_Selection.cuts.extend(copy.deepcopy(Muon_Preselection.cuts))
for cut in WWtoEMu_Selection.cuts:
    if "muons" in str(cut.inputCollection) or "electrons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 1')

leptonpair_pt_cut =  cms.PSet (
    inputCollection = cms.string("electron-muon pairs"),
    cutString = cms.string("pt > 45"),
    numberRequired = cms.string(">= 1"),
)
WWtoEMu_Selection.cuts.append(leptonpair_pt_cut)

WWtoEMu_Selection.cuts.append(emu_os_cut)

bjet_veto =  cms.PSet (
    inputCollection = cms.string("jets"),
    cutString = cms.string("pt > 30 | btagCombinedSecVertex > 0.244"),
    numberRequired = cms.string("== 0")
)
WWtoEMu_Selection.cuts.append(bjet_veto)

met_cut =  cms.PSet (
    inputCollection = cms.string("mets"),
    cutString = cms.string("pt > 60"),
    numberRequired = cms.string(">= 1"),
    alias = cms.string("met > 60")
)
WWtoEMu_Selection.cuts.append(met_cut)

##########################################################################

WZtoEMuMu_Selection = cms.PSet(
    name = cms.string("WZ to EMuMu"),
    triggers = muEGTriggerPath,
    cuts = cms.VPSet ()
)
WZtoEMuMu_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
WZtoEMuMu_Selection.cuts.extend(copy.deepcopy(Electron_Preselection.cuts))
for cut in WZtoEMuMu_Selection.cuts:
    if "electrons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 1')

WZtoEMuMu_Selection.cuts.extend(copy.deepcopy(Muon_Preselection.cuts))
for cut in WZtoEMuMu_Selection.cuts:
    if "muons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 2')

WZtoEMuMu_Selection.cuts.append(mu_os_cut)
WZtoEMuMu_Selection.cuts.append(mu_zpeak_cut)

met_cut =  cms.PSet (
    inputCollection = cms.string("mets"),
    cutString = cms.string("pt > 30"),
    numberRequired = cms.string(">= 1"),
    alias = cms.string("met > 30")
)
WZtoEMuMu_Selection.cuts.append(met_cut)

##########################################################################

WZtoEEMu_Selection = cms.PSet(
    name = cms.string("WZ to EEMu"),
    triggers = muEGTriggerPath,
    cuts = cms.VPSet ()
)
WZtoEEMu_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
WZtoEEMu_Selection.cuts.extend(copy.deepcopy(Electron_Preselection.cuts))
for cut in WZtoEEMu_Selection.cuts:
    if "electrons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 2')

WZtoEEMu_Selection.cuts.extend(copy.deepcopy(Muon_Preselection.cuts))
for cut in WZtoEEMu_Selection.cuts:
    if "muons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 1')

WZtoEEMu_Selection.cuts.append(e_os_cut)
WZtoEEMu_Selection.cuts.append(e_zpeak_cut)

met_cut =  cms.PSet (
    inputCollection = cms.string("mets"),
    cutString = cms.string("pt > 30"),
    numberRequired = cms.string(">= 1"),
    alias = cms.string("met > 30")
)
WZtoEEMu_Selection.cuts.append(met_cut)

##########################################################################

ZZtoEEMuMu_Selection = cms.PSet(
    name = cms.string("ZZ to EEMuMu"),
    triggers = muEGTriggerPath,
    cuts = cms.VPSet ()
)
ZZtoEEMuMu_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
ZZtoEEMuMu_Selection.cuts.extend(copy.deepcopy(Electron_Preselection.cuts))
for cut in ZZtoEEMuMu_Selection.cuts:
    if "electrons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 2')
ZZtoEEMuMu_Selection.cuts.extend(copy.deepcopy(Muon_Preselection.cuts))
for cut in ZZtoEEMuMu_Selection.cuts:
    if "muons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 2')

ZZtoEEMuMu_Selection.cuts.append(e_os_cut)
ZZtoEEMuMu_Selection.cuts.append(e_zpeak_cut)
ZZtoEEMuMu_Selection.cuts.append(mu_os_cut)
ZZtoEEMuMu_Selection.cuts.append(mu_zpeak_cut)

##########################################################################

TTbartoEMu_Selection = cms.PSet(
    name = cms.string("TTbar to EMu"),
    triggers = muEGTriggerPath,
    cuts = cms.VPSet ()
)
TTbartoEMu_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
TTbartoEMu_Selection.cuts.extend(copy.deepcopy(Electron_Preselection.cuts))
for cut in TTbartoEMu_Selection.cuts:
    if "electrons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 1')
TTbartoEMu_Selection.cuts.extend(copy.deepcopy(Muon_Preselection.cuts))
for cut in TTbartoEMu_Selection.cuts:
    if "muons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 1')

TTbartoEMu_Selection.cuts.append(emu_os_cut)

jet_eta_cut =  cms.PSet (
    inputCollection = cms.string("jets"),
    cutString = cms.string("abs(eta) < 2.4"),
    numberRequired = cms.string(">= 2")
)
TTbartoEMu_Selection.cuts.append(jet_eta_cut)

jet_pt_cut =  cms.PSet (
    inputCollection = cms.string("jets"),
    cutString = cms.string("pt > 30"),
    numberRequired = cms.string(">= 2")
)
TTbartoEMu_Selection.cuts.append(jet_pt_cut)

btag_cut =  cms.PSet (
    inputCollection = cms.string("jets"),
    cutString = cms.string("btagCombinedSecVertex > 0.679"),
    numberRequired = cms.string(">= 1")
)
TTbartoEMu_Selection.cuts.append(btag_cut)


##########################################################################

QCDtoEMu_Selection = cms.PSet(
    name = cms.string("QCD to EMu"),
    triggers = muEGTriggerPath,
    cuts = cms.VPSet ()
)
QCDtoEMu_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
QCDtoEMu_Selection.cuts.extend(copy.deepcopy(Electron_Preselection.cuts))
for cut in QCDtoEMu_Selection.cuts:
    if "electrons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 1')
    if "relPFrhoIso" in str(cut.cutString) and "electrons" in str(cut.inputCollection):
        cut.cutString = cms.string('relPFrhoIso > 0.1')
QCDtoEMu_Selection.cuts.extend(copy.deepcopy(Muon_Preselection.cuts))
for cut in QCDtoEMu_Selection.cuts:
    if "muons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 1')
    if "relPFdBetaIso" in str(cut.cutString) and "muons" in str(cut.inputCollection):
        cut.cutString = cms.string('relPFdBetaIso > 0.12')

##########################################################################




