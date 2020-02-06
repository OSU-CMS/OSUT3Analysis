import FWCore.ParameterSet.Config as cms
import OSUT3Analysis.DBTools.osusub_cfg as osusub
from OSUT3Analysis.Configuration.configurationOptions import *
import copy
import os

def copyConfiguration (dst, src):
    for parameter in src:
        setattr (dst, parameter, src[parameter])

class CollectionProducer:
  pass

collectionProducer = CollectionProducer ()

################################################################################
# Configuration for base classes
################################################################################
collectionProducer.genMatchables = {
    "maxDeltaRForGenMatching":  cms.double  (0.1),
    "minPtForGenMatching":      cms.double  (10.0),
}
################################################################################

################################################################################
# Configuration for derived classes
################################################################################

#-------------------------------------------------------------------------------

collectionProducer.beamspots = cms.EDProducer ("OSUBeamspotProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.bxlumis = cms.EDProducer ("OSUBxlumiProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.cschits = cms.EDProducer ("OSUCschitProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.cscsegs = cms.EDProducer ("OSUCscsegProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.dtsegs = cms.EDProducer ("OSUDtsegProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.electrons = cms.EDProducer ("OSUElectronProducer",
    pfCandidate      =  cms.InputTag   ('packedPFCandidates','',''),
    conversions      =  cms.InputTag   ("reducedEgamma",                  "reducedConversions",          ""),
    rho              =  cms.InputTag   ("fixedGridRhoFastjetAll",         "",                            ""),
    vidVetoIdMap     =  cms.InputTag   ("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-veto"),
    vidLooseIdMap    =  cms.InputTag   ("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-loose"),
    vidMediumIdMap   =  cms.InputTag   ("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-medium"),
    vidTightIdMap    =  cms.InputTag   ("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-tight"),
    effAreasPayload  =  cms.FileInPath ("RecoEgamma/ElectronIdentification/data/Spring15/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_25ns.txt"),

    # the following tag is not used by the OSUElectronProducer, but is needed
    # so that the reco::GsfElectronCore collection is saved in skims, which is
    # needed because the pat::Electron collection references it
    gsfElectronCore  =  cms.InputTag  ("reducedEgamma", "reducedGedGsfElectronCores",  ""),
    gsfTrack         =  cms.InputTag  ("reducedEgamma", "reducedGsfTracks",            ""),

    # similarly as above these tags are required in skims as they are referenced
    # in using the superCluster's seed for the dEtaInSeed cut
    EBEEClusters    = cms.InputTag ("reducedEgamma", "reducedEBEEClusters",    ""),
    ESClusters      = cms.InputTag ("reducedEgamma", "reducedESClusters",      ""),
    ootEBEEClusters = cms.InputTag ("reducedEgamma", "reducedOOTEBEEClusters", ""),
    ootESClusters   = cms.InputTag ("reducedEgamma", "reducedOOTESClusters",   ""),

    d0SmearingWidth = cms.double (-1.0),

)

if os.environ["CMSSW_VERSION"].startswith ("CMSSW_8_0"):
    collectionProducer.electrons.vidVetoIdMap    = cms.InputTag   ("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-veto")
    collectionProducer.electrons.vidLooseIdMap   = cms.InputTag   ("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-loose")
    collectionProducer.electrons.vidMediumIdMap  = cms.InputTag   ("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-medium")
    collectionProducer.electrons.vidTightIdMap   = cms.InputTag   ("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-tight")
    collectionProducer.electrons.effAreasPayload = cms.FileInPath ("RecoEgamma/ElectronIdentification/data/Summer16/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_80X.txt")

if os.environ["CMSSW_VERSION"].startswith ("CMSSW_9_4"):
    collectionProducer.electrons.vidVetoIdMap    = cms.InputTag   ("egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V1-veto")
    collectionProducer.electrons.vidLooseIdMap   = cms.InputTag   ("egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V1-loose")
    collectionProducer.electrons.vidMediumIdMap  = cms.InputTag   ("egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V1-medium")
    collectionProducer.electrons.vidTightIdMap   = cms.InputTag   ("egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V1-tight")
    collectionProducer.electrons.effAreasPayload = cms.FileInPath ("RecoEgamma/ElectronIdentification/data/Fall17/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_92X.txt")
    if (False and int(os.environ["CMSSW_VERSION"].split("_")[3]) >=9):
        collectionProducer.electrons.vidVetoIdMap    = cms.InputTag   ("egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V2-veto")
        collectionProducer.electrons.vidLooseIdMap   = cms.InputTag   ("egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V2-loose")
        collectionProducer.electrons.vidMediumIdMap  = cms.InputTag   ("egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V2-medium")
        collectionProducer.electrons.vidTightIdMap   = cms.InputTag   ("egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V2-tight")
        collectionProducer.electrons.effAreasPayload = cms.FileInPath ("RecoEgamma/ElectronIdentification/data/Fall17/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_94X.txt")

if os.environ["CMSSW_VERSION"].startswith ("CMSSW_10_2"):
    collectionProducer.electrons.vidVetoIdMap    = cms.InputTag   ("egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V2-veto")
    collectionProducer.electrons.vidLooseIdMap   = cms.InputTag   ("egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V2-loose")
    collectionProducer.electrons.vidMediumIdMap  = cms.InputTag   ("egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V2-medium")
    collectionProducer.electrons.vidTightIdMap   = cms.InputTag   ("egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V2-tight")
    collectionProducer.electrons.effAreasPayload = cms.FileInPath ("RecoEgamma/ElectronIdentification/data/Fall17/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_94X.txt")

copyConfiguration (collectionProducer.electrons, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.events = cms.EDProducer ("OSUEventProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.eventvariables = cms.EDProducer ("EventvariableProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.genjets = cms.EDProducer ("OSUGenjetProducer",
)
copyConfiguration (collectionProducer.genjets, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.jets = cms.EDProducer ("OSUJetProducer",
    rho = cms.InputTag("fixedGridRhoFastjetAll", "", ""),
    jetResolutionPayload = cms.string(os.environ['CMSSW_BASE'] + "/src/OSUT3Analysis/Collections/data/Fall15_25nsV2_MC_PtResolution_AK4PFchs.txt"),
    jetResSFPayload = cms.string(os.environ['CMSSW_BASE'] + "/src/OSUT3Analysis/Collections/data/Fall15_25nsV2_MC_SF_AK4PFchs.txt"),
    jetResFromGlobalTag = cms.bool(False),
    jetResNewPrescription = cms.bool(False),
)

if os.environ["CMSSW_VERSION"].startswith ("CMSSW_8_0_") or os.environ["CMSSW_VERSION"].startswith ("CMSSW_9_4_") or os.environ["CMSSW_VERSION"].startswith ("CMSSW_10_2_"):
    collectionProducer.jets.jetResFromGlobalTag = cms.bool(True)
    collectionProducer.jets.jetResNewPrescription = cms.bool(True)

copyConfiguration (collectionProducer.jets, collectionProducer.genMatchables)

collectionProducer.bjets = copy.deepcopy (collectionProducer.jets)
collectionProducer.bjets._TypedParameterizable__type = "OSUBjetProducer"

#-------------------------------------------------------------------------------

collectionProducer.mcparticles = cms.EDProducer ("McparticleProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.hardInteractionMcparticles = cms.EDProducer ("HardInteractionMcparticleProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.mets = cms.EDProducer ("OSUMetProducer",
    pfCandidates  =  cms.InputTag  ('packedPFCandidates',  '',  ''),
    BadChargedCandidateFilter = cms.InputTag ("BadChargedCandidateFilter"),
    BadPFMuonFilter = cms.InputTag ("BadChargedCandidateFilter"),
    ecalBadCalibReducedMINIAODFilter = cms.InputTag ("ecalBadCalibReducedMINIAODFilter"),
)

#-------------------------------------------------------------------------------

collectionProducer.muons = cms.EDProducer ("OSUMuonProducer",
    pfCandidate =  cms.InputTag  ('packedPFCandidates','',''),
    d0SmearingWidth = cms.double (-1.0),
    hltMatchingInfo = cms.VPSet (
        cms.PSet(name = cms.string("HLT_IsoMu20_v"),   collection = cms.string("hltL3MuonCandidates::HLT"),     filter = cms.string("hltL3crIsoL1sMu16L1f0L2f10QL3f20QL3trkIsoFiltered0p09")),
        cms.PSet(name = cms.string("HLT_IsoTkMu20_v"), collection = cms.string("hltHighPtTkMuonCands::HLT"),    filter = cms.string("hltL3fL1sMu16L1f0Tkf20QL3trkIsoFiltered0p09")),
        cms.PSet(name = cms.string("HLT_IsoMu24_v"),   collection = cms.string("hltL3MuonCandidates::HLT"),     filter = cms.string("hltL3crIsoL1sMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p09")),
        cms.PSet(name = cms.string("HLT_IsoTkMu24_v"), collection = cms.string("hltHighPtTkMuonCands::HLT"),    filter = cms.string("hltL3fL1sMu22L1f0Tkf24QL3trkIsoFiltered0p09")),
        cms.PSet(name = cms.string("HLT_IsoMu27_v"),   collection = cms.string("hltIterL3MuonCandidates::HLT"), filter = cms.string("hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07")),
    ),
)
if os.environ["CMSSW_VERSION"].startswith ("CMSSW_10_2_"):
    for x in collectionProducer.muons.hltMatchingInfo:
        if x.name.value() == "HLT_IsoMu20_v":
            x.collection = cms.string("hltIterL3MuonCandidates::HLT")
            x.filter = cms.string("hltL3crIsoL1sMu18L1f0L2f10QL3f20QL3trkIsoFiltered0p07")
        if x.name.value() == "HLT_IsoMu24_v":
            x.collection = cms.string("hltIterL3MuonCandidates::HLT")
            x.filter = cms.string("hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07")
copyConfiguration (collectionProducer.muons, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.photons = cms.EDProducer ("OSUPhotonProducer",
    rho = cms.InputTag ('fixedGridRhoFastjetAll', '', ''),
    vidLooseIdMap  = cms.InputTag ("egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-loose"),
    vidMediumIdMap = cms.InputTag ("egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-medium"),
    vidTightIdMap  = cms.InputTag ("egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-tight"),
    effAreasChargedHadronPayload = cms.FileInPath ("RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfChargedHadrons_90percentBased.txt"),
    effAreasNeutralHadronPayload = cms.FileInPath ("RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfNeutralHadrons_90percentBased.txt"),
    effAreasPhotonPayload        = cms.FileInPath ("RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfPhotons_90percentBased.txt"),
)

if os.environ["CMSSW_VERSION"].startswith ("CMSSW_9_4") or os.environ["CMSSW_VERSION"].startswith("CMSSW_10_2"):
    collectionProducer.photons.vidLooseIdMap  = cms.InputTag ("egmPhotonIDs:cutBasedPhotonID-Fall17-94X-V2-loose")
    collectionProducer.photons.vidMediumIdMap = cms.InputTag ("egmPhotonIDs:cutBasedPhotonID-Fall17-94X-V2-medium")
    collectionProducer.photons.vidTightIdMap  = cms.InputTag ("egmPhotonIDs:cutBasedPhotonID-Fall17-94X-V2-tight")
    collectionProducer.photons.effAreasChargedHadronPayload = cms.FileInPath ("RecoEgamma/PhotonIdentification/data/Fall17/effAreaPhotons_cone03_pfChargedHadrons_90percentBased_V2.txt")
    collectionProducer.photons.effAreasNeutralHadronPayload = cms.FileInPath ("RecoEgamma/PhotonIdentification/data/Fall17/effAreaPhotons_cone03_pfNeutralHadrons_90percentBased_V2.txt")
    collectionProducer.photons.effAreasPhotonPayload        = cms.FileInPath ("RecoEgamma/PhotonIdentification/data/Fall17/effAreaPhotons_cone03_pfPhotons_90percentBased_V2.txt")

copyConfiguration (collectionProducer.photons, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.pileUpInfos = cms.EDProducer ("PileUpInfoProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.primaryvertexs = cms.EDProducer ("OSUPrimaryvertexProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.rpchits = cms.EDProducer ("OSURpchitProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.superclusters = cms.EDProducer ("OSUSuperclusterProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.taus = cms.EDProducer ("OSUTauProducer",
)
copyConfiguration (collectionProducer.taus, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.tracks = cms.EDProducer ("OSUTrackProducer",
    fiducialMaps = cms.PSet (
        electrons = cms.VPSet (
            cms.PSet (
                histFile = cms.FileInPath ("OSUT3Analysis/Configuration/data/electronFiducialMap_mc.root"),
                beforeVetoHistName = cms.string ("beforeVeto"), # must be eta on x-axis, phi on y-axis
                afterVetoHistName = cms.string ("afterVeto"), # must be eta on x-axis, phi on y-axis
                thresholdForVeto = cms.double (0.0), # in sigma
                era = cms.string (''),
            ),
        ),
        muons = cms.VPSet (
            cms.PSet (
                histFile = cms.FileInPath ("OSUT3Analysis/Configuration/data/muonFiducialMap_mc.root"),
                beforeVetoHistName = cms.string ("beforeVeto"), # must be eta on x-axis, phi on y-axis
                afterVetoHistName = cms.string ("afterVeto"), # must be eta on x-axis, phi on y-axis
                thresholdForVeto = cms.double (0.0), # in sigma
                era = cms.string (''),
            ),
        )
    ),
    useEraByEraFiducialMaps = cms.bool (False),
    minDeltaRForFiducialTrack = cms.double (0.05),
    maskedEcalChannelStatusThreshold = cms.int32 (3),
    outputBadEcalChannels = cms.bool (False),

    EBRecHits          =  cms.InputTag  ("reducedEcalRecHitsEB"),
    EERecHits          =  cms.InputTag  ("reducedEcalRecHitsEE"),
    HBHERecHits        =  cms.InputTag  ("reducedHcalRecHits", "hbhereco"),

    pfCandidates   = cms.InputTag ('packedPFCandidates',  '',  ''),
    lostTracks     = cms.InputTag ('lostTracks', '', ''),
    isolatedTracks = cms.InputTag ('isolatedTracks', '', ''),

    gsfTracks    =  cms.InputTag  ("electronGsfTracks",      ""),
    maxDeltaRForGsfTrackMatching = cms.double (0.2), # if cutting on dRToMatchedGsfTrack, must set this to be greater than the cut threshold

    # these are only relavent for the disappearing tracks analysis
    candidateTracks = cms.InputTag ("candidateTrackProducer", ""),
    maxDeltaRForCandidateTrackMatching = cms.double (0.2), # if cutting on dRToMatchedCandidateTrack, must set this to be greater than the cut threshold
    maxDeltaRForIsolatedTrackMatching = cms.double (0.2), # if cutting on dRToMatchedIsolatedTrack, must set this to be greater than the cut threshold

    # The following three parameters are used to correct the missing outer hits
    # distribution in MC for the disappearing tracks analysis.
    dropTOBProbability          =  cms.double  (0.00830971251971), # probability of dropping all hits in the TOB
    preTOBDropHitInefficiency   =  cms.double  (0.00515089150972), # probability of randomly dropping strip hits before the TOB
    postTOBDropHitInefficiency  =  cms.double  (0.27336245212),    # probability of randomly dropping strip hits in the TOB
    hitInefficiency             =  cms.double  (0.0175874821487),  # probability of randomly dropping strip hits in the TOB

    originalElectrons = cms.InputTag ("slimmedElectrons"),
    originalMuons     = cms.InputTag ("slimmedMuons"),
    originalTaus      = cms.InputTag ("slimmedTaus"),

    eleVIDVetoIdMap   = collectionProducer.electrons.vidVetoIdMap,
    eleVIDLooseIdMap  = collectionProducer.electrons.vidLooseIdMap,
    eleVIDMediumIdMap = collectionProducer.electrons.vidMediumIdMap,
    eleVIDTightIdMap  = collectionProducer.electrons.vidTightIdMap,

    # Cut values are ordered by ID, as: veto, loose, medium, tight
    # https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2Archive#Spring15_selection_25ns
    eleVtx_d0Cuts_barrel = cms.vdouble(0.0564, 0.0261, 0.0118, 0.0111),
    eleVtx_dzCuts_barrel = cms.vdouble(0.472, 0.41, 0.373, 0.0466),
    eleVtx_d0Cuts_endcap = cms.vdouble(0.222, 0.118, 0.0739, 0.0351),
    eleVtx_dzCuts_endcap = cms.vdouble(0.921, 0.822, 0.602, 0.417),
)

# if running over data, switch to the data fiducial map files
if osusub.batchMode and types[osusub.datasetLabel] == "data":
    if "Run2016" in osusub.dataset:
        collectionProducer.tracks.fiducialMaps.electrons[0].histFile = cms.FileInPath ("OSUT3Analysis/Configuration/data/electronFiducialMap_2016_data.root")
        collectionProducer.tracks.fiducialMaps.muons[0].histFile = cms.FileInPath ("OSUT3Analysis/Configuration/data/muonFiducialMap_2016_data.root")
    elif "Run2017" in osusub.dataset:
        collectionProducer.tracks.fiducialMaps.electrons[0].histFile = cms.FileInPath ("OSUT3Analysis/Configuration/data/electronFiducialMap_2017_data.root")
        collectionProducer.tracks.fiducialMaps.muons[0].histFile = cms.FileInPath ("OSUT3Analysis/Configuration/data/muonFiducialMap_2017_data.root")
    elif "Run2018" in osusub.dataset:
        collectionProducer.tracks.fiducialMaps.electrons[0].histFile = cms.FileInPath ("OSUT3Analysis/Configuration/data/electronFiducialMap_2018_data.root")
        collectionProducer.tracks.fiducialMaps.muons[0].histFile = cms.FileInPath ("OSUT3Analysis/Configuration/data/muonFiducialMap_2018_data.root")
    else:
        collectionProducer.tracks.fiducialMaps.electrons[0].histFile = cms.FileInPath ("OSUT3Analysis/Configuration/data/electronFiducialMap_2015_data.root")
        collectionProducer.tracks.fiducialMaps.muons[0].histFile = cms.FileInPath ("OSUT3Analysis/Configuration/data/muonFiducialMap_2015_data.root")
    # determine which era this dataset is in
    if "_201" in osusub.datasetLabel:
        collectionProducer.tracks.fiducialMaps.electrons[0].era = cms.string (osusub.datasetLabel[osusub.datasetLabel.find('_201'):])
        collectionProducer.tracks.fiducialMaps.muons[0].era = cms.string (osusub.datasetLabel[osusub.datasetLabel.find('_201'):])

# For 94X/102X which use electron VIDs, define the vertexing requirements for veto electrons
if os.environ["CMSSW_VERSION"].startswith ("CMSSW_9_4_") or os.environ["CMSSW_VERSION"].startswith ("CMSSW_10_2_"):
    # Cut values are ordered by ID, as: veto, loose, medium, tight
    # https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#Working_points_for_92X_and_later
    collectionProducer.tracks.eleVtx_d0Cuts_barrel = cms.vdouble(0.05, 0.05, 0.05, 0.05)
    collectionProducer.tracks.eleVtx_dzCuts_barrel = cms.vdouble(0.10, 0.10, 0.10, 0.10)
    collectionProducer.tracks.eleVtx_d0Cuts_endcap = cms.vdouble(0.10, 0.10, 0.10, 0.10)
    collectionProducer.tracks.eleVtx_dzCuts_endcap = cms.vdouble(0.20, 0.20, 0.20, 0.20)

copyConfiguration (collectionProducer.tracks, collectionProducer.genMatchables)
#-------------------------------------------------------------------------------

collectionProducer.secondaryTracks = copy.deepcopy (collectionProducer.tracks)
collectionProducer.secondaryTracks._TypedParameterizable__type = "OSUSecondaryTrackProducer"

#-------------------------------------------------------------------------------

collectionProducer.trigobjs = cms.EDProducer ("OSUTrigobjProducer",
)
copyConfiguration (collectionProducer.trigobjs, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.uservariables = cms.EDProducer ("UservariableProducer",
)

################################################################################
