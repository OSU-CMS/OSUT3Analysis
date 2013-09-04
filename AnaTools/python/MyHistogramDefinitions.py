import FWCore.ParameterSet.Config as cms


###############################################
##### Set up the histograms to be plotted #####
###############################################


ExtraElectronHistograms = cms.PSet(
    inputCollection = cms.string("electrons"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("electronPtVariableBins"),
            title = cms.string("Electron Transverse Momentum; p_{T} [GeV]"),
            variableBinsX = cms.untracked.vdouble(20.0, 30.889, 47.707, 73.681, 113.796, 175.753, 271.442, 419.229, 647.479, 1000.0),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("electronNonTrigMVAvsD0"),
            title = cms.string("electron non-triggering MVA output vs. d0; d0 [cm];MVA output"),
            bins = cms.untracked.vdouble(1000,-1,1,1000,-1,1),
            inputVariables = cms.vstring("correctedD0Vertex","mvaNonTrigV0"),
        ),
        cms.PSet (
            name = cms.string("electronTrigMVAvsD0"),
            title = cms.string("electron triggering MVA output vs. d0; d0 [cm];MVA output"),
            bins = cms.untracked.vdouble(1000,-1,1,1000,-1,1),
            inputVariables = cms.vstring("correctedD0Vertex","mvaTrigV0"),
        ),
        cms.PSet (
            name = cms.string("electronDzvsD0"),
            title = cms.string("electron dz vs. d0; d0 [cm]; dz [cm]"),
            bins = cms.untracked.vdouble(1000,-5,5,1000,-20,20),
            inputVariables = cms.vstring("correctedD0Vertex","correctedDZ"),
        ),

    )
)

ExtraMuonHistograms = cms.PSet(
    inputCollection = cms.string("muons"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("muonPtVariableBins"),
            title = cms.string("Muon Transverse Momentum; p_{T} [GeV]"),
            variableBinsX = cms.untracked.vdouble(20.0, 30.889, 47.707, 73.681, 113.796, 175.753, 271.442, 419.229, 647.479, 1000.0),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("muonDzvsD0"),
            title = cms.string("muon dz vs. d0; d0 [cm]; dz [cm]"),
            bins = cms.untracked.vdouble(1000,-5,5,1000,-20,20),
            inputVariables = cms.vstring("correctedD0Vertex","correctedDZ"),
        ),
    )
)
