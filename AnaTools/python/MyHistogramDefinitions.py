import FWCore.ParameterSet.Config as cms


###############################################
##### Set up the histograms to be plotted #####
###############################################


MyMuonHistograms = cms.PSet(
    inputCollection = cms.string("muons"),
    # this vector of parameter sets will contain all the muon histogram definitions
    histograms = cms.VPSet (
        # this parameter set will contain the muon pt histogram
        cms.PSet (

        ),
        # this parameter set will contain the muon eta-phi histogram
        cms.PSet (

        ),
    )
)

