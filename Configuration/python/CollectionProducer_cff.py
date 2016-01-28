import FWCore.ParameterSet.Config as cms

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
    rho = cms.InputTag  ("fixedGridRhoFastjetAll",    "",                ""),
    beamSpot = cms.InputTag  ("offlineBeamSpot",           "",                ""),
    gsfElectronCore = cms.InputTag ("reducedEgamma","reducedGedGsfElectronCores","")
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
)

#-------------------------------------------------------------------------------

collectionProducer.muons = cms.EDProducer ("OSUMuonProducer",
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
