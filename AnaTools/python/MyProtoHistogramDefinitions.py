import FWCore.ParameterSet.Config as cms


###############################################
##### Set up the histograms to be plotted #####
###############################################


MyProtoHistograms = cms.PSet(
    # using "events" tells the code to pass the full event
    # to the special module to calculate the final quantity
    inputCollection = cms.string("userVariables"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("dimuonMetDeltaPhi"),
            title = cms.string("#Delta(#phi) of the dimuon-MET system; dimuon-MET #Delta(#phi)"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("dimuonMetDeltaPhi"),
        ),
        cms.PSet (
            name = cms.string("metPt"),
            title = cms.string("MET;MET [GeV];"),
            binsX = cms.untracked.vdouble(100, 0, 500),  
            inputVariables = cms.vstring("metPt"),
        ),
        cms.PSet (
            name = cms.string("diMuonInvMass"),
            title = cms.string("Di-muon Invariant Mass; M_{#mu#mu} [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("dimuonInvMass"),
        ),
    )
)

