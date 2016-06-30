#!/usr/bin/env python
# Utilities for applying cuts.

import FWCore.ParameterSet.Config as cms

def cutsAreEqual(a, b):
    return (sorted (a.inputCollection) == sorted (b.inputCollection) and a.cutString == b.cutString and a.numberRequired == b.numberRequired)

##################################################
## Functions for adding, removing cuts
##################################################

def addCuts(cutVPset, cutsToAdd):
    for cut in cutsToAdd:
        cutVPset.append(cut)

def addSingleCut(cutVPset, cutToAdd, previousExistingCut):
    # Add cutToAdd immediately after previousExistingCut
    for i in xrange(0, len(cutVPset)):
        if cutsAreEqual(cutVPset[i], previousExistingCut):
            cutVPset.insert(i+1, cutToAdd) # Use i+1 to put cutToAdd afterward

def removeCuts(cutVPset, cutsToRemove):
    for cut in cutsToRemove:
        for i in xrange(len(cutVPset) - 1, -1, -1):  # iterate backwards to avoid error
            if cutsAreEqual(cutVPset[i], cut):
                del cutVPset[i]

def printCuts(cutVPset):  # For debugging
    for i in xrange(len(cutVPset)):
        print cutVPset[i].cutString


##################################################
## Variable string definitions
## These may be used in histogram or cut definitions.
## Example:
## cutString = cms.string("fabs ( " + trackD0WRTBeamspot + " ) < 0.1"),
##################################################
# Calculation from https://github.com/cms-sw/cmssw/blob/CMSSW_7_4_X/DataFormats/TrackReco/interface/TrackBase.h#L650
trackD0WRTBeamspot = "((track.vx -                beamspot.x0) * track.py - (track.vy -                beamspot.y0) * track.px) / track.pt"
trackD0WRTPV       = "((track.vx - eventvariable.leadingPV_x) * track.py - (track.vy - eventvariable.leadingPV_y) * track.px) / track.pt"

# Calculation from https://github.com/cms-sw/cmssw/blob/CMSSW_7_4_X/DataFormats/TrackReco/interface/TrackBase.h#L674
trackDZWRTPV       = "(track.vz - eventvariable.leadingPV_z) \
- ((track.vx - eventvariable.leadingPV_x) * track.px + (track.vy - eventvariable.leadingPV_y) * track.py) / track.pt * track.pz / track.pt"

# Calculation from https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideMuonIdRun2#Muon_Isolation
muonPFIsolation = "(pfIsolationR04_.sumChargedHadronPt \
+ max(0.0, pfIsolationR04_.sumNeutralHadronEt + pfIsolationR04_.sumPhotonEt - 0.5*pfIsolationR04_.sumPUPt )) / pt"



##################################################
## Common cut definitions
##################################################
cutMuonPFIsoTight = cms.PSet (
    inputCollection = cms.vstring("muons"),
    cutString = cms.string(muonPFIsolation + " < 0.15"),
    numberRequired = cms.string(">= 1"),
    alias = cms.string("muon tight PF isolation")
)






