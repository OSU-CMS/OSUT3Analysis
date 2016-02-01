import FWCore.ParameterSet.Config as cms
from OSUT3Analysis.Configuration.pdgIdBins import *


###############################################
##### Set up the histograms to be plotted #####
###############################################


##############################################################################################

MuonHistograms = cms.PSet(
    inputCollection = cms.vstring("muons"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("numMuons"),
            title = cms.string("Number of Selected Muons; # muons"),
            binsX = cms.untracked.vdouble(10, 0, 10),
            inputVariables = cms.vstring("number(muon)"),
        ),
        cms.PSet (
            name = cms.string("muonPt"),
            title = cms.string("Muon Transverse Momentum; muon p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("muonPt_0"),
            title = cms.string("Muon Transverse Momentum; muon p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("muonPt_1"),
            title = cms.string("Muon Transverse Momentum; muon p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(0,5,10,15,20,25,30,40,50,70,90,120,150,200,300,500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("muonPt_2"),
            title = cms.string("Muon Transverse Momentum; muon p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(0,10,20,30,50,90,150,300,500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("muonEta"),
            title = cms.string("Muon Eta; muon #eta"),
            binsX = cms.untracked.vdouble(100, -4, 4),
            inputVariables = cms.vstring("eta"),
        ),
        cms.PSet (
            name = cms.string("muonPhi"),
            title = cms.string("Muon Phi; muon #phi"),
            binsX = cms.untracked.vdouble(100, -3.15, 3.15),
            inputVariables = cms.vstring("phi"),
        ),
        cms.PSet (
            name = cms.string("muonCharge"),
            title = cms.string("Muon Charge; muon charge"),
            binsX = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("charge"),
        ),
        cms.PSet (
            name = cms.string("muonEtaPhi"),
            title = cms.string("Muon Eta vs. Phi; muon #phi; muon #eta"),
            binsX = cms.untracked.vdouble(100, -3.15, 3.15),
            binsY = cms.untracked.vdouble(100, -3, 3),
            inputVariables = cms.vstring("phi","eta"),
        ),
        cms.PSet (
            name = cms.string("muonNumberOfValidHits"),
            title = cms.string("Muon Number of Valid Hits; # Hits"),
            binsX = cms.untracked.vdouble(40, 0, 40),
            inputVariables = cms.vstring("numberOfValidHits"),
        ),
#        cms.PSet (
#            name = cms.string("muonNormalizedChi2"),
#            title = cms.string("Muon Chi Squared"),
#            binsX = cms.untracked.vdouble(20, 0, 20),
#            inputVariables = cms.vstring("normChi2"),
#        ),
        cms.PSet (
            name = cms.string("muonNumberOfMatchedStations"),
            title = cms.string("Muon Track Number of Matched Stations; # matched stations"),
            binsX = cms.untracked.vdouble(10, 0, 10),
            inputVariables = cms.vstring("numberOfMatchedStations"),
        ),
        cms.PSet (
            name = cms.string("muonNumberOfValidPixelHits"),
            title = cms.string("Muon Number of Valid Pixel Hits;muon numberOfValidPixelHits"),
            binsX = cms.untracked.vdouble(30, 0, 30),
            inputVariables = cms.vstring("innerTrack.hitPattern_.numberOfValidPixelHits"),
        ),
        cms.PSet (
            name = cms.string("muonTrackerLayersWithMeasurement"),
            title = cms.string("Muon Number of Tracker Layer with Measurement;muon trackerLayersWithMeasurement"),
            binsX = cms.untracked.vdouble(30, 0, 30),
            inputVariables = cms.vstring("innerTrack.hitPattern_.trackerLayersWithMeasurement"),
            ),
        cms.PSet (
            name = cms.string("muonIsGlobalMuon"),
            title = cms.string("Muon isGlobalMuon; isGlobalMuon"),
            binsX = cms.untracked.vdouble(2, 0, 2),
            inputVariables = cms.vstring("isGlobalMuon"),
        ),
        cms.PSet (
            name = cms.string("muonIsPFMuon"),
            title = cms.string("Muon isPFMuon; isPFMuon"),
            binsX = cms.untracked.vdouble(2, 0, 2),
            inputVariables = cms.vstring("isPFMuon"),
        ),
        cms.PSet (
            name = cms.string("muonIsTightMuonWRTVtx"),
            title = cms.string("Muon isTightMuon; isTightMuon"),
            binsX = cms.untracked.vdouble(2, 0, 2),
            inputVariables = cms.vstring("isTightMuonWRTVtx"),
        ),
        cms.PSet (
            name = cms.string("muonDbetaIsolation"),
            title = cms.string("Muon Isolation; muon #Delta#beta Isolation"),
            binsX = cms.untracked.vdouble(200, 0, 6.0),
            inputVariables = cms.vstring("(pfIsolationR04_.sumChargedHadronPt + max(0.0,pfIsolationR04_.sumNeutralHadronEt + pfIsolationR04_.sumPhotonEt - 0.5*pfIsolationR04_.sumPUPt))/pt"),
        ),
    )
)

##############################################################################################

MuonIPHistograms = cms.PSet(
    inputCollection = cms.vstring("muons","beamspots"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("muonD0Beamspot"),
            title = cms.string("Muon d_{0} wrt Beamspot; muon d_{0} [cm]"),
            binsX = cms.untracked.vdouble(100, -0.02, 0.02),
            inputVariables = cms.vstring("(-(muon.vx - beamspot.x0)*muon.py + (muon.vy - beamspot.y0)*muon.px)/muon.pt"),
        ),
        cms.PSet (
            name = cms.string("muonAbsD0Beamspot"),
            title = cms.string("Muon |d_{0}| wrt Beamspot; muon |d_{0}| [cm]"),
            binsX = cms.untracked.vdouble(100, 0.0, 0.02),
            inputVariables = cms.vstring("abs(-(muon.vx - beamspot.x0)*muon.py + (muon.vy - beamspot.y0)*muon.px)/muon.pt"),
        ),
        cms.PSet (
            name = cms.string("muonD0SigBeamspot"),
            title = cms.string("Muon d_{0}/#sigma(d_{0}) wrt Beamspot; d_{0}/#sigma(d_{0})"),
            binsX = cms.untracked.vdouble(100, -20, 20),
            inputVariables = cms.vstring("((-(muon.vx - beamspot.x0)*muon.py + (muon.vy - beamspot.y0)*muon.px)/muon.pt)/hypot(muon.innerTrack.d0Error, hypot(beamspot.x0Error, beamspot.y0Error))"),
        ),
        cms.PSet (
            name = cms.string("muonAbsD0SigBeamspot"),
            title = cms.string("Muon |d_{0}/#sigma(d_{0})| wrt Beamspot; |d_{0}/#sigma(d_{0})|"),
            binsX = cms.untracked.vdouble(100, 0.0, 20),
            inputVariables = cms.vstring("abs(((-(muon.vx - beamspot.x0)*muon.py + (muon.vy - beamspot.y0)*muon.px)/muon.pt)/hypot(muon.innerTrack.d0Error, hypot(beamspot.x0Error, beamspot.y0Error)))"),
        ),
        cms.PSet (
            name = cms.string("muonAbsD0BeamspotVsMuonAbsD0SigBeamspot"),
            title = cms.string("Muon |d_{0}| wrt Beamspot vs. Muon |d_{0}/#sigma(d_{0})| wrt Beamspot; muon |d_{0}/#sigma(d_{0})| [cm]; muon |d_{0}| [cm]"),
            binsX = cms.untracked.vdouble(100, 0, 20),
            binsY = cms.untracked.vdouble(100, 0, 0.2),
            inputVariables = cms.vstring("abs(((-(muon.vx - beamspot.x0)*muon.py + (muon.vy - beamspot.y0)*muon.px)/muon.pt)/hypot(muon.innerTrack.d0Error, hypot(beamspot.x0Error, beamspot.y0Error)))","abs((-(muon.vx - beamspot.x0)*muon.py + (muon.vy - beamspot.y0)*muon.px)/muon.pt)"),
        ),
        cms.PSet (
            name = cms.string("muonDz"),
            title = cms.string("Muon d_{z} wrt Beamspot;muon d_{z} [cm]"),
            binsX = cms.untracked.vdouble(200, -20, 20),
            inputVariables = cms.vstring("(muon.vz - beamspot.z0) - ((muon.vx - beamspot.x0)*muon.px + (muon.vy - beamspot.y0)*muon.py)/muon.pt*(muon.pz/muon.pt)"),
        ),
        cms.PSet (
            name = cms.string("muonAbsDz"),
            title = cms.string("Muon |d_{z}| wrt Beamspot; muon |d_{z}| [cm]"),
            binsX = cms.untracked.vdouble(200, 0, 20),
            inputVariables = cms.vstring("abs((muon.vz - beamspot.z0) - ((muon.vx - beamspot.x0)*muon.px + (muon.vy - beamspot.y0)*muon.py)/muon.pt*(muon.pz/muon.pt))"),
        ),
    )
)

##############################################################################################

DiMuonHistograms = cms.PSet(
    inputCollection = cms.vstring("muons", "muons"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("diMuonInvMass"),
            title = cms.string("Di-muon Invariant Mass; M_{#mu#mu} [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass (muon, muon)"),
        ),
        cms.PSet (
            name = cms.string("diMuonInvMassZmassWindow"),
            title = cms.string("Di-muon Invariant Mass; M_{#mu#mu} [GeV]"),
            binsX = cms.untracked.vdouble(100, 50, 150),
            inputVariables = cms.vstring("invMass (muon, muon)"),
        ),
        cms.PSet (
            name = cms.string("diMuonChargeProduct"),
            title = cms.string("Di-muon Charge Product; charge_{#mu}_{1}*charge_{#mu}_{2}"),
            binsX = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("muon.charge * muon.charge"),
        ),
        cms.PSet (
            name = cms.string("diMuonDeltaPhi"),
            title = cms.string("Di-muon Phi Difference; |#Delta(#phi)|"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("abs ( deltaPhi (muon, muon) )"),
        ),
        cms.PSet (
            name = cms.string("diMuonDeltaEta"),
            title = cms.string("Di-muon Eta Difference; |#Delta(#eta)|"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("abs (muon.eta - muon.eta)"),
        ),
        cms.PSet (
            name = cms.string("diMuonDeltaR"),
            title = cms.string("Di-muon #DeltaR; #DeltaR"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR (muon, muon)"),
        ),
    )
)

##############################################################################################

# WARNING: NOT UPDATED FOR MINIAOD
MuonTauHistograms = cms.PSet(
    inputCollection = cms.vstring("muon-tau pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("MuonTauInvMass"),
            title = cms.string("Muon-Tau Invariant Mass; M_{#mu#tau} [GeV]"),
            binsX = cms.untracked.vdouble(90, 0, 180),
            inputVariables = cms.vstring("invMass"),
            ),
        cms.PSet (
            name = cms.string("MuonTauDeltaEta"),
            title = cms.string("Muon-Tau Eta Difference; |#Delta(#eta)|"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaEta"),
            ),
        cms.PSet (
            name = cms.string("MuonTauDeltaPhi"),
            title = cms.string("Muon-Tau Phi Difference; |#Delta(#phi)|"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
            ),
        cms.PSet (
            name = cms.string("MuonTauDeltaR"),
            title = cms.string("Muon-Tau #DeltaR; #DeltaR"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR"),
            ),
        )
    )

##############################################################################################

ElectronHistograms = cms.PSet(
    inputCollection = cms.vstring("electrons"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("numElectrons"),
            title = cms.string("Number of Selected Electrons; # electrons"),
            binsX = cms.untracked.vdouble(10, 0, 10),
            inputVariables = cms.vstring("number(electron)"),
        ),
        cms.PSet (
            name = cms.string("electronPt"),
            title = cms.string("Electron Transverse Momentum; electron p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("electronPt_0"),
            title = cms.string("Electron Transverse Momentum; electron p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("electronPt_1"),
            title = cms.string("Electron Transverse Momentum; electron p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(0,5,10,15,20,25,30,40,50,70,90,120,150,200,300,500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("electronPt_2"),
            title = cms.string("Electron Transverse Momentum; electron p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(0,10,20,30,50,90,150,300,500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("electronEta"),
            title = cms.string("Electron Eta; electron #eta"),
            binsX = cms.untracked.vdouble(100, -3, 3),
            inputVariables = cms.vstring("eta"),
        ),
        cms.PSet (
            name = cms.string("electronPhi"),
            title = cms.string("Electron Phi; electron #phi"),
            binsX = cms.untracked.vdouble(100, -3.15, 3.15),
            inputVariables = cms.vstring("phi"),
        ),
        cms.PSet (
            name = cms.string("electronCharge"),
            title = cms.string("Electron Charge; electron charge"),
            binsX = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("charge"),
        ),
        cms.PSet (
            name = cms.string("electronEtaPhi"),
            title = cms.string("Electron Eta vs. Phi; electron #phi; electron #eta"),
            binsX = cms.untracked.vdouble(100, -3.15, 3.15),
            binsY = cms.untracked.vdouble(100, -3, 3),
            inputVariables = cms.vstring("phi","eta"),
        ),
        cms.PSet (
            name = cms.string("electronNumberOfMissingInnerHits"),
            title = cms.string("Electron  Number of Missing Inner Hits; electron # missing inner hits"),
            binsX = cms.untracked.vdouble(30, 0, 30),
            inputVariables = cms.vstring("missingInnerHits"),
        ),
        cms.PSet (
            name = cms.string("electronDeltaEtaSuperClusterTrackAtVtx"),
            title = cms.string("Electron deltaEtaSuperClusterTrackAtVtx; #Delta#eta(SC, track) at vertex"),
            binsX = cms.untracked.vdouble(100, 0, 0.01),
            inputVariables = cms.vstring("abs(deltaEtaSuperClusterTrackAtVtx)"),
        ),
        cms.PSet (
            name = cms.string("electronDeltaPhiSuperClusterTrackAtVtx"),
            title = cms.string("Electron deltaPhiSuperClusterTrackAtVtx; #Delta#phi(SC, track) at vertex"),
            binsX = cms.untracked.vdouble(100, 0, 0.1),
            inputVariables = cms.vstring("abs(deltaPhiSuperClusterTrackAtVtx)"),
        ),
        cms.PSet (
            name = cms.string("electronFull5x5_sigmaIetaIeta"),
            title = cms.string("Electron full5x5_sigmaIetaIeta; #sigmai#etai#eta"),
            binsX = cms.untracked.vdouble(100, 0, 0.1),
            inputVariables = cms.vstring("full5x5_sigmaIetaIeta"),
        ),
        cms.PSet (
            name = cms.string("electronHadronicOverEm"),
            title = cms.string("Electron H/E; H/E"),
            binsX = cms.untracked.vdouble(100, 0, 0.5),
            inputVariables = cms.vstring("hadronicOverEm"),
        ),
        cms.PSet (
            name = cms.string("electronOoEmooP"),
            title = cms.string("Electron 1/E - 1/p; 1/E - 1/p [GeV^{-1}]"),
            binsX = cms.untracked.vdouble(100, 0, 0.5),
            inputVariables = cms.vstring("abs(1/ecalEnergy - eSuperClusterOverP/ecalEnergy)"),
        ),
        cms.PSet (
            name = cms.string("electronVtxFitConversion"),
            title = cms.string("Electron hasMatchedConversion; hasMatchedConversion"),
            binsX = cms.untracked.vdouble(2, 0, 2),
            inputVariables = cms.vstring("vtxFitConversion"),
        ),

#        cms.PSet (
#            name = cms.string("electronTkNormChi2"),
#            title = cms.string("Electron Track NormChi2; #chi^{2}"),
#            binsX = cms.untracked.vdouble(50, 0, 50),
#            inputVariables = cms.vstring("gsfTrack.normalizedChi2"),
#        ),
#        cms.PSet (
#            name = cms.string("electronTkValidHits"),
#            title = cms.string("Electron Track Number of Valid Hits; # Hits"),
#            binsX = cms.untracked.vdouble(20, 0, 20),
#            inputVariables = cms.vstring("gsfTrack.numberOfValidHits"),
#        ),
        cms.PSet (
            name = cms.string("electronPFrhoIso"),
            title = cms.string("Electron PF-based #rho-corrected Isolation; electron rel. iso."),
            binsX = cms.untracked.vdouble(100, 0, 3),
            inputVariables = cms.vstring("(pfIso_.sumChargedHadronPt + max(0.0,pfIso_.sumNeutralHadronEt + pfIso_.sumPhotonEt - rho*AEff))/pt"),
        ),
#        cms.PSet (
#            name = cms.string("electronFbrem"),
#            title = cms.string("Electron Brem. Energy Fraction; electron fbrem"),
#            binsX = cms.untracked.vdouble(100, 0, 2),
#            inputVariables = cms.vstring("fbrem"),
#        ),
#        cms.PSet (
#            name = cms.string("electronNumberOfLostHits"),
#            title = cms.string("Electon Number of Lost Hits; # lost hits"),
#            binsX = cms.untracked.vdouble(10, 0, 10),
#            inputVariables = cms.vstring("gsfTrack.numberOfLostHits"),
#        ),
    )
)

##############################################################################################

ElectronIPHistograms = cms.PSet(
    inputCollection = cms.vstring("electrons","beamspots"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("electronD0Beamspot"),
            title = cms.string("Electron d_{0} wrt Beamspot; electron d_{0} [cm]"),
            binsX = cms.untracked.vdouble(100, -0.02, 0.02),
            inputVariables = cms.vstring("(-(electron.vx - beamspot.x0)*electron.py + (electron.vy - beamspot.y0)*electron.px)/electron.pt"),
        ),
        cms.PSet (
            name = cms.string("electronAbsD0Beamspot"),
            title = cms.string("Electron |d_{0}| wrt Beamspot; electron |d_{0}| [cm]"),
            binsX = cms.untracked.vdouble(100, 0.0, 0.02),
            inputVariables = cms.vstring("abs(-(electron.vx - beamspot.x0)*electron.py + (electron.vy - beamspot.y0)*electron.px)/electron.pt"),
        ),
        cms.PSet (
            name = cms.string("electronD0SigBeamspot"),
            title = cms.string("Electron d_{0}/#sigma(d_{0}) wrt Beamspot; d_{0}/#sigma(d_{0})"),
            binsX = cms.untracked.vdouble(100, -20, 20),
            inputVariables = cms.vstring("((-(electron.vx - beamspot.x0)*electron.py + (electron.vy - beamspot.y0)*electron.px)/electron.pt)/hypot(electron.gsfTrack.d0Error, hypot(beamspot.x0Error, beamspot.y0Error))"),
        ),
        cms.PSet (
            name = cms.string("electronAbsD0SigBeamspot"),
            title = cms.string("Electron |d_{0}/#sigma(d_{0})| wrt Beamspot; |d_{0}/#sigma(d_{0})|"),
            binsX = cms.untracked.vdouble(100, 0.0, 20),
            inputVariables = cms.vstring("abs(((-(electron.vx - beamspot.x0)*electron.py + (electron.vy - beamspot.y0)*electron.px)/electron.pt)/hypot(electron.gsfTrack.d0Error, hypot(beamspot.x0Error, beamspot.y0Error)))"),
        ),
        cms.PSet (
            name = cms.string("electronDz"),
            title = cms.string("Electron d_{z} wrt Beamspot;electron d_{z} [cm]"),
            binsX = cms.untracked.vdouble(200, -20, 20),
            inputVariables = cms.vstring("(electron.vz - beamspot.z0) - ((electron.vx - beamspot.x0)*electron.px + (electron.vy - beamspot.y0)*electron.py)/electron.pt*(electron.pz/electron.pt)"),
        ),
        cms.PSet (
            name = cms.string("electronAbsDz"),
            title = cms.string("Electron |d_{z}| wrt Beamspot; electron |d_{z}| [cm]"),
            binsX = cms.untracked.vdouble(200, 0, 20),
            inputVariables = cms.vstring("abs((electron.vz - beamspot.z0) - ((electron.vx - beamspot.x0)*electron.px + (electron.vy - beamspot.y0)*electron.py)/electron.pt*(electron.pz/electron.pt))"),
        ),
    )
)

##############################################################################################

DiElectronHistograms = cms.PSet(
    inputCollection = cms.vstring("electrons", "electrons"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("diElectronInvMass"),
            title = cms.string("Di-electron Invariant Mass; M_{ee} [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass(electron, electron)"),
        ),
        cms.PSet (
            name = cms.string("diElectronChargeProduct"),
            title = cms.string("Di-electron Charge Product; charge_{e}_{1}*charge_{e}_{2}"),
            binsX = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("electron.charge * electron.charge"),
        ),
        cms.PSet (
            name = cms.string("diElectronDeltaPhi"),
            title = cms.string("Di-electron Phi Difference; |#Delta(#phi)|"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("abs(deltaPhi(electron, electron))"),
        ),
        cms.PSet (
            name = cms.string("diElectronDeltaEta"),
            title = cms.string("Di-electron Eta Difference; |#Delta(#eta)|"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("abs(electron.eta - electron.eta)"),
        ),
        cms.PSet (
            name = cms.string("diElectronDeltaR"),
            title = cms.string("Di-electron #DeltaR; #DeltaR"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR(electron, electron)"),
        ),
   )
)

##############################################################################################

ElectronMuonHistograms = cms.PSet(
    inputCollection = cms.vstring("electrons","muons"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("electronPtMuonPt"),
            title = cms.string("Electron Momentum vs Muon Momentum; muon p_{T} [GeV]; electron p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(100,0,500),
            binsY = cms.untracked.vdouble(100,0,500),
            inputVariables = cms.vstring("muon.pt","electron.pt"),
        ),
        cms.PSet (
            name = cms.string("electronMuonInvMass"),
            title = cms.string("Electron-muon Invariant Mass; M_{e#mu} [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass(electron, muon)"),
        ),
        cms.PSet (
            name = cms.string("electronMuonChargeProduct"),
            title = cms.string("Electron-muon Charge Product; charge_{e}*charge_{#mu}"),
            binsX = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("electron.charge * muon.charge"),
        ),
        cms.PSet (
            name = cms.string("electronMuonDeltaPhi"),
            title = cms.string("Electron-muon Phi Difference; |#Delta(#phi)|"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("abs(deltaPhi(electron, muon))"),
        ),
        cms.PSet (
            name = cms.string("electronMuonDeltaEta"),
            title = cms.string("Electron-muon Eta Difference; |#Delta(#eta)|"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("abs(electron.eta - muon.eta)"),
        ),
        cms.PSet (
            name = cms.string("electronMuonDeltaR"),
            title = cms.string("Electron-muon #DeltaR; #DeltaR"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR(electron, muon)"),
        ),
        cms.PSet (
            name = cms.string("electronMuonDeltaZ"),
            title = cms.string("Electron-muon #DeltaZ; #DeltaZ [cm]"),
            binsX = cms.untracked.vdouble(100, -20, 20),
            inputVariables = cms.vstring("electron.vz - muon.vz"),
        ),
#        cms.PSet (
#            name = cms.string("electronMuonThreeDAngle"),
#            title = cms.string("Electron-muon 3D angle; 3D angle"),
#            binsX = cms.untracked.vdouble(100, 0, 3.15),
#            inputVariables = cms.vstring("threeDAngle"),
#        ),

        cms.PSet (
            name = cms.string("electronRhoIsoVsMuonDbetaIso"),
            title = cms.string("Electron #rho-corrected Isolation vs. Muon #Delta#beta-corrected Isolation; muon rel. iso.; electron rel. iso."),
            binsX = cms.untracked.vdouble(100, 0, 3),
            binsY = cms.untracked.vdouble(100, 0, 3),
            inputVariables = cms.vstring("(muon.pfIsolationR04_.sumChargedHadronPt + max(0.0,muon.pfIsolationR04_.sumNeutralHadronEt + muon.pfIsolationR04_.sumPhotonEt - 0.5*muon.pfIsolationR04_.sumPUPt))/muon.pt","(electron.pfIso_.sumChargedHadronPt + max(0.0,electron.pfIso_.sumNeutralHadronEt + electron.pfIso_.sumPhotonEt - electron.rho*electron.AEff))/electron.pt"),
        ),
    )
)

##############################################################################################

ElectronMuonIPHistograms = cms.PSet(
    inputCollection = cms.vstring("electrons","muons","beamspots"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("electronD0vsMuonD0Beamspot"),
            title = cms.string("Electron d_{0} wrt Beamspot vs. Muon d_{0} wrt Beamspot; muon d_{0} [cm]; electron d_{0} [cm]"),
            binsX = cms.untracked.vdouble(100, -0.02, 0.02),
            binsY = cms.untracked.vdouble(100, -0.02, 0.02),
            inputVariables = cms.vstring("(-(muon.vx - beamspot.x0)*muon.py + (muon.vy - beamspot.y0)*muon.px)/muon.pt","(-(electron.vx - beamspot.x0)*electron.py + (electron.vy - beamspot.y0)*electron.px)/electron.pt"),
        ),
        cms.PSet (
            name = cms.string("electronAbsD0BeamspotVsMuonAbsD0Beamspot"),
            title = cms.string("Electron |d_{0}| wrt Beamspot vs. Muon |d_{0}| wrt Beamspot; muon |d_{0}| [cm]; electron |d_{0}| [cm]"),
            binsX = cms.untracked.vdouble(100, 0, 0.02),
            binsY = cms.untracked.vdouble(100, 0, 0.02),
            inputVariables = cms.vstring("abs((-(muon.vx - beamspot.x0)*muon.py + (muon.vy - beamspot.y0)*muon.px)/muon.pt)","abs((-(electron.vx - beamspot.x0)*electron.py + (electron.vy - beamspot.y0)*electron.px)/electron.pt)"),
        ),
    )
)

##############################################################################################

# WARNING: NOT UPDATED FOR MINIAOD
MuonPhotonHistograms = cms.PSet(
    inputCollection = cms.vstring("muon-photon pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("muonPhotonPt"),
            title = cms.string("Muon-photon Pair Transverse Momentum; p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("muonPhotonInvMass"),
            title = cms.string("Muon-photon Pair Invariant Mass; M_{e#mu} [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass"),
        ),
        cms.PSet (
            name = cms.string("muonPhotonDeltaPhi"),
            title = cms.string("Muon-photon Phi Difference; |#Delta(#phi)|"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
        ),
        cms.PSet (
            name = cms.string("muonPhotonDeltaEta"),
            title = cms.string("Muon-photon Eta Difference; |#Delta(#eta)|"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaEta"),
        ),
        cms.PSet (
            name = cms.string("muonPhotonDeltaR"),
            title = cms.string("Muon-photon #DeltaR; #DeltaR"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR"),
        ),
        cms.PSet (
            name = cms.string("muonPhotonThreeDAngle"),
            title = cms.string("Muon-photon 3D angle; 3D angle"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("threeDAngle"),
        ),
        cms.PSet (
            name = cms.string("muonPFdBetaIsoVsMuon-photonDeltaR"),
            title = cms.string("Muon PF-based #Delta#beta-corrected Isolation vs Muon-photon DeltaR;|muon-photon #Delta(R)|; muon rel. iso."),
            binsX = cms.untracked.vdouble(100, 0, 6),
            binsY = cms.untracked.vdouble(100, 0, 3),
            inputVariables = cms.vstring("deltaR","muonRelPFdBetaIso"),
        ),
        cms.PSet (
            name = cms.string("muonPFdBetaIsoVsMuon-photonDeltaEta"),
            title = cms.string("Muon PF-based #Delta#beta-corrected Isolation vs Muon-photon DeltaEta; |muon-photon #Delta(#eta)|; muon rel. iso."),
            binsX = cms.untracked.vdouble(100, 0, 6),
            binsY = cms.untracked.vdouble(100, 0, 3),
            inputVariables = cms.vstring("deltaEta","muonRelPFdBetaIso"),
        ),
        cms.PSet (
            name = cms.string("muonPFdBetaIsoVsMuon-photonDeltaPhi"),
            title = cms.string("Muon PF-based #Delta#beta-corrected Isolation vs Muon-photon DeltaPhi; |muon-photon #Delta(#phi)|; muon rel. iso."),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            binsY = cms.untracked.vdouble(100, 0, 3),
            inputVariables = cms.vstring("deltaPhi","muonRelPFdBetaIso"),
        ),
    )
)

##############################################################################################

# WARNING: NOT UPDATED FOR MINIAOD
ElectronPhotonHistograms = cms.PSet(
    inputCollection = cms.vstring("electron-photon pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("electronPhotonPt"),
            title = cms.string("Electron-photon Pair Transverse Momentum; p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("electronPhotonInvMass"),
            title = cms.string("Electron-photon Pair Invariant Mass; M_{e#mu} [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass"),
        ),
        cms.PSet (
            name = cms.string("electronPhotonDeltaPhi"),
            title = cms.string("Electron-photon Phi Difference; |#Delta(#phi)|"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
        ),
        cms.PSet (
            name = cms.string("electronPhotonDeltaEta"),
            title = cms.string("Electron-photon Eta Difference; |#Delta(#eta)|"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaEta"),
        ),
        cms.PSet (
            name = cms.string("electronPhotonDeltaR"),
            title = cms.string("Electron-photon #DeltaR; #DeltaR"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR"),
        ),
        cms.PSet (
            name = cms.string("electronPhotonThreeDAngle"),
            title = cms.string("Electron-photon 3D angle; 3D angle"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("threeDAngle"),
        ),
        cms.PSet (
            name = cms.string("electronPFrhoIsoVsElectron-photonDeltaR"),
            title = cms.string("Electron PF-based #Delta#beta-corrected Isolation vs Electron-photon DeltaR;|electron-photon #Delta(R)|; electron rel. iso."),
            binsX = cms.untracked.vdouble(100, 0, 6),
            binsY = cms.untracked.vdouble(100, 0, 3),
            inputVariables = cms.vstring("deltaR","electronRelPFrhoIso"),
        ),
        cms.PSet (
            name = cms.string("electronPFrhoIsoVsElectron-photonDeltaEta"),
            title = cms.string("Electron PF-based #Delta#beta-corrected Isolation vs Electron-photon DeltaEta; |electron-photon #Delta(#eta)|; electron rel. iso."),
            binsX = cms.untracked.vdouble(100, 0, 6),
            binsY = cms.untracked.vdouble(100, 0, 3),
            inputVariables = cms.vstring("deltaEta","electronRelPFrhoIso"),
        ),
        cms.PSet (
            name = cms.string("electronPFrhoIsoVsElectron-photonDeltaPhi"),
            title = cms.string("Electron PF-based #Delta#beta-corrected Isolation vs Electron-photon DeltaPhi; |electron-photon #Delta(#phi)|; electron rel. iso."),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            binsY = cms.untracked.vdouble(100, 0, 3),
            inputVariables = cms.vstring("deltaPhi","electronRelPFrhoIso"),
        ),
    )
)

############################################################################################

# WARNING: NOT UPDATED FOR MINIAOD
TauHistograms = cms.PSet(
    inputCollection = cms.vstring("taus"),
    histograms = cms.VPSet (
         cms.PSet (
             name = cms.string("tauPt"),
             title = cms.string("Tau Transverse Momentum; p_{T} [GeV]"),
             binsX = cms.untracked.vdouble(100, 0, 500),
             inputVariables = cms.vstring("pt"),
             ),
         cms.PSet (
             name = cms.string("tauEta"),
             title = cms.string("Tau Eta; #eta"),
             binsX = cms.untracked.vdouble(100, -3, 3),
             inputVariables = cms.vstring("eta"),
             ),
         cms.PSet (
             name = cms.string("tauPhi"),
             title = cms.string("Tau Phi; #phi"),
             binsX = cms.untracked.vdouble(100, -3.15, 3.15),
             inputVariables = cms.vstring("phi"),
             ),
         cms.PSet (
             name = cms.string("tauleadingTrackPt"),
             title = cms.string("Tau Leading Track Transverse Momentum; Leading Track p_{T} [GeV]"),
             binsX = cms.untracked.vdouble(100, 0, 300),
             inputVariables = cms.vstring("leadingTrackPt"),
             ),
         cms.PSet (
             name = cms.string("tauEnergy"),
             title = cms.string("Tau Energy; Energy [GeV]"),
             binsX  = cms.untracked.vdouble(100, 0, 300),
             inputVariables = cms.vstring("energy"),
             ),
         cms.PSet (
             name = cms.string("tauEMFraction"),
             title = cms.string("Tau EM Fraction; EM Fraction"),
             binsX = cms.untracked.vdouble(100, 0, 1.5),
             inputVariables = cms.vstring("emFraction"),
             ),
         cms.PSet (
             name = cms.string("tauNumProngs"),
             title = cms.string("Tau Number of Prongs; # Prongs"),
             binsX = cms.untracked.vdouble(100, -0.5, 4.5),
             inputVariables = cms.vstring("numProngs"),
             ),
         cms.PSet (
             name = cms.string("tauNumSignalGammas"),
             title = cms.string("Tau Number of Signal Gammas; # Signal Gammas"),
             binsX = cms.untracked.vdouble(100, -0.5, 12.5),
             inputVariables = cms.vstring("numSignalGammas"),
             ),
         cms.PSet (
             name = cms.string("tauNumSignalNeutrals"),
             title = cms.string("Tau Number of Signal Neutrals; # Signal Neutrals"),
             binsX = cms.untracked.vdouble(100, -0.5, 8.5),
             inputVariables = cms.vstring("numSignalNeutrals"),
             ),
         cms.PSet (
             name = cms.string("tauNumSignalPiZeros"),
             title = cms.string("Tau Number of Signal #pi^{0}'s; # Signal #pi^{0}'s"),
             binsX = cms.untracked.vdouble(100, -0.5, 8.5),
             inputVariables = cms.vstring("numSignalPiZeros"),
             ),
         cms.PSet (
             name = cms.string("tauDecayMode"),
             title = cms.string("Tau Decay Mode;Decay Mode "),
             binsX = cms.untracked.vdouble(100, -0.5, 12.5),
             inputVariables = cms.vstring("decayMode"),
             ),
         )
    )

##############################################################################################

# WARNING: NOT UPDATED FOR MINIAOD

DiTauHistograms = cms.PSet(
    inputCollection = cms.vstring("tau-tau pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("diTauInvMass"),
            title = cms.string("Di-Tau Invariant Mass; M_{#tau#tau} [GeV]"),
            binsX = cms.untracked.vdouble(90, 0, 180),
            inputVariables = cms.vstring("invMass"),
            ),
        cms.PSet (
            name = cms.string("diTauDeltaEta"),
            title = cms.string("Di-tau Eta Difference; |#Delta(#eta)|"),
            binsX = cms.untracked.vdouble(100, 0, 10),
            inputVariables = cms.vstring("deltaEta"),
            ),
        cms.PSet (
            name = cms.string("diTauDeltaPhi"),
            title = cms.string("Di-tau Phi Difference; |#Delta(#phi)|"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
            ),
        cms.PSet (
            name = cms.string("diTauDeltaR"),
            title = cms.string("Di-tau #DeltaR; #DeltaR"),
            binsX = cms.untracked.vdouble(100, 0, 10),
            inputVariables = cms.vstring("deltaR"),
            ),

        )
    )

##############################################################################################

JetHistograms = cms.PSet(
    inputCollection = cms.vstring("jets"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("numJets"),
            title = cms.string("Number of Selected Jets; # jets"),
            binsX = cms.untracked.vdouble(10, 0, 10),
            inputVariables = cms.vstring("number(jet)"),
        ),
        cms.PSet (
            name = cms.string("jetPt"),
            title = cms.string("Jet Transverse Momentum;jet p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("jetEta"),
            title = cms.string("Jet Eta;jet #eta"),
            binsX = cms.untracked.vdouble(100, -3, 3),
            inputVariables = cms.vstring("eta"),
        ),
        cms.PSet (
            name = cms.string("jetPhi"),
            title = cms.string("Jet Phi;jet #phi"),
            binsX = cms.untracked.vdouble(100, -3.15, 3.15),
            inputVariables = cms.vstring("phi"),
        ),
        cms.PSet (
            name = cms.string("jetCharge"),
            title = cms.string("Jet Charge;jet charge"),
            binsX = cms.untracked.vdouble(41, -20.5, 20.5),
            inputVariables = cms.vstring("charge"),
        ),
        cms.PSet (
            name = cms.string("jetEtaPhi"),
            title = cms.string("Jet Eta vs. Phi;jet #phi;jet #eta"),
            binsX = cms.untracked.vdouble(100, -3.15, 3.15),
            binsY = cms.untracked.vdouble(100, -3, 3),
            inputVariables = cms.vstring("phi","eta"),
        ),
        # cms.PSet (
        #     name = cms.string("jetFlavor"),
        #     title = cms.string("Jet Flavor"),
        #     binsX = cms.untracked.vdouble(42, -21, 21),
        #     inputVariables = cms.vstring("flavour"),
        # ),
        cms.PSet (
            name = cms.string("jetChargedHadronEnergyFraction"),
            title = cms.string("Jet Charged Hadron Fraction;jet charged hadron energy fraction"),
            binsX = cms.untracked.vdouble(101, 0.0, 1.0 + (1.0 / 100.0)),
            inputVariables = cms.vstring("chargedHadronEnergyFraction"),
        ),
        cms.PSet (
            name = cms.string("jetNeutralHadronEnergyFraction"),
            title = cms.string("Jet Neutral Hadron Fraction;jet neutral hadron energy fraction"),
            binsX = cms.untracked.vdouble(101, 0.0, 1.0 + (1.0 / 100.0)),
            inputVariables = cms.vstring("neutralHadronEnergyFraction"),
        ),
        cms.PSet (
            name = cms.string("jetNeutralEMEnergyFraction"),
            title = cms.string("Jet Neutral EM Fraction;jet neutral EM energy fraction"),
            binsX = cms.untracked.vdouble(101, 0.0, 1.0 + (1.0 / 100.0)),
            inputVariables = cms.vstring("neutralEmEnergyFraction"),
        ),
        cms.PSet (
            name = cms.string("jetChargedEMEnergyFraction"),
            title = cms.string("Jet Charged EM Fraction;jet charged EM energy fraction"),
            binsX = cms.untracked.vdouble(101, 0.0, 1.0 + (1.0 / 100.0)),
            inputVariables = cms.vstring("chargedEmEnergyFraction"),
        ),
        cms.PSet (
            name = cms.string("jetCSV"),
            title = cms.string("Jet Combined Secondary Vertex B-tagging Discriminant"),
            binsX = cms.untracked.vdouble(100, 0, 1),
            inputVariables = cms.vstring("pfCombinedInclusiveSecondaryVertexV2BJetTags"),
        ),
        # cms.PSet (
        #     name = cms.string("jetMetDeltaPhi"),
        #     title = cms.string("Jet-MET Delta Phi; |#Delta(#phi)|"),
        #     binsX = cms.untracked.vdouble(100, 0, 3.15),
        #     inputVariables = cms.vstring("dPhiMet"),
        # ),
    )
)

#####################################################################################

BjetHistograms = cms.PSet(
    inputCollection = cms.vstring("bjets"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("numBjets"),
            title = cms.string("Number of Selected Bjets; # bjets"),
            binsX = cms.untracked.vdouble(10, 0, 10),
            inputVariables = cms.vstring("number(bjet)"),
        ),
        cms.PSet (
            name = cms.string("bjetPt"),
            title = cms.string("Bjet Transverse Momentum;bjet p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("bjetEta"),
            title = cms.string("Bjet Eta;bjet #eta"),
            binsX = cms.untracked.vdouble(100, -3, 3),
            inputVariables = cms.vstring("eta"),
        ),
        cms.PSet (
            name = cms.string("bjetPhi"),
            title = cms.string("Bjet Phi;bjet #phi"),
            binsX = cms.untracked.vdouble(100, -3.15, 3.15),
            inputVariables = cms.vstring("phi"),
        ),
        cms.PSet (
            name = cms.string("bjetCharge"),
            title = cms.string("Bjet Charge;bjet charge"),
            binsX = cms.untracked.vdouble(41, -20.5, 20.5),
            inputVariables = cms.vstring("charge"),
        ),
        cms.PSet (
            name = cms.string("bjetEtaPhi"),
            title = cms.string("Bjet Eta vs. Phi;bjet #phi;bjet #eta"),
            binsX = cms.untracked.vdouble(100, -3.15, 3.15),
            binsY = cms.untracked.vdouble(100, -3, 3),
            inputVariables = cms.vstring("phi","eta"),
        ),
        # cms.PSet (
        #     name = cms.string("bjetFlavor"),
        #     title = cms.string("Bjet Flavor"),
        #     binsX = cms.untracked.vdouble(42, -21, 21),
        #     inputVariables = cms.vstring("flavour"),
        # ),
        cms.PSet (
            name = cms.string("bjetChargedHadronEnergyFraction"),
            title = cms.string("Bjet Charged Hadron Fraction;bjet charged hadron energy fraction"),
            binsX = cms.untracked.vdouble(101, 0.0, 1.0 + (1.0 / 100.0)),
            inputVariables = cms.vstring("chargedHadronEnergyFraction"),
        ),
        cms.PSet (
            name = cms.string("bjetNeutralHadronEnergyFraction"),
            title = cms.string("Bjet Neutral Hadron Fraction;bjet neutral hadron energy fraction"),
            binsX = cms.untracked.vdouble(101, 0.0, 1.0 + (1.0 / 100.0)),
            inputVariables = cms.vstring("neutralHadronEnergyFraction"),
        ),
        cms.PSet (
            name = cms.string("bjetNeutralEMEnergyFraction"),
            title = cms.string("Bjet Neutral EM Fraction;bjet neutral EM energy fraction"),
            binsX = cms.untracked.vdouble(101, 0.0, 1.0 + (1.0 / 100.0)),
            inputVariables = cms.vstring("neutralEmEnergyFraction"),
        ),
        cms.PSet (
            name = cms.string("bjetChargedEMEnergyFraction"),
            title = cms.string("Bjet Charged EM Fraction;bjet charged EM energy fraction"),
            binsX = cms.untracked.vdouble(101, 0.0, 1.0 + (1.0 / 100.0)),
            inputVariables = cms.vstring("chargedEmEnergyFraction"),
        ),
        cms.PSet (
            name = cms.string("bjetCSV"),
            title = cms.string("Bjet Combined Secondary Vertex B-tagging Discriminant"),
            binsX = cms.untracked.vdouble(100, 0, 1),
            inputVariables = cms.vstring("pfCombinedInclusiveSecondaryVertexV2BJetTags"),
        ),
    )
)

#####################################################################################

JetIsoHistograms = cms.PSet(
    inputCollection = cms.vstring("jets"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("jetChargedHadronEnergy"),
            title = cms.string("Charged Hadron Energy Deposited Within the Jet; E [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("chargedHadronEnergy"),
        ),
        cms.PSet (
            name = cms.string("jetNeutralHadronEnergy"),
            title = cms.string("Neutral Hadron Energy Deposited Within the Jet; E [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("neutralHadronEnergy"),
        ),
        cms.PSet (
            name = cms.string("jetChargedEmEnergy"),
            title = cms.string("Charged Em Energy Deposited Near the Jet; E [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("chargedEmEnergy"),
        ),
        cms.PSet (
            name = cms.string("jetNeutralEmEnergy"),
            title = cms.string("Neutral Em Energy Deposited Near the Jet; E [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("neutralEmEnergy"),
        ),
    )
)

#####################################################################################
# WARNING: NOT UPDATED FOR MINIAOD
DiJetHistograms = cms.PSet(
    inputCollection = cms.vstring("jet-jet pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("diJetInvMass"),
            title = cms.string("Di-jet Invariant Mass; M_{jet-pair} [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass"),
        ),
        cms.PSet (
            name = cms.string("diJetChargeProduct"),
            title = cms.string("Di-jet Charge Product; charge_{jet}_{1}*charge_{jet}_{2}"),
            binsX = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("chargeProduct"),
        ),
        cms.PSet (
            name = cms.string("diJetDeltaPhi"),
            title = cms.string("Di-jet Phi Difference; |#Delta(#phi)|"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
        ),
        cms.PSet (
            name = cms.string("diJetDeltaEta"),
            title = cms.string("Di-jet Eta Difference; |#Delta(#eta)|"),
            binsX = cms.untracked.vdouble(100, 0, 10),
            inputVariables = cms.vstring("deltaEta"),
        ),
        cms.PSet (
            name = cms.string("diJetDeltaR"),
            title = cms.string("Di-jet #DeltaR; #DeltaR"),
            binsX = cms.untracked.vdouble(100, 0, 10),
            inputVariables = cms.vstring("deltaR"),
        ),
        cms.PSet (
            name = cms.string("diJetPt"),
            title = cms.string("Di-jet Transverse Momentum ; p_{T}_{jet-pair} [GeV] "),
            binsX = cms.untracked.vdouble(100, 0,500),
            inputVariables = cms.vstring("pt"),
        ),
        ## cms.PSet (
        ##     name = cms.string("diJetDeltaPt"),
        ##     title = cms.string("Di-jet Transverse Momentum Difference; |p_{T}_{jet1}-p_{T}_{jet2}| [GeV] "),
        ##     binsX = cms.untracked.vdouble(100, 0,500),
        ##     inputVariables = cms.vstring("absDeltaPt"),  # Variable not available in OSUAnalysis.cc
        ## ),
        cms.PSet (
            name = cms.string("diJetThreeDAngle"),
            title = cms.string("Di-jet 3D angle; 3D angle"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("threeDAngle"),
        ),
    )
)

##########################################################################################

JetBjetHistograms = cms.PSet(
    inputCollection = cms.vstring("jets","bjets"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("jetBjetInvMass"),
            title = cms.string("Jet-bjet Invariant Mass; M(jet,bjet) [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass(jet,bjet)"),
        ),
        cms.PSet (
            name = cms.string("jetBjetDeltaPhi"),
            title = cms.string("Jet-bjet Phi Difference; |#Delta#phi(jet,bjet)|"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("abs(deltaPhi(jet,bjet))"),
        ),
        cms.PSet (
            name = cms.string("jetBjetDeltaEta"),
            title = cms.string("Jet-bjet Eta Difference; |#Delta#eta(jet,bjet)|"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("abs(jet.eta - bjet.eta)"),
        ),
        cms.PSet (
            name = cms.string("jetBjetDeltaR"),
            title = cms.string("Jet-bjet #DeltaR; #DeltaR(jet,bjet)"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR(jet,bjet)"),
        ),
        cms.PSet (
            name = cms.string("jetBjetDeltaZ"),
            title = cms.string("Jet-bjet Z Difference; |#Deltaz(jet,bjet)| [cm]"),
            binsX = cms.untracked.vdouble(100, 0, 20),
            inputVariables = cms.vstring("abs(jet.vz - bjet.vz)"),
        ),
        cms.PSet (
            name = cms.string("jetBjetDeltaRvsjetBjetDeltaEta"),
            title = cms.string("Jet-bjet #DeltaR vs #Delta#eta; |#Delta#eta(jet,bjet)|; #DeltaR(jet,bjet)"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            binsY = cms.untracked.vdouble(100, 0 ,6),
            inputVariables = cms.vstring("abs(jet.eta - bjet.eta)","deltaR(jet,bjet)"),
        ),
        cms.PSet (
            name = cms.string("jetBjetDeltaRvsjetBjetDeltaPhi"),
            title = cms.string("Jet-bjet #DeltaR vs #Delta#phi; |#Delta#phi(jet,bjet)|; #DeltaR(jet,bjet)"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            binsY = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("abs(deltaPhi(jet,bjet))","deltaR(jet,bjet)"),
        ),
        cms.PSet (
            name = cms.string("jetEtavsBjetEta"),
            title = cms.string("Jet Eta.vs Bjet Eta; Bjet #eta; Jet #eta"),
            binsX = cms.untracked.vdouble(100, -6, 6),
            binsY = cms.untracked.vdouble(100, -6 ,6),
            inputVariables = cms.vstring("bjet.eta","jet.eta"),
        ),
        cms.PSet (
            name = cms.string("jetPhivsBjetPhi"),
            title = cms.string("Jet Phi.vs Bjet Phi; Bjet #phi; Jet #phi"),
            binsX = cms.untracked.vdouble(100, -3.15, 3.15),
            binsY = cms.untracked.vdouble(100, -3.15 ,3.15),
            inputVariables = cms.vstring("bjet.phi","jet.phi"),
        ),
    )
)

##########################################################################################
##########################################################################################

MuonJetHistograms = cms.PSet(
    inputCollection = cms.vstring("muons","jets"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("muonJetInvMass"),
            title = cms.string("Muon-jet Invariant Mass; M(#mu,jet) [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass(muon,jet)"),
        ),
        cms.PSet (
            name = cms.string("muonJetDeltaPhi"),
            title = cms.string("Muon-jet Phi Difference; |#Delta#phi(#mu,jet)|"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("abs(deltaPhi(muon,jet))"),
        ),
        cms.PSet (
            name = cms.string("muonJetDeltaEta"),
            title = cms.string("Muon-jet Eta Difference; |#Delta#eta(#mu,jet)|"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("abs(muon.eta - jet.eta)"),
        ),
        cms.PSet (
            name = cms.string("muonJetDeltaR"),
            title = cms.string("Muon-jet #DeltaR; #DeltaR(#mu,jet)"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR(muon,jet)"),
        ),
        cms.PSet (
            name = cms.string("muonJetDeltaZ"),
            title = cms.string("Muon-jet Z Difference; |#Deltaz(#mu,jet)| [cm]"),
            binsX = cms.untracked.vdouble(100, 0, 20),
            inputVariables = cms.vstring("abs(muon.vz - jet.vz)"),
        ),
        cms.PSet (
            name = cms.string("muonJetDeltaRvsmuonJetDeltaEta"),
            title = cms.string("Muon-jet #DeltaR vs #Delta#eta; |#Delta#eta(#mu,jet)|; #DeltaR(#mu,jet)"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            binsY = cms.untracked.vdouble(100, 0 ,6),
            inputVariables = cms.vstring("abs(muon.eta - jet.eta)","deltaR(muon,jet)"),
        ),
        cms.PSet (
            name = cms.string("muonJetDeltaRvsmuonJetDeltaPhi"),
            title = cms.string("Muon-jet #DeltaR vs #Delta#phi; |#Delta#phi(#mu,jet)|; #DeltaR(#mu,jet)"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            binsY = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("abs(deltaPhi(muon,jet))","deltaR(muon,jet)"),
        ),
        cms.PSet (
            name = cms.string("muonEtavsJetEta"),
            title = cms.string("Muon Eta.vs Jet Eta; Jet #eta; Muon #eta"),
            binsX = cms.untracked.vdouble(100, -6, 6),
            binsY = cms.untracked.vdouble(100, -6 ,6),
            inputVariables = cms.vstring("jet.eta","muon.eta"),
        ),
        cms.PSet (
            name = cms.string("muonPhivsJetPhi"),
            title = cms.string("Muon Phi.vs Jet Phi; Jet #phi; Muon #phi"),
            binsX = cms.untracked.vdouble(100, -3.15, 3.15),
            binsY = cms.untracked.vdouble(100, -3.15 ,3.15),
            inputVariables = cms.vstring("jet.phi","muon.phi"),
        ),
    )
)

##########################################################################################
##########################################################################################

ElectronJetHistograms = cms.PSet(
    inputCollection = cms.vstring("electrons","jets"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("electronJetInvMass"),
            title = cms.string("Electron-jet Invariant Mass; M(e,jet) [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass(electron,jet)"),
        ),
        cms.PSet (
            name = cms.string("electronJetDeltaPhi"),
            title = cms.string("Electron-jet Phi Difference; |#Delta#phi(e,jet)|"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("abs(deltaPhi(electron,jet))"),
        ),
        cms.PSet (
            name = cms.string("electronJetDeltaEta"),
            title = cms.string("Electron-jet Eta Difference; |#Delta#eta(e,jet)|"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("abs(electron.eta - jet.eta)"),
        ),
        cms.PSet (
            name = cms.string("electronJetDeltaR"),
            title = cms.string("Electron-jet #DeltaR; #DeltaR(e,jet)"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR(electron,jet)"),
        ),
        cms.PSet (
            name = cms.string("electronJetDeltaZ"),
            title = cms.string("Electron-jet Z Difference; |#Deltaz(e,jet)| [cm]"),
            binsX = cms.untracked.vdouble(100, 0, 20),
            inputVariables = cms.vstring("abs(electron.vz - jet.vz)"),
        ),
        cms.PSet (
            name = cms.string("electronJetDeltaRvselectronJetDeltaEta"),
            title = cms.string("Electron-jet #DeltaR vs #Delta#eta; |#Delta#eta(e,jet)|; #DeltaR(e,jet)"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            binsY = cms.untracked.vdouble(100, 0 ,6),
            inputVariables = cms.vstring("abs(electron.eta - jet.eta)","deltaR(electron,jet)"),
        ),
        cms.PSet (
            name = cms.string("electronJetDeltaRvselectronJetDeltaPhi"),
            title = cms.string("Electron-jet #DeltaR vs #Delta#phi; |#Delta#phi(e,jet)|; #DeltaR(e,jet)"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            binsY = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("abs(deltaPhi(electron,jet))","deltaR(electron,jet)"),
        ),
        cms.PSet (
            name = cms.string("electronEtavsJetEta"),
            title = cms.string("Electron Eta.vs Jet Eta; Jet #eta; Electron #eta"),
            binsX = cms.untracked.vdouble(100, -6, 6),
            binsY = cms.untracked.vdouble(100, -6 ,6),
            inputVariables = cms.vstring("jet.eta","electron.eta"),
        ),
        cms.PSet (
            name = cms.string("electronPhivsJetPhi"),
            title = cms.string("Electron Phi.vs Jet Phi; Jet #phi; Electron #phi"),
            binsX = cms.untracked.vdouble(100, -3.15, 3.15),
            binsY = cms.untracked.vdouble(100, -3.15 ,3.15),
            inputVariables = cms.vstring("jet.phi","electron.phi"),
        ),
    )
)

##########################################################################################

MuonBjetHistograms = cms.PSet(
    inputCollection = cms.vstring("muons","bjets"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("muonBjetInvMass"),
            title = cms.string("Muon-bjet Invariant Mass; M(#mu,bjet) [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass(muon,bjet)"),
        ),
        cms.PSet (
            name = cms.string("muonBjetDeltaPhi"),
            title = cms.string("Muon-bjet Phi Difference; |#Delta#phi(#mu,bjet)|"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("abs(deltaPhi(muon,bjet))"),
        ),
        cms.PSet (
            name = cms.string("muonBjetDeltaEta"),
            title = cms.string("Muon-bjet Eta Difference; |#Delta#eta(#mu,bjet)|"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("abs(muon.eta - bjet.eta)"),
        ),
        cms.PSet (
            name = cms.string("muonBjetDeltaR"),
            title = cms.string("Muon-bjet #DeltaR; #DeltaR(#mu,bjet)"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR(muon,bjet)"),
        ),
        cms.PSet (
            name = cms.string("muonBjetDeltaZ"),
            title = cms.string("Muon-bjet Z Difference; |#Deltaz(#mu,bjet)| [cm]"),
            binsX = cms.untracked.vdouble(100, 0, 20),
            inputVariables = cms.vstring("abs(muon.vz - bjet.vz)"),
        ),
        cms.PSet (
            name = cms.string("muonBjetDeltaRvsmuonBjetDeltaEta"),
            title = cms.string("Muon-bjet #DeltaR vs #Delta#eta; |#Delta#eta(#mu,bjet)|; #DeltaR(#mu,bjet)"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            binsY = cms.untracked.vdouble(100, 0 ,6),
            inputVariables = cms.vstring("abs(muon.eta - bjet.eta)","deltaR(muon,bjet)"),
        ),
        cms.PSet (
            name = cms.string("muonBjetDeltaRvsmuonBjetDeltaPhi"),
            title = cms.string("Muon-bjet #DeltaR vs #Delta#phi; |#Delta#phi(#mu,bjet)|; #DeltaR(#mu,bjet)"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            binsY = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("abs(deltaPhi(muon,bjet))","deltaR(muon,bjet)"),
        ),
        cms.PSet (
            name = cms.string("muonEtavsBjetEta"),
            title = cms.string("Muon Eta.vs Bjet Eta; Bjet #eta; Muon #eta"),
            binsX = cms.untracked.vdouble(100, -6, 6),
            binsY = cms.untracked.vdouble(100, -6 ,6),
            inputVariables = cms.vstring("bjet.eta","muon.eta"),
        ),
        cms.PSet (
            name = cms.string("muonPhivsBjetPhi"),
            title = cms.string("Muon Phi.vs Bjet Phi; Bjet #phi; Muon #phi"),
            binsX = cms.untracked.vdouble(100, -3.15, 3.15),
            binsY = cms.untracked.vdouble(100, -3.15 ,3.15),
            inputVariables = cms.vstring("bjet.phi","muon.phi"),
        ),
    )
)

##########################################################################
PhotonHistograms = cms.PSet(
    inputCollection = cms.vstring("photons"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("photonPt"),
            title = cms.string("Photon Transverse Momentum; p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("photonEta"),
            title = cms.string("Photon Eta; #eta"),
            binsX = cms.untracked.vdouble(100, -3, 3),
            inputVariables = cms.vstring("eta"),
        ),
        cms.PSet (
            name = cms.string("photonGenEta"),
            title = cms.string("Photon Gen. Eta; #eta"),
            binsX = cms.untracked.vdouble(100, -3, 3),
            inputVariables = cms.vstring("genEta"),
        ),
        cms.PSet (
            name = cms.string("photonPhi"),
            title = cms.string("Photon Phi; #phi"),
            binsX = cms.untracked.vdouble(100, -3.15, 3.15),
            inputVariables = cms.vstring("phi"),
        ),
        cms.PSet (
            name = cms.string("photonEtaPhi"),
            title = cms.string("Photon Eta vs. Phi; #phi; #eta"),
            binsX = cms.untracked.vdouble(100, -3.15, 3.15),
            binsY = cms.untracked.vdouble(100, -3, 3),
            inputVariables = cms.vstring("phi","eta"),
        ),
    )
)


############################################################################
# WARNING: NOT UPDATED FOR MINIAOD
PhotonJetHistograms = cms.PSet(
    inputCollection = cms.vstring("photon-jet pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("photonJetPt"),
            title = cms.string("Photon-Jet Pair Transverse Momentum; p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("photonJetInvMass"),
            title = cms.string("Photon-jet Invariant Mass; M_{#mu-jet} [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("invMass"),
        ),
        cms.PSet (
            name = cms.string("photonJetDeltaPhi"),
            title = cms.string("Photon-jet Phi Difference; |#Delta(#phi)|"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
        ),
        cms.PSet (
            name = cms.string("photonJetDeltaEta"),
            title = cms.string("Photon-jet Eta Difference; |#Delta(#eta)|"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaEta"),
        ),
        cms.PSet (
            name = cms.string("photonJetDeltaRvsphotonJetDeltaEta "),
            title = cms.string("Photon-jet #DeltaR.vs #Delta(eta); |#Delta(#eta)|; #DeltaR"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            binsY = cms.untracked.vdouble(100, 0 ,6),
            inputVariables = cms.vstring("deltaEta","deltaR"),
        ),
        cms.PSet (
            name = cms.string("photonJetDeltaRvsphotonJetDeltaPhi "),
            title = cms.string("Photon-jet #DeltaR.vs #Delta(#phi); |#Delta(#phi)|; #DeltaR"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            binsY = cms.untracked.vdouble(100, 0 ,6),
            inputVariables = cms.vstring("deltaPhi","deltaR"),
        ),
        cms.PSet (
            name = cms.string("photonEtavsJetEta "),
            title = cms.string("Photon Eta.vs Jet Eta; Jet #eta; Photon #eta"),
            binsX = cms.untracked.vdouble(100, -6, 6),
            binsY = cms.untracked.vdouble(100, -6 ,6),
            inputVariables = cms.vstring("jetEta","photonEta"),
        ),
        cms.PSet (
            name = cms.string("photonPhivsJetPhi "),
            title = cms.string("Photon Phi.vs Jet Phi; Jet #phi; Photon #phi"),
            binsX = cms.untracked.vdouble(100, -3.15, 3.15),
            binsY = cms.untracked.vdouble(100, -3.15 ,3.15),
            inputVariables = cms.vstring("jetPhi","photonPhi"),
        ),
        cms.PSet (
            name = cms.string("photonJetDeltaR"),
            title = cms.string("Photon-jet #DeltaR; #DeltaR"),
            binsX = cms.untracked.vdouble(100, 0, 6),
            inputVariables = cms.vstring("deltaR"),
        ),
        cms.PSet (
            name = cms.string("photonJetThreeDAngle"),
            title = cms.string("Photon-jet 3D angle; 3D angle"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("threeDAngle"),
        ),
    )
)
#######################################################################################
# WARNING: NOT UPDATED FOR MINIAOD
MCParticleHistograms = cms.PSet(
    inputCollection = cms.vstring("mcparticles"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("mcparticlePt"),
            title = cms.string("MCparticle Transverse Momentum; p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("mcparticleEta"),
            title = cms.string("MCparticle Eta; #eta"),
            binsX = cms.untracked.vdouble(100, -5, 5),
            inputVariables = cms.vstring("eta"),
        ),
        cms.PSet (
            name = cms.string("mcparticleStatus"),
            title = cms.string("MCparticle Status; status"),
            binsX = cms.untracked.vdouble(3, 1, 4),
            inputVariables = cms.vstring("status"),
        ),
        cms.PSet (
            name = cms.string("mcparticleAbsD0Vertex"),
            title = cms.string("MCParticle |d_{0}| wrt PV; |d_{0}| [cm]"),
            binsX = cms.untracked.vdouble(100, 0, 0.2),
            inputVariables = cms.vstring("abs(d0Vertex)")
        ),
        cms.PSet (
            name = cms.string("mcparticleAbsD0Beamspot"),
            title = cms.string("MCParticle |d_{0}| wrt Beamspot; |d_{0}| [cm]"),
            binsX = cms.untracked.vdouble(100, 0, 0.2),
            inputVariables = cms.vstring("abs(d0Beamspot)")
        ),
        cms.PSet (
            name = cms.string("mcparticleAbsD0Origin"),
            title = cms.string("MCParticle |d_{0}| wrt Origin; |d_{0}| [cm]"),
            binsX = cms.untracked.vdouble(100, 0, 0.2),
            inputVariables = cms.vstring("abs(d0Origin)")
        ),
        cms.PSet (
            name = cms.string("mcparticleDz"),
            title = cms.string("MCparticle d_{z}; d_{z} [cm]"),
            binsX = cms.untracked.vdouble(100, -20, 20),
            inputVariables = cms.vstring("dz"),
        ),
        cms.PSet (
            name = cms.string("mcparticleAbsDZ"),
            title = cms.string("MCparticle d_{z}; |d_{z}| [cm]"),
            binsX = cms.untracked.vdouble(100, 0, 20),
            inputVariables = cms.vstring("abs(dz)"),
        ),
    )
)


##############################################################################################

MetHistograms = cms.PSet(
    inputCollection = cms.vstring("mets"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("metPt"),
            title = cms.string("Missing E_{T};E_{T}^{miss} [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
            name = cms.string("metPhi"),
            title = cms.string("Phi of Missing E_{T};#phi(E_{T}^{miss})"),
            binsX = cms.untracked.vdouble(100, -3.15, 3.15),
            inputVariables = cms.vstring("phi"),
        ),
    )
)

##############################################################################################

TrackHistograms = cms.PSet(
     inputCollection = cms.vstring("tracks"),
     histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("trackPt"),
            title = cms.string("Track Transverse Momentum;track p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(100, 10, 510),
            inputVariables = cms.vstring("pt"),
        ),
        cms.PSet (
             name = cms.string("trackEta"),
             title = cms.string("Track Eta;track #eta"),
             binsX = cms.untracked.vdouble(100, -3, 3),
             inputVariables = cms.vstring("eta"),
        ),
        cms.PSet (
             name = cms.string("trackEtaMag"),
             title = cms.string("Track Eta;track |#eta|"),
             binsX = cms.untracked.vdouble(50, 0, 3),
             inputVariables = cms.vstring("fabs(eta)"),
        ),
        cms.PSet (
             name = cms.string("trackPhi"),
             title = cms.string("Track Phi;track #phi"),
             binsX = cms.untracked.vdouble(100, -3.15, 3.15),
             inputVariables = cms.vstring("phi"),
        ),
        cms.PSet (
            name = cms.string("trackEtaVsPhi"),
            title = cms.string("#eta vs #phi; #eta; #phi"),
            binsX = cms.untracked.vdouble(100, -3, 3),
            binsY = cms.untracked.vdouble(100, -3.15, 3.15),
            inputVariables = cms.vstring("eta", "phi"),
        ),
        cms.PSet (
            name = cms.string("trackd0"),
            title = cms.string("Track d_{0};track d_{0} [cm]"),
            binsX = cms.untracked.vdouble(100, -0.5, 0.5),
            inputVariables = cms.vstring("d0"),
        ),
        cms.PSet (
            name = cms.string("trackd0Mag"),
            title = cms.string("Track d_{0};track |d_{0}| [cm]"),
            binsX = cms.untracked.vdouble(50, 0.0, 0.5),
            inputVariables = cms.vstring("fabs(d0)"),
        ),
        cms.PSet (
            name = cms.string("trackdz"),
            title = cms.string("Track d_{z};track d_{z} [cm]"),
            binsX = cms.untracked.vdouble(100, -30, 30),
            inputVariables = cms.vstring("dz"),
        ),
        cms.PSet (
            name = cms.string("trackdzMag"),
            title = cms.string("Track d_{z};track |d_{z}| [cm]"),
            binsX = cms.untracked.vdouble(50, 0, 30),
            inputVariables = cms.vstring("fabs(dz)"),
        ),
        cms.PSet (
            name = cms.string("trackNumValidHits"),
            title = cms.string("Track Number of Valid Hits;number of valid hits"),
            binsX = cms.untracked.vdouble(31, -0.5, 30.5),
            inputVariables = cms.vstring("numberOfValidHits"),
        ),
        cms.PSet (
            name = cms.string("trackChi2"),
            title = cms.string("Track Reduced Chi2;track #chi^{2}/ndf"),
            binsX = cms.untracked.vdouble(100, 0, 30),
            inputVariables = cms.vstring("normalizedChi2"),
        ),
        cms.PSet (
            name = cms.string("trackCharge"),
            title = cms.string("Track Charge;track charge"),
            binsX = cms.untracked.vdouble(3, -1.5, 1.5),
            inputVariables = cms.vstring("charge"),
        ),
        cms.PSet (
            name = cms.string("genMatchedPromptFinalStateIsMatched"),
            title = cms.string(";track is matched to generator particle"),
            binsX = cms.untracked.vdouble(2.0, -0.5, 1.5),
            inputVariables = cms.vstring("genMatchedParticle.promptFinalState.isNonnull"),
        ),
        cms.PSet (
            name = cms.string("genMatchedPromptFinalStatePdgId"),
            title = cms.string(";PDG ID of matched generator particle"),
            binsX = cms.untracked.vdouble(getPdgBins(["quarks", "leptons", "bosons", "hadrons"])),
            inputVariables = cms.vstring("abs (genMatchedParticle.promptFinalState.pdgId)"),
        ),
        cms.PSet (
            name = cms.string("genMatchedPromptFinalStatePdgIdNoHadrons"),
            title = cms.string(";PDG ID of matched generator particle"),
            binsX = cms.untracked.vdouble(getPdgBins(["quarks", "leptons", "bosons"])),
            inputVariables = cms.vstring("abs (genMatchedParticle.promptFinalState.pdgId)"),
        ),
        cms.PSet (
            name = cms.string("genMatchedDirectPromptTauDecayProductFinalStateIsMatched"),
            title = cms.string(";track is matched to generator #tau decay product"),
            binsX = cms.untracked.vdouble(2.0, -0.5, 1.5),
            inputVariables = cms.vstring("genMatchedParticle.directPromptTauDecayProductFinalState.isNonnull"),
        ),
        cms.PSet (
            name = cms.string("genMatchedDirectPromptTauDecayProductFinalStatePdgId"),
            title = cms.string(";PDG ID of matched generator #tau decay product"),
            binsX = cms.untracked.vdouble(getPdgBins(["quarks", "leptons", "bosons", "hadrons"])),
            inputVariables = cms.vstring("abs (genMatchedParticle.directPromptTauDecayProductFinalState.pdgId)"),
        ),
        cms.PSet (
            name = cms.string("genMatchedDirectPromptTauDecayProductFinalStatePdgIdNoHadrons"),
            title = cms.string(";PDG ID of matched generator #tau decay product"),
            binsX = cms.untracked.vdouble(getPdgBins(["quarks", "leptons", "bosons"])),
            inputVariables = cms.vstring("abs (genMatchedParticle.directPromptTauDecayProductFinalState.pdgId)"),
        ),
    )
)

##############################################################################################

TrackBeamspotHistograms = cms.PSet(
     inputCollection = cms.vstring("tracks", "beamspots"),
     histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("trackd0"),
            title = cms.string("Track d_{0};track d_{0} [cm]"),
            binsX = cms.untracked.vdouble(100, -0.5, 0.5),
            inputVariables = cms.vstring("((track.vx - beamspot.x0) * track.py - (track.vy - beamspot.y0) * track.px) / track.pt"),
        ),
        cms.PSet (
            name = cms.string("trackd0Mag"),
            title = cms.string("Track d_{0};track |d_{0}| [cm]"),
            binsX = cms.untracked.vdouble(50, 0, 0.5),
            inputVariables = cms.vstring("fabs(((track.vx - beamspot.x0) * track.py - (track.vy - beamspot.y0) * track.px) / track.pt)"),
        ),
        cms.PSet (
            name = cms.string("trackdz"),
            title = cms.string("Track d_{z};track d_{z} [cm]"),
            binsX = cms.untracked.vdouble(100, -30, 30),
            inputVariables = cms.vstring("(track.vz - beamspot.z0) - ((track.vx - beamspot.x0) * track.px + (track.vy - beamspot.y0) * track.py) / track.pt * (track.pz / track.pt)"),
        ),
        cms.PSet (
            name = cms.string("trackdzMag"),
            title = cms.string("Track d_{z};track |d_{z}| [cm]"),
            binsX = cms.untracked.vdouble(50, 0, 30),
            inputVariables = cms.vstring("fabs((track.vz - beamspot.z0) - ((track.vx - beamspot.x0) * track.px + (track.vy - beamspot.y0) * track.py) / track.pt * (track.pz / track.pt))"),
        ),
    )
)

##############################################################################################

ElectronTrackHistograms = cms.PSet(
    inputCollection = cms.vstring("electron-track pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("ElectronTrackInvMass"),
            title = cms.string("Electron-Track Invariant Mass; M_{e+track} [GeV]"),
            binsX = cms.untracked.vdouble(90, 0, 180),
            inputVariables = cms.vstring("invMass"),
            ),
        cms.PSet (
            name = cms.string("ElectronTrackDeltaEta"),
            title = cms.string("Electron-Track Eta Difference; |#Delta(#eta)|"),
            binsX = cms.untracked.vdouble(100, 0, 10),
            inputVariables = cms.vstring("deltaEta"),
            ),
        cms.PSet (
            name = cms.string("ElectronTrackDeltaPhi"),
            title = cms.string("Electron-Track Phi Difference; |#Delta(#phi)|"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
            ),
        cms.PSet (
            name = cms.string("ElectronTrackDeltaR"),
            title = cms.string("Electron-Track #DeltaR; #DeltaR"),
            binsX = cms.untracked.vdouble(100, 0, 10),
            inputVariables = cms.vstring("deltaR"),
            ),
        cms.PSet (
            name = cms.string("ElectronTrackDeltaRZoom"),
            title = cms.string("Electron-Track #DeltaR; #DeltaR"),
            binsX = cms.untracked.vdouble(100, 0, 1),
            inputVariables = cms.vstring("deltaR"),
            ),
        )
    )

##############################################################################################

MuonTrackHistograms = cms.PSet(
    inputCollection = cms.vstring("muon-track pairs"),
    histograms = cms.VPSet (
    cms.PSet (
        name = cms.string("MuonTrackInvMass"),
        title = cms.string("Muon-Track Invariant Mass; M_{#mu+track} [GeV]"),
        binsX = cms.untracked.vdouble(90, 0, 180),
        inputVariables = cms.vstring("invMass"),
        ),
    cms.PSet (
        name = cms.string("MuonTrackPiMinusThreeDAngle"),
        title = cms.string("Pi - angle between the two muons;log(#alpha [rad])"),
        binsX = cms.untracked.vdouble(100, -9, 5),
        inputVariables = cms.vstring("log10(alpha)"),
        ),
    cms.PSet (
        name = cms.string("MuonTrackDeltaEta"),
        title = cms.string("Muon-Track Eta Difference; |#Delta(#eta)|"),
        binsX = cms.untracked.vdouble(100, 0, 10),
        inputVariables = cms.vstring("deltaEta"),
        ),
    cms.PSet (
        name = cms.string("MuonTrackDeltaPhi"),
        title = cms.string("Muon-Track Phi Difference; |#Delta(#phi)|"),
        binsX = cms.untracked.vdouble(100, 0, 3.15),
        inputVariables = cms.vstring("deltaPhi"),
        ),
    cms.PSet (
        name = cms.string("MuonTrackDeltaR"),
        title = cms.string("Muon-Track #DeltaR; #DeltaR"),
        binsX = cms.untracked.vdouble(100, 0, 10),
        inputVariables = cms.vstring("deltaR"),
        ),
    cms.PSet (
        name = cms.string("MuonTrackDeltaRZoom"),
        title = cms.string("Muon-Track #DeltaR; #DeltaR"),
        binsX = cms.untracked.vdouble(100, 0, 1),
        inputVariables = cms.vstring("deltaR"),
        ),
    cms.PSet (
        name = cms.string("MuonTrackDeltaPt"),
        title = cms.string("Muon-Track #DeltaPt; p_{T}^{#mu} - p_{T}^{trk} [GeV]"),
        binsX = cms.untracked.vdouble(100, -50, 50),
        inputVariables = cms.vstring("deltaPt"),
        ),
    )
)

##############################################################################################

TauTrackHistograms = cms.PSet(
    inputCollection = cms.vstring("tau-track pairs"),
    histograms = cms.VPSet (
    cms.PSet (
        name = cms.string("TauTrackInvMass"),
        title = cms.string("Tau-Track Invariant Mass; M_{#mu+track} [GeV]"),
        binsX = cms.untracked.vdouble(90, 0, 180),
        inputVariables = cms.vstring("invMass"),
        ),
    cms.PSet (
        name = cms.string("TauTrackDeltaEta"),
        title = cms.string("Tau-Track Eta Difference; |#Delta(#eta)|"),
        binsX = cms.untracked.vdouble(100, 0, 10),
        inputVariables = cms.vstring("deltaEta"),
        ),
    cms.PSet (
        name = cms.string("TauTrackDeltaPhi"),
        title = cms.string("Tau-Track Phi Difference; |#Delta(#phi)|"),
        binsX = cms.untracked.vdouble(100, 0, 3.15),
        inputVariables = cms.vstring("deltaPhi"),
        ),
    cms.PSet (
        name = cms.string("TauTrackDeltaR"),
        title = cms.string("Tau-Track #DeltaR; #DeltaR"),
        binsX = cms.untracked.vdouble(100, 0, 10),
        inputVariables = cms.vstring("deltaR"),
        ),
    cms.PSet (
        name = cms.string("TauTrackDeltaRZoom"),
        title = cms.string("Tau-Track #DeltaR; #DeltaR"),
        binsX = cms.untracked.vdouble(100, 0, 1),
        inputVariables = cms.vstring("deltaR"),
        ),
    )
)




##############################################################################################

TrackMCParticleHistograms = cms.PSet(
    inputCollection = cms.vstring("track-mcparticle pairs"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("TrackMCPartDeltaEta"),
            title = cms.string("Track-MCParticle Eta Difference; |#Delta(#eta)|"),
            binsX = cms.untracked.vdouble(100, 0, 10),
            inputVariables = cms.vstring("deltaEta"),
            ),
        cms.PSet (
            name = cms.string("TrackMCPartDeltaPhi"),
            title = cms.string("Track-MCParticle Phi Difference; |#Delta(#phi)|"),
            binsX = cms.untracked.vdouble(100, 0, 3.15),
            inputVariables = cms.vstring("deltaPhi"),
            ),
        cms.PSet (
            name = cms.string("TrackMCPartDeltaR"),
            title = cms.string("Track-MCParticle #DeltaR; #DeltaR"),
            binsX = cms.untracked.vdouble(100, 0, 10),
            inputVariables = cms.vstring("deltaR"),
            ),
        cms.PSet (
            name = cms.string("TrackMCPartDeltaRZoom"),
            title = cms.string("Track-MCParticle #DeltaR; #DeltaR"),
            binsX = cms.untracked.vdouble(100, 0, 1),
            inputVariables = cms.vstring("deltaR"),
            ),
        cms.PSet (
            name = cms.string("TrackMCPart3DAngle"),
            title = cms.string("Track-MCParticle 3D Angle; 3D angle"),
            binsX = cms.untracked.vdouble(100, 0, 4),
            inputVariables = cms.vstring("threeDAngle"),
            ),
        )
    )



##############################################################################################
# WARNING: NOT UPDATED FOR MINIAOD
EventHistograms = cms.PSet(
    inputCollection = cms.vstring("events"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("ht"),
            title = cms.string("H_{T}; #Sigma p_{T} selected jets [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("ht"),
        ),
        cms.PSet (
            name = cms.string("unfilteredHt"),
            title = cms.string("Unfiltered H_{T}; #Sigma p_{T} all jets [GeV]"),
            binsX = cms.untracked.vdouble(150, 0, 750),
            inputVariables = cms.vstring("unfilteredHt"),
        ),
        cms.PSet (
            name = cms.string("st"),
            title = cms.string("S_{T}; #Sigma p_{T} selected e/#mu/jets [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("st"),
        ),
        cms.PSet (
            name = cms.string("unfilteredSt"),
            title = cms.string("Unfiltered S_{T}; #Sigma p_{T} all e/#mu/jets [GeV]"),
            binsX = cms.untracked.vdouble(150, 0, 750),
            inputVariables = cms.vstring("unfilteredSt"),
        ),
        cms.PSet (
            name = cms.string("nExtraPartons"),
            title = cms.string("Number of Extra Partons; # Extra Partons"),
            binsX = cms.untracked.vdouble(6, 0, 6),
            inputVariables = cms.vstring("nExtraPartons"),
        ),
        cms.PSet (
            name = cms.string("puScaleFactor"),
            title = cms.string("Pileup Scale Factor;pileup weight"),
            binsX = cms.untracked.vdouble(100, 0, 5),
            inputVariables = cms.vstring("puScaleFactor"),
        ),
        cms.PSet (
            name = cms.string("topPtScaleFactor"),
            title = cms.string("Top Pt Reweighting Factor; top pt weight "),
            binsX = cms.untracked.vdouble(100, 0.5, 1.5),
            inputVariables = cms.vstring("topPtScaleFactor"),
        ),
        cms.PSet (
            name = cms.string("triggerScaleFactor"),
            title = cms.string("Trigger Efficiency Reweighting Factor; trigger SF weight "),
            binsX = cms.untracked.vdouble(100, 0.5, 1.5),
            inputVariables = cms.vstring("triggerScaleFactor"),
        ),
        cms.PSet (
            name = cms.string("muonScaleFactor"),
            title = cms.string("Muon Scale Factor;muon weight"),
            binsX = cms.untracked.vdouble(100, 0.5, 1.5),
            inputVariables = cms.vstring("muonScaleFactor"),
        ),
        cms.PSet (
            name = cms.string("muonTrackScaleFactor"),
            title = cms.string("Track Scale Factor for muons;muon weight"),
            binsX = cms.untracked.vdouble(100, 0.5, 1.5),
            inputVariables = cms.vstring("muonTrackScaleFactor"),
        ),
        cms.PSet (
            name = cms.string("electronScaleFactor"),
            title = cms.string("Electron Scale Factor;electron weight"),
            binsX = cms.untracked.vdouble(100, 0.5, 1.5),
            inputVariables = cms.vstring("electronScaleFactor"),
        ),
        cms.PSet (
            name = cms.string("electronTrackScaleFactor"),
            title = cms.string("Track Scale Factor for electrons;electron weight"),
            binsX = cms.untracked.vdouble(100, 0.5, 1.5),
            inputVariables = cms.vstring("electronTrackScaleFactor"),
        ),
        cms.PSet (
            name = cms.string("bTagScaleFactor"),
            title = cms.string("Btag Scale Factor;btag weight"),
            binsX = cms.untracked.vdouble(100, 0.5, 1.5),
            inputVariables = cms.vstring("bTagScaleFactor"),
        ),
        cms.PSet (
            name = cms.string("triggerMetScaleFactor"),
            title = cms.string("Met Trigger Efficiency Reweighting Factor; trigger efficiency weight "),
            binsX = cms.untracked.vdouble(100, 0.5, 1.5),
            inputVariables = cms.vstring("triggerMetScaleFactor"),
        ),
        cms.PSet (
            name = cms.string("trackNMissOutScaleFactor"),
            title = cms.string("trackNMissOut Reweighting Factor; N_{miss}^{out} SF weight "),
            binsX = cms.untracked.vdouble(100, 0.5, 1.5),
            inputVariables = cms.vstring("trackNMissOutScaleFactor"),
        ),
        cms.PSet (
            name = cms.string("isrVaryScaleFactor"),
            title = cms.string("isrVaryScaleFactor Reweighting Factor; ISR variation SF weight "),
            binsX = cms.untracked.vdouble(100, 0.0, 2.0),
            inputVariables = cms.vstring("isrVaryScaleFactor"),
        ),
        cms.PSet (
            name = cms.string("stopCTauScaleFactor"),
            title = cms.string("#LTc#tau#GT Scale Factor;#LTc#tau#GT weight"),
            binsX = cms.untracked.vdouble(100, 0, 10),
            inputVariables = cms.vstring("stopCTauScaleFactor"),
        ),
        cms.PSet (
            name = cms.string("globalScaleFactor"),
            title = cms.string("Global Scale Factor; event weight"),
            binsX = cms.untracked.vdouble(100, 0, 2),
            inputVariables = cms.vstring("globalScaleFactor"),
        ),
        cms.PSet (
            name = cms.string("channelScaleFactor"),
            title = cms.string("Channel Scale Factor; event weight"),
            binsX = cms.untracked.vdouble(100, 0, 2),
            inputVariables = cms.vstring("channelScaleFactor"),
        ),
        cms.PSet (
            name = cms.string("eventScaleFactor"),
            title = cms.string("Event Scale Factor; event weight"),
            binsX = cms.untracked.vdouble(100, 0, 2),
            inputVariables = cms.vstring("eventScaleFactor"),
        ),
    )
)

##############################################################################################
