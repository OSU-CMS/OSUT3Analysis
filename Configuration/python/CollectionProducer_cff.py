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
    pfCandidate      =  cms.InputTag  ('packedPFCandidates','',''),
    conversions      =  cms.InputTag  ("reducedEgamma",                  "reducedConversions",          ""),
    rho              =  cms.InputTag  ("fixedGridRhoFastjetAll",         "",                            ""),

    # the following tag is not used by the OSUElectronProducer, but is needed
    # so that the reco::GsfElectronCore collection is saved in skims, which is
    # needed because the pat::Electron collection references it
    gsfElectronCore  =  cms.InputTag  ("reducedEgamma",                  "reducedGedGsfElectronCores",  ""),
)
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
)

if 'CMSSW_8' in os.environ['CMSSW_VERSION']:
    collectionProducer.jets.jetResFromGlobalTag = cms.bool(True)

copyConfiguration (collectionProducer.jets, collectionProducer.genMatchables)

collectionProducer.bjets = copy.deepcopy (collectionProducer.jets)
collectionProducer.bjets._TypedParameterizable__type = "OSUBjetProducer"

#-------------------------------------------------------------------------------

collectionProducer.mcparticles = cms.EDProducer ("McparticleProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.mets = cms.EDProducer ("OSUMetProducer",
    pfCandidates  =  cms.InputTag  ('packedPFCandidates',  '',  ''),
    BadChargedCandidateFilter = cms.InputTag ("BadChargedCandidateFilter"),
    BadPFMuonFilter = cms.InputTag ("BadChargedCandidateFilter"),
)

#-------------------------------------------------------------------------------

collectionProducer.muons = cms.EDProducer ("OSUMuonProducer",
    pfCandidate =  cms.InputTag  ('packedPFCandidates','',''),
)
copyConfiguration (collectionProducer.muons, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.photons = cms.EDProducer ("OSUPhotonProducer",
    rho = cms.InputTag ('fixedGridRhoFastjetAll', '', ''),
)
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
            ),
        ),
        muons = cms.VPSet (
            cms.PSet (
                histFile = cms.FileInPath ("OSUT3Analysis/Configuration/data/muonFiducialMap_mc.root"),
                beforeVetoHistName = cms.string ("beforeVeto"), # must be eta on x-axis, phi on y-axis
                afterVetoHistName = cms.string ("afterVeto"), # must be eta on x-axis, phi on y-axis
                thresholdForVeto = cms.double (0.0), # in sigma
            ),
        )
    ),
    minDeltaRForFiducialTrack = cms.double (0.05),
    maskedEcalChannelStatusThreshold = cms.int32 (3),
    outputBadEcalChannels = cms.bool (False),

    EBRecHits          =  cms.InputTag  ("reducedEcalRecHitsEB"),
    EERecHits          =  cms.InputTag  ("reducedEcalRecHitsEE"),
    HBHERecHits        =  cms.InputTag  ("reducedHcalRecHits", "hbhereco"),

    pfCandidates = cms.InputTag  ('packedPFCandidates',  '',  ''),

    gsfTracks    =  cms.InputTag  ("electronGsfTracks",      ""),
    maxDeltaRForGsfTrackMatching = cms.double (0.2), # if cutting on dRToMatchedGsfTrack, must set this to be greater than the cut threshold

    # these are only relavent for the disappearing tracks analysis
    candidateTracks = cms.InputTag ("candidateTrackProducer", ""),
    maxDeltaRForCandidateTrackMatching = cms.double (0.2), # if cutting on dRToMatchedIsolatedTrack, must set this to be greater than the cut threshold

    # The following three parameters are used to correct the missing outer hits
    # distribution in MC for the disappearing tracks analysis.
    dropTOBProbability          =  cms.double  (0.00830971251971), # probability of dropping all hits in the TOB
    preTOBDropHitInefficiency   =  cms.double  (0.00515089150972), # probability of randomly dropping strip hits before the TOB
    postTOBDropHitInefficiency  =  cms.double  (0.27336245212),    # probability of randomly dropping strip hits in the TOB
    hitInefficiency             =  cms.double  (0.0175874821487),  # probability of randomly dropping strip hits in the TOB
)
if osusub.batchMode and types[osusub.datasetLabel] == "data":
    if "Run2016" in osusub.dataset:
        collectionProducer.tracks.fiducialMaps.electrons[0].histFile = cms.FileInPath ("OSUT3Analysis/Configuration/data/electronFiducialMap_2016_data.root")
        collectionProducer.tracks.fiducialMaps.muons[0].histFile = cms.FileInPath ("OSUT3Analysis/Configuration/data/muonFiducialMap_2016_data.root")
    else:
        collectionProducer.tracks.fiducialMaps.electrons[0].histFile = cms.FileInPath ("OSUT3Analysis/Configuration/data/electronFiducialMap_2015_data.root")
        collectionProducer.tracks.fiducialMaps.muons[0].histFile = cms.FileInPath ("OSUT3Analysis/Configuration/data/muonFiducialMap_2015_data.root")

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
