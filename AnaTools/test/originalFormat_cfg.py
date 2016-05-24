# Create a new file with collections in original format, using a skim file as input.
# To enable viewing skim files in Fireworks.

import FWCore.ParameterSet.Config as cms

process = cms.Process("ORIGFORMAT")
process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring (
                                 "file:preselection/skim.root",
                             ),
)

process.maxEvents = cms.untracked.PSet (
    input = cms.untracked.int32 (100)
)

process.originalFormat = cms.EDProducer("OriginalFormatProducer",
                                       makeBeamspots       =  cms.bool(False),  # Not yet implemented.
                                       makeElectrons       =  cms.bool(True),
                                       makeGenjets         =  cms.bool(True),
                                       makeJets            =  cms.bool(True),
                                       makeMcparticles     =  cms.bool(True),
                                       makeMets            =  cms.bool(True),
                                       makeMuons           =  cms.bool(True),
                                       makePhotons         =  cms.bool(True),
                                       makePileUpInfos     =  cms.bool(False),  # Not yet implemented.
                                       makePrimaryvertexs  =  cms.bool(True),
                                       makeTaus            =  cms.bool(True),
                                       makeTracks          =  cms.bool(True),
                                       makeTrigobjs        =  cms.bool(True),
                                       makeSuperclusters   =  cms.bool(True),
)

process.p = cms.Path(process.originalFormat)

process.Out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string ("origFormat.root")
)

process.end = cms.EndPath(process.Out)

