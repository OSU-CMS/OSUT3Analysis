import FWCore.ParameterSet.Config as cms
import copy

##########################################################################
##### Set up control regions for use with the doubleElectronlectron dataset #####
##########################################################################

from OSUT3Analysis.ControlRegions.Preselections import *

doubleElectronTriggerPath = cms.vstring("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v")

#defining these cuts first since they will be used repeatedly 
os_cut = cms.PSet (
    inputCollection = cms.vstring("electron-secondary electron pairs"),
    cutString = cms.string("chargeProduct < 0"),
    numberRequired = cms.string(">= 1")
)

electron_deltaPhi_cut = cms.PSet(
    inputCollection = cms.vstring("electron-secondary electron pairs"),
    cutString = cms.string("deltaPhi > 0"),
    numberRequired = cms.string(">= 1")
)

electron_deltaR_cut = cms.PSet(
    inputCollection = cms.vstring("electron-secondary electron pairs"),
    cutString = cms.string("deltaR > 0.5"),
    numberRequired = cms.string(">= 1")
)
zpeak_cut = cms.PSet (
    inputCollection = cms.vstring("electron-secondary electron pairs"),
    cutString = cms.string("invMass > 60 & invMass < 120"),
    numberRequired = cms.string(">= 1")
)

zpeak_veto = cms.PSet (
    inputCollection = cms.vstring("electron-secondary electron pairs"),
    cutString = cms.string("invMass < 75 | invMass > 105"),
    numberRequired = cms.string(">= 1")
)

##########################################################################
electron_blind_cut = cms.PSet(
	inputCollection = cms.vstring("electrons"),
        cutString = cms.string("abs(correctedD0) < 0.02"),
        numberRequired = cms.string(">= 1")
)

secondary_electron_cuts = cms.VPSet()
secondary_electron_cuts.extend(copy.deepcopy(Electron_Preselection.cuts))
for cut in secondary_electron_cuts:
	if cut.inputCollection == cms.string("electrons"):
		cut.inputCollection = cms.vstring("secondary electrons")
                cut.numberRequired = cms.string("== 2")

ZtoEE_Selection = cms.PSet(
    name = cms.string("Z to EE"),
    triggers = doubleElectronTriggerPath,
    cuts = cms.VPSet ()
)
ZtoEE_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
ZtoEE_Selection.cuts.extend(copy.deepcopy(Electron_Preselection.cuts))
ZtoEE_Selection.cuts.append(electron_blind_cut)
ZtoEE_Selection.cuts.extend(copy.copy(secondary_electron_cuts))
ZtoEE_Selection.cuts.append(os_cut)
ZtoEE_Selection.cuts.append(zpeak_cut)
ZtoEE_Selection.cuts.append(electron_deltaR_cut)

##########################################################################
QCDinZtoEE_Selection = cms.PSet(
    name = cms.string("QCD in Z to EE"),
    triggers = doubleElectronTriggerPath,
    cuts = cms.VPSet ()
)

QCDinZtoEE_Selection.cuts.extend(copy.deepcopy(ZtoEE_Selection.cuts))
for cut in QCDinZtoEE_Selection.cuts:
	if "rel" in str(cut.cutString):
		cut.cutString = cms.string("relPFrhoIso > 0.1 & relPFrhoIso < 1")

##########################################################################

ZtoTauTautoEE_Selection = cms.PSet(
    name = cms.string("Z to TauTau to EE"),
    triggers = doubleElectronTriggerPath,
    cuts = cms.VPSet ()
)
ZtoTauTautoEE_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
ZtoTauTautoEE_Selection.cuts.extend(copy.deepcopy(Electron_Preselection.cuts))
for cut in ZtoTauTautoEE_Selection.cuts:
    if "electrons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 2')

metMT_cut =  cms.PSet (
    inputCollection = cms.vstring("electrons"),
    cutString = cms.string("metMT < 50"),
    numberRequired = cms.string("== 2")
)
ZtoTauTautoEE_Selection.cuts.append(metMT_cut)

ztotaupeak_cut = cms.PSet (
    inputCollection = cms.vstring("electron-electron pairs"),
    cutString = cms.string("invMass > 20 & invMass < 75"),
    numberRequired = cms.string(">= 1")
)
ZtoTauTautoEE_Selection.cuts.append(ztotaupeak_cut)


met_cut =  cms.PSet (
    inputCollection = cms.vstring("mets"),
    cutString = cms.string("pt > 10"),
    numberRequired = cms.string(">= 1"),
    alias = cms.string("met > 10")
)
ZtoTauTautoEE_Selection.cuts.append(met_cut)


##########################################################################

WWtoEE_Selection = cms.PSet(
    name = cms.string("WW to EE"),
    triggers = doubleElectronTriggerPath,
    cuts = cms.VPSet ()
)
WWtoEE_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
WWtoEE_Selection.cuts.extend(copy.deepcopy(Electron_Preselection.cuts))
for cut in WWtoEE_Selection.cuts:
    if "electrons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 2')

leptonpair_pt_cut =  cms.PSet (
    inputCollection = cms.vstring("electron-electron pairs"),
    cutString = cms.string("pt > 45"),
    numberRequired = cms.string(">= 1"),
)
WWtoEE_Selection.cuts.append(leptonpair_pt_cut)

WWtoEE_Selection.cuts.append(os_cut)

WWtoEE_Selection.cuts.append(zpeak_veto)

bjet_veto =  cms.PSet (
    inputCollection = cms.vstring("jets"),
    cutString = cms.string("pt > 30 | btagCombinedSecVertex > 0.244"),
    numberRequired = cms.string("== 0")
)
WWtoEE_Selection.cuts.append(bjet_veto)

met_cut =  cms.PSet (
    inputCollection = cms.vstring("mets"),
    cutString = cms.string("pt > 60"),
    numberRequired = cms.string(">= 1"),
    alias = cms.string("met > 60")
)
WWtoEE_Selection.cuts.append(met_cut)

##########################################################################

WZtoEEE_Selection = cms.PSet(
    name = cms.string("WZ to EEE"),
    triggers = doubleElectronTriggerPath,
    cuts = cms.VPSet ()
)
WZtoEEE_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
WZtoEEE_Selection.cuts.extend(copy.deepcopy(Electron_Preselection.cuts))
for cut in WZtoEEE_Selection.cuts:
    if "electrons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 3')

os_zpeak_cut = cms.PSet (
    inputCollection = cms.vstring("electron-electron pairs"),
    cutString = cms.string("chargeProduct < 0 & invMass > 60 & invMass < 120"),
    numberRequired = cms.string("== 1")
)
WZtoEEE_Selection.cuts.append(os_zpeak_cut)

met_cut =  cms.PSet (
    inputCollection = cms.vstring("mets"),
    cutString = cms.string("pt > 50"),
    numberRequired = cms.string(">= 1"),
    alias = cms.string("met > 50")
)
WZtoEEE_Selection.cuts.append(met_cut)

##########################################################################

ZZtoEEEE_Selection = cms.PSet(
    name = cms.string("ZZ to EEEE"),
    triggers = doubleElectronTriggerPath,
    cuts = cms.VPSet ()
)
ZZtoEEEE_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
ZZtoEEEE_Selection.cuts.extend(copy.deepcopy(Electron_Preselection.cuts))
for cut in ZZtoEEEE_Selection.cuts:
    if "electrons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 4')

double_os_zpeak_cut = cms.PSet (
    inputCollection = cms.vstring("electron-electron pairs"),
    cutString = cms.string("chargeProduct < 0 & invMass > 60 & invMass < 120"),
    numberRequired = cms.string(">= 2")
)
ZZtoEEEE_Selection.cuts.append(double_os_zpeak_cut)

##########################################################################

TTbartoEE_Selection = cms.PSet(
    name = cms.string("TTbar to EE"),
    triggers = doubleElectronTriggerPath,
    cuts = cms.VPSet ()
)
TTbartoEE_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
TTbartoEE_Selection.cuts.extend(copy.deepcopy(Electron_Preselection.cuts))
for cut in TTbartoEE_Selection.cuts:
    if "electrons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 2')

TTbartoEE_Selection.cuts.append(os_cut)

TTbartoEE_Selection.cuts.append(zpeak_veto)

invMass_cut = cms.PSet (
    inputCollection = cms.vstring("electron-electron pairs"),
    cutString = cms.string("invMass > 20"),
    numberRequired = cms.string(">= 1")
)
TTbartoEE_Selection.cuts.append(invMass_cut)

jet_eta_cut =  cms.PSet (
    inputCollection = cms.vstring("jets"),
    cutString = cms.string("abs(eta) < 2.4"),
    numberRequired = cms.string(">= 2")
)
TTbartoEE_Selection.cuts.append(jet_eta_cut)

jet_pt_cut =  cms.PSet (
    inputCollection = cms.vstring("jets"),
    cutString = cms.string("pt > 30"),
    numberRequired = cms.string(">= 2")
)
TTbartoEE_Selection.cuts.append(jet_pt_cut)

btag_cut =  cms.PSet (
    inputCollection = cms.vstring("jets"),
    cutString = cms.string("btagCombinedSecVertex > 0.679"),
    numberRequired = cms.string(">= 1")
)
TTbartoEE_Selection.cuts.append(btag_cut)

met_cut =  cms.PSet (
    inputCollection = cms.vstring("mets"),
    cutString = cms.string("pt > 60"),
    numberRequired = cms.string(">= 1"),
    alias = cms.string("met > 60")
)
TTbartoEE_Selection.cuts.append(met_cut)

##########################################################################

QCDtoEE_Selection = cms.PSet(
    name = cms.string("QCD to EE"),
    triggers = doubleElectronTriggerPath,
    cuts = cms.VPSet ()
)
QCDtoEE_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
QCDtoEE_Selection.cuts.extend(copy.deepcopy(Electron_Preselection.cuts))
for cut in QCDtoEE_Selection.cuts:
    if "electrons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 2')
    if "relPFrhoIso" in str(cut.cutString) and "electrons" in str(cut.inputCollection):
        cut.cutString = cms.string('relPFrhoIso > 0.12')

QCDtoEE_Selection.cuts.append(zpeak_veto)


##########################################################################




