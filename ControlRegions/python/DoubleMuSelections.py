import FWCore.ParameterSet.Config as cms
import copy

##########################################################################
##### Set up control regions for use with the doubleElectron dataset #####
##########################################################################

from OSUT3Analysis.ControlRegions.Preselections import *

#doubleMuTriggerPath = cms.vstring("HLT_Mu17_Mu8_v")
doubleMuTriggerPath = cms.vstring("")

#defining these cuts first since they will be used repeatedly
os_cut = cms.PSet (
    inputCollection = cms.vstring("muons", "muons"),
    cutString = cms.string("muon.charge * muon.charge < 0"),
    numberRequired = cms.string(">= 1")
)

zpeak_cut = cms.VPSet (
    cms.PSet(
    inputCollection = cms.vstring("muons", "muons"),
    cutString = cms.string("invMass (muon, muon) > 60"),
#    cutString = cms.string("invMass (muon, muon) > 60 && invMass (muon, muon) < 120"),  # causes a seg fault
    numberRequired = cms.string(">= 1")
    ),
    cms.PSet(
    inputCollection = cms.vstring("muons", "muons"),
    cutString = cms.string("invMass (muon, muon) < 120"),
    numberRequired = cms.string(">= 1")
    ),
)

zpeak_veto = cms.PSet (
    inputCollection = cms.vstring("muon-muon pairs"),
    cutString = cms.string("invMass < 75 | invMass > 105"),
    numberRequired = cms.string(">= 1")
)

##########################################################################

ZtoMuMuSelection = cms.PSet(
    name = cms.string("ZtoMuMu"),
    triggers = doubleMuTriggerPath,
    cuts = cms.VPSet ()
)
#ZtoMuMuSelection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
ZtoMuMuSelection.cuts.extend(copy.deepcopy(Muon_Preselection.cuts))
for cut in ZtoMuMuSelection.cuts:
    if "muons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 2')
ZtoMuMuSelection.cuts.append(os_cut)
#ZtoMuMuSelection.cuts.append(zpeak_cut)
ZtoMuMuSelection.cuts.extend(copy.deepcopy(zpeak_cut))

##########################################################################

ZtoTauTautoMuMu_Selection = cms.PSet(
    name = cms.string("Z to TauTau to MuMu"),
    triggers = doubleMuTriggerPath,
    cuts = cms.VPSet ()
)
ZtoTauTautoMuMu_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
ZtoTauTautoMuMu_Selection.cuts.extend(copy.deepcopy(Muon_Preselection.cuts))
for cut in ZtoTauTautoMuMu_Selection.cuts:
    if "muons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 2')

metMT_cut =  cms.PSet (
    inputCollection = cms.vstring("muons"),
    cutString = cms.string("metMT < 50"),
    numberRequired = cms.string("== 2")
)
ZtoTauTautoMuMu_Selection.cuts.append(metMT_cut)

ztotaupeak_cut = cms.PSet (
    inputCollection = cms.vstring("muon-muon pairs"),
    cutString = cms.string("invMass > 20 & invMass < 75"),
    numberRequired = cms.string(">= 1")
)
ZtoTauTautoMuMu_Selection.cuts.append(ztotaupeak_cut)


met_cut =  cms.PSet (
    inputCollection = cms.vstring("mets"),
    cutString = cms.string("pt > 10"),
    numberRequired = cms.string(">= 1"),
    alias = cms.string("met > 10")
)
ZtoTauTautoMuMu_Selection.cuts.append(met_cut)


##########################################################################

WWtoMuMu_Selection = cms.PSet(
    name = cms.string("WW to MuMu"),
    triggers = doubleMuTriggerPath,
    cuts = cms.VPSet ()
)
WWtoMuMu_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
WWtoMuMu_Selection.cuts.extend(copy.deepcopy(Muon_Preselection.cuts))
for cut in WWtoMuMu_Selection.cuts:
    if "muons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 2')

leptonpair_pt_cut =  cms.PSet (
    inputCollection = cms.vstring("muon-muon pairs"),
    cutString = cms.string("pt > 45"),
    numberRequired = cms.string(">= 1"),
)
WWtoMuMu_Selection.cuts.append(leptonpair_pt_cut)

WWtoMuMu_Selection.cuts.append(os_cut)

WWtoMuMu_Selection.cuts.append(zpeak_veto)

bjet_veto =  cms.PSet (
    inputCollection = cms.vstring("jets"),
    cutString = cms.string("pt > 30 | btagCombinedSecVertex > 0.244"),
    numberRequired = cms.string("== 0")
)
WWtoMuMu_Selection.cuts.append(bjet_veto)

met_cut =  cms.PSet (
    inputCollection = cms.vstring("mets"),
    cutString = cms.string("pt > 60"),
    numberRequired = cms.string(">= 1"),
    alias = cms.string("met > 60")
)
WWtoMuMu_Selection.cuts.append(met_cut)

##########################################################################

WZtoMuMuMu_Selection = cms.PSet(
    name = cms.string("WZ to MuMuMu"),
    triggers = doubleMuTriggerPath,
    cuts = cms.VPSet ()
)
WZtoMuMuMu_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
WZtoMuMuMu_Selection.cuts.extend(copy.deepcopy(Muon_Preselection.cuts))
for cut in WZtoMuMuMu_Selection.cuts:
    if "muons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 3')

os_zpeak_cut = cms.PSet (
    inputCollection = cms.vstring("muon-muon pairs"),
    cutString = cms.string("chargeProduct < 0 & invMass > 60 & invMass < 120"),
    numberRequired = cms.string("== 1")
)
WZtoMuMuMu_Selection.cuts.append(os_zpeak_cut)

met_cut =  cms.PSet (
    inputCollection = cms.vstring("mets"),
    cutString = cms.string("pt > 50"),
    numberRequired = cms.string(">= 1"),
    alias = cms.string("met > 50")
)
WZtoMuMuMu_Selection.cuts.append(met_cut)

##########################################################################

ZZtoMuMuMuMu_Selection = cms.PSet(
    name = cms.string("ZZ to MuMuMuMu"),
    triggers = doubleMuTriggerPath,
    cuts = cms.VPSet ()
)
ZZtoMuMuMuMu_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
ZZtoMuMuMuMu_Selection.cuts.extend(copy.deepcopy(Muon_Preselection.cuts))
for cut in ZZtoMuMuMuMu_Selection.cuts:
    if "muons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 4')

double_os_zpeak_cut = cms.PSet (
    inputCollection = cms.vstring("muon-muon pairs"),
    cutString = cms.string("chargeProduct < 0 & invMass > 60 & invMass < 120"),
    numberRequired = cms.string(">= 2")
)
ZZtoMuMuMuMu_Selection.cuts.append(double_os_zpeak_cut)

##########################################################################

TTbartoMuMu_Selection = cms.PSet(
    name = cms.string("TTbar to MuMu"),
    triggers = doubleMuTriggerPath,
    cuts = cms.VPSet ()
)
TTbartoMuMu_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
TTbartoMuMu_Selection.cuts.extend(copy.deepcopy(Muon_Preselection.cuts))
for cut in TTbartoMuMu_Selection.cuts:
    if "muons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 2')

TTbartoMuMu_Selection.cuts.append(os_cut)

TTbartoMuMu_Selection.cuts.append(zpeak_veto)

invMass_cut = cms.PSet (
    inputCollection = cms.vstring("muon-muon pairs"),
    cutString = cms.string("invMass > 20"),
    numberRequired = cms.string(">= 1")
)
TTbartoMuMu_Selection.cuts.append(invMass_cut)

jet_eta_cut =  cms.PSet (
    inputCollection = cms.vstring("jets"),
    cutString = cms.string("abs(eta) < 2.4"),
    numberRequired = cms.string(">= 2")
)
TTbartoMuMu_Selection.cuts.append(jet_eta_cut)

jet_pt_cut =  cms.PSet (
    inputCollection = cms.vstring("jets"),
    cutString = cms.string("pt > 30"),
    numberRequired = cms.string(">= 2")
)
TTbartoMuMu_Selection.cuts.append(jet_pt_cut)

btag_cut =  cms.PSet (
    inputCollection = cms.vstring("jets"),
    cutString = cms.string("btagCombinedSecVertex > 0.679"),
    numberRequired = cms.string(">= 1")
)
TTbartoMuMu_Selection.cuts.append(btag_cut)

met_cut =  cms.PSet (
    inputCollection = cms.vstring("mets"),
    cutString = cms.string("pt > 60"),
    numberRequired = cms.string(">= 1"),
    alias = cms.string("met > 60")
)
TTbartoMuMu_Selection.cuts.append(met_cut)

##########################################################################

QCDtoMuMu_Selection = cms.PSet(
    name = cms.string("QCD to MuMu"),
    triggers = doubleMuTriggerPath,
    cuts = cms.VPSet ()
)
QCDtoMuMu_Selection.cuts.extend(copy.deepcopy(Event_Preselection.cuts))
QCDtoMuMu_Selection.cuts.extend(copy.deepcopy(Muon_Preselection.cuts))
for cut in QCDtoMuMu_Selection.cuts:
    if "muons" in str(cut.inputCollection):
        cut.numberRequired = cms.string('== 2')
    if "relPFdBetaIso" in str(cut.cutString) and "muons" in str(cut.inputCollection):
        cut.cutString = cms.string('relPFdBetaIso > 0.12')

QCDtoMuMu_Selection.cuts.append(zpeak_veto)


##########################################################################




