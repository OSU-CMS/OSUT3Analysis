import FWCore.ParameterSet.Config as cms



###############################################
##### Set up the histograms to be plotted #####
###############################################


##############################################################################################

MuonHistograms = cms.PSet(
    inputCollection = cms.string("muons"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("muonPt"),
            title = cms.string("Muon Transverse Momentum; p_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("muonEta"),
            title = cms.string("Muon Eta; #eta"),
            bins = cms.untracked.vdouble(100, -3, 3),
            inputVariables = cms.vstring("eta"),
        ),
        cms.PSet (
            name = cms.string("muonGenEta"),
            title = cms.string("Muon Gen. Eta; #eta"),
            bins = cms.untracked.vdouble(100, -3, 3),
            inputVariables = cms.vstring("genEta"),
        ),
        cms.PSet (
            name = cms.string("muonPhi"),
            title = cms.string("Muon Phi; #phi"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15),
            inputVariables = cms.vstring("phi"),
        ),
        cms.PSet (
            name = cms.string("muonCharge"),
            title = cms.string("Muon Charge; charge"),
            bins = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("charge"),
        ),
        cms.PSet (
            name = cms.string("muonEtaPhi"),
            title = cms.string("Muon Eta vs. Phi; #phi; #eta"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15, 100, -3, 3),
            inputVariables = cms.vstring("phi","eta"),
        ),
         cms.PSet (
            name = cms.string("muonsIsoD0"),
            title = cms.string("Muon Iso vs. d_{0}; D0; Iso"),
            bins = cms.untracked.vdouble(50, 0, 0.05, 150, 0, 3),
            inputVariables = cms.vstring("abs(correctedD0)","relPFdBetaIso"),
        ),
        cms.PSet (
            name = cms.string("muonD0Vertex"),
            title = cms.string("Muon d_{0} wrt PV; d_{0} [cm]"),
            bins = cms.untracked.vdouble(100, -0.02, 0.02),
            inputVariables = cms.vstring("correctedD0Vertex"),
        ),
        cms.PSet (
            name = cms.string("muonD0Beamspot"),
            title = cms.string("Muon d_{0} wrt Beamspot; d_{0} [cm]"),
            bins = cms.untracked.vdouble(100, -0.02, 0.02),
            inputVariables = cms.vstring("correctedD0"),
        ),
        cms.PSet (
            name = cms.string("muonD0VertexPhi"),
            title = cms.string("Muon d_{0} wrt Vertex vs. Phi; #phi; d_{0} [cm]"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15, 100, -0.02, 0.02),
            inputVariables = cms.vstring("phi","correctedD0Vertex"),
        ),
        cms.PSet (
            name = cms.string("muonAbsD0Vertex"),
            title = cms.string("Muon |d_{0}| wrt Vertex; |d_{0}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.02),
            inputVariables = cms.vstring("abs(correctedD0Vertex)")
        ),
        cms.PSet (
            name = cms.string("muonAbsD0Beamspot"),
            title = cms.string("Muon |d_{0}| wrt Beamspot; |d_{0}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.02),
            inputVariables = cms.vstring("abs(correctedD0)")
        ),
        cms.PSet (
            name = cms.string("muonD0VertexSig"),
            title = cms.string("Muon d_{0} Significance; d_{0} / #sigma_{d_{0}}"),
            bins = cms.untracked.vdouble(100, -10.0, 10.0),
            inputVariables = cms.vstring("correctedD0VertexSig"),
        ),
        cms.PSet (
            name = cms.string("muonAbsD0VertexSig"),
            title = cms.string("Muon d_{0} Significance; |d_{0}| / #sigma_{d_{0}}"),
            bins = cms.untracked.vdouble(100, 0, 10.0),
            inputVariables = cms.vstring("abs(correctedD0VertexSig)"),
        ),
        cms.PSet (
            name = cms.string("muonDz"),
            title = cms.string("Muon d_{z}; d_{z} [cm]"),
            bins = cms.untracked.vdouble(100, -0.5, 0.5),
            inputVariables = cms.vstring("correctedDZ"),
        ),
        cms.PSet (
            name = cms.string("muonAbsDz"),
            title = cms.string("Muon |d_{z}|; |d_{z}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.5),
            inputVariables = cms.vstring("abs(correctedDZ)"),
        ),
        cms.PSet (
            name = cms.string("muonAbsD0BeamspotVsAbsDz"),
            title = cms.string("Muon |d_{0}| wrt Beamspot vs. Muon |d_{z}|; |d_{z}| [cm]; d_{0} [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.5, 100, 0, 0.02),
            inputVariables = cms.vstring("abs(correctedDZ)","abs(correctedD0)"),
        ),
        cms.PSet (
            name = cms.string("muonDetIso"),
            title = cms.string("Muon Detector-based Isolation; rel. iso."),
            bins = cms.untracked.vdouble(100, 0, 3),
            inputVariables = cms.vstring("detIso"),
        ),
        cms.PSet (
            name = cms.string("muonPFdBetaIso"),
            title = cms.string("Muon PF-based #Delta#beta-corrected Isolation; rel. iso."),
            bins = cms.untracked.vdouble(150, 0, 3),
            inputVariables = cms.vstring("relPFdBetaIso"),
        ),
        cms.PSet (
            name = cms.string("muonMetMt"),
            title = cms.string("Transverse Mass of Muon-MET System; M_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 200),
            inputVariables = cms.vstring("metMT"),
        ),
    )
)
##########################################################################
PhotonHistograms = cms.PSet(
    inputCollection = cms.string("photons"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("photonPt"),
            title = cms.string("Photon Transverse Momentum; p_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("photonEta"),
            title = cms.string("Photon Eta; #eta"),
            bins = cms.untracked.vdouble(100, -3, 3),
            inputVariables = cms.vstring("eta"),
        ),
        cms.PSet (
            name = cms.string("photonGenEta"),
            title = cms.string("Photon Gen. Eta; #eta"),
            bins = cms.untracked.vdouble(100, -3, 3),
            inputVariables = cms.vstring("genEta"),
        ),
        cms.PSet (
            name = cms.string("photonPhi"),
            title = cms.string("Photon Phi; #phi"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15),
            inputVariables = cms.vstring("phi"),
        ),
        cms.PSet (
            name = cms.string("photonEtaPhi"),
            title = cms.string("Photon Eta vs. Phi; #phi; #eta"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15, 100, -3, 3),
            inputVariables = cms.vstring("phi","eta"),
        ),
    )
)
##############################################################################################

SecondaryMuonHistograms = cms.PSet(
    inputCollection = cms.string("secondary muons"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("secondaryMuonPt"),
            title = cms.string("Secondary Muon Transverse Momentum; p_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("secondaryMuonEta"),
            title = cms.string("Secondary Muon Eta; #eta"),
            bins = cms.untracked.vdouble(100, -3, 3),
            inputVariables = cms.vstring("eta"),
        ),
        cms.PSet (
            name = cms.string("secondaryMuonGenEta"),
            title = cms.string("Secondary Muon Gen. Eta; #eta"),
            bins = cms.untracked.vdouble(100, -3, 3),
            inputVariables = cms.vstring("genEta"),
        ),
        cms.PSet (
            name = cms.string("secondaryMuonPhi"),
            title = cms.string("Secondary Muon Phi; #phi"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15),
            inputVariables = cms.vstring("phi"),
        ),
        cms.PSet (
            name = cms.string("secondaryMuonCharge"),
            title = cms.string("Secondary Muon Charge; charge"),
            bins = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("charge"),
        ),
        cms.PSet (
            name = cms.string("secondaryMuonEtaPhi"),
            title = cms.string("Secondary Muon Eta vs. Phi; #phi; #eta"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15, 100, -3, 3),
            inputVariables = cms.vstring("phi","eta"),
        ),
        cms.PSet (
            name = cms.string("secondaryMuonD0Vertex"),
            title = cms.string("Secondary Muon d_{0} wrt PV; d_{0} [cm]"),
            bins = cms.untracked.vdouble(100, -0.02, 0.02),
            inputVariables = cms.vstring("correctedD0Vertex"),
        ),
        cms.PSet (
            name = cms.string("secondaryMuonD0Beamspot"),
            title = cms.string("Secondary Muon d_{0} wrt Beamspot; d_{0} [cm]"),
            bins = cms.untracked.vdouble(100, -0.02, 0.02),
            inputVariables = cms.vstring("correctedD0"),
        ),
        cms.PSet (
            name = cms.string("secondaryMuonD0VertexPhi"),
            title = cms.string("Secondary Muon d_{0} wrt Vertex vs. Phi; #phi; d_{0} [cm]"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15, 100, -0.02, 0.02),
            inputVariables = cms.vstring("phi","correctedD0Vertex"),
        ),
        cms.PSet (
            name = cms.string("secondaryMuonAbsD0Vertex"),
            title = cms.string("Secondary Muon |d_{0}| wrt Vertex; |d_{0}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.02),
            inputVariables = cms.vstring("abs(correctedD0Vertex)")
        ),
        cms.PSet (
            name = cms.string("secondaryMuonAbsD0Beamspot"),
            title = cms.string("Secondary Muon |d_{0}| wrt Beamspot; |d_{0}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.02),
            inputVariables = cms.vstring("abs(correctedD0)")
        ),
        cms.PSet (
            name = cms.string("secondaryMuonD0VertexSig"),
            title = cms.string("Secondary Muon d_{0} Significance; d_{0} / #sigma_{d_{0}}"),
            bins = cms.untracked.vdouble(100, -2, 2),
            inputVariables = cms.vstring("correctedD0VertexSig"),
        ),
        cms.PSet (
            name = cms.string("secondaryMuonAbsD0VertexSig"),
            title = cms.string("Secondary Muon d_{0} Significance; |d_{0}| / #sigma_{d_{0}}"),
            bins = cms.untracked.vdouble(100, 0, 10),
            inputVariables = cms.vstring("abs(correctedD0VertexSig)"),
        ),
        cms.PSet (
            name = cms.string("secondaryMuonDz"),
            title = cms.string("Secondary Muon d_{z}; d_{z} [cm]"),
            bins = cms.untracked.vdouble(100, -0.5, 0.5),
            inputVariables = cms.vstring("correctedDZ"),
        ),
        cms.PSet (
            name = cms.string("secondaryMuonAbsDz"),
            title = cms.string("Secondary Muon |d_{z}|; |d_{z}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.5),
            inputVariables = cms.vstring("abs(correctedDZ)"),
        ),
        cms.PSet (
            name = cms.string("secondaryMuonAbsD0BeamspotVsAbsDz"),
            title = cms.string("Secondary Muon |d_{0}| wrt Beamspot vs. Secondary Muon |d_{z}|; |d_{z}| [cm]; d_{0} [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.5, 100, 0, 0.02),
            inputVariables = cms.vstring("abs(correctedDZ)","abs(correctedD0)"),
        ),
        cms.PSet (
            name = cms.string("secondaryMuonDetIso"),
            title = cms.string("Secondary Muon Detector-based Isolation; rel. iso."),
            bins = cms.untracked.vdouble(100, 0, 3),
            inputVariables = cms.vstring("detIso"),
        ),
        cms.PSet (
            name = cms.string("secondaryMuonPFdBetaIso"),
            title = cms.string("Secondary Muon PF-based #Delta#beta-corrected Isolation; rel. iso."),
            bins = cms.untracked.vdouble(100, 0, 3),
            inputVariables = cms.vstring("relPFdBetaIso"),
        ),
        cms.PSet (
            name = cms.string("secondaryMuonMetMt"),
            title = cms.string("Transverse Mass of Muon-MET System; M_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 200),
            inputVariables = cms.vstring("metMT"),
        ),
    )
)

##############################################################################################

DiMuonHistograms = cms.PSet(
    inputCollection = cms.string("muon-muon pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("diMuonInvMass"),
            title = cms.string("Di-muon Invariant Mass; M_{#mu#mu} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass"),
        ),
        cms.PSet (
            name = cms.string("diMuonChargeProduct"),
            title = cms.string("Di-muon Charge Product; charge_{#mu}_{1}*charge_{#mu}_{2}"),
            bins = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("chargeProduct"),
        ),
        cms.PSet (
            name = cms.string("diMuonDeltaPhi"),
            title = cms.string("Di-muon Phi Difference; |#Delta(#phi)|"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
        ),
        cms.PSet (
            name = cms.string("diMuonDeltaEta"),
            title = cms.string("Di-muon Eta Difference; |#Delta(#eta)|"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaEta"),
        ),
        cms.PSet (
            name = cms.string("diMuonDeltaR"),
            title = cms.string("Di-muon #DeltaR; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR"),
        ),
        cms.PSet (
            name = cms.string("diMuonThreeDAngle"),
            title = cms.string("Di-muon 3D angle; 3D angle"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("threeDAngle"),
        ),
        cms.PSet (
            name = cms.string("muon1D0vsMuon2D0"),
            title = cms.string("Muon #1 d_{0} vs. Muon #2 d_{0}; d_{0} [cm]; d_{0} [cm]"),
            bins = cms.untracked.vdouble(100, -0.02, 0.02, 100, -0.02, 0.02),
            inputVariables = cms.vstring("muon2CorrectedD0Vertex","muon1CorrectedD0Vertex"),
        ),
        cms.PSet (
            name = cms.string("muon1AbsD0vsMuon2AbsD0"),
            title = cms.string("Muon #1 |d_{0}| vs. Muon #2 |d_{0}|; |d_{0}| [cm]; |d_{0}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.02, 100, 0, 0.02),
            inputVariables = cms.vstring("abs(muon2CorrectedD0Vertex)","abs(muon1CorrectedD0Vertex)"),
        ),
    )
)

##############################################################################################

MuonSecondaryMuonHistograms = cms.PSet(
    inputCollection = cms.string("muon-secondary muon pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("muonSecondaryMuonInvMass"),
            title = cms.string("Muon-secondary Muon Invariant Mass; M_{#mu#mu} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass"),
        ),
        cms.PSet (
            name = cms.string("muonSecondaryMuonChargeProduct"),
            title = cms.string("Muon-secondary Muon Charge Product; charge_{#mu}_{1}*charge_{#mu}_{2}"),
            bins = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("chargeProduct"),
        ),
        cms.PSet (
            name = cms.string("muonSecondaryMuonDeltaPhi"),
            title = cms.string("Muon-secondary Muon Phi Difference; |#Delta(#phi)|"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
        ),
        cms.PSet (
            name = cms.string("muonSecondaryMuonDeltaEta"),
            title = cms.string("Muon-secondary Muon Eta Difference; |#Delta(#eta)|"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaEta"),
        ),
        cms.PSet (
            name = cms.string("muonSecondaryMuonDeltaR"),
            title = cms.string("Muon-secondary Muon #DeltaR; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR"),
        ),
        cms.PSet (
            name = cms.string("muonSecondaryMuonThreeDAngle"),
            title = cms.string("Muon-secondary Muon 3D angle; 3D angle"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("threeDAngle"),
        ),
        cms.PSet (
            name = cms.string("muonD0vsSecondaryMuonD0"),
            title = cms.string("Muon d_{0} vs. Secondary Muon d_{0}; d_{0} [cm]; d_{0} [cm]"),
            bins = cms.untracked.vdouble(100, -0.02, 0.02, 100, -0.02, 0.02),
            inputVariables = cms.vstring("muon2CorrectedD0Vertex","muon1CorrectedD0Vertex"),
        ),
        cms.PSet (
            name = cms.string("muonAbsD0vsSecondaryMuonAbsD0"),
            title = cms.string("Muon |d_{0}| vs. Secondary Muon |d_{0}|; |d_{0}| [cm]; |d_{0}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.02, 100, 0, 0.02),
            inputVariables = cms.vstring("abs(muon2CorrectedD0Vertex)","abs(muon1CorrectedD0Vertex)"),
        ),
    )
)

##############################################################################################

MuonTauHistograms = cms.PSet(
    inputCollection = cms.string("muon-tau pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("MuonTauInvMass"),
            title = cms.string("Muon-Tau Invariant Mass; M_{#mu#tau} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 180),
            inputVariables = cms.vstring("invMass"),
            ),
        cms.PSet (
            name = cms.string("MuonTauDeltaEta"),
            title = cms.string("Muon-Tau Eta Difference; |#Delta(#eta)|"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaEta"),
            ),
        cms.PSet (
            name = cms.string("MuonTauDeltaPhi"),
            title = cms.string("Muon-Tau Phi Difference; |#Delta(#phi)|"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
            ),
        cms.PSet (
            name = cms.string("MuonTauDeltaR"),
            title = cms.string("Muon-Tau #DeltaR; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR"),
            ),
        )
    )

##############################################################################################

ElectronHistograms = cms.PSet(
    inputCollection = cms.string("electrons"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("electronPt"),
            title = cms.string("Electron Transverse Momentum; p_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("electronEta"),
            title = cms.string("Electron Eta; #eta"),
            bins = cms.untracked.vdouble(100, -3, 3),
            inputVariables = cms.vstring("eta"),
        ),
        cms.PSet (
            name = cms.string("electronPhi"),
            title = cms.string("Electron Phi; #phi"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15),
            inputVariables = cms.vstring("phi"),
        ),
        cms.PSet (
            name = cms.string("electronCharge"),
            title = cms.string("Electron Charge; charge"),
            bins = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("charge"),
        ),
        cms.PSet (
            name = cms.string("electronEtaPhi"),
            title = cms.string("Electron Eta vs. Phi; #phi; #eta"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15, 100, -3, 3),
            inputVariables = cms.vstring("phi","eta"),
        ),
        cms.PSet (
            name = cms.string("electronD0Vertex"),
            title = cms.string("Electron d_{0} wrt PV; d_{0} [cm]"),
            bins = cms.untracked.vdouble(100, -0.02, 0.02),
            inputVariables = cms.vstring("correctedD0Vertex"),
        ),
        cms.PSet (
            name = cms.string("electronD0Beamspot"),
            title = cms.string("Electron d_{0} wrt Beamspot; d_{0} [cm]"),
            bins = cms.untracked.vdouble(100, -0.02, 0.02),
            inputVariables = cms.vstring("correctedD0"),
        ),
        cms.PSet (
            name = cms.string("electronD0VertexPhi"),
            title = cms.string("Electron d_{0} wrt Vertex vs. Phi; #phi; d_{0} [cm]"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15, 100, -0.02, 0.02),
            inputVariables = cms.vstring("phi","correctedD0Vertex"),
        ),
        cms.PSet (
            name = cms.string("electronAbsD0Vertex"),
            title = cms.string("Electron |d_{0}| wrt Vertex; |d_{0}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.02),
            inputVariables = cms.vstring("abs(correctedD0Vertex)"),
        ),
        cms.PSet (
            name = cms.string("electronAbsD0Beamspot"),
            title = cms.string("Electron |d_{0}| wrt Beamspot; |d_{0}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.02),
            inputVariables = cms.vstring("abs(correctedD0)"),
        ),
        cms.PSet (
            name = cms.string("electronD0VertexSig"),
            title = cms.string("Electron d_{0} Significance; d_{0} / #sigma_{d_{0}}"),
            bins = cms.untracked.vdouble(100, -2, 2),
            inputVariables = cms.vstring("correctedD0VertexSig"),
        ),
        cms.PSet (
            name = cms.string("electronAbsD0VertexSig"),
            title = cms.string("Electron d_{0} Significance; |d_{0}| / #sigma_{d_{0}}"),
            bins = cms.untracked.vdouble(100, 0, 10),
            inputVariables = cms.vstring("abs(correctedD0VertexSig)"),
        ),
        cms.PSet (
            name = cms.string("electronDz"),
            title = cms.string("Electron d_{z}; d_{z} [cm]"),
            bins = cms.untracked.vdouble(100, -0.5, 0.5),
            inputVariables = cms.vstring("correctedDZ"),
        ),
        cms.PSet (
            name = cms.string("electronAbsDz"),
            title = cms.string("Electron |d_{z}|; |d_{z}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.5),
            inputVariables = cms.vstring("abs(correctedDZ)"),
        ),
        cms.PSet (
            name = cms.string("electronAbsD0BeamspotVsAbsDz"),
            title = cms.string("Electron |d_{0}| wrt Beamspot vs. Electron |d_{z}|; |d_{z}| [cm]; d_{0} [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.5, 100, 0, 0.02),
            inputVariables = cms.vstring("abs(correctedDZ)","abs(correctedD0)"),
        ),
        cms.PSet (
            name = cms.string("electronDetIso"),
            title = cms.string("Electron Detector-based Isolation; rel. iso."),
            bins = cms.untracked.vdouble(100, 0, 3),
            inputVariables = cms.vstring("detIso"),
        ),
        cms.PSet (
            name = cms.string("electronPFrhoIso"),
            title = cms.string("Electron PF-based #rho-corrected Isolation; rel. iso."),
            bins = cms.untracked.vdouble(150, 0, 3),
            inputVariables = cms.vstring("relPFrhoIso"),
        ),
        cms.PSet (
            name = cms.string("electronFbrem"),
            title = cms.string("Electron Brem. Energy Fraction; fbrem"),
            bins = cms.untracked.vdouble(100, 0, 2),
            inputVariables = cms.vstring("fbrem"),
        ),
        cms.PSet (
            name = cms.string("electronMvaTrigV0"),
            title = cms.string("Electron ID Triggering MVA Output"),
            bins = cms.untracked.vdouble(100, -1.1, 1.1),
            inputVariables = cms.vstring("mvaTrigV0"),
        ),
        cms.PSet (
            name = cms.string("electronMvaNonTrigV0"),
            title = cms.string("Electron ID Non-triggering MVA Output"),
            bins = cms.untracked.vdouble(100, -1.1, 1.1),
            inputVariables = cms.vstring("mvaNonTrigV0"),
        ),
        cms.PSet (
            name = cms.string("electronMetMt"),
            title = cms.string("Transverse Mass of Electron-MET System; M_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 200),
            inputVariables = cms.vstring("metMT"),
        ),
        cms.PSet (
            name = cms.string("electronNumberOfLostHits"),
            title = cms.string("Electon Number of Lost Hits; # lost hits"),
            bins = cms.untracked.vdouble(10, 0, 10),
            inputVariables = cms.vstring("numberOfLostHits"),
        ),
        cms.PSet (
            name = cms.string("electronNumberOfExpectedInnerHits"),
            title = cms.string("Electron Number of Expected Inner Hits; # exp. inner hits"),
            bins = cms.untracked.vdouble(10, 0, 10),
            inputVariables = cms.vstring("numberOfExpectedInnerHits"),
        ),
    )
)

##############################################################################################

SecondaryElectronHistograms = cms.PSet(
    inputCollection = cms.string("secondary electrons"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("secondaryElectronPt"),
            title = cms.string("Secondary Electron Transverse Momentum; p_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("secondaryElectronEta"),
            title = cms.string("Secondary Electron Eta; #eta"),
            bins = cms.untracked.vdouble(100, -3, 3),
            inputVariables = cms.vstring("eta"),
        ),
        cms.PSet (
            name = cms.string("secondaryElectronGenEta"),
            title = cms.string("Secondary Electron Gen. Eta; #eta"),
            bins = cms.untracked.vdouble(100, -3, 3),
            inputVariables = cms.vstring("genEta"),
        ),
        cms.PSet (
            name = cms.string("secondaryElectronPhi"),
            title = cms.string("Secondary Electron Phi; #phi"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15),
            inputVariables = cms.vstring("phi"),
        ),
        cms.PSet (
            name = cms.string("secondaryElectronCharge"),
            title = cms.string("Secondary Electron Charge; charge"),
            bins = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("charge"),
        ),
        cms.PSet (
            name = cms.string("secondaryElectronEtaPhi"),
            title = cms.string("Secondary Electron Eta vs. Phi; #phi; #eta"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15, 100, -3, 3),
            inputVariables = cms.vstring("phi","eta"),
        ),
        cms.PSet (
            name = cms.string("secondaryElectronD0Vertex"),
            title = cms.string("Secondary Electron d_{0} wrt PV; d_{0} [cm]"),
            bins = cms.untracked.vdouble(100, -0.02, 0.02),
            inputVariables = cms.vstring("correctedD0Vertex"),
        ),
        cms.PSet (
            name = cms.string("secondaryElectronD0Beamspot"),
            title = cms.string("Secondary Electron d_{0} wrt Beamspot; d_{0} [cm]"),
            bins = cms.untracked.vdouble(100, -0.02, 0.02),
            inputVariables = cms.vstring("correctedD0"),
        ),
        cms.PSet (
            name = cms.string("secondaryElectronD0VertexPhi"),
            title = cms.string("Secondary Electron d_{0} wrt Vertex vs. Phi; #phi; d_{0} [cm]"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15, 100, -0.02, 0.02),
            inputVariables = cms.vstring("phi","correctedD0Vertex"),
        ),
        cms.PSet (
            name = cms.string("secondaryElectronAbsD0Vertex"),
            title = cms.string("Secondary Electron |d_{0}| wrt Vertex; |d_{0}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.02),
            inputVariables = cms.vstring("abs(correctedD0Vertex)")
        ),
        cms.PSet (
            name = cms.string("secondaryElectronAbsD0Beamspot"),
            title = cms.string("Secondary Electron |d_{0}| wrt Beamspot; |d_{0}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.02),
            inputVariables = cms.vstring("abs(correctedD0)")
        ),
        cms.PSet (
            name = cms.string("secondaryElectronD0VertexSig"),
            title = cms.string("Secondary Electron d_{0} Significance; d_{0} / #sigma_{d_{0}}"),
            bins = cms.untracked.vdouble(100, -2, 2),
            inputVariables = cms.vstring("correctedD0VertexSig"),
        ),
        cms.PSet (
            name = cms.string("secondaryElectronAbsD0VertexSig"),
            title = cms.string("Secondary Electron d_{0} Significance; |d_{0}| / #sigma_{d_{0}}"),
            bins = cms.untracked.vdouble(100, 0, 10),
            inputVariables = cms.vstring("abs(correctedD0VertexSig)"),
        ),
        cms.PSet (
            name = cms.string("secondaryElectronDz"),
            title = cms.string("Secondary Electron d_{z}; d_{z} [cm]"),
            bins = cms.untracked.vdouble(100, -0.5, 0.5),
            inputVariables = cms.vstring("correctedDZ"),
        ),
        cms.PSet (
            name = cms.string("secondaryElectronAbsDz"),
            title = cms.string("Secondary Electron |d_{z}|; |d_{z}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.5),
            inputVariables = cms.vstring("abs(correctedDZ)"),
        ),
        cms.PSet (
            name = cms.string("secondaryElectronAbsD0BeamspotVsAbsDz"),
            title = cms.string("Secondary Electron |d_{0}| wrt Beamspot vs. Secondary Electron |d_{z}|; |d_{z}| [cm]; d_{0} [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.5, 100, 0, 0.02),
            inputVariables = cms.vstring("abs(correctedDZ)","abs(correctedD0)"),
        ),
        cms.PSet (
            name = cms.string("secondaryElectronDetIso"),
            title = cms.string("Secondary Electron Detector-based Isolation; rel. iso."),
            bins = cms.untracked.vdouble(100, 0, 3),
            inputVariables = cms.vstring("detIso"),
        ),
        cms.PSet (
            name = cms.string("secondaryElectronPFrhoIso"),
            title = cms.string("Secondary Electron PF-based #rho-corrected Isolation; rel. iso."),
            bins = cms.untracked.vdouble(100, 0, 3),
            inputVariables = cms.vstring("relPFrhoIso"),
        ),
        cms.PSet (
            name = cms.string("secondaryElectronFbrem"),
            title = cms.string("Secondary Electron Brem. Energy Fraction; fbrem"),
            bins = cms.untracked.vdouble(100, 0, 2),
            inputVariables = cms.vstring("fbrem"),
        ),
        cms.PSet (
            name = cms.string("secondaryElectronMvaTrigV0"),
            title = cms.string("Secondary Electron ID Triggering MVA Output"),
            bins = cms.untracked.vdouble(100, -1.1, 1.1),
            inputVariables = cms.vstring("mvaTrigV0"),
        ),
        cms.PSet (
            name = cms.string("secondaryElectronMvaNonTrigV0"),
            title = cms.string("Secondary Electron ID Non-triggering MVA Output"),
            bins = cms.untracked.vdouble(100, -1.1, 1.1),
            inputVariables = cms.vstring("mvaNonTrigV0"),
        ),
        cms.PSet (
            name = cms.string("secondaryElectronMetMt"),
            title = cms.string("Transverse Mass of Electron-MET System; M_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 200),
            inputVariables = cms.vstring("metMT"),
        ),
    )
)

##############################################################################################

DiElectronHistograms = cms.PSet(
    inputCollection = cms.string("electron-electron pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("diElectronInvMass"),
            title = cms.string("Di-electron Invariant Mass; M_{ee} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass"),
        ),
        cms.PSet (
            name = cms.string("diElectronChargeProduct"),
            title = cms.string("Di-electron Charge Product; charge_{e}_{1}*charge_{e}_{2}"),
            bins = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("chargeProduct"),
        ),
        cms.PSet (
            name = cms.string("diElectronDeltaPhi"),
            title = cms.string("Di-electron Phi Difference; |#Delta(#phi)|"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
        ),
        cms.PSet (
            name = cms.string("diElectronDeltaEta"),
            title = cms.string("Di-electron Eta Difference; |#Delta(#eta)|"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaEta"),
        ),
        cms.PSet (
            name = cms.string("diElectronDeltaR"),
            title = cms.string("Di-electron #DeltaR; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR"),
        ),
        cms.PSet (
            name = cms.string("diElectronThreeDAngle"),
            title = cms.string("Di-electron 3D angle; 3D angle"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("threeDAngle"),
        ),        
        cms.PSet (
            name = cms.string("electron1D0vsElectron2D0"),
            title = cms.string("Electron #1 d_{0} vs. Electron #2 d_{0}; d_{0} [cm]; d_{0} [cm]"),
            bins = cms.untracked.vdouble(100, -0.02, 0.02, 100, -0.02, 0.02),
            inputVariables = cms.vstring("electron2CorrectedD0Vertex","electron1CorrectedD0Vertex"),
        ),
        cms.PSet (
            name = cms.string("electron1AbsD0vsElectron2AbsD0"),
            title = cms.string("Electron #1 |d_{0}| vs. Electron #2 |d_{0}|; |d_{0}| [cm]; |d_{0}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.02, 100, 0, 0.02),
            inputVariables = cms.vstring("abs(electron2CorrectedD0Vertex)","abs(electron1CorrectedD0Vertex)"),
        ),
    )
)

##############################################################################################

ElectronSecondaryElectronHistograms = cms.PSet(
    inputCollection = cms.string("electron-secondary electron pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("electronSecondaryElectronInvMass"),
            title = cms.string("Electron-secondary Electron Invariant Mass; M_{ee} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass"),
        ),
        cms.PSet (
            name = cms.string("electronSecondaryElectronChargeProduct"),
            title = cms.string("Electron-secondary Electron Charge Product; charge_{e}_{1}*charge_{e}_{2}"),
            bins = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("chargeProduct"),
        ),
        cms.PSet (
            name = cms.string("electronSecondaryElectronDeltaPhi"),
            title = cms.string("Electron-secondary Electron Phi Difference; |#Delta(#phi)|"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
        ),
        cms.PSet (
            name = cms.string("electronSecondaryElectronDeltaEta"),
            title = cms.string("Electron-secondary Electron Eta Difference; |#Delta(#eta)|"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaEta"),
        ),
        cms.PSet (
            name = cms.string("electronSecondaryElectronDeltaR"),
            title = cms.string("Electron-secondary Electron #DeltaR; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR"),
        ),
        cms.PSet (
            name = cms.string("electronSecondaryElectronThreeDAngle"),
            title = cms.string("Electron-secondary Electron 3D angle; 3D angle"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("threeDAngle"),
        ),
        cms.PSet (
            name = cms.string("electronD0vsSecondaryElectronD0"),
            title = cms.string("Electron d_{0} vs. Secondary Electron d_{0}; d_{0} [cm]; d_{0} [cm]"),
            bins = cms.untracked.vdouble(100, -0.02, 0.02, 100, -0.02, 0.02),
            inputVariables = cms.vstring("electron2CorrectedD0Vertex","electron1CorrectedD0Vertex"),
        ),
        cms.PSet (
            name = cms.string("electronAbsD0vsSecondaryElectronAbsD0"),
            title = cms.string("Electron |d_{0}| vs. Secondary Electron |d_{0}|; |d_{0}| [cm]; |d_{0}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.02, 100, 0, 0.02),
            inputVariables = cms.vstring("abs(electron2CorrectedD0Vertex)","abs(electron1CorrectedD0Vertex)"),
        ),

    )
)

##############################################################################################

ElectronMuonHistograms = cms.PSet(
    inputCollection = cms.string("electron-muon pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("electronMuonPt"),
            title = cms.string("Electron-muon Pair Transverse Momentum; p_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("electronMuonInvMass"),
            title = cms.string("Electron-muon Invariant Mass; M_{e#mu} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass"),
        ),
        cms.PSet (
            name = cms.string("electronMuonChargeProduct"),
            title = cms.string("Electron-muon Charge Product; charge_{e}*charge_{#mu}"),
            bins = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("chargeProduct"),
        ),
        cms.PSet (
            name = cms.string("electronMuonDeltaPhi"),
            title = cms.string("Electron-muon Phi Difference; |#Delta(#phi)|"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
        ),
        cms.PSet (
            name = cms.string("electronMuonDeltaEta"),
            title = cms.string("Electron-muon Eta Difference; |#Delta(#eta)|"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaEta"),
        ),
        cms.PSet (
            name = cms.string("electronMuonDeltaR"),
            title = cms.string("Electron-muon #DeltaR; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR"),
        ),
        cms.PSet (
            name = cms.string("electronMuonThreeDAngle"),
            title = cms.string("Electron-muon 3D angle; 3D angle"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("threeDAngle"),
        ),
        cms.PSet (
            name = cms.string("electronD0vsMuonD0"),
            title = cms.string("Electron d_{0} vs. Muon d_{0}; muon d_{0} [cm]; electron d_{0} [cm]"),
            bins = cms.untracked.vdouble(100, -0.02, 0.02, 100, -0.02, 0.02),
            inputVariables = cms.vstring("muonCorrectedD0Vertex","electronCorrectedD0Vertex"),
        ),
        cms.PSet (
            name = cms.string("electronAbsD0VertexVsMuonAbsD0Vertex"),
            title = cms.string("Electron |d_{0}| wrt Vertex vs. Muon |d_{0}| wrt Vertex; muon |d_{0}| [cm]; electron |d_{0}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.02, 100, 0, 0.02),
            inputVariables = cms.vstring("abs(muonCorrectedD0Vertex)","abs(electronCorrectedD0Vertex)"),
        ),
        cms.PSet (
            name = cms.string("electronAbsD0BeamspotVsMuonAbsD0Beamspot"),
            title = cms.string("Electron |d_{0}| wrt Beamspot vs. Muon |d_{0}| wrt Beamspot; muon |d_{0}| [cm]; electron |d_{0}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.02, 100, 0, 0.02),
            inputVariables = cms.vstring("abs(muonCorrectedD0)","abs(electronCorrectedD0)"),
        ),
        cms.PSet (
            name = cms.string("electronDetIsoVsMuonDetIso"),
            title = cms.string("Electron Detector-based Isolation vs. Muon Detector-based Isolation; muon rel. iso.; electron rel. iso."),
            bins = cms.untracked.vdouble(100, 0, 3, 100, 0, 3),
            inputVariables = cms.vstring("muonDetIso","electronDetIso"),
        ),
        cms.PSet (
            name = cms.string("electronPFrhoIsoVsMuonPFdBetaIso"),
            title = cms.string("Electron PF-based #rho-corrected Isolation vs. Muon PF-based #Delta#beta-corrected Isolation; muon rel. iso.; electron rel. iso."),
            bins = cms.untracked.vdouble(150, 0, 3, 150, 0, 3),
            inputVariables = cms.vstring("muonRelPFdBetaIso","electronRelPFrhoIso"),
        ),
    )
)

##############################################################################################

MuonPhotonHistograms = cms.PSet(
    inputCollection = cms.string("muon-photon pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("muonPhotonPt"),
            title = cms.string("Muon-photon Pair Transverse Momentum; p_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("muonPhotonInvMass"),
            title = cms.string("Muon-photon Pair Invariant Mass; M_{e#mu} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass"),
        ),
        cms.PSet (
            name = cms.string("muonPhotonDeltaPhi"),
            title = cms.string("Muon-photon Phi Difference; |#Delta(#phi)|"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
        ),
        cms.PSet (
            name = cms.string("muonPhotonDeltaEta"),
            title = cms.string("Muon-photon Eta Difference; |#Delta(#eta)|"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaEta"),
        ),
        cms.PSet (
            name = cms.string("muonPhotonDeltaR"),
            title = cms.string("Muon-photon #DeltaR; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR"),
        ),
        cms.PSet (
            name = cms.string("muonPhotonThreeDAngle"),
            title = cms.string("Muon-photon 3D angle; 3D angle"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("threeDAngle"),
        ),
        cms.PSet (
            name = cms.string("muonPFdBetaIsoVsMuon-photonDeltaR"),
            title = cms.string("Muon PF-based #Delta#beta-corrected Isolation vs Muon-photon DeltaR;|muon-photon #Delta(R)|; muon rel. iso."),
            bins = cms.untracked.vdouble(100, 0, 6, 100, 0, 3),
            inputVariables = cms.vstring("deltaR","muonRelPFdBetaIso"),
        ),
        cms.PSet (
            name = cms.string("muonPFdBetaIsoVsMuon-photonDeltaEta"),
            title = cms.string("Muon PF-based #Delta#beta-corrected Isolation vs Muon-photon DeltaEta; |muon-photon #Delta(#eta)|; muon rel. iso."),
            bins = cms.untracked.vdouble(100, 0, 6, 100, 0, 3),
            inputVariables = cms.vstring("deltaEta","muonRelPFdBetaIso"),
        ),
        cms.PSet (
            name = cms.string("muonPFdBetaIsoVsMuon-photonDeltaPhi"),
            title = cms.string("Muon PF-based #Delta#beta-corrected Isolation vs Muon-photon DeltaPhi; |muon-photon #Delta(#phi)|; muon rel. iso."),
            bins = cms.untracked.vdouble(100, 0, 3.15, 100, 0, 3),
            inputVariables = cms.vstring("deltaPhi","muonRelPFdBetaIso"),
        ),
    )
)

##############################################################################################

ElectronPhotonHistograms = cms.PSet(
    inputCollection = cms.string("electron-photon pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("electronPhotonPt"),
            title = cms.string("Electron-photon Pair Transverse Momentum; p_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("electronPhotonInvMass"),
            title = cms.string("Electron-photon Pair Invariant Mass; M_{e#mu} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass"),
        ),
        cms.PSet (
            name = cms.string("electronPhotonDeltaPhi"),
            title = cms.string("Electron-photon Phi Difference; |#Delta(#phi)|"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
        ),
        cms.PSet (
            name = cms.string("electronPhotonDeltaEta"),
            title = cms.string("Electron-photon Eta Difference; |#Delta(#eta)|"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaEta"),
        ),
        cms.PSet (
            name = cms.string("electronPhotonDeltaR"),
            title = cms.string("Electron-photon #DeltaR; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR"),
        ),
        cms.PSet (
            name = cms.string("electronPhotonThreeDAngle"),
            title = cms.string("Electron-photon 3D angle; 3D angle"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("threeDAngle"),
        ),
        cms.PSet (
            name = cms.string("electronPFrhoIsoVsElectron-photonDeltaR"),
            title = cms.string("Electron PF-based #Delta#beta-corrected Isolation vs Electron-photon DeltaR;|electron-photon #Delta(R)|; electron rel. iso."),
            bins = cms.untracked.vdouble(100, 0, 6, 100, 0, 3),
            inputVariables = cms.vstring("deltaR","electronRelPFrhoIso"),
        ),
        cms.PSet (
            name = cms.string("electronPFrhoIsoVsElectron-photonDeltaEta"),
            title = cms.string("Electron PF-based #Delta#beta-corrected Isolation vs Electron-photon DeltaEta; |electron-photon #Delta(#eta)|; electron rel. iso."),
            bins = cms.untracked.vdouble(100, 0, 6, 100, 0, 3),
            inputVariables = cms.vstring("deltaEta","electronRelPFrhoIso"),
        ),
        cms.PSet (
            name = cms.string("electronPFrhoIsoVsElectron-photonDeltaPhi"),
            title = cms.string("Electron PF-based #Delta#beta-corrected Isolation vs Electron-photon DeltaPhi; |electron-photon #Delta(#phi)|; electron rel. iso."),
            bins = cms.untracked.vdouble(100, 0, 3.15, 100, 0, 3),
            inputVariables = cms.vstring("deltaPhi","electronRelPFrhoIso"),
        ),
    )
)
############################################################################################
TauHistograms = cms.PSet(
    inputCollection = cms.string("taus"),
    histograms = cms.VPSet (
         cms.PSet (
             name = cms.string("tauPt"),
             title = cms.string("Tau Transverse Momentum; p_{T} [GeV]"),
             bins = cms.untracked.vdouble(100, 0, 500),
             inputVariables = cms.vstring("pt"),
             ),
         cms.PSet (
             name = cms.string("tauEta"),
             title = cms.string("Tau Eta; #eta"),
             bins = cms.untracked.vdouble(100, -3, 3),
             inputVariables = cms.vstring("eta"),
             ),
         cms.PSet (
             name = cms.string("tauPhi"),
             title = cms.string("Tau Phi; #phi"),
             bins = cms.untracked.vdouble(100, -3.15, 3.15),
             inputVariables = cms.vstring("phi"),
             ),
         cms.PSet (
             name = cms.string("tauleadingTrackPt"),
             title = cms.string("Tau Leading Track Transverse Momentum; Leading Track p_{T} [GeV]"),
             bins = cms.untracked.vdouble(100, 0, 300),
             inputVariables = cms.vstring("leadingTrackPt"),
             ),
         cms.PSet (
             name = cms.string("tauEnergy"),
             title = cms.string("Tau Energy; Energy [GeV]"),
             bins  = cms.untracked.vdouble(100, 0, 300),
             inputVariables = cms.vstring("energy"),
             ),
         cms.PSet (
             name = cms.string("tauEMFraction"),
             title = cms.string("Tau EM Fraction; EM Fraction"),
             bins = cms.untracked.vdouble(100, 0, 1.5),
             inputVariables = cms.vstring("emFraction"),
             ),
         cms.PSet (
             name = cms.string("tauNumProngs"),
             title = cms.string("Tau Number of Prongs; # Prongs"),
             bins = cms.untracked.vdouble(100, -0.5, 4.5),
             inputVariables = cms.vstring("numProngs"),
             ),
         cms.PSet (
             name = cms.string("tauNumSignalGammas"),
             title = cms.string("Tau Number of Signal Gammas; # Signal Gammas"),
             bins = cms.untracked.vdouble(100, -0.5, 12.5),
             inputVariables = cms.vstring("numSignalGammas"),
             ),
         cms.PSet (
             name = cms.string("tauNumSignalNeutrals"),
             title = cms.string("Tau Number of Signal Neutrals; # Signal Neutrals"),
             bins = cms.untracked.vdouble(100, -0.5, 8.5),
             inputVariables = cms.vstring("numSignalNeutrals"),
             ),
         cms.PSet (
             name = cms.string("tauNumSignalPiZeros"),
             title = cms.string("Tau Number of Signal #pi^{0}'s; # Signal #pi^{0}'s"),
             bins = cms.untracked.vdouble(100, -0.5, 8.5),
             inputVariables = cms.vstring("numSignalPiZeros"),
             ),
         cms.PSet (
             name = cms.string("tauDecayMode"),
             title = cms.string("Tau Decay Mode;Decay Mode "),
             bins = cms.untracked.vdouble(100, -0.5, 12.5),
             inputVariables = cms.vstring("decayMode"),
             ),
         
         )
    )

##############################################################################################   

DiTauHistograms = cms.PSet(
    inputCollection = cms.string("tau-tau pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("diTauInvMass"),
            title = cms.string("Di-Tau Invariant Mass; M_{#tau#tau} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 180),
            inputVariables = cms.vstring("invMass"),
            ),
        cms.PSet (
            name = cms.string("diTauDeltaEta"),
            title = cms.string("Di-tau Eta Difference; |#Delta(#eta)|"),
            bins = cms.untracked.vdouble(100, 0, 10),
            inputVariables = cms.vstring("deltaEta"),
            ),
        cms.PSet (
            name = cms.string("diTauDeltaPhi"),
            title = cms.string("Di-tau Phi Difference; |#Delta(#phi)|"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
            ),
        cms.PSet (
            name = cms.string("diTauDeltaR"),
            title = cms.string("Di-tau #DeltaR; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 10),
            inputVariables = cms.vstring("deltaR"),
            ),

        )
    )

##############################################################################################   

JetHistograms = cms.PSet(
    inputCollection = cms.string("jets"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("jetPt"),
            title = cms.string("Jet Transverse Momentum; p_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("jetEta"),
            title = cms.string("Jet Eta; #eta"),
            bins = cms.untracked.vdouble(100, -3, 3),
            inputVariables = cms.vstring("eta"),
        ),
        cms.PSet (
            name = cms.string("jetPhi"),
            title = cms.string("Jet Phi; #phi"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15),
            inputVariables = cms.vstring("phi"),
        ),
        cms.PSet (
            name = cms.string("jetCharge"),
            title = cms.string("Jet Charge; charge"),
            bins = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("charge"),
        ),
        cms.PSet (
            name = cms.string("jetEtaPhi"),
            title = cms.string("Jet Eta vs. Phi; #phi; #eta"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15, 100, -3, 3),
            inputVariables = cms.vstring("phi","eta"),
        ),
        cms.PSet (
            name = cms.string("jetFlavor"),
            title = cms.string("Jet Flavor"),
            bins = cms.untracked.vdouble(42, -21, 21),
            inputVariables = cms.vstring("flavour"),
        ),
        cms.PSet (
            name = cms.string("jetCSV"),
            title = cms.string("Jet Combined Secondary Vertex B-tagging Discriminant"),
            bins = cms.untracked.vdouble(100, -1, 1),
            inputVariables = cms.vstring("btagCombinedSecVertex"),
        ),
    )
)

SecondaryJetHistograms = cms.PSet(
    inputCollection = cms.string("secondary jets"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("secondaryJetPt"),
            title = cms.string("Jet Transverse Momentum; p_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("secondaryJetEta"),
            title = cms.string("Jet Eta; #eta"),
            bins = cms.untracked.vdouble(100, -3, 3),
            inputVariables = cms.vstring("eta"),
        ),
        cms.PSet (
            name = cms.string("secondaryJetPhi"),
            title = cms.string("Jet Phi; #phi"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15),
            inputVariables = cms.vstring("phi"),
        ),
        cms.PSet (
            name = cms.string("secondaryJetCharge"),
            title = cms.string("Jet Charge; charge"),
            bins = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("charge"),
        ),
        cms.PSet (
            name = cms.string("secondaryJetEtaPhi"),
            title = cms.string("Jet Eta vs. Phi; #phi; #eta"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15, 100, -3, 3),
            inputVariables = cms.vstring("phi","eta"),
        ),
        cms.PSet (
            name = cms.string("secondaryJetFlavor"),
            title = cms.string("Jet Flavor"),
            bins = cms.untracked.vdouble(42, -21, 21),
            inputVariables = cms.vstring("flavour"),
        ),
        cms.PSet (
            name = cms.string("secondaryJetCSV"),
            title = cms.string("Jet Combined Secondary Vertex B-tagging Discriminant"),
            bins = cms.untracked.vdouble(100, -1, 1),
            inputVariables = cms.vstring("btagCombinedSecVertex"),
        ),
    )
)
##############################################################################################

DiJetHistograms = cms.PSet(
    inputCollection = cms.string("jet-jet pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("diJetInvMass"),
            title = cms.string("Di-jet Invariant Mass; M_{jet-pair} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass"),
        ),
        cms.PSet (
            name = cms.string("diJetChargeProduct"),
            title = cms.string("Di-jet Charge Product; charge_{jet}_{1}*charge_{jet}_{2}"),
            bins = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("chargeProduct"),
        ),
        cms.PSet (
            name = cms.string("diJetDeltaPhi"),
            title = cms.string("Di-jet Phi Difference; |#Delta(#phi)|"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
        ),
        cms.PSet (
            name = cms.string("diJetDeltaEta"),
            title = cms.string("Di-jet Eta Difference; |#Delta(#eta)|"),
            bins = cms.untracked.vdouble(100, 0, 10),
            inputVariables = cms.vstring("deltaEta"),
        ),
        cms.PSet (
            name = cms.string("diJetDeltaR"),
            title = cms.string("Di-jet #DeltaR; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 10),
            inputVariables = cms.vstring("deltaR"),
        ),
        cms.PSet (
            name = cms.string("diJetPt"),
            title = cms.string("Di-jet Transverse Momentum ; p_{T}_{jet-pair} [GeV] "),
            bins = cms.untracked.vdouble(100, 0,500),
            inputVariables = cms.vstring("pt"),
        ),
        ## cms.PSet (
        ##     name = cms.string("diJetDeltaPt"),
        ##     title = cms.string("Di-jet Transverse Momentum Difference; |p_{T}_{jet1}-p_{T}_{jet2}| [GeV] "),
        ##     bins = cms.untracked.vdouble(100, 0,500),
        ##     inputVariables = cms.vstring("absDeltaPt"),  # Variable not available in OSUAnalysis.cc  
        ## ),
        cms.PSet (
            name = cms.string("diJetThreeDAngle"),
            title = cms.string("Di-jet 3D angle; 3D angle"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("threeDAngle"),
        ),
    )
)

##############################################################################################
JetSecondaryJetHistograms = cms.PSet(
    inputCollection = cms.string("jet-secondary jet pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("JetSecondaryJetInvMass"),
            title = cms.string("Di-jet Invariant Mass; M_{jet-pair} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass"),
        ),
        cms.PSet (
            name = cms.string("JetSecondaryJetChargeProduct"),
            title = cms.string("Di-jet Charge Product; charge_{jet}_{1}*charge_{jet}_{2}"),
            bins = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("chargeProduct"),
        ),
        cms.PSet (
            name = cms.string("JetSecondaryJetDeltaPhi"),
            title = cms.string("Di-jet Phi Difference; |#Delta(#phi)|"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
        ),
        cms.PSet (
            name = cms.string("JetSecondaryJetDeltaEta"),
            title = cms.string("Di-jet Eta Difference; |#Delta(#eta)|"),
            bins = cms.untracked.vdouble(100, 0, 10),
            inputVariables = cms.vstring("deltaEta"),
        ),
        cms.PSet (
            name = cms.string("JetSecondaryJetDeltaR"),
            title = cms.string("Di-jet #DeltaR; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 10),
            inputVariables = cms.vstring("deltaR"),
        ),
        cms.PSet (
            name = cms.string("JetSecondaryJetPt"),
            title = cms.string("Di-jet Transverse Momentum ; p_{T}_{jet-pair} [GeV] "),
            bins = cms.untracked.vdouble(100, 0,500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("JetSecondaryJetThreeDAngle"),
            title = cms.string("Di-jet 3D angle; 3D angle"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("threeDAngle"),
        ),
    )
)

ElectronJetHistograms = cms.PSet(
    inputCollection = cms.string("electron-jet pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("electronJetPt"),
            title = cms.string("Electron-Jet Pair Transverse Momentum; p_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("electronJetInvMass"),
            title = cms.string("Electron-jet Invariant Mass; M_{e-jet} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass"),
        ),
        cms.PSet (
            name = cms.string("electronJetChargeProduct"),
            title = cms.string("Electron-jet Charge Product; charge_{e}*charge_{jet}"),
            bins = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("chargeProduct"),
        ),
        cms.PSet (
            name = cms.string("electronJetDeltaPhi"),
            title = cms.string("Electron-jet Phi Difference; |#Delta(#phi)|"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
        ),
        cms.PSet (
            name = cms.string("electronJetDeltaEta"),
            title = cms.string("Electron-jet Eta Difference; |#Delta(#eta)|"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaEta"),
        ),
        cms.PSet (
            name = cms.string("electronJetDeltaRvselectronJetDeltaEta "),
            title = cms.string("Electron-jet #DeltaR.vs #Delta(#eta); |#Delta(#eta)|; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 6, 100, 0 ,6),
            inputVariables = cms.vstring("deltaEta","deltaR"),
        ),
        cms.PSet (
            name = cms.string("electronJetDeltaRvselectronJetDeltaPhi "),
            title = cms.string("Electron-jet #DeltaR.vs #Delta(#phi); |#Delta(#phi)|; |#DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 3.15, 100, 0, 6),
            inputVariables = cms.vstring("deltaPhi","deltaR"),
        ),
        cms.PSet (
            name = cms.string("electronEtavsJetEta "),
            title = cms.string("Electron Eta.vs Jet Eta; Jet #eta; Electron #eta"),
            bins = cms.untracked.vdouble(100, -6, 6, 100, -6 ,6),
            inputVariables = cms.vstring("jetEta","electronEta"),
        ),
        cms.PSet (
            name = cms.string("electronPhivsJetPhi "),
            title = cms.string("Electron Phi.vs Jet Phi; Jet #phi; Electron #phi"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15, 100, -3.15 ,3.15),
            inputVariables = cms.vstring("jetPhi","electronPhi"),
        ),
        cms.PSet (
            name = cms.string("electronJetDeltaR"),
            title = cms.string("Electron-jet #DeltaR; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR"),
        ),
        cms.PSet (
            name = cms.string("electronJetThreeDAngle"),
            title = cms.string("Electron-jet 3D angle; 3D angle"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("threeDAngle"),
        ),
    )
)

##############################################################################################

MuonJetHistograms = cms.PSet(
    inputCollection = cms.string("muon-jet pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("muonJetPt"),
            title = cms.string("Muon-Jet Pair Transverse Momentum; p_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("muonJetInvMass"),
            title = cms.string("Muon-jet Invariant Mass; M_{#mu-jet} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass"),
        ),
        cms.PSet (
            name = cms.string("muonJetChargeProduct"),
            title = cms.string("Muon-jet Charge Product; charge_{#mu}*charge_{jet}"),
            bins = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("chargeProduct"),
        ),
        cms.PSet (
            name = cms.string("muonJetDeltaPhi"),
            title = cms.string("Muon-jet Phi Difference; |#Delta(#phi)|"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
        ),
        cms.PSet (
            name = cms.string("muonJetDeltaEta"),
            title = cms.string("Muon-jet Eta Difference; |#Delta(#eta)|"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaEta"),
        ),
        cms.PSet (
            name = cms.string("muonJetDeltaRvsmuonJetDeltaEta "),
            title = cms.string("Muon-jet #DeltaR.vs #Delta(eta); |#Delta(#eta)|; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 6, 100, 0 ,6),
            inputVariables = cms.vstring("deltaEta","deltaR"),
        ),
        cms.PSet (
            name = cms.string("muonJetDeltaRvsmuonJetDeltaPhi "),
            title = cms.string("Muon-jet #DeltaR.vs #Delta(#phi); |#Delta(#phi)|; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 3.15, 100, 0 ,6),
            inputVariables = cms.vstring("deltaPhi","deltaR"),
        ),
        cms.PSet (
            name = cms.string("muonEtavsJetEta "),
            title = cms.string("Muon Eta.vs Jet Eta; Jet #eta; Muon #eta"),
            bins = cms.untracked.vdouble(100, -6, 6, 100, -6 ,6),
            inputVariables = cms.vstring("jetEta","muonEta"),
        ),
        cms.PSet (
            name = cms.string("muonPhivsJetPhi "),
            title = cms.string("Muon Phi.vs Jet Phi; Jet #phi; Muon #phi"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15, 100, -3.15 ,3.15),
            inputVariables = cms.vstring("jetPhi","muonPhi"),
        ),
        cms.PSet (
            name = cms.string("muonJetDeltaR"),
            title = cms.string("Muon-jet #DeltaR; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR"),
        ),
        cms.PSet (
            name = cms.string("muonJetThreeDAngle"),
            title = cms.string("Muon-jet 3D angle; 3D angle"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("threeDAngle"),
        ),
    )
)

##############################################################################################
MuonSecondaryJetHistograms = cms.PSet(
    inputCollection = cms.string("muon-secondary jet pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("muonSecondaryJetPt"),
            title = cms.string("Muon-Jet Pair Transverse Momentum; p_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("muonSecondaryJetInvMass"),
            title = cms.string("Muon-jet Invariant Mass; M_{#mu-jet} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass"),
        ),
        cms.PSet (
            name = cms.string("muonSecondaryJetChargeProduct"),
            title = cms.string("Muon-jet Charge Product; charge_{#mu}*charge_{jet}"),
            bins = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("chargeProduct"),
        ),
        cms.PSet (
            name = cms.string("muonSecondaryJetDeltaPhi"),
            title = cms.string("Muon-jet Phi Difference; |#Delta(#phi)|"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
        ),
        cms.PSet (
            name = cms.string("muonSecondaryJetDeltaEta"),
            title = cms.string("Muon-jet Eta Difference; |#Delta(#eta)|"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaEta"),
        ),
        cms.PSet (
            name = cms.string("muonSecondaryJetDeltaRvsmuonSecondaryJetDeltaEta "),
            title = cms.string("Muon-jet #DeltaR.vs #Delta(eta); |#Delta(#eta)|; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 6, 100, 0 ,6),
            inputVariables = cms.vstring("deltaEta","deltaR"),
        ),
        cms.PSet (
            name = cms.string("muonSecondaryJetDeltaRvsmuonSecondaryJetDeltaPhi "),
            title = cms.string("Muon-jet #DeltaR.vs #Delta(#phi); |#Delta(#phi)|; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 3.15, 100, 0 ,6),
            inputVariables = cms.vstring("deltaPhi","deltaR"),
        ),
        cms.PSet (
            name = cms.string("muonEtavsSecondaryJetEta "),
            title = cms.string("Muon Eta.vs Jet Eta; Jet #eta; Muon #eta"),
            bins = cms.untracked.vdouble(100, -6, 6, 100, -6 ,6),
            inputVariables = cms.vstring("jetEta","muonEta"),
        ),
        cms.PSet (
            name = cms.string("muonPhivsSecondaryJetPhi "),
            title = cms.string("Muon Phi.vs Jet Phi; Jet #phi; Muon #phi"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15, 100, -3.15 ,3.15),
            inputVariables = cms.vstring("jetPhi","muonPhi"),
        ),
        cms.PSet (
            name = cms.string("muonSecondaryJetDeltaR"),
            title = cms.string("Muon-jet #DeltaR; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR"),
        ),
        cms.PSet (
            name = cms.string("muonSecondaryJetThreeDAngle"),
            title = cms.string("Muon-jet 3D angle; 3D angle"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("threeDAngle"),
        ),
    )
)
##############################################################################################
ElectronSecondaryJetHistograms = cms.PSet(
    inputCollection = cms.string("electron-secondary jet pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("electronSecondaryJetPt"),
            title = cms.string("Electron-Jet Pair Transverse Momentum; p_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("electronSecondaryJetInvMass"),
            title = cms.string("Electron-jet Invariant Mass; M_{#mu-jet} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass"),
        ),
        cms.PSet (
            name = cms.string("electronSecondaryJetChargeProduct"),
            title = cms.string("Electron-jet Charge Product; charge_{#mu}*charge_{jet}"),
            bins = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("chargeProduct"),
        ),
        cms.PSet (
            name = cms.string("electronSecondaryJetDeltaPhi"),
            title = cms.string("Electron-jet Phi Difference; |#Delta(#phi)|"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
        ),
        cms.PSet (
            name = cms.string("electronSecondaryJetDeltaEta"),
            title = cms.string("Electron-jet Eta Difference; |#Delta(#eta)|"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaEta"),
        ),
        cms.PSet (
            name = cms.string("electronSecondaryJetDeltaRvselectronSecondaryJetDeltaEta "),
            title = cms.string("Electron-jet #DeltaR.vs #Delta(eta); |#Delta(#eta)|; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 6, 100, 0 ,6),
            inputVariables = cms.vstring("deltaEta","deltaR"),
        ),
        cms.PSet (
            name = cms.string("electronSecondaryJetDeltaRvselectronSecondaryJetDeltaPhi "),
            title = cms.string("Electron-jet #DeltaR.vs #Delta(#phi); |#Delta(#phi)|; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 3.15, 100, 0 ,6),
            inputVariables = cms.vstring("deltaPhi","deltaR"),
        ),
        cms.PSet (
            name = cms.string("electronEtavsSecondaryJetEta "),
            title = cms.string("Electron Eta.vs Jet Eta; Jet #eta; Electron #eta"),
            bins = cms.untracked.vdouble(100, -6, 6, 100, -6 ,6),
            inputVariables = cms.vstring("jetEta","electronEta"),
        ),
        cms.PSet (
            name = cms.string("electronPhivsSecondaryJetPhi "),
            title = cms.string("Electron Phi.vs Jet Phi; Jet #phi; Electron #phi"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15, 100, -3.15 ,3.15),
            inputVariables = cms.vstring("jetPhi","electronPhi"),
        ),
        cms.PSet (
            name = cms.string("electronSecondaryJetDeltaR"),
            title = cms.string("Electron-jet #DeltaR; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR"),
        ),
        cms.PSet (
            name = cms.string("electronSecondaryJetThreeDAngle"),
            title = cms.string("Electron-jet 3D angle; 3D angle"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("threeDAngle"),
        ),
    )
)
#################################################################################
PhotonSecondaryJetHistograms = cms.PSet(
    inputCollection = cms.string("photon-secondary jet pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("photonSecondaryJetPt"),
            title = cms.string("Photon-Jet Pair Transverse Momentum; p_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("photonSecondaryJetInvMass"),
            title = cms.string("Photon-jet Invariant Mass; M_{#mu-jet} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass"),
        ),
        cms.PSet (
            name = cms.string("photonSecondaryJetDeltaPhi"),
            title = cms.string("Photon-jet Phi Difference; |#Delta(#phi)|"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
        ),
        cms.PSet (
            name = cms.string("photonSecondaryJetDeltaEta"),
            title = cms.string("Photon-jet Eta Difference; |#Delta(#eta)|"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaEta"),
        ),
        cms.PSet (
            name = cms.string("photonSecondaryJetDeltaRvsphotonSecondaryJetDeltaEta "),
            title = cms.string("Photon-jet #DeltaR.vs #Delta(eta); |#Delta(#eta)|; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 6, 100, 0 ,6),
            inputVariables = cms.vstring("deltaEta","deltaR"),
        ),
        cms.PSet (
            name = cms.string("photonSecondaryJetDeltaRvsphotonSecondaryJetDeltaPhi "),
            title = cms.string("Photon-jet #DeltaR.vs #Delta(#phi); |#Delta(#phi)|; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 3.15, 100, 0 ,6),
            inputVariables = cms.vstring("deltaPhi","deltaR"),
        ),
        cms.PSet (
            name = cms.string("photonEtavsSecondaryJetEta "),
            title = cms.string("Photon Eta.vs Jet Eta; Jet #eta; Photon #eta"),
            bins = cms.untracked.vdouble(100, -6, 6, 100, -6 ,6),
            inputVariables = cms.vstring("jetEta","photonEta"),
        ),
        cms.PSet (
            name = cms.string("photonPhivsSecondaryJetPhi "),
            title = cms.string("Photon Phi.vs Jet Phi; Jet #phi; Photon #phi"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15, 100, -3.15 ,3.15),
            inputVariables = cms.vstring("jetPhi","photonPhi"),
        ),
        cms.PSet (
            name = cms.string("photonSecondaryJetDeltaR"),
            title = cms.string("Photon-jet #DeltaR; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR"),
        ),
        cms.PSet (
            name = cms.string("photonSecondaryJetThreeDAngle"),
            title = cms.string("Photon-jet 3D angle; 3D angle"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("threeDAngle"),
        ),
    )
)
############################################################################
PhotonJetHistograms = cms.PSet(
    inputCollection = cms.string("photon-jet pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("photonJetPt"),
            title = cms.string("Photon-Jet Pair Transverse Momentum; p_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("photonJetInvMass"),
            title = cms.string("Photon-jet Invariant Mass; M_{#mu-jet} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass"),
        ),
        cms.PSet (
            name = cms.string("photonJetDeltaPhi"),
            title = cms.string("Photon-jet Phi Difference; |#Delta(#phi)|"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
        ),
        cms.PSet (
            name = cms.string("photonJetDeltaEta"),
            title = cms.string("Photon-jet Eta Difference; |#Delta(#eta)|"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaEta"),
        ),
        cms.PSet (
            name = cms.string("photonJetDeltaRvsphotonJetDeltaEta "),
            title = cms.string("Photon-jet #DeltaR.vs #Delta(eta); |#Delta(#eta)|; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 6, 100, 0 ,6),
            inputVariables = cms.vstring("deltaEta","deltaR"),
        ),
        cms.PSet (
            name = cms.string("photonJetDeltaRvsphotonJetDeltaPhi "),
            title = cms.string("Photon-jet #DeltaR.vs #Delta(#phi); |#Delta(#phi)|; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 3.15, 100, 0 ,6),
            inputVariables = cms.vstring("deltaPhi","deltaR"),
        ),
        cms.PSet (
            name = cms.string("photonEtavsJetEta "),
            title = cms.string("Photon Eta.vs Jet Eta; Jet #eta; Photon #eta"),
            bins = cms.untracked.vdouble(100, -6, 6, 100, -6 ,6),
            inputVariables = cms.vstring("jetEta","photonEta"),
        ),
        cms.PSet (
            name = cms.string("photonPhivsJetPhi "),
            title = cms.string("Photon Phi.vs Jet Phi; Jet #phi; Photon #phi"),
            bins = cms.untracked.vdouble(100, -3.15, 3.15, 100, -3.15 ,3.15),
            inputVariables = cms.vstring("jetPhi","photonPhi"),
        ),
        cms.PSet (
            name = cms.string("photonJetDeltaR"),
            title = cms.string("Photon-jet #DeltaR; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR"),
        ),
        cms.PSet (
            name = cms.string("photonJetThreeDAngle"),
            title = cms.string("Photon-jet 3D angle; 3D angle"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("threeDAngle"),
        ),
    )
)
#######################################################################################
MCParticleHistograms = cms.PSet(
    inputCollection = cms.string("mcparticles"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("mcparticlePt"),
            title = cms.string("MCparticle Transverse Momentum; p_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("mcparticleEta"),
            title = cms.string("MCparticle Eta; #eta"),
            bins = cms.untracked.vdouble(100, -3, 3),
            inputVariables = cms.vstring("eta"),
        ),
        cms.PSet (
            name = cms.string("mcparticleStatus"),
            title = cms.string("MCparticle Status; status"),
            bins = cms.untracked.vdouble(3, 1, 4),
            inputVariables = cms.vstring("status"),
        ),
        cms.PSet (
            name = cms.string("mcparticleAbsD0Vertex"),
            title = cms.string("MCParticle |d_{0}| wrt PV; |d_{0}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 20),
            inputVariables = cms.vstring("abs(d0Vertex)")
        ),
        cms.PSet (
            name = cms.string("mcparticleAbsD0Beamspot"),
            title = cms.string("MCParticle |d_{0}| wrt Beamspot; |d_{0}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 20),
            inputVariables = cms.vstring("abs(d0Beamspot)")
        ),
        cms.PSet (
            name = cms.string("mcparticleAbsD0Origin"),
            title = cms.string("MCParticle |d_{0}| wrt Origin; |d_{0}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 20),
            inputVariables = cms.vstring("abs(d0Origin)")
        ),
        cms.PSet (
            name = cms.string("mcparticleDz"),
            title = cms.string("MCparticle d_{z}; d_{z} [cm]"),
            bins = cms.untracked.vdouble(100, -0.5, 0.5),
            inputVariables = cms.vstring("dz"),
        ),
        cms.PSet (
            name = cms.string("mcparticleAbsDZ"),
            title = cms.string("MCparticle d_{z}; |d_{z}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.5),
            inputVariables = cms.vstring("abs(dz)"),
        ),
    )
)    

#######################################################################################
SecondaryMCParticleHistograms = cms.PSet(
    inputCollection = cms.string("secondary mcparticles"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("secondaryMcparticlePt"),
            title = cms.string("Secondary MCparticle Transverse Momentum; p_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("secondaryMcparticleEta"),
            title = cms.string("Secondary MCparticle Eta; #eta"),
            bins = cms.untracked.vdouble(100, -3, 3),
            inputVariables = cms.vstring("eta"),
        ),
        cms.PSet (
            name = cms.string("secondaryMcparticleStatus"),
            title = cms.string("Secondary MCparticle Status; status"),
            bins = cms.untracked.vdouble(3, 1, 4),
            inputVariables = cms.vstring("status"),
        ),
        cms.PSet (
            name = cms.string("secondaryMcparticleAbsD0Vertex"),
            title = cms.string("MCParticle |d_{0}| wrt PV; |d_{0}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 20),
            inputVariables = cms.vstring("abs(d0Vertex)")
        ),
        cms.PSet (
            name = cms.string("secondaryMcparticleAbsD0Beamspot"),
            title = cms.string("MCParticle |d_{0}| wrt Beamspot; |d_{0}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 20),
            inputVariables = cms.vstring("abs(d0Beamspot)")
        ),
        cms.PSet (
            name = cms.string("secondaryMcparticleAbsD0Origin"),
            title = cms.string("MCParticle |d_{0}| wrt Origin; |d_{0}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 20),
            inputVariables = cms.vstring("abs(d0Origin)")
        ),
        cms.PSet (
            name = cms.string("secondaryMcparticleDz"),
            title = cms.string("Secondary MCparticle d_{z}; d_{z} [cm]"),
            bins = cms.untracked.vdouble(100, -0.5, 0.5),
            inputVariables = cms.vstring("dz"),
        ),
        cms.PSet (
            name = cms.string("secondaryMcparticleAbsDZ"),
            title = cms.string("Secondary MCparticle d_{z}; |d_{z}| [cm]"),
            bins = cms.untracked.vdouble(100, 0, 0.5),
            inputVariables = cms.vstring("abs(dz)"),
        ),
    )
)    

##############################################################################################

MetHistograms = cms.PSet(
    inputCollection = cms.string("mets"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("met"),
            title = cms.string("Missing E_{T}; Missing E_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
    )
)

##############################################################################################

TrackHistograms = cms.PSet(
     inputCollection = cms.string("tracks"),
     histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("trackPt"),
            title = cms.string("Track Transverse Momentum; p_{T} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
             name = cms.string("trackEta"),
             title = cms.string("Track Eta; #eta"),
             bins = cms.untracked.vdouble(100, -3, 3),
             inputVariables = cms.vstring("eta"),
        ),
        cms.PSet (
             name = cms.string("trackPhi"),
             title = cms.string("Track Phi; #phi"),
             bins = cms.untracked.vdouble(100, -3, 3),
             inputVariables = cms.vstring("phi"),
        ),
        cms.PSet (
            name = cms.string("trackd0"),
            title = cms.string("Track d_{0}; d_{0} [cm]"),
            bins = cms.untracked.vdouble(100, -0.5, 0.5),
            inputVariables = cms.vstring("d0"),
        ),
        cms.PSet (
            name = cms.string("trackdz"),
            title = cms.string("Track d_{z}; d_{z} [cm]"),
            bins = cms.untracked.vdouble(100, -30, 30),
            inputVariables = cms.vstring("dZ"),
         ),
        cms.PSet (
            name = cms.string("trackNumValidHits"),
            title = cms.string("Track Number of Valid Hits; Number of Valid Hits"),
            bins = cms.untracked.vdouble(100, 0, 30),
            inputVariables = cms.vstring("numValidHits"),
        ),
        cms.PSet (
            name = cms.string("trackChi2"),
            title = cms.string("Track Reduced Chi2; #chi^{2} / DOF"),
            bins = cms.untracked.vdouble(100, 0, 30),
            inputVariables = cms.vstring("normChi2"),
        ),
        cms.PSet (
            name = cms.string("trackCharge"),
            title = cms.string("Track Charge; charge"),
            bins = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("charge"),
        ),
    )
)

##############################################################################################

ElectronTrackHistograms = cms.PSet(
    inputCollection = cms.string("electron-track pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("ElectronTrackInvMass"),
            title = cms.string("Electron-Track Invariant Mass; M_{e+track} [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 180),
            inputVariables = cms.vstring("invMass"),
            ),
        cms.PSet (
            name = cms.string("ElectronTrackDeltaEta"),
            title = cms.string("Electron-Track Eta Difference; |#Delta(#eta)|"),
            bins = cms.untracked.vdouble(100, 0, 10),
            inputVariables = cms.vstring("deltaEta"),
            ),
        cms.PSet (
            name = cms.string("ElectronTrackDeltaPhi"),
            title = cms.string("Electron-Track Phi Difference; |#Delta(#phi)|"),
            bins = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
            ),
        cms.PSet (
            name = cms.string("ElectronTrackDeltaR"),
            title = cms.string("Electron-Track #DeltaR; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 10),
            inputVariables = cms.vstring("deltaR"),
            ),
        cms.PSet (
            name = cms.string("ElectronTrackDeltaRZoom"),
            title = cms.string("Electron-Track #DeltaR; #DeltaR"),
            bins = cms.untracked.vdouble(100, 0, 1),
            inputVariables = cms.vstring("deltaR"),
            ),
        )
    )

##############################################################################################

MuonTrackHistograms = cms.PSet(
    inputCollection = cms.string("muon-track pairs"),
    histograms = cms.VPSet (
    cms.PSet (
        name = cms.string("MuonTrackInvMass"),
        title = cms.string("Muon-Track Invariant Mass; M_{#mu+track} [GeV]"),
        bins = cms.untracked.vdouble(100, 0, 180),
        inputVariables = cms.vstring("invMass"),
        ),
    cms.PSet (
        name = cms.string("MuonTrackDeltaEta"),
        title = cms.string("Muon-Track Eta Difference; |#Delta(#eta)|"),
        bins = cms.untracked.vdouble(100, 0, 10),
        inputVariables = cms.vstring("deltaEta"),
        ),
    cms.PSet (
        name = cms.string("MuonTrackDeltaPhi"),
        title = cms.string("Muon-Track Phi Difference; |#Delta(#phi)|"),
        bins = cms.untracked.vdouble(100, 0, 3.15),
        inputVariables = cms.vstring("deltaPhi"),
        ),
    cms.PSet (
        name = cms.string("MuonTrackDeltaR"),
        title = cms.string("Muon-Track #DeltaR; #DeltaR"),
        bins = cms.untracked.vdouble(100, 0, 10),
        inputVariables = cms.vstring("deltaR"),
        ),
    cms.PSet (
        name = cms.string("MuonTrackDeltaRZoom"),
        title = cms.string("Muon-Track #DeltaR; #DeltaR"),
        bins = cms.untracked.vdouble(100, 0, 1),
        inputVariables = cms.vstring("deltaR"),
        ),
    cms.PSet (
        name = cms.string("MuonTrackDeltaPt"),
        title = cms.string("Muon-Track #DeltaPt; p_{T}^{#mu} - p_{T}^{trk} [GeV]"),
        bins = cms.untracked.vdouble(100, -50, 50),
        inputVariables = cms.vstring("deltaPt"),
        ),
    )
)

##############################################################################################

TauTrackHistograms = cms.PSet(
    inputCollection = cms.string("tau-track pairs"),
    histograms = cms.VPSet (
    cms.PSet (
        name = cms.string("TauTrackInvMass"),
        title = cms.string("Tau-Track Invariant Mass; M_{#mu+track} [GeV]"),
        bins = cms.untracked.vdouble(100, 0, 180),
        inputVariables = cms.vstring("invMass"),
        ),
    cms.PSet (
        name = cms.string("TauTrackDeltaEta"),
        title = cms.string("Tau-Track Eta Difference; |#Delta(#eta)|"),
        bins = cms.untracked.vdouble(100, 0, 10),
        inputVariables = cms.vstring("deltaEta"),
        ),
    cms.PSet (
        name = cms.string("TauTrackDeltaPhi"),
        title = cms.string("Tau-Track Phi Difference; |#Delta(#phi)|"),
        bins = cms.untracked.vdouble(100, 0, 3.15),
        inputVariables = cms.vstring("deltaPhi"),
        ),
    cms.PSet (
        name = cms.string("TauTrackDeltaR"),
        title = cms.string("Tau-Track #DeltaR; #DeltaR"),
        bins = cms.untracked.vdouble(100, 0, 10),
        inputVariables = cms.vstring("deltaR"),
        ),
    cms.PSet (
        name = cms.string("TauTrackDeltaRZoom"),
        title = cms.string("Tau-Track #DeltaR; #DeltaR"),
        bins = cms.untracked.vdouble(100, 0, 1),
        inputVariables = cms.vstring("deltaR"),
        ),
    )
)

##############################################################################################

EventHistograms = cms.PSet(
    inputCollection = cms.string("events"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("ht"),
            title = cms.string("H_{T}; #Sigma p_{T} selected jets [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("ht"),
        ),
        cms.PSet (
            name = cms.string("unfilteredHt"),
            title = cms.string("Unfiltered H_{T}; #Sigma p_{T} all jets [GeV]"),
            bins = cms.untracked.vdouble(150, 0, 750),
            inputVariables = cms.vstring("unfilteredHt"),
        ),
        cms.PSet (
            name = cms.string("st"),
            title = cms.string("S_{T}; #Sigma p_{T} selected e/#mu/jets [GeV]"),
            bins = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("st"),
        ),
        cms.PSet (
            name = cms.string("unfilteredSt"),
            title = cms.string("Unfiltered S_{T}; #Sigma p_{T} all e/#mu/jets [GeV]"),
            bins = cms.untracked.vdouble(150, 0, 750),
            inputVariables = cms.vstring("unfilteredSt"),
        ),
        cms.PSet (
            name = cms.string("nExtraPartons"),
            title = cms.string("Number of Extra Partons; # Extra Partons"),
            bins = cms.untracked.vdouble(6, 0, 6),
            inputVariables = cms.vstring("nExtraPartons"),
        ),
        cms.PSet (
            name = cms.string("puScaleFactor"),
            title = cms.string("Pileup Scale Factor;pileup weight"),
            bins = cms.untracked.vdouble(100, 0, 5),
            inputVariables = cms.vstring("puScaleFactor"),
        ),
        cms.PSet (
            name = cms.string("topPtScaleFactor"),
            title = cms.string("Top Pt Reweighting Factor; top pt weight "),
            bins = cms.untracked.vdouble(100, 0.5, 1.5),
            inputVariables = cms.vstring("topPtScaleFactor"),
        ),
        cms.PSet (
            name = cms.string("triggerScaleFactor"),
            title = cms.string("Trigger Efficiency Reweighting Factor; trigger SF weight "),
            bins = cms.untracked.vdouble(100, 0.5, 1.5),
            inputVariables = cms.vstring("triggerScaleFactor"),
        ),
        cms.PSet (
            name = cms.string("muonScaleFactor"),
            title = cms.string("Muon Scale Factor;muon weight"),
            bins = cms.untracked.vdouble(100, 0.5, 1.5),
            inputVariables = cms.vstring("muonScaleFactor"),
        ),
        cms.PSet (
            name = cms.string("bTagScaleFactor"),
            title = cms.string("Btag Scale Factor;btag weight"),
            bins = cms.untracked.vdouble(100, 0.5, 1.5),
            inputVariables = cms.vstring("bTagScaleFactor"),
        ),
        cms.PSet (
            name = cms.string("electronScaleFactor"),
            title = cms.string("Electron Scale Factor;electron weight"),
            bins = cms.untracked.vdouble(100, 0.5, 1.5),
            inputVariables = cms.vstring("electronScaleFactor"),
        ),
        cms.PSet (
            name = cms.string("stopCTauScaleFactor"),
            title = cms.string("#LTc#tau#GT Scale Factor;#LTc#tau#GT weight"),
            bins = cms.untracked.vdouble(100, 0, 10),
            inputVariables = cms.vstring("stopCTauScaleFactor"),
        ),
        cms.PSet (
            name = cms.string("globalScaleFactor"),
            title = cms.string("Global Scale Factor; event weight"),
            bins = cms.untracked.vdouble(100, 0, 2),
            inputVariables = cms.vstring("globalScaleFactor"),
        ),
        cms.PSet (
            name = cms.string("channelScaleFactor"),
            title = cms.string("Channel Scale Factor; event weight"),
            bins = cms.untracked.vdouble(100, 0, 2),
            inputVariables = cms.vstring("channelScaleFactor"),
        ),
        cms.PSet (
            name = cms.string("eventScaleFactor"),
            title = cms.string("Event Scale Factor; event weight"),
            bins = cms.untracked.vdouble(100, 0, 2),
            inputVariables = cms.vstring("eventScaleFactor"),
        ),
    )
)

##############################################################################################
