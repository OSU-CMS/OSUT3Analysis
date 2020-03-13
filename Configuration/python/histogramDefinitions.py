import FWCore.ParameterSet.Config as cms
import os
from OSUT3Analysis.Configuration.pdgIdBins import *
from OSUT3Analysis.Configuration.cutUtilities import *

###############################################
######  Example histogram configurations ######
######  see comments for explanations    ######
###############################################

ExampleMuonHistograms = cms.PSet(

    # what collection to plot; see AnaTools/python/osuAnalysis_cfi.py for options
    inputCollection = cms.vstring("muons"),
    
    # all histograms to create and fill
    histograms = cms.VPSet (

        # any function available can be plotted, such as muon.pt()
        cms.PSet (
            name = cms.string("muonPt"),
            title = cms.string("Muon Transverse Momentum;Muon p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            inputVariables = cms.vstring("pt"),
        ),

        # "number" gives the vector's size after cuts have been applied,
        # ie muons.size(). for a full list of operators see
        # AnaTools/src/ValueLookupTree.cc
        cms.PSet (
            name = cms.string("numMuons"),
            title = cms.string("Number of Selected Muons;# muons"),
            binsX = cms.untracked.vdouble(10, 0, 10),
            inputVariables = cms.vstring("number(muon)"),
        ),

        # the "index" variable can be supplied to make a cut on the
        # pt-ordered index of the object. here indexX = 1 meaning
        # the only second-highest pt muon is used to fill this plot.
        # if there's only one muon in the event, this will not fill the
        # histogram
        cms.PSet (
            name = cms.string("muonSubleadingPt"),
            title = cms.string("Muon Transverse Momentum;Muon p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(500, 0, 500),
            indexX = cms.untracked.int32(1),
            inputVariables = cms.vstring("pt"),
        ),

        # two-dimensional plots can be defined as:
        cms.PSet (
            name = cms.string("muonEtaPhi"),
            title = cms.string("Muon Eta vs. Muon Phi;Muon #phi;Muon #eta"),
            binsX = cms.untracked.vdouble(64, -3.2, 3.2),
            binsY = cms.untracked.vdouble(60, -3, 3),
            inputVariables = cms.vstring("phi","eta"),
        ),
        # three-dimensional plots would provide a third variable and binsZ

        # non-uniform binnings can be provided as lists, such as from getPdgBins
        cms.PSet (
            name = cms.string("muonGenMatchPdgId"),
            title = cms.string(";|PDG ID| of generator particle matched to muon"),
            binsX = cms.untracked.vdouble(getPdgBins(["unmatched", "quarks", "leptons", "bosons"])),
            inputVariables = cms.vstring("abs (genMatchedParticle.noFlagsPdgId)"),
        ),

        # boolean variables are expressed as False=0 and True=1
        cms.PSet (
            name = cms.string("muonIsTightMuonWRTVtx"),
            title = cms.string("Muon isTightMuon;Muon passes tight ID"),
            binsX = cms.untracked.vdouble(2, 0, 2),
            inputVariables = cms.vstring("isTightMuonWRTVtx"),
        ),

    )
)

##############################################################################################

# if the variable plotted involves two different types of objects, supply both.
# keep in mind, all possible combinations of these objects will be considered;
# if there are 2 electrons and 3 muons left after cuts, these histograms will
# be filled 2*3=6 times for the event

ExampleElectronMuonHistograms = cms.PSet(

    # loop over both electron and muon collections
    inputCollection = cms.vstring("electrons","muons"),
    
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("electronPtMuonPt"),
            title = cms.string("Electron Momentum vs Muon Momentum;Muon p_{T} [GeV];Electron p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(10, 0, 500),
            binsY = cms.untracked.vdouble(10, 0, 500),
            inputVariables = cms.vstring("muon.pt","electron.pt"),
        ),

        # more complicated expressions can be defined
        cms.PSet (
            name = cms.string("electronRhoIsoVsMuonDbetaIso"),
            title = cms.string("Electron #rho-corrected Isolation vs. Muon #Delta#beta-corrected Isolation;Muon rel. iso.;Electron rel. iso."),
            binsX = cms.untracked.vdouble(300, 0, 3),
            binsY = cms.untracked.vdouble(300, 0, 3),
            inputVariables = cms.vstring("(muon.pfIsolationR04_.sumChargedHadronPt + max(0.0,muon.pfIsolationR04_.sumNeutralHadronEt + muon.pfIsolationR04_.sumPhotonEt - 0.5*muon.pfIsolationR04_.sumPUPt))/muon.pt","(electron.pfIso_.sumChargedHadronPt + max(0.0,electron.pfIso_.sumNeutralHadronEt + electron.pfIso_.sumPhotonEt - electron.rho*electron.AEff))/electron.pt"),
        ),
    )
)

##############################################################################################

# if the variable plotted is calculated from pairs of objects with the same type,
# proceed as above with two collections, just the same listed twice.
# all possible combinations/pairs will be considered, except the same object
# will never be used with itself -- this is checked for! 

ExampleDiMuonHistograms = cms.PSet(

    # loop over all muon-muon pairs, but don't use the same muon twice in the same pair
    # if only one muon is in the event, there's no pair of two so nothing is filled
    inputCollection = cms.vstring("muons", "muons"),
    
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("diMuonPt"),
            title = cms.string("Di-muon Tranverse Momentum;p_{T} [GeV]"),
            binsX = cms.untracked.vdouble(100, 0, 500),
            # each instance is a separate muon
            inputVariables = cms.vstring("pT (muon, muon)"),
        ),
        
    )
)

##############################################################################################

# an arbitrary number of input collections is possible to use, and all objects in those
# collections will be looped over. in this example "beamspots" is used and there is only
# one entry in that collection, so the combinatorics are limited.

ExampleElectronMuonIPHistograms = cms.PSet(
    inputCollection = cms.vstring("electrons","muons","beamspots"),
    histograms = cms.VPSet (
        cms.PSet (
            name = cms.string("electronD0vsMuonD0Beamspot"),
            title = cms.string("Electron d_{0} wrt Beamspot vs. Muon d_{0} wrt Beamspot;Muon d_{0} [cm];Electron d_{0} [cm]"),
            binsX = cms.untracked.vdouble(10, -0.02, 0.02),
            binsY = cms.untracked.vdouble(10, -0.02, 0.02),
            inputVariables = cms.vstring("(-(muon.vx - beamspot.x0)*muon.py + (muon.vy - beamspot.y0)*muon.px)/muon.pt","(-(electron.vx - beamspot.x0)*electron.py + (electron.vy - beamspot.y0)*electron.px)/electron.pt"),
        ),
    )
)
