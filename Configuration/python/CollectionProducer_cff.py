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

collectionProducer.basicjets = cms.EDProducer ("BasicjetProducer",
)
copyConfiguration (collectionProducer.basicjets, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.beamspots = cms.EDProducer ("BeamspotProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.bjets = cms.EDProducer ("BjetProducer",
)
copyConfiguration (collectionProducer.bjets, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.bxlumis = cms.EDProducer ("BxlumiProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.candeles = cms.EDProducer ("CandeleProducer",
)
copyConfiguration (collectionProducer.candeles, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.candjets = cms.EDProducer ("CandjetProducer",
)
copyConfiguration (collectionProducer.candjets, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.electrons = cms.EDProducer ("ElectronProducer",
)
copyConfiguration (collectionProducer.electrons, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.events = cms.EDProducer ("EventProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.eventvariables = cms.EDProducer ("EventvariableProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.genjets = cms.EDProducer ("GenjetProducer",
)
copyConfiguration (collectionProducer.genjets, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.jets = cms.EDProducer ("JetProducer",
)
copyConfiguration (collectionProducer.jets, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.mcparticles = cms.EDProducer ("McparticleProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.mets = cms.EDProducer ("MetProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.muons = cms.EDProducer ("MuonProducer",
)
copyConfiguration (collectionProducer.muons, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.photons = cms.EDProducer ("PhotonProducer",
)
copyConfiguration (collectionProducer.photons, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.pileUpInfos = cms.EDProducer ("PileUpInfoProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.primaryvertexs = cms.EDProducer ("PrimaryvertexProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.superclusters = cms.EDProducer ("SuperclusterProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.taus = cms.EDProducer ("TauProducer",
)
copyConfiguration (collectionProducer.taus, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.tracks = cms.EDProducer ("TrackProducer",
)
copyConfiguration (collectionProducer.tracks, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.trigobjs = cms.EDProducer ("TrigobjProducer",
)
copyConfiguration (collectionProducer.trigobjs, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.uservariables = cms.EDProducer ("UservariableProducer",
)

################################################################################
