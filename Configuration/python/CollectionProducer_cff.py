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

collectionProducer.electrons = cms.EDProducer ("OSUElectronProducer",
    conversions      =  cms.InputTag  ("reducedEgamma",                  "reducedConversions",          ""),
    rho              =  cms.InputTag  ("fixedGridRhoFastjetAll",         "",                            ""),
    beamSpot         =  cms.InputTag  ("offlineBeamSpot",                "",                            ""),
    vertices         =  cms.InputTag  ('offlineSlimmedPrimaryVertices',  ''),

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

collectionProducer.superclusters = cms.EDProducer ("OSUSuperclusterProducer",
)

#-------------------------------------------------------------------------------

collectionProducer.taus = cms.EDProducer ("OSUTauProducer",
)
copyConfiguration (collectionProducer.taus, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.tracks = cms.EDProducer ("OSUTrackProducer",
    fiducialMaps = cms.PSet (
        electrons = cms.PSet (
            histFile = cms.FileInPath ("OSUT3Analysis/Configuration/data/electronFiducialMap_data.root"),
            beforeVetoHistName = cms.string ("beforeVeto"), # must be eta on x-axis, phi on y-axis
            afterVetoHistName = cms.string ("afterVeto"), # must be eta on x-axis, phi on y-axis
            thresholdForVeto = cms.double (2.0), # in sigma
            outputHotSpots = cms.bool (False),
        ),
        muons = cms.PSet (
            histFile = cms.FileInPath ("OSUT3Analysis/Configuration/data/muonFiducialMap_data.root"),
            beforeVetoHistName = cms.string ("beforeVeto"), # must be eta on x-axis, phi on y-axis
            afterVetoHistName = cms.string ("afterVeto"), # must be eta on x-axis, phi on y-axis
            thresholdForVeto = cms.double (2.0), # in sigma
            outputHotSpots = cms.bool (False),
        ),
    ),
    minDeltaRForFiducialTrack = cms.double (0.05),
)
if osusub.batchMode and types[osusub.datasetLabel] == "data":
    collectionProducer.fiducialMaps.electrons.histFile = cms.FileInPath ("OSUT3Analysis/Configuration/data/electronFiducialMap_data.root")
    collectionProducer.fiducialMaps.muons.histFile = cms.FileInPath ("OSUT3Analysis/Configuration/data/muonFiducialMap_data.root")
copyConfiguration (collectionProducer.tracks, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.trigobjs = cms.EDProducer ("OSUTrigobjProducer",
)
copyConfiguration (collectionProducer.trigobjs, collectionProducer.genMatchables)

#-------------------------------------------------------------------------------

collectionProducer.uservariables = cms.EDProducer ("UservariableProducer",
)

################################################################################
