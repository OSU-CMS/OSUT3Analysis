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
            return
    print '[addSingleCut] Warning: previousExistingCut not found in VPSet with cutString:', previousExistingCut.cutString

def replaceSingleCut(cutVPset, replacementCut, cutToBeReplaced):
    # Add replacementCut in the same position as cutToBeReplaced
    for i in xrange(len(cutVPset) - 1, -1, -1):
        if cutsAreEqual(cutVPset[i], cutToBeReplaced):
            del cutVPset[i]
            cutVPset.insert(i, replacementCut)
            return
    print '[replaceSingleCut] Warning: previousExistingCut not found in VPSet with cutString:', previousExistingCut.cutString

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
trackD0WRTBeamspot = "((track.vx - beamspot.x0) * track.py - (track.vy - beamspot.y0) * track.px) / track.pt"
trackD0WRTPV       = "((track.vx - eventvariable.leadingPV_x) * track.py - (track.vy - eventvariable.leadingPV_y) * track.px) / track.pt"

electronD0WRTBeamspot = "((electron.vx - beamspot.x0) * electron.py - (electron.vy - beamspot.y0) * electron.px) / electron.pt"
electronD0WRTBeamspotErr = "hypot(electron.gsfTrack.d0Error, hypot(beamspot.x0Error, beamspot.y0Error))"
electronD0WRTBeamspotSig = "(((electron.vx - beamspot.x0) * electron.py - (electron.vy - beamspot.y0) * electron.px) / electron.pt) / (hypot(electron.gsfTrack.d0Error, hypot(beamspot.x0Error, beamspot.y0Error)))"
electronD0WRTPV       = "((electron.vx - eventvariable.leadingPV_x) * electron.py - (electron.vy - eventvariable.leadingPV_y) * electron.px) / electron.pt"

muonD0WRTBeamspot = "((muon.vx - beamspot.x0) * muon.py - (muon.vy - beamspot.y0) * muon.px) / muon.pt"
muonD0WRTBeamspotErr = "hypot(muon.innerTrack.d0Error, hypot(beamspot.x0Error, beamspot.y0Error))"
muonD0WRTBeamspotSig = "(((muon.vx - beamspot.x0) * muon.py - (muon.vy - beamspot.y0) * muon.px) / muon.pt) / (hypot(muon.innerTrack.d0Error, hypot(beamspot.x0Error, beamspot.y0Error)))"
muonD0WRTPV       = "((muon.vx - eventvariable.leadingPV_x) * muon.py - (muon.vy - eventvariable.leadingPV_y) * muon.px) / muon.pt"


# Calculation from https://github.com/cms-sw/cmssw/blob/CMSSW_7_4_X/DataFormats/TrackReco/interface/TrackBase.h#L674
electronDZWRTBeamspot = "(electron.vz - beamspot.z0) \
- ((electron.vx - beamspot.x0) * electron.px + (electron.vy - beamspot.y0) * electron.py) / electron.pt * electron.pz / electron.pt"
muonDZWRTBeamspot = "(muon.vz - beamspot.z0) \
- ((muon.vx - beamspot.x0) * muon.px + (muon.vy - beamspot.y0) * muon.py) / muon.pt * muon.pz / muon.pt"

trackDZWRTPV       = "(track.vz - eventvariable.leadingPV_z) \
- ((track.vx - eventvariable.leadingPV_x) * track.px + (track.vy - eventvariable.leadingPV_y) * track.py) / track.pt * track.pz / track.pt"
muonDZWRTPV       = "(muon.vz - eventvariable.leadingPV_z) \
- ((muon.vx - eventvariable.leadingPV_x) * muon.px + (muon.vy - eventvariable.leadingPV_y) * muon.py) / muon.pt * muon.pz / muon.pt"
electronDZWRTPV       = "(electron.vz - eventvariable.leadingPV_z) \
- ((electron.vx - eventvariable.leadingPV_x) * electron.px + (electron.vy - eventvariable.leadingPV_y) * electron.py) / electron.pt * electron.pz / electron.pt"

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
