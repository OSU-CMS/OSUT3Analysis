import FWCore.ParameterSet.Config as cms
import copy

##########################################################################
######## Set up a sample event selection isolating Z->mumu events ########
##########################################################################

ZtoMuMu_Selection = cms.PSet(
    name = cms.string("Z to MuMu"),
    # multiple triggers implies OR
    triggers = cms.vstring("HLT_Mu17_Mu8_v"),
    cuts = cms.VPSet (
        cms.PSet (
            inputCollection = cms.string("muons"),
            cutString = cms.string("abs(eta) < 2.5"),
            numberRequired = cms.string(">= 1")
        ),
        cms.PSet (
            inputCollection = cms.string("muons"),
            cutString = cms.string("pt > 25"),
            numberRequired = cms.string(">= 1")
        ),
        cms.PSet (
            inputCollection = cms.string("muons"),
            cutString = cms.string("tightID > 0"),
            numberRequired = cms.string(">= 1")
        ),
        cms.PSet (
            inputCollection = cms.string("muons"),
            cutString = cms.string("relPFdBetaIso < 0.12"),
            numberRequired = cms.string(">= 1")
        ),
        cms.PSet (
            inputCollection = cms.string("muons"),
            # dummy cut, we're really just counting good muons
            cutString = cms.string("pt > 0"),
            numberRequired = cms.string("<= 2"),
            # alias overwrites cut name in cutflow table
            alias = cms.string("veto events with more than 2 good muons")
        ),
        cms.PSet (
            inputCollection = cms.string("muon-muon pairs"),
            cutString = cms.string("chargeProduct < 0"),
            numberRequired = cms.string("== 1")
        ),
        cms.PSet (
            inputCollection = cms.string("muon-muon pairs"),
            cutString = cms.string("invMass > 60 & invMass < 120"),
            numberRequired = cms.string("== 1")
        )
    )
)

##########################################################################




