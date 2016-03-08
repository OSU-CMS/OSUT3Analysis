#!/usr/bin/env python
# Utilities for applying cuts.  

import FWCore.ParameterSet.Config as cms

##################################################
## Functions for adding, removing cuts
##################################################
def addCuts(cutVPset, cutsToAdd):
    for cut in cutsToAdd:
        cutVPset.append(cut)

def removeCuts(cutVPset, cutsToRemove):
    for cut in cutsToRemove:
        for i in xrange(len(cutVPset) - 1, -1, -1):  # iterate backwards to avoid error
            if cutVPset[i].cutString == cut.cutString:
                del cutVPset[i]


##################################################
## Variable string definitions   
## These may be used in histogram or cut definitions. 
## Example:
## cutString = cms.string("fabs ( " + trackD0WRTBeamspot + " ) < 0.1"),
##################################################
trackD0WRTBeamspot = "((track.vx - beamspot.x0) * track.py - (track.vy - beamspot.y0) * track.px) / track.pt" 

muonPFIsolation = "(pfIsolationR04_.sumChargedHadronPt \
+ max(0.0, pfIsolationR04_.sumNeutralHadronEt + pfIsolationR04_.sumPhotonEt - 0.5*pfIsolationR04_.sumPUPt )) / pt" 
# See https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideMuonIdRun2#Muon_Isolation



##################################################
## Common cut definitions 
##################################################
cutMuonPFIsoTight = cms.PSet (
    inputCollection = cms.vstring("muons"),
    cutString = cms.string(muonPFIsolation + " < 0.15"), 
    numberRequired = cms.string(">= 1"),
    alias = cms.string("muon tight PF isolation")
) 






