import FWCore.ParameterSet.Config as cms
from OSUT3Analysis.Configuration.cutUtilities import *

##############################################
##### Set up the branches for flat trees #####
##############################################

##############################################################################################

MuonBranches = cms.PSet (
    inputCollection = cms.vstring("muons"),
    branches = cms.VPSet (
        cms.PSet (
            name = cms.string("numMuons"),
            inputVariables = cms.vstring("number(muon)"),
        ),
        cms.PSet (
            name = cms.string("leadMuonPt"),
            index = cms.untracked.int32(0),
            inputVariables = cms.vstring("pt"),
        ),
    )
)

MuonIPBranches = cms.PSet (
    inputCollection = cms.vstring("muons", "beamspots"),
    branches = cms.VPSet (
        cms.PSet (
            name = cms.string("leadMuonD0Beamspot"),
            index = cms.untracked.int32(0),
            inputVariables = cms.vstring("(-(muon.vx - beamspot.x0)*muon.py + (muon.vy - beamspot.y0)*muon.px)/muon.pt"),

        ),
        cms.PSet (
            name = cms.string("leadMuonDzBeamspot"),
            index = cms.untracked.int32(0),
            inputVariables = cms.vstring("(muon.vz - beamspot.z0) - ((muon.vx - beamspot.x0)*muon.px + (muon.vy - beamspot.y0)*muon.py)/muon.pt*(muon.pz/muon.pt)"),
        ),
    )
)
