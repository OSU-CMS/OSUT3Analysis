import FWCore.ParameterSet.Config as cms
import copy

##########################################################################
##### Set up a sample event selection requiring an electron and muon #####
##########################################################################

eMuMinimal = cms.PSet(
    name = cms.string("EMuMinimal"),
    triggers = cms.vstring("HLT_Mu23NoFiltersNoVtx_Photon23_CaloIdL_v"), # TRIGGER
    cuts = cms.VPSet (
        # EVENT HAS GOOD PV
        cms.PSet (
            inputCollection = cms.vstring("primaryvertexs"),
            cutString = cms.string("isValid > 0 && ndof >= 4"),
            numberRequired = cms.string(">= 1")
        ),
        # ELECTRON ETA CUT
        cms.PSet (
            inputCollection = cms.vstring("electrons"),
            cutString = cms.string("abs(eta) < 2.5"),
            numberRequired = cms.string(">= 1")
        ),
        # ELECTRON PT CUT
        cms.PSet (
            inputCollection = cms.vstring("electrons"),
            cutString = cms.string("pt > 25"),
            numberRequired = cms.string(">= 1")
        ),
        # MUON ETA CUT
        cms.PSet (
            inputCollection = cms.vstring("muons"),
            cutString = cms.string("abs(eta) < 2.5"),
            numberRequired = cms.string(">= 1")
        ),
        # MUON PT CUT
        cms.PSet (
            inputCollection = cms.vstring("muons"),
            cutString = cms.string("pt > 25"),
            numberRequired = cms.string(">= 1")
        ),
    )
)

##########################################################################
######## Set up the preselection for the displaced SUSY analysis #########
##########################################################################

preselection = cms.PSet(
    name = cms.string("Preselection"),
    triggers = cms.vstring("HLT_Mu23NoFiltersNoVtx_Photon23_CaloIdL_v"), # TRIGGER
    cuts = cms.VPSet (
        # EVENT HAS GOOD PV
        cms.PSet (
            inputCollection = cms.vstring("primaryvertexs"),
            cutString = cms.string("isValid > 0 && ndof >= 4"),
            numberRequired = cms.string(">= 1")
        ),
        # ELECTRON ETA CUT
        cms.PSet (
            inputCollection = cms.vstring("electrons"),
            cutString = cms.string("abs(eta) < 2.5"),
            numberRequired = cms.string(">= 1")
        ),
        # ELECTRON CRACK VETO
        cms.PSet (
            inputCollection = cms.vstring("electrons"),
            cutString = cms.string("isEBEEGap = 0"),
            numberRequired = cms.string(">= 1"),
            alias = cms.string("electron ECAL crack veto")
        ),
        # ELECTRON PT CUT
        cms.PSet (
            inputCollection = cms.vstring("electrons"),
            cutString = cms.string("pt > 25"),
            numberRequired = cms.string(">= 1")
        ),
        # ELECTRON ID
        cms.PSet (
            inputCollection = cms.vstring("electrons"),
            cutString = cms.string("                              \
          (isEB & \
          abs(deltaEtaSuperClusterTrackAtVtx) < 0.00926 & \
          abs(deltaPhiSuperClusterTrackAtVtx) < 0.0336 & \
          full5x5_sigmaIetaIeta < 0.0101 & \
          hadronicOverEm < 0.0597 & \
          abs(1/ecalEnergy - eSuperClusterOverP/ecalEnergy) < 0.012 & \
          passConversionVeto)|\
          (isEE & \
          abs(deltaEtaSuperClusterTrackAtVtx) < 0.00724 & \
          abs(deltaPhiSuperClusterTrackAtVtx) < 0.0918 & \
          full5x5_sigmaIetaIeta < 0.0279 & \
          hadronicOverEm < 0.0615 & \
          abs(1/ecalEnergy - eSuperClusterOverP/ecalEnergy) < 0.00999 & \
          passConversionVeto)"),
            numberRequired = cms.string(">= 1"),
            alias = cms.string("electron tight displaced ID")
            ),
        # ELECTRON ISOLATION
        cms.PSet (
            inputCollection = cms.vstring("electrons"),
            cutString = cms.string("        \
        ((pfIso_.sumChargedHadronPt   \
        + max(0.0,                    \
        pfIso_.sumNeutralHadronEt     \
        + pfIso_.sumPhotonEt          \
        - rho*AEff))                  \
        /pt <= 0.0646 && isEE)  |        \
        ((pfIso_.sumChargedHadronPt   \
        + max(0.0,                    \
        pfIso_.sumNeutralHadronEt     \
        + pfIso_.sumPhotonEt          \
        - rho_*AEff_))                \
        /pt <= 0.0354 && isEB)"),
            numberRequired = cms.string(">= 1"),
            alias = cms.string("electron isolation")
            ),
        # MUON ETA CUT
        cms.PSet (
            inputCollection = cms.vstring("muons"),
            cutString = cms.string("abs(eta) < 2.5"),
            numberRequired = cms.string(">= 1")
        ),
        # MUON PT CUT
        cms.PSet (
            inputCollection = cms.vstring("muons"),
            cutString = cms.string("pt > 25"),
            numberRequired = cms.string(">= 1")
        ),
        # MUON ID
        cms.PSet (
            inputCollection = cms.vstring("muons"),
            # moving these selections to a separate cut
            # to avoid non-global muons from passing the ID
            #        isGlobalMuon & \
                #        isPFMuon & \
                cutString = cms.string("\
        globalTrack.hitPattern_.numberOfValidMuonHits > 0 & \
        globalTrack.normalizedChi2 < 10 & \
        numberOfMatchedStations > 1 & \
        innerTrack.hitPattern_.numberOfValidPixelHits > 0 & \
        innerTrack.hitPattern_.trackerLayersWithMeasurement > 5"),
            numberRequired = cms.string(">= 1"),
            alias = cms.string("muon tight displaced ID")
            ),
        # MUON ISOLATION
        cms.PSet (
            inputCollection = cms.vstring("muons"),
            cutString = cms.string("                \
        (pfIsolationR04_.sumChargedHadronPt \
        + max(0.0,                          \
        pfIsolationR04_.sumNeutralHadronEt  \
        + pfIsolationR04_.sumPhotonEt       \
        - 0.5*pfIsolationR04_.sumPUPt))     \
        /pt <= 0.15                         \
       "),
            numberRequired = cms.string(">= 1"),
            alias = cms.string("muon isolation")
            ),
        # VETO EVENTS WITH EXTRA ELECTRON
        cms.PSet (
            inputCollection = cms.vstring("electrons"),
            cutString = cms.string("pt > -1"),
            numberRequired = cms.string("== 1"),
            alias = cms.string("extra electron veto")
        ),
        # VETO EVENTS WITH EXTRA MUON
        cms.PSet (
            inputCollection = cms.vstring("muons"),
            cutString = cms.string("pt > -1"),
            numberRequired = cms.string("== 1"),
            alias = cms.string("extra muon veto")
        ),
        # OPPOSITE SIGN E-MU PAIR
        cms.PSet (
            inputCollection = cms.vstring("electrons", "muons"),
            cutString = cms.string("electron.charge * muon.charge < 0"),
            numberRequired = cms.string("== 1")
        ),
        # ELECTRON AND MUON ARE NOT OVERLAPPING
        cms.PSet (
            inputCollection = cms.vstring("electrons", "muons"),
            cutString = cms.string("deltaR(electron, muon) > 0.5"),
            numberRequired = cms.string("== 1")
        ),
        #########START OF ADDITIONAL CUTS TO REQUIRE LEPTON IS NOT IN A JET
        # ONLY CONSIDER 10 GEV JETS
        cms.PSet (
            inputCollection = cms.vstring("jets"),
            cutString = cms.string("pt > 10"),
            numberRequired = cms.string(">= 0")
        ),
        ########### END OF ADDITIONAL CUTS TO REQUIRE LEPTON IS NOT IN A JET
        )
    )

##########################################################################




