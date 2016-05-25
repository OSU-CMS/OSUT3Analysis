import FWCore.ParameterSet.Config as cms
import OSUT3Analysis.DBTools.osusub_cfg as osusub
from OSUT3Analysis.Configuration.configurationOptions import *

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
    "maxDeltaRForGenMatching":  cms.double (0.1),
}
################################################################################

################################################################################
# Configuration for derived classes
################################################################################

collectionProducer.basicjets = cms.EDProducer ("OSUBasicjetProducer",
)
copyConfiguration (collectionProducer.basicjets, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.beamspots = cms.EDProducer ("OSUBeamspotProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.bjets = cms.EDProducer ("OSUBjetProducer",
)
copyConfiguration (collectionProducer.bjets, collectionProducer.genMatchables)

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
    conversions      =  cms.InputTag  ("reducedEgamma",                  "reducedConversions",          ""),
    rho              =  cms.InputTag  ("fixedGridRhoFastjetAll",         "",                            ""),
    beamSpot         =  cms.InputTag  ("offlineBeamSpot",                "",                            ""),
    vertices         =  cms.InputTag  ('offlineSlimmedPrimaryVertices',  ''),
    pfCandidate      =  cms.InputTag  ('packedPFCandidates','',''),

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
)
copyConfiguration (collectionProducer.jets, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.mcparticles = cms.EDProducer ("McparticleProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.mets = cms.EDProducer ("OSUMetProducer",
    pfCandidates  =  cms.InputTag  ('packedPFCandidates',  '',  ''),
)

#-------------------------------------------------------------------------------

collectionProducer.muons = cms.EDProducer ("OSUMuonProducer",
    pfCandidate =  cms.InputTag  ('packedPFCandidates','',''),
)
copyConfiguration (collectionProducer.muons, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.photons = cms.EDProducer ("OSUPhotonProducer",
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
                histFile = cms.FileInPath ("OSUT3Analysis/Configuration/data/electronFiducialMap_data.root"),
                beforeVetoHistName = cms.string ("beforeVeto"), # must be eta on x-axis, phi on y-axis
                afterVetoHistName = cms.string ("afterVeto"), # must be eta on x-axis, phi on y-axis
                thresholdForVeto = cms.double (2.0), # in sigma
            ),
            cms.PSet (
                histFile = cms.FileInPath ("OSUT3Analysis/Configuration/data/electronFiducialMap_mc.root"),
                beforeVetoHistName = cms.string ("beforeVeto"), # must be eta on x-axis, phi on y-axis
                afterVetoHistName = cms.string ("afterVeto"), # must be eta on x-axis, phi on y-axis
                thresholdForVeto = cms.double (2.0), # in sigma
            ),
        ),
        muons = cms.VPSet (
            cms.PSet (
                histFile = cms.FileInPath ("OSUT3Analysis/Configuration/data/muonFiducialMap_mc.root"),
                beforeVetoHistName = cms.string ("beforeVeto"), # must be eta on x-axis, phi on y-axis
                afterVetoHistName = cms.string ("afterVeto"), # must be eta on x-axis, phi on y-axis
                thresholdForVeto = cms.double (2.0), # in sigma
            ),
            cms.PSet (
                histFile = cms.FileInPath ("OSUT3Analysis/Configuration/data/muonFiducialMap_data.root"),
                beforeVetoHistName = cms.string ("beforeVeto"), # must be eta on x-axis, phi on y-axis
                afterVetoHistName = cms.string ("afterVeto"), # must be eta on x-axis, phi on y-axis
                thresholdForVeto = cms.double (2.0), # in sigma
            ),
        )
    ),
    minDeltaRForFiducialTrack = cms.double (0.05),

    EBRecHits          =  cms.InputTag  ("reducedEcalRecHitsEB"),
    EERecHits          =  cms.InputTag  ("reducedEcalRecHitsEE"),
    HBHERecHits        =  cms.InputTag  ("reducedHcalRecHits", "hbhereco"),

    gsfTracks    =  cms.InputTag  ("electronGsfTracks",      ""),
    maxDeltaRForGsfTrackMatching = cms.double (0.2), # if cutting on dRToMatchedGsfTrack, must set this to be greater than the cut threshold
)
copyConfiguration (collectionProducer.tracks, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.trigobjs = cms.EDProducer ("OSUTrigobjProducer",
)
copyConfiguration (collectionProducer.trigobjs, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.uservariables = cms.EDProducer ("UservariableProducer",
)

################################################################################
