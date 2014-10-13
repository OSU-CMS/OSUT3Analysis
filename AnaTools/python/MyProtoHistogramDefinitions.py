import FWCore.ParameterSet.Config as cms


###############################################
##### Set up the histograms to be plotted #####
###############################################


MyProtoHistograms = cms.PSet(
    # using "events" tells the code to pass the full event
    # to the special module to calculate the final quantity
    inputCollection = cms.string("events"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("dimuonMetDeltaPhi"),
            title = cms.string("#Delta(#phi) of the dimuon-MET system; dimuon-MET #Delta(#phi)"),
            binsX = cms.untracked.vdouble(100, -3.15, 3.15),
            inputVariables = cms.vstring("dimuonMetDeltaPhi"),
        ),
    )
)

