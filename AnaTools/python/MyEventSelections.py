import FWCore.ParameterSet.Config as cms


################################################
##### Empty event selection to be expanded #####
##### through the OSUT3Analysis Tutorial   #####
################################################

MyFirstSelection = cms.PSet(
    name = cms.string("MyFirstSelection"),
    # this vector of parameter sets will contain the cut definitions
    cuts = cms.VPSet (
        # this parameter set will contain the cut of muon pt > 25 GeV
        cms.PSet (

        ),
    )
)


